/*
 * Copyright 2004-2018 Cray Inc.
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
#include "UnmanagedClassType.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

AggregateType* dtObject = NULL;
AggregateType* dtString = NULL;

AggregateType::AggregateType(AggregateTag initTag) :
  Type(E_AggregateType, NULL) {

  aggregateTag        = initTag;
  unmanagedClass      = NULL;

  typeConstructor     = NULL;
  defaultInitializer  = NULL;
  initializerStyle    = DEFINES_NONE_USE_DEFAULT;
  initializerResolved = false;
  outer               = NULL;
  iteratorInfo        = NULL;
  doc                 = NULL;

  instantiatedFrom    = NULL;

  fields.parent       = this;
  inherits.parent     = this;

  genericField        = 0;
  mIsGeneric          = false;

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

bool AggregateType::fieldIsGeneric(Symbol* field) const {
  bool retval = false;

  if (VarSymbol* var = toVarSymbol(field)) {
    if (var->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      retval = true;

    } else if (var->hasFlag(FLAG_PARAM) == true) {
      retval = true;

    } else if (var->type == dtUnknown) {
      DefExpr* def = var->defPoint;

      if (def->init == NULL && def->exprType == NULL) {
        retval = true;
      }
    }
  }

  return retval;
}

DefExpr* AggregateType::toSuperField(SymExpr*  expr) {
  DefExpr* retval = NULL;

  if (isClass() == true) {
    forv_Vec(AggregateType, pt, dispatchParents) {
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

DefExpr* AggregateType::toSuperField(CallExpr* expr) {
  DefExpr* retval = NULL;

  if (isClass() == true) {
    forv_Vec(AggregateType, pt, dispatchParents) {
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

  if (initializerStyle == DEFINES_INITIALIZER) {
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
    if (fieldIsGeneric(getField(index)) == true) {
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
  if (isClass() == true) {
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

    if (fieldIsGeneric(field) == true) {
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

void AggregateType::createOuterWhenRelevant() {
  SET_LINENO(this);
  Symbol* parSym = symbol->defPoint->parentSymbol;

  if (AggregateType* outerType = toAggregateType(parSym->type)) {

    // Lydia NOTE 09/12/17: (Temporary) error case
    if (outerType->initializerStyle == DEFINES_INITIALIZER ||
        initializerStyle            == DEFINES_INITIALIZER) {
      if (outerType->isGeneric() || isGeneric()) {
        USR_FATAL(this,
                  "initializers not supported on nested types "
                  "when either type is generic");
      }
    }

    // Create an "outer" pointer to the outer class in the inner class
    VarSymbol* tmpOuter = new VarSymbol("outer", outerType);

    // Save the pointer to the outer class
    fields.insertAtTail(new DefExpr(tmpOuter));

    outer = tmpOuter;
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void AggregateType::buildConstructors() {
  if (defaultInitializer == NULL) {
    SET_LINENO(this);

    if (typeConstructor == NULL) {
      buildTypeConstructor();
    }

    buildConstructor();
  }
}

/************************************* | **************************************
*                                                                             *
* Create the (default) type constructor for this class.                       *
*                                                                             *
************************************** | *************************************/

FnSymbol* AggregateType::buildTypeConstructor() {
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
    retval = new CallExpr(parent->symbol->name);

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

  for_fields(tmp, this) {
    SET_LINENO(tmp);

    if (VarSymbol* field = toVarSymbol(tmp)) {
      if (field->hasFlag(FLAG_SUPER_CLASS) == true) {
        if (superCall != NULL) {
          CallExpr* call = new CallExpr(PRIM_TYPE_INIT, superCall);

          typeConstrSetField(fn, field, call);
        }

      } else if (field == this->outer) {
        Symbol*        _this     = fn->_this;
        Symbol*        name      = new_CStringSymbol("outer");
        AggregateType* outerType = toAggregateType(outer->type);
        Symbol*        _outer    = outerType->moveConstructorToOuter(fn);

        fn->insertAtHead(new CallExpr(PRIM_SET_MEMBER, _this, name, _outer));

      } else {
        fieldNamesSet.set_add(field->name);

        if (field->isType()            == true ||
            field->hasFlag(FLAG_PARAM) == true) {
          ArgSymbol* arg = insertGenericArg(fn, field);

          typeConstrSetField(fn, field, new SymExpr(arg));

        } else if (Expr* type = field->defPoint->exprType) {
          CallExpr* call = new CallExpr(PRIM_TYPE_INIT,   type->copy());

          typeConstrSetField(fn, field, call);

        } else if (Expr* init = field->defPoint->init) {
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

/************************************* | **************************************
*                                                                             *
* For the given class type, this builds the compiler-generated constructor    *
* which is also called by user-defined constructors to pre-initialize all     *
* fields to their declared or type-specific initial values.                   *
*                                                                             *
************************************** | *************************************/

void AggregateType::buildConstructor() {
  if (initializerStyle == DEFINES_INITIALIZER) {
    // Don't want to create the default constructor if we have seen
    // initializers defined.  The work is completely unnecessary,
    // since we won't call the default constructor, and it mutates
    // information about the fields that we would rather stayed unmutated.
    return;

  } else if (initializerStyle == DEFINES_NONE_USE_DEFAULT) {
    // If neither a constructor nor an initializer has been defined for the
    // type, determine whether we should create a default constructor now or
    // create a default initializer later.
    if (!needsConstructor()) {
      return;
    }
  }

  // Don't use initializers or constructors for tuples.
  if (symbol->hasFlag(FLAG_TUPLE)) {
    return;
  }

  // Create the default constructor function symbol,
  FnSymbol* fn = new FnSymbol(astr("_construct_", symbol->name));

  fn->cname = fn->name;

  fn->addFlag(FLAG_DEFAULT_CONSTRUCTOR);
  fn->addFlag(FLAG_CONSTRUCTOR);
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_LAST_RESORT);
  fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);

  if (symbol->hasFlag(FLAG_REF) == true) {
    fn->addFlag(FLAG_REF);
  }

  // And insert it into the class type.
  defaultInitializer = fn;

  // Create "this".
  fn->_this = new VarSymbol("this", this);
  fn->_this->addFlag(FLAG_ARG_THIS);

  fn->insertAtTail(new DefExpr(fn->_this));

  // Walk the fields in the class type.
  std::map<VarSymbol*, ArgSymbol*> fieldArgMap;
  Vec<const char*>                 fieldNamesSet;

  for_fields(tmp, this) {
    SET_LINENO(tmp);

    if (VarSymbol* field = toVarSymbol(tmp)) {
      // Filter inherited fields and other special cases.
      // "outer" is used internally to supply a pointer to
      // the outer parent of a nested class.
      if (field->hasFlag(FLAG_SUPER_CLASS)      == false &&
          strcmp(field->name, "outer")          != 0) {
        // Create an argument to the default constructor
        // corresponding to the field.
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);

        fieldArgMap[field] = arg;

        fieldNamesSet.set_add(field->name);
      }
    }
  }

  ArgSymbol* meme      = NULL;
  CallExpr*  superCall = NULL;
  CallExpr*  allocCall = NULL;

  if (symbol->hasFlag(FLAG_REF) == true) {
    // For ref, sync and single classes, just allocate space.
    allocCall = callChplHereAlloc(fn->_this->type);

    fn->insertAtTail(new CallExpr(PRIM_MOVE, fn->_this, allocCall));

  } else if (symbol->hasFlag(FLAG_TUPLE) == false) {
    // Create a meme (whatever that is).
    meme = new ArgSymbol(INTENT_BLANK,
                         "meme",
                         this,
                         NULL,
                         new SymExpr(gTypeDefaultToken));

    meme->addFlag(FLAG_IS_MEME);

    // Move the meme into "this".
    fn->insertAtTail(new CallExpr(PRIM_MOVE, fn->_this, meme));

    if (isClass() == true) {
      if (dispatchParents.n > 0 && symbol->hasFlag(FLAG_EXTERN) == false) {
        // This class has a parent class.
        AggregateType* at = dispatchParents.v[0];

        if (at->defaultInitializer == NULL) {
          // If it doesn't yet have an initializer, make one.
          at->buildConstructors();

          // Error out if the parent type or one that it inherits from
          // defines an initializer - we should not be creating a default
          // constructor in that case, it won't know what to do with it.
          if (at->initializerStyle == DEFINES_INITIALIZER ||
              at->wantsDefaultInitializer() ||
              at->parentDefinesInitializer() == true) {
            // at->defaultInitializer will still be NULL
            USR_FATAL(this,
                      "Cannot create default constructor on type '%s', which "
                      "inherits from a type that defines an initializer",
                      symbol->name);
          }
        }

        // Get the parent constructor.
        // Note that since we only pay attention to the first entry in the
        // dispatchParents list, we are effectively implementing
        // single class inheritance, multiple interface inheritance.
        FnSymbol* superCtor = at->defaultInitializer;

        // Create a call to the superclass constructor.
        superCall = new CallExpr(superCtor->name);

        // Walk the formals of the default super class constructor
        for_formals_backward(formal, superCtor) {
          if (formal->hasFlag(FLAG_IS_MEME))
            continue;

          DefExpr* superArg = formal->defPoint->copy();

          // Omit the arguments shadowed by this class's fields.
          if (fieldNamesSet.set_in(superArg->sym->name))
            continue;

          fieldNamesSet.set_add(superArg->sym->name);

          // Inserting each successive ancestor argument at the head in
          // reverse-lexical order results in all of the arguments appearing
          // in lexical order, starting with those in the most ancient class
          // and ending with those in the most-derived class.
          fn->insertFormalAtHead(superArg);

          superCall->insertAtHead(superArg->sym);
        }

        // Create a temp variable and add it to the actual argument list
        // in the superclass constructor call.  This temp will hold
        // the pointer to the parent subobject.
        VarSymbol* tmp = newTemp();

        superCall->insertAtTail(new NamedExpr("meme", new SymExpr(tmp)));

        // Add super call to the constructor function.
        fn->insertAtTail(superCall);

        // Declare that variable in the scope of this constructor.
        // And initialize it with the super class pointer.
        superCall->insertBefore(new DefExpr(tmp));

        superCall->insertBefore(
          new CallExpr(PRIM_MOVE,
                       tmp,
                       new CallExpr(PRIM_GET_MEMBER_VALUE,
                                    fn->_this,
                                    new_CStringSymbol("super"))));
      }
    }
  }

  if (isUnion() == true) {
    fn->insertAtTail(new CallExpr(PRIM_SET_UNION_ID,
                                  fn->_this,
                                  new_IntSymbol(0)));
  }

  symbol->defPoint->insertBefore(new DefExpr(fn));

  for_fields(tmp, this) {
    VarSymbol* field = toVarSymbol(tmp);

    if (field == NULL) {
      continue;
    }

    if (fieldArgMap.count(field) == 0) {
      continue;
    }

    ArgSymbol* arg = fieldArgMap[field];

    SET_LINENO(field);

    if (field->hasFlag(FLAG_PARAM) == true) {
      arg->intent = INTENT_PARAM;
    }

    Expr* exprType = field->defPoint->exprType;
    Expr* init     = field->defPoint->init;

    bool  hadType  = exprType;
    bool  hadInit  = init;

    if (exprType != NULL) {
      exprType->remove();
    }

    if (init != NULL) {
      init->remove();
    }

    if (init != NULL) {
      if (field->isType() ==  false && exprType == NULL) {
        // init && !exprType
        VarSymbol* tmp = newTemp();

        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        tmp->addFlag(FLAG_MAYBE_PARAM);
        tmp->addFlag(FLAG_MAYBE_TYPE);

        exprType = new BlockStmt(new DefExpr(tmp), BLOCK_TYPE);

        toBlockStmt(exprType)->insertAtTail(
                               new CallExpr(PRIM_MOVE,
                                            tmp,
                                            new CallExpr("chpl__initCopy",
                                                         init->copy())));

        toBlockStmt(exprType)->insertAtTail(new CallExpr(PRIM_TYPEOF, tmp));
      }

    } else if (hadType                    == true  &&
               field->isType()            == false &&
               field->hasFlag(FLAG_PARAM) == false) {
      init = new CallExpr(PRIM_INIT, exprType->copy());
    }


    if (field->isType() == false && field->hasFlag(FLAG_PARAM) == false) {
      if (hadType == true) {
        init = new CallExpr("_createFieldDefault", exprType->copy(), init);

      } else if (init != NULL)
        init = new CallExpr("chpl__initCopy", init);
    }

    if (exprType != NULL) {
      if (isBlockStmt(exprType) == false)
        arg->typeExpr = new BlockStmt(exprType, BLOCK_TYPE);
      else
        arg->typeExpr = toBlockStmt(exprType);
    }

    if (init != NULL) {
      if (hadInit == true)
        arg->defaultExpr = new BlockStmt(init, BLOCK_SCOPELESS);
      else {
        Expr* initVal = new SymExpr(gTypeDefaultToken);

        arg->defaultExpr = new BlockStmt(initVal);
      }
    }

    if (field->isType() == true) {
      // Args with this flag are removed after resolution.
      // Note that in the default type constructor, this flag is also applied
      // (along with FLAG_GENERIC) to arguments whose type is unknown,
      // but would not be pruned in resolution.
      arg->addFlag(FLAG_TYPE_VARIABLE);
    }

    if (exprType == NULL && arg->type == dtUnknown) {
      arg->type = dtAny;
    }

    fn->insertFormalAtTail(arg);

    if (arg->type                        == dtAny &&
        arg->hasFlag(FLAG_TYPE_VARIABLE) == false &&
        arg->hasFlag(FLAG_PARAM)         == false &&
        symbol->hasFlag(FLAG_REF)        == false) {
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                    fn->_this,
                                    new_CStringSymbol(arg->name),
                                    new CallExpr("chpl__initCopy", arg)));

    } else {
      // Since we don't copy the argument before stuffing it in a field,
      // we will have to remove the autodestroy flag for specific cases.
      // Namely, if the function is a default constructor and the target
      // of a PRIM_SET_MEMBER is a record, then the INSERT_AUTO_DESTROY
      // flag must be removed.
      // (See NOTE 1 in callDestructors.cpp.)
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                    fn->_this,
                                    new_CStringSymbol(arg->name),
                                    arg));
    }
  }

  if (meme != NULL) {
    fn->insertFormalAtTail(meme);
  }

  AggregateType::insertImplicitThis(fn, fieldNamesSet);

  Symbol*        parSym    = symbol->defPoint->parentSymbol;
  AggregateType* outerType = toAggregateType(parSym->type);

  if (outerType != NULL) {
    outerType->moveConstructorToOuter(fn);

    // Save the pointer to the outer class
    fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                  fn->_this,
                                  new_CStringSymbol("outer"),
                                  fn->_outer));
  }

  //
  // Insert a call to the "initialize()" method if one is defined.
  // The return value of this method (if any) is ignored.
  //
  forv_Vec(FnSymbol, method, methods) {
    // Select a method named "initialize" and taking no arguments
    // (aside from _mt and the implicit 'this').
    if (method && !strcmp(method->name, "initialize")) {
      if (method->numFormals() == 2) {
        CallExpr* init = new CallExpr("initialize", gMethodToken, fn->_this);

        fn->insertAtTail(init);

        // If a record type has an initialize method, it's not Plain Old Data.
        if (isClass() == false) {
          symbol->addFlag(FLAG_NOT_POD);
        }

        break;
      }
    }
  }

  fn->insertAtTail(new CallExpr(PRIM_RETURN, fn->_this));

  addToSymbolTable(fn);
}

void AggregateType::buildDefaultInitializer() {
  if (defaultInitializer                       == NULL ||
      strcmp(defaultInitializer->name, "init") !=    0) {
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

      defaultInitializer = fn;

      symbol->defPoint->insertBefore(def);

      fn->setMethod(true);
      fn->addFlag(FLAG_METHOD_PRIMARY);

      preNormalizeInitMethod(fn);
      normalize(fn);

      methods.add(fn);
    } else {
      fieldToArg(fn, names, fieldArgMap);
    }

  }
}

void AggregateType::fieldToArg(FnSymbol*              fn,
                               std::set<const char*>& names,
                               SymbolMap&             fieldArgMap) {
  for_fields(fieldDefExpr, this) {
    SET_LINENO(fieldDefExpr);

    if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
      if (field->hasFlag(FLAG_SUPER_CLASS) == false &&
             strcmp(field->name, "outer")) {

        DefExpr*    defPoint = field->defPoint;
        const char* name     = field->name;
        ArgSymbol*  arg      = new ArgSymbol(INTENT_BLANK, name, dtUnknown);

        names.insert(name);
        fieldArgMap.put(field, arg);

        // Insert initialization for each field from the argument provided.
        SET_LINENO(field);

        if (field->hasFlag(FLAG_PARAM) == true) {
          arg->intent = INTENT_PARAM;
        }

        if (field->isType() == true) {
          arg->addFlag(FLAG_TYPE_VARIABLE);
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
          VarSymbol* tmp      = newTemp();
          BlockStmt* typeExpr = new BlockStmt(new DefExpr(tmp), BLOCK_TYPE);

          // tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
          // Lydia NOTE 06/16/17: The default constructor adds this flag
          // to its equivalent temporary.  I have decided not to do so
          // and am not seeing issues so far, but may have missed something,
          // so I am leaving it here just in case.

          tmp->addFlag(FLAG_MAYBE_TYPE);
          tmp->addFlag(FLAG_MAYBE_PARAM);

          typeExpr->insertAtTail(new CallExpr(PRIM_MOVE,
                                              tmp,
                                              defPoint->init->copy()));

          // Lydia NOTE 06/16/17: I believe we don't need to make an
          // initCopy call for the field's init (like the default
          // constructor version attempts).
          // I might have missed something, though, so if it turns out we
          // do need that initCopy, use this instead of the above statement:
          // typeExpr->insertAtTail(
          //           new CallExpr(PRIM_MOVE,
          //                        tmp,
          //                        new CallExpr("chpl__initCopy",
          //                                     defPoint->init->copy())));

          typeExpr->insertAtTail(new CallExpr(PRIM_TYPEOF, tmp));

          arg->typeExpr = typeExpr;
          if (arg->hasFlag(FLAG_TYPE_VARIABLE)) {
            arg->type = dtAny;
          }

          // set up the ArgSymbol appropriately for the type
          // and initialization from the field declaration.
          arg->defaultExpr = new BlockStmt(defPoint->init->copy());


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
      if (parent->initializerStyle != DEFINES_CONSTRUCTOR) {
        CallExpr* superPortion = new CallExpr(".",
                                              new SymExpr(fn->_this),
                                              new_CStringSymbol("super"));

        SymExpr*  initPortion  = new SymExpr(new_CStringSymbol("init"));
        CallExpr* base         = new CallExpr(".", superPortion, initPortion);
        CallExpr* superCall    = new CallExpr(base);

        if (parent->initializerStyle == DEFINES_NONE_USE_DEFAULT) {
          // We want to call the compiler-generated all-fields initializer

          // First, ensure we have a default initializer for the parent
          if (parent->defaultInitializer == NULL) {
            // ... but only if it is valid to do so
            if (parent->wantsDefaultInitializer() == true) {
              parent->buildDefaultInitializer();
            }
          }

          if (parent->defaultInitializer == NULL) {
            // The parent might have inherited from a class that defines
            // any initializer but not one without arguments.
            // In this case, we shouldn't define a default initializer
            // for this class either.
            retval = false;

          } else {
            // Otherwise, we are good to go!

            // Add an argument per argument in the parent initializer
            for_formals(formal, parent->defaultInitializer) {
              if (formal->type                   == dtMethodToken ||
                  formal->hasFlag(FLAG_ARG_THIS) == true          ||
                  formal->hasFlag(FLAG_IS_MEME)  == true) {

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

        } else {
          INT_ASSERT(parent->initializerStyle == DEFINES_INITIALIZER);

          // We want to call a user-defined no-argument initializer.
          // Insert no arguments
        }

        fn->body->insertAtHead(superCall);

      } else {
        USR_FATAL(this,
                  "Cannot create default initializer on type '%s', "
                  "which inherits from type '%s' that defines a constructor",
                  symbol->name,
                  parent->symbol->name);

        // The parent has defined a constructor, we cannot have a
        // default initializer call that constructor via super.init();
        retval = false;
      }
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
    fn->addFlag(FLAG_DEFAULT_COPY_INIT);

    _this->addFlag(FLAG_ARG_THIS);

    // Detect if the type has at least one generic field,
    // so we should mark the "other" arg as generic.
    for_fields(fieldDefExpr, this) {
      if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
        if (field->hasFlag(FLAG_SUPER_CLASS) == false &&
            strcmp(field->name, "outer")     != 0) {
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

    // Copy the fields from "other" into our fields
    for_fields(fieldDefExpr, this) {
      // TODO: outer (nested types), promotion type?
      if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
        if (field->hasFlag(FLAG_SUPER_CLASS) == false &&
            strcmp(field->name, "outer")     != 0) {
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

    symbol->defPoint->insertBefore(def);

    fn->setMethod(true);
    fn->addFlag(FLAG_METHOD_PRIMARY);

    preNormalizeInitMethod(fn);
    normalize(fn);

    methods.add(fn);
  }
}

// Returns false if we should not generate a default constructor for this
// AggregateType, true if we still require one.  The result of this function
// will vary in most cases if --force-initializers is thrown: that flag tells
// us to only generate default constructors for types that already have defined
// constructors (as the constructor implementation relies on every user
// constructor being modified to call the default constructor), and to try to
// generate default initializers for types where neither an initializer nor a
// constructor has been defined.
bool AggregateType::needsConstructor() {
  // Temporarily only generate default initializers for classes and records
  if (isUnion())
    return true;

  // We don't want a default constructor if the type has been explicitly marked
  if (symbol->hasFlag(FLAG_USE_DEFAULT_INIT))
    return false;

  if (hasPostInitializer() == true) {
    return false;
  }

  ModuleSymbol* mod = getModule();

  // For now, always generate a default constructor for types in the internal
  // and library modules
  if (mod && (mod->modTag == MOD_INTERNAL || mod->modTag == MOD_STANDARD))
    return true;
  else if (fUserDefaultInitializers)
    // Don't generate a default constructor when --force-initializers is true,
    // we want to generate a default initializer or fail.
    return false;

  if (initializerStyle == DEFINES_INITIALIZER) {
    // Defining an initializer means we don't need a default constructor
    return false;
  } else if (initializerStyle == DEFINES_CONSTRUCTOR) {
    // Defining a constructor means we need a default constructor
    return true;
  } else {
    // The above two branches are only relevant in the recursive version
    // of this call, as the outside call site for this function has
    // already ensured that the type which is the entry point has defined
    // neither an initializer nor a constructor.

    // Classes that define an initialize() method need a default constructor
    forv_Vec(FnSymbol, method, methods) {
      if (method && strcmp(method->name, "initialize") == 0) {
        if (method->numFormals() == 2) {
          return true;
        }
      }
    }

    // Make a default constructor for extern classes only if we are not forcing
    // initializers
    if (symbol->hasFlag(FLAG_EXTERN) && !fUserDefaultInitializers) {
      return true;
    }

    // If the parent type needs a default constructor, we need a default
    // constructor.
    if (dispatchParents.n > 0) {
      if (AggregateType* pt = dispatchParents.v[0]) {
        return pt->needsConstructor();
      }
    }
  }

  // Otherwise, we need a default constructor.
  return true;
}

bool AggregateType::parentDefinesInitializer() const {
  bool retval = false;

  if (dispatchParents.n > 0) {
    if (AggregateType* pt = dispatchParents.v[0]) {
      if (pt->initializerStyle == DEFINES_INITIALIZER ||
          pt->wantsDefaultInitializer()) {
        retval = true;

      } else {
        retval = pt->parentDefinesInitializer();
      }
    }
  }

  return retval;
}

// Returns true for the cases where we want to generate a default initializer.
// Some cases are temporarily false, while others are permanently so: we never
// want to generate a default initializer for a type that has defined an
// explicit initializer or constructor, and we don't want to generate a default
// initializer if --force-initializers has not been thrown (currently), unless
// the pragma "use default init" has been applied
//
// Note that this method does not generate the opposite of needsConstructor -
// when the type has defined an initializer both methods will return false.
bool AggregateType::wantsDefaultInitializer() const {
  AggregateType* nonConstHole = (AggregateType*) this;
  ModuleSymbol*  mod          = nonConstHole->getModule();
  bool           retval       = true;

  // We want a default initializer if the type has been explicitly marked
  if (symbol->hasFlag(FLAG_USE_DEFAULT_INIT) == true) {
    retval = true;

  // For now, no default initializers for library and internal types
  } else if (mod         == NULL         ||
             mod->modTag == MOD_INTERNAL ||
             mod->modTag == MOD_STANDARD) {
    retval = false;

  // No default initializers if the --force-initializers flag is not used
  } else if (fUserDefaultInitializers == false) {
    retval = false;

  // Only want a default initializer when no
  // initializer or constructor is defined
  } else if (initializerStyle != DEFINES_NONE_USE_DEFAULT) {
    retval = false;

  // For now, no default initializers for unions
  } else if (isUnion()  == true) {
    retval = false;

  } else if (symbol->hasFlag(FLAG_REF) == true) {
    retval = false;

  // Iterator classes and records want neither default constructors nor
  // initializers, and never will
  } else if (symbol->hasFlag(FLAG_ITERATOR_CLASS) ||
             symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    retval = false;

  } else {
    // No default initializer for types that have an initialize() method
    forv_Vec(FnSymbol, method, nonConstHole->methods) {
      if (method != NULL && strcmp(method->name, "initialize") == 0) {
        if (method->numFormals() == 2) {
          retval = false;
          break;
        }
      }
    }
  }

  return retval;
}

// Replace implicit references to 'this' in the body of this
// type constructor with explicit member reference (dot) expressions.
void AggregateType::insertImplicitThis(FnSymbol*         fn,
                                       Vec<const char*>& fieldNamesSet) {
  std::vector<BaseAST*> asts;

  collect_asts(fn->body, asts);

  for_vector(BaseAST, ast, asts) {
    if (UnresolvedSymExpr* se = toUnresolvedSymExpr(ast)) {
      if (fieldNamesSet.set_in(se->unresolved)) {
        // The name of this UnresolvedSymExpr matches a field name.
        // So replace it with a dot expression.
        se->replace(buildDotExpr(fn->_this, se->unresolved));
      }
    }
  }
}

ArgSymbol* AggregateType::moveConstructorToOuter(FnSymbol* fn) {
  Expr*      insertPoint = symbol->defPoint;
  ArgSymbol* _mt         = new ArgSymbol(INTENT_BLANK, "_mt",   dtMethodToken);
  ArgSymbol* retval      = new ArgSymbol(INTENT_BLANK, "outer", this);

  methods.add(fn);

  retval->addFlag(FLAG_GENERIC);

  fn->_outer = retval;

  fn->insertFormalAtHead(new DefExpr(retval));
  fn->insertFormalAtHead(new DefExpr(_mt));

  fn->setMethod(true);
  fn->addFlag(FLAG_METHOD_PRIMARY);

  while (isTypeSymbol(insertPoint->parentSymbol) == true) {
    insertPoint = insertPoint->parentSymbol->defPoint;
  }

  insertPoint->insertBefore(fn->defPoint->remove());

  return retval;
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
  UnresolvedSymExpr* se  = toUnresolvedSymExpr(storesName);
  Symbol*            sym = lookup(se->unresolved, storesName);
  TypeSymbol*        ts  = toTypeSymbol(sym);

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
  bool isCtor = (strcmp(t->name,  fn->name) == 0);
  bool isInit = (strcmp(fn->name, "init")   == 0);

  isCtor = isCtor || (strcmp(fn->name, "initialize") == 0);

  if (isCtor == true || isInit == true) {
    AggregateType* ct = toAggregateType(t->type);

    if (ct == NULL) {
      INT_FATAL(fn, "initializer on non-class type");
    }

    if (fn->hasFlag(FLAG_NO_PARENS)) {
      USR_FATAL(fn,
                "a%s cannot be declared without parentheses",
                isCtor ? " constructor" : "n initializer");
    }

    if (ct->symbol->hasFlag(FLAG_USE_DEFAULT_INIT)) {
      USR_FATAL_CONT(fn, "cannot apply 'use default init' to type '%s', it"
                     " defines a%s here", ct->symbol->name,
                     isCtor ? " constructor" : "n initializer");
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

    if (ct->initializerStyle == DEFINES_NONE_USE_DEFAULT) {
      // We hadn't previously seen a constructor or initializer definition.
      // Update the field on the type appropriately.
      if (isInit) {
        if (fn->hasFlag(FLAG_METHOD_PRIMARY) == true ||
            fn->getModule() == t->getModule()) {
          // Only mark the type as defining an initializer if the initializer
          // we found was in the same module as the type itself.  If there is
          // no such initializer, we would need to define a default constructor
          // or initializer for the scopes where the secondary initializer is
          // not visible.
          ct->initializerStyle = DEFINES_INITIALIZER;
        }

      } else if (isCtor) {
        ct->initializerStyle = DEFINES_CONSTRUCTOR;

      } else {
        // Should never reach here, but just in case...
        INT_FATAL(fn, "Function was neither a constructor nor an initializer");
      }

    } else if ((ct->initializerStyle == DEFINES_CONSTRUCTOR && !isCtor) ||
               (ct->initializerStyle == DEFINES_INITIALIZER && !isInit)) {
      // We've previously seen a constructor but this new method
      // is an initializer or we've previously seen an initializer
      // but this new method is a constructor.
      // We don't allow both to be defined on a type.

      USR_FATAL_CONT(fn,
                     "Definition of both constructor '%s' and "
                     "initializer 'init'.  Please choose one.",
                     ct->symbol->name);
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
    // The generated code should just use the canonical class name
    tsUnmanaged->cname = at->symbol->cname;
    DefExpr* defUnmanaged = new DefExpr(tsUnmanaged);
    at->symbol->defPoint->insertAfter(defUnmanaged);
  }
}
