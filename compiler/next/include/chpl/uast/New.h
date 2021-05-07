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

#ifndef CHPL_UAST_NEW_H_
#define CHPL_UAST_NEW_H

#include "chpl/uast/Expression.h"

namespace chpl {
namespace uast {

/**
  This class represents a new expression. For example:

  \rst
  .. code-block:: chapel

    // Example 1:
    var foo = new bar(a = 1, 2);
  \endrst

 */
class New : public Call {
 public:

  /**
    Possible management flavors for a new expression.
  */
  enum struct Management {
    None,
    Borrowed,
    Owned,
    Shared,
    Unmanaged
  };

 private:
  New(Management::Style management, ASTList children,
      std::vecctor<UniqueString> actualNames);
  bool callContentsMatchInner(const New* other) const;
  void callMarkUniqueStringsInner(Context* context) const;
  Management management_;

 public:

  /**
    Create and return a new expression with the given management style,
  */
  static owned<New> build(Builder* builder, Location loc,
                          Management management,
                          owned<Expression> calledExpression,
                          ASTList actuals,
                          std::vector<UniqueString> actualNames);

  /**
    Create and return a new expression with no management style,
  */
  static owned<New> build(Builder* builder, Location loc,
                          owned<Expression> calledExpression,
                          ASTList actuals,
                          std::vector<UniqueString> actualNames);

  /**
    Returns whether the i'th actual is named as with 'f(a=3)' where the
    actual is '3' and the name is 'a'.
  */
  bool isNamedActual(int i) const;

  /**
    Returns the name of the i'th actual, or the empty string if the actual
    was not named.
  */
  UniqueString actualName(int i) const;

  /**
    Returns the management style of this new expression.
  */
  Management management() const;
};


} // end namespace uast
} // end namespace chpl

#endif
