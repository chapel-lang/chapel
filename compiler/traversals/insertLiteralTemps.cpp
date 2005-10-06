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
                     seqCat->get(1)->copy()));
  } else if (notSequenceType(rightType)) {
    seqCat->replace(new CallExpr(
                     new MemberAccess(seqCat->get(1)->copy(),
                       new UnresolvedSymbol("_append")),
                     seqCat->get(2)->copy()));
  }
}

static void destructureTuple(CallExpr* tuple) {
  static int uid = 1;
  char* name = stringcat("_tuple_tmp_", intstring(uid++));
  DefExpr* def = new DefExpr(new VarSymbol(name));
  tuple->parentStmt->insertBefore(new ExprStmt(def));
  int i = 1;
  for_alist(Expr, expr, tuple->argList) {
    if (NamedExpr* namedExpr = dynamic_cast<NamedExpr*>(expr)) {
      tuple->parentStmt->insertAfter(
        new ExprStmt(
          new CallExpr(OP_GETSNORM, namedExpr->actual->copy(),
            new CallExpr(name,
              new_IntLiteral(i++)))));
    }
    TRAVERSE(tuple->parentStmt->next, new InsertLiteralTemps(), true);
  }
  tuple->replace(new SymExpr(def->sym));
}

static void createTupleBaseType(int size) {
  char *name = stringcat("_tuple", intstring(size));

  if (Symboltable::lookupInScope(name, commonModule->modScope))
    return;

  AList<Stmt>* decls = new AList<Stmt>();

  // Build type declarations
  Vec<Type*> types;
  for (int i = 1; i <= size; i++) {
    char* typeName = stringcat("_t", intstring(i));
    VariableType* type = new VariableType(getMetaType(0));
    TypeSymbol* typeSymbol = new TypeSymbol(typeName, type);
    type->addSymbol(typeSymbol);
    decls->insertAtTail(new ExprStmt(new DefExpr(typeSymbol)));
    types.add(type);
  }

  // Build field declarations
  Vec<VarSymbol*> fields;
  for (int i = 1; i <= size; i++) {
    char* fieldName = stringcat("_f", intstring(i));
    VarSymbol* field = new VarSymbol(fieldName, types.v[i-1]);
    decls->insertAtTail(new ExprStmt(new DefExpr(field)));
    fields.add(field);
  }

  // Build this methods
  for (int i = 1; i <= size; i++) {
    FnSymbol* fn = new FnSymbol("this");
    ArgSymbol* arg = new ArgSymbol(INTENT_PARAM, "index", dtInteger);
    fn->formals = new AList<DefExpr>(new DefExpr(arg));
    fn->whereExpr = new CallExpr(OP_EQUAL, arg, new_IntLiteral(i));
    fn->retType = dtUnknown;
    fn->retRef = true;
    fn->body = new BlockStmt(new ReturnStmt(new SymExpr(fields.v[i-1])));
    decls->insertAtTail(new ExprStmt(new DefExpr(fn)));
  }

  // Build tuple
  ClassType* tupleType = new ClassType(CLASS_RECORD);
  TypeSymbol* tupleSym = new TypeSymbol(name, tupleType);
  tupleType->addSymbol(tupleSym);
  tupleType->addDeclarations(decls);
  commonModule->stmts->insertAtHead(new ExprStmt(new DefExpr(tupleSym)));

  // Build write function
  FnSymbol* writeFn = new FnSymbol("write");
  ArgSymbol* writeArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
  writeFn->formals = new AList<DefExpr>(new DefExpr(writeArg));
  writeFn->retType = dtUnknown;
  AList<Expr>* actuals = new AList<Expr>();
  actuals->insertAtTail(new_StringLiteral(stringcpy("(")));
  for (int i = 1; i <= size; i++) {
    if (i != 1)
      actuals->insertAtTail(new_StringLiteral(stringcpy(", ")));
    actuals->insertAtTail(
      new MemberAccess(
        new SymExpr(
          new UnresolvedSymbol("val")),
        new UnresolvedSymbol(stringcat("_f", intstring(i)))));
  }
  actuals->insertAtTail(new_StringLiteral(stringcpy(")")));
  writeFn->body = new BlockStmt(new ExprStmt(new CallExpr("write", actuals)));
  commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(writeFn)));

  // Build htuple = tuple function
  {
    FnSymbol* assignFn = new FnSymbol("=");
    TypeSymbol* htuple =
      dynamic_cast<TypeSymbol*>(Symboltable::lookup("_htuple"));
    ArgSymbol* htupleArg = 
      new ArgSymbol(INTENT_BLANK, "_htuple", htuple->definition);
    ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
    assignFn->formals = new AList<DefExpr>(new DefExpr(htupleArg),
                                           new DefExpr(tupleArg));
    assignFn->retType = dtUnknown;
    assignFn->body = new BlockStmt();
    for (int i = 1; i <= size; i++) {
      assignFn->insertAtTail(
        new ExprStmt(
          new CallExpr(OP_GETSNORM,
            new CallExpr(htupleArg, new_IntLiteral(i)),
            new CallExpr(tupleArg, new_IntLiteral(i)))));
    }
    assignFn->insertAtTail(new ReturnStmt(new SymExpr(htupleArg)));
    commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(assignFn)));
  }

  // Build tuple = _ function
//   {
//     FnSymbol* assignFn = new FnSymbol("=");
//     ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "tuple", tupleType);
//     ArgSymbol* secondArg = new ArgSymbol(INTENT_BLANK, "val", dtUnknown);
//     assignFn->formals = new AList<DefExpr>(new DefExpr(tupleArg),
//                                            new DefExpr(secondArg));
//     assignFn->retType = dtUnknown;
//     assignFn->body = new BlockStmt();
//     for (int i = 1; i <= size; i++) {
//       assignFn->insertAtTail(
//         new ExprStmt(
//           new CallExpr(OP_GETSNORM,
//             new CallExpr(tupleArg, new_IntLiteral(i)),
//             new CallExpr(secondArg, new_IntLiteral(i)))));
//     }
//     assignFn->insertAtTail(new ReturnStmt(new SymExpr(tupleArg)));
//     commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(assignFn)));
//   }

}

void InsertLiteralTemps::postProcessStmt(Stmt* stmt) {
  if (ForLoopStmt* loop = dynamic_cast<ForLoopStmt*>(stmt)) {
    if (loop->indices->length() > 1) {
      VarSymbol* indextmp = new VarSymbol("_index_temp");
      int i = 1;
      for_alist(DefExpr, indexDef, loop->indices) {
        indexDef->remove();
        indexDef->init = new CallExpr(indextmp, new_IntLiteral(i++));
        loop->innerStmt->insertAtHead(new ExprStmt(indexDef));
      }
      loop->indices->insertAtTail(new DefExpr(indextmp));
    }
  }
}

void InsertLiteralTemps::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->opTag == OP_SEQCAT) {
      handleBasicSequenceAppendPrependOperations(call);
    }
    if (SymExpr* variable = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (!strncmp(variable->var->name, "_tuple", 6)) {
        createTupleBaseType(call->argList->length());
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
