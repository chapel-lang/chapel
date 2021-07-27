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

#include "chpl/types/Type.h"

namespace chpl {
namespace types {


// TODO: is this name too weird given that it includes param values?
class QualifiedType {
 public:
  typedef enum {
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
  } Kind;

 private:
  Kind kind_ = UNKNOWN;
  const Type* type_ = nullptr;
  int64_t param_ = -1; // TODO: replace with Immediates

 public:
  QualifiedType() { }

  QualifiedType(Kind kind, const Type* type)
    : kind_(kind), type_(type)
  { }

  QualifiedType(Kind kind, const Type* type, int64_t param)
    : kind_(kind), type_(type), param_(param)
  { }

  Kind kind() const { return kind_; }
  const Type* type() const { return type_; }
  int64_t param() const { return param_; }

  bool hasType() const {
    return type_ != nullptr;
  }
  bool hasParam() const {
    // TODO: replace param_ != -1 with ptr check with Immediate
    return kind_ == PARAM && param_ != -1;
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
    Kind tmpKind = this->kind_;
    this->kind_ = other.kind_;
    other.kind_ = tmpKind;

    const Type* tmpType = this->type_;
    this->type_ = other.type_;
    other.type_ = tmpType;

    int tmpParam = this->param_;
    this->param_ = other.param_;
    other.param_ = tmpParam;
  }
  size_t hash() const {
    size_t h1 = chpl::hash(kind_);
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


} // end namespace resolution

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
