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

#ifndef CHPL_TYPE_TYPETAG_H
#define CHPL_TYPE_TYPETAG_H

namespace chpl {
namespace types {
namespace typetags {


/**

  This enum is used to identify which ASTType subclass a node is.
 */
enum TypeTag {
  // define the enum for all of the non-virtual Type nodes
  // using macros and TypeClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define TYPE_NODE(NAME) NAME ,
  #define TYPE_BEGIN_SUBCLASSES(NAME) START_##NAME ,
  #define TYPE_END_SUBCLASSES(NAME) END_##NAME ,
  /// \endcond
  // Apply the above macros to TypeClassesList.h
  #include "chpl/types/TypeClassesList.h"
  // clear the macros
  #undef TYPE_NODE
  #undef TYPE_BEGIN_SUBCLASSES
  #undef TYPE_END_SUBCLASSES
  NUM_TYPE_TAGS
};

// define is___ for regular nodes
// (not yet for abstract parent classes)
/// \cond DO_NOT_DOCUMENT
#define IS_TYPE(NAME) \
  static inline bool is##NAME(TypeTag tag) { \
    return tag == NAME; \
  }
#define TYPE_NODE(NAME) IS_TYPE(NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME)
#define TYPE_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to TypeClassesList.h
#include "chpl/types/TypeClassesList.h"
// clear the macros
#undef TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef IS_TYPE

// define is___ for abstract parent classes
/// \cond DO_NOT_DOCUMENT
#define IS_BASE_CLASS_TYPE(NAME) \
  static inline bool is##NAME(TypeTag tag) { \
    return START_##NAME < tag && tag < END_##NAME; \
  }
#define TYPE_NODE(NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME) IS_BASE_CLASS_TYPE(NAME)
#define TYPE_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to TYPEClassesList.h
#include "chpl/types/TypeClassesList.h"
// clear the macros
#undef TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef IS_BASE_CLASS_TYPE

const char* tagToString(TypeTag tag);


} // end namespace typetags

// Enable ASTTag to be used as chpl::types::TypeTag
using chpl::types::typetags::TypeTag;

} // end namespace types
} // end namespace chpl

#endif
