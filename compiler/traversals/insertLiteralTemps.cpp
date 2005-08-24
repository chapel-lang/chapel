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


static void destructureTuple(CallExpr* tuple) {
  static int uid = 1;
  char* name = glomstrings(2, "_tuple_tmp_", intstring(uid++));
  DefExpr* def = new DefExpr(new VarSymbol(name));
  tuple->parentStmt->insertBefore(new ExprStmt(def));
  int i = 1;
  for_alist(Expr, expr, tuple->argList) {
    if (NamedExpr* namedExpr = dynamic_cast<NamedExpr*>(expr)) {
      tuple->parentStmt->insertAfter(
        new ExprStmt(
          new CallExpr(OP_GETSNORM, namedExpr->actual->copy(),
            new CallExpr(name,
              new IntLiteral(i++)))));
    }
    TRAVERSE(tuple->parentStmt->next, new InsertLiteralTemps(), true);
  }
  tuple->replace(new SymExpr(def->sym));
}


static void createTupleBaseType(int size) {
  char *name = glomstrings(2, "_tuple", intstring(size));
  if (Symboltable::lookupInScope(name, commonModule->modScope)) {
    return;
  }
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
    DefExpr* def = new DefExpr(new VarSymbol(glomstrings(2, "_f", intstring(i)),
                                             types.v[i-1]->definition));
    decls->insertAtTail(new ExprStmt(def));
    VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);
    fields.add(var);
  }
  for (int i = 1; i <= size; i++) {
    FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("this"));
    ArgSymbol* argSymbol =
      new ArgSymbol(INTENT_PARAM, "index", dtInteger);
    AList<DefExpr>* formals = new AList<DefExpr>(new DefExpr(argSymbol));
    Expr* where = new CallExpr(OP_EQUAL, 
                               new SymExpr(argSymbol),
                               new IntLiteral(i));
    Symboltable::continueFnDef(fn, formals, dtUnknown, true, where);
    AList<Stmt>* body = new AList<Stmt>(new ReturnStmt(new SymExpr(fields.v[i-1])));
    Symboltable::finishFnDef(fn, new BlockStmt(body));
    decls->insertAtTail(new ExprStmt(new DefExpr(fn)));
  }
  DefExpr* def = Symboltable::defineStructType(name, new ClassType(CLASS_RECORD), decls);
  commonModule->stmts->insertAtHead(new ExprStmt(def));
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
  ArgSymbol* argSymbol =
    new ArgSymbol(INTENT_BLANK, "val", dtUnknown);
  AList<DefExpr>* formals =
    new AList<DefExpr>(new DefExpr(argSymbol,
                                   NULL,
                                   new SymExpr(new UnresolvedSymbol(name))));
  Symboltable::continueFnDef(fn, formals, dtUnknown, false, NULL);
  AList<Expr>* args = new AList<Expr>();
  args->insertAtTail(new StringLiteral(copystring("(")));
  for (int i = 1; i <= size; i++) {
    if (i != 1) {
      args->insertAtTail(new StringLiteral(copystring(", ")));
    }
    args->insertAtTail(new MemberAccess(new SymExpr(new UnresolvedSymbol("val")),
                                        new UnresolvedSymbol(glomstrings(2, "_f", intstring(i)))));
  }
  args->insertAtTail(new StringLiteral(copystring(")")));
  Stmt* writeBody = new ExprStmt(new CallExpr("write", args));
  AList<Stmt>* body = new AList<Stmt>(writeBody);
  Symboltable::finishFnDef(fn, new BlockStmt(body));
  commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(fn)));
}


void InsertLiteralTemps::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->opTag == OP_SEQCAT) {
      handleBasicSequenceAppendPrependOperations(call);
    }
    if (SymExpr* variable = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (!strncmp(variable->var->name, "_tuple", 6)) {
        createTupleBaseType(call->argList->length());
        if (call->argList->length() / 2 > 1) {
          createTupleBaseType(call->argList->length() / 2); // while
                                                            // we
                                                            // instantiated
                                                            // on
                                                            // dtUnknown
        }
        if (call->parentExpr) {
          if (CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr)) {
            if (OP_ISASSIGNOP(parent->opTag) && parent->get(1) == call) {
              destructureTuple(call); // Handle destructuring
              return;
            }
          }
        }
      }
    }
  }
}
