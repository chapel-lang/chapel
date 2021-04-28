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

#ifndef CHPL_AST_FNCALL_H
#define CHPL_AST_FNCALL_H

#include "chpl/AST/Call.h"

#include <vector>

namespace chpl {
namespace uast {

/**
  This class represents a function call or method call, including virtual and
  non-virtual calls (since the difference is not known before resolution).
 */
class FnCall final : public Call {
 private:
  // for each actual (matching CallExpr's actuals), what are the names?
  // if the actual is unnamed, it is the empty string.
  std::vector<UniqueString> actualNames_;
  // TODO: do we need partialTag / methodTag?
  bool matchesInner(const ASTBase* other) const override;
 public:
  ~FnCall() override = default;

  /** Returns whether actual i is named as with 'f(a=3)' */
  bool actualIsNamed(int i) const { return !actualNames_[i].isEmpty(); }
  /** Returns the name of the actual, if used; otherwise the empty string */
  UniqueString actualName(int i) const { return actualNames_[i]; }
};


} // end namespace uast
} // end namespace chpl

#endif
