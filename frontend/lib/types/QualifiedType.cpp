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

#include "chpl/types/QualifiedType.h"

#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/Param.h"
#include "chpl/types/Type.h"
#include "chpl/types/TupleType.h"

namespace chpl {
namespace types {


bool QualifiedType::isParamTrue() const {
  if (kind_ == Kind::PARAM && param_ != nullptr) {
    if (auto bp = param_->toBoolParam()) {
      if (bp->value() == true) {
        return true;
      }
    }
  }

  return false;
}
bool QualifiedType::isParamFalse() const {
  if (kind_ == Kind::PARAM && param_ != nullptr) {
    if (auto bp = param_->toBoolParam()) {
      if (bp->value() == false) {
        return true;
      }
    }
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
