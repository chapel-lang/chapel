#include "buildDefaultFunctions.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"
#include "filesToAST.h"
#include "runtime.h"


static void build_chpl_main(void);
static void build_record_equality_function(ClassType* ct);
static void build_record_inequality_function(ClassType* ct);
static void build_record_assignment_function(ClassType* ct);
static void buildDefaultIOFunctions(Type* type);
static void construct_tuple_type(int size);

// function_exists returns true iff
//  function's name matches name
//  function's number of formals matches numFormals if not -1
//  function's typeBinding matches typeBinding if not NULL
//  function's first formal's type's name matches formalTypeName1 if not NULL
//  function's second formal's type's name matches formalTypeName2 if not NULL

static bool type_name_match(char* name, Symbol* sym) {
  if (!strcmp(name, sym->type->symbol->name))
    return true;
  SymExpr* symExpr = dynamic_cast<SymExpr*>(sym->defPoint->exprType);
  if (symExpr && !strcmp(name, symExpr->var->name))
    return true;
  return false;
}

static FnSymbol* function_exists(char* name,
                                 int numFormals = -1,
                                 Type* typeBinding = NULL,
                                 char* formalTypeName1 = NULL,
                                 char* formalTypeName2 = NULL) {
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    if (strcmp(name, fn->name))
      continue;

    if (numFormals != -1)
      if (numFormals != fn->formals->length())
        continue;

    if (typeBinding)
      if (typeBinding != fn->typeBinding->definition)
        continue;

    if (formalTypeName1)
      if (!type_name_match(formalTypeName1, fn->formals->get(1)->sym))
        continue;

    if (formalTypeName2)
      if (!type_name_match(formalTypeName2, fn->formals->get(2)->sym))
        continue;

    return fn;
  }
  return NULL;
}


void build_default_functions(void) {
  build_chpl_main();

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      SymExpr* base = dynamic_cast<SymExpr*>(a->baseExpr);
      if (base && !strncmp(base->var->name, "_construct__tuple", 17)) {
        construct_tuple_type(atoi(base->var->name+17));
      }
    }
  }
  if (!fnostdincs_but_file && !fnostdincs)
    build(tupleModule);

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(ast)) {
      buildDefaultIOFunctions(type->definition);
      if (ClassType* ct = dynamic_cast<ClassType*>(type->definition)) {
        if (ct->classTag == CLASS_RECORD || ct->classTag == CLASS_VALUECLASS) {
          build_record_equality_function(ct);
          build_record_inequality_function(ct);
          build_record_assignment_function(ct);
        }
      }
    }
  }
}


static FnSymbol* chpl_main_exists(void) {
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  FnSymbol* match = NULL;
  forv_Vec(FnSymbol, fn, fns) {
    if (!strcmp("main", fn->name) && !fn->formals->length()) {
      if (!match) {
        match = fn;
      } else {
        USR_FATAL(fn, "main multiply defined -- first occurrence at %s",
                  match->stringLoc());
      }
    }
  }
  return match;
}


static void build_chpl_main(void) {
  chpl_main = chpl_main_exists();
  if (!chpl_main) {
    if (userModules.n == 1) {
      chpl_main = new FnSymbol("main", NULL, new AList<DefExpr>(), dtVoid);
      userModules.v[0]->stmts->insertAtTail(new DefExpr(chpl_main));
      build(chpl_main);
    } else
      USR_FATAL("Code defines multiple modules but no main function.");
  } else if (chpl_main->getModule() != chpl_main->defPoint->parentSymbol)
    USR_FATAL(chpl_main, "Main function must be defined at module scope");
  currentLineno = -1;
  chpl_main->insertAtHead(new CallExpr(chpl_main->getModule()->initFn));
  chpl_main->insertAtHead(new CallExpr(prelude->initFn));
  chpl_main->insertAtHead(new CallExpr(compilerModule->initFn));
}


static void build_record_equality_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("==");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  args->insertAtTail(new DefExpr(arg2));
  fn->formals = args;
  fn->retType = dtBoolean;
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, ct->fields) {
    Expr* left = new CallExpr(tmp->name, methodToken, arg1);
    Expr* right = new CallExpr(tmp->name, methodToken, arg2);
    cond = (cond)
      ? new CallExpr("and", cond, new CallExpr("==", left, right))
      : new CallExpr("==", left, right);
  }
  fn->body = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
}


static void build_record_inequality_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("!=");

  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  args->insertAtTail(new DefExpr(arg2));
  fn->formals = args;
  fn->retType = dtBoolean;
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, ct->fields) {
    Expr* left = new CallExpr(tmp->name, methodToken, arg1);
    Expr* right = new CallExpr(tmp->name, methodToken, arg2);
    cond = (cond)
      ? new CallExpr("or", cond, new CallExpr("!=", left, right))
      : new CallExpr("!=", left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  fn->body = retStmt;
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
}


static void build_record_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, NULL, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* _arg1 = 
    f_equal_method ? new ArgSymbol(INTENT_REF, "this", ct)
    : new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(_arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtUnknown);
  args->insertAtTail(new DefExpr(arg2));
  fn->formals = args;
  fn->retType = dtUnknown;
  AList<Stmt>* body = new AList<Stmt>();
  forv_Vec(Symbol, tmp, ct->fields)
    body->insertAtTail(new CallExpr(tmp->name, methodToken, _arg1, setterToken,
                                    new CallExpr(tmp->name, methodToken, arg2)));
  body->insertAtTail(new ReturnStmt(_arg1));
  BlockStmt* block_stmt = new BlockStmt(body);
  fn->body = block_stmt;
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  if (f_equal_method) {
    ct->methods.add(fn);
    fn->method_type = PRIMARY_METHOD;
    fn->typeBinding = ct->symbol;
    fn->_this = _arg1;
  }
  if (no_infer)
    fn->retType = ct;
  build(fn);
}


void buildDefaultIOFunctions(Type* type) {
  if (fnostdincs)
    return;
  if (type->hasDefaultWriteFunction()) {
    if (!function_exists("fwrite", 2, NULL, "file", type->symbol->name)) {
      FnSymbol* fn = new FnSymbol("fwrite");
      fn->cname = stringcat("_auto_", type->symbol->name, "_fwrite");
      TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
      ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->definition);
      ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "val", type);

      fn->formals = new AList<DefExpr>(new DefExpr(fileArg), new DefExpr(arg));
      fn->retType = dtVoid;
      AList<Stmt>* body = type->buildDefaultWriteFunctionBody(fileArg, arg);
      BlockStmt* block_stmt = new BlockStmt(body);
      fn->body = block_stmt;
      DefExpr* def = new DefExpr(fn);
      type->symbol->defPoint->parentStmt->insertBefore(def);
      reset_file_info(def, type->symbol->lineno, type->symbol->filename);
      build(fn);
    }
  }

  if (type->hasDefaultReadFunction()) {
    if (!function_exists("fread", 2, NULL, "file", type->symbol->name)) {
      FnSymbol* fn = new FnSymbol("fread");
      fn->cname = stringcat("_auto_", type->symbol->name, "_fread");
      TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
      ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->definition);
      ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "val", type);
      fn->formals = new AList<DefExpr>(new DefExpr(fileArg), new DefExpr(arg));
      fn->retType = dtVoid;
      AList<Stmt>* body = type->buildDefaultReadFunctionBody(fileArg, arg);
      BlockStmt* block_stmt = new BlockStmt(body);
      fn->body = block_stmt;
      DefExpr* def = new DefExpr(fn);
      type->symbol->defPoint->parentStmt->insertBefore(def);
      reset_file_info(def, type->symbol->lineno, type->symbol->filename);
      build(fn);
    }
  }
}


/*** construct_tuple_type
 ***   builds rank-dependent tuple type
 ***/
static void construct_tuple_type(int rank) {
  currentLineno = 0;

  char *name = stringcat("_tuple", intstring(rank));

  if (Symboltable::lookupInScope(name, tupleModule->modScope))
    return;

  AList<Stmt>* decls = new AList<Stmt>();

  // Build type declarations
  Vec<Type*> types;
  for (int i = 1; i <= rank; i++) {
    char* typeName = stringcat("_t", intstring(i));
    VariableType* type = new VariableType(getMetaType(0));
    TypeSymbol* typeSymbol = new TypeSymbol(typeName, type);
    type->addSymbol(typeSymbol);
    decls->insertAtTail(new DefExpr(typeSymbol));
    types.add(type);
  }

  // Build field declarations
  Vec<VarSymbol*> fields;
  for (int i = 1; i <= rank; i++) {
    char* fieldName = stringcat("_f", intstring(i));
    VarSymbol* field = new VarSymbol(fieldName, types.v[i-1]);
    decls->insertAtTail(new DefExpr(field));
    fields.add(field);
  }

  // Build this methods
  for (int i = 1; i <= rank; i++) {
    FnSymbol* fn = new FnSymbol("this");
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "index", new_LiteralType(new_IntSymbol(i)));
    fn->formals = new AList<DefExpr>(new DefExpr(arg));

    fn->retRef = true;
    fn->body = new BlockStmt(new ReturnStmt(fields.v[i-1]->name));
    DefExpr* def = new DefExpr(fn);
    if (no_infer)
      fn->retExpr = new SymExpr(types.v[i-1]->symbol);
    decls->insertAtTail(def);
  }

  // Build tuple
  ClassType* tupleType = new ClassType(CLASS_RECORD);
  TypeSymbol* tupleSym = new TypeSymbol(name, tupleType);
  tupleType->addSymbol(tupleSym);
  tupleType->addDeclarations(decls);
  tupleModule->stmts->insertAtHead(new DefExpr(tupleSym));

  if (!fnostdincs) {
    // Build write function
    FnSymbol* fwriteFn = new FnSymbol("fwrite");
    TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
    ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->definition);
    ArgSymbol* fwriteArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
    fwriteFn->formals = new AList<DefExpr>(new DefExpr(fileArg), new DefExpr(fwriteArg));
    AList<Expr>* actuals = new AList<Expr>();
    actuals->insertAtTail(new_StringLiteral(stringcpy("(")));
    for (int i = 1; i <= rank; i++) {
      if (i != 1)
        actuals->insertAtTail(new_StringLiteral(stringcpy(", ")));
      actuals->insertAtTail(
        new CallExpr(".", new SymExpr("val"),
          new_StringSymbol(stringcat("_f", intstring(i)))));
    }
    actuals->insertAtTail(new_StringLiteral(stringcpy(")")));
    Expr* fwriteCall = new CallExpr("fwrite", new SymExpr(fileArg), actuals);
    fwriteFn->body = new BlockStmt(new ExprStmt(fwriteCall));
    tupleModule->stmts->insertAtTail(new DefExpr(fwriteFn));
  }

  // Build htuple = tuple function
  if (!fnostdincs && !fnostdincs_but_file) {
    FnSymbol* assignFn = new FnSymbol("=");
    ArgSymbol* htupleArg = 
      new ArgSymbol(INTENT_BLANK, "_htuple", chpl_htuple->definition);
    ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
    assignFn->formals = new AList<DefExpr>(new DefExpr(htupleArg),
                                           new DefExpr(tupleArg));
    assignFn->body = new BlockStmt();
    for (int i = 1; i <= rank; i++) {
      assignFn->insertAtTail(
        new CallExpr("=",
          new CallExpr(htupleArg, new_IntLiteral(i)),
          new CallExpr(tupleArg, new_IntLiteral(i))));
    }
    assignFn->insertAtTail(new ReturnStmt(htupleArg));
    tupleModule->stmts->insertAtTail(new DefExpr(assignFn));
  }

  // Build tuple = _ function
//   {
//     FnSymbol* assignFn = new FnSymbol("=");
//     ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "tuple", tupleType);
//     ArgSymbol* secondArg = new ArgSymbol(INTENT_BLANK, "val", dtUnknown);
//     assignFn->formals = new AList<DefExpr>(new DefExpr(tupleArg),
//                                            new DefExpr(secondArg));
//     assignFn->body = new BlockStmt();
//     for (int i = 1; i <= rank; i++) {
//       assignFn->insertAtTail(
//         new ExprStmt(
//           new CallExpr(PRIMITIVE_MOVE,
//             new CallExpr(tupleArg, new_IntLiteral(i)),
//             new CallExpr(secondArg, new_IntLiteral(i)))));
//     }
//     assignFn->insertAtTail(new ReturnStmt(tupleArg));
//     tupleModule->stmts->insertAtTail(new ExprStmt(new DefExpr(assignFn)));
//   }
}
