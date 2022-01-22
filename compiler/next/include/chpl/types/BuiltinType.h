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

#ifndef CHPL_TYPES_BUILTINTYPE_H
#define CHPL_TYPES_BUILTINTYPE_H

#include "chpl/types/Type.h"

namespace chpl {
namespace types {


/**
  This class represents a builtin type that only needs a name to
  be used by resolution.

  For example, `numeric`, `borrowed`, `chpl_anyreal`.

  Types that are used very commonly (e.g. AnyType, UnknownType) should get
  their own classes.

  Types that have need for different fields (e.g. PrimitiveType) should get
  their own classes.

 */
class BuiltinType : public Type {
 protected:
  BuiltinType(TypeTag tag)
    : Type(tag) {
  }

  bool contentsMatchInner(const Type* other,
                          MatchAssumptions& assumptions) const override {
    return true;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  bool isGeneric() const override {
    return (int) tag() >= (int) typetags::AnyBoolType;
  }

 public:
  ~BuiltinType() = default;

  static void gatherBuiltins(Context* context,
                             std::unordered_map<UniqueString,const Type*>& map);

  /**
    Returns a C string for the name of this BuiltinType.
   */
  const char* c_str() const;
};

// define the subclasses using macros and BuiltinTypeList.h
/// \cond DO_NOT_DOCUMENT
#define TYPE_NODE(NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME)
#define TYPE_END_SUBCLASSES(NAME)

#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) \
  class NAME final : public BuiltinType { \
   private: \
    NAME() : BuiltinType(typetags::NAME) { } \
    static const owned<NAME>& get##NAME(Context* context); \
   public: \
    ~NAME() = default; \
    static const NAME* get(Context* context) { \
      return get##NAME(context).get(); \
    } \
  };
/// \endcond

// Apply the above macros to TypeClassesList.h
#include "chpl/types/TypeClassesList.h"

// clear the macros
#undef TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef BUILTIN_TYPE_NODE


} // end namespace uast
} // end namespace chpl

#endif
