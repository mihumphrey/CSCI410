SRCFILES := $(wildcard src/*.c)
SRCFILES2 := $(wildcard src2/*.c)
HFILES := $(wildcard src/*.h)

all: translator

translator: $(SRCFILES) $(HFILES)
	gcc -Wall -g -Ofast -o translator $(SRCFILES) -lm
working: translator
	gcc -Wall -g -Ofast -o working $(SRCFILES2) -lm
	
submission: translator
	make clean >/dev/null
	zip -r michaelhumphrey.zip src bin ProgramFlow FunctionCalls Makefile README

grade: translator
	make clean >/dev/null
	make >/dev/null
	./bin/grade.sh

clean:
	rm -rfv translator *.zip

.PHONY: all submission clean grade
