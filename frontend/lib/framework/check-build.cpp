/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */  

#include "chpl/framework/check-build.h"

#include <cstdio>

bool validateCompiledCorrectly(int headersHaveLlvm, int headersLlvmMajor) {
  int cppHaveLlvm=
#ifdef HAVE_LLVM
        1;
#else
        0;
#endif
  int cppLlvmMajor=LLVM_VERSION_MAJOR;

  if (headersHaveLlvm != cppHaveLlvm) {
    const char* headers = headersHaveLlvm ? "with" : "without";
    const char* cpp =  cppHaveLlvm ? "with" : "without";
    fprintf(stderr, "Misconfiguration of the C++ Chapel compiler library\n");
    fprintf(stderr, "  headers in use %s HAVE_LLVM defined\n", headers);
    fprintf(stderr, "  .cpp files compiled %s HAVE_LLVM defined\n", cpp);
    return false;
  }

  if (headersLlvmMajor != cppLlvmMajor) {
    fprintf(stderr, "Misconfiguration of the C++ Chapel compiler library\n");
    fprintf(stderr, "  headers in use have LLVM_VERSION_MAJOR %i\n",
            headersLlvmMajor);
    fprintf(stderr, "  .cpp files have LLVM_VERSION_MAJOR %i\n", cppLlvmMajor);
    return false;
  }

  return true;
}
