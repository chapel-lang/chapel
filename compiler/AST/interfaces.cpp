/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "AstVisitor.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"

/////////////////////////////////////////////////////////////////////////////
// InterfaceSymbol
//

//
// An interface formal is a TypeSymbol, defining a ConstrainedType.
//

static Symbol* isInterfaceFormalSymbol(Symbol* sym) {
  if (TypeSymbol* var = toTypeSymbol(sym))
    return var;
  // we could also allow param formals
  return NULL;
}

static Symbol* isInterfaceFormalDecl(Expr* expr) {
  if (DefExpr* def = toDefExpr(expr))
    if (Symbol* ifor = isInterfaceFormalSymbol(def->sym))
      return ifor;
  return NULL;
}

static FnSymbol* toInterfaceFunDecl(Expr* expr) {
  if (DefExpr* def = toDefExpr(expr))
    if (FnSymbol* fn = toFnSymbol(def->sym))
      return fn;
  return NULL;
}

DefExpr* InterfaceSymbol::buildDef(const char* name,
                                   CallExpr*   formals,
                                   BlockStmt*  body)
{
  InterfaceSymbol* isym = new InterfaceSymbol(name, body);

  for_alist(formal, formals->argList) {
    isym->ifcFormals.insertAtTail(formal->remove());
    INT_ASSERT(isInterfaceFormalDecl(formal)); // from parser
  }

  // at least 1 formal is required
  if (isym->ifcFormals.empty())
    USR_FATAL_CONT(isym, "interface %s has no formal arguments\n"
                   "  at least one formal argument is required", isym->name);

  // ifcBody can be empty; check that the content is appropriate
  for_alist(expr, isym->ifcBody->body)
    if (toInterfaceFunDecl(expr) == NULL) {
      USR_FATAL_CONT(expr, "not a 'proc' or 'iter' declaration");
      USR_PRINT(expr,"  only function declarations"
                     " are currently allowed in an interface");
    }

  return new DefExpr(isym);
}

DefExpr* InterfaceSymbol::buildFormal(const char* name,
                                      IntentTag intent)
{
  Symbol* formal = NULL;
  if (intent == INTENT_TYPE) {
    formal = ConstrainedType::build(name);
  } else {
    INT_FATAL(formal, "unexpected intent");
  }
  return new DefExpr(formal);
}

InterfaceSymbol::InterfaceSymbol(const char* name, BlockStmt* body) :
  Symbol(E_InterfaceSymbol, name, NULL), ifcFormals(), ifcBody(body)
{
  ifcFormals.parent = this;
  gInterfaceSymbols.add(this);
}

void InterfaceSymbol::verify() {
  Symbol::verify();
  INT_ASSERT(astTag == E_InterfaceSymbol);

  verifyNotOnList(ifcBody);
  INT_ASSERT(ifcBody->parentSymbol == this);

  INT_ASSERT(!resolved);
}

void InterfaceSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterInterfaceSym(this)) {
    for_alist(formal, ifcFormals)
      formal->accept(visitor);
    ifcBody->accept(visitor);
    visitor->exitInterfaceSym(this);
  }
}

InterfaceSymbol* InterfaceSymbol::copyInner(SymbolMap* map) {
  if (this->defPoint && isGlobal(this))
    INT_FATAL(this, "unexpected");
  else
    USR_FATAL_CONT(this,
                   "only module-level interfaces are currently implemented");
  return this; //dummy
}

void InterfaceSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == ifcBody)
    ifcBody = toBlockStmt(new_ast);
  else
    INT_FATAL(this, "Unexpected case in InterfaceSymbol::replaceChild");
}

void InterfaceSymbol::printDocs(std::ostream* file, unsigned int tabs) {
  if (this->noDocGen()) return;
  INT_FATAL(this, "printDocs is to be implemented");
}


/////////////////////////////////////////////////////////////////////////////
// IfcConstraint
//

IfcConstraint* IfcConstraint::build(const char* name,
                                    CallExpr* actuals) {
  IfcConstraint* impl = new IfcConstraint(new UnresolvedSymExpr(name));
  for_alist(actual, actuals->argList)
    impl->consActuals.insertAtTail(actual->remove());
  return impl;
}

IfcConstraint::IfcConstraint(Expr* iifc) :
  Expr(E_IfcConstraint), interfaceExpr(iifc), consActuals() {
  consActuals.parent = this;
  gIfcConstraints.add(this);
}

IfcConstraint* IfcConstraint::copyInner(SymbolMap* map) {
  IfcConstraint* result = new IfcConstraint(COPY_INT(interfaceExpr));
  for_alist(actual, consActuals)
    result->consActuals.insertAtTail(COPY_INT(actual));
  return result;
}

void IfcConstraint::verify() {
  Expr::verify(E_IfcConstraint);
  verifyParent(interfaceExpr);
  verifyNotOnList(interfaceExpr);
  if (normalized)
    INT_ASSERT(isSymExpr(interfaceExpr));
  INT_ASSERT(consActuals.parent == this);
  INT_ASSERT(!resolved);
}

void IfcConstraint::accept(AstVisitor* visitor) {
  if (visitor->enterIfcConstraint(this)) {
    interfaceExpr->accept(visitor);
    for_alist(actual, consActuals)
      actual->accept(visitor);
    visitor->exitIfcConstraint(this);
  }
}

QualifiedType IfcConstraint::qualType() {
  return QualifiedType(dtBool, QUAL_VAL);
}

void IfcConstraint::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == interfaceExpr)
    interfaceExpr = newAst;
  else
    INT_FATAL(this, "Unexpected case in IfcConstraint::replaceChild");
}

Expr* IfcConstraint::getFirstExpr() {
  return interfaceExpr->getFirstExpr();
}

Expr* IfcConstraint::getNextExpr(Expr* expr) {
  if (expr == interfaceExpr && !consActuals.empty())
    return consActuals.head->getFirstExpr();

  return this;
}

void IfcConstraint::prettyPrint(std::ostream* o) {
  *o << "implements ";
  interfaceExpr->prettyPrint(o);
  *o << "(...)";
}


/////////////////////////////////////////////////////////////////////////////
// ImplementsStmt
//

Symbol* gDummyWitness = NULL;

ImplementsStmt* ImplementsStmt::build(const char* name, CallExpr* actuals,
                                      BlockStmt* body) {
 if (body == NULL) body = new BlockStmt();
 IfcConstraint* icon = IfcConstraint::build(name, actuals);
 return new ImplementsStmt(icon, body);
}

ImplementsStmt::ImplementsStmt(IfcConstraint* con, BlockStmt* body)
  : Stmt(E_ImplementsStmt), iConstraint(con), implBody(body) {
  gImplementsStmts.add(this);
}

ImplementsStmt* ImplementsStmt::copyInner(SymbolMap* map) {
  if (witnesses.n > 0) // this is a non-empty map
    INT_FATAL(this, "copying of witnesses is to be implemented");
  return new ImplementsStmt(COPY_INT(iConstraint),
                            COPY_INT(implBody));
}

static void verifyWitnesses(ImplementsStmt* istmt) {
  if (!normalized) return;
  IfcConstraint* icon = istmt->iConstraint;
  InterfaceSymbol* isym = icon->ifcSymbol();
  form_Map(SymbolMapElem, witn, istmt->witnesses) {
    INT_ASSERT(witn->key->defPoint->parentSymbol == isym);
    // witn->value can be defined anywhere
  }
}

void ImplementsStmt::verify() {
  Expr::verify(E_ImplementsStmt);
  verifyParent(iConstraint);
  verifyParent(implBody);
  verifyNotOnList(iConstraint);
  verifyNotOnList(implBody);
  verifyWitnesses(this);
  INT_ASSERT(!resolved);
}

void ImplementsStmt::accept(AstVisitor* visitor) {
  if (visitor->enterImplementsStmt(this)) {
    iConstraint->accept(visitor);
    implBody->accept(visitor);
    visitor->exitImplementsStmt(this);
  }
}

void ImplementsStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  // CG TODO: update 'witnesses' appropriately if there are any?
  if (witnesses.n > 0)
    USR_FATAL(this, "unsupported copying of this implements statement");

  if (oldAst == iConstraint)
    iConstraint = toIfcConstraint(newAst);
  else if (oldAst == implBody)
    implBody = toBlockStmt(newAst);
  else
    INT_FATAL(this, "Unexpected case in ImplementsStmt::replaceChild");
}

Expr* ImplementsStmt::getFirstExpr() {
  return iConstraint->getFirstExpr();
}

Expr* ImplementsStmt::getNextExpr(Expr* expr) {
  if (expr == iConstraint)
    return implBody;
  if (expr == implBody)
    return this;

  INT_FATAL(this, "Unexpected case in ImplementsStmt::getNextExpr");
  return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Support for normalize()


//
// Change proc cgFun(cgFormal: ?T)
// to     proc cgFun(cgFormal: T)
// plus add (DefExpr type T) to cgFun.constrainedTypes
// where we expect 'T' to be bound by interface constraint(s).
//
void introduceConstrainedTypes(FnSymbol* fn) {
  for_formals(formal, fn)
   if (BlockStmt* typeExpr = formal->typeExpr)
    if (DefExpr* def = toDefExpr(typeExpr->body.tail)) {
      INT_ASSERT(formal->type == dtUnknown); //fyi
      SET_LINENO(def);
      Symbol* queryT = def->sym;
      // introduce a ConstrainedType
      TypeSymbol* CT = ConstrainedType::build(queryT->name);
      fn->interfaceInfo->addConstrainedType(new DefExpr(CT));

      // replace queryT with CT throughout
      formal->type = CT->type;
      for_SymbolSymExprs(se, queryT)
        se->setSymbol(CT);

      // cleanup
      def->remove();
      if (! typeExpr->body.empty())
        USR_FATAL(typeExpr, "this formal's type query expression"
          " is currently not supported for constrained generic functions");
      typeExpr->remove();
    }
}

//
// Change proc cgFun(cgFormal: IFC)
// to     proc cgFun(cgFormal: t_IFC) where implements IFC(t_IFC)
// plus add (DefExpr type t_IFC) to cgFun.constrainedTypes
//
Type* desugarInterfaceAsType(ArgSymbol* arg, SymExpr* se,
                             InterfaceSymbol* isym) {
  FnSymbol* parentFn = toFnSymbol(arg->defPoint->parentSymbol);
  bool wasConstrainedGeneric = parentFn->isConstrainedGeneric();
  InterfaceInfo* ifcInfo = parentFn->ensureInterfaceInfo();

  // follows introduceConstrainedTypes()
  SET_LINENO(se);

  // introduce a ConstrainedType
  TypeSymbol* CT = ConstrainedType::build(astr("t_", isym->name));
  ifcInfo->addConstrainedType(new DefExpr(CT));

  // add an interface constraint
  IfcConstraint* icon = new IfcConstraint(new SymExpr(isym));
  icon->consActuals.insertAtTail(new SymExpr(CT));
  ifcInfo->addInterfaceConstraint(icon);

  // catch up with moveGlobalDeclarationsToModuleScope()
  if (!wasConstrainedGeneric)
    if (ModuleSymbol* mod = toModuleSymbol(parentFn->defPoint->parentSymbol))
      {
        FnSymbol* initFn = mod->initFn;
        // follows insertResolutionPoint()
        //
        // CG TODO: this will place the resolution point at the end of
        // the module, losing the position of parentFn among the module's
        // declarations. If this does not work, need to arrange for this
        // to be called before moveGlobalDeclarationsToModuleScope().
        //
        initFn->insertBeforeEpilogue(new CallExpr(PRIM_RESOLUTION_POINT,
                                                  parentFn));
        initFn->insertBeforeEpilogue(new CallExpr(PRIM_END_OF_STATEMENT));
      }

  return CT->type;
}


/////////////////////////////////////////////////////////////////////////////
// Wrapper Function for ImplementsStmt
//
// An ImplementsStmt wrapper function helps us find an ImplementsStmt
// for a IfcConstraint interface constraint.
//

const char* implementsStmtWrapperName(InterfaceSymbol* isym) {
  return astr("|", isym->name);
}

const char* interfaceNameForWrapperFn(FnSymbol* fn) {
  return astr(fn->name + 1);
}

ImplementsStmt* implementsStmtForWrapperFn(FnSymbol* wrapFn) {
  INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
  AList& body = wrapFn->body->body;
  if (ImplementsStmt* istm = toImplementsStmt(body.head))
    return istm;
  else
    INT_FATAL(wrapFn, "invalid implements wrapper function");

  return NULL; //dummy
}

FnSymbol* wrapperFnForImplementsStmt(ImplementsStmt* istm) {
  return toFnSymbol(istm->parentSymbol);
}

// Verify that the above functions work correctly.
static void verifyWrapImplementsStmt(ImplementsStmt* istm,
                                     InterfaceSymbol* isym,
                                     FnSymbol* wrapFn) {
  INT_ASSERT(wrapFn->name == implementsStmtWrapperName(isym));
  INT_ASSERT(interfaceNameForWrapperFn(wrapFn) == isym->name);
  INT_ASSERT(implementsStmtForWrapperFn(wrapFn) == istm);
  INT_ASSERT(wrapperFnForImplementsStmt(istm) == wrapFn);
}

static Symbol* computeConActualSym(Expr* conActual) {
  if (SymExpr* actualSE = toSymExpr(conActual)) {
    Symbol* actualSym = actualSE->symbol();

    if (actualSym->hasFlag(FLAG_TYPE_VARIABLE) &&
        ! actualSym->hasFlag(FLAG_TEMP)        )
      return actualSym;
  }

  USR_FATAL_CONT(conActual, "actual arguments in an 'implements' statement"
                 " currently must be type names");
  return gVoid;
}

// Stores each 'implements' statement in a wrapper function during normalize().
// Places this wrapper function where the implements statement used to be.
// Currently the wrapper function has a formal for each interface formal.
void wrapImplementsStatements() {
  forv_Vec(ImplementsStmt, istm, gImplementsStmts) {
    IfcConstraint*   icon = istm->iConstraint;
    InterfaceSymbol* isym = icon->ifcSymbol();

    // set up the wrapper function
    SET_LINENO(istm);
    FnSymbol* wrapFn = new FnSymbol(implementsStmtWrapperName(isym));
    wrapFn->addFlag(FLAG_IMPLEMENTS_WRAPPER);
    istm->insertBefore(new DefExpr(wrapFn));
    wrapFn->insertAtTail(istm->remove());

    // set up the formals
    // iterate in parallel over implements actuals and interface formals
    for (Expr *conActual = icon->consActuals.head,
              *ifcFormal = isym->ifcFormals.head;
         conActual != NULL;
         conActual = conActual->next,
         ifcFormal = ifcFormal->next)
    {
      Symbol* actualSym = computeConActualSym(conActual);
      Symbol* formalSym = toSymbol(toDefExpr(ifcFormal)->sym);
      SET_LINENO(conActual);

      //
      // CG TODO: handle the cases where 'conActual' requires resolving,
      // for example like this:
      // * During normalization, set the type of the wrapper function's formal
      //   ('wrapperFormal' below) to dtAny.
      // * Detect this case when resolving a call to the wrapper function,
      //   see constraintIsSatisfiedAtCallSite().
      // * There, replace wrapperFormal's type with the type of the actual
      //   of the implements statement, which by then should be resolved.
      //
      // No 'param' formals for now.
      //
      INT_ASSERT(isTypeSymbol(formalSym));

      ArgSymbol* wrapperFormal = new ArgSymbol(INTENT_BLANK, formalSym->name,
                                               actualSym->type);
      wrapperFormal->addFlag(FLAG_TYPE_VARIABLE);
      wrapFn->insertFormalAtTail(wrapperFormal);
    }

    if (fVerify) verifyWrapImplementsStmt(istm, isym, wrapFn);
  }
}
