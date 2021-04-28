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

#include "chpl/AST/Comment.h"

#include "chpl/AST/Builder.h"

#include <cstdlib>

namespace chpl {
namespace ast {

Comment::Comment(std::string s)
 : Expr(asttags::Comment), comment_(std::move(s)) {
}

bool Comment::contentsMatchInner(const BaseAST* other) const {
  const Comment* lhs = this;
  const Comment* rhs = (const Comment*) other;
  return lhs->exprContentsMatchInner(rhs) &&
         lhs->comment_ == rhs->comment_ ;
}

owned<Comment> Comment::build(Builder* builder, Location loc, std::string c) {
  Comment* ret = new Comment(std::move(c));
  builder->noteLocation(ret, loc);
  return toOwned(ret);
}

} // namespace ast
} // namespace chpl
