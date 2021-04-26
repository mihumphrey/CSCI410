#include "analyzer.h"
#include "symbolTable.h"

extern bool verbose;

//********************************************************************************************************************//
//* Function analyzeClass                                                                                            *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                     *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*     Converts entire class into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeClass(TokenList *tokens, FILE *outputFile) {
    
    
    if (verbose)
        fprintf(stderr, "\t\t* Beginning Analyzing\n");
    int indentLevel = 0;
    ASSERT(currentTokenWordEQ(tokens, "class"), "\"class\" expected as first token")
    writeTag("<class>", outputFile, indentLevel);
    
    for (int i = 0; i < CLASS_DEC_VARS; i++, advance(tokens)) {
        writeToken(tokens, outputFile, indentLevel + 1);
    }

    while (isClassVarOpening(tokens->list[tokens->iter]->name)) {
        analyzeClassVarDec(tokens, outputFile, indentLevel + 1);
    }

    while (isSubroutineOpening(tokens->list[tokens->iter]->name)) {
        analyzeSubroutineDec(tokens, outputFile, indentLevel + 1);
    }    
    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected after to close class dec")
    writeToken(tokens, outputFile, indentLevel + 1);
    advance(tokens);
    writeTag("</class>", outputFile, indentLevel);
    if (verbose)
        fprintf(stderr, "\t\t* Done Analyzing\n");
}

//********************************************************************************************************************//
//* Function analyzeClassVarDec                                                                                      *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                     *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts field/static var decs into xml format                                                               *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeClassVarDec(TokenList *tokens, FILE *outputFile, int indent) {
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
//* Function analyzeSubroutineDec                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts functions/constructors into xml format                                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeSubroutineDec(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Subroutine Declaration\n");
    writeTag("<subroutineDec>", outputFile, indent);
    for (int i = 0; i < SUBROUTINE_DEC_TOKENS; i++, advance(tokens)) {
        writeToken(tokens, outputFile, indent + 1);
    }
    analyzeParameterList(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected at end of parameter list")
    writeToken(tokens, outputFile, indent + 1); 
    advance(tokens);
    analyzeSubroutineBody(tokens, outputFile, indent + 1);
    writeTag("</subroutineDec>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Subroutine Declaration\n");
}

//********************************************************************************************************************//
//* Function analyzeSubroutineBody                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts body of subroutine into xml format                                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeSubroutineBody(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Subroutine Body\n");
    writeTag("<subroutineBody>", outputFile, indent);
    ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected after parameter list")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    while (currentTokenWordEQ(tokens, "var")) {
        analyzeVarDec(tokens, outputFile, indent + 1);
        advance(tokens);
    }
    analyzeStatements(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected to close subroutine")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeTag("</subroutineBody>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Subroutine Body\n");
}

//********************************************************************************************************************//
//* Function analyzeParameterList                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts parameter list (int x, int y, etc) into xml format                                                  *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeParameterList(TokenList *tokens, FILE *outputFile, int indent) {
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
//* Function analyzeVarDec                                                                                           *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts var decs into xml format                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeVarDec(TokenList *tokens, FILE *outputFile, int indent) {
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
//* Function analyzeStatements                                                                                       *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts all statements into xml format                                                                      *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeStatements(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Statements\n");
    writeTag("<statements>", outputFile, indent);
    while (isStatementOpening(tokens->list[tokens->iter]->name)) {
        if (STREQUALS(tokens->list[tokens->iter]->name, "do"))
            analyzeDo(tokens, outputFile, indent + 1);
        else if (STREQUALS(tokens->list[tokens->iter]->name, "if"))
            analyzeIf(tokens, outputFile, indent + 1);
        else if (STREQUALS(tokens->list[tokens->iter]->name, "let"))
            analyzeLet(tokens, outputFile, indent + 1);
        else if (STREQUALS(tokens->list[tokens->iter]->name, "return"))
            analyzeReturn(tokens, outputFile, indent + 1);
        else if (STREQUALS(tokens->list[tokens->iter]->name, "while"))
            analyzeWhile(tokens, outputFile, indent + 1);
    }
    writeTag("</statements>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Statements\n");
}

//********************************************************************************************************************//
//* Function analyzeDo                                                                                               *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts do statement into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeDo(TokenList *tokens, FILE *outputFile, int indent) {
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

    analyzeExpressionList(tokens, outputFile, indent + 1);

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
//* Function analyzeLet                                                                                              *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts let statement into xml format                                                                       *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeLet(TokenList *tokens, FILE *outputFile, int indent) {
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
    if (tokens->list[tokens->iter - 1]->name[0] == '[') {
        analyzeExpression(tokens, outputFile, indent + 1);
        ASSERT(currentSymbolEQ(tokens, ']'), "']' expected after end of expression")
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
        ASSERT(currentSymbolEQ(tokens, '='), "'=' expected after end of expression")
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    } 
    analyzeExpression(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after let statement 1")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeTag("</letStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Let Statement\n");
}

//********************************************************************************************************************//
//* Function analyzeWhile                                                                                            *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts while statement into xml format                                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeWhile(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing While Statement\n");
    writeTag("<whileStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(tokens, "while"), "'while' keyword expected")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '('), "'(' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    analyzeExpression(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    analyzeStatements(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected in while statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    writeTag("</whileStatement>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing While Statement\n");
}

//********************************************************************************************************************//
//* Function analyzeReturn                                                                                           *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts return statement into xml format                                                                    *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeReturn(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Return Statement\n");
    writeTag("<returnStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(tokens, "return"), "'return' keyword expected")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    if (currentSymbol(tokens) != ';') {
        analyzeExpression(tokens, outputFile, indent + 1);
    }
    
    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after let statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    writeTag("</returnStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Return Statement\n");
}

//********************************************************************************************************************//
//* Function analyzeIf                                                                                               *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts if statement into xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeIf(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing If Statement\n");
    writeTag("<ifStatement>", outputFile, indent);
    ASSERT(currentTokenWordEQ(tokens, "if"), "'if' keyword expected")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '('), "'(' expected in if statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    analyzeExpression(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected in if statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected in if statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    analyzeStatements(tokens, outputFile, indent + 1);

    ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected in if statement")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);

    if (currentTokenWordEQ(tokens, "else")) {
        ASSERT(currentTokenWordEQ(tokens, "else"), "'else' keyword expected")
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);

        ASSERT(currentSymbolEQ(tokens, '{'), "'{' expected in if statement")
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);

        analyzeStatements(tokens, outputFile, indent + 1);

        ASSERT(currentSymbolEQ(tokens, '}'), "'}' expected in if statement")
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }
    writeTag("</ifStatement>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing If Statement\n");
}

//********************************************************************************************************************//
//* Function analyzeExpression                                                                                       *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts nested expression into xml format                                                                   *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeExpression(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Expression\n");
    writeTag("<expression>", outputFile, indent);
    analyzeTerm(tokens, outputFile, indent + 1);
    while (isOperator(CURR_WORD)) {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
        analyzeTerm(tokens, outputFile, indent);
    }        
    writeTag("</expression>", outputFile, indent);

    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Expression\n");
}

//********************************************************************************************************************//
//* Function analyzeTerm                                                                                             *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts terminal into xml format                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeTerm(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Terminal\n");
    writeTag("<term>", outputFile, indent);
    writeToken(tokens, outputFile, indent + 1);
    switch(tokens->list[tokens->iter ]->type) {
        case IDENTIFIER : 
            advance(tokens);
            ASSERT(tokens->list[tokens->iter]->type == SYMBOL, "expected symbol")         
            switch (currentSymbol(tokens)) {
                case '.' :
                    writeToken(tokens, outputFile, indent + 1);
                    advance(tokens);
                    analyzeSubroutineCall(tokens, outputFile, indent);
                    break;
                case '(' :
                    writeToken(tokens, outputFile, indent + 1);
                    advance(tokens);
                    analyzeExpressionList(tokens, outputFile, indent + 1);
                    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected after expression list")
                    writeToken(tokens, outputFile, indent + 1);
                    advance(tokens);
                    break;
                case '[' :
                    writeToken(tokens, outputFile, indent + 1);
                    advance(tokens);
                    analyzeExpression(tokens, outputFile, indent + 1);
                    ASSERT(currentSymbolEQ(tokens, ']'), "']' expected after expression")
                    writeToken(tokens, outputFile, indent + 1);
                    advance(tokens);
                    break;
                default :
                    break;
            } 
            break;
        case SYMBOL :
            if (currentSymbolEQ(tokens, '(')) {
                advance(tokens);
                analyzeExpression(tokens, outputFile, indent + 1);
                ASSERT(currentSymbolEQ(tokens, ')'), "')' expected after expression list")
                writeToken(tokens, outputFile, indent + 1);
                advance(tokens);
            } else if (isUnaryOp(currentSymbol(tokens))) { 
                advance(tokens);
                analyzeTerm(tokens, outputFile, indent + 1);
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
    writeTag("</term>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Terminal\n");
}

//********************************************************************************************************************//
//* Function analyzesubroutineCall                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts subrioutineCall into xml format                                                                     *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeSubroutineCall(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Subroutine Call\n");
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeToken(tokens, outputFile, indent + 1);
    if (currentSymbolEQ(tokens, '.')) {
        advance(tokens);
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
        ASSERT(currentSymbolEQ(tokens, '('), "'(' expected after expression list")
        writeToken(tokens, outputFile, indent + 1);
    }
    advance(tokens);
    analyzeExpressionList(tokens, outputFile, indent + 1);
    ASSERT(currentSymbolEQ(tokens, ')'), "')' expected after expression list")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Subroutine Call\n");
}

//********************************************************************************************************************//
//* Function analyzeExpressionList                                                                                   *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts expression list (x, y) into xml format                                                              *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void analyzeExpressionList(TokenList *tokens, FILE *outputFile, int indent) {
    if (verbose)
        fprintf(stderr, "\t\t\t* Analyzing Expression List\n");
    writeTag("<expressionList>", outputFile, indent);
    while (currentSymbol(tokens) != ')') {
        analyzeExpression(tokens, outputFile, indent + 1);

        if (currentSymbolEQ(tokens, ',')) {
            writeToken(tokens, outputFile, indent + 1);
            advance(tokens);
        }
    }
    writeTag("</expressionList>", outputFile, indent);
    if (verbose)
        fprintf(stderr, "\t\t\t* Done Analyzing Expression List\n");
}

//********************************************************************************************************************//
//* Helper Function writeToken                                                                                       *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Converts the token into xml format, and writes it                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void writeToken(TokenList *tokens, FILE *outputFile, int indent) {
    char *name = tokens->list[tokens->iter]->name;
    char *type = getTokenType(tokens->list[tokens->iter]->type);
    for (int i = 0; i < indent; i++)
        WRITE("\t")
    WRITE("<%s> %s </%s>\n", type, name, type)
}

//********************************************************************************************************************//
//* Helper Function writeToken                                                                                       *//
//*     Input:                                                                                                       *//
//*         tag: char * --tag to write                                                                               *//
//*         outputFile: FILE * -- file to write to                                                                   *//
//*         indent: int -- amount to indent xml to                                                                   *//
//*     Writes a high scope tag in xml format                                                                        *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void writeTag(char *tag, FILE *outputFile, int indent) {
    for (int i = 0; i < indent; i++)
        WRITE("\t")
    WRITE("%s\n", tag)
}

//********************************************************************************************************************//
//* Helper Function advance                                                                                          *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*     increments tokens iterator by one                                                                            *//
//*     Returns:                                                                                                     *//
//*         void                                                                                                     *//
//********************************************************************************************************************//
void advance(TokenList *tokens) {
    tokens->iter++;
}

//********************************************************************************************************************//
//* Helper Function currentsymbol                                                                                    *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*     Returns:                                                                                                     *//
//*         char -- current symbol in char format                                                                    *//
//********************************************************************************************************************//
char currentSymbol(TokenList *tokens) {
    return tokens->list[tokens->iter]->name[0];
}

//********************************************************************************************************************//
//* Helper Function currentSymbolEQ                                                                                  *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         symbolName: char -- char to compare                                                                      *//
//*     Returns:                                                                                                     *//
//*         bool -- whether or not current symbol is equal to parameter                                              *//
//********************************************************************************************************************//
bool currentSymbolEQ(TokenList *tokens, char symbolName) {
    ASSERT(tokens->list[tokens->iter]->type == SYMBOL, "current token is expected to be SYMBOL but is not")
    return tokens->list[tokens->iter]->name[0] == symbolName;
}

//********************************************************************************************************************//
//* Helper Function currenTokenWord                                                                                  *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*     Returns:                                                                                                     *//
//*         char * -- current token full name                                                                        *//
//********************************************************************************************************************//
char *currentTokenWord(TokenList *tokens) {
    return tokens->list[tokens->iter]->name;
}

//********************************************************************************************************************//
//* Helper Function currentTokenWordEQ                                                                               *//
//*     Input:                                                                                                       *//
//*         tokens: TokenList * -- List of tokens                                                                    *//
//*         name: char * -- name to compare                                                                          *//
//*     Returns:                                                                                                     *//
//*         bool -- whether or not current token is equal to parameter                                               *//
//********************************************************************************************************************//
bool currentTokenWordEQ(TokenList *tokens, char *name) {
    return STREQUALS(tokens->list[tokens->iter]->name, name);
}
