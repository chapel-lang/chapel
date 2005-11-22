#include "normalizeParsedAST.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"
#include "runtime.h"


// Move anonymous record definition into separate statement
// Or move anonymous forall expression iterator into separate statement
static void normalize_anonymous_record_or_forall_expression(DefExpr* def) {
  if ((!strncmp("_anon_record", def->sym->name, 12)) ||
      (!strncmp("_forallexpr", def->sym->name, 11))) {
    Stmt* stmt = def->parentStmt;
    def->replace(new SymExpr(def->sym));
    stmt->insertBefore(new ExprStmt(def));
  }
}


// Destructure ForLoopStmt indices
static void destructure_indices(ForLoopStmt* stmt) {
  if (stmt->indices->length() > 1) {
    VarSymbol* indextmp = new VarSymbol("_index_temp");
    int i = 1;
    for_alist(DefExpr, indexDef, stmt->indices) {
      indexDef->remove();
      indexDef->init = new CallExpr(indextmp, new_IntLiteral(i++));
      stmt->innerStmt->insertAtHead(new ExprStmt(indexDef));
    }
    stmt->indices->insertAtTail(new DefExpr(indextmp));
  }
}


// Destructure LHS tuples
static void destructure_tuple(CallExpr* call, Vec<BaseAST*>* asts) {
  static int uid = 1;
  char* name = stringcat("_tuple_tmp_", intstring(uid++));
  DefExpr* def = new DefExpr(new VarSymbol(name));
  call->parentStmt->insertBefore(new ExprStmt(def));
  int start = call->argList->length()/2;
  int i = 0;
  for_alist(Expr, expr, call->argList) {
    if (++i <= start)
      continue;
    Stmt* stmt = new ExprStmt(
                   new CallExpr(OP_GETSNORM, expr->copy(),
                     new CallExpr(name, new_IntLiteral(i-start))));
    collect_asts(asts, stmt);
    call->parentStmt->insertAfter(stmt);
  }
  call->replace(new SymExpr(def->sym));
}


// Construct tuple class of particular size
static void construct_tuple_type(int size) {
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
    if (no_infer) {
      ArgSymbol* arg = new ArgSymbol(INTENT_PARAM, "index", dtInteger);
      fn->formals = new AList<DefExpr>(new DefExpr(arg));
      fn->whereExpr = new CallExpr(OP_EQUAL, arg, new_IntLiteral(i));
    } else {
      ArgSymbol* arg = new ArgSymbol(INTENT_PARAM, "index", new_LiteralType(new_IntSymbol(i)));
      fn->formals = new AList<DefExpr>(new DefExpr(arg));
    }

    fn->retRef = true;
    fn->body = new BlockStmt(new ReturnStmt(new SymExpr(fields.v[i-1]->name)));
    DefExpr* def = new DefExpr(fn);
    if (no_infer)
      def->exprType = new SymExpr(types.v[i-1]->symbol);
    decls->insertAtTail(new ExprStmt(def));
  }

  // Build tuple
  ClassType* tupleType = new ClassType(CLASS_RECORD);
  TypeSymbol* tupleSym = new TypeSymbol(name, tupleType);
  tupleType->addSymbol(tupleSym);
  tupleType->addDeclarations(decls);
  commonModule->stmts->insertAtHead(new ExprStmt(new DefExpr(tupleSym)));

  // Build write function
  FnSymbol* fwriteFn = new FnSymbol("fwrite");
  TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
  ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->definition);
  ArgSymbol* fwriteArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
  fwriteFn->formals = new AList<DefExpr>(new DefExpr(fileArg), new DefExpr(fwriteArg));
  AList<Expr>* actuals = new AList<Expr>();
  actuals->insertAtTail(new_StringLiteral(stringcpy("(")));
  for (int i = 1; i <= size; i++) {
    if (i != 1)
      actuals->insertAtTail(new_StringLiteral(stringcpy(", ")));
    actuals->insertAtTail(
      new MemberAccess(
          new UnresolvedSymbol("val"),
          stringcat("_f", intstring(i))));
  }
  actuals->insertAtTail(new_StringLiteral(stringcpy(")")));
  Expr* fwriteCall = new CallExpr("fwrite", new SymExpr(fileArg), actuals);
  fwriteFn->body = new BlockStmt(new ExprStmt(fwriteCall));
  commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(fwriteFn)));

  // Build htuple = tuple function
  {
    FnSymbol* assignFn = new FnSymbol("=");
    ArgSymbol* htupleArg = 
      new ArgSymbol(INTENT_BLANK, "_htuple", htuple->definition);
    ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
    assignFn->formals = new AList<DefExpr>(new DefExpr(htupleArg),
                                           new DefExpr(tupleArg));
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


void normalizeParsedAST(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      normalize_anonymous_record_or_forall_expression(a);
    } else if (ForLoopStmt* a = dynamic_cast<ForLoopStmt*>(ast)) {
      destructure_indices(a);
    } else if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      SymExpr* base = dynamic_cast<SymExpr*>(a->baseExpr);
      CallExpr* parent = dynamic_cast<CallExpr*>(a->parentExpr);
      if (base && !strncmp(base->var->name, "_tuple", 6)) {
        construct_tuple_type(atoi(base->var->name+6));
        if (parent && OP_ISASSIGNOP(parent->opTag) && parent->get(1) == a)
          destructure_tuple(a, &asts);
      }
    }
  }
}
