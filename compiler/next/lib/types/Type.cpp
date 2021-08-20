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

#include "chpl/types/Type.h"

#include "chpl/types/AnyType.h"
#include "chpl/types/BoolType.h"
#include "chpl/types/BuiltinType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/PrimitiveType.h"
#include "chpl/types/RealType.h"
#include "chpl/types/UintType.h"
#include "chpl/types/UnknownType.h"

namespace chpl {
namespace types {


Type::~Type() {
}

static void gatherType(Context* context,
                       std::unordered_map<UniqueString,const Type*>& map,
                       const char* c_str,
                       const Type* t) {
  auto name = UniqueString::build(context, c_str);
  map.insert( {name, t} );
}

static
void gatherPrimitiveType(Context* context,
                         std::unordered_map<UniqueString,const Type*>& map,
                         const PrimitiveType* p) {
  gatherType(context, map, p->c_str(), p);
}

void Type::gatherBuiltins(Context* context,
                          std::unordered_map<UniqueString,const Type*>& map) {

  gatherPrimitiveType(context, map, BoolType::get(context, 0));
  gatherPrimitiveType(context, map, BoolType::get(context, 8));
  gatherPrimitiveType(context, map, BoolType::get(context, 16));
  gatherPrimitiveType(context, map, BoolType::get(context, 32));
  gatherPrimitiveType(context, map, BoolType::get(context, 64));

  gatherPrimitiveType(context, map, IntType::get(context, 8));
  gatherPrimitiveType(context, map, IntType::get(context, 16));
  gatherPrimitiveType(context, map, IntType::get(context, 32));
  gatherPrimitiveType(context, map, IntType::get(context, 64));
  // and 'int' as an alias for 'int(64)'
  gatherType(context, map, "int", IntType::get(context, 0));

  gatherPrimitiveType(context, map, UintType::get(context, 8));
  gatherPrimitiveType(context, map, UintType::get(context, 16));
  gatherPrimitiveType(context, map, UintType::get(context, 32));
  gatherPrimitiveType(context, map, UintType::get(context, 64));
  // and 'uint' as an alias for 'uint(64)'
  gatherType(context, map, "uint", UintType::get(context, 0));

  gatherPrimitiveType(context, map, RealType::get(context, 32));
  gatherPrimitiveType(context, map, RealType::get(context, 64));
  // and 'real' as an alias for 'real(64)'
  gatherType(context, map, "real", RealType::get(context, 0));

  gatherPrimitiveType(context, map, ImagType::get(context, 32));
  gatherPrimitiveType(context, map, ImagType::get(context, 64));
  // and 'imag' as an alias for 'imag(64)'
  gatherType(context, map, "imag", ImagType::get(context, 0));

  gatherPrimitiveType(context, map, ComplexType::get(context, 64));
  gatherPrimitiveType(context, map, ComplexType::get(context, 128));
  // and 'complex' as an alias for 'complex(128)'
  gatherType(context, map, "imag", ImagType::get(context, 0));

  gatherType(context, map, "_any", AnyType::get(context));
  gatherType(context, map, "_unknown", UnknownType::get(context));

  BuiltinType::gatherBuiltins(context, map);
}

bool Type::completeMatch(const Type* other) const {
  const Type* lhs = this;
  const Type* rhs = other;
  if (lhs->tag() != rhs->tag())
    return false;
  if (!lhs->contentsMatchInner(rhs))
    return false;

  return true;
}

bool Type::updateType(owned<Type>& keep, owned<Type>& addin) {
  if (keep->completeMatch(addin.get())) {
    // no changes are necessary
    return false;
  } else {
    // swap the AST
    keep.swap(addin);
    return true;
  }
}

void Type::markType(Context* context, const Type* keep) {
  if (keep == nullptr) return;
  // run markUniqueStrings on the node
  keep->markUniqueStringsInner(context);
}

void Type::dump(const Type* type, int leadingSpaces) {
  for (int i = 0; i < leadingSpaces; i++) {
    printf("  ");
  }

  printf("type %s \n", typetags::tagToString(type->tag()));
}

std::string Type::toString() const {
  std::string ret = typetags::tagToString(tag());
  return ret;
}


} // end namespace types
} // end namespace chpl
