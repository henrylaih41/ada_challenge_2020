#! /bin/bash
# should be placed under or-tool 
INPUT_DIR=../ada-private-cases/release
make build SOURCE=../ada/ada_challenge.cc
#some flags
#INPUTNAME=--inputname
T=--T
TIMEOUT=--timeout
SORT_JOB=--sortjob
ADD_STRATEGY=--addstrategy
SAVE=--save
LOAD=--load
TOPO=--toposort
VERBOSE=--verbose

cnt=0
t=96000
timeout=78*3600/78 #can be set to $1
for input in `ls $INPUT_DIR`; do
    #echo $input
    echo $cnt
    ./bin/ada_challenge $INPUTNAME $cnt $T $t $TIMEOUT $timeout $VERBOSE < "$INPUT_DIR/$input" > "$cnt.out"
    cnt=$((cnt+1))
done


#./bin/ada_challenge $INPUTNAME 00 $T 96000 < $INPUT_DIR/00.in
#./bin/ada_challenge $INPUTNAME 01 $T 96000 < $INPUT_DIR/01.in
#./bin/ada_challenge $INPUTNAME 02 $T 96000 < $INPUT_DIR/02.in 
#./bin/ada_challenge $INPUTNAME 03 $T 96000 < $INPUT_DIR/03.in
#./bin/ada_challenge $INPUTNAME 04 $T 96000 < $INPUT_DIR/04.in
#./bin/ada_challenge $INPUTNAME 05 $T 96000 < $INPUT_DIR/05.in
#./bin/ada_challenge $INPUTNAME 06 $T 96000 < $INPUT_DIR/06.in
#./bin/ada_challenge $INPUTNAME 07 $T 96000 < $INPUT_DIR/07.in
#./bin/ada_challenge $INPUTNAME 08 $T 96000 < $INPUT_DIR/08.in
