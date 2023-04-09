#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PARENT_DIRECTORY="${SCRIPT_DIR%/*}"

RESULTS_DIR=$PARENT_DIRECTORY/results/
mkdir -p $RESULTS_DIR

SOURCE_DIR=$PARENT_DIRECTORY/c_src

FILE=$RESULTS_DIR"size_comparison.csv"
[ -f $FILE ] && rm $FILE

# Defaults 
max=1024
reps=1

# Parse command line arguments
while getopts "m:r:" opt; do
  case $opt in
    m)
      max="$OPTARG"
      ;;
    r)
      reps="$OPTARG"
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

echo "This script will build and run tiny_ising with different L values to measure time."
echo "L,time" >> $FILE
for ((i=32; i<=$max; i+=32))
do
    echo "Building and running Ising for L=$i"
    make -s -C $SOURCE_DIR clean
    make -s -C $SOURCE_DIR "SIM_PARAMS=-DL=$i -DMETRIC"
    for ((j=1; j<=$reps; j++))
    do
        $SOURCE_DIR/tiny_ising >> $FILE
    done
    
done
echo "Results stored in: $FILE"
cat $FILE