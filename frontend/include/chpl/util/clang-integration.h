/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_CLANG_INTEGRATION_H
#define CHPL_UTIL_CLANG_INTEGRATION_H

#include <string>
#include <vector>

namespace chpl {
class Context;

namespace util {


/** Initialize all LLVM targets */
void initializeLlvmTargets();

/** Given arguments to 'clang', convert them into arguments for 'cc1' */
const std::vector<std::string>& getCC1Arguments(Context* context,
                                                std::vector<std::string> args,
                                                bool forGpuCodegen);

/** Given arguments to 'clang' and some code (normally, the contents of an
    extern block), create a precompiled header with clang and return
    its contents. */
const std::string& createClangPrecompiledHeader(Context* context,
                                                std::vector<std::string> args,
                                                std::string code);


} // end namespace util
} // end namespace chpl

#endif
