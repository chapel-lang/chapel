# Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
    addAutoDestroyCalls.cpp
    AutoDestroyScope.cpp
    caches.cpp
    callDestructors.cpp
    callInfo.cpp
    cleanups.cpp
    cullOverReferences.cpp
    expandVarArgs.cpp
    fcf-support.cpp
    fixupExports.cpp
    functionResolution.cpp
    generics.cpp
    implementForallIntents.cpp
    initializerResolution.cpp
    interfaceResolution.cpp
    lateConstCheck.cpp
    lifetime.cpp
    loopDetails.cpp
    lowerForalls.cpp
    lowerIterators.cpp
    lowerLoopContexts.cpp
    nilChecking.cpp
    postFold.cpp
    preFold.cpp
    ResolutionCandidate.cpp
    resolveFunction.cpp
    tuples.cpp
    typeSpecifier.cpp
    virtualDispatch.cpp
    visibleFunctions.cpp
    wrappers.cpp
   )
add_compiler_sources("${SRCS}" "${CMAKE_CURRENT_SOURCE_DIR}")
