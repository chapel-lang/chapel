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

#include "chpl/types/BuiltinType.h"
#include "chpl/queries/query-impl.h"

namespace chpl {
namespace types {

// implement the subclasses using macros and TypeClassesList.h
#define TYPE_NODE(NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME)
#define TYPE_END_SUBCLASSES(NAME)

#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) \
  const owned<NAME>& NAME::get##NAME(Context* context) { \
    QUERY_BEGIN(get##NAME, context); \
    auto result = toOwned(new NAME()); \
    return QUERY_END(result); \
  }

// Apply the above macros to TypeClassesList.h
#include "chpl/types/TypeClassesList.h"

// clear the macros
#undef TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef BUILTIN_TYPE_NODE

static void gatherType(Context* context,
                       std::unordered_map<UniqueString,const Type*>& map,
                       const BuiltinType* t) {
  auto name = UniqueString::build(context, t->c_str());
  map.insert( {name, t} );
}

void BuiltinType::gatherBuiltins(Context* context,
                                 std::unordered_map<UniqueString,const Type*>& map) {

  // call gatherType for each Kind using macros and TypeClassesList.h
  #define TYPE_NODE(NAME)
  #define TYPE_BEGIN_SUBCLASSES(NAME)
  #define TYPE_END_SUBCLASSES(NAME)

  #define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) \
    gatherType(context, map, NAME::get(context));

  // Apply the above macros to TypeClassesList.h
  #include "chpl/types/TypeClassesList.h"

  // clear the macros
  #undef TYPE_NODE
  #undef TYPE_BEGIN_SUBCLASSES
  #undef TYPE_END_SUBCLASSES
  #undef BUILTIN_TYPE_NODE
}

const char* BuiltinType::c_str() const {
  // create switch statement using macros and TypeClassesList.h
  #define TYPE_NODE(NAME)
  #define TYPE_BEGIN_SUBCLASSES(NAME)
  #define TYPE_END_SUBCLASSES(NAME)

  #define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) \
    case typetags::NAME: return CHPL_NAME_STR;

  switch (tag()) {
    // Apply the above macros to TypeClassesList.h
    #include "chpl/types/TypeClassesList.h"
    // Default case should not be reachable
    default:
      assert(false && "should not be reachable");
      return "<unknown builtin type>";
  }

  // clear the macros
  #undef TYPE_NODE
  #undef TYPE_BEGIN_SUBCLASSES
  #undef TYPE_END_SUBCLASSES
  #undef BUILTIN_TYPE_NODE

}


} // end namespace types
} // end namespace chpl
