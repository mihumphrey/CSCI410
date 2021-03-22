#!/bin/bash 

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'

CORRECT="End of script - Comparison ended successfully"

echo -en "Testing ProgramFlow/FibonacciSeries \t"
rm ProgramFlow/FibonacciSeries/FibonacciSeries.asm
./translator ProgramFlow/FibonacciSeries/FibonacciSeries.vm 
OUT=$(../../tools/CPUEmulator.sh ProgramFlow/FibonacciSeries/FibonacciSeries.tst 2>/dev/null)
if [[ $CORRECT = $OUT ]]
then
    echo -en "${GREEN}PASS"
    echo -e "${NC}"
else 
    echo -en "${RED}FAIL"
    echo -e "${NC}"
fi

echo -en "Testing ProgramFlow/BasicLoop     \t"
rm ProgramFlow/BasicLoop/BasicLoop.asm
./translator ProgramFlow/BasicLoop/BasicLoop.vm
OUT=$(../../tools/CPUEmulator.sh ProgramFlow/BasicLoop/BasicLoop.tst 2>/dev/null)
if [[ $CORRECT = $OUT ]]
then
    echo -en "${GREEN}PASS"
    echo -e "${NC}"
else 
    echo -en "${RED}FAIL"
    echo -e "${NC}"
fi
 
echo -en "Testing FunctionCalls/SimpleFunction \t"
rm FunctionCalls/SimpleFunction/SimpleFunction.asm
./translator FunctionCalls/SimpleFunction/SimpleFunction.vm 
OUT=$(../../tools/CPUEmulator.sh FunctionCalls/SimpleFunction/SimpleFunction.tst 2>/dev/null)
if [[ $CORRECT = $OUT ]]
then
    echo -en "${GREEN}PASS"
    echo -e "${NC}"
else 
    echo -en "${RED}FAIL"
    echo -e "${NC}"
fi
 
echo -en "Testing FunctionCalls/NestedCall     \t"
rm FunctionCalls/NestedCall/NestedCall.asm 
./translator FunctionCalls/NestedCall 
OUT=$(../../tools/CPUEmulator.sh FunctionCalls/NestedCall/NestedCall.tst 2>/dev/null)
if [[ $CORRECT = $OUT ]]
then
    echo -en "${GREEN}PASS"
    echo -e "${NC}"
else 
    echo -en "${RED}FAIL"
    echo -e "${NC}"
fi
 
echo -en "Testing FunctionCalls/FibonacciElement \t"
rm FunctionCalls/FibonacciElement/FibonacciElement.asm 
./translator FunctionCalls/FibonacciElement
OUT=$(../../tools/CPUEmulator.sh FunctionCalls/FibonacciElement/FibonacciElement.tst 2>/dev/null)
if [[ $CORRECT = $OUT ]]
then
    echo -en "${GREEN}PASS"
    echo -e "${NC}"
else 
    echo -en "${RED}FAIL"
    echo -e "${NC}"
fi
 
echo -en "Testing FunctionCalls/StaticsTest     \t"
rm FunctionCalls/StaticsTest/StaticsTest.asm 
./translator FunctionCalls/StaticsTest
OUT=$(../../tools/CPUEmulator.sh FunctionCalls/StaticsTest/StaticsTest.tst 2>/dev/null)
if [[ $CORRECT = $OUT ]]
then
    echo -en "${GREEN}PASS"
    echo -e "${NC}"
else 
    echo -en "${RED}FAIL"
    echo -e "${NC}"
fi
    
