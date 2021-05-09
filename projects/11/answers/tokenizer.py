import re
import os
import sys

# keywords = ['class', 'constructor', 'function', 
# 			'method', 'field', 'static', 'var', 
# 			'int', 'char', 'boolean', 'void', 
# 			'true', 'false', 'null', 'this', 
# 			'let', 'do', 'if', 'else', 'while', 'return']
# symbols = ['{', '}', '(', ')', '[', ']', '.', 
# 			',', ';', '+', '-', '*', '/', '&', 
# 			', ', '<', '>', '=', '~', '|']
# escapedSymbols = {'<':'&lt;','>':'&gt;','"':'&quot;','&':'&amp;'}

# integerConstants = re.compile(r'\d{1,5}')
# stringConstants = re.compile(r'"(.*?)"')

# identifiers = re.compile(r'[a-zA-Z_]\w*')

class token():
	escapedSymbols = {'<':'&lt;','>':'&gt;','"':'&quot;','&':'&amp;'}
	def __init__(self, kind, value):
		self.kind = kind
		self.value = value
	def __repr__(self):
		if self.kind == 'keyword':
			return f'<keyword> {self.value} </keyword>\n'
		elif self.kind == 'symbol':
			if self.value in self.escapedSymbols.keys():
				return f'<symbol> {self.escapedSymbols[self.value]} </symbol>\n'
			else:
				return f'<symbol> {self.value} </symbol>\n' 
		elif self.kind == 'integerConstant':
			return f'<integerConstant> {self.value} </integerConstant>\n'
		elif self.kind == 'stringConstant':
			return f'<stringConstant> {self.value} </stringConstant>\n'
		elif self.kind == 'identifier':
			return f'<identifier> {self.value} </identifier>\n'
		


class Tokenizer():
	tokenTypes = ['keyword','symbol','identifier','integerConstant','stringConstant']

	keywords = (r'(?P<keyword>class|constructor|function|method|field|static|var|int|'
					r'char|boolean|void|true|false|null|this|let|do|if|else|while|return)'
					r'(?=[^\w])')
	symbols = r'(?P<symbol>[{}()[\].,;+\-*/&|<>=~])'
	integerConstants = r'(?P<integerConstant>\d+)'
	stringConstants	= r'\"(?P<stringConstant>[^\n]*)\"'
	identifiers = r'(?P<identifier>[A-Za-z_]\w*)'

	tokens = r'{}|{}|{}|{}|{}'.format(keywords, symbols, integerConstants,
	stringConstants, identifiers)

	tokenRegex = re.compile(tokens)
	tokenList = []

	classNames = []
	def __init__(self, inputFile):
		self.inputFile = inputFile.read()
		self.tokens = self.tokenizer()

	def doublePeekNextToken(self):
		if len(self.tokens) > 1:
			return self.tokens[1]
		return None
	def peekNextToken(self):
		if (self.hasNextToken()):
			return self.tokens[0]
		else:
			return None
	def getNextToken(self):
		if (self.hasNextToken()):
			return self.tokens.pop(0)
		else:
			return None
	def hasNextToken(self):
		print(len(self.tokens))
		if len(self.tokens) == 0:
			return False
		return True

	def tokenizer(self):
		noComments = self.removeComments()
		matches = self.tokenRegex.findall(noComments)
		match = self.tokenRegex.finditer(noComments)
		tokenList =[]
		# Base on tokenizer from https://docs.python.org/3/library/re.html
		for matchObject in self.tokenRegex.finditer(noComments):
			kind = matchObject.lastgroup
			value = matchObject.group()
			tokenList.append(token(kind, value))
		return tokenList

	def removeComments(self):
		multiLine = re.sub(r"\/\*.*?\*\/", ' ', self.inputFile, flags = re.DOTALL)
		singleLine = re.sub(r"\/\/.*?\n", '\n', multiLine)
		return singleLine
				