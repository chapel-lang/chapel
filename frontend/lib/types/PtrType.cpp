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

#include "chpl/types/PtrType.h"

#include "chpl/resolution/can-pass.h"

namespace chpl {
namespace types {

bool PtrType::isEltTypeInstantiationOf(Context* context,
                                       const PtrType* other) const {
  auto r =
      resolution::canPass(context, QualifiedType(QualifiedType::TYPE, eltType_),
                          QualifiedType(QualifiedType::TYPE, other->eltType_));
  // instantiation and same-type passing are allowed here
  return r.passes() && !r.promotes() && !r.converts();
}

bool PtrType::isInstantiationOf(Context* context, const PtrType* genericType) const {
  auto thisFrom = instantiatedFromPtrType();
  auto argFrom = genericType->instantiatedFromPtrType();
  if (argFrom == nullptr) {
    // if genericType is not a partial instantiation
    return (thisFrom != nullptr && thisFrom == genericType);
  }

  if (thisFrom == argFrom) {
    // handle the case of genericType being partly instantiated
    // (or instantiated with a generic type)
    return isEltTypeInstantiationOf(context, genericType);
  }

  return false;
}


} // end namespace types
} // end namespace chpl
