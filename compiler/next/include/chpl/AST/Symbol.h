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

#ifndef CHPL_AST_SYMBOL_H
#define CHPL_AST_SYMBOL_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for Symbols
 */
class Symbol : public BaseAST {

 public:
  enum Visibility {
    VISIBILITY_DEFAULT,
    VISIBILITY_PUBLIC,
    VISIBILITY_PRIVATE,
  };

 private:
  UniqueString name_;
  Visibility visibility_;

 protected:
  Symbol(asttags::ASTTag tag, UniqueString name, Visibility vis);
  Symbol(asttags::ASTTag tag, ASTList children,
         UniqueString name, Visibility vis);
  bool symbolContentsMatchInner(const Symbol* other) const {
    return this->name_ == other->name_ &&
           this->visibility_ == other->visibility_;
  }

 public:
  virtual ~Symbol() = 0; // this is an abstract base class

  UniqueString name() const { return name_; }
  Visibility visibility() const { return visibility_; }
};

} // end namespace ast
} // end namespace chpl

#endif
