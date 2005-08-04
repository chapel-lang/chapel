#include "insertLiteralTemps.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"


//
// This is a hack to handle some basic appends and prepends until
// where is implemented and # is overloaded in the sequence module.
//
static bool notSequenceType(Type* type) {
  return type == dtInteger || type == dtString;
}
static void handleBasicSequenceAppendPrependOperations(CallExpr* seqCat) {
  Type* leftType = seqCat->get(1)->typeInfo();
  Type* rightType = seqCat->get(2)->typeInfo();

  if (notSequenceType(leftType) && notSequenceType(rightType)) {
    INT_FATAL(seqCat, "Bad # operation");
  }

  if (notSequenceType(leftType)) {
    seqCat->replace(new CallExpr(
                     new MemberAccess(seqCat->get(2)->copy(),
                       new UnresolvedSymbol("_prepend")),
                     new AList<Expr>(seqCat->get(1)->copy())));
  } else if (notSequenceType(rightType)) {
    seqCat->replace(new CallExpr(
                     new MemberAccess(seqCat->get(1)->copy(),
                       new UnresolvedSymbol("_append")),
                     new AList<Expr>(seqCat->get(2)->copy())));
  }
}


static void replaceTupleLiteral(Tuple* tuple) {
  AList<Expr>* argList = new AList<Expr>();
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(new Variable(dtUnknown->symbol));
  }
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(expr->copy());
  }
  tuple->replace(
    new CallExpr(
      new Variable(
        new UnresolvedSymbol(
          glomstrings(2, "_tuple", intstring(tuple->exprs->length())))),
      argList));
}


static void replaceTupleLiteral2(Tuple* tuple) {
  static int uid = 1;
  char* name = glomstrings(2, "_tuple_temp_", intstring(uid++));
  AList<Expr>* argList = new AList<Expr>();
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(new Variable(dtUnknown->symbol));
  }
  for_alist(Expr, expr, tuple->exprs) {
    argList->insertAtTail(expr->copy());
  }
  Expr* init = new CallExpr(
                 new Variable(
                   new UnresolvedSymbol(
                     glomstrings(2, "_tuple", intstring(tuple->exprs->length())))),
                 argList);
  DefExpr* def = Symboltable::defineSingleVarDef(name,
                                                 dtUnknown,
                                                 init,
                                                 VAR_NORMAL,
                                                 VAR_VAR);
  tuple->getStmt()->insertBefore(new ExprStmt(def));
  Symbol* tmp = def->sym;
  int i = 1;
  for_alist(Expr, expr, tuple->exprs) {
    CallExpr* assignOp = new CallExpr(OP_GETSNORM, expr->copy(),
                                      new CallExpr(
                                        new Variable(tmp),
                                        new AList<Expr>(
                                          new IntLiteral(i))));
    tuple->getStmt()->insertAfter(new ExprStmt(assignOp));
    i++;
  }
  tuple->replace(new Variable(tmp));
}


static void createTupleBaseType(int size) {
  char *name = glomstrings(2, "_tuple", intstring(size));
  if (Symboltable::lookupInScope(name, commonModule->modScope)) {
    return;
  }
  SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  Symboltable::pushScope(SCOPE_CLASS);
  AList<Stmt>* decls = new AList<Stmt>();
  Vec<TypeSymbol*> types;
  Vec<VarSymbol*> fields;
  for (int i = 1; i <= size; i++) {
    VariableType* variableType = new VariableType(getMetaType(0));
    TypeSymbol* typeSymbol =
      new TypeSymbol(glomstrings(2, "_t", intstring(i)), variableType);
    variableType->addSymbol(typeSymbol);
    decls->insertAtTail(new ExprStmt(new DefExpr(typeSymbol)));
    types.add(typeSymbol);
  }
  for (int i = 1; i <= size; i++) {
    DefExpr* def = 
      Symboltable::defineSingleVarDef(glomstrings(2, "_f", intstring(i)),
                                      types.v[i-1]->definition,
                                      NULL,
                                      VAR_NORMAL,
                                      VAR_VAR);
    decls->insertAtTail(new ExprStmt(def));
    VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);
    fields.add(var);
  }
  for (int i = 1; i <= size; i++) {
    FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("this"));
    ParamSymbol* paramSymbol =
      new ParamSymbol(PARAM_PARAMETER, "index", dtInteger);
    AList<DefExpr>* formals = new AList<DefExpr>(new DefExpr(paramSymbol));
    Expr* where = new CallExpr(OP_EQUAL, 
                                  new Variable(paramSymbol),
                                  new IntLiteral(i));
    Symboltable::continueFnDef(fn, formals, dtUnknown, true, where);
    AList<Stmt>* body = new AList<Stmt>(new ReturnStmt(new Variable(fields.v[i-1])));
    Symboltable::finishFnDef(fn, new BlockStmt(body));
    decls->insertAtTail(new ExprStmt(new DefExpr(fn)));
  }
  SymScope *scope = Symboltable::popScope();
  DefExpr* def = Symboltable::defineStructType(name, new RecordType(), scope, decls);
  commonModule->stmts->insertAtHead(new ExprStmt(def));
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
  ParamSymbol* paramSymbol =
    new ParamSymbol(PARAM_BLANK, "val", dtUnknown);
  AList<DefExpr>* formals =
    new AList<DefExpr>(new DefExpr(paramSymbol,
                                   NULL,
                                   new Variable(new UnresolvedSymbol(name))));
  Symboltable::continueFnDef(fn, formals, dtUnknown, false, NULL);
  AList<Expr>* args = new AList<Expr>();
  args->insertAtTail(new StringLiteral(copystring("(")));
  for (int i = 1; i <= size; i++) {
    if (i != 1) {
      args->insertAtTail(new StringLiteral(copystring(", ")));
    }
    args->insertAtTail(new MemberAccess(new Variable(new UnresolvedSymbol("val")),
                                        new UnresolvedSymbol(glomstrings(2, "_f", intstring(i)))));
  }
  args->insertAtTail(new StringLiteral(copystring(")")));
  Stmt* writeBody =
    new ExprStmt(
      new CallExpr(
        new Variable(
          new UnresolvedSymbol("write")),
        args));
  AList<Stmt>* body = new AList<Stmt>(writeBody);
  Symboltable::finishFnDef(fn, new BlockStmt(body));
  commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(fn)));
  Symboltable::setCurrentScope(saveScope);
}


void InsertLiteralTemps::postProcessExpr(Expr* expr) {
  if (CallExpr* seqCat = dynamic_cast<CallExpr*>(expr)) {
    if (seqCat->opTag == OP_SEQCAT) {
      handleBasicSequenceAppendPrependOperations(seqCat);
    }
  } else if (Tuple* literal = dynamic_cast<Tuple*>(expr)) {
    createTupleBaseType(literal->exprs->length());
    if (CallExpr* call = dynamic_cast<CallExpr*>(literal->parentExpr)) {
      if (call->get(1) == literal) {
        replaceTupleLiteral2(literal); // Handle destructuring
        return;
      }
    }
    replaceTupleLiteral(literal);
  } else if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    if (CallExpr* parenOpExpr = dynamic_cast<CallExpr*>(def->exprType)) {
      if (Variable* variable = dynamic_cast<Variable*>(parenOpExpr->baseExpr)) {
        if (!strncmp(variable->var->name, "_tuple", 6)) {
          createTupleBaseType(parenOpExpr->argList->length());
        }
      }
    }
  }
}
