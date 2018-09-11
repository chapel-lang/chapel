try:
  # numpy and cython are required for building Python modules
  import numpy
  import cython
  print(False)
except ImportError:
  print(True)
