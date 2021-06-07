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

#ifndef CHPL_UAST_STRINGLITERAL_H
#define CHPL_UAST_STRINGLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/StringLikeLiteral.h"

namespace chpl {
namespace uast {


/**
  This class represents a string literal, for example `"hello"`
  and `''' string contents here '''`.
 */
class StringLiteral final : public StringLikeLiteral {
 private:
  StringLiteral(std::string value, StringLikeLiteral::QuoteStyle quotes)
    : StringLikeLiteral(asttags::StringLiteral, std::move(value), quotes)
  { }

  // contentsMatchInner / markUniqueStringsInner are in StringLikeLiteral
  // and would need to be defined here if any fields are added.

 public:
  ~StringLiteral() override = default;

  static owned<StringLiteral> build(Builder* builder, Location loc,
                                    std::string value,
                                    StringLikeLiteral::QuoteStyle quotes);
};


} // end namespace uast
} // end namespace chpl

#endif
