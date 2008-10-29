#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "yy.h"

Vec<BaseAST*> gAsts;
Vec<FnSymbol*> gFns;
Vec<TypeSymbol*> gTypes;
Vec<CallExpr*> gCalls;
static int uid = 1;

#define decl_counters(typeName)                 \
  int n##typeName = 0, k##typeName = 0

#define case_counters(typeName)                 \
  case E_##typeName: n##typeName++; break

#define calc_counters(typeName)                         \
  k##typeName = n##typeName*sizeof(typeName)/1024

void printStatistics(const char* pass) {
  static int last_asts = -1;
  static int maxK = -1, maxN = -1;

  if (!strcmp(pass, "makeBinary")) {
    if (strstr(fPrintStatistics, "m")) {
      fprintf(stderr, "Maximum # of ASTS: %d\n", maxN);
      fprintf(stderr, "Maximum Size (KB): %d\n", maxK);
    }
  }

  if (last_asts == gAsts.n) {
    fprintf(stderr, "%23s%s\n", "", pass);
    return;
  }

  decl_counters(CondStmt);
  decl_counters(BlockStmt);
  decl_counters(GotoStmt);
  decl_counters(SymExpr);
  decl_counters(UnresolvedSymExpr);
  decl_counters(DefExpr);
  decl_counters(CallExpr);
  decl_counters(NamedExpr);
  decl_counters(ModuleSymbol);
  decl_counters(VarSymbol);
  decl_counters(ArgSymbol);
  decl_counters(TypeSymbol);
  decl_counters(FnSymbol);
  decl_counters(EnumSymbol);
  decl_counters(LabelSymbol);
  decl_counters(PrimitiveType);
  decl_counters(EnumType);
  decl_counters(ClassType);
  forv_Vec(BaseAST, ast, gAsts) {
    switch (ast->astTag) {
      case_counters(CondStmt);
      case_counters(BlockStmt);
      case_counters(GotoStmt);
      case_counters(SymExpr);
      case_counters(UnresolvedSymExpr);
      case_counters(DefExpr);
      case_counters(CallExpr);
      case_counters(NamedExpr);
      case_counters(ModuleSymbol);
      case_counters(VarSymbol);
      case_counters(ArgSymbol);
      case_counters(TypeSymbol);
      case_counters(FnSymbol);
      case_counters(EnumSymbol);
      case_counters(LabelSymbol);
      case_counters(PrimitiveType);
      case_counters(EnumType);
      case_counters(ClassType);
    default: break;
    }
  }
  calc_counters(CondStmt);
  calc_counters(BlockStmt);
  calc_counters(GotoStmt);
  calc_counters(SymExpr);
  calc_counters(UnresolvedSymExpr);
  calc_counters(DefExpr);
  calc_counters(CallExpr);
  calc_counters(NamedExpr);
  calc_counters(ModuleSymbol);
  calc_counters(VarSymbol);
  calc_counters(ArgSymbol);
  calc_counters(TypeSymbol);
  calc_counters(FnSymbol);
  calc_counters(EnumSymbol);
  calc_counters(LabelSymbol);
  calc_counters(PrimitiveType);
  calc_counters(EnumType);
  calc_counters(ClassType);
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
  last_asts = gAsts.n;
}


void cleanAst() {
  forv_Vec(TypeSymbol, ts, gTypes) {
    for(int i = 0; i < ts->type->methods.n; i++) {
      FnSymbol* method = ts->type->methods.v[i];
      if (method && !method->defPoint->parentSymbol)
        ts->type->methods.v[i] = NULL;
    }
    for(int i = 0; i < ts->type->dispatchChildren.n; i++) {
      Type* type = ts->type->dispatchChildren.v[i];
      if (type && !type->symbol->defPoint->parentSymbol)
        ts->type->dispatchChildren.v[i] = NULL;
    }
  }
  int iasts = 0, ifn = 0, itypes = 0, icalls = 0;
  forv_Vec(BaseAST, ast, gAsts) {
    if (Type* type = toType(ast)) {
      if (type->symbol->defPoint->parentSymbol) {
        gAsts.v[iasts++] = type;
      } else {
        delete type;
      }
    } else if (Symbol* sym = toSymbol(ast)) {
      if (sym == rootModule || sym->defPoint->parentSymbol) {
        gAsts.v[iasts++] = sym;
        if (FnSymbol* fn = toFnSymbol(sym))
          gFns.v[ifn++] = fn;
        else if (TypeSymbol* type = toTypeSymbol(sym))
          gTypes.v[itypes++] = type;
      } else {
        delete sym;
      }
    } else if (Expr* expr = toExpr(ast)) {
      if (expr->parentSymbol) {
        gAsts.v[iasts++] = ast;
        if (CallExpr* call = toCallExpr(expr))
          gCalls.v[icalls++] = call;
      } else {
        delete expr;
      }
    }
  }
  gAsts.n = iasts;
  gFns.n = ifn;
  gTypes.n = itypes;
  gCalls.n = icalls;
}


void destroyAst() {
  forv_Vec(BaseAST, ast, gAsts) {
    delete ast;
  }
}


void
verify() {
  forv_Vec(BaseAST, ast, gAsts)
    ast->verify();

  if (fExtraVerification) {
    //
    // verify gAsts and collect_asts are identical
    //
    Vec<Expr*> set;
    Vec<BaseAST*> vAsts;
    collect_asts(rootModule, vAsts);

    forv_Vec(BaseAST, ast, gAsts) {
      if (Expr* expr = toExpr(ast)) {
        set.set_add(expr);
      }
    }
    forv_Vec(BaseAST, ast, vAsts) {
      if (Expr* expr = toExpr(ast)) {
        if (!set.set_in(expr)) {
          INT_FATAL(expr, "collected expr %d is not in gAsts", expr->id);
        }
      }
    }

    set.set_clear();

    forv_Vec(BaseAST, ast, vAsts) {
      if (Expr* expr = toExpr(ast)) {
        set.set_add(expr);
      }
    }

    forv_Vec(BaseAST, ast, gAsts) {
      if (Expr* expr = toExpr(ast)) {
        if (!set.set_in(expr)) {
          INT_FATAL(expr, "expr %d in gAsts is not collected", expr->id);
        }
      }
    }
  }
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
  gAsts.add(this);
}


BaseAST*
BaseAST::copyInner(SymbolMap* map) {
  INT_FATAL(this, "copyInner not implemented for BaseAST subclass");
  return NULL;
}


bool BaseAST::inTree(void) {
  INT_FATAL(this, "Unexpected call to BaseAST::inTree()");
  return false;
}


Type* BaseAST::typeInfo(void) {
  INT_FATAL(this, "Unexpected call to BaseAST::typeInfo()");
  return NULL;
}


void BaseAST::verify() {
}


void BaseAST::codegen(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling codegen() on a Null AST");
  } else {
    INT_FATAL("codegen() is unimplemented for a subclass of BaseAST");
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
