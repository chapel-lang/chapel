#!/usr/bin/env python3

import os
import sys

chplenv_dir = os.path.join(os.path.dirname(__file__), '..', '..', '..', 'chplenv')
sys.path.insert(0, os.path.abspath(chplenv_dir))

import chpl_compiler
import compiler_utils


# re2 requires c++11 for std atomics (and possibly other features). Use whether
# std atomics are supported as a rough proxy for whether re2 should build
compiler_val = chpl_compiler.get('target')
std_atomics = compiler_utils.has_std_atomics(compiler_val)

sys.stdout.write('re2' if std_atomics else 'none')
