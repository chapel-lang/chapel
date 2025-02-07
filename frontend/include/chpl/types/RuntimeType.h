/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TYPES_RUNTIME_TYPE_H
#define CHPL_TYPES_RUNTIME_TYPE_H

#include "chpl/types/Type.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace types {

/**
  This class encodes Chapel's notion of runtime types. In production,
  calls to functions tagged with "runtime type init fn" are adjusted to
  store their arguments into a new record. This record exists at runtime,
  and is used as part of Chapel's runtime type support.
 */
class RuntimeType final : public Type {
 private:
  const resolution::TypedFnSignature* initializer_;

  RuntimeType(const resolution::TypedFnSignature* initializer):
    Type(typetags::RuntimeType), initializer_(initializer) {
    CHPL_ASSERT(initializer != nullptr);
    CHPL_ASSERT(!initializer->needsInstantiation());
  }

  static owned<RuntimeType> const&
  getRuntimeType(Context* context,
                 const resolution::TypedFnSignature* initializer);

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = (const RuntimeType*) other;
    return initializer_ == rhs->initializer_;
  }

  void markUniqueStringsInner(Context* context) const override {
    initializer_->mark(context);
  }

  Genericity genericity() const override {
    return CONCRETE;
  }

 public:
  static const RuntimeType* get(Context* context,
                                const resolution::TypedFnSignature* initializer);

  const Type* substitute(Context* context,
                         const PlaceholderMap& subs) const override {
    return get(context, initializer_->substitute(context, subs));
  }

  const resolution::TypedFnSignature* initializer() const { return initializer_; }
};

} // end namespace types
} // end namespace chpl

#endif
