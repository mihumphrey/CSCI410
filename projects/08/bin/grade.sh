#!/bin/bash 

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'

CORRECT="End of script - Comparison ended successfully"

for i in MemoryAccess/BasicTest/BasicTest.vm MemoryAccess/PointerTest/PointerTest.vm MemoryAccess/StaticTest/StaticTest.vm StackArithmetic/SimpleAdd/SimpleAdd.vm StackArithmetic/StackTest/StackTest.vm;do
    echo -en "Testing $i     \t"
    ./translator $i
    OUT=$(CPUEmulator.sh ${i%.vm}.tst 2>/dev/null )
    if [[ $CORRECT = $OUT ]]
    then
        echo -en "${GREEN}PASS"
        echo -e "${NC}"
    else 
        echo -en "${RED}FAIL"
        echo -e "${NC}"
    fi
done
