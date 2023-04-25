/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
#include "chpl/types/CStringType.h"
#include "chpl/types/ClassType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/DomainType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/NilType.h"
#include "chpl/types/NothingType.h"
#include "chpl/types/PrimitiveType.h"
#include "chpl/types/RealType.h"
#include "chpl/types/RecordType.h"
#include "chpl/types/UintType.h"
#include "chpl/types/UnknownType.h"
#include "chpl/types/TupleType.h"
#include "chpl/types/VoidType.h"

namespace chpl {
namespace types {


Type::~Type() {
}

static void gatherType(Context* context,
                       std::unordered_map<UniqueString,const Type*>& map,
                       const char* c_str,
                       const Type* t) {
  auto name = UniqueString::get(context, c_str);
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
  gatherType(context, map, "complex", ComplexType::get(context, 0));

  gatherType(context, map, "_any", AnyType::get(context));
  gatherType(context, map, "_nilType", NilType::get(context));
  gatherType(context, map, "_unknown", UnknownType::get(context));
  gatherType(context, map, "c_string", CStringType::get(context));
  gatherType(context, map, "nothing", NothingType::get(context));
  gatherType(context, map, "void", VoidType::get(context));

  gatherType(context, map, "object", BasicClassType::getObjectType(context));

  gatherType(context, map, "_tuple", TupleType::getGenericTupleType(context));

  auto bytesType = CompositeType::getBytesType(context);
  gatherType(context, map, "bytes", bytesType);
  gatherType(context, map, "_bytes", bytesType);
  auto stringType = CompositeType::getStringType(context);
  gatherType(context, map, "string", stringType);
  gatherType(context, map, "_string", stringType);

  gatherType(context, map, "Error", CompositeType::getErrorType(context));

  gatherType(context, map, "domain", DomainType::getGenericDomainType(context));

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

void Type::stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
  int leadingSpaces = 0;
  for (int i = 0; i < leadingSpaces; i++) {
    ss << "  ";
  }
  ss << "type ";
  ss << typetags::tagToString(this->tag());
}

IMPLEMENT_DUMP(Type);

bool Type::isStringType() const {
  if (auto rec = toRecordType()) {
    if (rec->name() == USTR("string"))
      return true;
  }
  return false;
}

bool Type::isBytesType() const {
  if (auto rec = toRecordType()) {
    if (rec->name() == USTR("bytes"))
      return true;
  }
  return false;
}

bool Type::isNilablePtrType() const {
  if (isPtrType()) {

    if (auto ct = toClassType()) {
      if (!ct->decorator().isNilable())
        return false;
    }

    return true;
  }

  return false;
}

bool Type::isUserRecordType() const {
  if (!isRecordType())
    return false;

  // TODO: add exceptions in here
  // for types implemented as records but where that
  // isn't the user's view -- e.g.
  //  * string, bytes, distribution, domain, array, range
  //    tuple, sync, single, atomic, managed pointer

  return true;
}


const CompositeType* Type::getCompositeType() const {
  if (auto at = toCompositeType())
    return at;

  if (auto ct = toClassType())
    return ct->basicClassType();

  return nullptr;
}


} // end namespace types
} // end namespace chpl
