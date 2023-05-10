from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import os
import sys

chpl_home = os.getenv('CHPL_HOME')
chpl_printchplenv = os.path.join(chpl_home, "util", "printchplenv")
chpl_variables = subprocess.check_output([chpl_printchplenv, "--internal"]).decode(sys.stdout.encoding).strip().splitlines()
chpl_lib_path = os.path.join(chpl_home, "lib", "compiler")

chpl_bin_prefix = "CHPL_HOST_BIN_SUBDIR: "
for line in chpl_variables:
    if line.startswith(chpl_bin_prefix):
        chpl_lib_path = os.path.join(chpl_lib_path, line[len(chpl_bin_prefix):])

CXXFLAGS = []
CXXFLAGS += ["-Wno-c99-designator"]
CXXFLAGS += subprocess.check_output(["llvm-config", "--cxxflags"]).decode(sys.stdout.encoding).strip().split(" ")
CXXFLAGS += ["-std=c++17", "-I{}/frontend/include".format(chpl_home)]

LDFLAGS = []
LDFLAGS += ["-L{}".format(chpl_lib_path), "-lChplFrontend"]

setup(name = "chapel",
      version = "0.1",
      ext_modules = [Extension("chapel.core", ["chapel.cpp"], extra_compile_args = CXXFLAGS, extra_link_args=LDFLAGS)]
      );
