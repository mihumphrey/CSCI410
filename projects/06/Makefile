SRCFILES := $(wildcard src/*.c)
HFILES := $(wildcard src/*.h)

all: assembler

assembler: $(SRCFILES) $(HFILES)
	gcc -Wall -g -o assembler $(SRCFILES) -lm

submission: assembler
	zip -r michaelhumphrey.zip src rect max pong add Makefile README

grade: assembler
	make clean >/dev/null
	make >/dev/null
	./bin/grade.sh

clean:
	rm -rfv assembler *.zip

.PHONY: all submission clean grade
