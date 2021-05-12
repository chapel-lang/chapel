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

#ifndef CHPL_UAST_FNCALL_H
#define CHPL_UAST_FNCALL_H

#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/Call.h"

namespace chpl {
namespace uast {


/**
  This class represents a call to a function.

  For example `f(1,2)`, is a call to a function `f`.

  Note that calls to paren-less free functions are not
  represented with this type since early in compilation
  they are just Identifiers.

  For example, in
  \rst
  .. code-block:: chapel

      proc x { }

      x; // here 'x' is represented as an Identifier, not as a Call
  \endrst


 */
class FnCall : public Call {
 private:
  // For each actual (matching Call's actuals), what are the names?
  // if the actual is unnamed, it is the empty string.
  // If no actuals are named, it is the empty vector.
  std::vector<UniqueString> actualNames_;
  bool callUsedSquareBrackets_;

  FnCall(ASTList children, std::vector<UniqueString> actualNames,
         bool callUsedSquareBrackets)
    : Call(asttags::FnCall, std::move(children), /* hasCalledExpression */ 1),
      actualNames_(std::move(actualNames)),
      callUsedSquareBrackets_(callUsedSquareBrackets) {
  }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~FnCall() override = default;
  static owned<FnCall> build(Builder* builder,
                             Location loc,
                             owned<Expression> calledExpression,
                             ASTList actuals,
                             std::vector<UniqueString> actualNames,
                             bool callUsedSquareBrackets);
  static owned<FnCall> build(Builder* builder,
                             Location loc,
                             owned<Expression> calledExpression,
                             ASTList actuals,
                             bool callUsedSquareBrackets);
  static owned<FnCall> build(Builder* builder,
                             Location loc,
                             owned<Expression> calledExpression,
                             bool callUsedSquareBrackets);

  /** Returns whether actual i is named as with 'f(a=3)'
      where the actual is 3 and the name is 'a'. */
  bool isNamedActual(int i) const {
    if (i < 0 || i >= actualNames_.size())
      return false;

    return !actualNames_[i].isEmpty();
  }

  /** Returns the name of the actual, if used; otherwise the empty string */
  UniqueString actualName(int i) const {
    if (actualNames_.size() == 0)
      return UniqueString();

    return actualNames_[i];
  }

  /** Returns true if the call used square brackets e.g. f[1];
      the alternative is parentheses e.g. f(1). */
  bool callUsedSquareBrackets() const {
    return callUsedSquareBrackets_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
