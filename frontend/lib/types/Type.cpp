/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/query-impl.h"
#include "chpl/types/AnyClassType.h"
#include "chpl/types/AnyType.h"
#include "chpl/types/BoolType.h"
#include "chpl/types/BuiltinType.h"
#include "chpl/types/CStringType.h"
#include "chpl/types/ClassType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/CPtrType.h"
#include "chpl/types/DomainType.h"
#include "chpl/types/HeapBufferType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/NilType.h"
#include "chpl/types/NothingType.h"
#include "chpl/types/PrimitiveType.h"
#include "chpl/types/PtrType.h"
#include "chpl/types/RealType.h"
#include "chpl/types/RecordType.h"
#include "chpl/types/UintType.h"
#include "chpl/types/UnknownType.h"
#include "chpl/types/TupleType.h"
#include "chpl/types/VoidType.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"

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

  gatherPrimitiveType(context, map, BoolType::get(context));

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
  gatherType(context, map, "chpl_c_string", CStringType::get(context));
  gatherType(context, map, "nothing", NothingType::get(context));
  gatherType(context, map, "void", VoidType::get(context));
  gatherType(context, map, "_ddata", HeapBufferType::get(context));

  gatherType(context, map, "RootClass", BasicClassType::getRootClassType(context));

  gatherType(context, map, "_tuple", TupleType::getGenericTupleType(context));

  auto bytesType = CompositeType::getBytesType(context);
  gatherType(context, map, "bytes", bytesType);
  gatherType(context, map, "_bytes", bytesType);
  auto stringType = CompositeType::getStringType(context);
  gatherType(context, map, "string", stringType);
  gatherType(context, map, "_string", stringType);
  auto localeType = CompositeType::getLocaleType(context);
  gatherType(context, map, "locale", localeType);
  gatherType(context, map, "_locale", localeType);
  gatherType(context, map, "chpl_localeID_t", CompositeType::getLocaleIDType(context));

  auto rangeType = CompositeType::getRangeType(context);
  gatherType(context, map, "range", rangeType);
  gatherType(context, map, "_range", rangeType);

  gatherType(context, map, "Error", CompositeType::getErrorType(context));

  gatherType(context, map, "domain", DomainType::getGenericDomainType(context));

  gatherType(context, map, "class", AnyClassType::get(context));
  auto genericBorrowed = ClassType::get(context, AnyClassType::get(context), nullptr, ClassTypeDecorator(ClassTypeDecorator::BORROWED));
  gatherType(context, map, "borrowed", genericBorrowed);
  auto genericUnmanaged = ClassType::get(context, AnyClassType::get(context), nullptr, ClassTypeDecorator(ClassTypeDecorator::UNMANAGED));
  gatherType(context, map, "unmanaged", genericUnmanaged);

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
  if (stringKind != chpl::StringifyKind::CHPL_SYNTAX) {
    ss << "type ";
  }
  ss << typetags::tagToString(this->tag());
}

IMPLEMENT_DUMP(Type);

bool Type::isStringType() const {
  if (auto rec = toRecordType()) {
    if (rec->id().symbolPath() == USTR("String._string"))
      return true;
  }
  return false;
}

bool Type::isBytesType() const {
  if (auto rec = toRecordType()) {
    if (rec->id().symbolPath() == USTR("Bytes._bytes"))
      return true;
  }
  return false;
}

bool Type::isLocaleType() const {
  if (auto rec = toRecordType()) {
    if (rec->id().symbolPath() == USTR("ChapelLocale._locale"))
      return true;
  }
  return false;
}

bool Type::isNilablePtrType() const {
  if (isPointerLikeType()) {

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

bool Type::hasPragma(Context* context, uast::pragmatags::PragmaTag p) const {
  if (auto ct = this->toCompositeType()) {
    if (auto id = ct->id()) {
      if (auto ag = parsing::idToAttributeGroup(context, id)) {
        return ag->hasPragma(p);
      }
    }
  }
  return false;
}

bool Type::isRecordLike() const {
  if (auto ct = this->toClassType()) {
    auto decorator = ct->decorator();
    // no action needed for 'borrowed' or 'unmanaged'
    // (these should just default initialized to 'nil',
    //  so nothing else needs to be resolved)
    if (!(decorator.isBorrowed() || decorator.isUnmanaged() ||
          decorator.isUnknownManagement())) {
      return true;
    }
  } else if (this->isRecordType() || this->isUnionType()) {
    return true;
  }
  // TODO: tuples?

  return false;
}

const CompositeType* Type::getCompositeType() const {
  if (auto at = toCompositeType())
    return at;

  if (auto ct = toClassType())
    return ct->manageableType();

  return nullptr;
}

static bool
compositeTypeIsPod(Context* context, const Type* t) {
  using namespace resolution;

  if (auto cls = t->toClassType()) {
    return !cls->decorator().isManaged();
  }

  auto ct = t->getCompositeType();
  if (!ct) return false;

  const uast::AstNode* ast = nullptr;
  if (auto id = ct->id()) ast = parsing::idToAst(context, std::move(id));

  auto& rf = fieldsForTypeDecl(context, ct, DefaultsPolicy::USE_DEFAULTS);
  for (int i = 0; i < rf.numFields(); i++) {
    auto qt = rf.fieldType(i);
    if (auto ft = qt.type()) {
      if (qt.kind() == QualifiedType::PARAM ||
          qt.kind() == QualifiedType::TYPE) continue;
      if (!Type::isPod(context, ft)) return false;
    } else {
      return false;
    }
  }

  if (auto tfs = tryResolveDeinit(context, ast, t)) {
    if (!tfs->isCompilerGenerated()) return false;
  }
  if (auto tfs = tryResolveInitEq(context, ast, t, t)) {
    if (!tfs->isCompilerGenerated()) return false;
  }
  if (auto tfs = tryResolveAssign(context, ast, t, t)) {
    if (!tfs->isCompilerGenerated()) return false;
  }

  return true;
}

static const bool&
compositeTypeIsPodQuery(Context* context, const Type* t) {
  QUERY_BEGIN(compositeTypeIsPodQuery, context, t);
  bool ret = compositeTypeIsPod(context, t);
  return QUERY_END(ret);
}

bool Type::isPod(Context* context, const Type* t) {
  if (t->isUnknownType() || t->isErroneousType() ||
      t->isAnyType()) return false;
  if (t->hasPragma(context, uast::PRAGMA_POD)) return true;
  if (t->hasPragma(context, uast::PRAGMA_IGNORE_NOINIT)) return false;
  if (t->hasPragma(context, uast::PRAGMA_ATOMIC_TYPE)) return false;
  if (t->hasPragma(context, uast::PRAGMA_SYNC)) return false;
  if (t->isDomainType()) return false;
  if (t->isArrayType()) return false;
  if (auto cls = t->toClassType()) {
    if (cls->decorator().isManaged()) return false;
  }
  // TODO: We might like to be able to mark something as POD if it contains
  // all marked-as-POD members (e.g., all ranges) even if it is generic.
  // Currently, we can't do that, because call resolution can't get far
  // when given a generic actual.
  auto g = resolution::getTypeGenericity(context, t);
  if (g != Type::CONCRETE) return false;
  if (t->getCompositeType()) return compositeTypeIsPodQuery(context, t);
  return true;
}

bool Type::needsInitDeinitCall(const Type* t) {
  if (t == nullptr || t->isUnknownType() || t->isErroneousType()) {
    // can't do anything with these
    return false;
  } else if (t->isPrimitiveType() || t->isBuiltinType() || t->isCStringType() ||
             t->isNilType() || t->isNothingType() || t->isVoidType()) {
    // OK, we can always default initialize primitive numeric types,
    // and as well we assume that for the non-generic builtin types
    // e.g. TaskIdType.
    // No need to resolve anything additional now.
    return false;
  } else if (t->isEnumType()) {
    // OK, can default-initialize enums to first element
    return false;
  } else if (t->isFunctionType()) {
    return false;
  }

  return t->isRecordLike();
}

} // end namespace types
} // end namespace chpl
