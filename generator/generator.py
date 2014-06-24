#!/bin/false

import re
import os

re_indent    = re.compile('^([| -`]*)')
re_namespace = re.compile('^NamespaceDecl .+\<([^:]+):.+> (\w+)$')
re_record =    re.compile('^CXXRecordDecl .+\<([^:]+):.+> (struct|class) (\w+) definition$')

class Scope(object):
	def __init__(self, sourceDir, name, indent):
		self.sourceDir = sourceDir
		self.name = name
		self.indent = indent

		self.subscope = None
		self.currentFile = None

	def add(self, line, indent):
		if self.subscope:
			if indent =< self.indent:
				self.subscope = None
			else:
				self.subscope.add(line):

		# try record
		if indent == self.indent:
			match = re.match(re_record, line)
			if match:
				return self.on_record(match, line)

		return False

	def on_record(self, match, line):
			indent = len(match.group(1))/2
			file = match.group(2)
			cos = match.group(3)
			name = match.group(4)

			if file != 'line':
				self.currentFile = os.path.abspath(file)
				#print('current file set to %s' % self.currentFile)

			print('record: %s, indent: %d, my indent=%d, my name=%s' % (name, indent, self.indent, self.name))

			if indent == self.indent + 1 and (self.currentFile == None or self.currentFile.startswith(self.sourceDir)):
				print('Record: "%s"' % name)
				return True

			return False

class Record(Scope):
	def __init__(self, sourceDir, name, indent):
		Scope.__init__(self, sourceDir, name, indent)
		print('Record namespace "%s": indent: %d' % (name, indent))

class Namespace(Scope):
	def __init__(self, sourceDir, name, indent):
		Scope.__init__(self, sourceDir, name, indent)
		print('Created namespace "%s": indent: %d' % (name, indent))
		self.namespaces = {}

	def add(self, line):
		# pass to sub-scope, if set
		if Scope.add(self, line) == False:
			# try namespace
			match = re.match(re_namespace, line)
			if match:
				self.on_namespace(match, line)


	def on_namespace(self, match, line):
			indent = len(match.group(1))/2
			file = match.group(2)
			name = match.group(3)

			#print('namespace %s: indent: %d, file: %s' % (name, indent, file))

			if file != 'line':
				self.currentFile = os.path.abspath(file)
				#print('current file set to %s' % self.currentFile)


			if indent == self.indent + 1 and (self.currentFile == None or self.currentFile.startswith(self.sourceDir)):
				if name not in self.namespaces:
					self.namespaces[name] = Namespace(self.sourceDir, name, indent)

				self.subscope = self.namespaces[name]



class Generator(object):

	def __init__(self, sourceDir):
		self.sourceDir = os.path.abspath(sourceDir)
		print('source dir: %s' % self.sourceDir)
		self.root = Namespace(sourceDir, '', 0)

	def add(self, line):
		m = re.match(re_indent, line)
		if m:
			indent = m.group(1)
		self.root.add(line[len(indent):], indent/2)

	def dump(self, out):
		pass
