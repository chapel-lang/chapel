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

#ifndef CHPL_TYPES_HEAPBUFFER_TYPE_H
#define CHPL_TYPES_HEAPBUFFER_TYPE_H

#include "chpl/types/PtrType.h"
#include "chpl/types/Type.h"

namespace chpl {
namespace types {

class HeapBufferType final : public PtrType {
 private:
  HeapBufferType(const HeapBufferType* instantiatedFrom, const Type* eltType)
      : PtrType(typetags::HeapBufferType, instantiatedFrom, eltType) {}

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (HeapBufferType*)other;
    return instantiatedFrom_ == rhs->instantiatedFrom_ &&
           eltType_ == rhs->eltType_;
  }

  void markUniqueStringsInner(Context* context) const override {}

  static const owned<HeapBufferType>& getHeapBufferType(
      Context* context, const HeapBufferType* instantiatedFrom,
      const Type* eltType);

 public:
  static const HeapBufferType* get(Context* context);
  static const HeapBufferType* get(Context* context, const Type* eltType);
  static const ID& getId(Context* context);

  const ID& id(Context* context) const override {
    return getId(context);
  }

  virtual void stringify(std::ostream& ss,
                         chpl::StringifyKind stringKind) const override;
};

} // end namespace types
} // end namespace chpl

#endif
