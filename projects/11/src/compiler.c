#include "compiler.h"

extern bool verbose;

//********************************************************************************************************************//
//* Function compileClass                                                                                            *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                     *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*     Converts entire class into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileClass(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t* Beginning Analyzing\n");
    int indentLevel = 0;
    ASSERT(currentTokenWordEQ(tokens, "class"), "\"class\" expected as first token")
    writeTag("<class>", outputFile, indentLevel);
    
    for (int i = 0; i < CLASS_DEC_VARS; i++, advance(tokens)) {
        writeToken(tokens, outputFile, indentLevel + 1);
    }

    while (isClassVarOpening(compiler->tokens->list[compiler->tokens->iter]->name)) {
        compileClassVarDec(tokens, outputFile, indentLevel + 1);
    }

    while (isSubroutineOpening(compiler->tokens->list[compiler->tokens->iter]->name)) {
        compileSubroutineDec(tokens, outputFile, indentLevel + 1);
    }    
    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected after to close class dec")
    writeToken(tokens, outputFile, indentLevel + 1);
    advance(tokens);
    writeTag("</class>", outputFile, indentLevel);
    if (verbose)
        fprintf(stderr, "\t\t* Done Analyzing\n");
}

//********************************************************************************************************************//
//* Function compileClassVarDec                                                                                      *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                     *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts field/static var decs into xml format                                                               *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileClassVarDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Class Var Declaration\n");
    writeTag("<classVarDec>", outputFile, indent);
    while (currentSymbol(tokens) != ';') {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }
    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after class var declaration")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeTag("</classVarDec>", outputFile, indent);
    
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Class Var Declaration\n");
}

//********************************************************************************************************************//
//* Function compileSubroutineDec                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts functions/constructors into xml format                                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Subroutine Declaration\n");
    writeTag("<subroutineDec>", outputFile, indent);
    for (int i = 0; i < SUBROUTINE_DEC_TOKENS; i++, advance(tokens)) {
        writeToken(tokens, outputFile, indent + 1);
    }
    compileParameterList(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected at end of parameter list")
    writeToken(tokens, outputFile, indent + 1); 
    advance(tokens);
    compileSubroutineBody(tokens, outputFile, indent + 1);
    writeTag("</subroutineDec>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Subroutine Declaration\n");
}

//********************************************************************************************************************//
//* Function compileSubroutineBody                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts body of subroutine into xml format                                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineBody(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Subroutine Body\n");
    writeTag("<subroutineBody>", outputFile, indent);
    ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected after parameter list")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    while (currentTokenWordEQ(tokens, "var")) {
        compileVarDec(tokens, outputFile, indent + 1);
        advance(tokens);
    }
    compileStatements(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected to close subroutine")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeTag("</subroutineBody>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Subroutine Body\n");
}

//********************************************************************************************************************//
//* Function compileParameterList                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts parameter list (int x, int y, etc) into xml format                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileParameterList(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Parameter List\n");
    writeTag("<parameterList>", outputFile, indent);
    while (currentSymbol(tokens) != ')') {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }
    writeTag("</parameterList>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Parameter List\n");
}

//********************************************************************************************************************//
//* Function compileVarDec                                                                                           *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts var decs into xml format                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileVarDec(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Var Declaration\n");
    writeTag("<varDec>", outputFile, indent);
    while (currentSymbol(tokens) != ';') {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }

    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after class var declaration")
    writeToken(tokens, outputFile, indent + 1);

    writeTag("</varDec>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Var Declaration\n");
}

//********************************************************************************************************************//
//* Function compileStatements                                                                                       *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts all statements into xml format                                                                      *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileStatements(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Statements\n");
    writeTag("<statements>", outputFile, indent);
    while (isStatementOpening(compiler->tokens->list[compiler->tokens->iter]->name)) {
        if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "do"))
            compileDo(tokens, outputFile, indent + 1);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "if"))
            compileIf(tokens, outputFile, indent + 1);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "let"))
            compileLet(tokens, outputFile, indent + 1);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "return"))
            compileReturn(tokens, outputFile, indent + 1);
        else if (STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, "while"))
            compileWhile(tokens, outputFile, indent + 1);
    }
    writeTag("</statements>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Statements\n");
}

//********************************************************************************************************************//
//* Function compileDo                                                                                               *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts do statement into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileDo(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Do Statement\n");
    writeTag("<doStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(tokens, "do"), "'do' keyword expected")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    while (currentSymbol(tokens) != '(') {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }

    ASSERT(currentSymbolEQ(tokens, '('), "'(' expected at beginning of expression list")
    writeToken(tokens, outputFile, indent + 1); 
    advance(tokens);

    compileExpressionList(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected at end of expression list")
    writeToken(tokens, outputFile, indent + 1); 
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected at end of statement")
    writeToken(tokens, outputFile, indent + 1); 
    advance(tokens);

    writeTag("</doStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Do Statement\n");
}

//********************************************************************************************************************//
//* Function compileLet                                                                                              *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts let statement into xml format                                                                       *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileLet(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Let Statement\n");
    writeTag("<letStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(tokens, "let"), "'let' keyword expected")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    if (compiler->tokens->list[compiler->tokens->iter - 1]->name[0] == '[') {
        compileExpression(tokens, outputFile, indent + 1);
        ASSERT(currentSymbolEQ(tokens, ']'), "']' expected after end of expression")
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
        ASSERT(currentSymbolEQ(tokens, '='), "'=' expected after end of expression")
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    } 
    compileExpression(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after let statement 1")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeTag("</letStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Let Statement\n");
}

//********************************************************************************************************************//
//* Function compileWhile                                                                                            *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts while statement into xml format                                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileWhile(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing While Statement\n");
    writeTag("<whileStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(tokens, "while"), "'while' keyword expected")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '('), "'(' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    compileExpression(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    compileStatements(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    writeTag("</whileStatement>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing While Statement\n");
}

//********************************************************************************************************************//
//* Function compileReturn                                                                                           *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts return statement into xml format                                                                    *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileReturn(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Return Statement\n");
    //tag
    ASSERT(currentTokenWordEQ(tokens, "return"), "'return' keyword expected")
    //token
    advance(tokens);

    if (currentSymbol(tokens) != ';') {
        compileExpression(tokens, outputFile, indent + 1);
    }
    
    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after let statement")
    //token
    advance(tokens);

    //tag

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Return Statement\n");
}

//********************************************************************************************************************//
//* Function compileIf                                                                                               *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts if statement into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileIf(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing If Statement\n");
    //tag
    ASSERT(currentTokenWordEQ(tokens, "if"), "'if' keyword expected")
    //token
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '('), "'(' expected in if statement")
    //token
    advance(tokens);

    compileExpression(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected in if statement")
    //token
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected in if statement")
    //token
    advance(tokens);

    compileStatements(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected in if statement")
    //token
    advance(tokens);

    if (currentTokenWordEQ(tokens, "else")) {
        ASSERT(currentTokenWordEQ(tokens, "else"), "'else' keyword expected")
    //token
        advance(tokens);

        ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected in if statement")
    //token
        advance(tokens);

        compileStatements(tokens, outputFile, indent + 1);

        ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected in if statement")
    //token
        advance(tokens);
    }
    //tag

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing If Statement\n");
}

//********************************************************************************************************************//
//* Function compileExpression                                                                                       *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts nested expression into xml format                                                                   *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileExpression(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Expression\n");
    //tag
    compileTerm(tokens, outputFile, indent + 1);
    while (isOperator(CURR_WORD)) {
    //token
        advance(tokens);
        compileTerm(tokens, outputFile, indent);
    }        
    //tag

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Expression\n");
}

//********************************************************************************************************************//
//* Function compileTerm                                                                                             *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts terminal into xml format                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileTerm(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Terminal\n");
    //tag
    //token
    switch(compiler->tokens->list[compiler->tokens->iter ]->type) {
        case IDENTIFIER : 
            advance(tokens);
            ASSERT(compiler->tokens->list[compiler->tokens->iter]->type == SYMBOL, "expected symbol")         
            switch (currentSymbol(tokens)) {
                case '.' :
    //token
                    advance(tokens);
                    compileSubroutineCall(tokens, outputFile, indent);
                    break;
                case '(' :
    //token
                    advance(tokens);
                    compileExpressionList(tokens, outputFile, indent + 1);
                    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected after expression list")
    //token
                    advance(tokens);
                    break;
                case '[' :
    //token
                    advance(tokens);
                    compileExpression(tokens, outputFile, indent + 1);
                    ASSERT(currentSymbolEQ(tokens, ']'), "']' expected after expression")
    //token
                    advance(tokens);
                    break;
                default :
                    break;
            } 
            break;
        case SYMBOL :
            if (currentSymbolEQ(tokens, '(')) {
                advance(tokens);
                compileExpression(tokens, outputFile, indent + 1);
                ASSERT(currentSymbolEQ(tokens, ')'), "')' expected after expression list")
    //token
                advance(tokens);
            } else if (isUnaryOp(currentSymbol(tokens))) { 
                advance(tokens);
                compileTerm(tokens, outputFile, indent + 1);
            } 
            break;

        case KEYWORD :
            advance(tokens);
            break;

        case STRING_CONST :
            advance(tokens);
            break;

        case INT_CONST :
            advance(tokens);
            break;
        default :
            ASSERT(0 == 1, "Invalid terminal received")        
    }
    //tag
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Terminal\n");
}

//********************************************************************************************************************//
//* Function compilesubroutineCall                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts subrioutineCall into xml format                                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileSubroutineCall(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Subroutine Call\n");
    // token
    advance(tokens);
    // token
    if (currentSymbolEQ(tokens, '.')) {
        advance(tokens);
    // token
        advance(tokens);
        ASSERT(currentSymbolEQ(tokens, '('), "'(' expected after expression list")
    // token
    }
    advance(tokens);
    compileExpressionList(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected after expression list")
    // token
    advance(tokens);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Subroutine Call\n");
}

//********************************************************************************************************************//
//* Function compileExpressionList                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts expression list (x, y) into xml format                                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void compileExpressionList(Compiler *compiler) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Expression List\n");
    //tag
    while (currentSymbol(tokens) != ')') {
        compileExpression(tokens, outputFile, indent + 1);

        if (currentSymbolEQ(tokens, ',')) {
            //token
            advance(tokens);
        }
    }
    //tag
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Expression List\n");
}

//********************************************************************************************************************//
//* Helper Function advance                                                                                          *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*     increments tokens iterator by one                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void advance(Compiler *compiler) {
    compiler->tokens->iter++;
}

//********************************************************************************************************************//
//* Helper Function currentsymbol                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*     Returns:                                                                                                     *//
//*         char -- current symbol in char format                                                                    *//
//********************************************************************************************************************//
char currentSymbol(Compiler *compiler) {
    return compiler->tokens->list[compiler->tokens->iter]->name[0];
}

//********************************************************************************************************************//
//* Helper Function currentSymbolEQ                                                                                  *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
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
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*     Returns:                                                                                                     *//
//*         char * -- current token full name                                                                        *//
//********************************************************************************************************************//
char *currentTokenWord(Compiler *compiler) {
    return compiler->tokens->list[compiler->tokens->iter]->name;
}

//********************************************************************************************************************//
//* Helper Function currentTokenWordEQ                                                                               *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         name: char * -- name to compare                                                                          *//
//*     Returns:                                                                                                     *//
//*         bool -- whether or not current token is equal to parameter                                               *//
//********************************************************************************************************************//
bool currentTokenWordEQ(Compiler *compiler, char *name) {
    return STREQUALS(compiler->tokens->list[compiler->tokens->iter]->name, name);
}
