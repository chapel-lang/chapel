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

#ifndef CHPL_TYPES_TYPE_H
#define CHPL_TYPES_TYPE_H

#include "chpl/queries/Context.h"
#include "chpl/queries/update-functions.h"
#include "chpl/types/TypeTag.h"

#include <deque>

namespace chpl {
namespace uast {
  class Decl;
}
namespace types {


// forward declare the various Type subclasses
// using macros and type-classes-list.h
/// \cond DO_NOT_DOCUMENT
#define TYPE_DECL(NAME) class NAME;
#define TYPE_NODE(NAME) TYPE_DECL(NAME)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) TYPE_DECL(NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME) TYPE_DECL(NAME)
#define TYPE_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to type-classes-list.h
#include "chpl/types/type-classes-list.h"
// clear the macros
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef TYPE_DECL


/**
  This is the base class for classes that represent a type.

  Functions like someType->isRecord() / someType->toRecord() are available and
  generated for all Type subclasses.

 */
class Type {
 public:
  enum Genericity {
    /** A concrete type e.g. 'int' */
    CONCRETE,

    /** A generic type, e.g. 'integral' */
    GENERIC,

    /** A type that is generic but has a default for each generic field.
        E.g. 'record R { type t = int; }'.
      */
    GENERIC_WITH_DEFAULTS,

    /** When we need more information to decide if the type is generic.
        It might depend on fields, e.g. */
    MAYBE_GENERIC,
  };

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
   stored as fields.
   */
  virtual void markUniqueStringsInner(Context* context) const = 0;

 public:
  /**
   This function returns an enum Genericity value to indicate
   if the type is concrete, generic, generic with defaults,
   or maybe generic (e.g. depending on the fields).

   This function needs to be defined by subclasses.
   */
  virtual Genericity genericity() const = 0;

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

  bool operator==(const Type& other) const {
    (void)tag_;  // quiet nextLinter
    return completeMatch(&other);
  }
  bool operator!=(const Type& other) const {
    return !(*this == other);
  }
  template<typename T>
  static bool update(owned<T>& keep, owned<T>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    return markUniqueStringsInner(context);
  }

  static void gatherBuiltins(Context* context,
                             std::unordered_map<UniqueString,const Type*>& map);

  bool completeMatch(const Type* other) const;

  virtual void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  // define is__ methods for the various Type subclasses
  // using macros and type-classes-list.h
  /// \cond DO_NOT_DOCUMENT
  #define TYPE_IS(NAME) \
    bool is##NAME() const { \
      return typetags::is##NAME(this->tag_); \
    }
  #define TYPE_NODE(NAME) TYPE_IS(NAME)
  #define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) TYPE_IS(NAME)
  #define TYPE_BEGIN_SUBCLASSES(NAME) TYPE_IS(NAME)
  #define TYPE_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to type-classes-list.h
  #include "chpl/types/type-classes-list.h"
  // clear the macros
  #undef TYPE_NODE
  #undef BUILTIN_TYPE_NODE
  #undef TYPE_BEGIN_SUBCLASSES
  #undef TYPE_END_SUBCLASSES
  #undef TYPE_IS

  // Additional helper functions
  // Don't name these queries 'isAny...'.
  // Why? Consider an example.
  // AnyNumericType is a builtin type called 'numeric' in the source code.
  // So, isAnyNumericType checks if the type is that builtin type 'numeric'.
  // In contrast, isNumericType checks to see if the type is one of the
  // numeric types.

  /** returns true if this represents the string type */
  bool isStringType() const;

  /** returns true if this represents the bytes type */
  bool isBytesType() const;

  /** returns true if it's string, bytes, or c_string type */
  bool isStringLikeType() const {
    return isStringType() || isBytesType() || isCStringType();
  }
  /** returns true if it's an int or uint type of any width */
  bool isIntegralType() const {
    return isIntType() || isUintType();
  }
  /** returns true if it's a numeric type of any width; that includes
      int, uint, real, imag, complex */
  bool isNumericType() const {
    return isIntType() || isUintType() ||
           isRealType() || isImagType() ||
           isComplexType();
  }

  /** returns true if it's a numeric type or bool type
      of any width */
  bool isNumericOrBoolType() const {
    return isNumericType() || isBoolType();
  }

  /** returns true for a type that is a kind of pointer */
  bool isPtrType() const {
    return isClassType() || isCFnPtrType() || isCVoidPtrType();
  }

  /** returns true for a pointer type that can store nil */
  bool isNilablePtrType() const;

  /** Returns true for a type that is a user-defined record.
      The compiler treats some internal types as records
      but the language design does not insist that they are.
   */
  bool isUserRecordType() const;

  /** If 'this' is a CompositeType, return it.
      If 'this' is a ClassType, return the basicClassType.
      Otherwise, returns nullptr.
   */
  const CompositeType* getCompositeType() const;

  // define to__ methods for the various Type subclasses
  // using macros and type-classes-list.h
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
  #define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) TYPE_TO(NAME)
  #define TYPE_BEGIN_SUBCLASSES(NAME) TYPE_TO(NAME)
  #define TYPE_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to type-classes-list.h
  #include "chpl/types/type-classes-list.h"
  // clear the macros
  #undef TYPE_NODE
  #undef BUILTIN_TYPE_NODE
  #undef TYPE_BEGIN_SUBCLASSES
  #undef TYPE_END_SUBCLASSES
  #undef TYPE_TO

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};


} // end namespace types


} // end namespace chpl

// TODO: is there a reasonable way to define std::less on Type*?
// Comparing pointers would lead to some nondeterministic ordering.

#endif
