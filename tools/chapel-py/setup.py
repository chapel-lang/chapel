from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import subprocess
import os
import sys

CXXFLAGS = []
CXXFLAGS += ["-Wno-c99-designator"]
CXXFLAGS += subprocess.check_output(["llvm-config", "--cxxflags"]).decode(sys.stdout.encoding).strip().split(" ")
CXXFLAGS += ["-std=c++17", "-I/Users/daniel.fedorin/Documents/software/chapel/frontend/include"]

LDFLAGS = []
LDFLAGS += ["-L/Users/daniel.fedorin/Documents/software/chapel/lib/compiler/darwin-arm6", "-lChplFrontend"]

setup(name = "chapel",
      version = "0.1",
      ext_modules = [Extension("chapel", ["test.cpp"], extra_compile_args = CXXFLAGS, extra_link_args=LDFLAGS)]
      );
