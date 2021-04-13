#include "analyzer.h"

void analyzeClass(TokenList *tokens, FILE *outputFile) {
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
}

void analyzeClassVarDec(TokenList *tokens, FILE *outputFile, int indent) {
    writeTag("<classVarDec>", outputFile, indent);
    while (currentSymbol(tokens) != ';') {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }
    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after class var declaration")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeTag("</classVarDec>", outputFile, indent);
    
}

void analyzeSubroutineDec(TokenList *tokens, FILE *outputFile, int indent) {
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

}

void analyzeSubroutineBody(TokenList *tokens, FILE *outputFile, int indent) {
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

}

void analyzeParameterList(TokenList *tokens, FILE *outputFile, int indent) {
    writeTag("<parameterList>", outputFile, indent);
    while (currentSymbol(tokens) != ')') {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }
    writeTag("</parameterList>", outputFile, indent);
}

void analyzeVarDec(TokenList *tokens, FILE *outputFile, int indent) {
    writeTag("<varDec>", outputFile, indent);
    while (currentSymbol(tokens) != ';') {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
    }

    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after class var declaration")
    writeToken(tokens, outputFile, indent + 1);

    writeTag("</varDec>", outputFile, indent);

}

void analyzeStatements(TokenList *tokens, FILE *outputFile, int indent) {
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

}

void analyzeDo(TokenList *tokens, FILE *outputFile, int indent) {
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

}

void analyzeLet(TokenList *tokens, FILE *outputFile, int indent) {
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
    //printf("WORD: %s\n", currentTokenWord(tokens));
    ASSERT(currentSymbolEQ(tokens, ';'), "';' expected after let statement 1")
    writeToken(tokens, outputFile, indent + 1);
    advance(tokens);
    writeTag("</letStatement>", outputFile, indent);

}

void analyzeWhile(TokenList *tokens, FILE *outputFile, int indent) {
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

}

void analyzeReturn(TokenList *tokens, FILE *outputFile, int indent) {
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

}

void analyzeIf(TokenList *tokens, FILE *outputFile, int indent) {
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

}

void analyzeExpression(TokenList *tokens, FILE *outputFile, int indent) {
    writeTag("<expression>", outputFile, indent);
    analyzeTerm(tokens, outputFile, indent + 1);
    while (isOperator(CURR_WORD)) {
        writeToken(tokens, outputFile, indent + 1);
        advance(tokens);
        analyzeTerm(tokens, outputFile, indent);
    }        
    writeTag("</expression>", outputFile, indent);

}

void analyzeTerm(TokenList *tokens, FILE *outputFile, int indent) {
    writeTag("<term>", outputFile, indent);
    writeToken(tokens, outputFile, indent + 1);
           // printf("CURR WORD: %s\n", currentTokenWord(tokens));
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
}

void analyzeSubroutineCall(TokenList *tokens, FILE *outputFile, int indent) {
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
}

void analyzeExpressionList(TokenList *tokens, FILE *outputFile, int indent) {
    writeTag("<expressionList>", outputFile, indent);
    while (currentSymbol(tokens) != ')') {
        analyzeExpression(tokens, outputFile, indent + 1);

        if (currentSymbolEQ(tokens, ',')) {
            writeToken(tokens, outputFile, indent + 1);
            advance(tokens);
        }
    }
    writeTag("</expressionList>", outputFile, indent);
}

void writeToken(TokenList *tokens, FILE *outputFile, int indent) {
    char *name = tokens->list[tokens->iter]->name;
    char *type = getTokenType(tokens->list[tokens->iter]->type);
    for (int i = 0; i < indent; i++)
        WRITE("\t")
    WRITE("<%s> %s </%s>\n", type, name, type)
}

void writeTag(char *tag, FILE *outputFile, int indent) {
    for (int i = 0; i < indent; i++)
        WRITE("\t")
    WRITE("%s\n", tag)
}

void advance(TokenList *tokens) {
    tokens->iter++;
}

char currentSymbol(TokenList *tokens) {
    return tokens->list[tokens->iter]->name[0];
}

bool currentSymbolEQ(TokenList *tokens, char symbolName) {
    ASSERT(tokens->list[tokens->iter]->type == SYMBOL, "current token is expected to be SYMBOL but is not")
    return tokens->list[tokens->iter]->name[0] == symbolName;
}

char *currentTokenWord(TokenList *tokens) {
    return tokens->list[tokens->iter]->name;
}

bool currentTokenWordEQ(TokenList *tokens, char *name) {
    return STREQUALS(tokens->list[tokens->iter]->name, name);
}
