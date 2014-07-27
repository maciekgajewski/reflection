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
		#print("Scope: add, indent=%d, my indent=%d, line=%s" % (indent, self.indent, line))
		if indent <= self.indent:
			self.subscope = None
		if indent == (self.indent+1):
			self.on_line(line)
		else:
			if self.subscope:
				self.subscope.add(line, indent)

	def on_line(self, line):
		#print("Scope, line: %s" % line)
		# try record
		match = re.match(re_record, line)
		if match:
			return self.on_record(match, line)


	def on_record(self, match, line):
		file = match.group(1)
		cos = match.group(2)
		name = match.group(3)

		if file != 'line':
			self.currentFile = os.path.abspath(file)
			#print('current file set to %s' % self.currentFile)

		print('record: %s, indent: %d, my indent=%d, my name=%s' % (name, indent, self.indent, self.name))

		if self.currentFile == None or self.currentFile.startswith(self.sourceDir):
			print('Record: "%s"' % name)

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

	def on_line(self, line):
		#print("Namespace, line=%s" % line)
		# try namespace
		match = re.match(re_namespace, line)
		if match:
			self.on_namespace(match, line)

	def on_namespace(self, match, line):
		file = match.group(1)
		name = match.group(2)

		print('namespace %s: file: %s' % (name, file))

		if file != 'line':
			self.currentFile = os.path.abspath(file)

		if self.currentFile == None or self.currentFile.startswith(self.sourceDir):
			if name not in self.namespaces:
				self.namespaces[name] = Namespace(self.sourceDir, name, self.indent+1)

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
			self.root.add(line[len(indent)-1:], int(len(indent)/2))
		else:
			print("NO INDENT: %s" % line)

	def dump(self, out):
		pass
