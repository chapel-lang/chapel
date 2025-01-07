# Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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
set(SRCS
    InitErrorHandling.cpp
    InitNormalize.cpp
    ResolveScope.cpp
    addInitGuards.cpp
    buildDefaultFunctions.cpp
    checkGeneratedAst.cpp
    checkNormalized.cpp
    checkResolved.cpp
    cleanup.cpp
    convert-typed-uast.cpp
    convert-help.cpp
    convert-uast.cpp
    createTaskFunctions.cpp
    denormalize.cpp
    errorHandling.cpp
    expandExternArrayCalls.cpp
    externCResolve.cpp
    flattenClasses.cpp
    flattenFunctions.cpp
    initializerRules.cpp
    insertLineNumbers.cpp
    insertWideReferences.cpp
    normalize.cpp
    normalizeErrors.cpp
    parallel.cpp
    parseAndConvert.cpp
    resolveIntents.cpp
    returnStarTuplesByRefArgs.cpp
    scopeResolve.cpp
    splitInit.cpp
   )
add_compiler_sources("${SRCS}" "${CMAKE_CURRENT_SOURCE_DIR}")
