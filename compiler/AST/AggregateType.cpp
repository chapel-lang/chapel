/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include "DecoratedClassType.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "initializerRules.h"
#include "iterator.h"
#include "LoopExpr.h"
#include "passes.h"
#include "resolution.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "visibleFunctions.h"
#include "wellknown.h"
#include "../ifa/prim_data.h"

#include <queue>

AggregateType* dtObject = NULL;
AggregateType* dtBytes  = NULL;
AggregateType* dtString = NULL;
AggregateType* dtLocale = NULL;
AggregateType* dtOwned  = NULL;
AggregateType* dtShared = NULL;

AggregateType::AggregateType(AggregateTag initTag) :
  Type(E_AggregateType, NULL) {

  aggregateTag        = initTag;
  memset(decoratedClasses, 0, sizeof(decoratedClasses));

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
  foundGenericFields = false;
  typeSignature      = NULL;


  classId             = 0;

  resolveStatus       = UNRESOLVED;

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

  if (dispatchParents.n > 0) {
    // Copy list of parent's uninstantiated fields
    std::vector<Symbol*>& parentGenerics = dispatchParents.v[0]->genericFields;
    copy_type->genericFields.insert(copy_type->genericFields.end(), parentGenerics.begin(), parentGenerics.end());
  }
  for_vector(Symbol, field, genericFields) {
    // Append to list corresponding generic fields in the copy
    if (toAggregateType(field->defPoint->parentSymbol->type)->getRootInstantiation() == getRootInstantiation()) {
      copy_type->genericFields.push_back(copy_type->getField(field->name));
    }
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
  symbol->addFlag(FLAG_GENERIC);
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

struct DecoratorTypePair {
  ClassTypeDecorator d;
  Type* t;
  DecoratorTypePair(ClassTypeDecorator d, Type* t) : d(d), t(t) { }
};

// Inspects a type expression and returns (class decorator, class type).
// For non-class types, returns (CLASS_TYPE_UNMANAGED_NILABLE, NULL)
static DecoratorTypePair getTypeExprDecorator(Expr* e) {
  if (SymExpr* se = toSymExpr(e))
    if (TypeSymbol* ts = toTypeSymbol(se->symbol()))
      if (isClassLikeOrManaged(ts->type))
        return DecoratorTypePair(classTypeDecorator(ts->type),
                                 canonicalClassType(ts->type));

  if (CallExpr* call = toCallExpr(e)) {
    if (isClassDecoratorPrimitive(call) && call->numActuals() >= 1) {
      DecoratorTypePair p = getTypeExprDecorator(call->get(1));
      ClassTypeDecorator d = p.d;
      if (call->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
          call->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED)) {
        if (isDecoratorNonNilable(d))
          d = CLASS_TYPE_UNMANAGED_NONNIL;
        else if (isDecoratorNilable(d))
          d = CLASS_TYPE_UNMANAGED_NILABLE;
        else
          d = CLASS_TYPE_UNMANAGED;
      } else if (call->isPrimitive(PRIM_TO_BORROWED_CLASS) ||
                 call->isPrimitive(PRIM_TO_BORROWED_CLASS_CHECKED)) {
        if (isDecoratorNonNilable(d))
          d = CLASS_TYPE_BORROWED_NONNIL;
        else if (isDecoratorNilable(d))
          d = CLASS_TYPE_BORROWED_NILABLE;
        else
          d = CLASS_TYPE_BORROWED;
      } else if (call->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
                 call->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED)) {
        d = addNilableToDecorator(d);
      } else if (call->isPrimitive(PRIM_TO_NON_NILABLE_CLASS)) {
        d = addNonNilToDecorator(d);
      } else {
        INT_FATAL("Case not handled");
      }
      p.d = d;
      return p;
    }
  }

  return DecoratorTypePair(CLASS_TYPE_UNMANAGED_NILABLE, NULL);
}

// Note that a field with generic type where that type has
// default values for all of its generic fields is considered concrete
// for the purposes of this function.
static bool isFieldTypeExprGeneric(Expr* typeExpr) {
  // Look in the field declaration for a concrete type
  Symbol* sym = NULL;

  DecoratorTypePair pair = getTypeExprDecorator(typeExpr);
  if (pair.t != NULL) {
    sym = pair.t->symbol;
    if (isDecoratorUnknownManagement(pair.d) ||
        isDecoratorUnknownNilability(pair.d))
      return true;
  }

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

bool AggregateType::fieldIsGeneric(Symbol* field, bool &hasDefault) {
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
      } else if (def->init == NULL && def->exprType != NULL &&
                 !mIsGenericWithDefaults) {

        // Temporarily mark the aggregate type as generic with defaults
        // in order to avoid infinite recursion.
        bool wasGenericWithDefaults = mIsGenericWithDefaults;
        mIsGenericWithDefaults = true;
        if (isFieldTypeExprGeneric(def->exprType)) {
          retval = true;
        }
        mIsGenericWithDefaults = wasGenericWithDefaults;
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

  } else if (isEndOfStatementMarker(expr)) {
    // drop it
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
      markAsGeneric();

    } else if (var->hasFlag(FLAG_PARAM) == true) {
      markAsGeneric();

    } else if (defExpr->exprType == NULL &&
               defExpr->init     == NULL) {
      markAsGeneric();
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

      if (UnresolvedSymExpr* sym  = toUnresolvedSymExpr(firstexpr))
        // ... then report it to the user
        USR_FATAL_CONT(fn->_this,
                     "Type binding clauses ('%s.' in this case) are not "
                     "supported in declarations within a class, record "
                     "or union",
                     sym->unresolved);
      else
        // got more than just a name
        USR_FATAL_CONT(fn->_this,
                     "Type binding clauses (in this case, the parenthesized "
                "expression preceding the dot before function name) are not "
                     "supported in declarations within a class, record "
                     "or union");

    } else {
      ArgSymbol* arg = new ArgSymbol(fn->thisTag, "this", this);

      if (fn->name == astrInitEquals) {
        if (fn->numFormals() != 1) {
          USR_FATAL_CONT(fn, "%s.init= must have exactly one argument",
                         this->name());
        }
      }

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
      if (methods.v[i] != NULL)
        retval = methods.v[i]->isPostInitializer();
    }

  } else {
    retval = instantiatedFrom->hasPostInitializer();
  }

  return retval;
}

bool AggregateType::hasUserDefinedInitEquals() const {
  bool retval = false;

  if (instantiatedFrom == NULL) {
    for (int i = 0; i < methods.n && retval == false; i++) {
      FnSymbol* method = methods.v[i];
      if (method &&
          method->isCopyInit() &&
          method->hasFlag(FLAG_COMPILER_GENERATED) == false) {
        retval = true;
      }
    }
  } else {
    retval = instantiatedFrom->hasUserDefinedInitEquals();
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

  if (resolveStatus == RESOLVED) {
    retval = true;
  } else if (instantiatedFrom != NULL && symbol->hasFlag(FLAG_GENERIC) == false) {
    retval = true;

  } else {
    retval = initializerResolved;
  }

  return retval;
}

// Determine the index for the first generic field (if present).
// Return true if a generic field was found.
// Note: this method will be invoked on each parent/ancestor for class types
bool AggregateType::setFirstGenericField() {
  if (genericField == 0) {
    if (setNextGenericField() == true) {
      symbol->addFlag(FLAG_GENERIC);
    }

    if (isClass() == true && symbol->hasFlag(FLAG_NO_OBJECT) == false) {
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
* where the type-expression is represented as a call to 'MyGenericType'       *
* itself, along with four actuals. There is not a corresponding FnSymbol for  *
* this call. Instead, the ``AggregateType::generateType`` method handles      *
* instantiation of the specified type. The arguments will correspond to each  *
* generic field in the type and the result will be a concrete type            *
* parameterized by these arguments.                                           *
*                                                                             *
* This call is associated with a generic type that is one of                  *
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
* The resolution process handles such calls by creating a SymbolMap mapping   *
* the fields of the specified type to the actuals. If the generic type is     *
* a class with a generic parent, directly or indirectly, then this            *
* dictionary must be passed up the hierarchy so that the parent types can     *
* be instantiated.                                                            *
*                                                                             *
************************************** | *************************************/

static Type* resolveFieldTypeForInstantiation(Symbol* field, CallExpr* call, const char* callString);

static void checkNumArgsErrors(AggregateType* at, CallExpr* call, const char* callString) {
  std::vector<Symbol*>& genericFields = at->genericFields;
  TypeSymbol* symbol                  = at->symbol;
  const char* typeSignature           = at->typeSignature;

  if (genericFields.size() == 0) {
    if (call->numActuals() > 0) {
      USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
      USR_PRINT(at, "type '%s' is not generic", symbol->name);
      USR_PRINT(call, "did you forget the 'new' keyword?");
      USR_STOP();
    }
  } else if (symbol->hasFlag(FLAG_GENERIC) == false &&
             at->instantiatedFrom !=NULL &&
             call->numActuals() > 0) {
    USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
    USR_PRINT(call, "type '%s' cannot be instantiated further", symbol->name);
    USR_PRINT(call, "did you forget the 'new' keyword?");
    USR_STOP();
  }

  unsigned int numWithoutDefaults = 0;
  for_vector(Symbol, sym, genericFields) {
    if (sym->defPoint->init == NULL) {
      numWithoutDefaults += 1;
    }
  }

  unsigned int numArgs = call->numActuals();
  if (numArgs > genericFields.size()) {
    USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
    USR_PRINT(call, "type specifier did not match: %s", typeSignature);
    USR_PRINT(call, "type was specified with %d arguments", numArgs);
    const char* plural = genericFields.size() > 1 ? "fields" : "field";
    USR_PRINT(at, "but type '%s' only has %d generic %s", symbol->name, genericFields.size(), plural);
    USR_STOP();
  }
}

static void resolveConcreteFields(AggregateType* ret, CallExpr* call, const char* callString) {
  if (ret->resolveStatus != RESOLVED) {
    ret->resolveStatus = RESOLVED;
    // TODO: How to handle cases where generic fields lean on non-generic
    // fields for type/init-expressions:
    //   class C {
    //     type T;
    //     param x : int;
    //
    //     var next : C(T, x);
    //
    //     param flag : bool = if next.type.T == int then true else false;
    //   }
    //
    // In this example, the current implementation fails to resolve the type
    // of field 'flag' because 'next' is not yet resolved. This particular
    // example is difficult to resolve because the field 'next' requires
    // recursive resolution of the type we're already trying to resolve. This
    // difficulty has lead to the current implementation which resolves
    // concrete fields after generic fields are resolved.
    //

    // TODO: Unfortunate workaround for the existing infrastructure. We need
    // to keep types marked as generic if their type fields are generic.
    for_fields(field, ret) {
      if (field->hasFlag(FLAG_TYPE_VARIABLE) && field->type->symbol->hasFlag(FLAG_GENERIC)) {
        ret->symbol->addFlag(FLAG_GENERIC);
        break;
      }
    }

    // Resolve the remaining non-generic fields
    if (ret->symbol->hasFlag(FLAG_GENERIC) == false) {

      makeRefType(ret);

      for_fields(field, ret) {
        if (field->hasFlag(FLAG_PARAM) == false &&
            field->hasFlag(FLAG_TYPE_VARIABLE) == false &&
            field->type == dtUnknown) {
          if (Type* type = resolveFieldTypeForInstantiation(field, call, callString)) {
            field->type = type;
          }
        }
      }
    }
  }
}

AggregateType* AggregateType::generateType(CallExpr* call, const char* callString) {

  checkNumArgsErrors(this, call, callString);

  if (call->numActuals() == 0 && mIsGenericWithDefaults == false) {
    // We do this to support cases where we just want to indicate the generic
    // type, e.g. a field 'var x : owned;'
    return this;
  }

  AggregateType* ret = this;

  bool evalDefaults = true;

  // Separate named and positional args, storing named-exprs in a map
  SymbolMap map;
  std::queue<Symbol*> notNamed;
  for (int i = 1; i <= call->numActuals(); i++) {
    Expr* actual = call->get(i);
    if (NamedExpr* ne = toNamedExpr(actual)) {
      Symbol* field = getField(ne->name, false);
      if (field == NULL) {
        USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
        USR_PRINT(call, "type specifier did not match: %s", typeSignature);
        USR_PRINT(call, "type '%s' does not contain a field named '%s'", symbol->name, ne->name);
        USR_STOP();
      }
      map.put(field, toSymExpr(ne->actual)->symbol());
    } else {
      SymExpr* se = toSymExpr(actual);
      if (se->symbol() == gUninstantiated) {
        evalDefaults = false;
      } else {
        notNamed.push(toSymExpr(actual)->symbol());
      }
    }
  }

  // place positional args in a map based on remaining unspecified fields
  for_vector(Symbol, field, genericFields) {
    if (substitutionForField(field, map) == NULL && notNamed.size() > 0) {
      map.put(field, notNamed.front());
      notNamed.pop();
    }
  }

  INT_ASSERT(notNamed.size() == 0);

  ret = ret->generateType(map, call, callString, evalDefaults, getInstantiationPoint(call));

  if (ret != this) {
    ret->instantiatedFrom = this;

    resolveConcreteFields(ret, call, callString);
  }

  return ret;
}

static Expr* resolveFieldExpr(Expr* expr, bool addCopy) {
  if (isBlockStmt(expr) == false) {
    BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);
    expr->replace(block);
    if (isSymExpr(expr) && toSymExpr(expr)->symbol()->hasFlag(FLAG_TYPE_VARIABLE) &&
        expr->typeInfo()->symbol->hasFlag(FLAG_GENERIC) &&
        isPrimitiveType(expr->typeInfo()) == false) {
      block->insertAtTail(new CallExpr(expr->typeInfo()->symbol));
    } else {
      block->insertAtTail(expr);
    }
    normalize(block);
    expr = block;
    if (CallExpr* last = toCallExpr(block->body.tail)) {
      VarSymbol* tmp = newTemp("field_result_tmp");
      tmp->addFlag(FLAG_MAYBE_PARAM);
      tmp->addFlag(FLAG_MAYBE_TYPE);
      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, last->remove()));
      if (addCopy) {
        VarSymbol* copyTmp = newTemp();
        copyTmp->addFlag(FLAG_MAYBE_PARAM);
        copyTmp->addFlag(FLAG_MAYBE_TYPE);
        block->insertAtTail(new DefExpr(copyTmp));
        block->insertAtTail(new CallExpr(PRIM_INIT_VAR, copyTmp, tmp));
        block->insertAtTail(new SymExpr(copyTmp));
      } else {
        block->insertAtTail(new SymExpr(tmp));
      }
    }
  } else {
    // If the field's type expression is already a BlockStmt, then some
    // recursive case was not handled correctly.
    INT_ASSERT(false);
  }

  BlockStmt* block = toBlockStmt(expr);
  resolveBlockStmt(block);

  Expr* tail = block->body.tail;
  block->replace(tail->remove());

  return tail;
}

//
// Issue an error if fields used in 'expr' are generic.
//
static void checkValidPartial(Expr* expr, Expr* errExpr, const char* errTypeString) {
  std::vector<SymExpr*> ses;
  collectSymExprs(expr, ses);
  std::set<Symbol*> syms;
  for_vector(SymExpr, se, ses) {
    syms.insert(se->symbol());
  }

  DefExpr* def = toDefExpr(expr->parentExpr);

  std::string fields;
  bool error = false;
  bool first = true;
  for_set(Symbol, sym, syms) {
    if (isTypeSymbol(sym->defPoint->parentSymbol) && sym->defPoint->parentExpr == NULL) {
      if (sym->type == dtUnknown || sym->type->symbol->hasFlag(FLAG_GENERIC)) {
        error = true;
        if (first) {
          first = false;
        } else {
          fields += ", ";
        }

        fields += "'";
        if (sym->hasFlag(FLAG_TYPE_VARIABLE)) {
          fields += "type ";
        } else if (sym->hasFlag(FLAG_PARAM)) {
          fields += "param ";
        } else if (sym->hasFlag(FLAG_CONST)) {
          fields += "const ";
        } else {
          fields += "var ";
        }
        fields += sym->name;
        fields += "'";
      }
    }
  }

  if (error) {
    USR_FATAL_CONT(errExpr, "Unable to resolve partial instantiation '%s'", errTypeString);
    USR_PRINT(errExpr, "Instantiation of field '%s' depends on uninstantiated fields: %s", def->sym->name, fields.c_str());
    USR_STOP();
  }
}

static Type* resolveFieldTypeExpr(Symbol* field, CallExpr* call, const char* callString) {
  Type* ret = NULL;
  Expr* expr = field->defPoint->exprType;

  Expr* errExpr = NULL;
  const char* errTypeString = NULL;
  if (call == NULL) {
    Symbol* ts = field->defPoint->parentSymbol;
    errExpr = ts->defPoint;
    errTypeString = ts->name;
  } else {
    errExpr = call;
    errTypeString = callString;
  }

  if (expr != NULL) {

    checkValidPartial(expr, errExpr, errTypeString);
    Expr* tail = resolveFieldExpr(expr, false);

    if (SymExpr* se = toSymExpr(tail)) {
      if (isTypeSymbol(se->symbol()) == false && se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        USR_FATAL_CONT(errExpr, "error while resolving type '%s'", errTypeString);
        if (se->symbol()->isImmediate()) {
          USR_PRINT(expr, "type expression of field '%s' resolves to a 'param' value, not a type", field->name);
        } else {
          USR_PRINT(expr, "type expression of field '%s' resolves to a value, not a type", field->name);
        }
        USR_STOP();
      } else {
        if (se->typeInfo() == dtUnknown) {
          ret = resolveDefaultGenericTypeSymExpr(se);
        } else {
          ret = se->typeInfo();
        }
      }
    } else {
      INT_FATAL("unexpected AST in field expr");
    }
  }

  if (ret != NULL) {
    // check that it's not dtUnknown
    if (ret == dtUnknown) {
      USR_FATAL_CONT(errExpr, "error while resolving type '%s'", errTypeString);
      USR_PRINT(expr, "unable to resolve type of field '%s'", field->name);
      USR_STOP();
    }
  }

  return ret;
}

static Symbol* resolveFieldDefault(Symbol* field, CallExpr* call, const char* callString) {
  Symbol* ret = NULL;

  Expr* expr = field->defPoint->init;

  Expr* errExpr = NULL;
  const char* errTypeString = NULL;
  if (call == NULL) {
    Symbol* ts = field->defPoint->parentSymbol;
    errExpr = ts->defPoint;
    errTypeString = ts->name;
  } else {
    errExpr = call;
    errTypeString = callString;
  }

  if (expr != NULL) {
    bool needsCopy = field->hasFlag(FLAG_PARAM) == false &&
                     field->hasFlag(FLAG_TYPE_VARIABLE) == false;
    Expr* tail = resolveFieldExpr(expr, needsCopy);
    if (SymExpr* se = toSymExpr(tail)) {
      ret = se->symbol();

    } else if (isCallExpr(tail)) {
      INT_FATAL("unexpected AST in field expr");
    }
  }

  if (ret != NULL) {
    if (field->hasFlag(FLAG_TYPE_VARIABLE)) {
      if (isTypeSymbol(ret) == false && ret->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        USR_FATAL_CONT(errExpr, "error while resolving type '%s'", errTypeString);
        USR_PRINT(expr, "type field '%s' has a default expression that does not resolve to a type", field->name);
        USR_STOP();
      }
    } else if (field->hasFlag(FLAG_PARAM)) {
      if (ret->isImmediate() == false && isEnumSymbol(ret) == false) {
        USR_FATAL_CONT(errExpr, "error while resolving type '%s'", errTypeString);
        USR_PRINT(expr, "param field '%s' has a default expression that does not resolve to a param", field->name);
        USR_STOP();
      }
    } else {
      if (isTypeSymbol(ret) || ret->hasFlag(FLAG_TYPE_VARIABLE)) {
        USR_FATAL_CONT(errExpr, "error while resolving type '%s'", errTypeString);
        USR_PRINT(expr, "field '%s' has a default expression that resolves to a type", field->name);
        USR_STOP();
      }
    }
  }

  return ret;
}

static Type* resolveFieldTypeForInstantiation(Symbol* field, CallExpr* call, const char* callString) {
  Type* ret = NULL;

  if (field->type == dtUnknown || field->type->symbol->hasFlag(FLAG_GENERIC)) {
    if (Type* type = resolveFieldTypeExpr(field, call, callString)) {
      ret = type;
    } else if (field->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      if (Symbol* val = resolveFieldDefault(field, call, callString)) {
        ret = val->type;
      }
    }
  }

  return ret;
}

static void checkTypesForInstantiation(AggregateType* at, CallExpr* call, const char* callString, Symbol* field, Symbol* val) {
  const char* typeSignature = at->typeSignature;
  if (field->hasFlag(FLAG_PARAM)) {
    if (val->isImmediate() == false && isEnumSymbol(val) == false) {
      USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
      USR_PRINT(call, "type specifier did not match: %s", typeSignature);
      USR_PRINT(call, "cannot instantiate param field '%s' with non-param", field->name);
      USR_STOP();
    }
  } else if (field->hasFlag(FLAG_TYPE_VARIABLE)) {
    if (val->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
      USR_PRINT(call, "type specifier did not match: %s", typeSignature);
      USR_PRINT(call, "cannot instantiate type field '%s' with non-type", field->name);
      USR_STOP();
    }
  } else if (val->hasFlag(FLAG_TYPE_VARIABLE) == false) {
    USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
    USR_PRINT(call, "type specifier did not match: %s", typeSignature);
    USR_PRINT(call, "generic field '%s' must be instantiated with a type-expression", field->name);
    USR_STOP();
  }

  if (Type* fieldType = resolveFieldTypeForInstantiation(field, call, callString)) {
    if (fieldType->symbol->hasFlag(FLAG_GENERIC)) {
      if (getInstantiationType(val->type, NULL,
                               fieldType, NULL, call) == NULL) {
        USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
        USR_PRINT(call, "type specifier did not match: %s", typeSignature);
        USR_PRINT(call, "unable to instantiate field '%s : %s' with type '%s'", field->name, fieldType->symbol->name, val->type->symbol->name);
        USR_STOP();
      }
    } else if (canDispatch(val->type, val, fieldType, NULL, NULL, NULL, NULL, field->hasFlag(FLAG_PARAM)) == false) {
      USR_FATAL_CONT(call, "invalid type specifier '%s'", callString);
      USR_PRINT(call, "type specifier did not match: %s", typeSignature);
      USR_PRINT(call, "unable to instantiate field '%s : %s' with type '%s'", field->name, fieldType->symbol->name, val->type->symbol->name);
      USR_STOP();
    }
  }
}

static void markManagedPointerIfNonNilable(AggregateType* mp, Symbol* mps) {
  if (! mps->hasFlag(FLAG_GENERIC))
    if (Symbol* chpl_t = mp->getField("chpl_t", false))
      if (isNonNilableClassType(chpl_t->type))
        mps->addFlag(FLAG_MANAGED_POINTER_NONNILABLE);
}

AggregateType* AggregateType::generateType(SymbolMap& subs, CallExpr* call, const char* callString, bool evalDefaults, Expr* insnPoint) {
  AggregateType* retval = this;

  // Determine if there is a generic parent class
  if (isClass() == true && symbol->hasFlag(FLAG_NO_OBJECT) == false) {
    AggregateType* parent = dispatchParents.v[0];

    // Is the parent generic?
    if (parent->genericFields.size() > 0) {
      AggregateType* instantiatedParent = parent->generateType(subs, call, callString, evalDefaults, insnPoint);

      resolveConcreteFields(instantiatedParent, call, callString);

      retval = instantiationWithParent(instantiatedParent, insnPoint);
    }
  }

  // Process the local fields
  for (int index = 1; index <= numFields(); index = index + 1) {
    Symbol* field = retval->getField(index);

    bool ignoredHasDefault = false;

    if (fieldIsGeneric(field, ignoredHasDefault)) {
      if (Symbol* val = substitutionForField(field, subs)) {
        if (val != gUninstantiated) {
          retval->genericField = index;

          checkTypesForInstantiation(this, call, callString, field, val);

          retval = retval->getInstantiation(val, index, insnPoint);
        }
      } else if (evalDefaults) {
        // Attempt to instantiate a field with a default value
        retval->genericField = index;

        if (field->hasFlag(FLAG_TYPE_VARIABLE)) {
          if (Symbol* sym = resolveFieldDefault(field, call, callString)) {
            retval = retval->getInstantiation(sym, index, insnPoint);
          }
        } else if (field->hasFlag(FLAG_PARAM) && field->defPoint->init != NULL) {
          Type* expected = resolveFieldTypeExpr(field, call, callString);
          Symbol* value = resolveFieldDefault(field, call, callString);

          if (expected != NULL && value != NULL) {
            if (getInstantiationType(value->type, NULL,
                                     expected, NULL, call) == NULL) {
              // TODO: pretty-print resolved value
              USR_FATAL_CONT(call, "unable to resolve type '%s'", callString);
              USR_PRINT(call, "param field '%s' has type '%s' but default value is of incompatible type '%s'",
                        field->name, expected->symbol->name, value->type->symbol->name);
              USR_STOP();
            }
            retval = retval->getInstantiation(value, index, insnPoint);
          } else if (expected == NULL && value != NULL) {
            retval = retval->getInstantiation(value, index, insnPoint);
          }
        }
      }
    }
  }

  return retval;
}

void AggregateType::resolveConcreteType() {
  if (resolveStatus == RESOLVING || resolveStatus == RESOLVED) {
    // Recursively constructing this type
    return;
  }

  this->resolveStatus = RESOLVING;
  this->symbol->instantiationPoint = getInstantiationPoint(this->symbol->defPoint);
  //if (this->symbol->instantiationPoint)
  //  this->symbol->userInstantiationPointLoc =
  //    getUserInstantiationPoint(this->symbol);

  if (isClass() == true && symbol->hasFlag(FLAG_NO_OBJECT) == false) {
    AggregateType* parent = dispatchParents.v[0];
    parent->resolveConcreteType();
  }

  for_fields(field, this) {
    if (field->type == dtUnknown || field->type->symbol->hasFlag(FLAG_GENERIC)) {
      if (Type* type = resolveFieldTypeForInstantiation(field, NULL, NULL)) {
        field->type = type->getValType();
      }
    }
  }

  makeRefType(this);

  this->resolveStatus = RESOLVED;
}

static void buildParentSubMap(AggregateType* at, SymbolMap& map) {
  AggregateType* root = at->getRootInstantiation();
  if (root->dispatchParents.n > 0) {
    buildParentSubMap(at->dispatchParents.v[0], map);
  }
  for_fields(field, at) {
    map.put(root->getField(field->name), field);
  }
}

// Find or create an instantiation that has the provided parent as its parent
AggregateType* AggregateType::instantiationWithParent(AggregateType* parent, Expr* insnPoint) {
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
    SymbolMap parentFieldMap;
    buildParentSubMap(parent, parentFieldMap);

    retval = toAggregateType(symbol->copy(&parentFieldMap)->type);

    // Rebuild genericFields list
    // TODO: Is this redundant with ::copyInner ?
    retval->genericFields.clear();
    retval->genericFields.insert(retval->genericFields.end(), parent->genericFields.begin(), parent->genericFields.end());
    for_vector(Symbol, field, this->genericFields) {
      if (toAggregateType(field->defPoint->parentSymbol->type)->getRootInstantiation() == getRootInstantiation()) {
        retval->genericFields.push_back(getField(field->name));
      }
    }

    for (int i = 1; i <= fields.length; i++) {
      Symbol* before = getField(i);
      Symbol* after = retval->getField(i);

      if (after->hasFlag(FLAG_PARAM)) {
        if (Symbol* val = paramMap.get(before)) {
          paramMap.put(after, val);
        }
      }
    }

    if (retval->symbol->instantiationPoint == NULL) {
      retval->symbol->instantiationPoint = toBlockStmt(insnPoint);
      //if (retval->symbol->instantiationPoint)
      //  retval->symbol->userInstantiationPointLoc =
      //    getUserInstantiationPoint(retval->symbol);
    }

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

    if (retval->genericFields.size() == 0) {
      retval->symbol->removeFlag(FLAG_GENERIC);
    }

    retval->renameInstantiation();

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

static const char* prettyPrintString(VarSymbol* var, bool cname) {
  std::string ret;
  const size_t bufSize = 128;
  char immediate[bufSize];
  snprint_imm(immediate, bufSize, *var->immediate);

  // escape quote characters in name string
  char name[bufSize];
  char * name_p = &name[0];
  char * immediate_p = &immediate[0];
  for ( ;
        name_p < &name[bufSize-1] && // don't overflow buffer
          '\0' != *immediate_p;      // stop at null in source
        name_p++, immediate_p++) {
    if ('"' == *immediate_p) { // escape quotes
      *name_p++ = '\\';
    }
    *name_p = *immediate_p;
  }
  *name_p = '\0';
  ret += name;

  // add ellipsis if too long for buffer
  if (name_p == &name[bufSize-1]) {
    ret += "...";
  }

  if (!cname) {
    return astr(ret);
  }

  // filter unacceptable characters for cname string
  char cstr[bufSize];
  char * cstr_p = &cstr[0];
  immediate_p = &immediate[0];
  size_t maxNameLength = 32; // add "_etc" after this many characters

  for ( ; immediate_p < &immediate_p[bufSize-1] &&  // don't overflow buffer
          cstr_p < &cstr[maxNameLength-1] &&      // stop at max length
          '\0' != *immediate_p;
        immediate_p++ ) {
    if (('A' <= *immediate_p && *immediate_p <= 'Z') ||
        ('a' <= *immediate_p && *immediate_p <= 'z') ||
        ('0' <= *immediate_p && *immediate_p <= '9') ||
        ('_' == *immediate_p)) {
      *cstr_p = *immediate_p;
      cstr_p++;
    }
  }
  *cstr_p = '\0';
  ret = cstr;

  // add _etc if too long
  if (immediate_p == &immediate[bufSize-1] || // too long for buffer
      cstr_p == &cstr[maxNameLength-1]) {   // exceeds max length
    ret += "_etc";
  }

  return astr(ret);
}

static const char* buildValueName(Symbol* field, bool cname) {
  if (field->hasFlag(FLAG_PARAM)) {
    Symbol* sym = paramMap.get(field);
    VarSymbol* var = toVarSymbol(sym);
    if (var && var->immediate) {
      std::string ret;
      Type* type = var->type;

      Immediate* imm = var->immediate;
      if (var->type == dtString || var->type == dtStringC) {
        ret += prettyPrintString(var, cname);
      } else if (imm->const_kind == NUM_KIND_BOOL) {
        const char* tn = cname ? "T" : "true";
        const char* fn = cname ? "F" : "false";
        ret += imm->bool_value() ? tn : fn;
      } else {
        char buf[128];
        snprint_imm(buf, sizeof(buf), *var->immediate);
        ret += buf;
      }

      if (is_int_type(type) ||
          is_uint_type(type) ||
          is_bool_type(type) ||
          is_real_type(type) ||
          is_imag_type(type) ||
          is_complex_type(type)) {
        if (!isNumericParamDefaultType(type)) {
          if (!cname) {
            ret += ":";
            ret += toString(type);
          } else {
            // TODO: The result of this is kind of weird. For example, if I have
            // a param uint(8) of '100' the string will be '1008'.
            char buf[16];
            snprintf(buf, sizeof(buf), "%i", get_width(type));
            ret += buf;
          }
        }
      }

      return astr(ret);
    } else {
      // Might be an enum
      return (cname ? sym->cname : sym->name);
    }
  } else {
    return (cname ? field->type->symbol->cname : field->type->symbol->name);
  }
}

static bool buildFieldNames(AggregateType* at, std::string& str, bool cname) {
  bool useNamed = false;

  AggregateType* root = at->getRootInstantiation();
  if (root->genericFields.size() > 0) {
    if (at->genericFields.size() == 0) {
      // A fully instantiated type
      bool isFirst = true;
      for_vector(Symbol, field, root->genericFields) {

        if (isFirst) {
          isFirst = false;
        } else {
          str += cname ? "_" : ",";
        }

        if (useNamed) {
          str += field->name;
          str += "=";
        }

        Symbol* newField = at->getField(field->name);
        str += buildValueName(newField, cname);
      }
    } else {
      // A partial instantiation
      unsigned int curIdx = 0;
      bool isFirst = true;
      for_vector(Symbol, field, root->genericFields) {
        if (curIdx < at->genericFields.size() &&
            field->name == at->genericFields[curIdx]->name) {
          useNamed = true;
          curIdx += 1;
        } else {
          if (isFirst) {
            isFirst = false;
          } else {
            str += cname ? "_" : ",";
          }

          if (useNamed) {
            str += field->name;
            str += "=";
          }

          Symbol* newField = at->getField(field->name);
          str += buildValueName(newField, cname);
        }
      }
    }
  }

  return useNamed;
}

void AggregateType::renameInstantiation() {
  std::string name = getRootInstantiation()->symbol->name;
  std::string cname = name + "_";

  if (!developer && isManagedPtrType(this)) {
    name = toString(this, false);
  } else if (!developer && symbol->hasFlag(FLAG_SYNC)) {
    name = "sync ";
    buildFieldNames(this, name, false);
  } else if (!developer && symbol->hasFlag(FLAG_SINGLE)) {
    name = "single ";
    buildFieldNames(this, name, false);
  } else {
    name += "(";
    buildFieldNames(this, name, false);
    name += ")";
  }

  symbol->name = astr(name);

  buildFieldNames(this, cname, true);
  symbol->cname = astr(cname);
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
AggregateType* AggregateType::getInstantiation(Symbol* sym, int index, Expr* insnPoint) {
  AggregateType* retval = NULL;

  Type* symType = sym->typeInfo();
  // Normalize `_owned(anymanaged-MyClass)` to `_owned(borrowed MyClass)`
  if (isManagedPtrType(this)) {
    if (isClassLikeOrManaged(symType)) {
      ClassTypeDecorator d = CLASS_TYPE_BORROWED_NONNIL;
      if (isNilableClassType(symType))
        d = CLASS_TYPE_BORROWED_NILABLE;

      if (isManagedPtrType(symType))
        checkDuplicateDecorators(this, symType, insnPoint);

      symType = ::getDecoratedClass(symType, d);
    }
  }

  this->genericField = index;
  if (AggregateType* at = getCurInstantiation(sym, symType)) {
    retval = at;
  } else {
    retval = getNewInstantiation(sym, symType, insnPoint);
  }

  if (retval->symbol->hasFlag(FLAG_MANAGED_POINTER))
    markManagedPointerIfNonNilable(retval, retval->symbol);

  return retval;
}

//
// This method tries to find a pre-existing AggregateType instance that
// represents the resulting instantiation of binding 'sym' to the current
// generic field. This way there is only ever one instance of AggregateType for
// a particular instantiation.
//
AggregateType* AggregateType::getCurInstantiation(Symbol* sym, Type* symType) {
  AggregateType* retval = NULL;

  for_vector(AggregateType, at, instantiations) {
    Symbol* field = at->getField(genericField);

    if (field->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      if (givesType(sym) == true && field->type == symType) {
        retval = at;
        break;
      }

    } else if (field->hasFlag(FLAG_PARAM) == true) {
      Type* expected = NULL;
      if (field->defPoint->exprType != NULL) {
        expected = field->defPoint->exprType->typeInfo();
      }

      //
      // The types of 'sym' and 'field' might by different if the user
      // specified a literal that will eventually be coerced into the correct
      // field type.  For example '42' is an 'int(64)' but could be coerced to
      // a 'uint(64)'. In such cases we should compare the values of 'sym'
      // and the current instantiation's field.
      //
      // Note: only check when the field has a type expression
      //
      // See param/ferguson/mismatched-param-type-error.chpl for an example
      // where this check is necessary.
      //
      if (expected != NULL && expected != symType) {
        Immediate result;
        Immediate* lhs = getSymbolImmediate(at->substitutions.get(field));
        Immediate* rhs = getSymbolImmediate(sym);
        fold_constant(P_prim_equal, lhs, rhs, &result);
        if (result.v_bool) {
          retval = at;
        }
      } else if (at->substitutions.get(field) == sym) {
        retval = at;
        break;
      }

    } else {
      if (field->type == symType) {
        retval = at;
        break;
      }
    }
  }

  return retval;
}

AggregateType* AggregateType::getNewInstantiation(Symbol* sym, Type* symType, Expr* insnPoint) {
  AggregateType* retval = toAggregateType(symbol->copy()->type);
  Symbol*        field  = retval->getField(genericField);

  for (unsigned int idx = 0; idx < retval->genericFields.size(); idx++) {
    if (retval->genericFields[idx] == field) {
      retval->genericFields.erase(retval->genericFields.begin() + idx);
      break;
    }
  }

  symbol->defPoint->insertBefore(new DefExpr(retval->symbol));

  retval->instantiatedFrom = this;
  if (retval->symbol->instantiationPoint == NULL) {
    retval->symbol->instantiationPoint = toBlockStmt(insnPoint);
    //if (retval->symbol->instantiationPoint != NULL)
    //  retval->symbol->userInstantiationPointLoc =
    //    getUserInstantiationPoint(retval->symbol);
  }

  retval->symbol->copyFlags(symbol);

  retval->substitutions.copy(substitutions);

  for (int i = 1; i <= fields.length; i++) {
    Symbol* before = getField(i);
    Symbol* after = retval->getField(i);

    if (after->hasFlag(FLAG_PARAM)) {
      paramMap.put(after, paramMap.get(before));
    }
  }

  if (field->hasFlag(FLAG_PARAM) == true) {
    Type* fieldType = NULL;
    if (field->defPoint->exprType) {
      fieldType = field->defPoint->exprType->typeInfo();
    }

    // Sometimes 'sym' might be a different type from the field. For example,
    // the literal '42' is an int(64), and the field might be a uint(64). In
    // such cases, we need to coerce to a new symbol that will be placed in
    // the substitutions map.
    if (fieldType != NULL  &&
        fieldType != dtUnknown &&
        fieldType != sym->getValType()) {
      Immediate coerce = getDefaultImmediate(fieldType);
      Immediate* from = toVarSymbol(sym)->immediate;
      coerce_immediate(from, &coerce);
      sym = new_ImmediateSymbol(&coerce);
      symType = sym->type;
    }

    retval->substitutions.put(field, sym);
    paramMap.put(field,sym);

  } else {
    retval->substitutions.put(field, symType->symbol);
  }

  if (field->hasFlag(FLAG_TYPE_VARIABLE) == true && givesType(sym) == true) {
    field->type = symType;

  } else if (field->defPoint->exprType == NULL) {
    if (field->type == dtUnknown) {
      field->type = symType;
    }

  } else {
    Type* fieldType = field->defPoint->exprType->typeInfo();
    if (fieldType->symbol->hasFlag(FLAG_GENERIC)) {
      field->type = symType;
    } else if (fieldType == symType) {
      field->type = symType;
    } else {
      INT_FATAL("unexpected type for field instantiation");
    }
  }

  forv_Vec(AggregateType, at, dispatchParents) {
    retval->dispatchParents.add(at);
    at->dispatchChildren.add_exclusive(retval);
  }

  retval->renameInstantiation();

  if (retval->genericFields.size() == 0) {
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

  SymbolMap parentMap;
  buildParentSubMap(parentType, parentMap);
  // Otherwise, we need to create an instantiation for that type
  AggregateType* newInstance = toAggregateType(this->symbol->copy(&parentMap)->type);

  // Update new type's 'genericFields' list with the symbols from the new type
  // TODO: Is this redundant with ::copyInner ?
  newInstance->genericFields.clear();
  newInstance->genericFields.insert(newInstance->genericFields.end(), parentType->genericFields.begin(), parentType->genericFields.end());
  for_vector(Symbol, field, this->genericFields) {
    if (toAggregateType(field->defPoint->parentSymbol->type)->getRootInstantiation() == getRootInstantiation()) {
      newInstance->genericFields.push_back(getField(field->name));
    }
  }

  this->symbol->defPoint->insertBefore(new DefExpr(newInstance->symbol));

  newInstance->symbol->copyFlags(this->symbol);

  newInstance->substitutions.copy(this->substitutions);

  newInstance->instantiatedFrom = this;

  Symbol* field = newInstance->getField(1);
  newInstance->substitutions.put(field, parentType->symbol);

  field->type = parentType;

  instantiations.push_back(newInstance);

  // Handle dispatch parent
  newInstance->dispatchParents.add(parentType);

  bool inserted = parentType->dispatchChildren.add_exclusive(newInstance);
  INT_ASSERT(inserted);

  newInstance->renameInstantiation();

  if (newInstance->genericFields.size() == 0) {
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

  if (fatal) {
    // TODO: report as a user error in certain cases
    INT_FATAL(this,
              "no field '%s' in class '%s' in getField()",
              name, this->symbol->name);
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

static const char* buildTypeSignature(AggregateType* at) {
  std::string temp = at->symbol->name;
  temp += "(";

  bool isFirst = true;
  for_vector(Symbol, field, at->genericFields) {
    if (isFirst) {
      isFirst = false;
    } else {
      temp += ", ";
    }

    if (field->hasFlag(FLAG_PARAM)) {
      temp += "param ";
    } else {
      temp += "type ";
    }

    temp += field->name;

    if (field->defPoint->exprType != NULL) {
      if (SymExpr* se = toSymExpr(field->defPoint->exprType)) {
        temp += ": ";
        temp += se->symbol()->name;
      }
    }
  }

  temp += ")";

  return astr(temp);
}

void AggregateType::processGenericFields() {
  if (foundGenericFields) {
    return;
  }

  foundGenericFields = true;
  bool isGenericWithDefaults = mIsGeneric;

  if (isClass() == true && dispatchParents.n > 0) {
    AggregateType* parent = dispatchParents.v[0];
    if (parent != dtObject) {
      parent->processGenericFields();

      if (parent->mIsGeneric) {
        isGenericWithDefaults = parent->mIsGenericWithDefaults;
      }

      for_vector(Symbol, field, parent->genericFields) {
        if (isFieldInThisClass(field->name) == false) {
          genericFields.push_back(field);
        }
      }
    }
  }

  for_fields(field, this) {
    if (field->hasFlag(FLAG_SUPER_CLASS)) continue;

    if (field->hasFlag(FLAG_PARAM) || field->hasFlag(FLAG_TYPE_VARIABLE)) {
      if (isTypeSymbol(field) == false) {
        genericFields.push_back(field);
        if (field->defPoint->init == NULL) {
          isGenericWithDefaults = false;
        }
      }
    } else if (field->defPoint->init == NULL) {
      if (field->defPoint->exprType == NULL) {
        genericFields.push_back(field); // "var x;"
        isGenericWithDefaults = false;
      } else if (isFieldTypeExprGeneric(field->defPoint->exprType)) {
        genericFields.push_back(field); // "var x : integral;"
        isGenericWithDefaults = false;
      }
    }
  }

  typeSignature = buildTypeSignature(this);

  this->mIsGenericWithDefaults = isGenericWithDefaults;
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

    if (this->isUnion() == false) {
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

        normalize(fn);

        // BHARSH INIT TODO: Should this be part of normalize(fn)? If we did
        // that we would emit two use-before-def errors for classes because of
        // the generated _new function.
        checkUseBeforeDefs(fn);

        methods.add(fn);
      } else {
        USR_FATAL(this, "Unable to generate initializer for type '%s'", this->symbol->name);
      }
    } else {
      DefExpr* def = new DefExpr(fn);
      symbol->defPoint->insertBefore(def);

      fn->setMethod(true);
      fn->addFlag(FLAG_METHOD_PRIMARY);

      fn->insertAtTail(new CallExpr(PRIM_SET_UNION_ID,
                                    fn->_this,
                                    new_IntSymbol(0)));

      normalize(fn);

      methods.add(fn);
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

        if (field->hasFlag(FLAG_UNSAFE))
          arg->addFlag(FLAG_UNSAFE);

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

            CallExpr* def    = new CallExpr(PRIM_DEFAULT_INIT_FIELD,
                    // It would be easiest to just put 'field' here, however
                    // it is replaced with 'arg' in buildDefaultInitializer().
                    new_StringSymbol(field->defPoint->parentSymbol->name),
                                            new_StringSymbol(field->name),
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
          if (method && method->isDefaultInit()) {
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
  if (isRecordOrUnionWithInitializers(this) == true) {
    SET_LINENO(this);

    bool isGeneric = false;
    // If this type is generic, then the 'other' formal needs to be generic as
    // well
    // TODO: Why can't we use 'fieldIsGeneric' here?
    for_fields(fieldDefExpr, this) {
      if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
        if (field->hasFlag(FLAG_SUPER_CLASS) == false) {
          if (field->hasFlag(FLAG_PARAM) || field->isType() ||
              (field->defPoint->init == NULL && field->defPoint->exprType == NULL)) {
            isGeneric = true;
          }
        }
      }
    }

    FnSymbol*  fn    = new FnSymbol(astrInitEquals);

    DefExpr*   def   = new DefExpr(fn);

    ArgSymbol* _mt   = new ArgSymbol(INTENT_BLANK, "_mt",   dtMethodToken);
    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this",  this);

    ArgSymbol* ThisType = NULL;
    ArgSymbol* other = NULL;
    if (isGeneric) {
      other = new ArgSymbol(INTENT_BLANK, "other", dtUnknown, new CallExpr(PRIM_TYPEOF, new SymExpr(_this)));
      other->addFlag(FLAG_MARKED_GENERIC);
    } else {
      other = new ArgSymbol(INTENT_BLANK, "other", this);
    }

    fn->cname = fn->name;
    fn->_this = _this;

    fn->addFlag(FLAG_COMPILER_GENERATED);
    fn->addFlag(FLAG_LAST_RESORT);
    fn->addFlag(FLAG_COPY_INIT);
    fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);

    _this->addFlag(FLAG_ARG_THIS);

    fn->insertFormalAtTail(_mt);
    fn->insertFormalAtTail(_this);
    if (ThisType != NULL) fn->insertFormalAtTail(ThisType);
    fn->insertFormalAtTail(other);

    if (symbol->hasFlag(FLAG_EXTERN)) {
      if (other->hasFlag(FLAG_MARKED_GENERIC))
        INT_FATAL("extern type is generic");

      // Generate a bit-copy for extern records in order to copy unknown fields.
      fn->insertAtHead(new CallExpr(PRIM_ASSIGN, fn->_this, other));

    } else if (aggregateTag == AGGREGATE_UNION) {
      // set field ID to 0 and then rely on field accessor
      // call below to set it to the right value (and default init)
      fn->insertAtTail(new CallExpr(PRIM_SET_UNION_ID,
                                    fn->_this,
                                    new_IntSymbol(0)));

      for_fields(fieldDefExpr, this) {
        if (VarSymbol* field = toVarSymbol(fieldDefExpr)) {
          Symbol* fieldNameSymbol = new_CStringSymbol(field->name);

          CallExpr* thisField  = new CallExpr(".",
                                              fn->_this,
                                              fieldNameSymbol);

          CallExpr* otherField = new CallExpr(".",
                                              other,
                                              fieldNameSymbol);

          CallExpr* setField = new CallExpr("=", thisField, otherField);

          CallExpr* isField =
            new CallExpr("==", new CallExpr(PRIM_GET_UNION_ID, other),
                               new CallExpr(PRIM_FIELD_NAME_TO_NUM,
                                            this->symbol,
                                            fieldNameSymbol));

          fn->insertAtTail(new CondStmt(isField, setField));
        }
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
    if (sym == NULL) {
      USR_FATAL(se, "unable to find parent class named '%s'", se->unresolved);
    }
    // Use AggregateType in class hierarchy rather than generic-management
    if (isDecoratedClassType(sym->type)) {
      sym = canonicalClassType(sym->type)->symbol;
    }
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

Type* AggregateType::getDecoratedClass(ClassTypeDecorator d) {

  int packedDecorator = -1;
  // -1 -> just use the canonical type (e.g. MyClass == borrowed MyClass!)
  //  0 -> borrowed MyClass?
  //  1 -> unmanaged MyClass!
  //  2 -> unmanaged MyClass?
  //  3 -> generic-management generic-nilability MyClass
  //  4 -> generic-management MyClass!
  //  5 -> generic-management MyClass?
  switch (d) {
    case CLASS_TYPE_BORROWED:          packedDecorator = -1; break;
    case CLASS_TYPE_BORROWED_NONNIL:   packedDecorator = -1; break;
    case CLASS_TYPE_BORROWED_NILABLE:  packedDecorator =  0; break;
    case CLASS_TYPE_UNMANAGED:         packedDecorator =  1; break;
    case CLASS_TYPE_UNMANAGED_NONNIL:  packedDecorator =  1; break;
    case CLASS_TYPE_UNMANAGED_NILABLE: packedDecorator =  2; break;
    case CLASS_TYPE_MANAGED:           packedDecorator = -1; break;
    case CLASS_TYPE_MANAGED_NONNIL:    packedDecorator =  1; break;
    case CLASS_TYPE_MANAGED_NILABLE:   packedDecorator =  2; break;
    case CLASS_TYPE_GENERIC:           packedDecorator =  3; break;
    case CLASS_TYPE_GENERIC_NONNIL:    packedDecorator =  4; break;
    case CLASS_TYPE_GENERIC_NILABLE:   packedDecorator =  5; break;
      // intentionally no default
  }

  INT_ASSERT(packedDecorator < NUM_PACKED_DECORATED_TYPES);

  if (aggregateTag != AGGREGATE_CLASS &&
      !isManagedPtrType(this))
    INT_FATAL("Bad call to getDecoratedClass");

  AggregateType* at = this;

  if (isManagedPtrType(this)) {
    if (d != CLASS_TYPE_MANAGED_NONNIL &&
        d != CLASS_TYPE_MANAGED_NILABLE) {
      // Get the class type underneath
      Type* bt = getManagedPtrBorrowType(this);
      if (bt && bt != dtUnknown && isAggregateType(bt))
        at = toAggregateType(bt);
    }
  }

  if (packedDecorator < 0)
    return at;

  // borrowed == canonical class type
  if (d == CLASS_TYPE_BORROWED) {
    if (aggregateTag == AGGREGATE_CLASS)
      return at;
    else
      INT_FATAL("invalid type for borrowed variant");
  }

  // Otherwise, gather the appropriate class type.
  if (!at->decoratedClasses[packedDecorator]) {
    SET_LINENO(at->symbol->defPoint);
    // Generate decorated class type
    DecoratedClassType* dec = new DecoratedClassType(at, d);
    at->decoratedClasses[packedDecorator] = dec;
    const char* astrName = decoratedTypeAstr(d, at->symbol->name);
    TypeSymbol* tsDec = new TypeSymbol(astrName, dec);
    // The dec type isn't really an object, shouldn't have its own fields
    tsDec->copyFlags(at->symbol);
    tsDec->addFlag(FLAG_NO_OBJECT);
    // Propagate generic-ness to the decorated type
    if (at->isGeneric() || at->symbol->hasFlag(FLAG_GENERIC))
      tsDec->addFlag(FLAG_GENERIC);
    // Generic management is generic
    if (isDecoratorUnknownManagement(d))
      tsDec->addFlag(FLAG_GENERIC);
    // The generated code should just use the canonical class name
    tsDec->cname = at->symbol->cname;
    DefExpr* defDec = new DefExpr(tsDec);
    symbol->defPoint->insertAfter(defDec);
  }

  return at->decoratedClasses[packedDecorator];
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
