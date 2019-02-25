/*
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

#include "AggregateType.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "initializerRules.h"
#include "iterator.h"
#include "LoopExpr.h"
#include "UnmanagedClassType.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

AggregateType* dtObject = NULL;
AggregateType* dtString = NULL;
AggregateType* dtLocale = NULL;

AggregateType::AggregateType(AggregateTag initTag) :
  Type(E_AggregateType, NULL) {

  aggregateTag        = initTag;
  unmanagedClass      = NULL;

  typeConstructor     = NULL;
  hasUserDefinedInit  = false;
  builtDefaultInit    = false;
  initializerResolved = false;
  iteratorInfo        = NULL;
  doc                 = NULL;

  instantiatedFrom    = NULL;

  fields.parent       = this;
  inherits.parent     = this;

  genericField        = 0;
  mIsGeneric          = false;
  mIsGenericWithDefaults = false;

  classId             = 0;

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

  copy_type->hasUserDefinedInit = hasUserDefinedInit;

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

  copy_type->genericField = genericField;

  return copy_type;
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

bool AggregateType::isGenericWithDefaults() const {
  return mIsGenericWithDefaults;
}

void AggregateType::markAsGenericWithDefaults() {
  mIsGenericWithDefaults = true;
}

void AggregateType::verify() {
  Type::verify();

  if (astTag != E_AggregateType) {
    INT_FATAL(this, "Bad AggregateType::astTag");

  } else if (aggregateTag != AGGREGATE_CLASS  &&
             aggregateTag != AGGREGATE_RECORD &&
             aggregateTag != AGGREGATE_UNION) {
    INT_FATAL(this, "Bad AggregateType::aggregateTag");

  } else if (fields.parent != this || inherits.parent != this) {
    INT_FATAL(this, "Bad AList::parent in AggregateType");

  } else {
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
}

int AggregateType::numFields() const {
  return fields.length;
}

// Note that a field with generic type where that type has
// default values for all of its generic fields is considered concrete
// for the purposes of this function.
static bool isFieldTypeExprGeneric(Expr* typeExpr) {
  // Look in the field declaration for a concrete type
  Symbol* sym = NULL;

  if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(typeExpr)) {
    sym = lookup(urse->unresolved, urse);
  } else if (SymExpr* se = toSymExpr(typeExpr)) {
    sym = se->symbol();
  }

  if (sym) {
    Type* t = sym->type;
    if (AggregateType* at = toAggregateType(t)) {
      if (at->isGeneric()) {
        // If it's a generic type that has default values
        // for all of it's generic attributes, it won't
        // make this type generic.
        bool foundGenericWithoutInit = false;
        for_fields(field, at) {
          bool hasDefault = false;
          bool fieldGeneric = at->fieldIsGeneric(field, hasDefault);
          if (fieldGeneric && !hasDefault)
            foundGenericWithoutInit = true;
        }
        return foundGenericWithoutInit;
      }
    } else if (t->symbol->hasFlag(FLAG_GENERIC)) {
      return true;
    }
  }

  return false;
}

bool AggregateType::fieldIsGeneric(Symbol* field, bool &hasDefault) const {
  bool retval = false;

  DefExpr* def = field->defPoint;
  INT_ASSERT(def);

  if (VarSymbol* var = toVarSymbol(field)) {
    if (var->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      retval = true;

    } else if (var->hasFlag(FLAG_PARAM) == true) {
      retval = true;

    } else if (var->type == dtUnknown
               /* check for FLAG_SUPER_CLASS avoids infinite loop  */
               || (!var->hasFlag(FLAG_SUPER_CLASS) &&
                   var->type->symbol->hasFlag(FLAG_GENERIC))) {

      if (def->init == NULL && def->exprType == NULL) {
        // if we end up in this case.. the compiler infinite loops
        INT_ASSERT(!var->type->symbol->hasFlag(FLAG_GENERIC));

        retval = true;
      } else if (def->init == NULL &&
                 def->exprType != NULL &&
                 isFieldTypeExprGeneric(def->exprType)) {
        retval = true;
      }

    }
  }

  hasDefault = (def->init != NULL);

  return retval;
}

DefExpr* AggregateType::toSuperField(const char*  name) const {
  DefExpr* retval = NULL;

  if (isClass() == true) {
    AggregateType* thisNC = const_cast<AggregateType*>(this);
    forv_Vec(AggregateType, pt, thisNC->dispatchParents) {
      AggregateType* root = pt->getRootInstantiation();
      if (DefExpr* field = pt->toLocalField(name)) {
        retval = field;
        break;
      } else if (DefExpr* field = pt->toSuperField(name)) {
        retval = field;
        break;
      } else if (pt != root) {
        if (DefExpr* field = root->toLocalField(name)) {
          retval = field;
          break;
        } else if (DefExpr* field = root->toSuperField(name)) {
          retval = field;
          break;
        }
      }
    }
  }

  return retval;
}


DefExpr* AggregateType::toSuperField(SymExpr*  expr) const {
  DefExpr* retval = NULL;

  if (isClass() == true) {
    AggregateType* thisNC = const_cast<AggregateType*>(this);
    forv_Vec(AggregateType, pt, thisNC->dispatchParents) {
      AggregateType* root = pt->getRootInstantiation();
      if (DefExpr* field = pt->toLocalField(expr)) {
        retval = field;
        break;
      } else if (DefExpr* field = pt->toSuperField(expr)) {
        retval = field;
        break;
      } else if (pt != root) {
        if (DefExpr* field = root->toLocalField(expr)) {
          retval = field;
          break;
        } else if (DefExpr* field = root->toSuperField(expr)) {
          retval = field;
          break;
        }
      }
    }
  }

  return retval;
}

DefExpr* AggregateType::toSuperField(CallExpr* expr) const {
  DefExpr* retval = NULL;

  if (isClass() == true) {
    AggregateType* thisNC = const_cast<AggregateType*>(this);
    forv_Vec(AggregateType, pt, thisNC->dispatchParents) {
      AggregateType* root = pt->getRootInstantiation();
      if (DefExpr* field = pt->toLocalField(expr)) {
        retval = field;
        break;
      } else if (DefExpr* field = pt->toSuperField(expr)) {
        retval = field;
        break;
      } else if (pt != root) {
        if (DefExpr* field = root->toLocalField(expr)) {
          retval = field;
          break;
        } else if (DefExpr* field = root->toSuperField(expr)) {
          retval = field;
          break;
        }
      }
    }
  }

  return retval;
}

DefExpr* AggregateType::toLocalField(const char* name) const {
  Expr*    currField = fields.head;
  DefExpr* retval    = NULL;

  while (currField != NULL && retval == NULL) {
    DefExpr*   defExpr = toDefExpr(currField);
    VarSymbol* var     = toVarSymbol(defExpr->sym);

    if (strcmp(var->name, name) == 0) {
      retval    = defExpr;
    } else {
      currField = currField->next;
    }
  }

  return retval;
}

DefExpr* AggregateType::toLocalField(SymExpr* expr) const {
  Expr*    currField = fields.head;
  Symbol*  sym       = expr->symbol();
  DefExpr* retval    = NULL;

  while (currField != NULL && retval == NULL) {
    DefExpr* defExpr = toDefExpr(currField);

    if (sym == defExpr->sym) {
      retval    = defExpr;
    } else {
      currField = currField->next;
    }
  }

  return retval;
}

DefExpr* AggregateType::toLocalField(CallExpr* expr) const {
  DefExpr* retval = NULL;

  if (expr->isNamed(".") == true) {
    SymExpr* base = toSymExpr(expr->get(1));
    SymExpr* name = toSymExpr(expr->get(2));

    if (base != NULL && name != NULL) {
      VarSymbol* var = toVarSymbol(name->symbol());

      // The base is <this> and the slot is a fieldName
      if (base->symbol()->hasFlag(FLAG_ARG_THIS) == true &&

          var                                    != NULL &&
          var->immediate                         != NULL &&
          var->immediate->const_kind             == CONST_KIND_STRING) {
        retval = toLocalField(var->immediate->v_string);
      }
    }
  }

  return retval;
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

    if (var->hasFlag(FLAG_EXTERN)) {
      if (!symbol->hasFlag(FLAG_EXTERN)) {
        USR_FATAL_CONT(var, "only external types can have external fields");
      }
    }

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

      fn->setMethod(true);
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

bool AggregateType::hasInitializers() const {
  bool retval = false;

  if (hasUserDefinedInit == true) {
    retval = true;

  } else {
    retval = wantsDefaultInitializer();
  }

  return retval;
}

bool AggregateType::hasPostInitializer() const {
  bool retval = false;

  // If there is postinit() it is defined on the defining type
  if (instantiatedFrom == NULL) {
    int size = methods.n;

    for (int i = 0; i < size && retval == false; i++) {
      retval = methods.v[i]->isPostInitializer();
    }

  } else {
    retval = instantiatedFrom->hasPostInitializer();
  }

  return retval;
}

// For a record
//     Return true if there are uses of new for this type
//
// For a class
//     Return true if there are uses of new or if instances
//     will be allocated via inheritance
bool AggregateType::mayHaveInstances() const {
  bool retval = false;

  if (typeConstructor != NULL) {
    retval = typeConstructor->isResolved();

  } else {
    retval = initializerResolved;
  }

  return retval;
}

// Determine the index for the first generic field (if present).
// Return true if a generic field was found.
bool AggregateType::setFirstGenericField() {
  if (genericField == 0) {
    if (setNextGenericField() == true) {
      symbol->addFlag(FLAG_GENERIC);
    }

    if (isClass() == true) {
      AggregateType* parent = dispatchParents.v[0];

      if (parent->isGeneric() == true) {
        parent->setFirstGenericField();
      }
    }
  }

  return genericField != 0 ? true : false;
}

bool AggregateType::setNextGenericField() {
  int index;

  for (index = genericField + 1; index <= fields.length; index++) {
    bool ignoredHasDefault = false;
    if (fieldIsGeneric(getField(index), ignoredHasDefault) == true) {
      break;
    }
  }

  genericField = index <= fields.length ? index : 0;

  return genericField != 0 ? true : false;
}

/************************************* | **************************************
*                                                                             *
* This method is invoked to handle declarations of the form                   *
*                                                                             *
*   var c : MyGenericType(int, int, 3, real);                                 *
*                                                                             *
* MyGenericType is a type constructor i.e a type function. In this example    *
* it has four formals.  The formals for the type constructor have the same    *
* names as the generic fields.  A use of this type function will find or      *
* create a concrete type that is parameterized with these actuals.            *
*                                                                             *
* This type constructor is associated with a generic type that is one of      *
*   1) A generic record                                                       *
*                                                                             *
*   2) A generic base    class                                                *
*   3) A generic derived class with a concrete parent                         *
*                                                                             *
*   4) A generic derived class with purely     concrete local fields          *
*   5) A generic derived class with additional generic  local fields          *
*                                                                             *
* The first three cases are effectively equivalent; this code needs to bind   *
* the local fields and find/create an instantiated type.                      *
*                                                                             *
* The 4th case is a simple recursive use.  This code must create a concrete   *
* class for the parent class using the actuals, and then generate a simple    *
* instantiation of MyGenericType with the concrete parent.                    *
*                                                                             *
* The final case builds on the former.  It is necessary to instantiate the    *
* generic parent and then instantiate the local generic fields.               *
*                                                                             *
* The resolution process handles a type constructor in much the same way as   *
* any type function.  A SymbolMap is constructed that maps the formals to     *
* this specific type constructor to the the actuals.  If the generic type is  *
* a class with a generic parent, directly or indirectly, then this            *
* dictionary must be passed up the hierarchy so that the parent types can     *
* be instantiated.                                                            *
*                                                                             *
************************************** | *************************************/

AggregateType* AggregateType::generateType(SymbolMap& subs) {
  AggregateType* retval = this;

  // Determine if there is a generic parent class
  if (isClass() == true && symbol->hasFlag(FLAG_NO_OBJECT) == false) {
    AggregateType* parent = dispatchParents.v[0];

    // Is the parent generic?
    if (parent->typeConstructor != NULL &&
        parent->typeConstructor->numFormals() > 0) {
      AggregateType* instantiatedParent = parent->generateType(subs);

      retval = instantiationWithParent(instantiatedParent);
    }
  }

  // Process the local fields
  for (int index = 1; index <= numFields(); index = index + 1) {
    Symbol* field = getField(index);

    bool ignoredHasDefault = false;
    if (fieldIsGeneric(field, ignoredHasDefault)) {
      if (Symbol* val = substitutionForField(field, subs)) {
        retval->genericField = index;

        retval = retval->getInstantiation(val, index);
      }
    }
  }

  retval->instantiatedFrom = this;

  return retval;
}

// Find or create an instantiation that has the provided parent as its parent
AggregateType* AggregateType::instantiationWithParent(AggregateType* parent) {
  AggregateType* retval = NULL;

  // Scan the current instantiations
  for (size_t i = 0; i < instantiations.size() && retval == NULL; i++) {
    AggregateType* at = instantiations[i];

    // Does this instantiation have the desired parent?
    if (at->dispatchParents.v[0] == parent) {
      retval = at;
    }
  }

  // If nothing was found then create a new instantiation
  if (retval == NULL) {
    const char* parentName  = parent->symbol->name;
    const char* parentCname = parent->symbol->cname;
    const char* paren       = strchr(parentName, '(');
    int         rootLen     = (int) (paren - parentName);
    Symbol*     sym         = NULL;

    retval     = toAggregateType(symbol->copy()->type);

    // Update the name/cname based on the parent's name/cname
    sym        = retval->symbol;
    sym->name  = astr(sym->name,  parentName  + rootLen);
    sym->cname = astr(sym->cname, parentCname + rootLen);

    // Update the type of the 'super' field
    for_fields(field, retval) {
      if (field->hasFlag(FLAG_SUPER_CLASS) == true) {
        field->type = parent;
        break;
      }
    }

    retval->instantiatedFrom = this;

    retval->symbol->copyFlags(symbol);

    retval->substitutions.copy(substitutions);

    retval->dispatchParents.add(parent);
    parent->dispatchChildren.add_exclusive(retval);

    if (retval->setFirstGenericField() == false) {
      retval->symbol->removeFlag(FLAG_GENERIC);
    }

    symbol->defPoint->insertBefore(new DefExpr(retval->symbol));

    instantiations.push_back(retval);
  }

  return retval;
}

Symbol* AggregateType::substitutionForField(Symbol*    field,
                                            SymbolMap& subs) const {
  Symbol* retval = NULL;

  form_Map(SymbolMapElem, e, subs) {
    if (strcmp(field->name, e->key->name) == 0) {
      retval = e->value;
      break;
    }
  }

  return retval;
}

// Returns an instantiation of this AggregateType at the given index.
//
// If the index is earlier than this AggregateType's first unsubstituted
// generic field, will just return itself.

// Otherwise, this method will check the list of instantiations for the
// first unsubstituted generic field to see if we have previously made
// an instantiation for the provided argument and will return that
// instantiation if we find one.

// Otherwise, will create a new instantiation with the given
// argument and will return that.
AggregateType* AggregateType::getInstantiation(Symbol* sym, int index) {
  AggregateType* retval = NULL;

  if (index < genericField) {
    retval = this;

  } else if (index == genericField) {
    if (AggregateType* at = getCurInstantiation(sym)) {
      retval = at;
    } else {
      retval = getNewInstantiation(sym);
    }

  } else {
    INT_FATAL(this, "trying to set a later generic field %d", index);
  }

  return retval;
}

AggregateType* AggregateType::getCurInstantiation(Symbol* sym) {
  AggregateType* retval = NULL;

  for_vector(AggregateType, at, instantiations) {
    Symbol* field = at->getField(genericField);

    if (field->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      if (givesType(sym) == true && field->type == sym->typeInfo()) {
        retval = at;
        break;
      }

    } else if (field->hasFlag(FLAG_PARAM) == true) {
      if (at->substitutions.get(field) == sym) {
        retval = at;
        break;
      }

    } else {
      if (field->type == sym->typeInfo()) {
        retval = at;
        break;
      }
    }
  }

  return retval;
}

AggregateType* AggregateType::getNewInstantiation(Symbol* sym) {
  AggregateType* retval = toAggregateType(symbol->copy()->type);
  Symbol*        field  = retval->getField(genericField);

  symbol->defPoint->insertBefore(new DefExpr(retval->symbol));

  retval->instantiatedFrom = this;

  retval->symbol->copyFlags(symbol);

  retval->substitutions.copy(substitutions);

  if (field->hasFlag(FLAG_PARAM) == true) {
    retval->substitutions.put(field, sym);
    retval->symbol->renameInstantiatedSingle(sym);

  } else {
    retval->substitutions.put(field, sym->typeInfo()->symbol);
    retval->symbol->renameInstantiatedSingle(sym->typeInfo()->symbol);
  }

  if (field->hasFlag(FLAG_TYPE_VARIABLE) == true && givesType(sym) == true) {
    field->type = sym->typeInfo();

  } else if (field->defPoint->exprType == NULL) {
    if (field->type == dtUnknown) {
      field->type = sym->typeInfo();
    }

  } else {
    if (field->defPoint->exprType->typeInfo() == sym->typeInfo()) {
      field->type = sym->typeInfo();
    }
  }

  forv_Vec(AggregateType, at, dispatchParents) {
    retval->dispatchParents.add(at);
    at->dispatchChildren.add_exclusive(retval);
  }

  if (retval->setNextGenericField() == false) {
    retval->symbol->removeFlag(FLAG_GENERIC);
  }

  instantiations.push_back(retval);

  return retval;
}

AggregateType*
AggregateType::getInstantiationParent(AggregateType* parentType) {
  if (!this->isClass()) {
    INT_FATAL(this,
              "only call getInstantiationParent on classes, others can't "
              "inherit");
  }

  // First, look to see if we have an instantiation with that value already
  for_vector(AggregateType, at, instantiations) {
    Symbol* field = at->getField(1); // super is always the first field

    if (field->type == parentType) {
      return at;
    }
  }

  // Otherwise, we need to create an instantiation for that type
  AggregateType* newInstance = toAggregateType(this->symbol->copy()->type);

  this->symbol->defPoint->insertBefore(new DefExpr(newInstance->symbol));

  newInstance->symbol->copyFlags(this->symbol);

  newInstance->substitutions.copy(this->substitutions);

  Symbol* field = newInstance->getField(1);
  newInstance->substitutions.put(field, parentType->symbol);
  newInstance->symbol->renameInstantiatedFromSuper(parentType->symbol);

  field->type = parentType;

  instantiations.push_back(newInstance);
  newInstance->instantiatedFrom = this;

  // Handle dispatch parent
  newInstance->dispatchParents.add(parentType);

  bool inserted = parentType->dispatchChildren.add_exclusive(newInstance);

  INT_ASSERT(inserted);

  if (newInstance->setFirstGenericField() == false) {
    newInstance->symbol->removeFlag(FLAG_GENERIC);
  }

  return newInstance;
}

bool AggregateType::isInstantiatedFrom(const AggregateType* base) const {
  const AggregateType* type   = this;
  bool                 retval = false;

  while (type != NULL && retval == false) {
    if (type == base) {
      retval = true;
    } else {
      type = type->instantiatedFrom;
    }
  }

  return retval;
}

AggregateType* AggregateType::getRootInstantiation() {
  AggregateType* retval = this;

  while (retval->instantiatedFrom != NULL) {
    retval = retval->instantiatedFrom;
  }

  return retval;
}

int AggregateType::getFieldPosition(const char* name, bool fatal) {
  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;

  current_p->set_add(this);

  int fieldPos = 1;

  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (AggregateType* ct = toAggregateType(t)) {
        for_fields(sym, ct) {
          if (!strcmp(sym->name, name)) {
            return fieldPos;
          }

          fieldPos++;
        }

        forv_Vec(AggregateType, parent, ct->dispatchParents) {
          if (parent != NULL) {
            next_p->set_add(parent);
          }
        }
      }
    }

    Vec<Type*>* temp = next_p;

    next_p    = current_p;
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


Symbol* AggregateType::getField(const char* name, bool fatal) const {
  // non-const-this: a workaround for const issues with Vec, baseAST
  AggregateType*       ncThis = const_cast<AggregateType*>(this);

  Vec<AggregateType*>  next;
  Vec<AggregateType*>  current;
  Vec<AggregateType*>* nextP    = &next;
  Vec<AggregateType*>* currentP = &current;

  currentP->set_add(ncThis);

  while (currentP->n != 0) {
    forv_Vec(AggregateType, ct, *currentP) {
      for_fields(sym, ct) {
        if (strcmp(sym->name, name) == 0) {
          return sym;
        }
      }

      forv_Vec(AggregateType, parent, ct->dispatchParents) {
        if (parent != NULL) {
          nextP->set_add(parent);
        }
      }
    }

    Vec<AggregateType*>* temp = nextP;

    nextP    = currentP;
    currentP = temp;

    nextP->clear();
  }

  if (fatal == true) {
    const char* className = "<no name>";

    if (this->symbol) { // this is always true?
      className = this->symbol->name;
    }

    // TODO: report as a user error in certain cases
    INT_FATAL(this,
              "no field '%s' in class '%s' in getField()",
              name,
              className);
  }

  return NULL;
}


Symbol* AggregateType::getField(int i) const {
  return toDefExpr(fields.get(i))->sym;
}

QualifiedType AggregateType::getFieldType(Expr* e) {
  SymExpr*   sym = NULL;
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
* Create the (default) type constructor for this class.                       *
*                                                                             *
************************************** | *************************************/

FnSymbol* AggregateType::buildTypeConstructor() {
  if (typeConstructor != NULL) {
    return typeConstructor;
  }

  SET_LINENO(this);

  const char* name   = astr("_type_construct_", symbol->name);
  const char* cName  = astr("_type_construct_", symbol->cname);
  VarSymbol*  _this  = new VarSymbol("this", this);
  FnSymbol*   retval = new FnSymbol(name);

  _this->addFlag(FLAG_ARG_THIS);

  retval->cname   = cName;
  retval->retTag  = RET_TYPE;
  retval->retType = this;
  retval->_this   = _this;

  retval->addFlag(FLAG_TYPE_CONSTRUCTOR);
  retval->addFlag(FLAG_COMPILER_GENERATED);
  retval->addFlag(FLAG_LAST_RESORT);

  if (symbol->hasFlag(FLAG_REF)   == true) {
    retval->addFlag(FLAG_REF);
  }

  if (symbol->hasFlag(FLAG_TUPLE) == true) {
    retval->addFlag(FLAG_TUPLE);
    retval->addFlag(FLAG_INLINE);

    gGenericTupleTypeCtor = retval;
  }

  symbol->defPoint->insertBefore(new DefExpr(retval));

  retval->insertAtTail(new DefExpr(_this));

  if (isClass() == true && dispatchParents.n > 0) {
    typeConstrSetFields(retval, typeConstrSuperCall(retval));

  } else {
    typeConstrSetFields(retval, NULL);
  }

  retval->insertAtTail(new CallExpr(PRIM_RETURN, _this));

  addToSymbolTable(retval);

  typeConstructor = retval;

  return retval;
}

CallExpr* AggregateType::typeConstrSuperCall(FnSymbol* fn) const {
  AggregateType* parent        = dispatchParents.v[0];
  FnSymbol*      superTypeCtor = parent->typeConstructor;
  CallExpr*      retval        = NULL;

  if (superTypeCtor == NULL) {
    superTypeCtor = parent->buildTypeConstructor();
  }

  if (superTypeCtor->numFormals() > 0) {
    retval = new CallExpr(parent->symbol);

    for_formals(formal, superTypeCtor) {
      ArgSymbol* arg = toArgSymbol(formal->copy());

      if (isFieldInThisClass(arg->name) == false) {
        arg->addFlag(FLAG_PARENT_FIELD);

        fn->insertFormalAtTail(arg);

        retval->insertAtTail(new SymExpr(arg));
      }
    }
  }

  return retval;
}

bool AggregateType::isFieldInThisClass(const char* name) const {
  bool retval = false;

  for_fields(sym, this) {
    if (strcmp(sym->name, name) == 0) {
      retval = true;
      break;
    }
  }

  return retval;
}

void AggregateType::typeConstrSetFields(FnSymbol* fn,
                                        CallExpr* superCall) const {
  Vec<const char*> fieldNamesSet;

  if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
    AggregateType* outerType = toAggregateType(ts->type);
    outerType->moveTypeConstructorToOuter(fn);
  }

  for_fields(tmp, this) {
    SET_LINENO(tmp);

    if (VarSymbol* field = toVarSymbol(tmp)) {
      if (field->hasFlag(FLAG_SUPER_CLASS) == true) {
        if (superCall != NULL) {
          CallExpr* call = new CallExpr(PRIM_TYPE_INIT, superCall);

          typeConstrSetField(fn, field, call);
        }

      } else {
        fieldNamesSet.set_add(field->name);

        if (field->isType()            == true ||
            field->hasFlag(FLAG_PARAM) == true) {
          ArgSymbol* arg = insertGenericArg(fn, field);

          typeConstrSetField(fn, field, new SymExpr(arg));

        } else if (field->defPoint->exprType
                   && !isFieldTypeExprGeneric(field->defPoint->exprType)) {
          Expr* type = field->defPoint->exprType;
          CallExpr* call = new CallExpr(PRIM_TYPE_INIT, type->copy());

          typeConstrSetField(fn, field, call);

        } else if (Expr* init = field->defPoint->init) {
          // It might be appealing to change this to PRIM_INIT_FIELD -
          // but that causes problems for fields with loop init expressions.
          // The field should end up with array type rather than iterator
          // record type.

          CallExpr* call = new CallExpr("chpl__initCopy", init->copy());

          typeConstrSetField(fn, field, call);

        } else {
          ArgSymbol* arg = insertGenericArg(fn, field);

          if (symbol->hasFlag(FLAG_REF) == false) {
            CallExpr* call = new CallExpr(PRIM_TYPE_INIT, new SymExpr(arg));

            typeConstrSetField(fn, field, call);
          }
        }
      }
    }
  }

  insertImplicitThis(fn, fieldNamesSet);

  resolveUnresolvedSymExprs(fn);
}

void AggregateType::typeConstrSetField(FnSymbol*  fn,
                                       VarSymbol* field,
                                       Expr*      expr) const {
  Symbol* _this = fn->_this;
  Symbol* name  = new_CStringSymbol(field->name);

  fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, _this, name, expr));
}

ArgSymbol* AggregateType::insertGenericArg(FnSymbol*  fn,
                                           VarSymbol* field) const {
  Expr*      type = field->defPoint->exprType;
  Expr*      init = field->defPoint->init;
  ArgSymbol* arg  = new ArgSymbol(INTENT_BLANK, field->name, field->type);

  if (field->hasFlag(FLAG_PARENT_FIELD) == true) {
    arg->addFlag(FLAG_PARENT_FIELD);
  }

  if (field->hasFlag(FLAG_PARAM) == true) {
    arg->intent = INTENT_PARAM;

  } else {
    arg->addFlag(FLAG_TYPE_VARIABLE);
  }

  if (type != NULL) {
    arg->typeExpr    = new BlockStmt(type->copy(), BLOCK_TYPE);
  }

  if (init != NULL) {
    arg->defaultExpr = new BlockStmt(init->copy(), BLOCK_SCOPELESS);
  }

  if (type == NULL && arg->type == dtUnknown) {
    if (field->isType() == false) {
      arg->addFlag(FLAG_GENERIC);
    }

    arg->type = dtAny;
  }

  fn->insertFormalAtTail(arg);

  return arg;
}

void AggregateType::buildDefaultInitializer() {
  if (builtDefaultInit == false &&
      symbol->hasFlag(FLAG_REF) == false) {
    SET_LINENO(this);
    FnSymbol*  fn    = new FnSymbol("init");
    ArgSymbol* _mt   = new ArgSymbol(INTENT_BLANK, "_mt",  dtMethodToken);
    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", this);

    fn->cname = fn->name;
    fn->_this = _this;

    // Lydia NOTE 06/16/17: I don't think I want to add the
    //  DEFAULT_CONSTRUCTOR flag to this function, but if I do,
    // then I will need to do something different in wrappers.cpp.
    fn->addFlag(FLAG_COMPILER_GENERATED);
    fn->addFlag(FLAG_LAST_RESORT);
    fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);

    _this->addFlag(FLAG_ARG_THIS);

    fn->insertFormalAtTail(_mt);
    fn->insertFormalAtTail(_this);

    std::set<const char*> names;
    SymbolMap fieldArgMap;

    if (addSuperArgs(fn, names, fieldArgMap) == true) {
      // Parent fields before child fields
      fieldToArg(fn, names, fieldArgMap);

      // Replaces field references with argument references
      // NOTE: doesn't handle inherited fields yet!
      update_symbols(fn, &fieldArgMap);

      DefExpr* def = new DefExpr(fn);

      symbol->defPoint->insertBefore(def);

      fn->setMethod(true);
      fn->addFlag(FLAG_METHOD_PRIMARY);

      preNormalizeInitMethod(fn);

      if (this->isUnion()) {
        fn->insertAtTail(new CallExpr(PRIM_SET_UNION_ID,
                                      fn->_this,
                                      new_IntSymbol(0)));
      }

      normalize(fn);

      // BHARSH INIT TODO: Should this be part of normalize(fn)? If we did that
      // we would emit two use-before-def errors for classes because of the
      // generated _new function.
      checkUseBeforeDefs(fn);

      methods.add(fn);
    } else {
      USR_FATAL(this, "Unable to generate initializer for type '%s'", this->symbol->name);
    }

    builtDefaultInit = true;
  }
}

void AggregateType::fieldToArg(FnSymbol*              fn,
                               std::set<const char*>& names,
                               SymbolMap&             fieldArgMap) {
  for_fields(fieldDefExpr, this) {
    SET_LINENO(fieldDefExpr);

    if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
      if (field->hasFlag(FLAG_SUPER_CLASS) == false) {

        DefExpr*    defPoint = field->defPoint;
        const char* name     = field->name;
        ArgSymbol*  arg      = new ArgSymbol(INTENT_IN, name, dtUnknown);

        names.insert(name);
        fieldArgMap.put(field, arg);

        // Insert initialization for each field from the argument provided.
        SET_LINENO(field);

        if (field->hasFlag(FLAG_PARAM) == true) {
          arg->intent = INTENT_PARAM;
        }

        if (field->isType() == true) {
          arg->intent = INTENT_BLANK;
          arg->addFlag(FLAG_TYPE_VARIABLE);
        }

        if (LoopExpr* fe = toLoopExpr(defPoint->init)) {
          if (field->isType() == false) {
            CallExpr* copy = new CallExpr("chpl__initCopy");
            defPoint->init->replace(copy);
            copy->insertAtTail(fe);
          }
        }

        //
        // A generic field.  Could be type/param/variable
        //
        if        (defPoint->exprType == NULL && defPoint->init == NULL) {
          arg->type = dtAny;


        //
        // Type inference is required if this is a param or variable field
        //
        } else if (defPoint->exprType == NULL && defPoint->init != NULL) {
          if (arg->hasFlag(FLAG_TYPE_VARIABLE)) {
            arg->type = dtAny;
          }

          // set up the ArgSymbol appropriately for the type
          // and initialization from the field declaration.
          arg->defaultExpr = new BlockStmt(defPoint->init->copy());

          // mimic normalize's hack_resolve_types
          arg->typeExpr = arg->defaultExpr->copy();


        //
        // Type is defined and default value should be applied
        // Could be param or variable
        //
        } else if (defPoint->exprType != NULL && defPoint->init == NULL) {
          // set up the ArgSymbol appropriately for the type
          // and initialization from the field declaration.
          Expr* initVal = new SymExpr(gTypeDefaultToken);

          fieldToArgType(defPoint, arg);

          if (arg->intent != INTENT_PARAM) {
            arg->defaultExpr = new BlockStmt(initVal);
          }


        //
        // Type is defined and type of init value must be consistent
        // Could be param or variable
        //
        } else if (defPoint->exprType != NULL && defPoint->init != NULL) {
          if (field->hasFlag(FLAG_PARAM)) {
            fieldToArgType(defPoint, arg);

            arg->defaultExpr = new BlockStmt(defPoint->init->copy());

          } else {
            fieldToArgType(defPoint, arg);

            CallExpr* def    = new CallExpr("_createFieldDefault",
                                            defPoint->exprType->copy(),
                                            defPoint->init->copy());

            arg->defaultExpr = new BlockStmt(def);
          }
        }

        fn->insertFormalAtTail(arg);

        fn->insertAtTail(new CallExpr("=",
                                      new CallExpr(".",
                                                   fn->_this,
                                                   new_CStringSymbol(name)),
                                      arg));
      }
    }
  }
}

void AggregateType::fieldToArgType(DefExpr* fieldDef, ArgSymbol* arg) {
  BlockStmt* exprType = new BlockStmt(fieldDef->exprType->copy(), BLOCK_TYPE);

  // If the type is simple, just set the argument's type directly.
  // Otherwise, give it the block we just created.
  if (exprType->body.length == 1) {
    Type* type = exprType->body.only()->typeInfo();
    if (type != dtUnknown && type != dtAny) {
      arg->type = type;

    } else {
      arg->typeExpr = exprType;
    }

  } else {
    arg->typeExpr = exprType;
  }
}

bool AggregateType::addSuperArgs(FnSymbol*                    fn,
                                 const std::set<const char*>& names,
                                 SymbolMap& fieldArgMap) {
  bool retval = true;

  // Lydia NOTE 06/16/17: be sure to avoid applying this to tuples, too!
  if (isClass()                    ==  true &&
      symbol->hasFlag(FLAG_REF)    == false &&
      dispatchParents.n            >      0 &&
      symbol->hasFlag(FLAG_EXTERN) == false) {
    if (AggregateType* parent = dispatchParents.v[0]) {
      CallExpr* superPortion = new CallExpr(".",
                                            new SymExpr(fn->_this),
                                            new_CStringSymbol("super"));

      SymExpr*  initPortion  = new SymExpr(new_CStringSymbol("init"));
      CallExpr* base         = new CallExpr(".", superPortion, initPortion);
      CallExpr* superCall    = new CallExpr(base);

      if (parent->hasUserDefinedInit == false && parent != dtObject) {
        // We want to call the compiler-generated all-fields initializer

        // First, ensure we have a default initializer for the parent
        if (parent->builtDefaultInit == false && parent->wantsDefaultInitializer()) {
          parent->buildDefaultInitializer();
        }

        // Otherwise, we are good to go!
        FnSymbol* defaultInit = NULL;
        forv_Vec(FnSymbol, method, parent->methods) {
          if (method->isDefaultInit()) {
            defaultInit = method;
            break;
          }
        }

        if (defaultInit == NULL) {
          retval = false;
        } else {
          // Add an argument per argument in the parent initializer
          for_formals(formal, defaultInit) {
            if (formal->type                   == dtMethodToken ||
                formal->hasFlag(FLAG_ARG_THIS) == true) {

            // Skip arguments shadowed by this class' fields
            } else if (names.find(formal->name) != names.end()) {

            } else {
              DefExpr* superArg = formal->defPoint->copy();

              VarSymbol* field = toVarSymbol(parent->getField(superArg->sym->name));
              fieldArgMap.put(field, superArg->sym);
              fieldArgMap.put(formal, superArg->sym);

              fn->insertFormalAtTail(superArg);

              superCall->insertAtTail(superArg->sym);
            }
          }
        }

      }

      fn->body->insertAtHead(superCall);
    }
  }

  // Nothing to be done for records.

  return retval;
}

void AggregateType::buildCopyInitializer() {
  if (isRecordWithInitializers(this) == true) {
    SET_LINENO(this);

    FnSymbol*  fn    = new FnSymbol("init");

    DefExpr*   def   = new DefExpr(fn);

    ArgSymbol* _mt   = new ArgSymbol(INTENT_BLANK, "_mt",   dtMethodToken);
    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this",  this);
    ArgSymbol* other = new ArgSymbol(INTENT_BLANK, "other", this);

    fn->cname = fn->name;
    fn->_this = _this;

    fn->addFlag(FLAG_COMPILER_GENERATED);
    fn->addFlag(FLAG_LAST_RESORT);
    fn->addFlag(FLAG_COPY_INIT);

    _this->addFlag(FLAG_ARG_THIS);

    // Detect if the type has at least one generic field,
    // so we should mark the "other" arg as generic.
    for_fields(fieldDefExpr, this) {
      if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
        if (field->hasFlag(FLAG_SUPER_CLASS) == false) {
          if (field->hasFlag(FLAG_PARAM) ||
              field->isType() == true    ||
              (field->defPoint->init     == NULL &&
               field->defPoint->exprType == NULL)) {

            if (other->hasFlag(FLAG_MARKED_GENERIC) == false) {
              other->addFlag(FLAG_MARKED_GENERIC);
            }
          }
        }
      }
    }

    fn->insertFormalAtTail(_mt);
    fn->insertFormalAtTail(_this);
    fn->insertFormalAtTail(other);

    if (symbol->hasFlag(FLAG_EXTERN)) {
      if (other->hasFlag(FLAG_MARKED_GENERIC))
        INT_FATAL("extern type is generic");

      // Generate a bit-copy for extern records in order to copy unknown fields.
      if (symbol->hasFlag(FLAG_EXTERN)) {
        fn->insertAtHead(new CallExpr(PRIM_ASSIGN, fn->_this, other));
      }
    } else {
      // Copy the fields from "other" into our fields
      for_fields(fieldDefExpr, this) {
        if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
          if (field->hasFlag(FLAG_SUPER_CLASS) == false) {
            const char* name       = field->name;

            CallExpr*   thisField  = new CallExpr(".",
                                                  fn->_this,
                                                  new_CStringSymbol(name));

            CallExpr*   otherField = new CallExpr(".",
                                                  other,
                                                  new_CStringSymbol(name));

            fn->insertAtTail(new CallExpr("=", thisField, otherField));
          }
        }
      }
    }

    symbol->defPoint->insertBefore(def);

    fn->setMethod(true);
    fn->addFlag(FLAG_METHOD_PRIMARY);

    if (symbol->hasFlag(FLAG_EXTERN) == false)
      preNormalizeInitMethod(fn);

    normalize(fn);

    methods.add(fn);
  }
}

//
// Returns true for the cases where we want to generate a default initializer.
// Some internal types (e.g. tuples) do not currently use initializers.
//
bool AggregateType::wantsDefaultInitializer() const {
  bool           retval       = true;

  if (symbol->hasFlag(FLAG_TUPLE)) {
    return false;

  // Only want a default initializer when no
  // initializer or constructor is defined
  } else if (hasUserDefinedInit == true) {
    retval = false;

  // Iterator classes and records want neither default constructors nor
  // initializers, and never will
  } else if (symbol->hasFlag(FLAG_ITERATOR_CLASS) ||
             symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    retval = false;
  }

  return retval;
}

// Replace implicit references to 'this' in the body of this
// type constructor with explicit member reference (dot) expressions.
void AggregateType::insertImplicitThis(FnSymbol*         fn,
                                       Vec<const char*>& fieldNamesSet) const {
  std::vector<BaseAST*> asts;

  collect_asts(fn->body, asts);

  for_vector(BaseAST, ast, asts) {
    if (UnresolvedSymExpr* se = toUnresolvedSymExpr(ast)) {
      if (fieldNamesSet.set_in(se->unresolved)) {
        // The name of this UnresolvedSymExpr matches a field name.
        // So replace it with a dot expression.
        se->replace(buildDotExpr(fn->_this, se->unresolved));
      }
    } else if (SymExpr* se = toSymExpr(ast)) {
      DefExpr* def = this->toLocalField(se);
      if (def == NULL) {
        def = this->toSuperField(se);
      }
      if (def != NULL && isTypeSymbol(def->sym) == false) {
        se->replace(buildDotExpr(fn->_this, se->symbol()->name));
      }
    }
  }
}

void AggregateType::moveTypeConstructorToOuter(FnSymbol* fn) {
  Expr*      insertPoint = symbol->defPoint;

  while (isTypeSymbol(insertPoint->parentSymbol) == true) {
    insertPoint = insertPoint->parentSymbol->defPoint;
  }

  insertPoint->insertBefore(fn->defPoint->remove());
}

/************************************* | **************************************
*                                                                             *
* Compute dispatchParents and dispatchChildren vectors; add base class fields *
* to subclasses; identify cyclic or illegal class or record hierarchies       *
*                                                                             *
************************************** | *************************************/

void AggregateType::addClassToHierarchy() {
  std::set<AggregateType*> localSeen; // classes in potential cycle

  addClassToHierarchy(localSeen);
}

void AggregateType::addClassToHierarchy(std::set<AggregateType*>& localSeen) {
  // classes already in hierarchy
  static std::set<AggregateType*> globalSeen;

  if (localSeen.find(this)  != localSeen.end())  {
    USR_FATAL(this, "Class hierarchy is cyclic");
  }

  if (globalSeen.find(this) != globalSeen.end()) {
    return;
  }

  globalSeen.insert(this);

  addRootType();

  // Walk the base class list, and add parents into the class hierarchy.
  for_alist(expr, inherits) {
    AggregateType* pt = discoverParentAndCheck(expr);

    localSeen.insert(this);

    pt->addClassToHierarchy(localSeen);

    dispatchParents.add(pt);

    if (isGeneric() == false && pt->isGeneric() == true) {
      markAsGeneric();
    }

    if (pt->dispatchChildren.add_exclusive(this) == false) {
      INT_ASSERT(false);
    }

    expr->remove();

    if (isClass() == true) {
      SET_LINENO(this);

      // For a class, just add a super class pointer.
      VarSymbol* super = new VarSymbol("super", pt);

      super->addFlag(FLAG_SUPER_CLASS);

      fields.insertAtHead(new DefExpr(super));

    } else {
      SET_LINENO(this);

      // For records and unions, scan the fields in the parent type.
      for_fields_backward(field, pt) {
        if (isVarSymbol(field)               == true &&
            field->hasFlag(FLAG_SUPER_CLASS) == false) {
          // If not already in derived class (by name), copy it.
          bool alreadyContainsField = false;

          for_fields(myfield, this) {
            if (strcmp(myfield->name, field->name) == 0) {
              alreadyContainsField = true;
              break;
            }
          }

          if (alreadyContainsField == false) {
            DefExpr* def = field->defPoint->copy();

            fields.insertAtHead(def);

            def->sym->addFlag(FLAG_PARENT_FIELD);
          }
        }
      }
    }
  }
}

AggregateType* AggregateType::discoverParentAndCheck(Expr* storesName) {
  TypeSymbol*        ts  = NULL;

  if (UnresolvedSymExpr* se = toUnresolvedSymExpr(storesName)) {
    Symbol* sym = lookup(se->unresolved, storesName);
    ts = toTypeSymbol(sym);
  } else if (SymExpr* se = toSymExpr(storesName)) {
    ts = toTypeSymbol(se->symbol());
  }

  if (ts == NULL) {
    USR_FATAL(storesName, "Illegal super class");
  }

  AggregateType* pt = toAggregateType(ts->type);

  if (pt == NULL) {
    USR_FATAL(storesName, "Illegal super class %s", ts->name);
  }

  if (isUnion() == true && pt->isUnion() == true) {
    USR_FATAL(storesName, "Illegal inheritance involving union type");
  }

  if (isRecord() == true && pt->isClass() == true) {
    USR_FATAL(storesName,
              "Record %s inherits from class %s",
              symbol->name,
              pt->symbol->name);
  }

  if (isClass() == true && pt->isRecord() == true) {
    USR_FATAL(storesName,
              "Class %s inherits from record %s",
              symbol->name,
              pt->symbol->name);
  }

  return pt;
}

void AggregateType::setCreationStyle(TypeSymbol* t, FnSymbol* fn) {
  bool isInit = (strcmp(fn->name, "init")   == 0);

  if (isInit) {
    AggregateType* ct = toAggregateType(t->type);

    if (ct == NULL) {
      INT_FATAL(fn, "initializer on non-class type");
    }

    if (fn->hasFlag(FLAG_NO_PARENS)) {
      USR_FATAL(fn, "an initializer cannot be declared without parentheses");
    }

    if (fn->hasFlag(FLAG_METHOD_PRIMARY) == false &&
        fn->getModule() != t->getModule()) {
      // We are looking at a secondary initializer defined in a module
      // other than the module defining the type.
      USR_WARN(fn, "initializers defined outside the module where the "
               "type was originally defined may cause issues");
      USR_PRINT(fn, "This will no longer be a problem when constructors "
                "are deprecated");
    }

    if (ct->hasUserDefinedInit == false) {
      // We hadn't previously seen an initializer definition.
      // Update the field on the type appropriately.
      if (fn->hasFlag(FLAG_METHOD_PRIMARY) == true ||
          fn->getModule() == t->getModule()) {
        // Only mark the type as defining an initializer if the initializer
        // we found was in the same module as the type itself.  If there is
        // no such initializer, we would need to define a default initializer
        // for the scopes where the secondary initializer is not visible.
        ct->hasUserDefinedInit = true;
      }
    }
  }
}

void AggregateType::addRootType() {
  if (isClass() == true) {
    if (inherits.length == 0 && symbol->hasFlag(FLAG_NO_OBJECT) == false) {
      SET_LINENO(this);

      VarSymbol* super = new VarSymbol("super", dtObject);

      dispatchParents.add(dtObject);

      // Assume that this addition is unique; report if not.
      if (dtObject->dispatchChildren.add_exclusive(this) == false) {
        INT_ASSERT(false);
      }

      super->addFlag(FLAG_SUPER_CLASS);

      fields.insertAtHead(new DefExpr(super));
    }
  }
}

DefExpr* defineObjectClass() {
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
  // Because we never create the actual field, we have a special case
  //  for it in TypeSymbol::codegenAggMetadata().  Remember to change
  //  that special case if we ever change the contents of object or
  //  if we start creating the field.
  //
  DefExpr* retval = buildClassDefExpr("object",
                                      NULL,
                                      AGGREGATE_CLASS,
                                      NULL,
                                      new BlockStmt(),
                                      FLAG_UNKNOWN,
                                      NULL);

  retval->sym->addFlag(FLAG_OBJECT_CLASS);

  // Prevents removal in pruneResolvedTree().
  retval->sym->addFlag(FLAG_GLOBAL_TYPE_SYMBOL);
  retval->sym->addFlag(FLAG_NO_OBJECT);

  dtObject = toAggregateType(retval->sym->type);

  INT_ASSERT(isAggregateType(dtObject));

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Symbol* AggregateType::getSubstitution(const char* name) {
  Vec<Symbol*> keys;
  Symbol*      retval = NULL;

  substitutions.get_keys(keys);

  forv_Vec(Symbol, key, keys) {
    if (strcmp(name, key->name) == 0) {
      retval = substitutions.get(key);
      break;
    }
  }

  if (retval == NULL && dispatchParents.n == 1) {
    retval = dispatchParents.v[0]->getSubstitution(name);
  }

  return retval;
}

UnmanagedClassType* AggregateType::getUnmanagedClass() {
  if (aggregateTag == AGGREGATE_CLASS) {

    if (!unmanagedClass)
      generateUnmanagedClassTypes();

    return unmanagedClass;
  }
  return NULL;
}

void AggregateType::generateUnmanagedClassTypes() {
  AggregateType* at = this;
  if (aggregateTag == AGGREGATE_CLASS && at->unmanagedClass == NULL) {
    SET_LINENO(at->symbol->defPoint);
    // Generate unmanaged class type
    UnmanagedClassType* unmanaged = new UnmanagedClassType(at);
    at->unmanagedClass = unmanaged;
    TypeSymbol* tsUnmanaged = new TypeSymbol(astr("unmanaged ", at->symbol->name), unmanaged);
    // The unmanaged type isn't really an object, shouldn't have its own fields
    tsUnmanaged->addFlag(FLAG_NO_OBJECT);
    // Propagate generic-ness to the unmanaged type
    if (at->isGeneric() || at->symbol->hasFlag(FLAG_GENERIC))
      tsUnmanaged->addFlag(FLAG_GENERIC);
    // The generated code should just use the canonical class name
    tsUnmanaged->cname = at->symbol->cname;
    DefExpr* defUnmanaged = new DefExpr(tsUnmanaged);
    at->symbol->defPoint->insertAfter(defUnmanaged);
  }
}

Type* AggregateType::cArrayElementType() const {
  TypeSymbol* eltTS = NULL;
  INT_ASSERT(symbol->hasFlag(FLAG_C_ARRAY));
  form_Map(SymbolMapElem, e, substitutions) {
    if (TypeSymbol* ets = toTypeSymbol(e->value))
      eltTS = ets;
  }
  INT_ASSERT(eltTS);
  return eltTS->type;
}

int64_t AggregateType::cArrayLength() const {
  VarSymbol* sizeVar = NULL;
  INT_ASSERT(symbol->hasFlag(FLAG_C_ARRAY));
  form_Map(SymbolMapElem, e, substitutions) {
    if (VarSymbol* evs = toVarSymbol(e->value))
      sizeVar = evs;
  }
  INT_ASSERT(sizeVar);
  Immediate* imm = getSymbolImmediate(sizeVar);
  INT_ASSERT(imm);
  int64_t sizeInt = imm->to_int();
  if (sizeInt < 0)
    USR_FATAL(symbol, "c_array must have positive size");
  return sizeInt;
}
