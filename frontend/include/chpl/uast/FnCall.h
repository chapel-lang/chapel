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

#ifndef CHPL_UAST_FNCALL_H
#define CHPL_UAST_FNCALL_H

#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
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

  FnCall(AstList children, std::vector<UniqueString> actualNames,
         bool callUsedSquareBrackets)
    : Call(asttags::FnCall, std::move(children), /* hasCalledExpression */ 1),
      actualNames_(std::move(actualNames)),
      callUsedSquareBrackets_(callUsedSquareBrackets) {
  }
  FnCall(Deserializer& des)
    : Call(asttags::FnCall, des) {
    actualNames_ = des.read<std::vector<UniqueString>>();
    callUsedSquareBrackets_ = des.read<bool>();
  }
  bool contentsMatchInner(const AstNode* other) const override {
    const FnCall* lhs = this;
    const FnCall* rhs = (const FnCall*) other;

    if (!lhs->callContentsMatchInner(rhs))
      return false;

    if (lhs->callUsedSquareBrackets_ != rhs->callUsedSquareBrackets_ ||
        lhs->actualNames_.size() != rhs->actualNames_.size())
      return false;

    int nActualNames = (int) lhs->actualNames_.size();
    for (int i = 0; i < nActualNames; i++) {
      if (lhs->actualNames_[i] != rhs->actualNames_[i])
        return false;
    }

    return true;
  }
  void markUniqueStringsInner(Context* context) const override {
    callMarkUniqueStringsInner(context);
    for (const auto& str : actualNames_) {
      str.mark(context);
    }
  }
  void dumpFieldsInner(const DumpSettings& s) const override;

 public:
  ~FnCall() override = default;
  static owned<FnCall> build(Builder* builder,
                             Location loc,
                             owned<AstNode> calledExpression,
                             AstList actuals,
                             std::vector<UniqueString> actualNames,
                             bool callUsedSquareBrackets);
  static owned<FnCall> build(Builder* builder,
                             Location loc,
                             owned<AstNode> calledExpression,
                             AstList actuals,
                             bool callUsedSquareBrackets);
  static owned<FnCall> build(Builder* builder,
                             Location loc,
                             owned<AstNode> calledExpression,
                             bool callUsedSquareBrackets);

  /** Returns whether actual i is named as with 'f(a=3)'
      where the actual is 3 and the name is 'a'. */
  bool isNamedActual(int i) const {
    if (i < 0 || i >= (int) actualNames_.size())
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

  void serialize(Serializer& ser) const override {
    Call::serialize(ser);
    ser.write(actualNames_);
    ser.write(callUsedSquareBrackets_);
  }

  DECLARE_STATIC_DESERIALIZE(FnCall);
};


} // end namespace uast
} // end namespace chpl

#endif
