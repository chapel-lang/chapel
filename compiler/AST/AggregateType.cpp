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

#include "AggregateType.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "docsDriver.h"
#include "expr.h"
#include "iterator.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

AggregateType* dtString            = NULL;
AggregateType* dtArray             = NULL;
AggregateType* dtBaseArr           = NULL;
AggregateType* dtBaseDom           = NULL;
AggregateType* dtDist              = NULL;
AggregateType* dtTuple             = NULL;
AggregateType* dtLocale            = NULL;
AggregateType* dtLocaleID          = NULL;
AggregateType* dtMainArgument      = NULL;
AggregateType* dtOnBundleRecord    = NULL;
AggregateType* dtTaskBundleRecord  = NULL;
AggregateType* dtError             = NULL;

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

  classId = 0;

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

void AggregateType::buildConstructors() {
  if (defaultInitializer == NULL) {
    SET_LINENO(this);

    buildTypeConstructor();
    buildConstructor();
  }
}

// Create the (default) type constructor for this class.
void AggregateType::buildTypeConstructor() {
  // Do nothing if it is already built
  if (defaultTypeConstructor != NULL)
    return;

  // Create the type constructor function,
  FnSymbol* fn = new FnSymbol(astr("_type_construct_", symbol->name));

  fn->addFlag(FLAG_TYPE_CONSTRUCTOR);
  fn->cname = astr("_type_construct_", symbol->cname);

  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->retTag = RET_TYPE;

  if (symbol->hasFlag(FLAG_REF)   == true) {
    fn->addFlag(FLAG_REF);
  }

  if (symbol->hasFlag(FLAG_TUPLE) == true) {
    fn->addFlag(FLAG_TUPLE);
    fn->addFlag(FLAG_INLINE);

    gGenericTupleTypeCtor = fn;
  }

  // and insert it into the class type.
  defaultTypeConstructor = fn;

  // Create "this".
  fn->_this = new VarSymbol("this", this);
  fn->_this->addFlag(FLAG_ARG_THIS);

  fn->insertAtTail(new DefExpr(fn->_this));

  Vec<const char*> fieldNamesSet;
  CallExpr*        superCall = NULL;

  // Copy arguments from superclass type constructor
  // (supporting inheritance from generic classes)
  if (isClass() == true && dispatchParents.n > 0) {

    if (AggregateType* parentTy = toAggregateType(dispatchParents.v[0])){

      // This class/record has a parent class/record
      if (parentTy->defaultTypeConstructor == NULL) {
        // If it doesn't yet have an type constructor, make one
        parentTy->buildTypeConstructor();
      }

      FnSymbol* superTypeCtor = parentTy->defaultTypeConstructor;

      if (superTypeCtor->numFormals() > 0) {
        superCall = new CallExpr(parentTy->symbol->name);

        // Now walk through arguments in super class type constructor
        for_formals(formal, superTypeCtor) {
          DefExpr*   superArg         = formal->defPoint->copy();
          ArgSymbol* arg              = toArgSymbol(superArg->sym->copy());
          bool       fieldInThisClass = false;

          for_fields(sym, this) {
            if (strcmp(sym->name, arg->name) == 0) {
              fieldInThisClass = true;
            }
          }

          // If the field is also present in the child, adjust the field
          // name in the super. Otherwise it would not be possible to
          // type construct the super.
          if (fieldInThisClass == false) {
            arg->addFlag(FLAG_PARENT_FIELD);

            fn->insertFormalAtTail(arg);

            superCall->insertAtTail(new SymExpr(arg));
          }
        }
      }
    }
  }

  for_fields(tmp, this) {
    SET_LINENO(tmp);

    if (VarSymbol* field = toVarSymbol(tmp)) {
      if (field->hasFlag(FLAG_SUPER_CLASS) == true) {
        // supporting inheritance from generic classes
        if (superCall != NULL) {
          CallExpr* newInit = new CallExpr(PRIM_TYPE_INIT, superCall);
          CallExpr* newSet  = new CallExpr(PRIM_SET_MEMBER,
                                           fn->_this,
                                           new_CStringSymbol(field->name),
                                           newInit);
          fn->insertAtTail(newSet);
        }

        continue;
      }


      Expr* exprType = field->defPoint->exprType;
      Expr* init     = field->defPoint->init;

      if (strcmp(field->name, "_promotionType") == 0) {
        fn->insertAtTail(
             new BlockStmt(new CallExpr(PRIM_SET_MEMBER,
                                        fn->_this,
                                        new_CStringSymbol(field->name),
                                        new CallExpr(PRIM_TYPE_INIT,
                                                     exprType->remove())),
                           BLOCK_TYPE));

      } else {
        fieldNamesSet.set_add(field->name);

        //
        // if formal is generic
        //
        if (field->isType() ||
            field->hasFlag(FLAG_PARAM)         ||
            (!exprType && !init)) {

          ArgSymbol* arg = AggregateType::createGenericArg(field);

          // Indicate which type constructor args are also for super class
          // This helps us to call the superclass type constructor in
          // resolution
          if (field->hasFlag(FLAG_PARENT_FIELD) == true) {
            arg->addFlag(FLAG_PARENT_FIELD);
          }

          fn->insertFormalAtTail(arg);

          if (field->hasFlag(FLAG_PARAM) == true || field->isType() == true) {
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                          fn->_this,
                                          new_CStringSymbol(field->name),
                                          arg));

          } else if (arg->type                 == dtAny &&
                     symbol->hasFlag(FLAG_REF) == false) {
            // It would be nice to be able to remove this case.
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                          fn->_this,
                                          new_CStringSymbol(field->name),
                                          new CallExpr(PRIM_TYPE_INIT, arg)));
          }

        } else if (exprType != NULL) {
          CallExpr* newInit = new CallExpr(PRIM_TYPE_INIT, exprType->copy());
          CallExpr* newSet  = new CallExpr(PRIM_SET_MEMBER,
                                           fn->_this,
                                           new_CStringSymbol(field->name),
                                           newInit);
          fn->insertAtTail(newSet);

        } else if (init     != NULL) {
          fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                        fn->_this,
                                        new_CStringSymbol(field->name),
                                        new CallExpr("chpl__initCopy",
                                                     init->copy())));
        }
      }
    }
  }

  // Add return
  fn->insertAtTail(new CallExpr(PRIM_RETURN, fn->_this));

  fn->retType = this;

  symbol->defPoint->insertBefore(new DefExpr(fn));

  // Make implicit references to 'this' explicit.
  AggregateType::insertImplicitThis(fn, fieldNamesSet);

  Symbol* parSym = symbol->defPoint->parentSymbol;

  if (AggregateType* outerType = toAggregateType(parSym->type)) {
    // Create an "outer" pointer to the outer class in the inner class
    VarSymbol* tmpOuter = new VarSymbol("outer");

    outerType->moveConstructorToOuter(fn);

    // Save the pointer to the outer class
    fields.insertAtTail(new DefExpr(tmpOuter));

    fn->insertAtHead(new CallExpr(PRIM_SET_MEMBER,
                                  fn->_this,
                                  new_CStringSymbol("outer"),
                                  fn->_outer));

    outer = tmpOuter;
  }

  addToSymbolTable(fn);
}


// For the given class type, this builds the compiler-generated constructor
// which is also called by user-defined constructors to pre-initialize all
// fields to their declared or type-specific initial values.
void AggregateType::buildConstructor() {
  if (initializerStyle == DEFINES_INITIALIZER) {
    // Don't want to create the default constructor if we have seen
    // initializers defined.  The work is completely unnecessary,
    // since we won't call the default constructor, and it mutates
    // information about the fields that we would rather stayed unmutated.
    return;
  }

  // Create the default constructor function symbol,
  FnSymbol* fn = new FnSymbol(astr("_construct_", symbol->name));

  fn->cname = fn->name;

  fn->addFlag(FLAG_DEFAULT_CONSTRUCTOR);
  fn->addFlag(FLAG_CONSTRUCTOR);
  fn->addFlag(FLAG_COMPILER_GENERATED);

  if (symbol->hasFlag(FLAG_REF) == true) {
    fn->addFlag(FLAG_REF);
  }

  if (symbol->hasFlag(FLAG_TUPLE) == true) {
    fn->addFlag(FLAG_TUPLE);
    fn->addFlag(FLAG_INLINE);

    gGenericTupleInit = fn;
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
          strcmp(field->name, "_promotionType") != 0 &&
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
        if (dispatchParents.v[0]->defaultInitializer == NULL) {
          AggregateType* at = toAggregateType(dispatchParents.v[0]);

          // If it doesn't yet have an initializer, make one.
          at->buildConstructors();
        }

        // Get the parent constructor.
        // Note that since we only pay attention to the first entry in the
        // dispatchParents list, we are effectively implementing
        // single class inheritance, multiple interface inheritance.
        FnSymbol* superCtor = dispatchParents.v[0]->defaultInitializer;

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

ArgSymbol* AggregateType::createGenericArg(VarSymbol* field) {
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);

  // We take it as a param argument if it is marked as a param field.
  if (field->hasFlag(FLAG_PARAM)) {
    arg->intent = INTENT_PARAM;
  } else {
    // Both type arguments and arguments of unspecified type get this flag.
    arg->addFlag(FLAG_TYPE_VARIABLE);
  }

  // Copy the field type if it exists.
  Expr* exprType = field->defPoint->exprType;

  if (exprType) {
    arg->typeExpr = new BlockStmt(exprType->copy(), BLOCK_TYPE);
  }

  // Copy the initialization expression if it exists.
  Expr* init = field->defPoint->init;

  if (init) {
    arg->defaultExpr = new BlockStmt(init->copy(), BLOCK_SCOPELESS);
  }

  // Translate an unknown field type into an unspecified arg type.
  if (exprType == NULL && arg->type == dtUnknown) {
    if (field->isType() == false) {
      arg->addFlag(FLAG_GENERIC);
    }

    arg->type = dtAny;
  }

  return arg;
}

/// Replace implicit references to 'this' in the body of this
/// type constructor with explicit member reference (dot) expressions.
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

void AggregateType:: moveConstructorToOuter(FnSymbol* fn) {
  Expr* insertPoint = symbol->defPoint;

  // Remove the DefPoint for this constructor, add it to the outer
  // class's method list.
  methods.add(fn);

  fn->_outer = new ArgSymbol(INTENT_BLANK, "outer", this);

  fn->_outer->addFlag(FLAG_GENERIC); // Arg expects a real object :-P.

  fn->insertFormalAtHead(new DefExpr(fn->_outer));

  fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK,
                                                   "_mt",
                                                   dtMethodToken)));
  fn->addFlag(FLAG_METHOD);

  fn->addFlag(FLAG_METHOD_PRIMARY);

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
  UnresolvedSymExpr* se  = toUnresolvedSymExpr(storesName);
  Symbol*            sym = lookup(storesName, se->unresolved);
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

    if (ct->initializerStyle == DEFINES_NONE_USE_DEFAULT) {
      // We hadn't previously seen a constructor or initializer definition.
      // Update the field on the type appropriately.
      if (isInit) {
        ct->initializerStyle = DEFINES_INITIALIZER;

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
  // make root records inherit from value
  // make root classes inherit from object
  if (inherits.length == 0 && symbol->hasFlag(FLAG_NO_OBJECT) == false) {
    SET_LINENO(this);

    if (isRecord() == true) {
      dispatchParents.add(dtValue);

      // Assume that this addition is unique; report if not.
      if (dtValue->dispatchChildren.add_exclusive(this) == false) {
        INT_ASSERT(false);
      }

    } else if (isClass() == true) {
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
