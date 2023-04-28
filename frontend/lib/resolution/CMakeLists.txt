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

               InitResolver.cpp
               Resolver.cpp
               VarScopeVisitor.cpp
               call-init-deinit.cpp
               can-pass.cpp
               copy-elision.cpp
               default-functions.cpp
               disambiguation.cpp
               extern-blocks.cpp
               intents.cpp
               maybe-const.cpp
               prims.cpp
               resolution-error-classes-list.cpp
               resolution-queries.cpp
               resolution-types.cpp
               return-type-inference.cpp
               scope-queries.cpp
               scope-types.cpp
               signature-checks.cpp
               split-init.cpp

              )
