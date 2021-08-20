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

#ifndef CHPL_UAST_IMAGLITERAL_H
#define CHPL_UAST_IMAGLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/NumericLiteral.h"

namespace chpl {
namespace uast {


/**
  This class represents an imaginary floating point literal, e.g. ``10.4i``.
 */
class ImagLiteral final : public NumericLiteral<double> {
 private:
  ImagLiteral(double value, UniqueString text)
    : NumericLiteral(asttags::ImagLiteral, value, text)
  { }

  // contentsMatchInner / markUniqueStringsInner are in NumericLiteral
  // and would need to be defined here if any fields are added.

 public:
  ~ImagLiteral() override = default;

  static owned<ImagLiteral> build(Builder* builder, Location loc,
                                  double value, UniqueString text);
};


} // end namespace uast
} // end namespace chpl

#endif
