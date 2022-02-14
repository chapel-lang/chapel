# Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

# TODO compile_commands missing stddef for some reason
# run with ./fieldsUsed --extra-arg-before=-I/usr/lib64/clang/13.0.0/include ../../lib/queries/ID.cpp
# TODO switch to cmake
opt=-O2
# san=-fsanitize=address -fno-omit-frame-pointer
libs=-lclang-cpp -lLLVM-13 -lLLVMSupport
fieldsUsed: fieldsUsed.cpp
	clang++ -std=c++14 $(opt) $(san) -g $< -o $@ $(libs)
