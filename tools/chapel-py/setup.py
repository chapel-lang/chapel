from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import os
import sys

chpl_home = os.getenv('CHPL_HOME')
chpl_printchplenv = os.path.join(chpl_home, "util", "printchplenv")
chpl_variables_lines = subprocess.check_output([chpl_printchplenv, "--internal", "--all", " --anonymize"]).decode(sys.stdout.encoding).strip().splitlines()
chpl_variables = dict()
for line in chpl_variables_lines:
    elms = line.split(":", maxsplit=1)
    if len(elms) == 2:
        chpl_variables[elms[0].strip()] = elms[1].strip()

llvm_config = chpl_variables.get("CHPL_LLVM_CONFIG")

chpl_lib_path = os.path.join(chpl_home, "lib", "compiler", chpl_variables.get("CHPL_HOST_BIN_SUBDIR"))

CXXFLAGS = []
CXXFLAGS += ["-Wno-c99-designator"]
CXXFLAGS += subprocess.check_output([llvm_config, "--cxxflags"]).decode(sys.stdout.encoding).strip().split(" ")
CXXFLAGS += ["-std=c++17", "-I{}/frontend/include".format(chpl_home)]

LDFLAGS = []
LDFLAGS += ["-L{}".format(chpl_lib_path), "-lChplFrontend"]

setup(name = "chapel",
      version = "0.1",
      ext_modules = [Extension("chapel.core", ["chapel.cpp"], extra_compile_args = CXXFLAGS, extra_link_args=LDFLAGS)]
      )
