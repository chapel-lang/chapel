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

#ifndef CHPL_TYPES_QUALIFIEDTYPE_H
#define CHPL_TYPES_QUALIFIEDTYPE_H

#include "chpl/framework/update-functions.h"
#include "chpl/uast/Qualifier.h"
#include "chpl/util/hash.h"
#include "chpl/framework/stringify-functions.h"
#include "chpl/types/Type.h"

#include <cstddef>
#include <string>

namespace chpl {
namespace types {
class Param;
class Type;

/**
  This class represents a combination of 3 things:
    a Type subclass pointer representing a Chapel type
    a Kind representing variable kind or intent (e.g. ref, value, const)
    a param value for when the Kind is PARAM.

  This combination is the type information that the resolver needs.
 */
class QualifiedType final {
 public:
  using Kind = uast::Qualifier;

  // instead of the below,
  // could use 'using enum uast::Qualifier' in C++20
  static const Kind UNKNOWN = uast::Qualifier::UNKNOWN;
  static const Kind DEFAULT_INTENT = uast::Qualifier::DEFAULT_INTENT;
  static const Kind CONST_INTENT = uast::Qualifier::CONST_INTENT;
  static const Kind REF_MAYBE_CONST = uast::Qualifier::REF_MAYBE_CONST;
  static const Kind VAR = uast::Qualifier::VAR;
  static const Kind CONST_VAR = uast::Qualifier::CONST_VAR;
  static const Kind CONST_REF = uast::Qualifier::CONST_REF;
  static const Kind REF = uast::Qualifier::REF;
  static const Kind IN = uast::Qualifier::IN;
  static const Kind CONST_IN = uast::Qualifier::CONST_IN;
  static const Kind OUT = uast::Qualifier::OUT;
  static const Kind INOUT = uast::Qualifier::INOUT;
  static const Kind PARAM = uast::Qualifier::PARAM;
  static const Kind TYPE = uast::Qualifier::TYPE;
  static const Kind TYPE_QUERY = uast::Qualifier::TYPE_QUERY;
  static const Kind INDEX = uast::Qualifier::INDEX;
  static const Kind FUNCTION = uast::Qualifier::FUNCTION;
  static const Kind PARENLESS_FUNCTION = uast::Qualifier::PARENLESS_FUNCTION;
  static const Kind MODULE = uast::Qualifier::MODULE;
  static const Kind INIT_RECEIVER = uast::Qualifier::INIT_RECEIVER;

  static const char* kindToString(Kind k);

 private:
  Kind kind_ = UNKNOWN;
  const Type* type_ = nullptr;
  const Param* param_ = nullptr;

 public:
  QualifiedType() { }

  QualifiedType(Kind kind, const Type* type)
    : kind_(kind), type_(type)
  { }

  QualifiedType(Kind kind, const Type* type, const Param* param)
    : kind_(kind), type_(type), param_(param)
  {
    // should only set param_ for kind_ == PARAM
    CHPL_ASSERT(param_ == nullptr || kind_ == Kind::PARAM);
  }

  /*
  QualifiedType(const QualifiedType& qt)
    : kind_(qt.kind_), type_(qt.type_), param_(qt.param_) {
  }*/

  /** Returns the kind of the expression this QualifiedType represents */
  Kind kind() const { return kind_; }
  /**
    Returns the Type subclass that represents the type part
    of this QualifiedType
   */
  const Type* type() const { return type_; }
  /**
    Returns the param value for this QualifiedType. Note that
    an uninstantiated param formal will have kind() == PARAM but won't
    yet have a param value. */
  const Param* param() const { return param_; }

  bool hasTypePtr() const {
    return type_ != nullptr;
  }
  bool hasParamPtr() const {
    return param_ != nullptr;
  }

  /** Returns the fast-to-compute genericity of the contained Type pointer.
      Does not include consideration of fields. If the type can have fields
      and they have not been considered, returns Type::MAYBE_GENERIC.

      To consider fields, use the getTypeGenericity.
   */
  Type::Genericity typeGenericity() const {
    if (type_ == nullptr)
      return Type::MAYBE_GENERIC;

    return type_->genericity();
  }

  /** Returns the fast-to-compute genericity of this QualifiedType.
      Does not include consideration of fields. If the type can have fields
      and they have not been considered, returns Type::MAYBE_GENERIC.

      To consider fields, use the getTypeGenericity.
  */
  Type::Genericity genericity() const {
    bool genericParam = kind_ == PARAM && !hasParamPtr();
    if (genericParam || kind_ == TYPE_QUERY)
      return Type::GENERIC;

    // params with know values (hasParamPtr()) can't be generic.
    if (kind_ == PARAM)
      return Type::CONCRETE;

    return typeGenericity();
  }

  bool isUnknown() const {
    return isUnknownKindOrType() ||
      (kind_ == PARAM && !hasParamPtr() && !isParamKnownTuple());
  }

  bool isUnknownKindOrType() const {
    return kind_ == UNKNOWN || !hasTypePtr() || type_->isUnknownType();
  }

  bool isErroneousType() const {
    return hasTypePtr() && type_->isErroneousType();
  }

  bool isGenericOrUnknown() const {
    return isUnknown() || (genericity() != Type::CONCRETE);
  }

  bool isUnknownOrErroneous() const {
    return isUnknown() || isErroneousType();
  }

  /** Returns true if kind is TYPE */
  bool isType() const { return kind_ == Kind::TYPE; }

  /** Returns true if kind is PARAM */
  bool isParam() const { return kind_ == Kind::PARAM; }

  /** Returns 'true' if storing a BoolParam 'true';
      'false' otherwise
   */
  bool isParamTrue() const;

  /** Returns 'true' if storing a BoolParam 'false';
      'false' otherwise
   */
  bool isParamFalse() const;

  /** Returns 'true' if storing a tuple type whose
      constituent types are all known params.
  */
  bool isParamKnownTuple() const;

  /**
    Returns true if the value cannot be modified directly (but might
    be modified by some other aliasing variable).
    In particular, returns true for all kinds other than REF and VALUE.
   */
  bool isConst() const {
    return uast::isConstQualifier(kind_);
  }
  /** Returns true if the type refers to something immutable
      (that cannot be modified by any task / other reference to it). */
  bool isImmutable() const {
    return uast::isImmutableQualifier(kind_);
  }
  /**
    Returns true if the value is a reference, whether constant or mutable.
   */
  bool isRef() const {
    return uast::isRefQualifier(kind_);
  }
  /**
    Returns true if the value is an in-intent formal, whether constant or
    mutable.
   */
  bool isIn() const {
    return uast::isInQualifier(kind_);
  }

  /**
    Returns true if the kind is one of the non-concrete intents
    (unknown, default intent, or const intent) and false otherwise.
   */
  bool isNonConcreteIntent() const {
    return uast::isGenericQualifier(kind_);
  }

  /**
    Returns true if the type might need to get more info from split-init.
  */
  bool needsSplitInitTypeInfo(Context* context) const;

  bool operator==(const QualifiedType& other) const {
    return kind_ == other.kind_ &&
           type_ == other.type_ &&
           param_ == other.param_;
  }
  bool operator!=(const QualifiedType& other) const {
    return !(*this == other);
  }
  void swap(QualifiedType& other) {
    std::swap(this->kind_, other.kind_);
    std::swap(this->type_, other.type_);
    std::swap(this->param_, other.param_);
  }
  size_t hash() const {
    size_t h1 = (size_t) kind_;
    size_t h2 = chpl::hash(type_);
    size_t h3 = chpl::hash(param_);

    size_t ret = 0;
    ret = hash_combine(ret, h1);
    ret = hash_combine(ret, h2);
    ret = hash_combine(ret, h3);
    return ret;
  }
  static bool update(QualifiedType& keep, QualifiedType& addin);
  void mark(Context* context) const;

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

} // end namespace types

template <>
struct stringify<types::QualifiedType::Kind> {
  void operator()(std::ostream& os, StringifyKind stringKind,
                  types::QualifiedType::Kind k) {
    os << types::QualifiedType::kindToString(k);
  }
};

// docs are turned off for this as a workaround for breathe errors
/// \cond DO_NOT_DOCUMENT

template <>
struct mark<types::QualifiedType::Kind> {
  void operator()(Context* context, types::QualifiedType::Kind t) {
    // No need to mark enums
  }
};


/// \endcond

} // end namespace chpl


namespace std {

template<> struct hash<chpl::types::QualifiedType>
{
  size_t operator()(const chpl::types::QualifiedType& key) const {
    return key.hash();
  }
};

} // end namespace std


#endif
