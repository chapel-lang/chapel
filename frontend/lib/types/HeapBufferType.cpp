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

#include "chpl/types/HeapBufferType.h"

#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/intents.h"
#include "chpl/types/Param.h"
#include "chpl/types/VoidType.h"
#include "chpl/resolution/can-pass.h"

namespace chpl {
namespace types {

const owned<HeapBufferType>& HeapBufferType::getHeapBufferType(Context* context,
                                             const HeapBufferType* instantiatedFrom,
                                             const Type* eltType) {
  QUERY_BEGIN(getHeapBufferType, context, instantiatedFrom, eltType);
  auto result = toOwned(new HeapBufferType(instantiatedFrom, eltType));
  return QUERY_END(result);
}

const HeapBufferType* HeapBufferType::get(Context* context) {
  return HeapBufferType::getHeapBufferType(context,
                                           /* instantiatedFrom */ nullptr,
                                           /* eltType */ nullptr)
      .get();
}

const HeapBufferType* HeapBufferType::get(Context* context,
                                          const Type* eltType) {
  return HeapBufferType::getHeapBufferType(
             context,
             /* instantiatedFrom */ HeapBufferType::get(context), eltType)
      .get();
}

const ID& HeapBufferType::getId(Context* context) {
  QUERY_BEGIN(getId, context);
  ID result =
      parsing::getSymbolIdFromTopLevelModule(context, "ChapelBase", "_ddata");
  return QUERY_END(result);
}

void HeapBufferType::stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const {
  ss << "_ddata";

  if (eltType_) {
    ss << "(";
    eltType_->stringify(ss, stringKind);
    ss << ")";
  }
}

} // end namespace types
} // end namespace chpl
