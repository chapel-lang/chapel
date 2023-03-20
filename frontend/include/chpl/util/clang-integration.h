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

#include "chpl/framework/ID.h"
#include "chpl/util/memory.h"

#include <string>
#include <vector>

namespace chpl {
class Context;
class TemporaryFileResult;

namespace util {


/** Return the currently set clang flags */
const std::vector<std::string>& clangFlags(Context* context);

/** Set the clang flags for this revision if they have not been set already */
void setClangFlags(Context* context, std::vector<std::string> clangFlags);

#if 0
/** Initialize all LLVM targets */
void initializeLlvmTargets();

/** Given arguments to 'clang', convert them into arguments for 'cc1' */
const std::vector<std::string>& getCC1Arguments(Context* context,
                                                std::vector<std::string> args,
                                                bool forGpuCodegen);
#endif

/** Given arguments to 'clang' and some code (normally, the contents of an
    extern block), create a precompiled header with clang and return
    its contents. */
const owned<TemporaryFileResult>&
createClangPrecompiledHeader(Context* context, ID externBlockId);


} // end namespace util
} // end namespace chpl

#endif
