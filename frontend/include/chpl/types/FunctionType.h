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

#ifndef CHPL_TYPES_FUNCTIONTYPE_H
#define CHPL_TYPES_FUNCTIONTYPE_H

#include "chpl/types/Type.h"
#include "chpl/uast/Function.h"
#include "chpl/resolution/resolution-types.h"

#include <map>

namespace chpl {
namespace types {

/**
  This class represents a function type.
 */
class FunctionType final : public Type {
 private:
  const resolution::TypedFnSignature* tfs_;
  
  FunctionType(const resolution::TypedFnSignature* tfs)
    : Type(typetags::FunctionType),
      tfs_(tfs) {
  }

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = other->toFunctionType();
    return this->tfs_ == rhs->tfs_;
  }

  void markUniqueStringsInner(Context* context) const override {
    tfs_->mark(context);
  }

  Genericity genericity() const override {
    CHPL_ASSERT(false && "Not implemented!");
    return CONCRETE;
  }

  static const owned<FunctionType>&
  getFunctionType(Context* context, const resolution::TypedFnSignature* tfs);

 public:
  ~FunctionType() = default;

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind)
  const override {
    tfs_->stringify(ss, stringKind);
  }

  /**
    Create a function type given a ``TypedFnSignature``. Not all information
    in the input signature is relevant, and certain information will be
    discarded by creating a new ``TypedFnSignature``, including:

      - Function name and originating ID
      - Where clause information
      - Formal ID information, ``hasDefaultValue()``
      - Instantiation information

    All created signatures are compiler generated.
  */
  static const FunctionType*
  get(Context* context, const resolution::TypedFnSignature* tfs);

  inline const resolution::TypedFnSignature* typed() const {
    return tfs_;
  }

  inline const resolution::UntypedFnSignature* untyped() const {
    return tfs_->untyped();
  }

  inline uast::Function::Kind kind() const {
    return untyped()->kind();
  }

  inline int numFormals() const {
    return untyped()->numFormals();
  }

  inline UniqueString formalName(int i) const {
    return untyped()->formalName(i);
  }

  inline bool formalhasDefault(int i) const {
    return untyped()->formalHasDefault(i);
  }

  inline bool formalIsVarArgs(int i) const {
    return untyped()->formalIsVarArgs(i);
  }

  inline bool formalIsInstantiated(int i) {
    return tfs_->formalIsInstantiated(i);    
  }

  inline const QualifiedType& formalType(int i) const {
    return tfs_->formalType(i);
  }

  inline bool throws() const {
    return untyped()->throws();
  }

  inline bool needsInstantiation() {
    return tfs_->needsInstantiation();
  }

};


} // end namespace uast
} // end namespace chpl

#endif
