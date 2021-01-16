#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include "ortools/sat/cp_model.h"
#include <string>
#include <assert.h>
#define print LOG(INFO)
#define SORT_JOB 1
#define ADD_STRATEGY 0
#define SAVE 1
#define LOAD 1
#define TOPO_SORT 1
using namespace std;
using namespace operations_research;
using namespace sat;
int T = 3200;   /*	by default set 3200	*/
int TIMEOUT = 3600; // default 1hr
class operation;
class Job;
CpModelBuilder cp_model;
const int64 WScale = 100000;
const string savePath = "08.save";
const string loadPath = "08.save";
char* outfile;
map<int64, int64> slice_map;
map<int64, int64> global_to_interval_index;
int64 total_count = 0;
// set precedence constraint

class operation{
    public:
        int64 slice_demand, duration, idx, global_id;
        vector<int64> dependencies;
        operation(int64 s, int64 d, int64 idxx, int64 count){
            slice_demand = s;
            duration = d;
            idx = idxx;
            global_id = count;        
        }
    friend ostream& operator<<(ostream& o, const operation &op){
        o << op.idx << " s: " << op.slice_demand << " d: " << op.duration << " Dependencies: ";
        for(auto d : op.dependencies) o << d << ' ';
        return o;
    }
    int64 index(){
        return idx;
    }  
};

class Job{
    public:
        vector<operation> ops;
        int64 index, total_duration, weight;
    friend ostream& operator<<(ostream& o, const Job &j){
        o << "Job: " << j.index << endl; 
        for(auto ops : j.ops) o << ops << endl;
        return o;
    }
    Job(int64 idx, int64 w){
        index = idx;
        weight = w;
    }
    double getWD(){
        return (double)weight / (double)total_duration;
    }
};

void dfs(int s, vector<operation> &adj, int* visited, vector<operation> &res){
    visited[s] = 1;
    for(auto i : adj[s].dependencies){ // i starts from 1
        if(!visited[i-1]){
            dfs(i-1, adj, visited, res);
        }
    }
    res.push_back(adj[s]);
}

void topologySort(vector<operation> &ops, vector<operation> &job_ops){
    int* visited = (int*)malloc(sizeof(int) * ops.size());
    memset(visited, 0, sizeof(int) * ops.size());
    vector<operation> res;
    
    for(int i = 0; i < ops.size(); ++i)
        if(!visited[i]) dfs(i, ops, visited, res);
    for(int i = 0; i < res.size(); ++i) job_ops.push_back(res[i]);
}

void constructJobs(vector<Job> &allJobs, int64 index){
    int64 m, s, d, p, a, total_d = 0;
    double w;
    vector<operation> ops_to_sort;
    cin >> m >> w;
    Job job(index, w * WScale); // weight scale by WScale
    for(int j = 0; j < m; ++j){
        cin >> s >> d >> p;
        total_d += d;
        operation op(s, d, j+1, total_count++);
        for(int k = 0; k < p; ++k){
            cin >> a; op.dependencies.push_back(a);
        }
        ops_to_sort.push_back(op);
    }
    job.total_duration = total_d;
    if(TOPO_SORT) topologySort(ops_to_sort, job.ops);
    else job.ops = ops_to_sort;
   
    allJobs.push_back(job);
}

inline void setDependencies(const IntervalVar &precedence, const IntervalVar& interval){
    cp_model.AddLessOrEqual(precedence.EndVar(), interval.StartVar());
}

// set makespan
inline void setMakeSpan(const IntervalVar &op, IntVar makespan){
    cp_model.AddLessOrEqual(op.EndVar(), makespan);
}

vector<IntervalVar> constructInterval(Job &current_job, IntVar &makespan, CumulativeConstraint &cm_rule, vector<IntVar> &task_starts, const Domain &time_horizon){
    static int64 count = 0;
    ++count;
    vector<IntervalVar> intervals; 
    vector<IntervalVar> interval_map(current_job.ops.size() + 1); // maps operations index to corresponding intervalVar
    print << "constructing Job: " << count; 
    
    for(auto op : current_job.ops){
        
        const IntVar start    = cp_model.NewIntVar(time_horizon);
        const IntVar duration = cp_model.NewConstant(op.duration);
        const IntVar end      = cp_model.NewIntVar(time_horizon);
        IntervalVar interval = cp_model.NewIntervalVar(start, duration, end);
        global_to_interval_index[op.global_id] = interval.index();
        intervals.push_back(interval);
        task_starts.push_back(start);
        // add slice demand
        const IntVar demand = cp_model.NewConstant(op.slice_demand); 
        cm_rule.AddDemand(interval, demand);
        slice_map[interval.index()] = op.slice_demand;
        interval_map[op.index()] = interval;
    }

    // setting dependencies 
    for(auto op : current_job.ops)
        for(auto j : op.dependencies)
            setDependencies(interval_map[j], interval_map[op.index()]); 

    // setting Ci: Finishing time of Job i
    for (auto iv : intervals){
        setMakeSpan(iv, makespan);
    }
    
    return intervals;
}

int64 calculate_gcd_and_divide(vector<Job> &allJobs){
    int64 GCD = 0;
    vector<int64> durations;
    
    for(auto &job : allJobs)
       for(auto &op : job.ops)
            durations.push_back(op.duration);
    
    for(auto d : durations){
        GCD = std::gcd(GCD, d); 
    }
    
    for(auto &job : allJobs)
        for(auto &op : job.ops)
            op.duration = op.duration / GCD;
            
    print << "GCD of durations is: " << GCD;
    return GCD;
}

void sort_by_wd(vector<Job>& target){
    sort(target.begin(), target.end(),
           [&](Job a, Job b) -> bool {
                return (a.getWD() > b.getWD()); 
           });
}

// get the availabe slice (assign slice id to s) 
int searchSlices(vector<int> &slices, IntervalVar &op, CpSolverResponse &response, int &s){
    int start = SolutionIntegerValue(response, op.StartVar());
    int end   = SolutionIntegerValue(response, op.EndVar());
    for(int i = 0; i < slices.size(); ++i){
        if(start >= slices[i]){
            s = i + 1; // slice num start from 1
            slices[i] = end;
            return 1;
        }
    }
    
    // If no available slice
    return 0;
}

// compare interval by their start time
class interval_cmp{
    CpSolverResponse response; 
    public:
        interval_cmp(CpSolverResponse &r){
            response = r;
        }
        bool operator()(IntervalVar a, IntervalVar b){
            return (SolutionIntegerValue(response, a.StartVar()) < SolutionIntegerValue(response, b.StartVar())); 
        }
};

void saveCheckPoint(CpSolverResponse response, vector<vector<IntervalVar> > &all_interval){
    ofstream output_file(savePath);
    for(auto j : all_interval)
        for(auto iv : j){
            output_file << SolutionIntegerValue(response, iv.StartVar()) << endl;
        }
    output_file.close();
}

void loadCheckPoint(vector<vector<IntervalVar> > &all_interval){
    print << "Loading from file: " + loadPath;
    ifstream input_file(loadPath);
    string line;
    int value;
    for(auto j : all_interval)
        for(auto iv : j){
            getline(input_file, line);
            value = std::stoi(line);
            cp_model.AddHint(iv.StartVar(), value);
        }
}

void createOutput(vector<vector<IntervalVar> > &allJobs, CpSolverResponse response, int slice_num, int64 gcd_of_durations){
    ofstream output_file;
    output_file.open(outfile);
    vector<int> slices(slice_num, 0); 
    vector<IntervalVar> all_interval; // a interval represents an operation 
    map<int64, int64> time_ans;
    map<int64, string> slice_ans;
    int s; // to store the slice id

    for(auto &job : allJobs)
        for(auto &iv : job)
            all_interval.push_back(iv);        

    // sort intervals by start time (else the output would be invalid)        
    sort(all_interval.begin(), all_interval.end(), interval_cmp(response));

    // getting the slice assignment 
    for(auto iv : all_interval){
        string slice_id = "";
        // a operation might need more than one slice
        for(int i = 0; i < slice_map[iv.index()]; ++i){
            if(!searchSlices(slices, iv, response, s)){
                print << "Error invalid solution"; 
                return;
            }
        slice_id += ' '; slice_id += to_string(s); 
        }

        slice_ans[iv.index()] = slice_id;
    }
    
    // getting the starting time from response(the solution)
    for(auto &jobs : allJobs)
        for(auto &iv : jobs){
            int t = SolutionIntegerValue(response, iv.StartVar());
            time_ans[iv.index()] = t;
        }

    // write to output file
    for(int i = 0; i < total_count; ++i){
        int64 iv_index = global_to_interval_index[i];
        output_file << time_ans[iv_index] * gcd_of_durations << slice_ans[iv_index] << '\n';
    }

    output_file.close();
}

int main(int argc, char *argv[]){
    int64 slice_num, job_num, gcd_of_durations;
    double w;
    vector<vector<IntervalVar> > allJobIntervals;
    vector<Job> allJobs;
    vector<IntVar> makespans;
    vector<IntVar> task_starts;

    /*	handle the correspong parameters for different input	*/
    outfile = (char*)malloc(1024*sizeof(char));
    if (argc == 3){
        sprintf(outfile, "%s.out", argv[1]);
        T = atoi(argv[2]);
    }
    printf("parameters: %s and %d\n", outfile, T);
    print << "Running: " << outfile << ".in";
    Domain time_horizon(0,T);
    // Reading Input and storing as Job object 
    cin >> slice_num >> job_num;
    cout << "Constructing\n";
    for(int i = 1; i <= job_num; ++i){
        constructJobs(allJobs, i);
        cout << allJobs[i-1] << endl;
    }
    // sort by w/d
    if(SORT_JOB) sort_by_wd(allJobs);

    // calcalate GCD of all duration and dividing the durations by GCD
    gcd_of_durations = calculate_gcd_and_divide(allJobs);
    
    // Add slice constraint
    // cm_rule is a object with AddDemand(IntervalVar s, IntVar d) method
    // the added invterval s takes d slices, the constraint will ensure 
    // the slice concurrently used by the added intervals won't exceed max_slice
    const IntVar max_slice = cp_model.NewConstant(slice_num);
    CumulativeConstraint cm_rule = cp_model.AddCumulative(max_slice); 
    print << "MaxSlice: " << max_slice; 
    
    // Init makespans Ci for every job
    for(int i = 0; i < job_num; ++i){
        makespans.push_back(cp_model.NewIntVar(time_horizon));
    }

    // Construct each operations in Job into intervals. 
    for(int i = 0; i < job_num; ++i)
        allJobIntervals.push_back(constructInterval(allJobs[i], makespans[i], cm_rule, task_starts, time_horizon));

    // Objective 
    const IntVar total_makespan = cp_model.NewIntVar(time_horizon);
    
    // the Finished time T satisfy Ci <= T for all i = 1 ~ job_num 
    for(auto &m : makespans)
        cp_model.AddLessOrEqual(m, total_makespan);
    makespans.push_back(total_makespan);
    vector<int64> weights; 
    for(auto j : allJobs) weights.push_back(j.weight);
    weights.push_back(WScale); // 1 * WScale = WScale 
    cp_model.Minimize(LinearExpr::ScalProd(makespans, weights)); // summation over the makespans (including the Finishing time T) with coefficent wi 
    
    // Decision strategy.
    //auto rd = std::random_device {}; auto rng = std::default_random_engine { rd() }; shuffle(task_starts.begin(), task_starts.end(), rng);
    if(ADD_STRATEGY) 
    cp_model.AddDecisionStrategy(task_starts,
                              DecisionStrategyProto::CHOOSE_FIRST, // CHOOSE_FIRST CHOOSE_LOWEST_MIN CHOOSE_HIGHEST_MAX CHOOSE_MIN_DOMAIN_SIZE CHOOSE_MAX_DOMAIN_SIZE
                              DecisionStrategyProto::SELECT_LOWER_HALF); // SELECT_MIN_VALUE  SELECT_MAX_VALUE SELECT_LOWER_HALF SELECT_UPPER_HALF SELECT_MEDIAN_VALUE
    //cp_model.AddDecisionStrategy(task_starts, DecisionStrategyProto::CHOOSE_LOWEST_MIN, DecisionStrategyProto::SELECT_LOWER_HALF);
    
    // Solving Part
    Model model;
    SatParameters parameters;

    // Adding time limit
    parameters.set_max_time_in_seconds(TIMEOUT);
    model.Add(NewSatParameters(parameters));
    auto start_time = std::chrono::steady_clock::now(); 

    // SolutionCallbacks (printing time and objective value)
    model.Add(NewFeasibleSolutionObserver([&](const CpSolverResponse& response){
        using namespace std::chrono;
        auto end_time = steady_clock::now(); 
        print << "Objective Value: " << -1 * (double)gcd_of_durations * response.objective_value() / (double)WScale << ' ' << duration_cast<seconds>(end_time - start_time).count() << "sec\n"; 
    }));
    
    // load init
    if(LOAD) loadCheckPoint(allJobIntervals);

    // Solve the model
    const CpSolverResponse response = SolveCpModel(cp_model.Build(), &model);

    // Printing Info and Creating Output
    print << CpSolverResponseStats(response); 
    createOutput(allJobIntervals, response, slice_num, gcd_of_durations);

    if(SAVE) saveCheckPoint(response, allJobIntervals);
    createOutput(allJobIntervals, response, slice_num, gcd_of_durations); // this sorts the allJobIntervals
    // Print the solution
    for(auto job : allJobIntervals)
        for(auto iv : job)
        print << SolutionIntegerValue(response, iv.EndVar()) << ' ' << slice_map[iv.index()];
    print << SolutionIntegerValue(response, total_makespan);
}
