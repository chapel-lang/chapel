/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/Type.h"
#include "chpl/util/clang-integration.h"

namespace chpl {
namespace resolution {

using namespace util;

bool externBlockContainsName(Context* context, ID externBlockId,
                             UniqueString name) {
  if (auto& tfr = createClangPrecompiledHeader(context, externBlockId)) {
    return precompiledHeaderContainsName(context, tfr.get(), name);
  }
  return false;
}

bool externBlockContainsFunction(Context* context, ID externBlockId,
                                 UniqueString name) {
  if (auto& tfr = createClangPrecompiledHeader(context, externBlockId)) {
    return precompiledHeaderContainsFunction(context, tfr.get(), name);
  }
  return false;
}

const types::QualifiedType externBlockTypeForSymbol(Context* context,
                                                    ID externBlockId,
                                                    UniqueString name) {
  if (auto& tfr = createClangPrecompiledHeader(context, externBlockId)) {
    return precompiledHeaderTypeForSymbol(context, tfr.get(), name);
  }
  return types::QualifiedType();
}

const TypedFnSignature* externBlockSigForFn(Context* context, ID externBlockId,
                                            UniqueString name) {
  if (auto& tfr = createClangPrecompiledHeader(context, externBlockId)) {
    ID fnId =
        ID::fabricateId(context, externBlockId, name, ID::ExternBlockElement);
    return precompiledHeaderSigForFn(context, tfr.get(), fnId);
  }
  return nullptr;
}

const types::QualifiedType externBlockRetTypeForFn(Context* context,
                                                   ID externBlockId,
                                                   UniqueString name) {
  if (auto& tfr = createClangPrecompiledHeader(context, externBlockId)) {
    return precompiledHeaderRetTypeForFn(context, tfr.get(), name);
  }

  return types::QualifiedType();
}

}  // end namespace resolution
}  // end namespace chpl
