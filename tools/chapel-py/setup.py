from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import os
import sys

chpl_home = os.getenv('CHPL_HOME')

sys.path.append(os.path.join(chpl_home, 'util', 'chplenv'))
import printchplenv

printchplenv.compute_all_values()
printchplenv.compute_internal_values()
chpl_variables = {k.strip():v for k,v in printchplenv.ENV_VALS.items()}

llvm_config = chpl_variables.get("CHPL_LLVM_CONFIG")

chpl_frontend_path = os.path.join(chpl_home, "build", "compiler", chpl_variables.get("CHPL_COMPILER_SUBDIR"))
chpl_lib_path = os.path.join(chpl_frontend_path, "frontend", "lib")
chpl_include_path = os.path.join(chpl_home, "frontend", "include")

CXXFLAGS = []
CXXFLAGS += ["-Wno-c99-designator"]
CXXFLAGS += subprocess.check_output([llvm_config, "--cppflags"]).decode(sys.stdout.encoding).strip().split(" ")
CXXFLAGS += ["-std=c++17", f"-I{chpl_include_path}"]

LDFLAGS = []
LDFLAGS += [f"-L{chpl_lib_path}", "-lChplFrontend"]
LDFLAGS += chpl_variables.get("CHPL_HOST_BUNDLED_LINK_ARGS").split(" ")
LDFLAGS += subprocess.check_output([llvm_config, "--system-libs"]).decode(sys.stdout.encoding).strip().split(" ")

setup(name = "chapel",
      version = "0.1",
      ext_modules = [Extension("chapel.core", ["chapel.cpp"], extra_compile_args = CXXFLAGS, extra_link_args=LDFLAGS)]
      );
