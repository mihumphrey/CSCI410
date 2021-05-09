# pip libraries
import sys
import os

# Personal libraries
from symbolTable import SymbolTable
from tokenizer import Tokenizer
from VMWriter import VMWriter

segementCorrections = {'arg':'argument','argument':'argument','static':'static','var':'local','field':'this'}

arithmeticSymbols = {'+':'add', '-':'sub',
				'=':'eq', '>':'gt', '<':'lt',
				'&':'and', '|':'or'}
unarySymbols = {'-':'neg', '~':'not'}
className = ''

ifIndex = 0 
whileIndex = 0


def compileString(tokens, symbols, vmWriter):
	string = tokens.getNextToken().value
	vmWriter.writePush('constant', len(string))
	vmWriter.writeCall('String.new', 1)

	for char in string:
		vmWriter.writePush('constant', ord(char))
		vmWriter.writeCall('String.appendChar', 2)

def compileKeyword(tokens, symbols, vmWriter):
	keyword = tokens.getNextToken().value
	if keyword == 'this':
		vmWriter.writePush('pointer',0)
	else:
		vmWriter.writePush('constant', 0)
		if keyword == 'true':
			vmWriter.writeArithmetic('not')

# <term> =>
#   integerConstant | stringConstant | keywordConstant |
#   <varName> | <varName> '[' <expression> ']' | <subroutineCall> |
#   '(' <expression> ')' | unaryOp <term>
def compileTerm(tokens, symbols, vmWriter):
	# unaryOp <term>
	# print(tokens.peekNextToken())
	if tokens.peekNextToken().value in ['-','~']:
		unaryOp = tokens.getNextToken().value
		compileTerm(tokens, symbols, vmWriter)
		vmWriter.writeArithmetic(unarySymbols[unaryOp])
	elif tokens.peekNextToken().value == '(':
		# '('
		tokens.getNextToken()
		# <expression>
		compileExpression(tokens, symbols, vmWriter)
		#')'
		tokens.getNextToken()
	# integerConstant
	elif tokens.peekNextToken().kind == 'integerConstant':
		vmWriter.writePush('constant', tokens.getNextToken().value)
	# stringConstant 
	elif tokens.peekNextToken().kind == 'stringConstant':
		compileString(tokens, symbols, vmWriter)
	# keywordConstant
	elif tokens.peekNextToken().kind == 'keyword':
		compileKeyword(tokens, symbols, vmWriter)
	# <varName> 
	else:
		# <varName> '[' <expression> ']'
		if tokens.doublePeekNextToken().value =='[':
			arrayVar = tokens.getNextToken().value
			# '['
			tokens.getNextToken()
			# <expression>
			compileExpression(tokens, symbols, vmWriter)
			# ']'
			tokens.getNextToken()

			arrayKind = symbols.getSegement(arrayVar)
			arrayIndex = symbols.getIndex(arrayVar)

			vmWriter.writePush(segementCorrections[arrayKind], arrayIndex)

			vmWriter.writeArithmetic('add')
			vmWriter.writePop('pointer', 1)
			vmWriter.writePush('that', 0)
		# <subroutineCall>
		elif tokens.doublePeekNextToken().value in ['.','(']:
			compileSubroutineCall(tokens, symbols, vmWriter)
		# <varName>
		else:
			var = tokens.getNextToken().value
			print(var)
			varKind = segementCorrections[symbols.getSegement(var)]
			varIndex = symbols.getIndex(var)
			vmWriter.writePush(varKind, varIndex)

# <subroutineCall> =>
#   <subroutineName> '(' <expressionList> ')' |
#   (<className>|<varName>) '.' <subroutineName> '(' <expressionList> ')'
def compileSubroutineCall(tokens, symbols, vmWriter):
	# <subroutineName>  or (<className>|<varName>)
	identifier = tokens.getNextToken().value
	functionName = identifier
	numberOfArgs = 0

	if tokens.peekNextToken().value == '.':
		# "."
		tokens.getNextToken()
		# <subroutineName>
		subroutineName = tokens.getNextToken().value
		
		type = symbols.getType(identifier)

		if type != None:
			instanceKind = symbols.getSegement(identifier)
			instanceIndex = symbols.getIndex(identifier)

			vmWriter.writePush(segementCorrections[instanceKind], instanceIndex)

			functionName = f'{type}.{subroutineName}'
			numberOfArgs +=1
		else:
			classNameCall = identifier
			functionName = f'{classNameCall}.{subroutineName}'
	elif tokens.peekNextToken().value == '(':
		subroutineName = identifier
		global className
		functionName = f'{className}.{subroutineName}'
		numberOfArgs += 1
		vmWriter.writePush('pointer', 0)

	# '('
	tokens.getNextToken()
	# <expression List>
	numberOfArgs += compileExpressionList(tokens, symbols, vmWriter)
	# ')'
	tokens.getNextToken()

	vmWriter.writeCall(functionName, numberOfArgs)


# <expression> =>
#   <term> (<op> <term>)*
def compileExpression(tokens, symbols, vmWriter):
	# <term>
	compileTerm(tokens,symbols, vmWriter)

	# (<op> <term>)*
	while tokens.peekNextToken().value in ['+', '-', '*', '/', '&', '|', '<', '>', '=']:
		# <op>
		op = tokens.getNextToken().value
		# <term>
		compileTerm(tokens,symbols, vmWriter)
		
		if op == '*':
			vmWriter.writeCall('Math.multiply', 2)
		elif op == '/':
			vmWriter.writeCall('Math.divide', 2)
		else:
			vmWriter.writeArithmetic(arithmeticSymbols[op])

# <expressionList> =>
#   (<expression> (',' <expression>)* )?
def compileExpressionList(tokens, symbols, vmWriter):
	numberOfArgs = 0

	if tokens.peekNextToken().value != ')':
		numberOfArgs +=1
		# <expression>
		compileExpression(tokens, symbols, vmWriter)
		while tokens.peekNextToken().value == ',':
			numberOfArgs += 1
			# ','
			tokens.getNextToken()
			# <expression>
			compileExpression(tokens, symbols, vmWriter)
	return numberOfArgs

##<varDec> =>
##  'var' <type> <varName> (',' <varName>)* ';'
def compileVarDec(tokens, symbols, vmWriter):
	# 'var'
	kind = tokens.getNextToken().value
	# <type>
	type = tokens.getNextToken().value
	# <varName> => identifier
	name = tokens.getNextToken().value
	symbols.put(name, type, kind)
	# (',' <varName>)*
	while(tokens.peekNextToken().value == ','):
		# ','
		tokens.getNextToken()

		# <varName> => identifier
		name = tokens.getNextToken().value
		symbols.put(name, type, kind)
		
	# ';'
	tokens.getNextToken()

##<letStatement> =>
##  'let' <varName> ('[' <expression> ']')? '=' <expression> ';'
def compileLetStatement(tokens, symbols, vmWriter):
	# 'let'
	tokens.getNextToken()
	# <varName>
	varName = tokens.getNextToken().value
	varKind = segementCorrections[symbols.getSegement(varName)]
	varIndex = symbols.getIndex(varName)
	# ('[' <expression> ']')?
	if tokens.peekNextToken().value == '[':
		# '['
		tokens.getNextToken()
		# <expression>
		compileExpression(tokens, symbols, vmWriter)
		# ']'
		tokens.getNextToken()
		
		vmWriter.writePush(varKind, varIndex)
		vmWriter.writeArithmetic('add')
		vmWriter.writePop('temp', 0)

		# '='
		tokens.getNextToken()
		
		# <expression>
		compileExpression(tokens, symbols, vmWriter)
		# ';'
		tokens.getNextToken()

		vmWriter.writePush('temp', 0)
		vmWriter.writePop('pointer', 1)
		vmWriter.writePop('that', 0)
	else:
		# '='
		tokens.getNextToken()
		# <expression>
		compileExpression(tokens, symbols, vmWriter)
		# ';'
		tokens.getNextToken()

		vmWriter.writePop(varKind, varIndex)


	

## <ifStatement> =>
##   'if' '(' <expression> ')' '{' <statements> '}'
##   ('else' '{' statements> '}')?
def compileIfStatements(tokens, symbols, vmWriter):
	global ifIndex
	ifIndex+=1
	myIfIndex = ifIndex
	# 'if'
	tokens.getNextToken()

	# '('
	tokens.getNextToken()

	# <expression>
	compileExpression(tokens, symbols, vmWriter)

	# ')'
	tokens.getNextToken()

	# '{'
	tokens.getNextToken()

	vmWriter.writeIf(f'IF_TRUE{myIfIndex}')
	vmWriter.writeGoto(f'IF_FALSE{myIfIndex}')
	vmWriter.writeLabel(f'IF_TRUE{myIfIndex}')

	# <statements>
	compileStatements(tokens, symbols, vmWriter)

	vmWriter.writeGoto(f'IF_END{myIfIndex}')

	# '}'
	tokens.getNextToken()

	vmWriter.writeLabel(f'IF_FALSE{myIfIndex}')
	if tokens.peekNextToken().value == 'else':
		# 'else'
		tokens.getNextToken()
		# '{'
		tokens.getNextToken()
		# <statements>
		compileStatements(tokens, symbols, vmWriter)
		# '}'
		tokens.getNextToken()
	vmWriter.writeLabel(f'IF_END{myIfIndex}')

## <whileStatement> =>
##  'while' '(' <expression> ')' '{' statements> '}'
def compileWhileStatements(tokens, symbols, vmWriter):
	# 'while'
	tokens.getNextToken()
	global whileIndex
	whileIndex+=1
	myWhileIndex = whileIndex

	vmWriter.writeLabel(f'WHILE{myWhileIndex}')
	
	# '('
	tokens.getNextToken()

	# <expression>
	compileExpression(tokens, symbols, vmWriter)
	vmWriter.writeArithmetic('not')

	# ')'
	tokens.getNextToken()

	# '{'
	tokens.getNextToken()
	vmWriter.writeIf(f'WHILE_END{myWhileIndex}')

	# <statements>
	compileStatements(tokens, symbols, vmWriter)

	vmWriter.writeGoto(f'WHILE{myWhileIndex}')
	vmWriter.writeLabel(f'WHILE_END{myWhileIndex}')

	# '}'
	tokens.getNextToken()

## <doStatement> =>
##   'do' <subroutineCall> ';'
def compileDoStatements(tokens, symbols, vmWriter):
	# 'do'
	tokens.getNextToken()

	# <subroutineCall> 
	compileSubroutineCall(tokens, symbols, vmWriter)
	vmWriter.writePop('temp', 0)
	# ';'
	tokens.getNextToken()
	
## <returnStatement> =>
##   'return' <expression>? ';'

def compileReturnStatement(tokens, symbols, vmWriter):
	# 'reutrn'
	tokens.getNextToken()
	
	# <subroutineCall>?
	if tokens.peekNextToken().value != ';':
		compileExpression(tokens, symbols, vmWriter)
	else:
		vmWriter.writePush('constant', 0)

	vmWriter.writeReturn()
	# ';'
	tokens.getNextToken()

##<statement> =>
##  <letStatement> | <ifStatement> | <whileStatement> |
##  <doStatement> | <returnStatement>
def compileStatement(tokens, symbols, vmWriter):

	token = tokens.peekNextToken()
	if token.value == 'let':
		compileLetStatement(tokens, symbols, vmWriter)
	elif token.value == 'if':
		compileIfStatements(tokens, symbols, vmWriter)
	elif token.value == 'while':
		compileWhileStatements(tokens, symbols, vmWriter)
	elif token.value == 'do':
		compileDoStatements(tokens, symbols, vmWriter)
	elif token.value == 'return':
		compileReturnStatement(tokens, symbols, vmWriter)


##<statements> =>
##  <statement>*
def compileStatements(tokens, symbols, vmWriter):

	# <statement>*
	# print(tokens.peekNextToken())
	while tokens.peekNextToken().value in ['if','let','while','do','return']:
		compileStatement(tokens, symbols, vmWriter)


## <subroutineBody> =>
##  '{' <varDec>* <statements> '}'
def compileSubroutineBody(tokens, symbols, vmWriter, subroutineName, subroutineKind):
	# '{'
	tokens.getNextToken()
	# <varDec>*
	while tokens.peekNextToken().value == 'var':

		compileVarDec(tokens, symbols, vmWriter)

	functionName = f'{className}.{subroutineName}'
	numLocals = symbols.getCount('var')
	vmWriter.writeFunction(functionName, numLocals)

	if subroutineKind == 'constructor':
		numFields = symbols.getCount('field')
		vmWriter.writePush('constant', numFields)
		vmWriter.writeCall('Memory.alloc', 1)
		vmWriter.writePop('pointer', 0)
	elif subroutineKind == 'method':
		vmWriter.writePush('argument', 0)
		vmWriter.writePop('pointer', 0)

	# <statements>
	compileStatements(tokens, symbols, vmWriter)

	# '}'
	tokens.getNextToken()

##  <parameterList> =>
##  	(<type> <varName> (',' <type> <varName>)* )?
def compileParameterList(tokens, symbols, vmWriter):
	if tokens.peekNextToken().value != ')':

		# <type>
		type = tokens.getNextToken().value

		# <varName> => identifier
		name = tokens.getNextToken().value

		symbols.put(name, type, 'argument')
		# (',' <type> <varName>)*
		while(tokens.peekNextToken().value == ','):
			# ','
			tokens.getNextToken()

			# <type>
			type = tokens.getNextToken().value

			# <varName> => identifier
			name = tokens.getNextToken().value

			symbols.put(name, type, 'argument')

## <subroutineDec> =>
##  ('constructor' | 'function' | 'method')
##  ('void' | <type>) <subroutineName> '(' <parameterList> ')'
##  <subroutineBody>
def compileSubroutineDec(tokens, symbols, vmWriter):

	# ('constructor' | 'function' | 'method')
	subroutineKind = tokens.getNextToken().value
	# ('void' | <type>)
	tokens.getNextToken()
	# <subroutineName>
	subroutineName = tokens.getNextToken().value

	symbols.startSubroutine()

	if subroutineKind == 'method':
		symbols.put('instance', className, 'argument')
	# '('
	tokens.getNextToken()
	# <parameterList>
	compileParameterList(tokens, symbols, vmWriter)
	# ')'
	tokens.getNextToken()

	# <subroutineBody>
	compileSubroutineBody(tokens, symbols, vmWriter, subroutineName, subroutineKind)

## <classVarDec> =>
## ('static' | 'field') <type> <varName> (',' <varName>)* ';'
def compileClassVarDec(tokens, symbols, vmWriter):

	# ('static' | 'field')
	segement = tokens.getNextToken().value
	# <type>
	type = tokens.getNextToken().value
	# <varName>
	name = tokens.getNextToken().value

	symbols.put(name, type, segement)

	# (',' <varName>)*
	while(tokens.peekNextToken().value == ','):
		#  ,
		tokens.getNextToken()
		# varName
		name = tokens.getNextToken().value

		symbols.put(name, type, segement)
	# ';'
	tokens.getNextToken()
## <class> =>
##  'class' <className> '{' <classVarDec>* <subroutineDec>* '}'
def compileClass(tokens, symbols, vmWriter):
	# 'class'
	tokens.getNextToken()
	# <className>
	token = tokens.getNextToken()
	global className
	className = token.value
	# '{'
	token = tokens.getNextToken()

	# <classVarDec>*
	while tokens.peekNextToken().value == 'field' or tokens.peekNextToken().value == 'static':
		compileClassVarDec(tokens, symbols, vmWriter)
	# <subroutineDec>*
	while (tokens.peekNextToken().value == 'constructor' 
			or tokens.peekNextToken().value == 'function' 
			or tokens.peekNextToken().value == 'method'):
		compileSubroutineDec(tokens, symbols, vmWriter)
		
	# }
	token = tokens.getNextToken()

def compileFile(tokens, symbols, vmWriter):
	compileClass(tokens, symbols, vmWriter)

def tokenizeFile(fileName):
	tokens = Tokenizer(fileName)
	return tokens


def main():
	filePath = sys.argv[1]

	if filePath.endswith('.jack'):
		f = open(filePath)
		out = open(filePath.split('.')[0]+'.vm','w')

		tokens = tokenizeFile(f)
		symbols = SymbolTable()
		vmWriter = VMWriter(out)

		compileFile(tokens, symbols, vmWriter)
	else:
		fileDir = os.listdir(filePath)
		filePath = os.path.normpath(filePath)
		fileName = os.path.basename(os.path.normpath(filePath))
		print(fileName)
		# if not os.path.exists(filePath+"/"+'compile'):
		# 	os.makedirs(filePath+"/"+'compile')
		for file in fileDir:
			if file.endswith('.jack'):
				f = open(filePath+'/'+file)
				out = open(filePath+'/'+file.split('.')[0]+'.vm','w')

				tokens = tokenizeFile(f)
				symbols = SymbolTable()
				vmWriter = VMWriter(out)

				compileFile(tokens, symbols, vmWriter)
				# tokens = tokenList()
				# out = open(filePath+"/compile/"+file.split('.')[0]+'T.xml','w')
				# tokenizeFile(filePath+"/"+file, out, tokens)



if __name__ == '__main__':
	main()