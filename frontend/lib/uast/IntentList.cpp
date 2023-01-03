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

#include "chpl/uast/IntentList.h"

namespace chpl {
namespace uast {


const char* intentToString(IntentList intent) {
  switch (intent) {
    case IntentList::UNKNOWN: return "<unknown-intent>";
    case IntentList::DEFAULT_INTENT: return "<default-intent>";
    case IntentList::CONST_INTENT: return "const";
    case IntentList::VAR: return "var";
    case IntentList::CONST_VAR: return "<const-var>";
    case IntentList::CONST_REF: return "const ref";
    case IntentList::REF: return "ref";
    case IntentList::IN: return "in";
    case IntentList::CONST_IN: return "const in";
    case IntentList::OUT: return "out";
    case IntentList::INOUT: return "inout";
    case IntentList::PARAM: return "param";
    case IntentList::TYPE: return "type";
    case IntentList::TYPE_QUERY: return "<type-query>";
    case IntentList::INDEX: return "<index>";
    case IntentList::FUNCTION: return "<function>";
    case IntentList::PARENLESS_FUNCTION: return "<parenless-function>";
    case IntentList::MODULE: return "<module>";
  }
  return "<unknown>";
}


} // namespace uast
} // namespace chpl
