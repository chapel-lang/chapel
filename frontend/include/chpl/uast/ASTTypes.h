/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_ASTTYPES_H
#define CHPL_UAST_ASTTYPES_H

namespace chpl {
namespace uast {


// forward declare the various AST types
// using macros and uast-classes-list.h
/// \cond DO_NOT_DOCUMENT
#define AST_DECL(NAME) class NAME;
#define AST_NODE(NAME) AST_DECL(NAME)
#define AST_LEAF(NAME) AST_DECL(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_DECL(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to uast-classes-list.h
#include "chpl/uast/uast-classes-list.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_DECL

// forward declare other classes
class AstNode;
class Builder;


} // end namespace uast
} // end namespace chpl

#endif
