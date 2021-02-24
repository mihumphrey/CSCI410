for FOLDER in add rect max pong; do
    for ASM in $(ls $FOLDER/*.asm); do
        ../../tools/Assembler.sh $ASM > /dev/null
        NAME=$(echo $ASM | cut -f1 -d ".").hack
        mv $NAME $FOLDER/correct.hack
        ./run $ASM > /dev/null
        echo "NAME: $NAME"
        echo "CORR: $FOLDER/correct.hack"
        DIFF=$(diff $NAME $FOLDER/correct.hack)
        echo $DIFF
#        mv ${$FOLDER/$ASM | cut -f1 -d"."}.hack $FOLDER/correct.hack

    done
#    echo $FOLDER
done

