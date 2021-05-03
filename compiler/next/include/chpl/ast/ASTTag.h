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

#ifndef CHPL_AST_ASTTAG_H
#define CHPL_AST_ASTTAG_H

namespace chpl {
namespace uast {
namespace asttags {


/**

  This enum is used to identify which AST class a node is.
 */
enum ASTTag {
  // define the enum for all of the non-virtual AST nodes
  // using macros and ASTClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define AST_NODE(NAME) NAME ,
  #define AST_LEAF(NAME) NAME ,
  #define AST_BEGIN_SUBCLASSES(NAME) START_##NAME ,
  #define AST_END_SUBCLASSES(NAME) END_##NAME ,
  /// \endcond
  // Apply the above macros to ASTClassesList.h
  #include "chpl/ast/ASTClassesList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  NUM_AST_TAGS
};

// define is___ for leaf and regular nodes
// (not yet for abstract parent classes)
/// \cond DO_NOT_DOCUMENT
#define IS_AST(NAME) \
  static inline bool is##NAME(ASTTag tag) { \
    return tag == NAME; \
  }
#define AST_NODE(NAME) IS_AST(NAME)
#define AST_LEAF(NAME) IS_AST(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/ast/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_AST

// define is___ for abstract parent classes
/// \cond DO_NOT_DOCUMENT
#define IS_BASE_CLASS_AST(NAME) \
  static inline bool is##NAME(ASTTag tag) { \
    return START_##NAME < tag && tag < END_##NAME; \
  }
#define AST_NODE(NAME)
#define AST_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) IS_BASE_CLASS_AST(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/ast/ASTClassesList.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_BASE_CLASS_AST

const char* tagToString(ASTTag tag);


} // end namespace asttags

// Enable ASTTag to be used as chpl::uast::ASTTag
using chpl::uast::asttags::ASTTag;

} // end namespace uast
} // end namespace chpl

#endif
