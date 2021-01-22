# Usage 

## Installation
 1. Install [ortool](https://github.com/google/or-tools) first, and follow the guide from [here](https://developers.google.com/optimization/install/cpp/source_linux) 

```
$ sudo apt-get -y install git wget pkg-config build-essential cmake autoconf libtool zlib1g-dev lsb-release  // for Ubuntu 20.04 LTS

$ git clone https://github.com/google/or-tools  //install stable branch of ortool

$ make third_party //build the required third party software

$ make cc //compile OR-Tools

$ make test_cc //check everything is running correcly
```

## Running

### Public Testcase

To run our code, place the folder `or-tool` in the parent folder of `ada_challenge_2020`. And then move the `run_public.sh` to the folder `or-tool`.

Remember to place the input data folder `ada-final-public` in the parent folder of our folder `ada_challenge_2020`
```
$ ./run_private.sh
```


### Private Testcase

To run our code, place the folder `or-tool` in the parent folder of `ada_challenge_2020`. And then move `run_private.sh` to the folder `or-tool`.

Remember to place the input data folder `ada-private-cases` in the parent folder of our folder `ada_challenge_2020`

```
$ ./run_private.sh
```

## Flags of `ada_challenge.cc`

```
    --inputname <your input file name>

    --T <number in sec>: determined the time horizon, by default 3200

    --timeout <number in sec>: determine the timeout, by default 3600

    --sortjob: turn on SORT_JOB

    --addstrategy: turn on ADD_STRATEGY

    --save: turn on SAVE

    --load: turn on LOAD

    --toposort: turn TOPO_SORT
    
    --verbose: turn on VERBOSE
```

## Testcase script

### Prerequisite
To run the script, place the following three files in the same folder.
1. ada_challenge.cc
2. testcase_geenrator.py
3. testcase_script.py
### Procedure
1. cd to the or-tools folder
2. build the source code: `make build SOURCE=<path to ada_challenge.cc>`
3. go back to the working folder (the folder where you put the above three files)
4. simply type: `python3 testcase_script.py <number of seconds to generate test case>`, e.g. `python3 testcase_script.py 24`
then the script will:

(1) generate testcase and its solution under the same folder

(2) create testcase_record to store the metric(score) and parameters of the testcase

(3) print its currect state to stdout as follow:

Note: you may need to change the path to the executable in line 67 `cmd = "../or-tools/bin/ada_challenge --inputname {} --T 9600 --timeout {}".format(str(count).zfill(2), timeout)`
```
2021-01-17 15:52:57.896717
Init metric: -151121.0

2021-01-17 15:53:27.636109
Get worse(better) metric: -719322.0

2021-01-17 15:53:43.295096
Get worse(better) metric: -1079460.0
```


## Reference
   
[Job Scheduling](https://developers.google.com/optimization/scheduling/job_shop)

[Constraint Programming](https://developers.google.com/optimization/reference/sat/cp_model/IntervalVar)

[Scheduling ortool manual](https://acrogenesis.com/or-tools/documentation/user_manual/manual/ls/scheduling_or_tools.html)
