#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PARENT_DIRECTORY="${SCRIPT_DIR%/*}"

RESULTS_DIR=$PARENT_DIRECTORY/results/
mkdir -p $RESULTS_DIR

SOURCE_DIR=$PARENT_DIRECTORY/c_src

FILE=$RESULTS_DIR"loop_march.csv"
[ -f $FILE ] && rm $FILE


echo "This script will build and run tiny_ising with different optimization flags."
echo "compiler, flags, L, metric" >> $FILE

compiler="clang"
flags="-O3";
make -s -C $SOURCE_DIR clean; make -s -C $SOURCE_DIR CC=$compiler OPTIM_FLAGS="$flags"; echo "$compiler, $flags, "$($SOURCE_DIR/tiny_ising) >> $FILE

flags="-O3 -march=native"
make -s -C $SOURCE_DIR clean; make -s -C $SOURCE_DIR CC=$compiler OPTIM_FLAGS="$flags"; echo "$compiler, $flags, "$($SOURCE_DIR/tiny_ising) >> $FILE

# flags="-O3 -march=native --floop-nest-optimize"
# make -s -C $SOURCE_DIR clean; make -s -C $SOURCE_DIR CC=$compiler OPTIM_FLAGS="$flags"; echo "$compiler, $flags, "$($SOURCE_DIR/tiny_ising) >> $FILE

compiler="gcc"
flags="-O3";
make -s -C $SOURCE_DIR clean; make -s -C $SOURCE_DIR CC=$compiler OPTIM_FLAGS="$flags"; echo "$compiler, $flags, "$($SOURCE_DIR/tiny_ising) >> $FILE

flags="-O3 -march=native"
make -s -C $SOURCE_DIR clean; make -s -C $SOURCE_DIR CC=$compiler OPTIM_FLAGS="$flags"; echo "$compiler, $flags, "$($SOURCE_DIR/tiny_ising) >> $FILE

echo "Results stored in: $FILE"
cat $FILE