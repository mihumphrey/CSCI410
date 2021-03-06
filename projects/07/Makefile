SRCFILES := $(wildcard src/*.c)
HFILES := $(wildcard src/*.h)

all: translator

assembler: $(SRCFILES) $(HFILES)
	gcc -Wall -g -o translator $(SRCFILES) -lm

submission: assembler
	zip -r michaelhumphrey.zip src bin MemoryTest StackTest Makefile README

grade: assembler
	make clean >/dev/null
	make >/dev/null
	./bin/grade.sh

clean:
	rm -rfv translator *.zip

.PHONY: all submission clean grade
