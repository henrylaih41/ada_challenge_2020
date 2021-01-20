#! /bin/bash
### place this script under or-tools folder
INPUT_DIR=../ada-private-cases/release # path to input files
OUTPUT_DIR=./Hint                      # path to hint folder
timeHor=9600 # time horizon
timeOut=10   # timeout time
### Might differ, change to source code path
make build SOURCE=../ada_challenge_2020/ada_challenge.cc
mkdir ./Hint
for input in $(ls $INPUT_DIR); do
    filename=$(echo $input | cut -f 1 -d '.')
    ### the --inputname is actually --outputname
    ./bin/ada_challenge --inputname "$OUTPUT_DIR/$filename.hint" --T $timeHor --timeout $timeOut < "$INPUT_DIR/$input"
    echo "$input Done"
done

