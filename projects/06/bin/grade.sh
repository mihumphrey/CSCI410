#!/bin/bash 
TOTAL=0
CORRECT=0

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'


for FOLDER in add rect max pong; do
    for ASM in $(ls $FOLDER/*.asm); do
        echo -en "Testing file $ASM...\t"
        NAME=$(echo $ASM | cut -f1 -d ".").hack
        rm -rf $FOLDER/correct.hack $FOLDER/$NAME
        TOTAL=$((TOTAL + 1))   
        ../../tools/Assembler.sh $ASM > /dev/null
        mv $NAME $FOLDER/correct.hack
        ./run $ASM $1 
        if ! diff -q $NAME $FOLDER/correct.hack &>/dev/null; then
            echo -en "${RED}FAIL"
            echo -en ${NC}
        else
            echo -en "${GREEN}PASS"
            echo -en ${NC}
            CORRECT=$((CORRECT + 1))
        fi
        echo
        rm -rf $FOLDER/correct.hack
    done
done

echo $CORRECT/$TOTAL
