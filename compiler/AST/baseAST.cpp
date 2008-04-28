#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"
#include "yy.h"

Vec<BaseAST*> gAsts;
Vec<FnSymbol*> gFns;
Vec<TypeSymbol*> gTypes;
static int uid = 1;

#define decl_counters(typeName, enumName)       \
  int n##typeName = 0, k##typeName = 0

#define case_counters(typeName, enumName)       \
  case enumName: n##typeName++; break

#define calc_counters(typeName, enumName)               \
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

  decl_counters(CondStmt, STMT_COND);
  decl_counters(BlockStmt, STMT_BLOCK);
  decl_counters(GotoStmt, STMT_GOTO);
  decl_counters(SymExpr, EXPR_SYM);
  decl_counters(DefExpr, EXPR_DEF);
  decl_counters(CallExpr, EXPR_CALL);
  decl_counters(NamedExpr, EXPR_NAMED);
  decl_counters(ModuleSymbol, SYMBOL_MODULE);
  decl_counters(VarSymbol, SYMBOL_VAR);
  decl_counters(ArgSymbol, SYMBOL_ARG);
  decl_counters(TypeSymbol, SYMBOL_TYPE);
  decl_counters(FnSymbol, SYMBOL_FN);
  decl_counters(EnumSymbol, SYMBOL_ENUM);
  decl_counters(LabelSymbol, SYMBOL_LABEL);
  decl_counters(PrimitiveType, TYPE_PRIMITIVE);
  decl_counters(EnumType, TYPE_ENUM);
  decl_counters(ClassType, TYPE_CLASS);
  forv_Vec(BaseAST, ast, gAsts) {
    switch (ast->astTag) {
      case_counters(CondStmt, STMT_COND);
      case_counters(BlockStmt, STMT_BLOCK);
      case_counters(GotoStmt, STMT_GOTO);
      case_counters(SymExpr, EXPR_SYM);
      case_counters(DefExpr, EXPR_DEF);
      case_counters(CallExpr, EXPR_CALL);
      case_counters(NamedExpr, EXPR_NAMED);
      case_counters(ModuleSymbol, SYMBOL_MODULE);
      case_counters(VarSymbol, SYMBOL_VAR);
      case_counters(ArgSymbol, SYMBOL_ARG);
      case_counters(TypeSymbol, SYMBOL_TYPE);
      case_counters(FnSymbol, SYMBOL_FN);
      case_counters(EnumSymbol, SYMBOL_ENUM);
      case_counters(LabelSymbol, SYMBOL_LABEL);
      case_counters(PrimitiveType, TYPE_PRIMITIVE);
      case_counters(EnumType, TYPE_ENUM);
      case_counters(ClassType, TYPE_CLASS);
    default: break;
    }
  }
  calc_counters(CondStmt, STMT_COND);
  calc_counters(BlockStmt, STMT_BLOCK);
  calc_counters(GotoStmt, STMT_GOTO);
  calc_counters(SymExpr, EXPR_SYM);
  calc_counters(DefExpr, EXPR_DEF);
  calc_counters(CallExpr, EXPR_CALL);
  calc_counters(NamedExpr, EXPR_NAMED);
  calc_counters(ModuleSymbol, SYMBOL_MODULE);
  calc_counters(VarSymbol, SYMBOL_VAR);
  calc_counters(ArgSymbol, SYMBOL_ARG);
  calc_counters(TypeSymbol, SYMBOL_TYPE);
  calc_counters(FnSymbol, SYMBOL_FN);
  calc_counters(EnumSymbol, SYMBOL_ENUM);
  calc_counters(LabelSymbol, SYMBOL_LABEL);
  calc_counters(PrimitiveType, TYPE_PRIMITIVE);
  calc_counters(EnumType, TYPE_ENUM);
  calc_counters(ClassType, TYPE_CLASS);
  int nStmt = nCondStmt + nBlockStmt + nGotoStmt;
  int kStmt = kCondStmt + kBlockStmt + kGotoStmt;
  int nExpr = nSymExpr + nDefExpr + nCallExpr + nNamedExpr;
  int kExpr = kSymExpr + kDefExpr + kCallExpr + kNamedExpr;
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
    fprintf(stderr, "    Expr %9d  Sym  %9d  Def   %9d  Call  %9d  Named %9d\n",
            nExpr, nSymExpr, nDefExpr, nCallExpr, nNamedExpr);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %9dK Sym  %9dK Def   %9dK Call  %9dK Named %9dK\n",
            kExpr, kSymExpr, kDefExpr, kCallExpr, kNamedExpr);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Expr %6dK Sym  %6dK Def   %6dK Call  %6dK Named %6dK\n",
            kExpr, kSymExpr, kDefExpr, kCallExpr, kNamedExpr);

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

static bool
isLive(Symbol* sym) {
  if (sym == theProgram)
    return true;
  if (sym->parentScope == rootScope)
    return true;
  if (VarSymbol* var = toVarSymbol(sym))
    if (var->immediate)
      return true;
  if (sym->defPoint->parentSymbol)
    return true;
  return false;
}


void cleanAst() {
  forv_Vec(BaseAST, ast, gAsts) {
    if (TypeSymbol* ts = toTypeSymbol(ast)) {
      for(int i = 0; i < ts->type->methods.n; i++) {
        FnSymbol* method = ts->type->methods.v[i];
        if (method && !isLive(method))
          ts->type->methods.v[i] = NULL;
      }
      for(int i = 0; i < ts->type->dispatchChildren.n; i++) {
        Type* type = ts->type->dispatchChildren.v[i];
        if (type && !isLive(type->symbol))
          ts->type->dispatchChildren.v[i] = NULL;
      }
    }
  }
  int iasts = 0, ifn = 0, itypes = 0;
  forv_Vec(BaseAST, ast, gAsts) {
    if (Symbol* sym = toSymbol(ast)) {
      if (isLive(sym)) {
        if (sym->astTag == SYMBOL_TYPE)
          gAsts.v[iasts++] = sym->type;
        gAsts.v[iasts++] = sym;
      } else {
        if (sym->astTag == SYMBOL_TYPE)
          delete sym->type;
        delete sym;
      }
    } else if (Expr* expr = toExpr(ast)) {
      if (expr->parentSymbol) {
        gAsts.v[iasts++] = ast;
        if (DefExpr* def = toDefExpr(ast)) {
          if (FnSymbol* fn = toFnSymbol(def->sym))
            gFns.v[ifn++] = fn;
          else if (TypeSymbol* type = toTypeSymbol(def->sym))
            gTypes.v[itypes++] = type;
        }
      } else
        delete expr;
    }
  }
  gAsts.n = iasts;
  gFns.n = ifn;
  gTypes.n = itypes;
}


void destroyAst() {
  forv_Vec(BaseAST, ast, gAsts) {
    delete ast;
  }
  delete rootScope;
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
    collect_asts(&vAsts);

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
  parentScope(NULL),
  filename(yyfilename), 
  lineno(yystartlineno)
{
  checkid(id);
  if (lineno == -1) {
    if (currentLineno && currentFilename) {
      lineno = currentLineno;
      filename = currentFilename;
    }
  }
  gAsts.add(this);
}


BaseAST*
BaseAST::copyInner(ASTMap* map) {
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

  snprintf(tmpBuff, tmpBuffSize, "%s:%d", filename, lineno);
  return astr(tmpBuff);
}


void BaseAST::addPragma(const char* str) {
  if (DefExpr* defExpr = toDefExpr(this)) {
    defExpr->sym->addPragma(str);
  } else if (Symbol* sym = toSymbol(this)) {
    sym->pragmas.add(str);
  } else {
    INT_FATAL(this, "pragma not added");
  }
}


void BaseAST::addPragmas(Vec<const char*>* srcPragmas) {
  forv_Vec(const char, srcPragma, *srcPragmas) {
    addPragma(srcPragma);
  }
}


void BaseAST::copyPragmas(BaseAST* ast) {
  if (Symbol* sym = toSymbol(ast))
    addPragmas(&(sym->pragmas));
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


Symbol* BaseAST::lookup(const char* name) {
  if (ModuleSymbol* a = toModuleSymbol(this))
    return a->block->blkScope->lookup(astr(name));
  return parentScope->lookup(astr(name));
}

Symbol* BaseAST::lookup(BaseAST* ast) {
  if (SymExpr* a = toSymExpr(ast)) {
    if (a->var)
      return lookup(a->var->name);
    else
      return lookup(a->unresolved);
  }
  INT_FATAL(ast, "Bad call to lookup");
  return NULL;
}

TypeSymbol* BaseAST::lookupType(const char* name) {
  return toTypeSymbol(lookup(astr(name)));
}

VarSymbol* BaseAST::lookupVar(const char* name) {
  return toVarSymbol(lookup(astr(name)));
}

const char* astTagName[BASE+1] = {
  "Expr",
  "SymExpr",
  "DefExpr",
  "CallExpr",
  "NamedExpr",
  "BlockStmt",
  "CondStmt",
  "GotoStmt",

  "Symbol",
  "ModuleSymbol",
  "VarSymbol",
  "ArgSymbol",
  "TypeSymbol",
  "FnSymbol",
  "EnumSymbol",
  "LabelSymbol",

  "Type",
  "PrimitiveType",
  "EnumType",
  "ClassType",

  "BaseAST"
};

int currentLineno = 0;
const char* currentFilename = NULL;

#define AST_ADD_CHILD(_t, _m) if (((_t*)a)->_m) asts.add(((_t*)a)->_m)
#define AST_ADD_LIST(_t, _m) for_alist(tmp, ((_t*)a)->_m) asts.add(tmp)

void
get_ast_children(BaseAST *a, Vec<BaseAST *> &asts) {
  switch (a->astTag) {
  case EXPR:
    break;
  case EXPR_SYM:
    break;
  case EXPR_DEF:
    AST_ADD_CHILD(DefExpr, init);
    AST_ADD_CHILD(DefExpr, exprType);
    AST_ADD_CHILD(DefExpr, sym);
    break;
  case EXPR_CALL:
    AST_ADD_CHILD(CallExpr, baseExpr);
    AST_ADD_LIST(CallExpr, argList);
    break;
  case EXPR_NAMED:
    AST_ADD_CHILD(NamedExpr, actual);
    break;
  case STMT_BLOCK:
    AST_ADD_LIST(BlockStmt, body);
    AST_ADD_CHILD(BlockStmt, loopInfo);
    break;
  case STMT_COND:
    AST_ADD_CHILD(CondStmt, condExpr);
    AST_ADD_CHILD(CondStmt, thenStmt);
    AST_ADD_CHILD(CondStmt, elseStmt);
    break;
  case STMT_GOTO:
    AST_ADD_CHILD(GotoStmt, label);
    break;
  case SYMBOL:
    break;
  case SYMBOL_MODULE:
    AST_ADD_CHILD(ModuleSymbol, block);
    break;
  case SYMBOL_VAR:
    break;
  case SYMBOL_ARG:
    AST_ADD_CHILD(ArgSymbol, typeExpr);
    AST_ADD_CHILD(ArgSymbol, defaultExpr);
    AST_ADD_CHILD(ArgSymbol, variableExpr);
    break;
  case SYMBOL_TYPE:
    AST_ADD_CHILD(Symbol, type);
    break;
  case SYMBOL_FN:
    AST_ADD_LIST(FnSymbol, formals);
    AST_ADD_CHILD(FnSymbol, setter);
    AST_ADD_CHILD(FnSymbol, body);
    AST_ADD_CHILD(FnSymbol, where);
    AST_ADD_CHILD(FnSymbol, retExprType);
    break;
  case SYMBOL_ENUM:
    break;
  case SYMBOL_LABEL:
    break;
  case TYPE:
    break;
  case TYPE_PRIMITIVE:
    break;
  case TYPE_ENUM:
    AST_ADD_LIST(EnumType, constants);
    break;
  case TYPE_CLASS:
    AST_ADD_LIST(ClassType, fields);
    AST_ADD_LIST(ClassType, inherits);
    break;
  case BASE:
    INT_FATAL(a, "Unexpected case in get_ast_children (BASE)");
    break;
  }
}

SymScope* rootScope = NULL;

Vec<ModuleSymbol*> allModules;  // Contains all modules
Vec<ModuleSymbol*> userModules; // Contains user modules

void registerModule(ModuleSymbol* mod) {
  switch (mod->modTag) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_STANDARD:
    allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
  }
}
