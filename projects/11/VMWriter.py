

class VMWriter(object):
	"""docstring for VMWriter"""
	def __init__(self, out):
		self.out = out
	def writePush(self, segement, index):
		self.out.write(f'push {segement} {index}\n')
	def writePop(self, segement, index):
		self.out.write(f'pop {segement} {index}\n')
	def writeCall(self, name, nLocals):
		self.out.write(f'call {name} {nLocals}\n')
	def writeFunction(self, name, nLocals):
		self.out.write(f'function {name} {nLocals}\n')
	def writeArithmetic(self, command):
		self.out.write(f'{command}\n')
	def writeLabel(self, label):
		self.out.write(f'label {label}\n')
	def writeGoto(self, label):
		self.out.write(f'goto {label}\n')
	def writeIf(self, label):
		self.out.write(f'if-goto {label}\n')
	def writeReturn(self):
		self.out.write(f'return\n')
	def close(self):
		self.out.close()

		