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
#include "chpl/uast/Literal.h"

namespace chpl {
namespace uast {


/**
  This class represents a string literal, for example `"hello"`
  and `''' string contents here '''`.
 */
class StringLiteral final : public Literal {
 public:
  enum QuoteStyle {
    SINGLE,
    DOUBLE,
    TRIPLE_SINGLE,
    TRIPLE_DOUBLE
  };

 private:
  std::string value_;
  QuoteStyle quotes_;

  StringLiteral(std::string value, QuoteStyle quotes)
    : Literal(asttags::StringLiteral),
      value_(std::move(value)),
      quotes_(quotes)
  { }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~StringLiteral() override = default;

  static owned<StringLiteral> build(Builder* builder, Location loc,
                                    std::string value,
                                    QuoteStyle quotes);

  /**
   Returns the value of this string literal as a C string,
   not including the quotes.
   */
  const char* c_str() const { return value_.c_str(); }
  /**
   Returns the value of this string literal as a C++ string,
   not including the quotes.
   */
  const std::string& str() const { return value_; }

  /**
   Returns the type of quotes used for this string literal.
   */
  QuoteStyle quoteStyle() const { return this->quotes_; }
};


} // end namespace uast
} // end namespace chpl

#endif
