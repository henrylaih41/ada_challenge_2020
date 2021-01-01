#include "ortools/sat/cp_model.h"
#define print LOG(INFO)
using namespace std;
using namespace operations_research;
using namespace sat;
CpModelBuilder cp_model;
const int64 max_jobs     = 30;
const Domain time_horizon(0, 9600);

void setDependencies(IntervalVar *precedence, IntervalVar* interval){
    
}

void constructJob(int index){
    int64 m, s, d, p, dependency; double w;
    vector<InterVals*> operations; 
    cin >> m >> w;
    for(int i = 0; i < m; ++i){
        cin >> s >> d >> p; 
        vector<int> dependencies; 
        for(int j = 0; j < p; ++j){
            cin >> dependency;
            dependencies.push_back(dependency);
        }
        const IntVar start    = cp_model.NewIntVar(time_horizon);
        const IntVar duration = cp_model.NewConstant(d);
        const IntVar end      = cp_model.NewIntVar(time_horizon);
        const IntervalVar operation = cp_model.NewIntervalVar(start, duration, end);
        operations.push_back(&operation);
        for(auto i : dependencies){
            setDependecies(operations[i], &operation);
        }
        print << operation.StartVar() << endl;
    }
}

int main(){
    int64 slice_num, job_num;
    cin >> slice_num >> job_num;
    for(int i = 0; i < job_num; ++i)
        constructJob(i);
    print << "Testing" << endl; 
}
/*
namespace operations_research {
namespace sat {
void NoOverlapSampleSat() {
  CpModelBuilder cp_model;
  const int64 kHorizon = 21;  // 3 weeks.

  const Domain horizon(0, kHorizon);
  // Task 0, duration 2.
  const IntVar start_0 = cp_model.NewIntVar(horizon);
  const IntVar duration_0 = cp_model.NewConstant(2);
  const IntVar end_0 = cp_model.NewIntVar(horizon);
  const IntervalVar task_0 =
      cp_model.NewIntervalVar(start_0, duration_0, end_0);

  // Task 1, duration 4.
  const IntVar start_1 = cp_model.NewIntVar(horizon);
  const IntVar duration_1 = cp_model.NewConstant(4);
  const IntVar end_1 = cp_model.NewIntVar(horizon);
  const IntervalVar task_1 =
      cp_model.NewIntervalVar(start_1, duration_1, end_1);

  // Task 2, duration 3.
  const IntVar start_2 = cp_model.NewIntVar(horizon);
  const IntVar duration_2 = cp_model.NewConstant(3);
  const IntVar end_2 = cp_model.NewIntVar(horizon);
  const IntervalVar task_2 =
      cp_model.NewIntervalVar(start_2, duration_2, end_2);

  // Week ends.
  const IntervalVar weekend_0 =
      cp_model.NewIntervalVar(cp_model.NewConstant(5), cp_model.NewConstant(2),
                              cp_model.NewConstant(7));
  const IntervalVar weekend_1 =
      cp_model.NewIntervalVar(cp_model.NewConstant(12), cp_model.NewConstant(2),
                              cp_model.NewConstant(14));
  const IntervalVar weekend_2 =
      cp_model.NewIntervalVar(cp_model.NewConstant(19), cp_model.NewConstant(2),
                              cp_model.NewConstant(21));

  // No Overlap constraint.
  cp_model.AddNoOverlap(
      {task_0, task_1, task_2, weekend_0, weekend_1, weekend_2});

  // Makespan.
  IntVar makespan = cp_model.NewIntVar(horizon);
  cp_model.AddLessOrEqual(end_0, makespan);
  cp_model.AddLessOrEqual(end_1, makespan);
  cp_model.AddLessOrEqual(end_2, makespan);

  cp_model.Minimize(makespan);

  // Solving part.
  Model model;
  const CpSolverResponse response = SolveCpModel(cp_model.Build(), &model);
  LOG(INFO) << CpSolverResponseStats(response);

  if (response.status() == CpSolverStatus::OPTIMAL) {
    LOG(INFO) << "Optimal Schedule Length: " << response.objective_value();
    LOG(INFO) << "Task 0 starts at " << SolutionIntegerValue(response, start_0);
    LOG(INFO) << "Task 1 starts at " << SolutionIntegerValue(response, start_1);
    LOG(INFO) << "Task 2 starts at " << SolutionIntegerValue(response, start_2);
  }
}

}  // namespace sat
}  // namespace operations_research

int main() {
  operations_research::sat::NoOverlapSampleSat();

  return EXIT_SUCCESS;
}*/
