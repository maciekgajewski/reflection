#!/usr/bin/env python3
#
# Simple reflection generator. works by parsing AST dumped by clang-check
# Requires:
# * Cmake running with -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# * clang-check in path
#
# Usage: reflection_generator.py BUID_DIR INPUT_FILE OUTPUT_FILE
#

import sys
import os
import subprocess
import re

import generator

CLANG_CHECK = 'clang-check'

if len(sys.argv) < 3:
	print("Usage: reflection_generator.py BUID_DIR INPUT_FILE OUTPUT_FILE")
	sys.exit(1)

build_dir = sys.argv[1]
input_file = sys.argv[2]
output_file = sys.argv[3]

# generate temp file
input_file_full_path = os.path.abspath(input_file)

args = [CLANG_CHECK, '-p', build_dir, input_file, '-ast-dump']
print('args=%s' % args)

# invoke clang-check
p = subprocess.Popen(
	args,
	executable = CLANG_CHECK,
	stdout = subprocess.PIPE
	)

# parse ouput
gen = generator.Generator()

cleaner = re.compile('\\033\[[0-9;m]*')
for line in p.stdout:
	# remove terminal control sybols
	clean = re.sub(cleaner, '', line.decode())
	gen.add(clean)

# write output
out = open(output_file, 'w')
gen.dump(out)
