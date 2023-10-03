#
# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
# Copyright 2004-2019 Cray Inc.
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

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
LDFLAGS += ["-L{}".format(chpl_lib_path), "-lChplFrontend", "-Wl,-rpath", chpl_lib_path]

setup(name = "chapel",
      version = "0.1",
      ext_modules = [Extension("chapel.core", ["chapel.cpp"], extra_compile_args = CXXFLAGS, extra_link_args=LDFLAGS)]
      )