INPUT_DIR=../ada-final-public
OUTPUT_DIR=./Team09_public_output
mkdir ./Team09_public_output
make build SOURCE=../ada_challenge_2020/ada_challenge.cc
# some flags (the flag toggles the parameter)
INPUTNAME=--inputname
T=--T
TIMEOUT=--timeout
SORT_JOB=--sortjob
ADD_STRATEGY=--addstrategy
SAVE=--save
LOAD=--load
TOPO=--toposort
VERBOSE=--verbose
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/00.out" $T 3200 $TIMEOUT 2 $SORT_JOB $TOPO  < $INPUT_DIR/00.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/01.out" $T 3200 $TIMEOUT 2 < $INPUT_DIR/01.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/02.out" $T 3200 $TIMEOUT 2 $SORT_JOB $TOPO< $INPUT_DIR/02.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/03.out" $T 4800 $TIMEOUT 100 $SORT_JOB $TOPO < $INPUT_DIR/03.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/04.out" $T 96000 $TIMEOUT 1200 $SORT_JOB $TOPO  < $INPUT_DIR/04.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/05.out" $T 1200 $TIMEOUT 3000 $SORT_JOB $TOPO < $INPUT_DIR/05.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/06.out" $T 4800 $TIMEOUT 2000 $SORT_JOB $TOPO < $INPUT_DIR/06.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/07.out" $T 1200 $TIMEOUT 1800 $SORT_JOB $TOPO < $INPUT_DIR/07.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/08.out" $T 1200 $TIMEOUT 20000 $SORT_JOB $TOPO < $INPUT_DIR/08.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/09.out" $T 96000 $TIMEOUT 20 < $INTPUT_DIR/09.in
./bin/ada_challenge $INPUTNAME "$OUTPUT_DIR/10.out" $T 96000 $TIMEOUT 20 < $INTPUT_DIR/10.in
