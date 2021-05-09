ALLSRCFILES := $(wildcard src/*.c)
COMPSRCFILES := $(wildcard compiler/src/*.c)
COMPHFILES := $(wildcard compiler/src/*.h)
ANALSRCFILES := $(wildcard analyzer/src/*.c)
ANALHFILES := $(wildcard analyzer/src/*.h)
TRANSSRCFILES := $(wildcard translator/src/*.c)
TRANSHFILES := $(wildcard translator/src/*.h)
ASSEMSRCFILES := $(wildcard assembler/src/*.c)
ASSEMHFILES := $(wildcard assembler/src/*.h)

all: $(COMPSRCFILES) $(COMPHFILES) $(ANALSRCFILES) $(ANALHFILES) $(TRANSSRCFILES) $(TRANSHFILES) $(ASSEMSRCFILES) $(ASSEMHFILES) 
	gcc -Wall -g -Ofast -o run $(ALLSRCFILES) -lm 
	gcc -Wall -g -Ofast -o bin/compiler $(COMPSRCFILES) -lm 
	gcc -Wall -g -Ofast -o bin/compiler $(COMPSRCFILES) -lm 
	gcc -Wall -g -Ofast -o bin/analyzer $(ANALSRCFILES) -lm 
	gcc -Wall -g -Ofast -o bin/translator $(TRANSSRCFILES) -lm 
	gcc -Wall -g -Ofast -o bin/assembler $(ASSEMSRCFILES) -lm 

total: $(ALLSRCFILES)
	gcc -Wall -g -Ofast -o run $(ALLSRCFILES) -lm 

compiler: $(COMPSRCFILES) $(COMPHFILES) 
	gcc -Wall -g -Ofast -o bin/compiler $(COMPSRCFILES) -lm 

analyzer: $(ANALSRCFILES) $(ANALHFILES)
	gcc -Wall -g -Ofast -o bin/analyzer $(ANALSRCFILES) -lm 

translator: $(TRANSSRCFILES) $(TRANSHFILES)
	gcc -Wall -g -Ofast -o bin/translator $(TRANSSRCFILES) -lm 

assembler: $(ASSEMSRCFILES) $(ASSEMHFILES) 
	gcc -Wall -g -Ofast -o bin/assembler $(ASSEMSRCFILES) -lm 

clean:
	rm -f bin/*

.PHONY: all total compiler analyzer translator assembler clean
