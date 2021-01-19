INPUT_DIR=../ada-final-public
make build SOURCE=../ada/ada_challenge.cc
#some flags
INPUTNAME=--inputname
T=--T
TIMEOUT=--timeout
SORT_JOB=--sortjob
ADD_STRATEGY=--addstrategy
SAVE=--save
LOAD=--load
TOPO=--toposort
VERBOSE=--verbose
./bin/ada_challenge $INPUTNAME 04 $T 96000 < $INPUT_DIR/04.in
