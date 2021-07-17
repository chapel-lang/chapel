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
  } Kind;

 private:
  Kind kind_ = UNKNOWN;
  const Type* type_ = nullptr;
  int param_ = 0; // TODO: replace with Immediates

 public:
  QualifiedType() { }

  QualifiedType(Kind kind, const Type* type)
    : kind_(kind), type_(type)
  { }

  QualifiedType(const Type* type, int param)
    : kind_(PARAM), type_(type), param_(param)
  { }

  Kind kind() const { return kind_; }
  const Type* type() const { return type_; }
  int param() const { return param_; }

  bool operator==(const QualifiedType& other) const {
    return kind_ == other.kind_ &&
           type_ == other.type_ &&
           param_ == other.param_;
  }
  bool operator!=(const QualifiedType& other) const {
    return !(*this == other);
  }
};


} // end namespace resolution
} // end namespace chpl
#endif
