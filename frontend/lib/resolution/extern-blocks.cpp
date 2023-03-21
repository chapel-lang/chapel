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

#include "extern-blocks.h"

#include "chpl/util/clang-integration.h"

namespace chpl {
namespace resolution {


using namespace util;

bool externBlockContainsName(Context* context,
                             ID externBlockId,
                             UniqueString name) {
  const owned<TemporaryFileResult>& tfs =
    createClangPrecompiledHeader(context, externBlockId);
  const TemporaryFileResult* ptr = tfs.get();
  if (ptr != nullptr && precompiledHeaderContainsName(context, ptr, name)) {
    return true;
  }

  return false;
}


} // end namespace resolution
} // end namespace chpl
