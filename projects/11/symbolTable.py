
class SymbolTable():
	"""docstring for SymbolTable"""
	counts = {'static': 0, 'field': 0, 'argument': 0, 'var': 0}
	staticScope = {}

	def __init__(self):
		self.counts['field'] = 0
		self.subroutineScope = {}
		self.fieldScope = {}

	def put(self, name, type, segement):
		id = 0
		if segement == 'arg':
			print('error')
		if segement == 'static':
			id = self.counts['static']
			self.counts['static'] += 1
			self.staticScope[name] = (type, segement, id)
		elif segement == 'field':
			id = self.counts['field']
			self.counts['field'] += 1
			self.fieldScope[name] = (type, segement, id)
		elif segement == 'argument' or segement == 'arg':
			id = self.counts['argument']
			self.counts['argument'] += 1
			self.subroutineScope[name] = (type, segement, id)
		elif segement == 'var':
			id = self.counts['var']
			self.counts['var'] += 1
			self.subroutineScope[name] = (type, segement, id)

	def contains(self, name):
		if name in self.staticScope.keys():
			return True
		elif name in self.subroutineScope.keys():
			return True
		else:
			return False

	def clearLocal(self):
		self.subroutineScope = {}

	def startSubroutine(self):
		self.subroutineScope = {}
		self.counts['argument'] = 0
		self.counts['var'] = 0


	def getCount(self, segement):
		return self.counts[segement]

	def getType(self, name):
		if name in self.subroutineScope.keys():
			return self.subroutineScope[name][0]
		elif name in self.fieldScope.keys():
			return self.fieldScope[name][0]
		elif name in self.staticScope.keys():
			return self.staticScope[name][0]
		else:
			return None
	def getSegement(self, name):
		if name in self.subroutineScope.keys():
			return self.subroutineScope[name][1]
		elif name in self.fieldScope.keys():
			return self.fieldScope[name][1]
		elif name in self.staticScope.keys():
			return self.staticScope[name][1]
		else:
			return None
	def getIndex(self, name):
		if name in self.subroutineScope.keys():
			return self.subroutineScope[name][2]
		elif name in self.fieldScope.keys():
			return self.fieldScope[name][2]
		elif name in self.staticScope.keys():
			return self.staticScope[name][2]
		else:
			return None

	def get(self, name):
		if name in self.subroutineScope.keys():
			return self.subroutineScope[name]
		elif name in self.fieldScope.keys():
			return self.fieldScope[name]
		elif name in self.staticScope.keys():
			return self.staticScope[name]
		else:
			return None