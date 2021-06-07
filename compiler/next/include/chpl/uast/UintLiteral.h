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

#ifndef CHPL_UAST_UINTLITERAL_H
#define CHPL_UAST_UINTLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/NumericLiteral.h"

namespace chpl {
namespace uast {


/**
  This class represents an unsigned integer literal.
  It is only used for integers too large to fit into `int64_t`.
  Such integer literals have type `uint`.
 */
class UintLiteral final : public NumericLiteral<uint64_t> {
 private:
  UintLiteral(uint64_t value, int base)
    : NumericLiteral(asttags::UintLiteral, value, base)
  { }

  // contentsMatchInner / markUniqueStringsInner are in NumericLiteral
  // and would need to be defined here if any fields are added.

 public:
  ~UintLiteral() override = default;

  static owned<UintLiteral> build(Builder* builder, Location loc,
                                  uint64_t value, int base);
};


} // end namespace uast
} // end namespace chpl

#endif
