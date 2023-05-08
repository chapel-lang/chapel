/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#define TYPE_EXTERN

#include "type.h"

#include "AggregateType.h"
#include "AstToText.h"
#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "fcf-support.h"
#include "DecoratedClassType.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "intents.h"
#include "intlimits.h"
#include "iterator.h"
#include "misc.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"
#include "vec.h"

#include "global-ast-vecs.h"

#include <cmath>

static bool isDerivedType(Type* type, Flag flag);

Type::Type(AstTag astTag, Symbol* iDefaultVal) : BaseAST(astTag) {
  symbol              = NULL;
  refType             = NULL;
  defaultValue        = iDefaultVal;
  destructor          = NULL;
  isInternalType      = false;
  scalarPromotionType = NULL;
}

const char* Type::name() const {
  return symbol->name;
}

void Type::verify() {
}

void Type::addSymbol(TypeSymbol* newSymbol) {
  symbol = newSymbol;
}

bool Type::inTree() {
  if (symbol && symbol->defPoint)
    return symbol->defPoint->inTree();
  else
    return false;
}

QualifiedType Type::qualType() {
  return QualifiedType(this);
}

// Are actuals of this type passed with const intent by default?
bool Type::isDefaultIntentConst() const {
  bool retval = true;

  // MPF 2017-03-09
  // It seems wrong to me that this returns true
  // for dtUnknown. However some parts of the compiler
  // currently rely on that behavior.

  if (symbol->hasFlag(FLAG_DEFAULT_INTENT_IS_REF) == true ||
      isReferenceType(this)                       == true ||
      isRecordWrappedType(this)                   == true)
    retval = false;

  return retval;
}

bool Type::isWidePtrType() const {
  if (symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS)) {
    // Workaround an ugly hack in insert wide references
    // which can make a wide _array record containing an "addr" record
    Type* baseType = this->getField("addr")->type;
    if (isReferenceType(baseType) || isClass(baseType) || baseType == dtNil)
      return true;
  }
  return false;
}

Symbol* Type::getField(const char* name, bool fatal) const {
  INT_FATAL(this, "getField not called on AggregateType");
  return NULL;
}

bool Type::hasDestructor() const {
  return destructor != NULL;
}

FnSymbol* Type::getDestructor() const {
  return destructor;
}

void Type::setDestructor(FnSymbol* fn) {
  destructor = fn;
}

bool Type::isSerializable() {
  if (isAggregateType(this)) {
    return serializeMap.find(this->getValType()) != serializeMap.end();
  }

  return false;
}

Symbol* Type::getSubstitutionWithName(const char* name) const {

  if (fVerify) {
    INT_ASSERT(name == astr(name));
  }

  if (this->substitutions.n > 0) {
    // should only exist during resolution
    form_Map(SymbolMapElem, e, this->substitutions) {
      if (e->key && e->key->name == name)
        return e->value;
    }
  }

  // after resolution (or possibly during)
  size_t n = this->substitutionsPostResolve.size();
  for (size_t i = 0; i < n; i++) {
    const NameAndSymbol& ns = this->substitutionsPostResolve[i];
    if (ns.name == name)
      return ns.value;
  }

  return NULL;
}

void Type::setSubstitutionWithName(const char* name, Symbol* value) {

  if (fVerify) {
    INT_ASSERT(name == astr(name));
  }

  size_t n = this->substitutionsPostResolve.size();
  for (size_t i = 0; i < n; i++) {
    NameAndSymbol& ns = this->substitutionsPostResolve[i];
    if (ns.name == name) {
      ns.value = value;
      return;
    }
  }

  // if none was found, we could add one, but that functionality
  // isn't currently used, so error.
  INT_FATAL("substitution not found");
}

const char* toString(Type* type, bool decorateAllClasses) {
  const char* retval = NULL;

  if (type == NULL ||
      type == dtUnknown ||
      type == dtSplitInitType) {
    retval = "<type unknown>";
  } else if (type == dtAny) {
    retval = "<any type>";
  } else if (auto fnType = toFunctionType(type)) {
    retval = fnType->toString();
  } else {
    Type* vt = type->getValType();

    if (AggregateType* at = toAggregateType(vt)) {
      const char* drDomName = "DefaultRectangularDom";
      const int   drDomNameLen = strlen(drDomName);

      if (isArrayClass(at) && !at->symbol->hasFlag(FLAG_BASE_ARRAY)) {
        Symbol* domField = at->getField("dom", false);
        Symbol* eltTypeField = at->getField("eltType", false);

        if (domField && eltTypeField) {
          Type* domainType = canonicalDecoratedClassType(domField->type);
          Type* eltType    = eltTypeField->type;

          if (domainType != dtUnknown && eltType != dtUnknown)
            retval = astr("[", toString(domainType,false), "] ",
                          toString(eltType));
        }

      } else if (strncmp(at->symbol->name, drDomName, drDomNameLen) == 0) {
        retval = astr("domain", at->symbol->name + drDomNameLen);

      } else if (at->symbol->hasFlag(FLAG_FUNCTION_CLASS)) {
        retval = fcfs::functionClassTypeToString(at);

      } else if (isRecordWrappedType(at) == true) {
        Symbol* instanceField = at->getField("_instance", false);

        if (instanceField) {
          Type* implType = canonicalDecoratedClassType(instanceField->type);

          if (implType != dtUnknown)
            retval = toString(implType, false);
          else if (at->symbol->hasFlag(FLAG_ARRAY))
            retval = astr("[]");
        }
      } else if (vt->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
        if (developer == false)
          retval = "iterator";
      } else if (at->symbol->hasFlag(FLAG_ATOMIC_TYPE) &&
                 (strcmp(at->symbol->name, "AtomicBool") == 0 ||
                  strcmp(at->symbol->name, "RAtomicBool") == 0)) {
        retval = astr("atomic bool");
      } else if (isManagedPtrType(vt)) {
        Type* borrowType = getManagedPtrBorrowType(vt);
        const char* borrowed = "borrowed ";
        const char* borrowName = toString(borrowType, false);
        if (startsWith(borrowName, borrowed)) {
          borrowName = borrowName + strlen(borrowed);
        }
        if (startsWith(vt->symbol->name, "_owned")) {
          if (borrowType == dtUnknown) {
            retval = "owned";
          } else {
            retval = astr("owned ", borrowName);
          }
        }
        else if (startsWith(vt->symbol->name, "_shared")) {
          if (borrowType == dtUnknown) {
            retval = "shared";
          } else {
            retval = astr("shared ", borrowName);
          }
        }

      } else if (isClassLike(at)) {
        if (isClass(at)) {
          // It's an un-decorated class type
          const char* borrowed = "borrowed ";
          const char* useName = vt->symbol->name;
          if (startsWith(useName, borrowed))
            useName = useName + strlen(borrowed);

          if (decorateAllClasses)
            useName = astr("borrowed ", useName);

          retval = useName;
        }
      }
    } else if (vt == dtCVoidPtr) {  // de-sugar chpl__c_void_ptr
      retval = "c_void_ptr";
    }

    if (retval == NULL)
      retval = vt->symbol->name;

  }

  return astr(retval);
}

/************************************* | **************************************
*                                                                             *
* Qualifier and QualifiedType                                                 *
*                                                                             *
************************************** | *************************************/

const char* qualifierToStr(Qualifier q) {
    switch (q) {
      case QUAL_UNKNOWN:
        return "unknown";

      case QUAL_CONST:
        return "const";
      case QUAL_REF:
        return "ref";
      case QUAL_CONST_REF:
        return "const-ref";

      case QUAL_PARAM:
        return "param";

      case QUAL_VAL:
        return "val";
      case QUAL_NARROW_REF:
        return "narrow-ref";
      case QUAL_WIDE_REF:
        return "wide-ref";

      case QUAL_CONST_VAL:
        return "const-val";
      case QUAL_CONST_NARROW_REF:
        return "const-narrow-ref";
      case QUAL_CONST_WIDE_REF:
        return "const-wide-ref";
    }

    INT_FATAL("Unhandled Qualifier");
    return "UNKNOWN-QUAL";
}

bool QualifiedType::isRefType() const {
  return _type->symbol->hasFlag(FLAG_REF);
}

bool QualifiedType::isWideRefType() const {
  return _type->symbol->hasFlag(FLAG_WIDE_REF);
}

const char* QualifiedType::qualStr() const {
  if (isRefType()) {
    if (_qual == QUAL_CONST_REF ||
        _qual == QUAL_CONST) {
      return qualifierToStr(QUAL_CONST_REF);
    } else {
      return qualifierToStr(QUAL_REF);
    }
  }

  if (isWideRefType())
    return qualifierToStr(QUAL_WIDE_REF);

  // otherwise
  return qualifierToStr(_qual);
}

QualifiedType QualifiedType::refToRefType() const {
  Qualifier qual = _qual;
  Type* type = _type;
  if (isRef() && !type->symbol->hasFlag(FLAG_REF)) {
    // Use a ref type here.
    // In the future, the Qualifier should be sufficient
    INT_ASSERT(type->refType != NULL);
    type = type->refType;
  }

  return QualifiedType(qual, type);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

PrimitiveType::PrimitiveType(Symbol *init, bool internalType) :
  Type(E_PrimitiveType, init)
{
  isInternalType = internalType;
  gPrimitiveTypes.add(this);
}


PrimitiveType*
PrimitiveType::copyInner(SymbolMap* map) {
  //
  // If we're trying to make a copy of an internal Chapel primitive
  // type (say 'int'), that's a sign that something is wrong.  For
  // external primitive types, it should be OK to make such copies.
  // This may be desired/required if the extern type declaration is
  // local to a generic Chapel procedure for example and we're
  // creating multiple instantiations of that procedure, each of which
  // wants/needs its own local type symbol.  This exception may
  // suggest that external primitive types should really be
  // represented as their own ExternType class...
  //
  if (!symbol->hasFlag(FLAG_EXTERN)) {
    INT_FATAL(this, "Unexpected call to PrimitiveType::copyInner");
  }

  return new PrimitiveType(NULL);
}


void PrimitiveType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in PrimitiveType::replaceChild");
}


void PrimitiveType::verify() {
  Type::verify();
  if (astTag != E_PrimitiveType) {
    INT_FATAL(this, "Bad PrimitiveType::astTag");
  }
}


void PrimitiveType::accept(AstVisitor* visitor) {
  visitor->visitPrimType(this);
}


ConstrainedType::ConstrainedType(ConstrainedTypeUse use) :
  Type(E_ConstrainedType, NULL), ctUse(use)
{
  gConstrainedTypes.add(this);
}

ConstrainedType* ConstrainedType::copyInner(SymbolMap* map) {
  return new ConstrainedType(ctUse);
}

void ConstrainedType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in ConstrainedType::replaceChild");
}

void ConstrainedType::verify() {
  Type::verify();
  INT_ASSERT(astTag == E_ConstrainedType);

  DefExpr* def = symbol->defPoint;  // assumes this->inTree()
  switch (ctUse) {
  case CT_IFC_FORMAL: {
    InterfaceSymbol* isym = toInterfaceSymbol(def->parentSymbol);
    INT_ASSERT(def->list == &(isym->ifcFormals));
    break;
  }
  case CT_IFC_ASSOC_TYPE: {
    InterfaceSymbol* isym = toInterfaceSymbol(def->parentSymbol);
    INT_ASSERT(def->parentExpr == isym->ifcBody);
    break;
  }
  case CT_CGFUN_FORMAL: {
    FnSymbol* fn = toFnSymbol(def->parentSymbol);
    INT_ASSERT(def->list == &(fn->interfaceInfo->constrainedTypes));
    break;
  }
  case CT_CGFUN_ASSOC_TYPE: {
    // These arise during resolution and are pruned at resolution end.
    INT_FATAL(this, "unexpected");
    break;
  }}
}

const char* ConstrainedType::useString() const {
  switch (ctUse) {
  case CT_IFC_FORMAL:       return "CT_IFC_FORMAL";
  case CT_IFC_ASSOC_TYPE:   return "CT_IFC_ASSOC_TYPE";
  case CT_CGFUN_FORMAL:     return "CT_CGFUN_FORMAL";
  case CT_CGFUN_ASSOC_TYPE: return "CT_CGFUN_ASSOC_TYPE";
  }
  INT_FATAL(this, "unknown ConstrainedType use");
  return NULL;
}

void ConstrainedType::accept(AstVisitor* visitor) {
  visitor->visitConstrainedType(this);
}

TypeSymbol* ConstrainedType::buildSym(const char* name,
                                      ConstrainedTypeUse use) {
  Type* ct = new ConstrainedType(use);
  return new TypeSymbol(name, ct);
}

ConstrainedType* ConstrainedType::buildType(const char* name,
                                         ConstrainedTypeUse use) {
  ConstrainedType* ct = new ConstrainedType(use);
  new TypeSymbol(name, ct); // attaches to 'ct'
  return ct;
}

bool isConstrainedType(Type* t, ConstrainedTypeUse use) {
  if (ConstrainedType* ct = toConstrainedType(t))
    if (ct->ctUse == use)
      return true;
  return false;
}

bool isConstrainedTypeSymbol(Symbol* s, ConstrainedTypeUse use) {
  if (TypeSymbol* ts = toTypeSymbol(s))
    if (isConstrainedType(ts->type, use))
      return true;
  return false;
}

EnumType::EnumType() :
  Type(E_EnumType, NULL),
  constants(), integerType(NULL),
  doc(NULL)
{
  gEnumTypes.add(this);
  constants.parent = this;
}

void EnumType::verify() {
  Type::verify();
  if (astTag != E_EnumType) {
    INT_FATAL(this, "Bad EnumType::astTag");
  }
  if (constants.parent != this)
    INT_FATAL(this, "Bad AList::parent in EnumType");
  for_alist(expr, constants) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad EnumType::constants::parentSymbol");
    if (!isDefExpr(expr))
      INT_FATAL(this, "Bad EnumType::constants - not a DefExpr");
  }
}


EnumType*
EnumType::copyInner(SymbolMap* map) {
  EnumType* copy = new EnumType();

  for_enums(def, this) {
    DefExpr* newDef = COPY_INT(def);
    newDef->sym->type = copy;
    copy->constants.insertAtTail(newDef);
  }
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumType::replaceChild");
}


bool EnumType::isAbstract() {
  for_enums(constant, this) {
    if (constant->init) {
      return false;
    }
  }
  return true;
}

bool EnumType::isConcrete() {
  // if the first constant has an initializer, it's concrete;
  // otherwise, it's not.  This loop with a guaranteed return is a
  // lazy way of getting that first constant.
  for_enums(constant, this) {
    if (constant->init) {
      return true;
    } else {
      return false;
    }
  }
  return true;
}


PrimitiveType* EnumType::getIntegerType() {
  INT_ASSERT(integerType);
  return integerType;
}


void EnumType::accept(AstVisitor* visitor) {
  if (visitor->enterEnumType(this) == true) {

    for_alist(next_ast, constants) {
      next_ast->accept(visitor);
    }

    visitor->exitEnumType(this);
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

namespace {
  using FormalVec = std::vector<FunctionType::Formal>;
}

FunctionType::FunctionType(Kind kind, FormalVec formals,
                           RetTag returnIntent,
                           Type* returnType,
                           bool throws,
                           bool isAnyFormalNamed,
                           const char* userTypeString)
    : Type(E_FunctionType, nullptr),
      kind_(kind),
      formals_(std::move(formals)),
      returnIntent_(returnIntent),
      returnType_(returnType),
      throws_(throws),
      isAnyFormalNamed_(isAnyFormalNamed),
      userTypeString_(userTypeString) {
}

void FunctionType::verify() {
  bool isAnyFormalNamed = false;

  for (auto& formal : formals_) {
    if (formal.name != nullptr) {
      isAnyFormalNamed = true;
      break;
    }
  }

  INT_ASSERT(isAnyFormalNamed == this->isAnyFormalNamed_);
}

void FunctionType::accept(AstVisitor* visitor) {
  visitor->visitFunctionType(this);
}

FunctionType* FunctionType::copyInner(SymbolMap* map) {
  INT_FATAL(this, "attempt to copy function type");
  return nullptr;
}

void
FunctionType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "unexpected case in 'FunctionType::%s'", __FUNCTION__);
}

const char*
FunctionType::buildUserFacingTypeString(FunctionType::Kind kind,
                                        const FormalVec& formals,
                                        RetTag returnIntent,
                                        Type* returnType,
                                        bool throws) {
  std::ostringstream oss;
  oss << FunctionType::kindToString(kind) << "(";

  for (size_t i = 0; i < formals.size(); i++) {
    auto& info = formals[i];
    bool skip = isIntentSameAsDefault(info.intent, info.type);
    if (!skip) oss << intentToString(info.intent);
    if (!skip && info.name) oss << " ";
    if (info.name) oss << info.name;
    if ((!skip || info.name) && info.type != dtAny) oss << ": ";
    if (info.type != dtAny) oss << typeToString(info.type);
    if ((i+1) != formals.size()) oss << ", ";
  }

  oss << ")";

  if (returnIntent != RET_VALUE) {
    oss << " " << returnIntentToString(returnIntent);
  }

  if (returnType != dtVoid) {
    oss << ": " << typeToString(returnType);
  }

  auto str = oss.str();
  auto ret = astr(str.c_str());
  return ret;
}

const char* FunctionType::kindToString(FunctionType::Kind kind) {
  switch (kind) {
    case PROC: return "proc";
    case ITER: return "iter";
    case OPERATOR: return "operator";
  }
  return nullptr;
}

const char* FunctionType::intentToString(IntentTag intent) {
  switch (intent) {
    case INTENT_IN: return "in";
    case INTENT_OUT: return "out";
    case INTENT_INOUT: return "inout";
    case INTENT_CONST: return "const";
    case INTENT_CONST_IN: return "const in";
    case INTENT_REF: return "ref";
    case INTENT_CONST_REF: return "const ref";
    case INTENT_REF_MAYBE_CONST: return nullptr;
    case INTENT_PARAM: return "param";
    case INTENT_TYPE: return "type";
    case INTENT_BLANK: return nullptr;
  }
  return nullptr;
}

const char* FunctionType::typeToString(Type* t) {
  auto vt = t->getValType();
  if (vt == dtInt[INT_SIZE_DEFAULT]) return "int";
  if (vt == dtUInt[INT_SIZE_DEFAULT]) return "uint";
  if (vt == dtReal[COMPLEX_SIZE_DEFAULT]) return "real";
  if (vt == dtBools[BOOL_SIZE_DEFAULT]) return "bool";
  if (vt == dtComplex[COMPLEX_SIZE_DEFAULT]) return "complex";
  if (vt == dtImag[FLOAT_SIZE_DEFAULT]) return "imag";
  auto ret = vt->symbol->cname;
  return ret;
}

const char* FunctionType::returnIntentToString(RetTag intent) {
  return retTagDescrString(intent);
}

// For the 'any' type, arbitrarily choose that only the default intent
// is the same as the default intent (what else makes sense?).
bool FunctionType::isIntentSameAsDefault(IntentTag intent, Type* t) {
  if (t == dtAny) return intent == INTENT_BLANK;
  auto ret = concreteIntent(INTENT_BLANK, t) == concreteIntent(intent, t);
  return ret;
}

FunctionType* FunctionType::create(FunctionType::Kind kind,
                                   FormalVec formals,
                                   RetTag returnIntent,
                                   Type* returnType,
                                   bool throws) {
  bool isAnyFormalNamed = false;

  for (auto& formal : formals) {
    isAnyFormalNamed |= formal.name != nullptr;
    formal.name = astr(formal.name);
  }

  auto cstr = FunctionType::buildUserFacingTypeString(kind, formals,
                                                      returnIntent,
                                                      returnType,
                                                      throws);
  auto ret = new FunctionType(kind, std::move(formals), returnIntent,
                              returnType,
                              throws,
                              isAnyFormalNamed,
                              cstr);
  return ret;
}

namespace {

  // Used to hash by value instead of doing pointer comparison.
  struct FunctionTypePtrHash {
    size_t operator()(const FunctionType* x) const {
      return x->hash();
    }
  };

  // Used to compare by value instead of doing pointer comparison.
  struct FunctionTypePtrEq {
    bool operator()(const FunctionType* lhs,
                    const FunctionType* rhs) const {
      return lhs->equals(rhs);
    }
  };

  using FunctionTypeCache =
    std::unordered_set<FunctionType*, FunctionTypePtrHash,
                       FunctionTypePtrEq>;
}

// Cache to make sure that we don't produce duplicate function types.
static FunctionTypeCache functionTypeCache;

static FunctionType* cacheFunctionTypeOrReuse(FunctionType* fnType) {
  auto it = functionTypeCache.find(fnType);
  if (it != functionTypeCache.end()) return *it;

  auto ts = new TypeSymbol(fnType->toString(), fnType);
  ts->cname = fnType->toStringMangledForCodegen();
  fnType->symbol = ts;

  rootModule->block->insertAtTail(new DefExpr(ts));

  std::ignore = functionTypeCache.emplace_hint(it, fnType);

  return fnType;
}

FunctionType* FunctionType::get(FunctionType::Kind kind,
                                FormalVec formals,
                                RetTag returnIntent,
                                Type* returnType,
                                bool throws) {
  auto fnType = FunctionType::create(kind, std::move(formals), returnIntent,
                                     returnType,
                                     throws);
  auto ret = cacheFunctionTypeOrReuse(fnType);
  return ret;
}

FunctionType::Kind FunctionType::determineKind(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_ITERATOR_FN)) return FunctionType::ITER;
  if (fn->hasFlag(FLAG_OPERATOR)) return FunctionType::OPERATOR;
  return FunctionType::PROC;
}

FunctionType* FunctionType::get(FnSymbol* fn) {
  FunctionType::Kind kind = determineKind(fn);
  std::vector<FunctionType::Formal> formals;
  RetTag returnIntent = fn->retTag;
  Type* returnType = fn->retType;
  bool throws = fn->throwsError();

  for_formals(f, fn) {
    FunctionType::Formal info;
    info.type = f->type;
    info.intent = f->intent;
    info.name = f->name;
    formals.push_back(std::move(info));
  }

  auto fnType = FunctionType::create(kind, std::move(formals), returnIntent,
                                     returnType,
                                     throws);
  auto ret = cacheFunctionTypeOrReuse(fnType);
  return ret;
}

FunctionType::Kind FunctionType::kind() const {
  return this->kind_;
}

int FunctionType::numFormals() const {
  auto ret = (int) this->formals_.size();
  return ret;
}

const FunctionType::Formal* FunctionType::formal(int idx) const {
  INT_ASSERT(0 <= idx && idx < numFormals());
  auto ret = &formals_[idx];
  return ret;
}

RetTag FunctionType::returnIntent() const {
  return this->returnIntent_;
}

Type* FunctionType::returnType() const {
  return this->returnType_;
}

bool FunctionType::throws() const {
  return this->throws_;
}

bool FunctionType::isAnyFormalNamed() const {
  return this->isAnyFormalNamed_;
}

const char* FunctionType::toString() const {
  return this->userTypeString_;
}

const char* FunctionType::intentTagMnemonicMangled(IntentTag tag) {
  switch (tag) {
    case INTENT_IN: return "I";
    case INTENT_OUT: return "O";
    case INTENT_INOUT: return "U";
    case INTENT_CONST: return "C";
    case INTENT_CONST_IN: return "CI";
    case INTENT_REF: return "R";
    case INTENT_CONST_REF: return "CR";
    case INTENT_REF_MAYBE_CONST: return "RMC";
    case INTENT_PARAM: return "P";
    case INTENT_TYPE: return "T";
    case INTENT_BLANK: return "";
  }
  return nullptr;
}

const char* FunctionType::retTagMnemonicMangled(RetTag tag) {
  switch (tag) {
    case RET_VALUE: return "";
    case RET_REF: return "R";
    case RET_CONST_REF: return "CR";
    case RET_PARAM: return "P";
    case RET_TYPE: return "T";
  }
  return nullptr;
}

const char* FunctionType::toStringMangledForCodegen() const {
  std::ostringstream oss;

  oss << "chpl_" << kindToString(kind_) << "_";

  for (int i = 0; i < numFormals(); i++) {
    auto f = this->formal(i);
    bool skip = isIntentSameAsDefault(f->intent, f->type);
    if (!skip) oss << intentTagMnemonicMangled(f->intent);
    oss << typeToString(f->type) << "_";
    if (f->name) oss << f->name;
    oss << "_";
  }

  oss << "_";
  if (returnIntent_ != RET_VALUE) {
    oss << retTagMnemonicMangled(returnIntent_) << "_";
  }

  oss << typeToString(returnType_);
  if (throws_) oss << "_throws";

  auto ret = astr(oss.str());
  return ret;
}

size_t FunctionType::hash() const {
  std::hash<void*> hasherPtr;
  std::hash<bool> hasherBool;

  size_t ret = ((size_t) kind_);

  // I think it's fine to hash the pointers here because types don't really
  // have a meaningful way to distinguish on contents, and should be unique,
  // while the formal names are all canonical using 'astr'.
  for (auto& formal : formals_) {
    ret = chpl::hash_combine(ret, formal.hash());
  }

  ret = chpl::hash_combine(ret, ((size_t) returnIntent_));
  ret = chpl::hash_combine(ret, hasherPtr(returnType_));
  ret = chpl::hash_combine(ret, hasherBool(throws_));
  ret = chpl::hash_combine(ret, hasherBool(isAnyFormalNamed_));

  return ret;
}

bool
FunctionType::Formal::operator==(const FunctionType::Formal& rhs) const {
  return this->type == rhs.type &&
    this->intent == rhs.intent &&
    this->name == rhs.name;
}

size_t FunctionType::Formal::hash() const {
  std::hash<void*> hasherPtr;
  std::hash<const char*> hasherConstCharPtr;

  size_t ret = hasherPtr(this->type);
  ret = chpl::hash_combine(ret, ((size_t) this->intent));
  ret = chpl::hash_combine(ret, hasherConstCharPtr(this->name));
  return ret;
}

bool FunctionType::Formal::isGeneric() const {
  auto t = this->type;
  if (t == dtUnknown || t == dtAny || t->symbol->hasFlag(FLAG_GENERIC)) {
    return true;
  }
  return false;
}

bool FunctionType::equals(const FunctionType* rhs) const {
  return this->kind_ == rhs->kind_ &&
    this->formals_ == rhs->formals_ &&
    this->returnIntent_ == rhs->returnIntent_ &&
    this->returnType_ == rhs->returnType_ &&
    this->throws_ == rhs->throws_ &&
    this->isAnyFormalNamed_ == rhs->isAnyFormalNamed_ &&
    this->userTypeString_ == rhs->userTypeString_;
}

bool FunctionType::isGeneric() const {
  auto rt = returnType();

  if (rt == dtUnknown || rt == dtAny || rt->symbol->hasFlag(FLAG_GENERIC)) {
    return true;
  }

  for (auto& formal : formals_) {
    if (formal.isGeneric()) return true;
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static PrimitiveType* createPrimitiveType(const char* name, const char* cname);
static PrimitiveType* createInternalType (const char* name, const char* cname);

static PrimitiveType* createType(const char* name,
                                 const char* cname,
                                 bool        internalType);

static VarSymbol*     createSymbol(PrimitiveType* primType, const char* name);

// Create new primitive type for integers.
// Specify name for now.
// Though it will probably be something like int1, int8, etc. in the end.
// In that case we can just specify the width (i.e., size).
#define INIT_PRIM_BOOL(name, width)                                             \
  dtBools[BOOL_SIZE_ ## width] = createPrimitiveType(name, "chpl_bool" #width); \
  dtBools[BOOL_SIZE_ ## width]->defaultValue = new_BoolSymbol( false, BOOL_SIZE_ ## width)

#define INIT_PRIM_INT( name, width)                                             \
  dtInt[INT_SIZE_ ## width] = createPrimitiveType (name, "int" #width "_t");    \
  dtInt[INT_SIZE_ ## width]->defaultValue = new_IntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIM_UINT( name, width)                                            \
  dtUInt[INT_SIZE_ ## width] = createPrimitiveType (name, "uint" #width "_t");  \
  dtUInt[INT_SIZE_ ## width]->defaultValue = new_UIntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIM_REAL( name, width)                                            \
  dtReal[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_real" #width);    \
  dtReal[FLOAT_SIZE_ ## width]->defaultValue = new_RealSymbol( "0.0", FLOAT_SIZE_ ## width)

#define INIT_PRIM_IMAG( name, width)                                            \
  dtImag[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_imag" #width);    \
  dtImag[FLOAT_SIZE_ ## width]->defaultValue = new_ImagSymbol( "0.0", FLOAT_SIZE_ ## width)

#define INIT_PRIM_COMPLEX( name, width)                                                   \
  dtComplex[COMPLEX_SIZE_ ## width]= createPrimitiveType (name, "_complex" #width);       \
  dtComplex[COMPLEX_SIZE_ ## width]->defaultValue = new_ComplexSymbol(                    \
                                  "_chpl_complex" #width "(0.0, 0.0)",                    \
                                   0.0, 0.0, COMPLEX_SIZE_ ## width);                     \
  dtComplex[COMPLEX_SIZE_ ## width]->GEPMap.insert(std::pair<std::string, int>("re", 0)); \
  dtComplex[COMPLEX_SIZE_ ## width]->GEPMap.insert(std::pair<std::string, int>("im", 1));

#define CREATE_DEFAULT_SYMBOL(primType, gSym, name)     \
  gSym = new VarSymbol (name, primType);                \
  gSym->addFlag(FLAG_CONST);                            \
  gSym->addFlag(FLAG_GLOBAL_VAR_BUILTIN);               \
  rootModule->block->insertAtTail(new DefExpr(gSym));   \
  primType->defaultValue = gSym


// This should probably be renamed since it creates primitive types, as
//  well as internal types and other types used in the generated code
void initPrimitiveTypes() {
  dtVoid                               = createInternalType("void", "void");
  dtVoid->symbol->addFlag(FLAG_NO_RENAME);
  dtNothing                            = createInternalType ("nothing",  "nothing");

  dtBools[BOOL_SIZE_SYS]               = createPrimitiveType("bool",     "chpl_bool");
  dtInt[INT_SIZE_64]                   = createPrimitiveType("int",      "int64_t");
  dtReal[FLOAT_SIZE_64]                = createPrimitiveType("real",     "_real64");

  dtStringC                            = createPrimitiveType("c_string", "c_string" );
  dtStringC->symbol->addFlag(FLAG_NO_CODEGEN);

  dtObject                             = new AggregateType(AGGREGATE_CLASS);
  dtObject->symbol                     = new TypeSymbol("object", dtObject);

  dtBytes                              = new AggregateType(AGGREGATE_RECORD);
  dtBytes->symbol                      = new TypeSymbol("bytes", dtBytes);

  dtString                             = new AggregateType(AGGREGATE_RECORD);
  dtString->symbol                     = new TypeSymbol("string", dtString);

  dtLocale                             = new AggregateType(AGGREGATE_RECORD);
  dtLocale->symbol                     = new TypeSymbol("locale", dtLocale);

  dtOwned                              = new AggregateType(AGGREGATE_RECORD);
  dtOwned->symbol                      = new TypeSymbol("_owned", dtOwned);

  dtShared                             = new AggregateType(AGGREGATE_RECORD);
  dtShared->symbol                     = new TypeSymbol("_shared", dtShared);

  gFalse                               = createSymbol(dtBools[BOOL_SIZE_SYS], "false");
  gTrue                                = createSymbol(dtBools[BOOL_SIZE_SYS], "true");

  gFalse->addFlag(FLAG_PARAM);
  gFalse->immediate                    = new Immediate;
  gFalse->immediate->v_bool            = false;
  gFalse->immediate->const_kind        = NUM_KIND_BOOL;
  gFalse->immediate->num_index         = BOOL_SIZE_DEFAULT;

  gTrue->addFlag(FLAG_PARAM);
  gTrue->immediate                     = new Immediate;
  gTrue->immediate->v_bool             = true;
  gTrue->immediate->const_kind         = NUM_KIND_BOOL;
  gTrue->immediate->num_index          = BOOL_SIZE_DEFAULT;

  dtBools[BOOL_SIZE_SYS]->defaultValue = gFalse;
  dtInt[INT_SIZE_64]->defaultValue     = new_IntSymbol(0, INT_SIZE_64);
  dtReal[FLOAT_SIZE_64]->defaultValue  = new_RealSymbol("0.0", FLOAT_SIZE_64);

  dtBool                               = dtBools[BOOL_SIZE_SYS];

  uniqueConstantsHash.put(gFalse->immediate, gFalse);
  uniqueConstantsHash.put(gTrue->immediate,  gTrue);

  dtNil = createInternalType ("_nilType", "_nilType");
  dtNil->symbol->addFlag(FLAG_NO_RENAME);
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");
  gNil->addFlag(FLAG_NO_RENAME);

  // dtStringC defaults to nil
  dtStringC->defaultValue = gNil;

  // This type should not be visible past normalize.
  CREATE_DEFAULT_SYMBOL (dtVoid, gNoInit, "_gnoinit");

  dtSplitInitType = createInternalType("_splitInitType", "_splitInitType");
  CREATE_DEFAULT_SYMBOL (dtSplitInitType, gSplitInit, "_gsplitinit");

  dtUnknown = createInternalType ("_unknown", "_unknown");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_gunknown");
  gUnknown->addFlag(FLAG_TYPE_VARIABLE);

  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");
  CREATE_DEFAULT_SYMBOL (dtNothing, gNone, "none");

  // parses from record
  dtAnyRecord = createInternalType("record", "_anyRecord");
  dtAnyRecord->symbol->addFlag(FLAG_GENERIC);

  gIteratorBreakToken = createSymbol(dtBool, "_iteratorBreakToken");
  gIteratorBreakToken->addFlag(FLAG_NO_CODEGEN);

  INIT_PRIM_BOOL("bool(8)", 8);
  INIT_PRIM_BOOL("bool(16)", 16);
  INIT_PRIM_BOOL("bool(32)", 32);
  INIT_PRIM_BOOL("bool(64)", 64);

  INIT_PRIM_INT( "int(8)", 8);
  INIT_PRIM_INT( "int(16)", 16);
  INIT_PRIM_INT( "int(32)", 32);

  INIT_PRIM_UINT( "uint(8)", 8);
  INIT_PRIM_UINT( "uint(16)", 16);
  INIT_PRIM_UINT( "uint(32)", 32);
  INIT_PRIM_UINT( "uint", 64);          // default size

  INIT_PRIM_REAL( "real(32)", 32);

  INIT_PRIM_IMAG( "imag(32)", 32);
  INIT_PRIM_IMAG( "imag", 64);            // default size

  INIT_PRIM_COMPLEX( "complex(64)", 64);
  INIT_PRIM_COMPLEX( "complex", 128);       // default size

  // Set up INFINITY and NAN params
  gInfinity = createSymbol(dtReal[FLOAT_SIZE_DEFAULT], "chpl_INFINITY");
  gInfinity->addFlag(FLAG_PARAM);
  gInfinity->immediate = new Immediate;
  gInfinity->immediate->v_float64 = INFINITY;
  gInfinity->immediate->const_kind = NUM_KIND_REAL;
  gInfinity->immediate->num_index = FLOAT_SIZE_DEFAULT;

  gNan = createSymbol(dtReal[FLOAT_SIZE_DEFAULT], "chpl_NAN");
  gNan->addFlag(FLAG_PARAM);
  gNan->immediate = new Immediate;
  gNan->immediate->v_float64 = NAN;
  gNan->immediate->const_kind = NUM_KIND_REAL;
  gNan->immediate->num_index = FLOAT_SIZE_DEFAULT;



  // Could be == c_ptr(int(8)) e.g.
  // used in some runtime interfaces
  dtCVoidPtr   = createPrimitiveType("chpl__c_void_ptr", "c_void_ptr" );
  dtCVoidPtr->symbol->addFlag(FLAG_NO_CODEGEN);
  dtCVoidPtr->defaultValue = gNil;

  dtCFnPtr = createPrimitiveType("c_fn_ptr", "c_fn_ptr");
  dtCFnPtr->symbol->addFlag(FLAG_NO_CODEGEN);
  dtCFnPtr->defaultValue = gNil;

  dtOpaque = createPrimitiveType("opaque", "chpl_opaque");

  CREATE_DEFAULT_SYMBOL(dtOpaque, gOpaque, "_nullOpaque");
  gOpaque->cname = astr("NULL");
  gOpaque->addFlag(FLAG_EXTERN);

  dtTaskID = createPrimitiveType("chpl_taskID_t", "chpl_taskID_t");
  dtTaskID->symbol->addFlag(FLAG_NO_CODEGEN);

  CREATE_DEFAULT_SYMBOL(dtTaskID, gTaskID, "chpl_nullTaskID");

  dtSyncVarAuxFields = createPrimitiveType( "_sync_aux_t", "chpl_sync_aux_t");

  CREATE_DEFAULT_SYMBOL (dtSyncVarAuxFields, gSyncVarAuxFields, "_nullSyncVarAuxFields");
  gSyncVarAuxFields->cname = astr("NULL");

  dtSingleVarAuxFields = createPrimitiveType( "_single_aux_t", "chpl_single_aux_t");

  CREATE_DEFAULT_SYMBOL (dtSingleVarAuxFields, gSingleVarAuxFields, "_nullSingleVarAuxFields");
  gSingleVarAuxFields->cname = astr("NULL");

  dtAny = createInternalType ("_any", "_any");
  dtAny->symbol->addFlag(FLAG_GENERIC);

  dtAnyBool = createInternalType("chpl_anybool", "bool");
  dtAnyBool->symbol->addFlag(FLAG_GENERIC);

  dtAnyComplex = createInternalType("chpl_anycomplex", "complex");
  dtAnyComplex->symbol->addFlag(FLAG_GENERIC);

  // parses from enum
  dtAnyEnumerated = createInternalType ("enum", "enum");
  dtAnyEnumerated->symbol->addFlag(FLAG_GENERIC);

  dtAnyImag = createInternalType("chpl_anyimag", "imag");
  dtAnyImag->symbol->addFlag(FLAG_GENERIC);

  dtAnyReal = createInternalType("chpl_anyreal", "real");
  dtAnyReal->symbol->addFlag(FLAG_GENERIC);

  dtAnyPOD = createInternalType ("chpl_anyPOD", "POD");
  dtAnyPOD->symbol->addFlag(FLAG_GENERIC);

  // could also be called dtAnyIntegral
  dtIntegral = createInternalType ("integral", "integral");
  dtIntegral->symbol->addFlag(FLAG_GENERIC);

  dtNumeric = createInternalType ("numeric", "numeric");
  dtNumeric->symbol->addFlag(FLAG_GENERIC);

  dtIteratorRecord = createInternalType("_iteratorRecord", "_iteratorRecord");
  dtIteratorRecord->symbol->addFlag(FLAG_GENERIC);

  dtIteratorClass = createInternalType("_iteratorClass", "_iteratorClass");
  dtIteratorClass->symbol->addFlag(FLAG_GENERIC);

  dtBorrowed = createInternalType("borrowed", "borrowed");
  dtBorrowed->symbol->addFlag(FLAG_GENERIC);

  dtBorrowedNonNilable = createInternalType("_borrowedNonNilable", "_borrowedNonNilable");
  dtBorrowedNonNilable->symbol->addFlag(FLAG_GENERIC);

  dtBorrowedNilable = createInternalType("_borrowedNilable", "_borrowedNilable");
  dtBorrowedNilable->symbol->addFlag(FLAG_GENERIC);

  dtUnmanaged = createInternalType("unmanaged", "unmanaged");
  dtUnmanaged->symbol->addFlag(FLAG_GENERIC);

  dtUnmanagedNonNilable = createInternalType("_unmanagedNonNilable", "_unmanagedNonNilable");
  dtUnmanagedNonNilable->symbol->addFlag(FLAG_GENERIC);

  dtUnmanagedNilable = createInternalType("_unmanagedNilable", "_unmanagedNilable");
  dtUnmanagedNilable->symbol->addFlag(FLAG_GENERIC);

  dtAnyManagementAnyNilable = createInternalType("_anyManagementAnyNilable", "_anyManagementAnyNilable");
  dtAnyManagementAnyNilable->symbol->addFlag(FLAG_GENERIC);

  // parses from class
  dtAnyManagementNonNilable = createInternalType("class", "_anyManagementNonNilable");
  dtAnyManagementNonNilable->symbol->addFlag(FLAG_GENERIC);

  dtAnyManagementNilable = createInternalType("_anyManagementNilable", "_anyManagementNilable");
  dtAnyManagementNilable->symbol->addFlag(FLAG_GENERIC);


  dtMethodToken = createInternalType ("_MT", "_MT");
  CREATE_DEFAULT_SYMBOL(dtMethodToken, gMethodToken, "_mt");

  dtDummyRef = createInternalType ("_DummyRef", "_DummyRef");
  CREATE_DEFAULT_SYMBOL(dtDummyRef, gDummyRef, "_dummyRef");
  CREATE_DEFAULT_SYMBOL(dtVoid, gDummyWitness, "_dummyWitness");
  CREATE_DEFAULT_SYMBOL(dtVoid, gFixupRequiredToken, "_fixupRequired");

  dtTypeDefaultToken = createInternalType("_TypeDefaultT", "_TypeDefaultT");
  CREATE_DEFAULT_SYMBOL(dtTypeDefaultToken, gTypeDefaultToken, "_typeDefaultT");

  dtModuleToken = createInternalType("tmodule=", "tmodule=");
  CREATE_DEFAULT_SYMBOL(dtModuleToken, gModuleToken, "module=");

  dtUninstantiated = createInternalType("_uninstantiated", "_uninstantiated");

  CREATE_DEFAULT_SYMBOL(dtUninstantiated, gUninstantiated, "?");
  gUninstantiated->addFlag(FLAG_PARAM);
}

static PrimitiveType* createPrimitiveType(const char* name, const char* cname) {
  PrimitiveType* newType = createType(name, cname, false);
  newType->symbol->addFlag(FLAG_NO_RENAME);
  return newType;
}

static PrimitiveType* createInternalType(const char* name, const char* cname) {
  return createType(name, cname,  true);
}

static PrimitiveType*
createType(const char* name, const char* cname, bool internalType) {
  PrimitiveType* pt = new PrimitiveType(NULL, internalType);
  TypeSymbol*    ts = new TypeSymbol(name, pt);

  ts->cname = astr(cname);

  // This prevents cleanAST() from sweeping these
  ts->addFlag(FLAG_GLOBAL_TYPE_SYMBOL);

  rootModule->block->insertAtTail(new DefExpr(ts));

  return pt;
}

static VarSymbol* createSymbol(PrimitiveType* primType, const char* name) {
  VarSymbol* retval = new VarSymbol(name, primType);

  retval->addFlag(FLAG_CONST);
  retval->addFlag(FLAG_GLOBAL_VAR_BUILTIN);

  rootModule->block->insertAtTail(new DefExpr(retval));

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void initChplProgram() {
  theProgram           = new ModuleSymbol("chpl__Program",
                                          MOD_INTERNAL,
                                          new BlockStmt());

  theProgram->filename = astr("<internal>");

  theProgram->addFlag(FLAG_NO_CODEGEN);

  rootModule->block->insertAtTail(new DefExpr(theProgram));
}

// Appends a VarSymbol to the root module and gives it the bool immediate
// matching 'value'. For use in initCompilerGlobals.
static void setupBoolGlobal(VarSymbol* globalVar, bool value) {
  rootModule->block->insertAtTail(new DefExpr(globalVar));

  if (value) {
     globalVar->immediate = new Immediate;
    *globalVar->immediate = *gTrue->immediate;
    paramMap.put(globalVar, gTrue);

  } else {
     globalVar->immediate = new Immediate;
    *globalVar->immediate = *gFalse->immediate;
    paramMap.put(globalVar, gFalse);
  }
}

void initCompilerGlobals() {

  gBoundsChecking = new VarSymbol("boundsChecking", dtBool);
  gBoundsChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gBoundsChecking, !fNoBoundsChecks);

  gCastChecking = new VarSymbol("castChecking", dtBool);
  gCastChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gCastChecking, !fNoCastChecks);

  gNilChecking = new VarSymbol("chpl_checkNilDereferences", dtBool);
  gNilChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gNilChecking, !fNoNilChecks);

  gOverloadSetsChecks = new VarSymbol("chpl_overloadSetsChecks", dtBool);
  gOverloadSetsChecks->addFlag(FLAG_PARAM);
  setupBoolGlobal(gOverloadSetsChecks, fOverloadSetsChecks);

  gDivZeroChecking = new VarSymbol("chpl_checkDivByZero", dtBool);
  gDivZeroChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gDivZeroChecking, !fNoDivZeroChecks);

  gCacheRemote = new VarSymbol("CHPL_CACHE_REMOTE", dtBool);
  gCacheRemote->addFlag(FLAG_PARAM);
  setupBoolGlobal(gCacheRemote, fCacheRemote);

  gPrivatization = new VarSymbol("_privatization", dtBool);
  gPrivatization->addFlag(FLAG_PARAM);
  setupBoolGlobal(gPrivatization, !(fNoPrivatization || fLocal));

  gLocal = new VarSymbol("_local", dtBool);
  gLocal->addFlag(FLAG_PARAM);
  setupBoolGlobal(gLocal, fLocal);

  gWarnUnstable = new VarSymbol("chpl_warnUnstable", dtBool);
  gWarnUnstable->addFlag(FLAG_PARAM);
  setupBoolGlobal(gWarnUnstable, fWarnUnstable);

  // defined and maintained by the runtime
  gNodeID = new VarSymbol("chpl_nodeID", dtInt[INT_SIZE_32]);
  gNodeID->addFlag(FLAG_EXTERN);
  rootModule->block->insertAtTail(new DefExpr(gNodeID));

  gUseIOSerializers = new VarSymbol("chpl_useIOSerializers", dtBool);
  gUseIOSerializers->addFlag(FLAG_PARAM);
  setupBoolGlobal(gUseIOSerializers, fUseIOSerializers);

  initForTaskIntents();
}

bool is_nothing_type(Type* t) {
  return t == dtNothing;
}

bool is_bool_type(Type* t) {
  return
    t == dtBools[BOOL_SIZE_SYS] ||
    t == dtBools[BOOL_SIZE_8] ||
    t == dtBools[BOOL_SIZE_16] ||
    t == dtBools[BOOL_SIZE_32] ||
    t == dtBools[BOOL_SIZE_64];
}


bool is_int_type(Type *t) {
  return
    t == dtInt[INT_SIZE_32] ||
    t == dtInt[INT_SIZE_8] ||
    t == dtInt[INT_SIZE_16] ||
    t == dtInt[INT_SIZE_64];
}


bool is_uint_type(Type *t) {
  return
    t == dtUInt[INT_SIZE_32] ||
    t == dtUInt[INT_SIZE_8] ||
    t == dtUInt[INT_SIZE_16] ||
    t == dtUInt[INT_SIZE_64];
}

bool is_signed(Type *t) {
  if( is_int_type(t) ||
      is_real_type(t) ||
      is_imag_type(t) ||
      is_complex_type(t) ) return true;
  if( is_uint_type(t) ) return false;
  if( is_enum_type(t) ) {
    return is_signed(toEnumType(t)->getIntegerType());
  }
  return false;
}

bool is_real_type(Type *t) {
  return
    t == dtReal[FLOAT_SIZE_64] ||
    t == dtReal[FLOAT_SIZE_32];
}


bool is_imag_type(Type *t) {
  return
    t == dtImag[FLOAT_SIZE_64] ||
    t == dtImag[FLOAT_SIZE_32];
}


bool is_complex_type(Type *t) {
  return
    t == dtComplex[COMPLEX_SIZE_128] ||
    t == dtComplex[COMPLEX_SIZE_64];
}


bool is_enum_type(Type *t) {
  return toEnumType(t);
}


bool isLegalParamType(Type* t) {
  return (is_bool_type(t) ||
          is_int_type(t) ||
          is_uint_type(t) ||
          is_real_type(t) ||
          is_imag_type(t) ||
          is_complex_type(t) ||
          is_enum_type(t) ||
          isString(t) ||
          isBytes(t) ||
          t == dtStringC ||
          t == dtUnknown);
}

int get_width(Type *t) {
  if (t == dtBools[BOOL_SIZE_8] ||
      t == dtInt[INT_SIZE_8] ||
      t == dtUInt[INT_SIZE_8])
    return 8;
  if (t == dtBools[BOOL_SIZE_16] ||
      t == dtInt[INT_SIZE_16] ||
      t == dtUInt[INT_SIZE_16])
    return 16;
  if (t == dtBools[BOOL_SIZE_32] ||
      t == dtInt[INT_SIZE_32] ||
      t == dtUInt[INT_SIZE_32] ||
      t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32])
    return 32;
  if (t == dtBools[BOOL_SIZE_64] ||
      t == dtInt[INT_SIZE_64] ||
      t == dtUInt[INT_SIZE_64] ||
      t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_64])
    return 64;
  if (t == dtComplex[COMPLEX_SIZE_128])
    return 128;
  INT_FATAL(t, "Unknown bit width");
  return 0;
}

// numbers between -2**width .. 2**width
// will fit exactly in a floating-point representation.
int get_mantissa_width(Type *t) {
  // FLOAT_SIZE_16 would have 11 bits of precision
  if (t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32] ||
      t == dtComplex[COMPLEX_SIZE_64])
    // mantissa for 32-bit float
    return 24;
  if (t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_128])
    // mantissa for 64-bit float
    return 53;
  INT_FATAL(t, "Unknown mantissa width");
  return 0;
}

int get_exponent_width(Type *t) {
  // FLOAT_SIZE_16 would have 5 bits of exponent
  if (t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32] ||
      t == dtComplex[COMPLEX_SIZE_64])
    // exponent bits for 32-bit float
    return 8;
  if (t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_128])
    // exponent bits for 64-bit float
    return 15;
  INT_FATAL(t, "Unknown exponent width");
  return 0;
}

bool isClass(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isClass();
  return false;
}

bool isHeapAllocatedType(Type* t) {
  if (AggregateType* ct = toAggregateType(t)) {
    TypeSymbol* ts = ct->symbol;
    if (ts->hasEitherFlag(FLAG_REF,FLAG_WIDE_REF))
      return false;
    if (ts->hasFlag(FLAG_C_ARRAY))
      return false;

    return (ts->hasFlag(FLAG_DATA_CLASS) ||
            ts->hasFlag(FLAG_WIDE_CLASS) ||
            ct->isClass());
  }
  return false;
}

bool isClassOrNil(Type* t) {
  if (t == dtNil) return true;
  return isClass(t);
}

bool isUnmanagedClass(Type* t) {
  if (DecoratedClassType* dt = toDecoratedClassType(t))
    if (dt->isUnmanaged())
      return true;
  return false;
}

bool isBorrowedClass(Type* t) {
  if (isClass(t))
    return true; // borrowed, non-nilable

  if (DecoratedClassType* dt = toDecoratedClassType(t))
    return dt->isBorrowed();

  return false;
}

// Todo: ideally this would be simply something like:
//   isChapelManagedType(t) || isChapelBorrowedType(t)
bool isOwnedOrSharedOrBorrowed(Type* t) {
  if (isClass(t))
    return true; // borrowed, non-nilable

  if (DecoratedClassType* dt = toDecoratedClassType(t))
    if (! dt->isUnmanaged())
      return true; // anything not unmanaged

  if (isManagedPtrType(t))
    return true; // owned or shared

  return false;
}

bool isBuiltinGenericClassType(Type* t) {
  return t == dtBorrowed ||
         t == dtBorrowedNonNilable ||
         t == dtBorrowedNilable ||
         t == dtUnmanaged ||
         t == dtUnmanagedNilable ||
         t == dtUnmanagedNonNilable ||
         t == dtAnyManagementAnyNilable ||
         t == dtAnyManagementNonNilable ||
         t == dtAnyManagementNilable;
}

bool isClassLike(Type* t) {
  return isDecoratedClassType(t) ||
         isBuiltinGenericClassType(t) ||
         (isClass(t) && !(t->symbol->hasFlag(FLAG_C_PTR_CLASS) ||
                          t->symbol->hasFlag(FLAG_DATA_CLASS) ||
                          t->symbol->hasFlag(FLAG_REF)));
}

bool isClassLikeOrManaged(Type* t) {
  return isClassLike(t) || isManagedPtrType(t);
}

bool isClassLikeOrPtr(Type* t) {
  return isClassLike(t) || (t->symbol->hasFlag(FLAG_C_PTR_CLASS) ||
                            t->symbol->hasFlag(FLAG_DATA_CLASS) ||
                            t == dtCVoidPtr ||
                            t == dtStringC ||
                            t == dtCFnPtr);
}

bool isClassLikeOrNil(Type* t) {
  if (t == dtNil) return true;
  return isClassLike(t);
}

bool isRecord(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isRecord();
  return false;
}

bool isUserRecord(Type* t) {
  if (!isRecord(t))
    return false;

  // Check for lots of exceptions - types that are implemented
  // as records but that isn't the user view.
  if (t == dtString ||
      t == dtBytes ||
      t->symbol->hasFlag(FLAG_SYNTACTIC_DISTRIBUTION) ||
      t->symbol->hasFlag(FLAG_DISTRIBUTION) ||
      t->symbol->hasFlag(FLAG_DOMAIN) ||
      t->symbol->hasFlag(FLAG_ARRAY) ||
      t->symbol->hasFlag(FLAG_RANGE) ||
      t->symbol->hasFlag(FLAG_TUPLE) ||
      t->symbol->hasFlag(FLAG_SYNC) ||
      t->symbol->hasFlag(FLAG_SINGLE) ||
      t->symbol->hasFlag(FLAG_ATOMIC_TYPE) ||
      t->symbol->hasFlag(FLAG_MANAGED_POINTER))
    return false;

  return true;
}

bool isUnion(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isUnion();
  return false;
}

bool isReferenceType(const Type* t) {
  return t->symbol->hasFlag(FLAG_REF);
}

bool isRefCountedType(Type* t) {
  // We may eventually want to add a separate flag and provide users
  //  with an interface to declare reference counted types that will
  //  be "automatically" reference counted when needed

  // The set of reference counted types currently coincides with the set
  // of record-wrapped types, so we can reuse the flag set.
  return getRecordWrappedFlags(t->symbol).any();
}

bool isRecordWrappedType(const Type* t) {
  return getRecordWrappedFlags(t->symbol).any();
}

// Returns true if the given type is one which can be returned by one of the
// dsiNew*Dom() functions; false otherwise.
// This check is performed by looking to see if the given type derives from the
// BaseDom class.
bool isDomImplType(Type* type)
{
  return isDerivedType(type, FLAG_BASE_DOMAIN);
}

// Returns true if the given type is one which can be returned by
// dsiBuildArray() or similar function returning a "nude" array implementation;
// false otherwise.
// The test is actually performed by looking to see if the given type derives
// from the BaseArr class.
bool isArrayImplType(Type* type)
{
  return isDerivedType(type, FLAG_BASE_ARRAY);
}

bool isDistImplType(Type* type)
{
  return isDerivedType(type, FLAG_BASE_DIST);
}

bool isAliasingArrayImplType(Type* t) {
  return t->symbol->hasFlag(FLAG_ALIASING_ARRAY);
}

bool isAliasingArrayType(Type* t) {
  if (t->symbol->hasFlag(FLAG_ARRAY)) {
    AggregateType* at = toAggregateType(t);
    INT_ASSERT(at);

    Symbol* instanceField = at->getField("_instance", false);
    if (instanceField) {
      return isAliasingArrayImplType(instanceField->type);
    }
  }

  return false;
}

static bool isDerivedType(Type* type, Flag flag)
{
  AggregateType* at     =  NULL;
  bool           retval = false;

  while ((at = toAggregateType(type)) != NULL && retval == false)
  {
    if (at->symbol->hasFlag(flag) == true)
      retval = true;
    else
      type   = at->dispatchParents.only();
  }

  return retval;
}

bool isManagedPtrType(const Type* t) {
  if (const DecoratedClassType* dt = toConstDecoratedClassType(t))
    t = dt->getCanonicalClass();

  return t && t->symbol->hasFlag(FLAG_MANAGED_POINTER);
}

Type* getManagedPtrBorrowType(const Type* managedPtrType) {
  INT_ASSERT(isManagedPtrType(managedPtrType));

  if (const DecoratedClassType* dt = toConstDecoratedClassType(managedPtrType))
    managedPtrType = dt->getCanonicalClass();

  const AggregateType* at = toConstAggregateType(managedPtrType);

  INT_ASSERT(at);

  const char* fieldName = astr("chpl_t");
  Type* borrowType = NULL;
  Symbol* field = at->getField(fieldName, /*fatal*/ false);
  if (field) {
    borrowType = field->type;
  } else {
    Symbol* sub = at->getSubstitution(fieldName);
    borrowType = sub->type;
  }
  if (borrowType == NULL)
    INT_FATAL("Could not determine borrow type");

  ClassTypeDecoratorEnum decorator = ClassTypeDecorator::BORROWED_NONNIL;

  if (isNilableClassType(borrowType))
    decorator = ClassTypeDecorator::BORROWED_NILABLE;

  borrowType = canonicalDecoratedClassType(borrowType);

  if (AggregateType* at = toAggregateType(borrowType))
    if (isClass(at))
      borrowType = at->getDecoratedClass(decorator);

  return borrowType;
}

AggregateType* getManagedPtrManagerType(Type* managedPtrType) {
  INT_ASSERT(isManagedPtrType(managedPtrType));

  if (DecoratedClassType* dt = toDecoratedClassType(managedPtrType))
    managedPtrType = dt->getCanonicalClass();

  AggregateType* at = toAggregateType(managedPtrType);
  at = at->getRootInstantiation();

  return at;
}

bool isSyncType(const Type* t) {
  return t->symbol->hasFlag(FLAG_SYNC);
}

bool isSingleType(const Type* t) {
  return t->symbol->hasFlag(FLAG_SINGLE);
}

bool isAtomicType(const Type* t) {
  return t->symbol->hasFlag(FLAG_ATOMIC_TYPE);
}

bool isRefIterType(Type* t) {
  Symbol* iteratorRecord = NULL;

  if (t->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
    AggregateType* at = toAggregateType(t);
    FnSymbol* getIterator = at->iteratorInfo->getIterator;
    iteratorRecord = getIterator->getFormal(1)->type->symbol;
  } else if (t->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    iteratorRecord = t->symbol;

  if (iteratorRecord)
    return iteratorRecord->hasFlag(FLAG_REF_ITERATOR_CLASS);
  return false;
}

bool isSubClass(Type* type, Type* baseType) {
  bool retval = false;

  if (type == baseType) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, pt, at->dispatchParents) {
      if (isSubClass(pt, baseType) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isDistClass(Type* type) {
  bool retval = false;

  if (type->symbol->hasFlag(FLAG_BASE_DIST) == true) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, pt, at->dispatchParents) {
      if (isDistClass(pt) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isDomainClass(Type* type) {
  bool retval = false;

  if (type->symbol->hasFlag(FLAG_BASE_DOMAIN) == true) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, pt, at->dispatchParents) {
      if (isDomainClass(pt) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isArrayClass(Type* type) {
  bool retval = false;

  if (type->symbol->hasFlag(FLAG_BASE_ARRAY) == true) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, t, at->dispatchParents) {
      if (isArrayClass(t) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isString(Type* type) {
  return type == dtString;
}

bool isBytes(Type* type) {
  return type == dtBytes;
}

// Indicates record-like memory management
bool typeNeedsCopyInitDeinit(Type* type) {
  bool retval = false;

  if (AggregateType* aggr = toAggregateType(type)) {
    Symbol*     sym  = aggr->symbol;

    // Must be a record or union type
    if (aggr->aggregateTag != AGGREGATE_RECORD &&
        aggr->aggregateTag != AGGREGATE_UNION) {
      retval = false;

    // Not a RUNTIME_type or an extern type
    } else if (sym->hasFlag(FLAG_RUNTIME_TYPE_VALUE) ||
               sym->hasFlag(FLAG_EXTERN)) {
      retval = false;

    } else {
      retval = true;
    }
  } else if (isConstrainedType(type)) {
    retval = true;
  }

  return retval;
}

// Do variables of the type 't' need capture for tasks?
bool needsCapture(Type* t) {
  INT_ASSERT(!isReferenceType(t)); // responsibility of the caller

  if (is_bool_type(t) ||
      is_int_type(t) ||
      is_uint_type(t) ||
      is_real_type(t) ||
      is_imag_type(t) ||
      is_complex_type(t) ||
      is_enum_type(t) ||
      t == dtStringC ||
      isClassLikeOrPtr(t) ||
      isRecord(t) ||
      isUnion(t) ||
      t == dtTaskID || // false?
      // TODO: Move these down to the "no" section.
      t == dtNil ||
      t == dtOpaque ||
      t->symbol->hasFlag(FLAG_EXTERN)) {
    return true;
  } else {
    // Ensure we have covered all types.
    INT_ASSERT(isRecordWrappedType(t) ||
               isSyncType(t)          ||
               isSingleType(t)        ||
               isAtomicType(t));
    return false;
  }
}

VarSymbol* resizeImmediate(VarSymbol* s, PrimitiveType* t)
{
  for( int i = 0; i < INT_SIZE_NUM; i++ ) {
    if( t == dtInt[i] ) {
      return new_IntSymbol(s->immediate->to_int(), (IF1_int_type) i);
    }
  }
  for( int i = 0; i < INT_SIZE_NUM; i++ ) {
    if( t == dtUInt[i] ) {
      return new_UIntSymbol(s->immediate->to_uint(), (IF1_int_type) i);
    }
  }
  return NULL;
}


/* After resolution, other passes can call isPOD
   in order to find out if a record type is POD.

   During resolution, one should call propagateNotPOD
   instead, so that the relevant calls can be resolved
   and POD fields can be properly handled.
 */
bool isPOD(Type* t)
{
  // things that aren't aggregate types are POD
  //   e.g. int, boolean, complex, etc
  if (!isAggregateType(t))
    return true;

  // sync/single and atomic types are not POD
  // but they should be marked with FLAG_NOT_POD
  // by propagateNotPOD in function resolution.

  // handle anything already marked
  if (t->symbol->hasFlag(FLAG_POD))
    return true;
  if (t->symbol->hasFlag(FLAG_NOT_POD))
    return false;

  // if we have not calculated POD-ness,
  // we should not be calling this function
  INT_ASSERT(false);
  return false;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool isPrimitiveScalar(Type* type) {
  bool retval = false;

  if (type == dtBools[BOOL_SIZE_8]         ||
      type == dtBools[BOOL_SIZE_16]        ||
      type == dtBools[BOOL_SIZE_32]        ||
      type == dtBools[BOOL_SIZE_64]        ||
      type == dtBools[BOOL_SIZE_SYS]       ||

      type == dtInt[INT_SIZE_8]            ||
      type == dtInt[INT_SIZE_16]           ||
      type == dtInt[INT_SIZE_32]           ||
      type == dtInt[INT_SIZE_64]           ||

      type == dtUInt[INT_SIZE_8]           ||
      type == dtUInt[INT_SIZE_16]          ||
      type == dtUInt[INT_SIZE_32]          ||
      type == dtUInt[INT_SIZE_64]          ||

      type == dtReal[FLOAT_SIZE_32]        ||
      type == dtReal[FLOAT_SIZE_64]        ||

      type == dtImag[FLOAT_SIZE_32]        ||
      type == dtImag[FLOAT_SIZE_64]        ||

      type == dtComplex[COMPLEX_SIZE_64]   ||
      type == dtComplex[COMPLEX_SIZE_128]) {

    retval = true;

  } else {
    retval = false;
  }

  return retval;
}

bool isNonGenericClass(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isClass()                    ==  true &&
        at->isGeneric()                  == false &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isNonGenericClassWithInitializers(Type* type) {
  bool retval = false;

  if (isNonGenericClass(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->hasUserDefinedInit == true) {
        retval = true;
      } else if (at->wantsDefaultInitializer()) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isGenericClass(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isClass()                    == true  &&
        at->isGeneric()                  == true  &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isGenericClassWithInitializers(Type* type) {
  bool retval = false;

  if (isGenericClass(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->hasUserDefinedInit == true) {
        retval = true;
      } else if (at->wantsDefaultInitializer()) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isClassWithInitializers(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isClass()                    == true  &&
        at->symbol->hasFlag(FLAG_EXTERN) == false &&
        (at->hasUserDefinedInit == true ||
         at->wantsDefaultInitializer())) {
      retval = true;
    }
  }

  return retval;
}

bool isNonGenericRecord(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isRecord()                   == true  &&
        at->isGeneric()                  == false) {
      retval = true;
    }
  }

  return retval;
}

bool isNonGenericRecordWithInitializers(Type* type) {
  bool retval = false;

  if (isNonGenericRecord(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->hasUserDefinedInit == true) {
        retval = true;
      } else if (at->wantsDefaultInitializer() &&
                 // don't count compiler-generated init for extern records
                 !at->symbol->hasFlag(FLAG_EXTERN)) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isGenericRecord(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isRecord()                   == true  &&
        at->isGeneric()                  == true  &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isGenericRecordWithInitializers(Type* type) {
  bool retval = false;

  if (isGenericRecord(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->hasUserDefinedInit == true) {
        retval = true;
      } else if (at->wantsDefaultInitializer()) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isRecordOrUnionWithInitializers(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if ((at->isRecord() || at->isUnion()) &&
        (at->hasUserDefinedInit == true ||
         at->wantsDefaultInitializer())) {
      retval = true;
    }
  }

  return retval;
}

//
// The simplest and most obvious case is that generic records need generic
// initializers.
//
// Instantiated records also require generic initializers, because their
// type or param fields need to be handled by an initializer.
//
bool needsGenericRecordInitializer(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (isRecordOrUnionWithInitializers(type)) {
      if (at->isGeneric() == true ||
          at->symbol->hasFlag(FLAG_GENERIC) == true ||
          at->instantiatedFrom != NULL) {
        retval = true;
      }
    }
  }

  return retval;
}

const Immediate& getDefaultImmediate(Type* t) {
  VarSymbol* defaultVar = toVarSymbol(t->defaultValue);
  // (or anything handled by coerce_immediate)
  if (defaultVar == NULL || defaultVar->immediate == NULL)
    INT_FATAL(t->symbol, "does not have a default value");

  // Numeric types should have a default of the right type
  if (defaultVar->type != t)
    INT_FATAL(t->symbol, "does not have a default of the same type");

  return *defaultVar->immediate;
}

// Returns 'true' for types that are the type of numeric literals.
// e.g. 1 is an 'int', so this function returns 'true' for 'int'.
// e.g. 0.0 is a 'real', so this function returns 'true' for 'real'.
bool isNumericParamDefaultType(Type* t)
{
  if (t == dtInt[INT_SIZE_DEFAULT] ||
      t == dtReal[FLOAT_SIZE_DEFAULT] ||
      t == dtImag[FLOAT_SIZE_DEFAULT] ||
      t == dtComplex[COMPLEX_SIZE_DEFAULT] ||
      t == dtBools[BOOL_SIZE_DEFAULT])
    return true;

  return false;
}

TypeSymbol*
getDataClassType(TypeSymbol* ts) {
  Symbol* value = ts->type->getSubstitutionWithName(astr("eltType"));

  return toTypeSymbol(value);
}

void
setDataClassType(TypeSymbol* ts, TypeSymbol* ets) {
  ts->type->setSubstitutionWithName(astr("eltType"), ets);
}
