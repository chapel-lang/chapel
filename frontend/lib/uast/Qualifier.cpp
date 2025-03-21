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

#include "chpl/uast/Qualifier.h"

namespace chpl {
namespace uast {

bool isGenericQualifier(Qualifier kind) {
  switch (kind) {
    case Qualifier::UNKNOWN:            return true;
    case Qualifier::DEFAULT_INTENT:     return true;
    case Qualifier::CONST_INTENT:       return true;
    case Qualifier::REF_MAYBE_CONST:    return true;
    case Qualifier::VAR:                return false;
    case Qualifier::CONST_VAR:          return false;
    case Qualifier::CONST_REF:          return false;
    case Qualifier::REF:                return false;
    case Qualifier::IN:                 return false;
    case Qualifier::CONST_IN:           return false;
    case Qualifier::OUT:                return false;
    case Qualifier::INOUT:              return false;
    case Qualifier::PARAM:              return false;
    case Qualifier::TYPE:               return false;
    case Qualifier::TYPE_QUERY:         return false;
    case Qualifier::INDEX:              return false;
    case Qualifier::FUNCTION:           return false;
    case Qualifier::PARENLESS_FUNCTION: return false;
    case Qualifier::MODULE:             return false;
    case Qualifier::INIT_RECEIVER:      return false;
  }
  return false;
}

bool isConstQualifier(Qualifier kind) {
  switch (kind) {
    case Qualifier::UNKNOWN:            return false;
    case Qualifier::DEFAULT_INTENT:     return false;
    case Qualifier::CONST_INTENT:       return true;
    case Qualifier::REF_MAYBE_CONST:    return false;
    case Qualifier::VAR:                return false;
    case Qualifier::CONST_VAR:          return true;
    case Qualifier::CONST_REF:          return true;
    case Qualifier::REF:                return false;
    case Qualifier::IN:                 return false;
    case Qualifier::CONST_IN:           return true;
    case Qualifier::OUT:                return false;
    case Qualifier::INOUT:              return false;
    case Qualifier::PARAM:              return true;
    case Qualifier::TYPE:               return true;
    case Qualifier::TYPE_QUERY:         return true;
    case Qualifier::INDEX:              return false;
    case Qualifier::FUNCTION:           return true;
    case Qualifier::PARENLESS_FUNCTION: return true;
    case Qualifier::MODULE:             return true;
    case Qualifier::INIT_RECEIVER:      return false;
  }
  return false;
}

bool isImmutableQualifier(Qualifier kind) {
  switch (kind) {
    case Qualifier::UNKNOWN:            return false;
    case Qualifier::DEFAULT_INTENT:     return false;
    case Qualifier::CONST_INTENT:       return false;
    case Qualifier::REF_MAYBE_CONST:    return false;
    case Qualifier::VAR:                return false;
    case Qualifier::CONST_VAR:          return true;
    case Qualifier::CONST_REF:          return false;
    case Qualifier::REF:                return false;
    case Qualifier::IN:                 return false;
    case Qualifier::CONST_IN:           return true;
    case Qualifier::OUT:                return false;
    case Qualifier::INOUT:              return false;
    case Qualifier::PARAM:              return true;
    case Qualifier::TYPE:               return true;
    case Qualifier::TYPE_QUERY:         return true;
    case Qualifier::INDEX:              return false;
    case Qualifier::FUNCTION:           return true;
    case Qualifier::PARENLESS_FUNCTION: return true;
    case Qualifier::MODULE:             return true;
    case Qualifier::INIT_RECEIVER:      return false;
  }
  return false;
}

bool isRefQualifier(Qualifier kind) {
  switch (kind) {
    case Qualifier::UNKNOWN:            return false;
    case Qualifier::DEFAULT_INTENT:     return false;
    case Qualifier::CONST_INTENT:       return false;
    case Qualifier::REF_MAYBE_CONST:    return true;
    case Qualifier::VAR:                return false;
    case Qualifier::CONST_VAR:          return false;
    case Qualifier::CONST_REF:          return true;
    case Qualifier::REF:                return true;
    case Qualifier::IN:                 return false;
    case Qualifier::CONST_IN:           return false;
    case Qualifier::OUT:                return false;
    case Qualifier::INOUT:              return false;
    case Qualifier::PARAM:              return false;
    case Qualifier::TYPE:               return false;
    case Qualifier::TYPE_QUERY:         return false;
    case Qualifier::INDEX:              return false;
    case Qualifier::FUNCTION:           return false;
    case Qualifier::PARENLESS_FUNCTION: return false;
    case Qualifier::MODULE:             return false;
    case Qualifier::INIT_RECEIVER:      return false;
  }
  return false;
}

bool isInQualifier(Qualifier kind) {
  switch (kind) {
    case Qualifier::UNKNOWN:            return false;
    case Qualifier::DEFAULT_INTENT:     return false;
    case Qualifier::CONST_INTENT:       return false;
    case Qualifier::REF_MAYBE_CONST:    return false;
    case Qualifier::VAR:                return false;
    case Qualifier::CONST_VAR:          return false;
    case Qualifier::CONST_REF:          return false;
    case Qualifier::REF:                return false;
    case Qualifier::IN:                 return true;
    case Qualifier::CONST_IN:           return true;
    case Qualifier::OUT:                return false;
    case Qualifier::INOUT:              return false;
    case Qualifier::PARAM:              return false;
    case Qualifier::TYPE:               return false;
    case Qualifier::TYPE_QUERY:         return false;
    case Qualifier::INDEX:              return false;
    case Qualifier::FUNCTION:           return false;
    case Qualifier::PARENLESS_FUNCTION: return false;
    case Qualifier::MODULE:             return false;
    case Qualifier::INIT_RECEIVER:      return false;
  }
  return false;
}

const char* qualifierToString(Qualifier intent) {
  switch (intent) {
    case Qualifier::UNKNOWN:            return "<unknown-intent>";
    case Qualifier::DEFAULT_INTENT:     return "<default-intent>";
    case Qualifier::CONST_INTENT:       return "const";
    case Qualifier::REF_MAYBE_CONST:    return "<ref-maybe-const>";
    case Qualifier::VAR:                return "var";
    case Qualifier::CONST_VAR:          return "<const-var>";
    case Qualifier::CONST_REF:          return "const ref";
    case Qualifier::REF:                return "ref";
    case Qualifier::IN:                 return "in";
    case Qualifier::CONST_IN:           return "const in";
    case Qualifier::OUT:                return "out";
    case Qualifier::INOUT:              return "inout";
    case Qualifier::PARAM:              return "param";
    case Qualifier::TYPE:               return "type";
    case Qualifier::TYPE_QUERY:         return "<type-query>";
    case Qualifier::INDEX:              return "<index>";
    case Qualifier::FUNCTION:           return "<function>";
    case Qualifier::PARENLESS_FUNCTION: return "<parenless-function>";
    case Qualifier::MODULE:             return "<module>";
    case Qualifier::INIT_RECEIVER:      return "<init-receiver>";
  }
  return "<unknown>";
}


} // namespace uast
} // namespace chpl
