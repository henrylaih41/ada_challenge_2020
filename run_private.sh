INPUT_DIR=../testcase
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
./bin/ada_challenge $INPUTNAME 00 $T 96000 < $INPUT_DIR/00.in
./bin/ada_challenge $INPUTNAME 01 $T 96000 < $INPUT_DIR/01.in
./bin/ada_challenge $INPUTNAME 02 $T 96000 < $INPUT_DIR/02.in 
./bin/ada_challenge $INPUTNAME 03 $T 96000 < $INPUT_DIR/03.in
./bin/ada_challenge $INPUTNAME 04 $T 96000 < $INPUT_DIR/04.in
./bin/ada_challenge $INPUTNAME 05 $T 96000 < $INPUT_DIR/05.in
./bin/ada_challenge $INPUTNAME 06 $T 96000 < $INPUT_DIR/06.in
./bin/ada_challenge $INPUTNAME 07 $T 96000 < $INPUT_DIR/07.in
./bin/ada_challenge $INPUTNAME 08 $T 96000 < $INPUT_DIR/08.in
