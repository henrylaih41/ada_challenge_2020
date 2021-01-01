// TODOs: early stopping (now only stops when reach optimal) 
// TODOs: retrieve output from response (need to write a function to convert the solution into correct output formats) 
#include <iostream>
#include "ortools/sat/cp_model.h"
#include <typeinfo>
#define print LOG(INFO)
using namespace std;
using namespace operations_research;
using namespace sat;
CpModelBuilder cp_model;
const int64 max_jobs = 30;
const Domain time_horizon(0, 9600);
const int64 WScale = 100000; 
// set precedence constraint
inline void setDependencies(const IntervalVar &precedence, const IntervalVar& interval){
    cp_model.AddLessOrEqual(precedence.EndVar(), interval.StartVar());
}

// set makespan
inline void setMakeSpan(const IntervalVar &op, IntVar makespan){
    cp_model.AddLessOrEqual(op.EndVar(), makespan);
}

vector<IntervalVar> constructJob(IntVar &makespan, vector<int64> &weights, CumulativeConstraint &cm_rule){
    static int count = 0;
    ++count;
    int64 m, s, d, p, dependency; double w;
    vector<IntervalVar> operations; 
    vector<vector<int> > dependencies; 
    cin >> m >> w;
    weights.push_back(w * WScale); // Scale w to int64
    print << "constructing Job: " << count; 
    
    for(int i = 0; i < m; ++i){
        cin >> s >> d >> p; 
        vector<int> list_of_depend;
        for(int j = 0; j < p; ++j){
            cin >> dependency;
            list_of_depend.push_back(dependency);
        }
        dependencies.push_back(list_of_depend);

        const IntVar start    = cp_model.NewIntVar(time_horizon);
        const IntVar duration = cp_model.NewConstant(d);
        const IntVar end      = cp_model.NewIntVar(time_horizon);
        IntervalVar operation = cp_model.NewIntervalVar(start, duration, end);
        operations.push_back(operation);
        // add slice demand
        const IntVar demand = cp_model.NewConstant(s); 
        cm_rule.AddDemand(operation, demand);
    }

    // setting dependencies 
    for(int i = 0; i < dependencies.size(); ++i)
        for(auto j : dependencies[i]) // index of j start from 1
            setDependencies(operations[j-1], operations[i]);       

    // setting Ci: Finishing time of Job i
    for (auto op : operations){
        setMakeSpan(op, makespan);
    }
    
    return operations;
}

int main(){
    int64 slice_num, job_num;
    vector<int64> weights; // Scaled
    vector<vector<IntervalVar> > allJobs;
    vector<IntVar> makespans;
    cin >> slice_num >> job_num;
    
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

    // Construction
    for(int i = 0; i < job_num; ++i)
        allJobs.push_back(constructJob(makespans[i], weights, cm_rule));

    // Objective 
    const IntVar total_makespan = cp_model.NewIntVar(time_horizon);
    // the Finished time T satisfy Ci <= T for all i = 1 ~ job_num 
    for(auto &m : makespans)
        cp_model.AddLessOrEqual(m, total_makespan);
    makespans.push_back(total_makespan);
    weights.push_back(WScale); // 1 * WScale = WScale 
    cp_model.Minimize(LinearExpr::ScalProd(makespans, weights)); // summation over the makespans (including the Finishing time T) with coefficent wi 
    
    // Solving
    Model model;
    const CpSolverResponse response = SolveCpModel(cp_model.Build(), &model);
    print << CpSolverResponseStats(response);
    for(auto job : allJobs)
        for(auto op : job)
        print << SolutionIntegerValue(response, op.EndVar()); 
    print << SolutionIntegerValue(response, total_makespan);
}
