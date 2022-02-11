/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

namespace chpl {
namespace types {


Type::Genericity QualifiedType::genericityWithFields(Context* context) const {
   Type::Genericity g = genericity();
   if (g == Type::MAYBE_GENERIC && type_ != nullptr ) {
     return resolution::getTypeGenericity(context, type_);
   }

   // otherwise return whatever we computed
   return g;
}

bool QualifiedType::update(QualifiedType& keep, QualifiedType& addin) {
  return defaultUpdate(keep, addin);
}

void QualifiedType::mark(Context* context) const {
  context->markPointer(type_);
  context->markPointer(param_);
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
