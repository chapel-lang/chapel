# Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

target_sources(ChplFrontend-obj
               PRIVATE

               # C sources
               my_aligned_alloc.h
               my_aligned_alloc.c
               my_strerror_r.h
               my_strerror_r.c

               # C++ sources
               assertions.cpp
               bitmap.cpp
               break.cpp
               chplenv.cpp
               clang-integration.cpp
               filesystem.cpp
               filesystem_help.h
               printf.cpp
               string-escapes.cpp
               subprocess.cpp
               terminal.cpp
               version-info.cpp
               filtering.cpp
              )
