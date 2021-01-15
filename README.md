# Usage 
1. install [ortool](https://github.com/google/or-tools) first, and follow the guide from [here](https://developers.google.com/optimization/install/cpp/source_linux) 
2. To run code type the following command in root folder of ortool: 

    `sudo make run SOURCE=file_path`

    ex: `sudo make run SOURCE=./ada/ada_challenge.cc << ./ada/inputdata.txt`

3. The script `run_ada.sh` should placed under the root folder of ortool
    Then simply type `./run_ada.sh`

4. Reference:

    [Job Scheduling](https://developers.google.com/optimization/scheduling/job_shop)

    [Constraint Programming](https://developers.google.com/optimization/reference/sat/cp_model/IntervalVar)

    [Scheduling ortool manual](https://acrogenesis.com/or-tools/documentation/user_manual/manual/ls/scheduling_or_tools.html)
