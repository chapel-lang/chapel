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

#ifndef CHPL_UAST_STRINGLIKELITERAL_H
#define CHPL_UAST_STRINGLIKELITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Literal.h"

namespace chpl {
namespace uast {


/**
  This is an abstract parent class for string/bytes/c-string literals.
 */
class StringLikeLiteral : public Literal {
 public:
  enum QuoteStyle {
    SINGLE,
    DOUBLE,
    TRIPLE_SINGLE,
    TRIPLE_DOUBLE
  };

 protected:
  std::string value_;
  QuoteStyle quotes_;

  StringLikeLiteral(ASTTag tag, std::string value, QuoteStyle quotes)
    : Literal(tag),
      value_(std::move(value)),
      quotes_(quotes)
  { }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  virtual ~StringLikeLiteral() = 0; // this is an abstract base class

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
