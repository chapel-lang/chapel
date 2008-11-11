#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "yy.h"

//
// declare global vectors gSymExprs, gCallExprs, gFnSymbols, ...
//
#define decl_gvecs(type) Vec<type*> g##type##s
foreach_ast(decl_gvecs);

static int uid = 1;

#define decl_counters(type)                                             \
  int n##type = g##type##s.n, k##type = n##type*sizeof(type)/1024

#define sum_gvecs(type) g##type##s.n

void printStatistics(const char* pass) {
  static int last_nasts = -1;
  static int maxK = -1, maxN = -1;

  if (!strcmp(pass, "makeBinary")) {
    if (strstr(fPrintStatistics, "m")) {
      fprintf(stderr, "Maximum # of ASTS: %d\n", maxN);
      fprintf(stderr, "Maximum Size (KB): %d\n", maxK);
    }
  }

  int nasts = foreach_ast_sep(sum_gvecs, +);

  if (last_nasts == nasts) {
    fprintf(stderr, "%23s%s\n", "", pass);
    return;
  }

  foreach_ast(decl_counters);

  int nStmt = nCondStmt + nBlockStmt + nGotoStmt;
  int kStmt = kCondStmt + kBlockStmt + kGotoStmt;
  int nExpr = nUnresolvedSymExpr + nSymExpr + nDefExpr + nCallExpr + nNamedExpr;
  int kExpr = kUnresolvedSymExpr + kSymExpr + kDefExpr + kCallExpr + kNamedExpr;
  int nSymbol = nModuleSymbol+nVarSymbol+nArgSymbol+nTypeSymbol+nFnSymbol+nEnumSymbol+nLabelSymbol;
  int kSymbol = kModuleSymbol+kVarSymbol+kArgSymbol+kTypeSymbol+kFnSymbol+kEnumSymbol+kLabelSymbol;
  int nType = nPrimitiveType+nEnumType+nClassType;
  int kType = kPrimitiveType+kEnumType+kClassType;

  fprintf(stderr, "%7d asts (%6dK) %s\n", nStmt+nExpr+nSymbol+nType, kStmt+kExpr+kSymbol+kType, pass);

  if (nStmt+nExpr+nSymbol+nType > maxN)
    maxN = nStmt+nExpr+nSymbol+nType;

  if (kStmt+kExpr+kSymbol+kType > maxK)
    maxK = kStmt+kExpr+kSymbol+kType;

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Stmt %9d  Cond %9d  Block %9d  Goto  %9d\n",
            nStmt, nCondStmt, nBlockStmt, nGotoStmt);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Stmt %9dK Cond %9dK Block %9dK Goto  %9dK\n",
            kStmt, kCondStmt, kBlockStmt, kGotoStmt);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Stmt %6dK Cond %6dK Block %6dK Goto  %6dK\n",
            kStmt, kCondStmt, kBlockStmt, kGotoStmt);

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %9d  Unre %9d  Sym  %9d  Def   %9d  Call  %9d  Named %9d\n",
            nExpr, nUnresolvedSymExpr, nSymExpr, nDefExpr, nCallExpr, nNamedExpr);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %9dK Unre %9dK Sym  %9dK Def   %9dK Call  %9dK Named %9dK\n",
            kExpr, kUnresolvedSymExpr, kSymExpr, kDefExpr, kCallExpr, kNamedExpr);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %6dK Unre %6dK Sym  %6dK Def   %6dK Call  %6dK Named %6dK\n",
            kExpr, kUnresolvedSymExpr, kSymExpr, kDefExpr, kCallExpr, kNamedExpr);

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Sym  %9d  Mod  %9d  Var   %9d  Arg   %9d  Type %9d  Fn %9d  Enum %9d  Label %9d\n",
            nSymbol, nModuleSymbol, nVarSymbol, nArgSymbol, nTypeSymbol, nFnSymbol, nEnumSymbol, nLabelSymbol);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Sym  %9dK Mod  %9dK Var   %9dK Arg   %9dK Type %9dK Fn %9dK Enum %9dK Label %9dK\n",
            kSymbol, kModuleSymbol, kVarSymbol, kArgSymbol, kTypeSymbol, kFnSymbol, kEnumSymbol, kLabelSymbol);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Sym  %6dK Mod  %6dK Var   %6dK Arg   %6dK Type %6dK Fn %6dK Enum %6dK Label %6dK\n",
            kSymbol, kModuleSymbol, kVarSymbol, kArgSymbol, kTypeSymbol, kFnSymbol, kEnumSymbol, kLabelSymbol);

  if (strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %9d  Prim  %9d  Enum %9d  Class %9d\n",
            nType, nPrimitiveType, nEnumType, nClassType);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %9dK Prim  %9dK Enum %9dK Class %9dK\n",
            kType, kPrimitiveType, kEnumType, kClassType);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %6dK Prim  %6dK Enum %6dK Class %6dK\n",
            kType, kPrimitiveType, kEnumType, kClassType);
  last_nasts = nasts;
}


static inline bool isAlive(Expr* expr) {
  return expr->parentSymbol;
}

static inline bool isAlive(Type* type) {
  return type->symbol->defPoint->parentSymbol;
}

static inline bool isAlive(Symbol* symbol) {
  return symbol == rootModule || symbol->defPoint->parentSymbol;
}

#define clean_gvec(type)                        \
  int i##type = 0;                              \
  forv_Vec(type, ast, g##type##s) {             \
    if (isAlive(ast)) {                         \
      g##type##s.v[i##type++] = ast;            \
    } else {                                    \
      delete ast;                               \
    }                                           \
  }                                             \
  g##type##s.n = i##type

void cleanAst() {
  //
  // clear back pointers to dead ast instances
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    for(int i = 0; i < ts->type->methods.n; i++) {
      FnSymbol* method = ts->type->methods.v[i];
      if (method && !method->defPoint->parentSymbol)
        ts->type->methods.v[i] = NULL;
      if (ClassType* ct = toClassType(ts->type)) {
        if (ct->defaultConstructor && !ct->defaultConstructor->defPoint->parentSymbol)
          ct->defaultConstructor = NULL;
        else if (ct->destructor && !ct->destructor->defPoint->parentSymbol)
          ct->destructor = NULL;
      }
    }
    for(int i = 0; i < ts->type->dispatchChildren.n; i++) {
      Type* type = ts->type->dispatchChildren.v[i];
      if (type && !type->symbol->defPoint->parentSymbol)
        ts->type->dispatchChildren.v[i] = NULL;
    }
  }

  //
  // clean global vectors and delete dead ast instances
  //
  foreach_ast(clean_gvec);
}


void destroyAst() {
  #define destroy_gvec(type)                    \
    forv_Vec(type, ast, g##type##s) {           \
      delete ast;                               \
    }
  foreach_ast(destroy_gvec);
}


void
verify() {
  #define verify_gvec(type)                       \
    forv_Vec(type, ast, g##type##s) {             \
      ast->verify();                              \
    }
  foreach_ast(verify_gvec);
}


// This is here so that we can break on the creation of a particular
// BaseAST instance in gdb.
static void checkid(int id) {
}


BaseAST::BaseAST(AstTag type) :
  astTag(type),
  id(uid++),
  lineno(yystartlineno)
{
  checkid(id);
  if (lineno == -1) {
    if (currentLineno) {
      lineno = currentLineno;
    }
  }
}


const char* BaseAST::stringLoc(void) {
  const int tmpBuffSize = 64;
  char tmpBuff[tmpBuffSize];

  snprintf(tmpBuff, tmpBuffSize, "%s:%d", getModule()->filename, lineno);
  return astr(tmpBuff);
}


ModuleSymbol* BaseAST::getModule() {
  if (!this)
    return NULL;
  if (ModuleSymbol* x = toModuleSymbol(this))
    return x;
  else if (Type* x = toType(this))
    return x->symbol->getModule();
  else if (Symbol* x = toSymbol(this))
    return x->defPoint->getModule();
  else if (Expr* x = toExpr(this))
    return x->parentSymbol->getModule();
  else
    INT_FATAL(this, "Unexpected case in BaseAST::getModule()");
  return NULL;
}


FnSymbol* BaseAST::getFunction() {
  if (!this)
    return NULL;
  if (ModuleSymbol* x = toModuleSymbol(this))
    return x->initFn;
  else if (FnSymbol* x = toFnSymbol(this))
    return x;
  else if (Type* x = toType(this))
    return x->symbol->getFunction();
  else if (Symbol* x = toSymbol(this))
    return x->defPoint->getFunction();
  else if (Expr* x = toExpr(this))
    return x->parentSymbol->getFunction();
  else
    INT_FATAL(this, "Unexpected case in BaseAST::getFunction()");
  return NULL;
}


const char* astTagName[E_BaseAST+1] = {
  "SymExpr",
  "UnresolvedSymExpr",
  "DefExpr",
  "CallExpr",
  "NamedExpr",
  "BlockStmt",
  "CondStmt",
  "GotoStmt",
  "Expr",

  "ModuleSymbol",
  "VarSymbol",
  "ArgSymbol",
  "TypeSymbol",
  "FnSymbol",
  "EnumSymbol",
  "LabelSymbol",
  "Symbol",

  "PrimitiveType",
  "EnumType",
  "ClassType",
  "Type",

  "BaseAST"
};

int currentLineno = 0;

Vec<ModuleSymbol*> allModules;  // Contains all modules
Vec<ModuleSymbol*> userModules; // Contains user modules

void registerModule(ModuleSymbol* mod) {
  switch (mod->modTag) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_STANDARD:
    if (strcmp(mod->name, "_root"))
      allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
  }
}
