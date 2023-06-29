#!/bin/bash

root=$(realpath $(dirname $0)/..)

input=
output=$root/SigilCompiler/inc/ParseTable.h

cd $root/ParseTableGen
python3 -m parse_table_gen.main tests/test_descr/test.ebnf $output