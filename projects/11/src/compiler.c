#include "compiler.h"


extern bool verbose;

//********************************************************************************************************************//
//* Function compileClass                                                                                            *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                     *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*     Converts entire class into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileClass(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t* Beginning Compiling\n");
    ASSERT(currentTokenWordEQ(compiler, "class"), "\"class\" expected as first token")
    clear(compiler->classTable);
    //writeTag("<class>", outputFile, indentLevel);
    
    
    advance(compiler);
    compiler->className = currentTokenWord(compiler);
    advance(compiler);
    advance(compiler);

    while (isClassVarOpening(compiler->tokens->list[compiler->tokens->iter]->name)) {
        compileClassVarDec(compiler);
    }
    printf("CLASS TABLE\n");
    printTable(compiler->classTable);
    while (isSubroutineOpening(compiler->tokens->list[compiler->tokens->iter]->name)) {
        compileSubroutineDec(compiler);
    }    
    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected after to close class dec")
    //writeToken(compiler->tokens, outputFile, indentLevel + 1);
    advance(compiler);
    //writeTag("</class>", outputFile, indentLevel);
    if (verbose)
        fprintf(stderr, "\t\t* Done Compiling\n");
}

//********************************************************************************************************************//
//* Function compileClassVarDec                                                                                      *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                     *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts field/static var decs into xml format                                                               *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileClassVarDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Class Var Declaration\n");
    //writeTag("<classVarDec>", outputFile, indent);
    //writeToken(compiler->tokens, outputFile, indent + 1);
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
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
    //writeTag("</classVarDec>", outputFile, indent);
    
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Class Var Declaration\n");
}

//********************************************************************************************************************//
//* Function compileSubroutineDec                                                                                    *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts functions/constructors into xml format                                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Subroutine Declaration\n");
    //writeTag("<subroutineDec>", outputFile, indent);
    clear(compiler->subroutineTable);
    ASSERT(currentTokenWordEQ(compiler, "method") || currentTokenWordEQ(compiler, "constructor") || currentTokenWordEQ(compiler, "function"), "expected method or constructor keyword at beginning of subroutine")

    if (currentTokenWordEQ(compiler, "constructor")) {
        compiler->classTable->isConstructor = true;
        compiler->classTable->isMethod = false;
    }
    else if (currentTokenWordEQ(compiler, "method")) {
        compiler->classTable->isMethod = true;
        compiler->classTable->isConstructor = false;
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
    //writeToken(compiler->tokens, outputFile, indent + 1); 
    advance(compiler);
    compileSubroutineBody(compiler);
    //writeTag("</subroutineDec>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Subroutine Declaration\n");
}

//********************************************************************************************************************//
//* Function compileSubroutineBody                                                                                   *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts body of subroutine into xml format                                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineBody(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Subroutine Body\n");
    //writeTag("<subroutineBody>", outputFile, indent);
    ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected after parameter list")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
    
    while (currentTokenWordEQ(compiler, "var")) {
        compileVarDec(compiler);
        advance(compiler);
    }
    WRITE("function %s.%s %d\n" ,compiler->className, compiler->subroutineName, getNextOffset(compiler->subroutineTable, ARG));
    
    if (compiler->classTable->isConstructor) {
        WRITE("push constant %d\n", getNextOffset(compiler->classTable, THIS))
        WRITE("call Memory.alloc 1\n")
        WRITE("pop pointer 0\n")
    } else if (compiler->classTable->isMethod) {
        WRITE("push arg 0\n");
        WRITE("pop pointer 0\n");
    }
    
    compileStatements(compiler);
    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected to close subroutine")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
    //writeTag("</subroutineBody>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Subroutine Body\n");
}

//********************************************************************************************************************//
//* Function compileParameterList                                                                                    *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts parameter list (int x, int y, etc) into xml format                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileParameterList(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Parameter List\n");
    //writeTag("<parameterList>", outputFile, indent);
    while (currentSymbol(compiler) != ')') {
        SymbolEntry *entry = malloc(sizeof(SymbolEntry));
        //writeToken(compiler->tokens, outputFile, indent + 1);
//        printf("TYPE: %s\n", currentTokenWord(compiler));
        entry->type = currentTokenWord(compiler);
        advance(compiler); 
//        printf("NAME: %s\n", currentTokenWord(compiler));
        entry->name = currentTokenWord(compiler);
        advance(compiler);
//        printf("COMMA OR CLOSE: %s\n", currentTokenWord(compiler));
        if (currentSymbolEQ(compiler, ','))
            advance(compiler);
        entry->segment = ARG;
        if (verbose)
            fprintf(stderr, "\t\t\t\t* Inserting symbol into subroutine table: SEGMENT: %d\tTYPE: %s\tNAME: %s\n", entry->segment, entry->type, entry->name);
        insertList_symbolTable(compiler->subroutineTable, entry);
    }
    //writeTag("</parameterList>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Parameter List\n");
}

//********************************************************************************************************************//
//* Function compileVarDec                                                                                           *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts var decs into xml format                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileVarDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Var Declaration\n");
    //writeTag("<varDec>", outputFile, indent);
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

        //writeToken(compiler->tokens, outputFile, indent + 1);
    }

    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected after class var declaration")
    //writeToken(compiler->tokens, outputFile, indent + 1);

    //writeTag("</varDec>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Var Declaration\n");
}

//********************************************************************************************************************//
//* Function compileStatements                                                                                       *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts all statements into xml format                                                                      *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileStatements(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Statements\n");
    //writeTag("<statements>", outputFile, indent);
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
    //writeTag("</statements>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Statements\n");
}

//********************************************************************************************************************//
//* Function compileDo                                                                                               *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts do statement into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileDo(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Do Statement\n");
    //writeTag("<doStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(compiler, "do"), "'do' keyword expected")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
   
    compileSubroutineCall(compiler);
    WRITE("pop temp 0\n"); 
    advance(compiler);
/*
    while (currentSymbol(compiler) != '(') {
        //writeToken(compiler->tokens, outputFile, indent + 1);
        advance(compiler);
    }

    ASSERT(currentSymbolEQ(compiler, '('), "'(' expected at beginning of expression list")
    //writeToken(compiler->tokens, outputFile, indent + 1); 
    advance(compiler);

    compileExpressionList(compiler);

    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected at end of expression list")
    //writeToken(compiler->tokens, outputFile, indent + 1); 
    advance(compiler);

    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected at end of statement")
    //writeToken(compiler->tokens, outputFile, indent + 1); 
    advance(compiler);
*/
    //writeTag("</doStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Do Statement\n");
}

//********************************************************************************************************************//
//* Function compileLet                                                                                              *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts let statement into xml format                                                                       *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileLet(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Let Statement\n");
    
    bool isArr = false;
    //writeTag("<letStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(compiler, "let"), "'let' keyword expected")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
    char *name = currentTokenWord(compiler);
    SymbolEntry *entry = getEntry(compiler, name);
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
    if (compiler->tokens->list[compiler->tokens->iter - 1]->name[0] == '[') {
        isArr = true;
        if (entry != NULL) {
            WRITE("push %s %d\n // let", getSymbolSegment(entry->segment), entry->offset)
        } else ASSERT(0 == 1, "symbol not defined")
        compileExpression(compiler);
        ASSERT(currentSymbolEQ(compiler, ']'), "']' expected after end of expression")
        //writeToken(compiler->tokens, outputFile, indent + 1);
        advance(compiler);
        ASSERT(currentSymbolEQ(compiler, '='), "'=' expected after end of expression")
        //writeToken(compiler->tokens, outputFile, indent + 1);
        advance(compiler);
    } 
    
    compileExpression(compiler);
    if (isArr) {
        WRITE("pop temp 0\n")
        WRITE("pop pointer 1\n")
        WRITE("push temp 0\n");
    }
    WRITE("pop %s %d\n", getSymbolSegment(entry->segment), entry->offset) 
    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected after let statement 1")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);
    //writeTag("</letStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Let Statement\n");
}

//********************************************************************************************************************//
//* Function compileWhile                                                                                            *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts while statement into xml format                                                                     *//
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
        
    //writeTag("<whileStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(compiler, "while"), "'while' keyword expected")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    ASSERT(currentSymbolEQ(compiler, '('), "'(' expected in while statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    WRITE("label %s\n", trueLabel)
    advance(compiler);

    compileExpression(compiler);

    WRITE("not\n")
    WRITE("if-goto %s\n", falseLabel)
    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected in while statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected in while statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    compileStatements(compiler);
    WRITE("goto %s\n", trueLabel)
    WRITE("label %s\n", falseLabel)
    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected in while statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    //writeTag("</whileStatement>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling While Statement\n");
}

//********************************************************************************************************************//
//* Function compileReturn                                                                                           *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts return statement into xml format                                                                    *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileReturn(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Return Statement\n");
    //writeTag("<returnStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(compiler, "return"), "'return' keyword expected")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    if (currentSymbol(compiler) != ';') {
        compileExpression(compiler);
    } else {
        WRITE("push constant 0\n")
    }
    WRITE("return\n")
    ASSERT(currentSymbolEQ(compiler, ';'), "';' expected after let statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    //writeTag("</returnStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Return Statement\n");
}

//********************************************************************************************************************//
//* Function compileIf                                                                                               *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts if statement into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileIf(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling If Statement\n");

    char trueLabel[256], falseLabel[256], endLabel[256];
    sprintf(trueLabel, "IF-TRUE-%d", compiler->labelNum++);
    sprintf(falseLabel, "IF-FALSE-%d", compiler->labelNum++);
    sprintf(endLabel, "IF-END-%d", compiler->labelNum++);


    printf("TRUE: %s\tFALSE: %s\tEND: %s\n", trueLabel, falseLabel, endLabel);
     
    //writeTag("<ifStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(compiler, "if"), "'if' keyword expected")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    ASSERT(currentSymbolEQ(compiler, '('), "'(' expected in if statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    compileExpression(compiler);

    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected in if statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    WRITE("if-goto %s\n", trueLabel)
    WRITE("goto %s\n", falseLabel)
    WRITE("label %s\n", trueLabel)

    ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected in if statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    compileStatements(compiler);

    ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected in if statement")
    //writeToken(compiler->tokens, outputFile, indent + 1);
    advance(compiler);

    if (currentTokenWordEQ(compiler, "else")) {
        ASSERT(currentTokenWordEQ(compiler, "else"), "'else' keyword expected")
        WRITE("goto %s\n", endLabel)
        WRITE("label %s\n", falseLabel);
        //writeToken(compiler->tokens, outputFile, indent + 1);
        advance(compiler);

        ASSERT(currentSymbolEQ(compiler, '{'), "'{' expected in if statement")
        //writeToken(compiler->tokens, outputFile, indent + 1);
        advance(compiler);

        compileStatements(compiler);

        ASSERT(currentSymbolEQ(compiler, '}'), "'}' expected in if statement")
        //writeToken(compiler->tokens, outputFile, indent + 1);
        advance(compiler);
        WRITE("label %s\n", endLabel)
    }
    WRITE("label %s\n", falseLabel)
    //writeTag("</ifStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling If Statement\n");
}

//********************************************************************************************************************//
//* Function compileExpression                                                                                       *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts nested expression into xml format                                                                   *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileExpression(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Expression\n");
    //writeTag("<expression>", outputFile, indent);
    compileTerm(compiler);
    while (isOperator(currentTokenWord(compiler))) {
        //writeToken(compiler->tokens, outputFile, indent + 1);
        const char *operator = getArithCommand(currentTokenWord(compiler));
            
        advance(compiler);
        compileTerm(compiler);
        WRITE("%s\n", operator)
    }
    //writeTag("</expression>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Expression\n");
}

//********************************************************************************************************************//
//* Function compileTerm                                                                                             *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts terminal into xml format                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileTerm(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Terminal\n");
    //writeTag("<term>", outputFile, indent);
    //writeToken(compiler->tokens, outputFile, indent + 1);
    char *name, *subName;
    SymbolEntry *entry;
    switch(compiler->tokens->list[compiler->tokens->iter ]->type) {
        case IDENTIFIER : 
            name = currentTokenWord(compiler);
            advance(compiler);
            ASSERT(compiler->tokens->list[compiler->tokens->iter]->type == SYMBOL, "expected symbol")         
            switch (currentSymbol(compiler)) {
                case '.' :
                    //writeToken(compiler->tokens, outputFile, indent + 1);
                    advance(compiler);
                    //writeToken(compiler->tokens, outputFile, indent + 1);
                    subName = currentTokenWord(compiler);
                    advance(compiler);
                    //writeToken(compiler->tokens, outputFile, indent + 1);
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
                    //compileSubroutineCall(compiler);
                    advance(compiler); 
                    break;
                case '(' :
                    //writeToken(compiler->tokens, outputFile, indent + 1);
                    advance(compiler);
                    WRITE("push pointer 0\n")
                    compileExpressionList(compiler);
                    compiler->numExpressions++;
                    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected after expression list")
                    //writeToken(compiler->tokens, outputFile, indent + 1);
                    WRITE("call %s.%s %d\n", compiler->className, name, compiler->numExpressions)
                    advance(compiler);
                    break;
                case '[' :
                    //writeToken(compiler->tokens, outputFile, indent + 1);
                    entry = getEntry(compiler, name);
                    if (entry != NULL)
                        WRITE("push %s %d\n", getSymbolSegment(entry->segment), entry->offset)
                    else ASSERT(0==1, "could not find symbol in table")
                    advance(compiler);
                    compileExpression(compiler);
                    ASSERT(currentSymbolEQ(compiler, ']'), "']' expected after expression")
                    WRITE("add\n")
                    WRITE("pop pointer 1\n")
                    WRITE("push that 0\n")
                    //writeToken(compiler->tokens, outputFile, indent + 1);
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
                //writeToken(compiler->tokens, outputFile, indent + 1);
                advance(compiler);
            } else if (isUnaryOp(currentSymbol(compiler))) { 
                char op = currentSymbol(compiler);
                advance(compiler);
                compileTerm(compiler);
                WRITE("%s\n", getUnaryArithCommand(op)) 
            } 
            break;

        case KEYWORD :
            if (STREQUALS(currentTokenWord(compiler), "null") || STREQUALS(currentTokenWord(compiler), "false"))
                WRITE("push constant 0\n")
            else if (STREQUALS(currentTokenWord(compiler), "true")) {
                WRITE("push constant 1\n")
                WRITE("neg\n")
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
                WRITE("call String.appendChar 2")
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
    //writeTag("</term>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Terminal\n");
}

//********************************************************************************************************************//
//* Function compilesubroutineCall                                                                                   *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts subrioutineCall into xml format                                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineCall(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Subroutine Call\n");
    //writeToken(compiler->tokens, outputFile, indent + 1);
    char *first = currentTokenWord(compiler);
    char *second = NULL;
    advance(compiler);
    //writeToken(compiler->tokens, outputFile, indent + 1);
    if (currentSymbolEQ(compiler, '.')) {
        advance(compiler);
        //writeToken(compiler->tokens, outputFile, indent + 1);
        second = currentTokenWord(compiler);
        advance(compiler);
        ASSERT(currentSymbolEQ(compiler, '('), "'(' expected after expression list")
        //writeToken(compiler->tokens, outputFile, indent + 1);
    }
    advance(compiler);
    SymbolEntry *entry = getEntry(compiler, first);
    if (entry != NULL && second != NULL)
        WRITE("push %s %d\n", getSymbolSegment(entry->segment), entry->offset)
    if (second == NULL)
        WRITE("push pointer 0\n")

    compileExpressionList(compiler);
    ASSERT(currentSymbolEQ(compiler, ')'), "')' expected after expression list")
    //writeToken(compiler->tokens, outputFile, indent + 1);
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
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts expression list (x, y) into xml format                                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileExpressionList(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Compiling Expression List\n");
    //writeTag("<expressionList>", outputFile, indent);
    compiler->numExpressions = 0;
    while (currentSymbol(compiler) != ')') {
        compiler->numExpressions++;
        compileExpression(compiler);

        if (currentSymbolEQ(compiler, ',')) {
            //writeToken(compiler->tokens, outputFile, indent + 1);
            advance(compiler);
        }
    }
    //writeTag("</expressionList>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Compiling Expression List\n");
}

//********************************************************************************************************************//
//* Helper Function advance                                                                                          *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*     increments compiler->tokens iterator by one                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void advance(Compiler *compiler) {
    compiler->tokens->iter++;
}

//********************************************************************************************************************//
//* Helper Function currentsymbol                                                                                    *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*     Returns:                                                                                                     *//
//*         char -- current symbol in char format                                                                    *//
//********************************************************************************************************************//
char currentSymbol(Compiler *compiler) {
    return compiler->tokens->list[compiler->tokens->iter]->name[0];
}

//********************************************************************************************************************//
//* Helper Function currentSymbolEQ                                                                                  *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         symbolName: char -- char to compare                                                                      *//
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
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*     Returns:                                                                                                     *//
//*         char * -- current token full name                                                                        *//
//********************************************************************************************************************//
char *currentTokenWord(Compiler *compiler) {
    return compiler->tokens->list[compiler->tokens->iter]->name;
}

//********************************************************************************************************************//
//* Helper Function currentTokenWordEQ                                                                               *//
//*     Input:                                                                                                       *//
//*         compiler->tokens: TokenList * -- List of compiler->tokens                                                                    *//
//*         name: char * -- name to compare                                                                          *//
//*     Returns:                                                                                                     *//
//*         bool -- whether or not current token is equal to parameter                                               *//
//********************************************************************************************************************//
bool currentTokenWordEQ(Compiler *compiler, char *name) {
    return STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, name);
}

SymbolEntry *getEntry(Compiler *compiler, char *name) {
    for (int i = 0; i < compiler->classTable->used; i++)
        if (STREQUALS(compiler->classTable->list[i]->name, name))
            return compiler->classTable->list[i];

    for (int i = 0; i < compiler->subroutineTable->used; i++)
        if (STREQUALS(compiler->subroutineTable->list[i]->name, name))
            return compiler->subroutineTable->list[i];
    return NULL;
}
