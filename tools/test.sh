#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PARENT_DIRECTORY="${SCRIPT_DIR%/*}"

RESULTS_DIR=$PARENT_DIRECTORY/results/
mkdir -p $RESULTS_DIR

SOURCE_DIR=$PARENT_DIRECTORY/c_src

compiler=clang
flags="-O3 -march=native"
make -C $SOURCE_DIR clean; make -C $SOURCE_DIR CC=$compiler OPTIM_FLAGS="$flags"; echo "$compiler, $flags, "$($SOURCE_DIR/tiny_ising)
