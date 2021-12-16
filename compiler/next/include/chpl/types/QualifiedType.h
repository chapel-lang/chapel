/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/queries/update-functions.h"
#include "chpl/uast/IntentList.h"
#include "chpl/util/hash.h"
#include "chpl/queries/stringify-functions.h"

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
  using Kind = uast::IntentList;

  // instead of the below,
  // could use 'using enum uast::IntentList' in C++20
  static const Kind UNKNOWN = uast::IntentList::UNKNOWN;
  static const Kind DEFAULT_INTENT = uast::IntentList::DEFAULT_INTENT;
  static const Kind CONST_INTENT = uast::IntentList::CONST_INTENT;
  static const Kind VAR = uast::IntentList::VAR;
  static const Kind CONST_VAR = uast::IntentList::CONST_VAR;
  static const Kind CONST_REF = uast::IntentList::CONST_REF;
  static const Kind REF = uast::IntentList::REF;
  static const Kind IN = uast::IntentList::IN;
  static const Kind CONST_IN = uast::IntentList::CONST_IN;
  static const Kind OUT = uast::IntentList::OUT;
  static const Kind INOUT = uast::IntentList::INOUT;
  static const Kind PARAM = uast::IntentList::PARAM;
  static const Kind TYPE = uast::IntentList::TYPE;
  static const Kind INDEX = uast::IntentList::INDEX;
  static const Kind FUNCTION = uast::IntentList::FUNCTION;
  static const Kind MODULE = uast::IntentList::MODULE;

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
    assert(param_ == nullptr || kind_ == Kind::PARAM);
  }

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

  // these are not defined here so we don't have to #include Type.h, Param.h
  bool isGenericType() const;
  bool isUnknownType() const;

  bool isGeneric() const {
    bool genericParam = kind_ == PARAM && !hasParamPtr();
    return genericParam || isGenericType();
  }

  bool isUnknown() const {
    return kind_ == UNKNOWN || !hasTypePtr() || isUnknownType();
  }

  bool isGenericOrUnknown() const { return isUnknown() || isGeneric(); }

  /** Returns true if kind is TYPE */
  bool isType() const { return kind_ == Kind::TYPE; }

  /** Returns true if kind is PARAM */
  bool isParam() const { return kind_ == Kind::PARAM; }

  /**
    Returns true if the value cannot be modified directly (but might
    be modified by some other aliasing variable).
    In particular, returns true for all kinds other than REF and VALUE.
   */
  bool isConst() const {
    return kind_ == Kind::CONST_INTENT ||
           kind_ == Kind::CONST_VAR ||
           kind_ == Kind::CONST_REF ||
           kind_ == Kind::CONST_IN ||
           kind_ == Kind::PARAM ||
           kind_ == Kind::TYPE ||
           kind_ == Kind::FUNCTION ||
           kind_ == Kind::MODULE;
  }
  /** Returns true if the type refers to something immutable
      (that cannot be modified by any task / other reference to it). */
  bool isImmutable() const {
    return kind_ == Kind::CONST_VAR ||
           kind_ == Kind::CONST_IN ||
           kind_ == Kind::PARAM ||
           kind_ == Kind::TYPE ||
           kind_ == Kind::FUNCTION ||
           kind_ == Kind::MODULE;
  }

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

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;
};


} // end namespace types

// docs are turned off for this as a workaround for breathe errors
/// \cond DO_NOT_DOCUMENT
template<> struct update<chpl::types::QualifiedType> {
  bool operator()(chpl::types::QualifiedType& keep,
                  chpl::types::QualifiedType& addin) const {
    return defaultUpdate(keep, addin);
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
