#!/bin/bash 

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'

CORRECT="Comparison ended successfully"

for i in ArrayTest Square ExpressionLessSquare; do
    ./compiler $i >/dev/null
   for j in $(ls $i/*.xml); do
        echo -en "Testing $i/${j##*/} \t"
        OUT=$(../../tools/TextComparer.sh $j $i/test/${j##*/} 2>/dev/null)
        if [[ $CORRECT = $OUT ]]
        then
            echo -e "${GREEN}PASS${NC}"
        else 
            echo -e "${RED}FAIL${NC}"
            echo -e "\t${RED}$OUT${NC}"
        fi
    done

done

    
