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


#include "chpl/framework/global-strings.h"
#include "chpl/uast/uast-util.h"
#include "chpl/uast/Identifier.h"

namespace chpl {
namespace uast {
namespace util {

  bool isQuestionMark(const AstNode* node) {
    if (node == nullptr) return false;

    if (auto ident = node->toIdentifier()) {
      if (ident->name() == USTR("?")) {
        return true;
      }
    }

    return false;
  }
} // namespace util
} // namespace uast
} // namespace chpl
