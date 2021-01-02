#include <iostream>
#include <fstream>
#include "ortools/sat/cp_model.h"
#include <string>
#define print LOG(INFO)
using namespace std;
using namespace operations_research;
using namespace sat;
class operation;
class Job;
CpModelBuilder cp_model;
const Domain time_horizon(0, 4800);
const int64 WScale = 100000;
const string file_name = "06.out";
map<int, int> slice_map;
map<int, string> ans;
// set precedence constraint

class Job{
    public:
        vector<operation> ops;
};

class operation{
    public:
        int64 slice_demand;
        int64 duration;
        vector<int64> dependencies;
        operation(int64 s, int64 d){
            slice_demand = s;
            duration = d;
        }
};

void constructJobs(vector<Job> &allJobs, vector<int64> &weight, int64 num_of_jobs){
    int64 m, s, d, p, a;
    double w;
    for(int i = 0; i < num_of_jobs; ++i){
        cin >> m >> w;
        Job job;
        for(int j = 0; j < m; ++j){
            cin >> s >> d >> p;
            operation op(s, d);
            for(int k = 0; k < p; ++k){
                cin >> a; op.dependencies.push_back(a);
            }
            job.ops.push_back(op);
        }
        allJobs.push_back(job);
        weight.push_back(w * WScale);
    }
}

inline void setDependencies(const IntervalVar &precedence, const IntervalVar& interval){
    cp_model.AddLessOrEqual(precedence.EndVar(), interval.StartVar());
}

// set makespan
inline void setMakeSpan(const IntervalVar &op, IntVar makespan){
    cp_model.AddLessOrEqual(op.EndVar(), makespan);
}

vector<IntervalVar> constructInterval(Job &current_job, IntVar &makespan, CumulativeConstraint &cm_rule, vector<IntVar> &task_starts){
    static int count = 0;
    ++count;
    vector<IntervalVar> operations; 
    print << "constructing Job: " << count; 
   
    
    for(auto op : current_job.ops){
        
        const IntVar start    = cp_model.NewIntVar(time_horizon);
        const IntVar duration = cp_model.NewConstant(op.duration);
        const IntVar end      = cp_model.NewIntVar(time_horizon);
        IntervalVar operation = cp_model.NewIntervalVar(start, duration, end);
        operations.push_back(operation);
        task_starts.push_back(start);
        // add slice demand
        const IntVar demand = cp_model.NewConstant(op.slice_demand); 
        cm_rule.AddDemand(operation, demand);
        slice_map[operation.index()] = op.slice_demand;
    }

    // setting dependencies 
    for(int i = 0; i < current_job.ops.size(); ++i)
        for(auto j : current_job.ops[i].dependencies) // index of j start from 1
            setDependencies(operations[j-1], operations[i]);       

    // setting Ci: Finishing time of Job i
    for (auto op : operations){
        setMakeSpan(op, makespan);
    }
    
    return operations;
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
    
    // If error
    for(auto i : slices) cout << i << ' ';
    cout << start << endl;
    return 0;
}
class op_cmp{
    CpSolverResponse response; 
    public:
        op_cmp(CpSolverResponse &r){
            response = r;
        }
        bool operator()(IntervalVar a, IntervalVar b){
            return (SolutionIntegerValue(response, a.StartVar()) < SolutionIntegerValue(response, b.StartVar())); 
        }
};

void createOutput(vector<vector<IntervalVar> > &allJobs, CpSolverResponse response, int slice_num, int64 gcd_of_durations){
    ofstream output_file;
    output_file.open(file_name);
    vector<int> slices(slice_num, 0); 
    vector<IntervalVar> all_op;  
    int s;
    for(auto &job : allJobs){
        for(auto &op : job){
            all_op.push_back(op);        
        }
    }
    sort(all_op.begin(), all_op.end(), op_cmp(response));
    for(auto op : all_op){
        string slice_id = "";
        for(int i = 0; i < slice_map[op.index()]; ++i){
            if(!searchSlices(slices, op, response, s)){
                print << "Error invalid solution"; 
                return;
            }
        slice_id += ' '; slice_id += to_string(s); 
        }
        ans[op.index()] = slice_id;
    }
    
    for(auto &jobs : allJobs)
        for(auto &op : jobs){
            int t = SolutionIntegerValue(response, op.StartVar());
            output_file << t * gcd_of_durations << ans[op.index()] << '\n';
        }
    output_file.close();
}

int main(){
    int64 slice_num, job_num, gcd_of_durations;
    double w;
    vector<int64> weights; // Scaled
    vector<vector<IntervalVar> > allJobIntervals;
    vector<Job> allJobs;
    vector<IntVar> makespans;
    vector<IntVar> task_starts;

    // Reading Input and storing as Job object 
    cin >> slice_num >> job_num;
    constructJobs(allJobs, weights, job_num);

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
        allJobIntervals.push_back(constructInterval(allJobs[i], makespans[i], cm_rule, task_starts));

    // Objective 
    const IntVar total_makespan = cp_model.NewIntVar(time_horizon);
    
    // the Finished time T satisfy Ci <= T for all i = 1 ~ job_num 
    for(auto &m : makespans)
        cp_model.AddLessOrEqual(m, total_makespan);
    makespans.push_back(total_makespan);
    weights.push_back(WScale); // 1 * WScale = WScale 
    cp_model.Minimize(LinearExpr::ScalProd(makespans, weights)); // summation over the makespans (including the Finishing time T) with coefficent wi 
    
    // Decision strategy.
    cp_model.AddDecisionStrategy(task_starts,
                               DecisionStrategyProto::CHOOSE_LOWEST_MIN,
                               DecisionStrategyProto::SELECT_MIN_VALUE);
    
    // Solving
    Model model;
    SatParameters parameters;

    // Adding time limit
    parameters.set_max_time_in_seconds(4800.0);
    model.Add(NewSatParameters(parameters));

    // SoluionCallbacks (printing time and objective value)
    model.Add(NewFeasibleSolutionObserver([&](const CpSolverResponse& response){
        using namespace std::chrono;
        std::time_t end_time = system_clock::to_time_t(system_clock::now());
        print << "Objective Value: " << -1 * (double)gcd_of_durations * response.objective_value() / (double)WScale << ' ' << std::ctime(&end_time); 

    }));

    const CpSolverResponse response = SolveCpModel(cp_model.Build(), &model);
    print << CpSolverResponseStats(response);
    
    createOutput(allJobIntervals, response, slice_num, gcd_of_durations);
    for(auto job : allJobIntervals)
        for(auto op : job)
        print << SolutionIntegerValue(response, op.EndVar()) << ' ' << slice_map[op.index()];
    print << SolutionIntegerValue(response, total_makespan);
}
