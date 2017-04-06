/*
 * Copyright 2004-2017 Cray Inc.
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

#include "AstToText.h"
#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "docsDriver.h"
#include "expr.h"
#include "files.h"
#include "intlimits.h"
#include "ipe.h"
#include "iterator.h"
#include "misc.h"
#include "passes.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"
#include "vec.h"

static bool isDerivedType(Type* type, Flag flag);

Type::Type(AstTag astTag, Symbol* init_defaultVal) :
  BaseAST(astTag),

  symbol(NULL),
  refType(NULL),
  hasGenericDefaults(false),
  defaultValue(init_defaultVal),
  defaultInitializer(NULL),
  defaultTypeConstructor(NULL),
  destructor(NULL),
  isInternalType(false),
  instantiatedFrom(NULL),
  scalarPromotionType(NULL) {
}

Type::~Type() {

}

void Type::verify() {
}

void Type::addSymbol(TypeSymbol* newsymbol) {
  symbol = newsymbol;
}


bool Type::inTree() {
  if (symbol)
    return symbol->inTree();
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

Symbol* Type::getField(const char* name, bool fatal) {
  INT_FATAL(this, "getField not called on AggregateType");
  return NULL;
}


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

void PrimitiveType::printDocs(std::ostream *file, unsigned int tabs) {
  // Only print extern types.
  if (this->symbol->noDocGen()) {
    return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();
  *file << "type ";
  *file << this->symbol->name;
  *file << std::endl;

  // For .rst mode, put a line break after the .. data:: directive and
  // its description text.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->symbol->doc != NULL) {
    this->printDocsDescription(this->symbol->doc, file, tabs + 1);
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


std::string PrimitiveType::docsDirective() {
  if (!fDocsTextOnly) {
    return ".. type:: ";
  } else {
    return "";
  }
}


void PrimitiveType::accept(AstVisitor* visitor) {
  visitor->visitPrimType(this);
}

bool QualifiedType::isRefType() const {
  return _type->symbol->hasFlag(FLAG_REF);
}

bool QualifiedType::isWideRefType() const {
  return _type->symbol->hasFlag(FLAG_WIDE_REF);
}

EnumType::EnumType() :
  Type(E_EnumType, NULL),
  constants(), integerType(NULL),
  doc(NULL)
{
  gEnumTypes.add(this);
  constants.parent = this;
}


EnumType::~EnumType() { }


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
  }
}


EnumType*
EnumType::copyInner(SymbolMap* map) {
  EnumType* copy = new EnumType();
  for_enums(def, this)
    copy->constants.insertAtTail(COPY_INT(def));
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumType::replaceChild");
}


void EnumType::sizeAndNormalize() {
  bool first = true;
  bool issigned = false;
  int64_t v;
  uint64_t uv;
  int64_t min_v, max_v;
  uint64_t min_uv, max_uv;
  int num_bytes;
  uint64_t max;
  int64_t min;
  PrimitiveType* ret = NULL;

  // First, look for negative numbers in the enum.
  // If there are any, we have to store all
  // the values in negative numbers.
  for_enums(constant, this) {
    if( constant->init ) {
      if( get_int( constant->init, &v ) ) {
        if( v < 0 ) {
          issigned = true;
        }
      } else if( get_uint( constant->init, &uv ) ) {
        // OK!
      } else {
        // If we get here, then the initializer does not have an immediate
        // value associated with it....
        SymExpr* sym = toSymExpr(constant->init);

        // We think that all params should have init values by now.
        INT_ASSERT(sym && !sym->symbol()->hasFlag(FLAG_PARAM));

        // So we're going to blame this on the user.
        USR_FATAL(constant,
                  "enumerator '%s' is not an integer param value",
                  constant->sym->name);
        // And unfortunately, if we get here, we don't know how to proceed,
        // which is why no USR_FATAL_CONT().
      }
    }
  }

  // Set initializers for all enum symbols and
  // compute the minimum and maximum values.
  v = 1;
  uv = 1;
  min_v = max_v = 1;
  min_uv = max_uv = 1;

  first = true;
  for_enums(constant, this) {
    SET_LINENO(constant);
    if( constant->init ) {
      // set v and uv to the initializer value
      if( get_int( constant->init, &v ) ) {
        if( v >= 0 ) uv = v;
        else uv = 1;
      } else if( get_uint( constant->init, &uv ) ) {
        v = uv;
      }
    } else {
      // create initializer with v
      if( issigned ) {
        if( v >= INT32_MIN && v <= INT32_MAX ) {
          constant->init = new SymExpr(new_IntSymbol(v, INT_SIZE_32));
        } else {
          constant->init = new SymExpr(new_IntSymbol(v, INT_SIZE_64));
        }
      } else {
        if( uv <= UINT32_MAX ) {
          constant->init = new SymExpr(new_UIntSymbol(uv, INT_SIZE_32));
        } else {
          constant->init = new SymExpr(new_UIntSymbol(uv, INT_SIZE_64));
        }
      }
      parent_insert_help(constant, constant->init);
    }
    if( first ) {
      min_v = v;
      max_v = v;
      min_uv = uv;
      max_uv = uv;
      first = false;
    } else {
      if( min_v > v ) min_v = v;
      if( max_v < v ) max_v = v;
      if( min_uv > uv ) min_uv = uv;
      if( max_uv < uv ) max_uv = uv;
    }
    // Increment v for the next one, in case it is not set.
    v++;
    uv++;
  }

  num_bytes = 0;

  // Now figure out, based on min/max values, what integer
  // size we must use.
  if( issigned ) {
    int num_bytes_neg = 0;
    if( min_v >= INT8_MIN ) {
      num_bytes_neg = 1;
    } else if( min_v >= INT16_MIN ) {
      num_bytes_neg = 2;
    } else if( min_v >= INT32_MIN ) {
      num_bytes_neg = 4;
    } else {
      num_bytes_neg = 8;
    }

    if( max_v <= INT8_MAX ) {
      num_bytes = 1;
    } else if( max_v <= INT16_MAX ) {
      num_bytes = 2;
    } else if( max_v <= INT32_MAX ) {
      num_bytes = 4;
    } else {
      num_bytes = 8;
    }

    if( num_bytes < num_bytes_neg ) num_bytes = num_bytes_neg;
  } else {
    if( max_v <= UINT8_MAX ) {
      num_bytes = 1;
    } else if( max_v <= UINT16_MAX ) {
      num_bytes = 2;
    } else if( max_v <= UINT32_MAX ) {
      num_bytes = 4;
    } else {
      num_bytes = 8;
    }
  }

  // Now figure out field min and max values.
  // and set et->integerType
  min = max = 0;

  if( num_bytes == 1 ) {
    if( issigned ) {
      max = INT8_MAX;
      min = INT8_MIN;
      ret = dtInt[INT_SIZE_8];
    } else {
      max = UINT8_MAX;
      ret = dtUInt[INT_SIZE_8];
    }
  } else if( num_bytes == 2 ) {
    if( issigned ) {
      max = INT16_MAX;
      min = INT16_MIN;
      ret = dtInt[INT_SIZE_16];
    } else {
      max = UINT16_MAX;
      ret = dtUInt[INT_SIZE_16];
    }
  } else if( num_bytes == 4 ) {
    if( issigned ) {
      max = INT32_MAX;
      min = INT32_MIN;
      ret = dtInt[INT_SIZE_32];
    } else {
      max = UINT32_MAX;
      ret = dtUInt[INT_SIZE_32];
    }
  } else if( num_bytes == 8 ) {
    if( issigned ) {
      max = INT64_MAX;
      min = INT64_MIN;
      ret = dtInt[INT_SIZE_64];
    } else {
      max = UINT64_MAX;
      ret = dtUInt[INT_SIZE_64];
    }
  }

  // At the end of it all, check that each enum
  // symbol fits into the assigned type.
  // This check is necessary because we might have
  // had the impossible-to-fit enum
  // because it has e.g. UINT64_MAX and INT64_MIN.
  for_enums(constant, this) {
    if( get_int( constant->init, &v ) ) {
      if( v < min || (v > 0 && (uint64_t)v > max) ) {
        INT_FATAL(constant, "Does not fit in enum integer type");
      }
    } else if( get_uint( constant->init, &uv ) ) {
      if( uv > max ) {
        INT_FATAL(constant, "Does not fit in enum integer type");
      }
    }
  }

  integerType = ret;
}

PrimitiveType* EnumType::getIntegerType() {
  if( ! integerType ) {
    sizeAndNormalize();
  }
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


void EnumType::printDocs(std::ostream *file, unsigned int tabs) {
  if (this->symbol->noDocGen()) {
    return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();
  *file << "enum ";
  AstToText info;
  info.appendEnumDecl(this);
  *file << info.text();
  *file << std::endl;

  // In rst mode, ensure there is an empty line between the enum signature and
  // its description or the next directive.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->doc != NULL) {
    this->printDocsDescription(this->doc, file, tabs + 1);
    *file << std::endl;

    // In rst mode, ensure there is an empty line between the enum description
    // and the next directive.
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


std::string EnumType::docsDirective() {
  if (fDocsTextOnly) {
    return "";
  } else {
    return ".. enum:: ";
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

AggregateType::AggregateType(AggregateTag initTag)
  : Type(E_AggregateType, NULL) {

  aggregateTag        = initTag;
  initializerStyle    = DEFINES_NONE_USE_DEFAULT;
  initializerResolved = false;
  outer               = NULL;
  iteratorInfo        = NULL;
  doc                 = NULL;

  fields.parent       = this;
  inherits.parent     = this;

  genericField        = 0;
  mIsGeneric          = false;

  // set defaultValue to nil to keep it from being constructed
  if (aggregateTag == AGGREGATE_CLASS) {
    defaultValue = gNil;
  }

  gAggregateTypes.add(this);
}


AggregateType::~AggregateType() {
  // Delete references to this in iteratorInfo when destroyed.
  if (iteratorInfo) {
    if (iteratorInfo->iclass == this) {
      iteratorInfo->iclass = NULL;
    }

    if (iteratorInfo->irecord == this) {
      iteratorInfo->irecord = NULL;
    }
  }
}


AggregateType* AggregateType::copyInner(SymbolMap* map) {
  AggregateType* copy_type = new AggregateType(aggregateTag);

  copy_type->initializerStyle = initializerStyle;
  copy_type->outer            = outer;

  for_alist(expr, fields) {
    copy_type->fields.insertAtTail(COPY_INT(expr));
  }

  for_alist(expr, inherits) {
    copy_type->inherits.insertAtTail(COPY_INT(expr));
  }

  for_fields(field, copy_type) {
    if (FnSymbol* fn = toFnSymbol(field)) {
      copy_type->methods.add(fn);
    }
  }

  for_alist(delegate, forwardingTo) {
    copy_type->forwardingTo.insertAtTail(COPY_INT(delegate));
  }

  return copy_type;
}


void AggregateType::verify() {
  Type::verify();

  if (astTag != E_AggregateType) {
    INT_FATAL(this, "Bad AggregateType::astTag");
  }

  if (aggregateTag != AGGREGATE_CLASS &&
      aggregateTag != AGGREGATE_RECORD &&
      aggregateTag != AGGREGATE_UNION) {
    INT_FATAL(this, "Bad AggregateType::aggregateTag");
  }

  if (fields.parent != this || inherits.parent != this) {
    INT_FATAL(this, "Bad AList::parent in AggregateType");
  }

  for_alist(expr, fields) {
    if (expr->parentSymbol != symbol) {
      INT_FATAL(this, "Bad AggregateType::fields::parentSymbol");
    }
  }

  for_alist(expr, inherits) {
    if (expr->parentSymbol != symbol) {
      INT_FATAL(this, "Bad AggregateType::inherits::parentSymbol");
    }
  }
  for_alist(expr, forwardingTo) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad AggregateType::forwardingTo::parentSymbol");
  }
}


int AggregateType::numFields() const {
  return fields.length;
}

bool AggregateType::isClass() const {
  return aggregateTag == AGGREGATE_CLASS;
}

bool AggregateType::isRecord() const {
  return aggregateTag == AGGREGATE_RECORD;
}

bool AggregateType::isUnion() const {
  return aggregateTag == AGGREGATE_UNION;
}

bool AggregateType::isGeneric() const {
  return mIsGeneric;
}

void AggregateType::markAsGeneric() {
  mIsGeneric = true;
}

void AggregateType::addDeclarations(Expr* expr) {
  if (DefExpr* defExpr = toDefExpr(expr)) {
    addDeclaration(defExpr);

  } else if (BlockStmt* block = toBlockStmt(expr)) {
    for_alist(stmt, block->body) {
      addDeclarations(stmt);
    }
  } else if (ForwardingStmt* forwarding = toForwardingStmt(expr)) {
    // forwarding expr is a def expr for a function that we should handle.
    DefExpr* def = forwarding->toFnDef;
    // Handle the function defining what we forwarding to
    this->addDeclaration(def);
    // Add the ForwardingStmt to the AST
    forwarding->toFnDef = NULL;
    this->forwardingTo.insertAtTail(forwarding);
  } else {
    INT_FATAL(expr, "unexpected case");
  }
}

void AggregateType::addDeclaration(DefExpr* defExpr) {
  if (defExpr->sym->hasFlag(FLAG_REF_VAR)) {
      USR_FATAL_CONT(defExpr,
                     "References cannot be members of classes "
                     "or records yet.");
  }

  if (VarSymbol* var = toVarSymbol(defExpr->sym)) {
    var->makeField();

    if (var->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      mIsGeneric = true;

    } else if (var->hasFlag(FLAG_PARAM) == true) {
      mIsGeneric = true;

    } else if (defExpr->exprType == NULL &&
               defExpr->init     == NULL) {
      mIsGeneric = true;
    }

  } else if (FnSymbol* fn = toFnSymbol(defExpr->sym)) {
    methods.add(fn);

    if (fn->_this) {
      // get the name used in the type binding clause
      // this is the way it comes from the parser (see fn_decl_stmt_inner)
      ArgSymbol* thisArg = toArgSymbol(fn->_this);

      INT_ASSERT(thisArg);
      INT_ASSERT(thisArg->type == dtUnknown);

      BlockStmt* bs = thisArg->typeExpr;
      INT_ASSERT(bs && bs->length() == 1);

      Expr* firstexpr = bs->body.first();
      INT_ASSERT(firstexpr);

      UnresolvedSymExpr* sym  = toUnresolvedSymExpr(firstexpr);
      const char*        name = sym->unresolved;

      // ... then report it to the user
      USR_FATAL_CONT(fn->_this,
                     "Type binding clauses ('%s.' in this case) are not "
                     "supported in declarations within a class, record "
                     "or union",
                     name);
    } else {
      ArgSymbol* arg = new ArgSymbol(fn->thisTag, "this", this);

      fn->_this = arg;

      if (fn->thisTag == INTENT_TYPE) {
        arg->intent = INTENT_BLANK;
        arg->addFlag(FLAG_TYPE_VARIABLE);
      }

      arg->addFlag(FLAG_ARG_THIS);

      fn->insertFormalAtHead(new DefExpr(fn->_this));
      fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK,
                                                       "_mt",
                                                       dtMethodToken)));

      fn->addFlag(FLAG_METHOD);
      fn->addFlag(FLAG_METHOD_PRIMARY);
    }
  }

  if (defExpr->parentSymbol != NULL || defExpr->list != NULL) {
    defExpr->remove();
  }

  fields.insertAtTail(defExpr);
}


void AggregateType::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  INT_FATAL(this, "Unexpected case in AggregateType::replaceChild");
}


void AggregateType::accept(AstVisitor* visitor) {
  if (visitor->enterAggrType(this) == true) {

    for_alist(next_ast, fields) {
      next_ast->accept(visitor);
    }

    for_alist(next_ast, inherits) {
      next_ast->accept(visitor);
    }

    for_alist(next_ast, forwardingTo) {
      next_ast->accept(visitor);
    }

    visitor->exitAggrType(this);
  }
}

// Returns true if the type has generic fields, false otherwise.  If the index
// of the first generic field has not previously been set, set it.
bool AggregateType::setNextGenericField() {
  // Don't redo work
  if (genericField > 0)
    return true;

  int idx = 1;
  for_fields(field, this) {
    if (field->hasFlag(FLAG_TYPE_VARIABLE) || field->hasFlag(FLAG_PARAM) ||
        (field->defPoint->init == NULL && field->defPoint->exprType == NULL
         && field->type == dtUnknown)) {
      // TODO: do something special if the type of the field is known but
      // generic
      genericField = idx;
      break;
    }
    idx++;
  }
  if (genericField != 0)
    return true;
  else
    return false;
}

// Returns an instantiation of this AggregateType at the given index.  If the
// index is earlier than this AggregateType's first unsubstituted generic field,
// will just return itself.  Otherwise, this method will check the list of
// instantiations for the first unsubstituted generic field to see if we have
// previously made an instantiation for the provided argument and will return
// that instantiation if we find one.  Otherwise, will create a new
// instantiation with the given argument and will return that.
AggregateType* AggregateType::getInstantiation(SymExpr* t, int index) {
  // If the index of the field is prior to the index of the next generic field
  // then trivially return ourselves
  if (index < genericField)
    return this;

  if (index > genericField) {
    // Internal error, because initializerRules should have ensured that we
    // access the generic fields in order, and so we should never try to update
    // a generic field after the current generic field when the current one
    // hasn't been updated.
    INT_FATAL(this, "trying to set a later generic field %d", index);
  }

  // First, look to see if we have an instantiation with that value already
  for_vector(AggregateType, at, instantiations) {
    // TODO: test me
    Symbol* field = at->getField(genericField);
    if (field->hasFlag(FLAG_TYPE_VARIABLE) && isTypeExpr(t)) {
      if (field->type == t->typeInfo())
        return at;
    }
    if (field->hasFlag(FLAG_PARAM) &&
        at->substitutions.get(field) == t->symbol()) {
      return at;
    }
  }
  // Otherwise, we need to create an instantiation for that type
  AggregateType* newInstance = toAggregateType(this->symbol->copy()->type);
  this->symbol->defPoint->insertBefore(new DefExpr(newInstance->symbol));
  newInstance->symbol->copyFlags(this->symbol);

  newInstance->substitutions.copy(this->substitutions);

  Symbol* field = newInstance->getField(genericField);
  if (field->hasFlag(FLAG_PARAM)) {
    newInstance->substitutions.put(field, t->symbol());
    newInstance->symbol->renameInstantiatedSingle(t->symbol());
  } else {
    newInstance->substitutions.put(field, t->typeInfo()->symbol);
    newInstance->symbol->renameInstantiatedSingle(t->typeInfo()->symbol);
  }

  if (field->hasFlag(FLAG_TYPE_VARIABLE) && isTypeExpr(t)) {
    field->type = t->typeInfo();
  } else {
    if (!field->defPoint->exprType && field->type == dtUnknown)
      field->type = t->typeInfo();
    else if (field->defPoint->exprType->typeInfo() != t->typeInfo()) {
      // TODO: Something something, casts and coercions
    } else {
      field->type = t->typeInfo();
    }
  }
  instantiations.push_back(newInstance);
  newInstance->instantiatedFrom = this;

  // Handle dispatch parents (because it totally makes sense for this to have
  // been done outside of the AggregateType by
  // instantiateTypeForTypeConstructor.  Totally)
  forv_Vec(Type, t, this->dispatchParents) {
    newInstance->dispatchParents.add(t);
    bool inserted = t->dispatchChildren.add_exclusive(newInstance);
    INT_ASSERT(inserted);
  }

  DefExpr* next = toDefExpr(field->defPoint->next);
  newInstance->genericField = this->genericField + 1;
  while (next) {
    if (next->sym->hasFlag(FLAG_TYPE_VARIABLE) ||
        next->sym->hasFlag(FLAG_PARAM) ||
        (next->init == NULL && next->exprType == NULL)) {
      // This is the next value for genericField
      break;
    } else {
      newInstance->genericField = newInstance->genericField + 1;
      next = toDefExpr(next->next);
    }
  }

  if (newInstance->genericField > newInstance->fields.length) {
    newInstance->genericField = 0;
    newInstance->symbol->removeFlag(FLAG_GENERIC);
  }

  return newInstance;
}


int AggregateType::getFieldPosition(const char* name, bool fatal) {
  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;

  current_p->set_add(this);

  int fieldPos = 0;

  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (AggregateType* ct = toAggregateType(t)) {
        for_fields(sym, ct) {
          if (!strcmp(sym->name, name)) {
            return fieldPos;
          }

          fieldPos++;
        }
        forv_Vec(Type, parent, ct->dispatchParents) {
          if (parent)
            next_p->set_add(parent);
        }
      }
    }
    Vec<Type*>* temp = next_p;
    next_p = current_p;
    current_p = temp;
    next_p->clear();
  }

  if (fatal) {
    const char *className = "<no name>";
    if (this->symbol) { // this is always true?
      className = this->symbol->name;
    }
    // TODO: report as a user error in certain cases
    INT_FATAL(this,
              "no field '%s' in class '%s' in getField()",
              name,
              className);
  }

  return -1;
}


Symbol* AggregateType::getField(const char* name, bool fatal) {
  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;
  current_p->set_add(this);
  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (AggregateType* ct = toAggregateType(t)) {
        for_fields(sym, ct) {
          if (!strcmp(sym->name, name))
            return sym;
        }
        forv_Vec(Type, parent, ct->dispatchParents) {
          if (parent)
            next_p->set_add(parent);
        }
      }
    }
    Vec<Type*>* temp = next_p;
    next_p = current_p;
    current_p = temp;
    next_p->clear();
  }
  if (fatal) {
    const char *className = "<no name>";
    if (this->symbol) { // this is always true?
      className = this->symbol->name;
    }
    // TODO: report as a user error in certain cases
    INT_FATAL(this, "no field '%s' in class '%s' in getField()",
              name, className);
  }
  return NULL;
}


Symbol* AggregateType::getField(int i) {
  return toDefExpr(fields.get(i))->sym;
}

QualifiedType AggregateType::getFieldType(Expr* e) {
  SymExpr* sym = NULL;
  VarSymbol* var = NULL;

  sym = toSymExpr(e);
  if (sym)
    var = toVarSymbol(sym->symbol());

  const char* name = NULL;

  // Special case: An integer field name is actually a tuple member index.
  {
    int64_t i;
    if (get_int(sym, &i)) {
      name = astr("x", istr(i));
    }
  }

  // Typical case: field is identified by its name
  if (var && var->immediate)
    name = var->immediate->v_string;

  // Special case: star tuples can have run-time integer field access
  if (name == NULL && this->symbol->hasFlag(FLAG_STAR_TUPLE)) {
    name = astr("x1"); // get the 1st field's type, since they're all the same
  }

  Symbol* fs = NULL;
  for_fields(field, this) {
    if (!strcmp(field->name, name)) {
      fs = field;
    }
  }

  if (fs) {
    Qualifier qual = QUAL_VAL;
    if (fs->type->symbol->hasFlag(FLAG_REF))
      qual = QUAL_REF;
    return QualifiedType(fs->type, qual);
  }
  else
    return QualifiedType(NULL, QUAL_UNKNOWN);
}


void AggregateType::printDocs(std::ostream *file, unsigned int tabs) {
  // TODO: Include unions... (thomasvandoren, 2015-02-25)
  if (this->symbol->noDocGen() || this->isUnion()) {
    return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();
  *file << this->symbol->name;
  *file << this->docsSuperClass();
  *file << std::endl;

  // In rst mode, ensure there is an empty line between the class/record
  // signature and its description or the next directive.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->doc != NULL) {
    this->printDocsDescription(this->doc, file, tabs + 1);
    *file << std::endl;

    // In rst mode, ensure there is an empty line between the class/record
    // description and the next directive.
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


/*
 * Returns super class string for documentation. If super class exists, returns
 * ": <super class name>".
 */
std::string AggregateType::docsSuperClass() {
  if (this->inherits.length > 0) {
    std::vector<std::string> superClassNames;

    for_alist(expr, this->inherits) {
      if (UnresolvedSymExpr* use = toUnresolvedSymExpr(expr)) {
        superClassNames.push_back(use->unresolved);
      } else {
        INT_FATAL(expr,
                  "Expected UnresolvedSymExpr for all members "
                  "of inherits alist.");
      }
    }

    if (superClassNames.empty()) {
      return "";
    }

    // If there are super classes, join them into a single comma delimited
    // string prefixed with a colon.
    std::string superClasses = " : " + superClassNames.front();
    for (unsigned int i = 1; i < superClassNames.size(); i++) {
      superClasses += ", " + superClassNames.at(i);
    }
    return superClasses;
  } else {
    return "";
  }
}


std::string AggregateType::docsDirective() {
  if (fDocsTextOnly) {
    if (this->isClass()) {
      return "Class: ";
    } else if (this->isRecord()) {
      return "Record: ";
    }
  } else {
    if (this->isClass()) {
      return ".. class:: ";
    } else if (this->isRecord()) {
      return ".. record:: ";
    }
  }

  return "";
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void initRootModule() {
  rootModule           = new ModuleSymbol("_root", MOD_INTERNAL, new BlockStmt());
  rootModule->filename = astr("<internal>");
}

void initStringLiteralModule() {
  stringLiteralModule = new ModuleSymbol("ChapelStringLiterals", MOD_INTERNAL, new BlockStmt());
  stringLiteralModule->filename = astr("<internal>");
  theProgram->block->insertAtTail(new DefExpr(stringLiteralModule));
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
  rootModule->block->insertAtTail(new DefExpr(gSym));   \
  primType->defaultValue = gSym


// This should probably be renamed since it creates primitive types, as
//  well as internal types and other types used in the generated code
void initPrimitiveTypes() {
  dtVoid                               = createInternalType ("void",     "void");

  dtBools[BOOL_SIZE_SYS]               = createPrimitiveType("bool",     "chpl_bool");
  dtInt[INT_SIZE_64]                   = createPrimitiveType("int",      "int64_t");
  dtReal[FLOAT_SIZE_64]                = createPrimitiveType("real",     "_real64");

  dtStringC                            = createPrimitiveType("c_string", "c_string" );

  dtString                             = new AggregateType(AGGREGATE_RECORD);

  gFalse                               = createSymbol(dtBools[BOOL_SIZE_SYS], "false");
  gTrue                                = createSymbol(dtBools[BOOL_SIZE_SYS], "true");

  gFalse->immediate                    = new Immediate;
  gFalse->immediate->v_bool            = false;
  gFalse->immediate->const_kind        = NUM_KIND_BOOL;
  gFalse->immediate->num_index         = BOOL_SIZE_SYS;

  gTrue->immediate                     = new Immediate;
  gTrue->immediate->v_bool             = true;
  gTrue->immediate->const_kind         = NUM_KIND_BOOL;
  gTrue->immediate->num_index          = BOOL_SIZE_SYS;

  //
  // Mark the "high water mark" for types that IPE relies on directly
  //
  if (fUseIPE == true) {
    ipeRootInit();
  }

  dtBools[BOOL_SIZE_SYS]->defaultValue = gFalse;
  dtInt[INT_SIZE_64]->defaultValue     = new_IntSymbol(0, INT_SIZE_64);
  dtReal[FLOAT_SIZE_64]->defaultValue  = new_RealSymbol("0.0", FLOAT_SIZE_64);
  dtStringC->defaultValue              = new_CStringSymbol("");

  dtBool                               = dtBools[BOOL_SIZE_SYS];

  uniqueConstantsHash.put(gFalse->immediate, gFalse);
  uniqueConstantsHash.put(gTrue->immediate,  gTrue);

  dtStringC->symbol->addFlag(FLAG_NO_CODEGEN);

  gTryToken = new VarSymbol("chpl__tryToken", dtBool);

  gTryToken->addFlag(FLAG_CONST);
  rootModule->block->insertAtTail(new DefExpr(gTryToken));

  dtNil = createInternalType ("_nilType", "_nilType");
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");

  // This type should not be visible past normalize.
  CREATE_DEFAULT_SYMBOL (dtVoid, gNoInit, "_gnoinit");

  dtUnknown = createInternalType ("_unknown", "_unknown");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_gunknown");
  gUnknown->addFlag(FLAG_TYPE_VARIABLE);

  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");

  dtValue = createInternalType("value", "_chpl_value");

  INIT_PRIM_BOOL("bool(1)", 1);
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

  dtStringCopy = createPrimitiveType( "c_string_copy", "c_string_copy" );
  dtStringCopy->defaultValue = gOpaque;
  dtStringCopy->symbol->addFlag(FLAG_NO_CODEGEN);

  CREATE_DEFAULT_SYMBOL(dtStringCopy, gStringCopy, "_nullString");
  gStringCopy->cname = "NULL";
  gStringCopy->addFlag(FLAG_EXTERN);

  // Like c_string_copy but unowned.
  // Could be == c_ptr(int(8)) e.g.
  // used in some runtime interfaces
  dtCVoidPtr   = createPrimitiveType("c_void_ptr", "c_void_ptr" );
  dtCVoidPtr->symbol->addFlag(FLAG_NO_CODEGEN);
  dtCVoidPtr->defaultValue = gOpaque;
  dtCFnPtr = createPrimitiveType("c_fn_ptr", "c_fn_ptr");
  dtCFnPtr->symbol->addFlag(FLAG_NO_CODEGEN);
  dtCFnPtr->defaultValue = gOpaque;
  CREATE_DEFAULT_SYMBOL(dtCVoidPtr, gCVoidPtr, "_nullVoidPtr");
  gCVoidPtr->cname = "NULL";
  gCVoidPtr->addFlag(FLAG_EXTERN);

  dtSymbol = createPrimitiveType( "symbol", "_symbol");

  dtFile = createPrimitiveType ("_file", "_cfile");
  dtFile->symbol->addFlag(FLAG_EXTERN);

  CREATE_DEFAULT_SYMBOL(dtFile, gFile, "NULL");
  gFile->addFlag(FLAG_EXTERN);

  dtOpaque = createPrimitiveType("opaque", "chpl_opaque");

  CREATE_DEFAULT_SYMBOL(dtOpaque, gOpaque, "_nullOpaque");
  gOpaque->cname = "NULL";
  gOpaque->addFlag(FLAG_EXTERN);

  dtTaskID = createPrimitiveType("chpl_taskID_t", "chpl_taskID_t");
  dtTaskID->symbol->addFlag(FLAG_NO_CODEGEN);

  CREATE_DEFAULT_SYMBOL(dtTaskID, gTaskID, "chpl_nullTaskID");

  dtSyncVarAuxFields = createPrimitiveType( "_sync_aux_t", "chpl_sync_aux_t");

  CREATE_DEFAULT_SYMBOL (dtSyncVarAuxFields, gSyncVarAuxFields, "_nullSyncVarAuxFields");
  gSyncVarAuxFields->cname = "NULL";

  dtSingleVarAuxFields = createPrimitiveType( "_single_aux_t", "chpl_single_aux_t");

  CREATE_DEFAULT_SYMBOL (dtSingleVarAuxFields, gSingleVarAuxFields, "_nullSingleVarAuxFields");
  gSingleVarAuxFields->cname = "NULL";

  dtAny = createInternalType ("_any", "_any");
  dtAny->symbol->addFlag(FLAG_GENERIC);

  dtIntegral = createInternalType ("integral", "integral");
  dtIntegral->symbol->addFlag(FLAG_GENERIC);

  dtAnyComplex = createInternalType("chpl_anycomplex", "complex");
  dtAnyComplex->symbol->addFlag(FLAG_GENERIC);

  dtNumeric = createInternalType ("numeric", "numeric");
  dtNumeric->symbol->addFlag(FLAG_GENERIC);

  dtIteratorRecord = createInternalType("_iteratorRecord", "_iteratorRecord");
  dtIteratorRecord->symbol->addFlag(FLAG_GENERIC);

  dtIteratorClass = createInternalType("_iteratorClass", "_iteratorClass");
  dtIteratorClass->symbol->addFlag(FLAG_GENERIC);

  dtMethodToken = createInternalType ("_MT", "_MT");

  CREATE_DEFAULT_SYMBOL(dtMethodToken, gMethodToken, "_mt");

  dtTypeDefaultToken = createInternalType("_TypeDefaultT", "_TypeDefaultT");

  CREATE_DEFAULT_SYMBOL(dtTypeDefaultToken, gTypeDefaultToken, "_typeDefaultT");

  dtModuleToken = createInternalType("tmodule=", "tmodule=");

  CREATE_DEFAULT_SYMBOL(dtModuleToken, gModuleToken, "module=");

  dtAnyEnumerated = createInternalType ("enumerated", "enumerated");
  dtAnyEnumerated->symbol->addFlag(FLAG_GENERIC);
}

static PrimitiveType* createPrimitiveType(const char* name, const char* cname) {
  return createType(name, cname, false);
}

static PrimitiveType* createInternalType(const char* name, const char* cname) {
  return createType(name, cname,  true);
}

static PrimitiveType*
createType(const char* name, const char* cname, bool internalType) {
  PrimitiveType* pt = new PrimitiveType(NULL, internalType);
  TypeSymbol*    ts = new TypeSymbol(name, pt);

  ts->cname = cname;

  // This prevents cleanAST() from sweeping these
  ts->addFlag(FLAG_GLOBAL_TYPE_SYMBOL);

  rootModule->block->insertAtTail(new DefExpr(ts));

  return pt;
}

static VarSymbol* createSymbol(PrimitiveType* primType, const char* name) {
  VarSymbol* retval = new VarSymbol(name, primType);

  retval->addFlag(FLAG_CONST);

  rootModule->block->insertAtTail(new DefExpr(retval));

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DefExpr* defineObjectClass() {
  DefExpr* retval = 0;

  // The base object class looks like this:
  //
  //   class object {
  //     chpl__class_id chpl__cid;
  //   }
  //
  // chpl__class_id is an int32_t field identifying the classes
  //  in the program.  We never create the actual field within the
  //  IR (it is directly generated in the C code).  It might
  //  be the right thing to do, so I made an attempt at adding the
  //  field.  Unfortunately, we would need some significant changes
  //  throughout compilation, and it seemed to me that the it might result
  //  in possibly more special case code.
  //
  dtObject = new AggregateType(AGGREGATE_CLASS);

  retval   = buildClassDefExpr("object",
                               NULL,
                               dtObject,
                               NULL,
                               new BlockStmt(),
                               FLAG_UNKNOWN,
                               NULL);

  retval->sym->addFlag(FLAG_OBJECT_CLASS);
  retval->sym->addFlag(FLAG_GLOBAL_TYPE_SYMBOL); // Prevents removal in pruneResolvedTree().
  retval->sym->addFlag(FLAG_NO_OBJECT);

  return retval;
}

void initChplProgram(DefExpr* objectDef) {
  theProgram           = new ModuleSymbol("chpl__Program", MOD_INTERNAL, new BlockStmt());
  theProgram->filename = astr("<internal>");

  theProgram->addFlag(FLAG_NO_CODEGEN);

  UseStmt* base = new UseStmt(new UnresolvedSymExpr("ChapelBase"));
  UseStmt* std  = new UseStmt(new UnresolvedSymExpr("ChapelStandard"));

  theProgram->block->insertAtTail(base);

  // it may be better to add the following use after parsing
  // to simplify insertion of module guard sync var defs
  theProgram->block->insertAtTail(std);

  theProgram->block->insertAtHead(objectDef);

  rootModule->block->insertAtTail(new DefExpr(theProgram));
}

// Appends a VarSymbol to the root module and gives it the bool immediate
// matching 'value'. For use in initCompilerGlobals.
static void setupBoolGlobal(VarSymbol* globalVar, bool value) {
  rootModule->block->insertAtTail(new DefExpr(globalVar));
  if (value) {
    globalVar->immediate = new Immediate;
    *globalVar->immediate = *gTrue->immediate;
  } else {
    globalVar->immediate = new Immediate;
    *globalVar->immediate = *gFalse->immediate;
  }
}

void initCompilerGlobals() {

  gBoundsChecking = new VarSymbol("boundsChecking", dtBool);
  gBoundsChecking->addFlag(FLAG_CONST);
  setupBoolGlobal(gBoundsChecking, !fNoBoundsChecks);

  gCastChecking = new VarSymbol("castChecking", dtBool);
  gCastChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gCastChecking, !fNoCastChecks);

  gDivZeroChecking = new VarSymbol("chpl_checkDivByZero", dtBool);
  gDivZeroChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gDivZeroChecking, !fNoDivZeroChecks);

  gPrivatization = new VarSymbol("_privatization", dtBool);
  gPrivatization->addFlag(FLAG_PARAM);
  setupBoolGlobal(gPrivatization, !(fNoPrivatization || fLocal));

  gLocal = new VarSymbol("_local", dtBool);
  gLocal->addFlag(FLAG_PARAM);
  setupBoolGlobal(gLocal, fLocal);

  // defined and maintained by the runtime
  gNodeID = new VarSymbol("chpl_nodeID", dtInt[INT_SIZE_32]);
  gNodeID->addFlag(FLAG_EXTERN);
  rootModule->block->insertAtTail(new DefExpr(gNodeID));

  initForTaskIntents();
}

bool is_void_type(Type* t) {
  return t == dtVoid;
}

bool is_bool_type(Type* t) {
  return
    t == dtBools[BOOL_SIZE_1] ||
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
          is_enum_type(t) ||
          isString(t) ||
          t == dtStringC ||
          t == dtUnknown);
}

int get_width(Type *t) {
  if (t == dtBools[BOOL_SIZE_1] ||
      t == dtBools[BOOL_SIZE_SYS]) {
    return 1;
    // BLC: This is a lie, but one I'm hoping we can get away with
    // based on how this function is used
  }
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


bool isClass(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isClass();
  return false;
}


bool isRecord(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isRecord();
  return false;
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

bool isSubClass(Type* type, Type* baseType)
{
  if (type == baseType)
    return true;

  forv_Vec(Type, pt, type->dispatchParents)
    if (isSubClass(pt, baseType))
      return true;

  return false;
}

bool isDistClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_DIST))
    return true;

  forv_Vec(Type, pt, type->dispatchParents)
    if (isDistClass(pt))
      return true;

  return false;
}

bool isDomainClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_DOMAIN))
    return true;

  forv_Vec(Type, pt, type->dispatchParents)
    if (isDomainClass(pt))
      return true;

  return false;
}

bool isArrayClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_ARRAY))
    return true;

  forv_Vec(Type, t, type->dispatchParents)
    if (isArrayClass(t))
      return true;

  return false;
}

bool isString(Type* type) {
  bool retval = false;

  if (AggregateType* aggr = toAggregateType(type))
    retval = strcmp(aggr->symbol->name, "string") == 0;

  return retval;
}

//
// Noakes 2016/02/29
//
// To support the merge of the string-as-rec branch we defined a
// function, isString(), which is only true of the record that was
// defined in the new implementation of String.  This predicate was
// applied in cullOverReferences and callDestructors to improve
// memory management for that particular record type.
//
// We seek to apply those routines to a wider set of record types but
// are not ready to apply them to range, tuple, and the reference-counted
// records.
//
// This shorter-term predicate, which has a slightly inelegant name, allows
// most record-like types to use the new business logic.
//
// In the longer term we plan to further broaden the cases that the new
// logic can handle and reduce the exceptions that are filtered out here.
//
//
//
// MPF    2016/09/15
// This function now includes tuples, distributions, domains, and arrays.
//
// Noakes 2017/03/02
// This function now includes range and atomics
//
bool isUserDefinedRecord(Type* type) {
  bool retval = false;

  if (AggregateType* aggr = toAggregateType(type)) {
    Symbol*     sym  = aggr->symbol;
    const char* name = sym->name;

    // Must be a record type
    if (aggr->aggregateTag != AGGREGATE_RECORD) {
      retval = false;

    // Not a RUNTIME_type
    } else if (sym->hasFlag(FLAG_RUNTIME_TYPE_VALUE) == true) {
      retval = false;

    // Not an iterator
    } else if (strncmp(name, "_ir_", 4)              ==    0) {
      retval = false;

    } else {
      retval = true;
    }
  }

  return retval;
}

Type* getNamedType(std::string name) {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if(name == ts->name || name == ts->cname) {
      return ts->type;
    }
  }

  return NULL;
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
      isClass(t) ||
      isRecord(t) ||
      isUnion(t) ||
      t == dtTaskID || // false?
      t == dtFile ||
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
      type == dtImag[FLOAT_SIZE_64]) {

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
      retval = at->initializerStyle == DEFINES_INITIALIZER;
    }
  }

  return retval;
}

bool isNonGenericRecord(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isRecord()                   == true  &&
        at->isGeneric()                  == false &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isNonGenericRecordWithInitializers(Type* type) {
  bool retval = false;

  if (isNonGenericRecord(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      retval = at->initializerStyle == DEFINES_INITIALIZER;
    }
  }

  return retval;
}
