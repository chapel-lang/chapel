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

#include "chpl/util/hash.h"
#include "chpl/queries/update-functions.h"

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
  enum Kind {
    UNKNOWN,
    CONST,
    REF,
    CONST_REF,
    VALUE,
    CONST_VALUE,
    TYPE,
    PARAM,
    FUNCTION,
    MODULE,
  };

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
    assert(param_ == nullptr || kind_ == PARAM);
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

  bool hasType() const {
    return type_ != nullptr;
  }
  bool hasParam() const {
    return param_ != nullptr;
  }

  bool isGenericOrUnknown() const;

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

  std::string toString() const;
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
