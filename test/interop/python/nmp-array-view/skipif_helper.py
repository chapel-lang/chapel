import os

try:
  # numpy and cython are required for building Python modules
  import numpy
  import cython

  compiler = os.environ['CHPL_TARGET_COMPILER']
  if (compiler.startswith("cray-prgenv") and compiler != "cray-prgenv-gnu"):
    print(True)
  else:
    print(False)
except ImportError:
  print(True)
