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

#ifndef CHPL_UAST_ASTTAG_H
#define CHPL_UAST_ASTTAG_H

#include "chpl/queries/mark-functions.h"
#include "chpl/queries/update-functions.h"
#include "chpl/queries/stringify-functions.h"

namespace chpl {
namespace uast {
namespace asttags {


/**

  This enum is used to identify which AST class a node is.
 */
enum AstTag {
  // define the enum for all of the non-virtual AST nodes
  // using macros and uast-classes-list.h
  /// \cond DO_NOT_DOCUMENT
  #define AST_NODE(NAME) NAME ,
  #define AST_LEAF(NAME) NAME ,
  #define AST_BEGIN_SUBCLASSES(NAME) START_##NAME ,
  #define AST_END_SUBCLASSES(NAME) END_##NAME ,
  /// \endcond
  // Apply the above macros to uast-classes-list.h
  #include "chpl/uast/uast-classes-list.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  NUM_AST_TAGS,
  AST_TAG_UNKNOWN
};

// define is___ for leaf and regular nodes
// (not yet for abstract parent classes)
/// \cond DO_NOT_DOCUMENT
#define IS_AST(NAME) \
  static inline bool is##NAME(AstTag tag) { \
    return tag == NAME; \
  }
#define AST_NODE(NAME) IS_AST(NAME)
#define AST_LEAF(NAME) IS_AST(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to uast-classes-list.h
#include "chpl/uast/uast-classes-list.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_AST

// define is___ for abstract parent classes
/// \cond DO_NOT_DOCUMENT
#define IS_BASE_CLASS_AST(NAME) \
  static inline bool is##NAME(AstTag tag) { \
    return START_##NAME < tag && tag < END_##NAME; \
  }
#define AST_NODE(NAME)
#define AST_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) IS_BASE_CLASS_AST(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to uast-classes-list.h
#include "chpl/uast/uast-classes-list.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef IS_BASE_CLASS_AST

const char* tagToString(AstTag tag);


} // end namespace asttags

// Enable AstTag to be used as chpl::uast::AstTag
using chpl::uast::asttags::AstTag;

} // end namespace uast

/// \cond DO_NOT_DOCUMENT
template<> struct update<uast::AstTag> {
  bool operator()(uast::AstTag& keep,
                  uast::AstTag& addin) const {
    return defaultUpdateBasic(keep, addin);
  }
};

template<> struct mark<uast::AstTag> {
  void operator()(Context* context, const uast::AstTag& keep) const {
    // nothing to do for enum
  }
};

template<> struct stringify<uast::AstTag> {
  void operator()(std::ostream& streamOut, StringifyKind stringKind,
                  uast::AstTag tag) const {
    streamOut << tagToString(tag);
  }
};

/// \endcond


} // end namespace chpl

namespace std {


template<> struct hash<chpl::uast::AstTag>
{
  size_t operator()(const chpl::uast::AstTag& key) const {
    return key;
  }
};


} // end namespace std


#endif
