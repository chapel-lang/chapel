/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "interfaces.h"

#include "AstVisitor.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"

/////////////////////////////////////////////////////////////////////////////
// InterfaceSymbol
//

//
// For now an interface formal is a VarSymbol, not a ArgSymbol.
// This is to avoid dealing with compiler's assumptions about ArgSymbol.
//

static Symbol* isInterfaceFormalSymbol(Symbol* sym) {
  if (TypeSymbol* var = toTypeSymbol(sym))
    return var;
  // future work: also allow params
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
  InterfaceSymbol* ifc = new InterfaceSymbol(name, body);

  for_alist(formal, formals->argList) {
    ifc->ifcFormals.insertAtTail(formal->remove());
    INT_ASSERT(isInterfaceFormalDecl(formal)); // from parser
  }

  // at least 1 formal is required
  if (ifc->ifcFormals.empty())
    USR_FATAL_CONT(ifc, "interface %s has no formal arguments\n"
                   "  at least one formal argument is required", ifc->name);

  // ifcBody can be empty; check that the content is appropriate
  for_alist(expr, ifc->ifcBody->body)
    if (FnSymbol* fn = toInterfaceFunDecl(expr))
      ; //OK
    else
      USR_FATAL_CONT(expr, "not a 'proc' or 'iter' declaration\n"
                     "  only function prototype declarations"
                     " are currently allowed in an interface");

  return new DefExpr(ifc);
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

// need this?
void InterfaceSymbol::verifyInterfaceFormal(Symbol* sym) {
  INT_ASSERT(isInterfaceFormalSymbol(sym));
}


/////////////////////////////////////////////////////////////////////////////
// ImplementsExpr
//

ImplementsExpr* ImplementsExpr::build(const char* name,
                                      CallExpr* actuals) {
  ImplementsExpr* impl = new ImplementsExpr(new UnresolvedSymExpr(name));
  for_alist(actual, actuals->argList)
    impl->implActuals.insertAtTail(actual);
  return impl;
}

ImplementsExpr::ImplementsExpr(Expr* iifc) :
  Expr(E_ImplementsExpr), implInterface(iifc), implActuals() {
  implActuals.parent = this;
  gImplementsExprs.add(this);
}

ImplementsExpr* ImplementsExpr::copyInner(SymbolMap* map) {
  ImplementsExpr* result = new ImplementsExpr(COPY_INT(implInterface));
  for_alist(actual, implActuals)
    result->implActuals.insertAtTail(COPY_INT(actual));
  return result;
}

void ImplementsExpr::verify() {
  Expr::verify(E_ImplementsExpr);
  verifyParent(implInterface);
  verifyNotOnList(implInterface);
  if (normalized)
    INT_ASSERT(isSymExpr(implInterface));
  INT_ASSERT(implActuals.parent == this);
  INT_ASSERT(!resolved);
}

void ImplementsExpr::accept(AstVisitor* visitor) {
  if (visitor->enterImplementsExpr(this)) {
    implInterface->accept(visitor);
    for_alist(actual, implActuals)
      actual->accept(visitor);
    visitor->exitImplementsExpr(this);
  }
}

QualifiedType ImplementsExpr::qualType() {
  return QualifiedType(dtBool, QUAL_VAL);
}

void ImplementsExpr::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == implInterface)
    implInterface = newAst;
  else
    INT_FATAL(this, "Unexpected case in ImplementsExpr::replaceChild");
}

Expr* ImplementsExpr::getFirstExpr() {
  return implInterface->getFirstExpr();
}

Expr* ImplementsExpr::getNextExpr(Expr* expr) {
  if (expr == implInterface && !implActuals.empty())
    return implActuals.head->getFirstExpr();

  return this;
}

void ImplementsExpr::prettyPrint(std::ostream* o) {
  *o << "implements ";
  implInterface->prettyPrint(o);
  *o << "(...)";
}


/////////////////////////////////////////////////////////////////////////////
// ImplementsStmt
//

Symbol* gDummyWitness;

ImplementsStmt* ImplementsStmt::build(const char* name, CallExpr* actuals,
                                      BlockStmt* body) {
 if (body == NULL) body = new BlockStmt();
 ImplementsExpr* ie = ImplementsExpr::build(name, actuals);
 return new ImplementsStmt(ie, body);
}

ImplementsStmt::ImplementsStmt(ImplementsExpr* iimplExpr, BlockStmt* iimplBody)
  : Stmt(E_ImplementsStmt), implExpr(iimplExpr), implBody(iimplBody) {
  gImplementsStmts.add(this);
}

ImplementsStmt* ImplementsStmt::copyInner(SymbolMap* map) {
  if (witnesses.n > 0) // this is a non-empty map
    INT_FATAL(this, "copying of witnesses is to be implemented");
  return new ImplementsStmt(COPY_INT(implExpr),
                            COPY_INT(implBody));
}

static void verifyWitnesses(ImplementsStmt* istmt) {
  if (!normalized) return;
  ImplementsExpr* ie = istmt->implExpr;
  InterfaceSymbol* isym = ie->implInterfaceSym();
  form_Map(SymbolMapElem, witn, istmt->witnesses) {
    INT_ASSERT(witn->key->defPoint->parentSymbol == isym);
    // witn->value can be defined anywhere
  }
}

void ImplementsStmt::verify() {
  Expr::verify(E_ImplementsStmt);
  verifyParent(implExpr);
  verifyParent(implBody);
  verifyNotOnList(implExpr);
  verifyNotOnList(implBody);
  verifyWitnesses(this);
  INT_ASSERT(!resolved);
}

void ImplementsStmt::accept(AstVisitor* visitor) {
  if (visitor->enterImplementsStmt(this)) {
    implExpr->accept(visitor);
    implBody->accept(visitor);
    visitor->exitImplementsStmt(this);
  }
}

void ImplementsStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  // TODO update 'witnesses'
  if (oldAst == implExpr)
    implExpr = toImplementsExpr(newAst);
  else if (oldAst == implBody)
    implBody = toBlockStmt(newAst);
  else
    INT_FATAL(this, "Unexpected case in ImplementsStmt::replaceChild");
}

Expr* ImplementsStmt::getFirstExpr() {
  return implExpr->getFirstExpr();
}

Expr* ImplementsStmt::getNextExpr(Expr* expr) {
  if (expr == implExpr)
    return implBody;
  if (expr == implBody)
    return this;

  INT_FATAL(this, "Unexpected case in ImplementsStmt::getNextExpr");
  return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Wrapper Function for ImplementsStmt
//
// An ImplementsStmt wrapper function helps us find an ImplementsStmt
// for a ImplementsExpr interface constraint.
//

const char* ImplementsStmt::wrapperName(InterfaceSymbol* sym) {
  return astr("|", sym->name);
}

// Stores each 'implements' statement in a wrapper function during normalize().
// Places this wrapper function where the implements statement used to be.
// Currently the wrapper function has a formal for each interface formal.
void wrapImplementsStatements() {
  forv_Vec(ImplementsStmt, istm, gImplementsStmts) {
    ImplementsExpr* ie = istm->implExpr;
    InterfaceSymbol* isym = ie->implInterfaceSym();

    // set up the wrapper function
    SET_LINENO(istm);
    FnSymbol* wrapFn = new FnSymbol(ImplementsStmt::wrapperName(isym));
    wrapFn->addFlag(FLAG_IMPLEMENTS_WRAPPER);
    wrapFn->where = new BlockStmt();
    istm->insertBefore(new DefExpr(wrapFn));
    wrapFn->insertAtTail(istm->remove());

    // set up the formals
    // iterate in parallel over implements actuals and interface formals
    CallExpr* where = NULL;
    Expr *implExpr = ie->implActuals.head, *formalExpr = isym->ifcFormals.head;
    while (implExpr != NULL) {
      Symbol* implSym = toSymExpr(implExpr)->symbol();
      Symbol* formalSym = toSymbol(toDefExpr(formalExpr)->sym);
      SET_LINENO(implExpr);
      ArgSymbol* arg = NULL;
      // also need to account for 'param' formals
      if (isTypeSymbol(formalSym)) {
        // currently type formals have the default intent and the type 'any'
        arg = new ArgSymbol(INTENT_BLANK, formalSym->name, dtAny);
        arg->addFlag(FLAG_TYPE_VARIABLE);
      }
      wrapFn->insertFormalAtTail(arg);
      CallExpr* cond = new CallExpr("==", arg, implSym);
      if (where == NULL) where = cond;
      else where = new CallExpr("&&", where, cond);

      implExpr = implExpr->next, formalExpr = formalExpr->next;
    }

    // make this function applicable only when istm is
    wrapFn->where->insertAtTail(where);
  }
}

ImplementsStmt* ImplementsStmt::extractImplementsStmtFromWrapper(FnSymbol*
                                                                   wrapFn) {
  INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
  AList& body = wrapFn->body->body;
  if (ImplementsStmt* result = toImplementsStmt(body.head))
    return result;
  else
    INT_FATAL(wrapFn, "invalid implements wrapper function");

  return NULL; //dummy
}
