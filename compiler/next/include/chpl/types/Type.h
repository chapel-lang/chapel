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

#ifndef CHPL_TYPES_TYPE_H
#define CHPL_TYPES_TYPE_H

#include "chpl/queries/Context.h"
#include "chpl/types/TypeClasses.h"
#include "chpl/types/TypeTag.h"

namespace chpl {
namespace types {


/**
  This is the base class for classes that represent a type.

  Functions like someType->isRecord() / someAst->toRecord() are available and
  generated for all Type subclasses.

 */
class Type {
 private:
  TypeTag tag_;

 protected:
  /**
    This function needs to be defined by subclasses.
    It should check only those fields defined in subclasses
    (it should not check the Type fields such as tag_).
    It can assume that other has the same type as the receiver.
   */
  virtual bool contentsMatchInner(const Type* other) const = 0;

  /**
   This function needs to be defined by subclasses.
   It should call the 'mark' method on any UniqueStrings
   stored as fields. It need not worry about the children nodes
   or the UniqueStrings stored in the ID.
   */
  virtual void markUniqueStringsInner(Context* context) const = 0;

 public:
  /**
   This function needs to be defined by subclasses.
   It returns 'true` if the type represents a generic type.
   */
  virtual bool isGeneric() = 0;

 protected:
  Type(TypeTag tag)
    : tag_(tag) {
  }

 public:
  virtual ~Type() = 0; // this is an abstract base class

  /**
    Returns the tag indicating which Type subclass this is.
   */
  TypeTag tag() const {
    return tag_;
  }

  bool completeMatch(const Type* other) const;

  // 'keep' is some old Type
  // 'addin' is some new Type we wish to combine with it
  //
  // on exit, 'keep' stores the Type we need to keep, and anything
  // not kept is stored in 'addin'.
  // the function returns 'true' if anything changed in 'keep'.
  static bool updateType(owned<Type>& keep, owned<Type>& addin);

  static void markType(Context* context, const Type* keep);

  static void dump(const Type* type, int leadingSpaces=0);

  // define is__ methods for the various Type subclasses
  // using macros and TypeClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define TYPE_IS(NAME) \
    bool is##NAME() const { \
      return typetags::is##NAME(this->tag_); \
    }
  #define TYPE_NODE(NAME) TYPE_IS(NAME)
  #define TYPE_BEGIN_SUBCLASSES(NAME) TYPE_IS(NAME)
  #define TYPE_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to TypeClassesList.h
  #include "chpl/types/TypeClassesList.h"
  // clear the macros
  #undef TYPE_NODE
  #undef TYPE_BEGIN_SUBCLASSES
  #undef TYPE_END_SUBCLASSES
  #undef TYPE_IS

  // define to__ methods for the various Type subclasses
  // using macros and TypeClassesList.h
  // Note: these offer equivalent functionality to C++ dynamic_cast<DstType*>
  /// \cond DO_NOT_DOCUMENT
  #define TYPE_TO(NAME) \
    const NAME * to##NAME() const { \
      return this->is##NAME() ? (const NAME *)this : nullptr; \
    } \
    NAME * to##NAME() { \
      return this->is##NAME() ? (NAME *)this : nullptr; \
    }
  #define TYPE_NODE(NAME) TYPE_TO(NAME)
  #define TYPE_BEGIN_SUBCLASSES(NAME) TYPE_TO(NAME)
  #define TYPE_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to TypeClassesList.h
  #include "chpl/types/TypeClassesList.h"
  // clear the macros
  #undef TYPE_NODE
  #undef TYPE_BEGIN_SUBCLASSES
  #undef TYPE_END_SUBCLASSES
  #undef TYPE_TO
};


} // end namespace uast
} // end namespace chpl

// TODO: is there a reasonable way to define std::less on Type*?
// Comparing pointers would lead to some nondeterministic ordering.

#endif
