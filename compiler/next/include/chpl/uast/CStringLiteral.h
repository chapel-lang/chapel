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

#ifndef CHPL_UAST_CSTRINGLITERAL_H
#define CHPL_UAST_CSTRINGLITERAL_H

#include "chpl/queries/Location.h"
#include "chpl/uast/Literal.h"
#include "chpl/uast/StringLiteral.h"

namespace chpl {
namespace uast {


/**
  This class represents a C string literal, for example `c"hello"`.
 */
class CStringLiteral final : public Literal {
 private:
  StringLiteral::QuoteStyle quotes_;
  std::string value_;

  CStringLiteral(StringLiteral::QuoteStyle quotes, std::string value)
    : Literal(asttags::CStringLiteral),
      quotes_(quotes),
      value_(std::move(value))
  { }
  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~CStringLiteral() override = default;

  static owned<CStringLiteral> build(Builder* builder, Location loc,
                                     StringLiteral::QuoteStyle quotes,
                                     std::string value);

  /**
   Returns the type of quotes used for this string literal.
   */
  StringLiteral::QuoteStyle quoteStyle() const { return this->quotes_; }

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
};


} // end namespace uast
} // end namespace chpl

#endif
