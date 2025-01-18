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

#include "chpl/types/QualifiedType.h"

#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/types/Param.h"
#include "chpl/types/Type.h"
#include "chpl/types/TupleType.h"

namespace chpl {
namespace types {


bool QualifiedType::isParamTrue() const {
  if (kind_ == Kind::PARAM && param_ != nullptr) {
    return param_->isNonZero();
  }

  return false;
}
bool QualifiedType::isParamFalse() const {
  if (kind_ == Kind::PARAM && param_ != nullptr) {
    return param_->isZero();
  }

  return false;
}

bool QualifiedType::isParamKnownTuple() const {
  if (kind_ == Kind::PARAM && type_ != nullptr) {
    if (auto tt = type_->toTupleType()) {
      return tt->isParamKnown();
    }
  }
  return false;
}

QualifiedType QualifiedType::makeParamBool(Context* context, bool b) {
  return {QualifiedType::PARAM, BoolType::get(context),
          BoolParam::get(context, b)};
}

QualifiedType QualifiedType::makeParamInt(Context* context, int64_t i) {
  return {QualifiedType::PARAM, IntType::get(context, 0),
          IntParam::get(context, i)};
}

QualifiedType QualifiedType::makeParamString(Context* context, UniqueString s) {
  return {QualifiedType::PARAM, RecordType::getStringType(context),
          StringParam::get(context, s)};
}

QualifiedType QualifiedType::makeParamString(Context* context, std::string s) {
  return makeParamString(context, UniqueString::get(context, s));
}

bool QualifiedType::needsSplitInitTypeInfo(Context* context) const {
  return (isParam() && !hasParamPtr()) ||
    isUnknownKindOrType() ||
    resolution::getTypeGenericity(context, type()) == Type::GENERIC;
}

bool QualifiedType::update(QualifiedType& keep, QualifiedType& addin) {
  return defaultUpdate(keep, addin);
}

void QualifiedType::mark(Context* context) const {
  context->markPointer(type_);
  context->markPointer(param_);
}

const char* QualifiedType::kindToString(QualifiedType::Kind kind) {
  return uast::qualifierToString(kind);
}

void QualifiedType::stringify(std::ostream& ss,
                              chpl::StringifyKind stringKind) const {
  const char* kindStr = kindToString(kind_);
  std::ostringstream strstream;

  ss << kindStr;

  if (type_ != nullptr) {
    ss << " ";
    type_->stringify(ss, stringKind);
  }

  if (kind_ == QualifiedType::PARAM && param_ != nullptr) {
    ss << " = ";
    param_->stringify(ss, stringKind);
  }
}

IMPLEMENT_DUMP(QualifiedType);


} // end namespace types
} // end namespace chpl
