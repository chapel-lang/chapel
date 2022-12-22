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

#include "chpl/uast/IntentList.h"

namespace chpl {
namespace uast {

bool isGenericIntent(IntentList kind) {
  switch (kind) {
    case IntentList::UNKNOWN:            return true;
    case IntentList::DEFAULT_INTENT:     return true;
    case IntentList::CONST_INTENT:       return true;
    case IntentList::VAR:                return false;
    case IntentList::CONST_VAR:          return false;
    case IntentList::CONST_REF:          return false;
    case IntentList::REF:                return false;
    case IntentList::IN:                 return false;
    case IntentList::CONST_IN:           return false;
    case IntentList::OUT:                return false;
    case IntentList::INOUT:              return false;
    case IntentList::PARAM:              return false;
    case IntentList::TYPE:               return false;
    case IntentList::TYPE_QUERY:         return false;
    case IntentList::INDEX:              return false;
    case IntentList::FUNCTION:           return false;
    case IntentList::PARENLESS_FUNCTION: return false;
    case IntentList::MODULE:             return false;
  }
  return false;
}

bool isConstIntent(IntentList kind) {
  switch (kind) {
    case IntentList::UNKNOWN:            return false;
    case IntentList::DEFAULT_INTENT:     return false;
    case IntentList::CONST_INTENT:       return true;
    case IntentList::VAR:                return false;
    case IntentList::CONST_VAR:          return true;
    case IntentList::CONST_REF:          return true;
    case IntentList::REF:                return false;
    case IntentList::IN:                 return false;
    case IntentList::CONST_IN:           return true;
    case IntentList::OUT:                return false;
    case IntentList::INOUT:              return false;
    case IntentList::PARAM:              return true;
    case IntentList::TYPE:               return true;
    case IntentList::TYPE_QUERY:         return true;
    case IntentList::INDEX:              return false;
    case IntentList::FUNCTION:           return true;
    case IntentList::PARENLESS_FUNCTION: return true;
    case IntentList::MODULE:             return true;
  }
  return false;
}


bool isRefIntent(IntentList kind) {
  switch (kind) {
    case IntentList::UNKNOWN:            return false;
    case IntentList::DEFAULT_INTENT:     return false;
    case IntentList::CONST_INTENT:       return false;
    case IntentList::VAR:                return false;
    case IntentList::CONST_VAR:          return false;
    case IntentList::CONST_REF:          return true;
    case IntentList::REF:                return true;
    case IntentList::IN:                 return false;
    case IntentList::CONST_IN:           return false;
    case IntentList::OUT:                return false;
    case IntentList::INOUT:              return false;
    case IntentList::PARAM:              return false;
    case IntentList::TYPE:               return false;
    case IntentList::TYPE_QUERY:         return false;
    case IntentList::INDEX:              return false;
    case IntentList::FUNCTION:           return false;
    case IntentList::PARENLESS_FUNCTION: return false;
    case IntentList::MODULE:             return false;
  }
  return false;
}

bool isInIntent(IntentList kind) {
  switch (kind) {
    case IntentList::UNKNOWN:            return false;
    case IntentList::DEFAULT_INTENT:     return false;
    case IntentList::CONST_INTENT:       return false;
    case IntentList::VAR:                return false;
    case IntentList::CONST_VAR:          return false;
    case IntentList::CONST_REF:          return false;
    case IntentList::REF:                return false;
    case IntentList::IN:                 return true;
    case IntentList::CONST_IN:           return true;
    case IntentList::OUT:                return false;
    case IntentList::INOUT:              return false;
    case IntentList::PARAM:              return false;
    case IntentList::TYPE:               return false;
    case IntentList::TYPE_QUERY:         return false;
    case IntentList::INDEX:              return false;
    case IntentList::FUNCTION:           return false;
    case IntentList::PARENLESS_FUNCTION: return false;
    case IntentList::MODULE:             return false;
  }
  return false;
}

const char* intentToString(IntentList intent) {
  switch (intent) {
    case IntentList::UNKNOWN:            return "<unknown-intent>";
    case IntentList::DEFAULT_INTENT:     return "<default-intent>";
    case IntentList::CONST_INTENT:       return "const";
    case IntentList::VAR:                return "var";
    case IntentList::CONST_VAR:          return "<const-var>";
    case IntentList::CONST_REF:          return "const ref";
    case IntentList::REF:                return "ref";
    case IntentList::IN:                 return "in";
    case IntentList::CONST_IN:           return "const in";
    case IntentList::OUT:                return "out";
    case IntentList::INOUT:              return "inout";
    case IntentList::PARAM:              return "param";
    case IntentList::TYPE:               return "type";
    case IntentList::TYPE_QUERY:         return "<type-query>";
    case IntentList::INDEX:              return "<index>";
    case IntentList::FUNCTION:           return "<function>";
    case IntentList::PARENLESS_FUNCTION: return "<parenless-function>";
    case IntentList::MODULE:             return "<module>";
  }
  return "<unknown>";
}


} // namespace uast
} // namespace chpl
