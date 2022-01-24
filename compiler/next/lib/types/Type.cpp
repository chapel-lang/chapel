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

#include "chpl/types/Type.h"

#include "chpl/types/AnyType.h"
#include "chpl/types/BoolType.h"
#include "chpl/types/BuiltinType.h"
#include "chpl/types/BytesType.h"
#include "chpl/types/CStringType.h"
#include "chpl/types/ClassType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/NilType.h"
#include "chpl/types/NothingType.h"
#include "chpl/types/PrimitiveType.h"
#include "chpl/types/RealType.h"
#include "chpl/types/StringType.h"
#include "chpl/types/UintType.h"
#include "chpl/types/UnknownType.h"
#include "chpl/types/VoidType.h"

namespace chpl {
namespace types {


Type::~Type() {
}

void Type::dump() const {
  stringify(std::cerr, DEBUG_DETAIL);
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
  gatherType(context, map, "complex", ComplexType::get(context, 0));

  gatherType(context, map, "_any", AnyType::get(context));
  gatherType(context, map, "_nilType", NilType::get(context));
  gatherType(context, map, "_unknown", UnknownType::get(context));
  gatherType(context, map, "bytes", BytesType::get(context));
  gatherType(context, map, "c_string", CStringType::get(context));
  gatherType(context, map, "nothing", NothingType::get(context));
  gatherType(context, map, "string", StringType::get(context));
  gatherType(context, map, "void", VoidType::get(context));

  gatherType(context, map, "object", BasicClassType::getObjectType(context));

  BuiltinType::gatherBuiltins(context, map);
}

bool Type::MatchAssumptions::assume(const Type* t, const Type* otherT) {
  // Nothing else to do if the pointers are identical
  if (t == otherT)
    return true;

  // if one is null and the other is not, it's not a match
  if ((t == nullptr) != (otherT == nullptr))
    return false;

  // if they are both null, it is a match
  if (t == nullptr && otherT == nullptr)
    return true;

  // now we know that neither is null

  // if the tags do not match, it's not a match
  if (t->tag() != otherT->tag())
    return false;

  Assumption a(t, otherT);

  auto found = checked.find(a);
  if (found != checked.end()) {
    // Found an assumption that was already checked
    return true;
  } else {
    // Otherwise, add it to toCheck
    toCheck.push_back(a);
    // Also assign labels to t and otherT,
    // if they don't already have labels.
    int label = nextLabel;
    bool addedT = tLabels.emplace(t, label).second;
    bool addedOther = otherLabels.emplace(otherT, label).second;

    if (addedT != addedOther)
      return false; // suspect this is not possible

    // If we added something to the label maps, increment the label
    if (addedT) {
      nextLabel++;
    }
  }
  return true;
}

bool Type::completeMatch(const Type* other) const {
  if (this->tag() != other->tag())
    return false;

  MatchAssumptions assumptions;

  // initialize the algorithm
  //  * labels 'this' and 'other'
  //  * adds the pair to assumptions.toCheck
  if (!assumptions.assume(this, other))
    return false;

  // Check the assumptions in toCheck
  while (!assumptions.toCheck.empty()) {
    // Grab the first element and remove it
    MatchAssumptions::Assumption a = assumptions.toCheck.front();
    assumptions.toCheck.pop_front();

    // now check assumption 'a'

    // adding to set in 'assume' should ensure neither is nullptr
    assert(a.first != nullptr && a.second != nullptr);
    // and check the tag
    assert(a.first->tag() == a.second->tag());

    // add to 'checked' if it is not already there
    bool addedToChecked = assumptions.checked.insert(a).second;
    if (addedToChecked) {
      // just added to checked, so need to do the rest of the checking now

      // check that the labels match

      // should have already added labels, in 'assume'
      assert(assumptions.tLabels.count(a.first) > 0);
      assert(assumptions.otherLabels.count(a.second) > 0);

      if (assumptions.tLabels[a.first] != assumptions.otherLabels[a.second]) {
        // labels don't match, so it's not a match
        return false;
      }

      // Call contentsMatchInner, which which can call 'assume'
      // to add more assumptions to be checked.
      // If it returns 'false', we know immediately it is not a match.
      if (!a.first->contentsMatchInner(a.second, assumptions)) {
        return false;
      }
    }
  }

  // if we got through checking all of the assumptions, there is a match
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
