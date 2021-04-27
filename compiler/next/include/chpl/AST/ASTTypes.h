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

#ifndef CHPL_AST_ASTTYPES_H
#define CHPL_AST_ASTTYPES_H

#include "chpl/Util/memory.h"

#include <vector>

namespace chpl {
namespace ast {

// forward declare the various AST types
// using macros and ASTClassesList.h
/// \cond DO_NOT_DOCUMENT
#define AST_DECL(NAME) class NAME;
#define AST_NODE(NAME) AST_DECL(NAME)
#define AST_LEAF(NAME) AST_DECL(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_DECL(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/AST/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_DECL

// forward declare other classes
class BaseAST;
class Builder;

/**
  ASTList is just a list that owns some AST nodes.
 */
using ASTList = std::vector<owned<BaseAST>>;

static inline ASTList makeASTList(owned<BaseAST> ast) {
  ASTList lst;
  lst.push_back(std::move(ast));
  return lst;
}

/**
 Update an AST list with some replacement AST.

 It's kindof like swapping 'keep' and 'addin' but it tries
 to keep old AST nodes when they are the same. This allows
 for more reuse of results in the query framework.

 'keep' is some old AST
 'addin' is some new AST we wish to replace it with

 on exit, 'keep' stores the AST we need to 'keep', and anything
 not kept is stored in 'addin'.

 The function returns 'true' if anything changed in 'keep'.
 */
bool updateASTList(ASTList& keep, ASTList& addin);

} // end namespace ast
} // end namespace chpl

#endif
