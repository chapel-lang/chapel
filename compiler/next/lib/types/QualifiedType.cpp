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

#include "chpl/types/QualifiedType.h"

#include "chpl/types/Param.h"
#include "chpl/types/Type.h"

namespace chpl {
namespace types {


bool QualifiedType::isGenericType() const {
  return type_->isGeneric();
}

bool QualifiedType::isUnknownType() const {
  return type_->isUnknownType();
}

static const char* kindToString(QualifiedType::Kind kind) {
  switch (kind) {
    case QualifiedType::UNKNOWN:        return "unknown";
    case QualifiedType::DEFAULT_INTENT: return "default intent";
    case QualifiedType::CONST_INTENT:   return "const intent";
    case QualifiedType::VAR:            return "var";
    case QualifiedType::CONST_VAR:      return "const";
    case QualifiedType::CONST_REF:      return "const ref";
    case QualifiedType::REF:            return "ref";
    case QualifiedType::IN:             return "in";
    case QualifiedType::CONST_IN:       return "const in";
    case QualifiedType::OUT:            return "out";
    case QualifiedType::INOUT:          return "inout";
    case QualifiedType::PARAM:          return "param";
    case QualifiedType::TYPE:           return "type";
    case QualifiedType::INDEX:          return "index";
    case QualifiedType::FUNCTION:       return "function";
    case QualifiedType::MODULE:         return "module";
  }

  assert(false && "should not be reachable");
  return "unknown";
}

std::string QualifiedType::toString() const {
  const char* kindStr = kindToString(kind_);
  std::string typeStr = (type_)?(type_->toString()):(std::string("nullptr"));

  std::string ret = kindStr;

  if (type_ != nullptr) {
    ret += " ";
    ret += type_->toString();
  }

  if (kind_ == QualifiedType::PARAM && param_ != nullptr) {
    ret += " = ";
    ret += param_->toString();
  }

  return ret;
}


} // end namespace types
} // end namespace chpl
