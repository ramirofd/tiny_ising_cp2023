#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PARENT_DIRECTORY="${SCRIPT_DIR%/*}"

RESULTS_DIR=$PARENT_DIRECTORY/results/
mkdir -p $RESULTS_DIR

SOURCE_DIR=$PARENT_DIRECTORY/c_src

FILE=$RESULTS_DIR"size_comparison.csv"
[ -f $FILE ] && rm $FILE

l_values=(32 64 128 256 512 1024)
echo "This script will build and run tiny_ising with different L values to measure time."
echo "L,time" >> $FILE
for i in ${l_values[@]}
do
    echo "Building and running Ising for L=$i"
    make -s -C $SOURCE_DIR clean
    make -s -C $SOURCE_DIR "SIM_PARAMS=-DL=$i -DTIME"
    $SOURCE_DIR/tiny_ising >> $FILE
    
done
echo "Results stored in: $FILE"
cat $FILE