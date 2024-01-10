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

#ifndef CHPL_TYPES_FUNCTIONTYPE_H
#define CHPL_TYPES_FUNCTIONTYPE_H

#include "chpl/types/Type.h"
#include "chpl/uast/Function.h"
#include "chpl/resolution/resolution-types.h"

#include <map>

namespace chpl {
namespace types {

/**
  This class represents a function type. A function type consists of a
  signature and a return type.
 */
class FunctionType final : public Type {
 public:
  using Signature = resolution::TypedFnSignature;
 private:
  const Signature* signature_;
  QualifiedType returnTypeAndIntent_;
  
  FunctionType(const Signature* signature, QualifiedType returnTypeAndIntent)
    : Type(typetags::FunctionType),
      signature_(signature),
      returnTypeAndIntent_(returnTypeAndIntent) {
  }

  bool contentsMatchInner(const Type* other) const override {
    auto rhs = other->toFunctionType();
    return this->returnTypeAndIntent_ == rhs->returnTypeAndIntent_ &&
           this->signature_ == rhs->signature_;
  }

  void markUniqueStringsInner(Context* context) const override {
    signature_->mark(context);
    returnTypeAndIntent_.mark(context);
  }

  Genericity genericity() const override {
    CHPL_ASSERT(false && "Not implemented!");
    return CONCRETE;
  }

  static const owned<FunctionType>&
  getFunctionType(Context* context, const Signature* signature,
                  QualifiedType returnTypeAndIntent);

 public:
  ~FunctionType() = default;

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind)
  const override {
    signature_->stringify(ss, stringKind);
    ss << " ";
    returnTypeAndIntent_.stringify(ss, stringKind);
  }

  /**
    Create a function type given a ``TypedFnSignature`` representing
    a function's formals, and a ``QualifiedType`` representing a
    function's return type and intent.

    Not all information in the input ``TypedFnSignature`` is relevant,
    and certain things will be discarded, such as:

      - Function name and originating ID
      - Where clause information
      - Formal ID information and ``hasDefaultValue()``
      - Any instantiation information

    The ``QualifiedType`` representing the return type and intent
    will also be normalized to ensure it stores a concrete intent.
  */
  static const FunctionType*
  get(Context* context, const Signature* tfs, QualifiedType returnType);

  /** Get the ``TypedFnSignature`` representing the formals of this
      function type. */
  inline const resolution::TypedFnSignature* signature() const {
    return signature_;
  }

  /** Get the ``QualifiedType`` representing the return type and
      return intent of this function type. */
  inline QualifiedType returnTypeAndIntent() const {
    return returnTypeAndIntent_;
  }

  inline const Type* returnType() const {
    return returnTypeAndIntent_.type();
  }

  inline QualifiedType::Kind returnIntent() const {
    return returnTypeAndIntent_.kind();
  }

  /** Get the 'kind' of this function type, e.g., 'proc' or 'iter'. */
  inline uast::Function::Kind kind() const {
    return signature_->untyped()->kind();
  }

  /** Get the number of formals in this function type. */
  inline int numFormals() const {
    return signature_->untyped()->numFormals();
  }

  inline UniqueString formalName(int i) const {
    return signature_->untyped()->formalName(i);
  }

  inline bool formalhasDefault(int i) const {
    return signature_->untyped()->formalHasDefault(i);
  }

  inline bool formalIsVarArgs(int i) const {
    return signature_->untyped()->formalIsVarArgs(i);
  }

  inline bool formalIsInstantiated(int i) {
    return signature_->formalIsInstantiated(i);
  }

  inline const QualifiedType& formalType(int i) const {
    return signature_->formalType(i);
  }

  inline bool throws() const {
    return signature_->untyped()->throws();
  }

  inline bool needsInstantiation() {
    return signature_->needsInstantiation();
  }
};


} // end namespace uast
} // end namespace chpl

#endif
