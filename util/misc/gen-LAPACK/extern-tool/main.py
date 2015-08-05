#!/usr/bin/python
import Passes
from Passes import *
from xmltool import *
import sys

root = None

def main():
  if len( sys.argv ) < 2:
    print "Path to LAPACK required"
    exit(-1)
  elif len( sys.argv ) > 2:
    print "Too many arguments. Only path to LAPACK required"
    exit(-1)
    
  Passes.lapack_root = sys.argv[1]
  Passes.lapack__root = "../LAPACK"
  Passes.lapack_src = Passes.lapack_root + "/SRC"
  Passes.lapack_matgen_src = Passes.lapack_root + "/TESTING/MATGEN"
  Passes.lapack_install_src = Passes.lapack_root + "/INSTALL"
  Passes.lapacke_include = Passes.lapack_root + "/lapacke/include"
  Passes.blas_src = Passes.lapack_root + "/BLAS/SRC"
  print Passes.lapack_root
  print Passes.lapack_src
  root = ET.Element( "root" );
  DumpCodePass.apply( root )

if __name__ == "__main__": main()