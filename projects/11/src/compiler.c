#include "compiler.h"


extern bool verbose;

//********************************************************************************************************************//
//* Function compileClass                                                                                            *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts entire class into vm code                                                                           *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileClass(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t* Beginning Compiling\n");
    ASSERT(currentTokenWordEQ(compiler, "class"), "\"class\" expected as first token")
    compiler->classTable = malloc(sizeof(SymbolTable));
    initList_symbolTable(compiler->classTable, 2); 
    
    
    advance(compiler);
    compiler->className = currentTokenWord(compiler);
    advance(compiler);
    advance(compiler);

    while (isClassVarOpening(compiler->tokens->list[compiler->tokens->iter]->name)) {
        compileClassVarDec(compiler);
    }
    while (isSubroutineOpening(compiler->tokens->list[compiler->tokens->iter]->name)) {
        compileSubroutineDec(compiler);
    }    
    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected after to close class dec")
    advance(compiler);
    freeList_symbolTable(compiler->classTable);
    if (verbose)
        fprintf(stderr, "\t\t* Done Compiling\n");
}

//********************************************************************************************************************//
//* Function compileClassVarDec                                                                                      *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts field/static var decs into vm code                                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileClassVarDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Class Var Declaration\n");
    ASSERT(currentTokenWordEQ(compiler, "static") || currentTokenWordEQ(compiler, "field"), "expected static or field keyword") 
    Segment segment;
    if (currentTokenWordEQ(compiler, "static"))
        segment = STATIC;
    else segment = THIS;
    advance(compiler);
    char *type = currentTokenWord(compiler);    
    advance(compiler);
    while (!currentTokenWordEQ(compiler, ";")) {
        if (currentTokenWordEQ(compiler, ",")) {
            advance(compiler);
        }
        SymbolEntry *entry = malloc(sizeof(SymbolEntry));
        entry->segment = segment;
        entry->type = type;
        entry->name= currentTokenWord(compiler);    
        insertList_symbolTable(compiler->classTable, entry);
        advance(compiler);
        if (verbose)
            fprintf(stderr, "\t\t\t\t* Inserting symbol into class table: SEGMENT: %d\tTYPE: %s\tNAME: %s\n", entry->segment, entry->type, entry->name);
    }
 
    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected after class var declaration")
    advance(compiler);
    
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Class Var Declaration\n");
}

//********************************************************************************************************************//
//* Function compileSubroutineDec                                                                                    *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts functions/constructors into vm code                                                                 *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineDec(Compiler *compiler) {
    compiler->subroutineTable = malloc(sizeof(SymbolTable));
    initList_symbolTable(compiler->subroutineTable, 2); 
    SymbolEntry *entry = malloc(sizeof(SymbolEntry));
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Subroutine Declaration\n");
    //clear(compiler->subroutineTable);
    ASSERT(currentTokenWordEQ(compiler, "method") || currentTokenWordEQ(compiler, "constructor") || currentTokenWordEQ(compiler, "function"), "expected method or constructor keyword at beginning of subroutine")

    if (currentTokenWordEQ(compiler, "constructor")) {
        compiler->classTable->isConstructor = true;
        compiler->classTable->isMethod = false;
    }
    else if (currentTokenWordEQ(compiler, "method")) {
        compiler->classTable->isMethod = true;
        compiler->classTable->isConstructor = false;
        entry->name = "instance";
        entry->type = compiler->className;
        entry->segment = ARG;
        insertList_symbolTable(compiler->subroutineTable, entry);
    } else {
        compiler->classTable->isMethod = false;
        compiler->classTable->isConstructor = false;
    }   
    

    advance(compiler);
    advance(compiler);
    compiler->subroutineName = currentTokenWord(compiler);
    advance(compiler);
    ASSERT(currentSymbolEQ(compiler, '('), "'(' expected at beginning of parameter list")
    advance(compiler);
    compileParameterList(compiler);
    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected at end of parameter list")
    advance(compiler);
    compileSubroutineBody(compiler);
    freeList_symbolTable(compiler->subroutineTable);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Subroutine Declaration\n");
}

//********************************************************************************************************************//
//* Function compileSubroutineBody                                                                                   *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts body of subroutine into vm code                                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineBody(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Subroutine Body\n");
    ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected after parameter list")
    advance(compiler);
    while (currentTokenWordEQ(compiler, "var")) {
        compileVarDec(compiler);
        advance(compiler);
    }
    WRITE("function %s.%s %d\n" ,compiler->className, compiler->subroutineName, getNextOffset(compiler->subroutineTable, LOCAL));
    
    if (compiler->classTable->isConstructor) {
        WRITE("push constant %d\n", getNextOffset(compiler->classTable, THIS))
        WRITE("call Memory.alloc 1\n")
        WRITE("pop pointer 0\n")
    } else if (compiler->classTable->isMethod) {
        WRITE("push argument 0\n");
        WRITE("pop pointer 0\n");
    }
    
    compileStatements(compiler);
    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected to close subroutine")
    advance(compiler);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Subroutine Body\n");
}

//********************************************************************************************************************//
//* Function compileParameterList                                                                                    *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts parameter list (int x, int y, etc) into vm code                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileParameterList(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Parameter List\n");
    while (currentSymbol(compiler) != ')') {
        SymbolEntry *entry = malloc(sizeof(SymbolEntry));
        entry->type = currentTokenWord(compiler);
        advance(compiler); 
        entry->name = currentTokenWord(compiler);
        advance(compiler);
        if (currentSymbolEQ(compiler, ','))
            advance(compiler);
        entry->segment = ARG;
        if (verbose)
            fprintf(stderr, "\t\t\t\t* Inserting symbol into subroutine table: SEGMENT: %d\tTYPE: %s\tNAME: %s\n", entry->segment, entry->type, entry->name);
        insertList_symbolTable(compiler->subroutineTable, entry);
    }
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Parameter List\n");
}

//********************************************************************************************************************//
//* Function compileVarDec                                                                                           *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts var decs into vm code                                                                               *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileVarDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Var Declaration\n");
    ASSERT(currentTokenWordEQ(compiler, "var"), "expected 'var' keyword to declare var")
    advance(compiler);
    char *type = currentTokenWord(compiler);
    advance(compiler); 
    while (currentSymbol(compiler) != ';') {
        if (currentTokenWordEQ(compiler, ",")) {
            advance(compiler);
        }
        SymbolEntry *entry = malloc(sizeof(SymbolEntry));
        entry->segment = LOCAL;
        entry->type = type;
        entry->name= currentTokenWord(compiler);    
        insertList_symbolTable(compiler->subroutineTable, entry);
        advance(compiler);
        if (verbose)
            fprintf(stderr, "\t\t\t\t* Inserting symbol into subroutine table: SEGMENT: %d\tTYPE: %s\tNAME: %s\n", entry->segment, entry->type, entry->name);

    }

    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected after class var declaration")


    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Var Declaration\n");
}

//********************************************************************************************************************//
//* Function compileStatements                                                                                       *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts all statements into vm code                                                                         *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileStatements(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Statements\n");
    while (isStatementOpening(currentTokenWord(compiler))) {
        if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "do"))
            compileDo(compiler);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "if"))
            compileIf(compiler);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "let"))
            compileLet(compiler);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "return"))
            compileReturn(compiler);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "while"))
            compileWhile(compiler);
    }

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Statements\n");
}

//********************************************************************************************************************//
//* Function compileDo                                                                                               *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts do statement into vm code                                                                           *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileDo(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Do Statement\n");
    ASSERT(currentTokenWordEQ(compiler, "do"), "'do' keyword expected")
    advance(compiler);
   
    compileSubroutineCall(compiler);
    WRITE("pop temp 0\n"); 
    advance(compiler);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Do Statement\n");
}

//********************************************************************************************************************//
//* Function compileLet                                                                                              *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts let statement into vm code                                                                          *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileLet(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Let Statement\n");
    
    bool isArr = false;
    ASSERT(currentTokenWordEQ(compiler, "let"), "'let' keyword expected")
    advance(compiler);
    char *name = currentTokenWord(compiler);
    char *segment = NULL;
    int offset = 0;
    
    SymbolEntry *entry = getEntry(compiler, name);
    advance(compiler);
    advance(compiler);
    if (compiler->tokens->list[compiler->tokens->iter - 1]->name[0] == '[') {
        isArr = true;
        if (entry != NULL) {
            WRITE("push %s %d\n", getSymbolSegment(entry->segment), entry->offset)
            compileExpression(compiler);
            ASSERT(currentSymbolEQ(compiler, ']'), "']' expected after end of expression")
            WRITE("add\n");
            segment = "that";
            offset = 0;
        } else ASSERT(0 == 1, "symbol not defined")
        advance(compiler);
        ASSERT(currentSymbolEQ(compiler, '='), "'=' expected after end of expression")
        advance(compiler);
    } else {
        segment = getSymbolSegment(entry->segment);
        offset = entry->offset;
    }
    
    compileExpression(compiler);
    if (isArr) {
        WRITE("pop temp 0\n")
        WRITE("pop pointer 1\n")
        WRITE("push temp 0\n");
    }
    WRITE("pop %s %d\n", segment, offset) 
    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected after let statement 1")
    advance(compiler);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Let Statement\n");
}

//********************************************************************************************************************//
//* Function compileWhile                                                                                            *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts while statement into vm code                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileWhile(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling While Statement\n");
    char labelNumchar[256];
    sprintf(labelNumchar, "%d", compiler->labelNum++);
    int len = strlen(labelNumchar) + 1;
    char *trueLabel = calloc(1, len + 1);
    trueLabel[0] = 'W';
    strcat(&trueLabel[1], labelNumchar);
    trueLabel[len] = '\0'; 

    sprintf(labelNumchar, "%d", compiler->labelNum++);
    len = strlen(labelNumchar) + 1;
    char *falseLabel = calloc(1, len + 1);
    falseLabel[0] = 'W';
    strcat(&falseLabel[1], labelNumchar);
    falseLabel[len] = '\0'; 
        
    ASSERT(currentTokenWordEQ(compiler, "while"), "'while' keyword expected")
    advance(compiler);

    WRITE("label %s\n", trueLabel)
    ASSERT(currentSymbolEQ(compiler, '('), "'(' expected in while statement")
    advance(compiler);

    compileExpression(compiler);

    WRITE("not\n")
    WRITE("if-goto %s\n", falseLabel)
    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected in while statement")
    advance(compiler);

    ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected in while statement")
    advance(compiler);

    compileStatements(compiler);
    WRITE("goto %s\n", trueLabel)
    WRITE("label %s\n", falseLabel)
    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected in while statement")
    advance(compiler);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling While Statement\n");
}

//********************************************************************************************************************//
//* Function compileReturn                                                                                           *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts return statement into vm code                                                                       *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileReturn(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Return Statement\n");
    ASSERT(currentTokenWordEQ(compiler, "return"), "'return' keyword expected")
    advance(compiler);

    if (currentSymbol(compiler) != ';') {
        compileExpression(compiler);
    } else {
        WRITE("push constant 0\n")
    }
    WRITE("return\n")
    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected after let statement")
    advance(compiler);


    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Return Statement\n");
}

//********************************************************************************************************************//
//* Function compileIf                                                                                               *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts if statement into vm code                                                                           *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileIf(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling If Statement\n");

    char trueLabel[256], falseLabel[256], endLabel[256];
    sprintf(trueLabel, "IF-TRUE%d", compiler->labelNum++);
    sprintf(falseLabel, "IF-FALSE%d", compiler->labelNum++);
    sprintf(endLabel, "IF-END%d", compiler->labelNum++);


    ASSERT(currentTokenWordEQ(compiler, "if"), "'if' keyword expected")
    advance(compiler);

    ASSERT(currentSymbolEQ(compiler, '('), "'(' expected in if statement")
    advance(compiler);

    compileExpression(compiler);

    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected in if statement")
    advance(compiler);

    WRITE("if-goto %s\n", trueLabel)
    WRITE("goto %s\n", falseLabel)
    WRITE("label %s\n", trueLabel)

    ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected in if statement")
    advance(compiler);

    compileStatements(compiler);
    WRITE("goto %s\n", endLabel);

    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected in if statement")
    advance(compiler);

    WRITE("label %s\n", falseLabel)
    if (currentTokenWordEQ(compiler, "else")) {
        ASSERT(currentTokenWordEQ(compiler, "else"), "'else' keyword expected")
        advance(compiler);

        ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected in if statement")
        advance(compiler);

        compileStatements(compiler);

        ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected in if statement")
        advance(compiler);
    }
    WRITE("label %s\n", endLabel)

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling If Statement\n");
}

//********************************************************************************************************************//
//* Function compileExpression                                                                                       *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts nested expression into vm code                                                                      *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileExpression(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Expression\n");
    compileTerm(compiler);
    while (isOperator(currentTokenWord(compiler))) {
        const char *operator = getArithCommand(currentTokenWord(compiler));
            
        advance(compiler);
        compileTerm(compiler);
        WRITE("%s\n", operator)
    }
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Expression\n");
}

//********************************************************************************************************************//
//* Function compileTerm                                                                                             *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts terminal into vm code                                                                               *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileTerm(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Terminal\n");
    char *name, *subName;
    SymbolEntry *entry;
    switch(compiler->tokens->list[compiler->tokens->iter ]->type) {
        case IDENTIFIER : 
            name = currentTokenWord(compiler);
            advance(compiler);
            ASSERT(compiler->tokens->list[compiler->tokens->iter]->type == SYMBOL, "expected symbol")         
            switch (currentSymbol(compiler)) {
                case '.' :
                    advance(compiler);
                    subName = currentTokenWord(compiler);
                    advance(compiler);
                    ASSERT(currentSymbolEQ(compiler, '('), "expected '(' to start expression list")
                    entry = getEntry(compiler, name);
                    if (entry != NULL) {
                        WRITE("push %s %d\n", getSymbolSegment(entry->segment), entry->offset)
                    }
                    advance(compiler);
                    compiler->numExpressions = 0;
                    compileExpressionList(compiler);
                    ASSERT(currentSymbolEQ(compiler, ')'), "expected ')' to end expression list")
                    entry = getEntry(compiler, name);
                    if (entry != NULL) {
                        WRITE("call %s.%s %d\n", entry->type, subName, compiler->numExpressions + 1)
                    } else WRITE("call %s.%s %d\n", name, subName, compiler->numExpressions)
                    advance(compiler); 
                    break;
                case '(' :
                    advance(compiler);
                    WRITE("push pointer 0\n")
                    compileExpressionList(compiler);
                    compiler->numExpressions++;
                    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected after expression list")
                    WRITE("call %s.%s %d\n", compiler->className, name, compiler->numExpressions)
                    advance(compiler);
                    break;
                case '[' :
                    entry = getEntry(compiler, name);
                    advance(compiler);
                    compileExpression(compiler);
                    if (entry != NULL)
                        WRITE("push %s %d\n", getSymbolSegment(entry->segment), entry->offset)
                    else ASSERT(0==1, "could not find symbol in table")
                    ASSERT(currentSymbolEQ(compiler, ']'), "']' expected after expression")
                    WRITE("add\n")
                    WRITE("pop pointer 1\n")
                    WRITE("push that 0\n")
                    advance(compiler);
                    break;
                default :
                    entry = getEntry(compiler, name);
                    if (entry != NULL)
                        WRITE("push %s %d\n", getSymbolSegment(entry->segment), entry->offset)
                    break;
            } 
            break;
        case SYMBOL :
            if (currentSymbolEQ(compiler, '(')) {
                advance(compiler);
                compileExpression(compiler);
                ASSERT(currentSymbolEQ(compiler, ')'), "')' expected after expression list")
                advance(compiler);
            } else if (isUnaryOp(currentSymbol(compiler))) { 
                char op = currentSymbol(compiler);
                advance(compiler);
                compileTerm(compiler);
                WRITE("%s\n", getUnaryArithCommand(op)) 
            } else printf("EHRERERERE\n");
            break;

        case KEYWORD :
            if (STREQUALS(currentTokenWord(compiler), "null") || STREQUALS(currentTokenWord(compiler), "false"))
                WRITE("push constant 0\n")
            else if (STREQUALS(currentTokenWord(compiler), "true")) {
                WRITE("push constant 0\n")
                WRITE("not\n")
            } else if (STREQUALS(currentTokenWord(compiler), "this"))
                WRITE("push pointer 0\n")
            else ASSERT(0 == 1, "invalid keyword")
            advance(compiler);
            break;

        case STRING_CONST :
            WRITE("push constant %d\n", (int)strlen(currentTokenWord(compiler)))
            WRITE("call String.new 1\n")
            for (int i = 0; i < strlen(currentTokenWord(compiler)); i++) {
                WRITE("push constant %d\n", currentTokenWord(compiler)[i])
                WRITE("call String.appendChar 2\n")
            }
            advance(compiler);
            break;

        case INT_CONST :
            WRITE("push constant %d\n", atoi(currentTokenWord(compiler))) 
            advance(compiler);
            break;
        default :
            ASSERT(0 == 1, "Invalid terminal received")        
    }
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Terminal\n");
}

//********************************************************************************************************************//
//* Function compilesubroutineCall                                                                                   *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts subrioutineCall into vm code                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineCall(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Subroutine Call\n");
    char *first = currentTokenWord(compiler);
    char *second = NULL;
    SymbolEntry *entry = getEntry(compiler, first);
    advance(compiler);
    if (currentSymbolEQ(compiler, '.')) {
        advance(compiler);
        second = currentTokenWord(compiler);
        if (entry != NULL)
            WRITE("push %s %d\n", getSymbolSegment(entry->segment), entry->offset)
        advance(compiler);
        ASSERT(currentSymbolEQ(compiler, '('), "'(' expected after expression list")
    } else WRITE("push pointer 0\n")
    advance(compiler);

    compileExpressionList(compiler);
    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected after expression list")
    advance(compiler);
    char call[256];
    if (second != NULL)
        if (entry != NULL) {
            sprintf(call, "%s.%s", entry->type, second);
            compiler->numExpressions++;
        } else 
            sprintf(call, "%s.%s", first, second);
    else {
        sprintf(call, "%s.%s", compiler->className, first);
        compiler->numExpressions++;
    }
    WRITE("call %s %d\n", call, compiler->numExpressions)
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Subroutine Call\n");
}

//********************************************************************************************************************//
//* Function compileExpressionList                                                                                   *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Converts expression list (x, y) into vm code                                                                 *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileExpressionList(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Expression List\n");
    compiler->numExpressions = 0;
    while (currentSymbol(compiler) != ')') {
        compiler->numExpressions++;
        compileExpression(compiler);
        if (currentSymbolEQ(compiler, ',')) {
            advance(compiler);
        }
    }
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Expression List\n");
}

//********************************************************************************************************************//
//* Helper Function advance                                                                                          *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     increments compiler->tokens iterator by one                                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void advance(Compiler *compiler) {
    compiler->tokens->iter++;
}

//********************************************************************************************************************//
//* Helper Function currentsymbol                                                                                    *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Returns:                                                                                                     *//
//*         char -- current symbol in char format                                                                    *//
//********************************************************************************************************************//
char currentSymbol(Compiler *compiler) {
    return compiler->tokens->list[compiler->tokens->iter]->name[0];
}

//********************************************************************************************************************//
//* Helper Function currentSymbolEQ                                                                                  *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*         symbolName: char * -- char to compare                                                                    *//
//*     Returns:                                                                                                     *//
//*         bool -- whether or not current symbol is equal to parameter                                              *//
//********************************************************************************************************************//
bool currentSymbolEQ(Compiler *compiler, char symbolName) {
    ASSERT(compiler->tokens->list[compiler->tokens->iter]->type == SYMBOL, "current token is expected to be SYMBOL but is not")
    return compiler->tokens->list[compiler->tokens->iter]->name[0] == symbolName;
}

//********************************************************************************************************************//
//* Helper Function currenTokenWord                                                                                  *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*     Returns:                                                                                                     *//
//*         char * -- current token full name                                                                        *//
//********************************************************************************************************************//
char *currentTokenWord(Compiler *compiler) {
    return compiler->tokens->list[compiler->tokens->iter]->name;
}

//********************************************************************************************************************//
//* Helper Function currentTokenWordEQ                                                                               *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*         name: char * -- string to compare to                                                                     *//
//*     Returns:                                                                                                     *//
//*         bool -- whether or not current token is equal to parameter                                               *//
//********************************************************************************************************************//
bool currentTokenWordEQ(Compiler *compiler, char *name) {
    return STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, name);
}

//********************************************************************************************************************//
//* Helper Function getEntry                                                                                         *//
//*     Input:                                                                                                       *//
//*         compiler: Compiler * -- main compiler object                                                             *//
//*         name: char * -- string to get                                                                            *//
//*     Returns:                                                                                                     *//
//*         SymbolEntry * -- entry in tables corresponding to given name                                             *//
//********************************************************************************************************************//
SymbolEntry *getEntry(Compiler *compiler, char *name) {
    for (int i = 0; i < compiler->classTable->used; i++)
        if (STREQUALS(name, compiler->classTable->list[i]->name))
            return compiler->classTable->list[i];

    for (int i = 0; i < compiler->subroutineTable->used; i++)
        if (STREQUALS(compiler->subroutineTable->list[i]->name, name))
            return compiler->subroutineTable->list[i];
    return NULL;
}
