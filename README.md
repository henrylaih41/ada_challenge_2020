# Usage 
 1. Install [ortool](https://github.com/google/or-tools) first, and follow the guide from [here](https://developers.google.com/optimization/install/cpp/source_linux) 
 
 1. To run code type the following command in root folder of ortool: 

    `sudo make run SOURCE=file_path`

    ex: `sudo make run SOURCE=./ada/ada_challenge.cc << ./ada/inputdata.txt`

    The script `run_ada.sh` should placed under the root folder of ortool

 1. Flags of `ada_challenge.cc`

    `--inputname <your input file name>`

    `--T <number in sec>`: determined the time horizon, by default 3200

    `--timeout <number in sec>`: determine the timeout, by default 3600

    `--sortjob`: turn on SORT_JOB

    `--addstrategy`: turn on ADD_STRATEGY

    `--save`: turn on SAVE

    `--load`: turn on LOAD

    `--toposort`: turn TOPO_SORT
    
    `--verbose`: turn on VERBOSE

 1. Reference:

    [Job Scheduling](https://developers.google.com/optimization/scheduling/job_shop)

    [Constraint Programming](https://developers.google.com/optimization/reference/sat/cp_model/IntervalVar)

    [Scheduling ortool manual](https://acrogenesis.com/or-tools/documentation/user_manual/manual/ls/scheduling_or_tools.html)
