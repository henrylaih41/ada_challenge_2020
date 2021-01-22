#! /bin/bash
### place this script under or-tools folder
INPUT_DIR=../ada_challenge_2020/ada-private-cases/release # path to input files
OUTPUT_DIR=./Team09_private_output     # path to output folder
timeHor=9600 # time horizon
timeOut=1 # timeout time
### Might differ, change to source code path
make build SOURCE=../ada_challenge_2020/ada_challenge.cc
mkdir ./Team09_private_output
echo "**********Running Private TestCase*********"
for input in $(ls $INPUT_DIR); do
    filename=$(echo $input | cut -f 1 -d '.')
    ### the --inputname is actually --outputname
    ./bin/ada_challenge --inputname "$OUTPUT_DIR/$filename.out" --T $timeHor --timeout $timeOut --verbose < "$INPUT_DIR/$input"
    echo "$input Done"
done
