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

#ifndef CHPL_UAST_INTLITERAL_H
#define CHPL_UAST_INTLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/NumericLiteral.h"

namespace chpl {
namespace uast {


/**
  This class represents a signed integer literal.
  All integer literals that are not too big are signed integer literals,
  i.e. have the type `int`.
  However there are no negative literals. Negative numbers are created
  by applying the unary `-` operator.
 */
class IntLiteral final : public NumericLiteral<int64_t> {
 private:
  IntLiteral(int64_t value, UniqueString text)
    : NumericLiteral(asttags::IntLiteral, value, text)
  { }

  // contentsMatchInner / markUniqueStringsInner are in NumericLiteral
  // and would need to be defined here if any fields are added.

 public:
  ~IntLiteral() override = default;

  static owned<IntLiteral> build(Builder* builder, Location loc,
                                 int64_t value, UniqueString text);
};


} // end namespace uast
} // end namespace chpl

#endif
