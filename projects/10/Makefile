SRCFILES := $(wildcard src/*.c)
HFILES := $(wildcard src/*.h)

all: compiler

compiler: $(SRCFILES) $(HFILES)
	gcc -Wall -g -Ofast -o compiler $(SRCFILES) -lm 
	
debug: $(SRCFILES) $(HFILES)
	gcc -Wall -g -Ofast -o compiler $(SRCFILES) -lm -fsanitize=address

submission: compiler
	make clean >/dev/null
	zip -r michaelhumphrey.zip src bin ProgramFlow FunctionCalls Makefile README

grade: compiler
	make clean >/dev/null
	make >/dev/null
	./bin/grade.sh

clean:
	rm -rfv compiler *.zip

.PHONY: all submission clean grade
