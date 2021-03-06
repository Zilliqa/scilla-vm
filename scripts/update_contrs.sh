#!/bin/bash
# Update contracts in the testsuit and their contrinfo files from compiler repo.

if [[ $# -ne 2 ]]
then
   echo "Usage: $0 scilla_compiler_root testsuite_dir"
   exit 1
fi

scilla_compiler_root=$1
testsuite_dir=$2

if [[ ! -d ${scilla_compiler_root}/tests/codegen/contr || ! -d ${testsuite_dir}/contr ]]
then
    echo "${scilla_compiler_root}/tests/codegen/contr or ${testsuite_dir}/contr not found"
    exit 1
fi

for llfile in ${testsuite_dir}/contr/*.ll
do
    llfile_base=$(basename $llfile .ll)
    if [ "$(basename "$llfile_base" .dbg)" == "$llfile_base" ]
    then
        input_f=${scilla_compiler_root}/tests/codegen/contr/gold/${llfile_base}.scilla.gold
        input_debug_f=${scilla_compiler_root}/tests/codegen/contr/dgold/${llfile_base}.scilla.gold
        input_source_f=${scilla_compiler_root}/tests/codegen/contr/${llfile_base}.scilla
        if [[ ! -f $input_f || ! -f $input_debug_f || ! -f $input_source_f ]]
        then
            echo "$input_f or $input_debug_f or $input_source_f input file(s) not found"
        else
            cat "$input_f" > "$llfile"
            cat "$input_debug_f" > ${testsuite_dir}/contr/"${llfile_base}.dbg.ll"
            # If the test has an init JSON, use that to generate the LLVM-IR.
            if [[ -r ${scilla_compiler_root}/tests/codegen/contr/${llfile_base}.json ]]
            then
                scilla-checker -gaslimit 10000 -libdir "${scilla_compiler_root}"/src/stdlib -contractinfo "$input_source_f" -init "${scilla_compiler_root}/tests/codegen/contr/${llfile_base}.json" > "${testsuite_dir}/contr/${llfile_base}.contrinfo.json"
            else
                scilla-checker -gaslimit 10000 -libdir "${scilla_compiler_root}"/src/stdlib -contractinfo "$input_source_f" > "${testsuite_dir}/contr/${llfile_base}.contrinfo.json"
            fi
        fi
    fi
done
