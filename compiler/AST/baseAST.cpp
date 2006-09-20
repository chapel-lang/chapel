#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"
#include "yy.h"

ChainHashMap<char*, StringHashFns, char*> chapelStringsTable;

char*
canonicalize_string(char *s) {
  char* ss = chapelStringsTable.get(s);
  if (!ss) {
    ss = strdup(s);
    chapelStringsTable.put(ss, ss);
  }
  return ss;
}

char*
astr(char* s1, char* s2, char* s3, char* s4) {
  char s[1024];
  strcpy(s, s1);
  if (s2)
    strcat(s, s2);
  if (s3)
    strcat(s, s3);
  if (s4)
    strcat(s, s4);
  return canonicalize_string(s);
}

Vec<BaseAST*> gAsts;
Vec<FnSymbol*> gFns;
Vec<TypeSymbol*> gTypes;
static long uid = 1;

void cleanAst(long* astCount, int* liveCount) {
  gFns.clear();
  gTypes.clear();
  Vec<BaseAST*> asts;
  forv_Vec(BaseAST, ast, gAsts) {
    if (dynamic_cast<Expr*>(ast) || dynamic_cast<Stmt*>(ast)) {
      if (ast->parentSymbol) {
        asts.add(ast);
        if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
          if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym))
            gFns.add(fn);
          if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(def->sym))
            gTypes.add(type);
        }
      } else {
        delete ast;
      }
    } else {
      asts.add(ast);
    }
  }
  gAsts.clear();
  gAsts.move(asts);
  forv_Vec(BaseAST, ast, gAsts)
    ast->clean();
  *astCount = uid;
  *liveCount = gAsts.n;
}


void destroyAst() {
  Vec<char*> keys;
  chapelStringsTable.get_keys(keys);
  forv_Vec(char, key, keys) {
    free(key);
  }
  forv_Vec(BaseAST, ast, gAsts) {
    delete ast;
  }
  delete rootScope;
}


// This is here so that we can break on the creation of a particular
// BaseAST instance in gdb.
static void checkid(long id) {
}


long BaseAST::getNumIDs(void) {
  return uid;
}


BaseAST::BaseAST(astType_t type) :
  astType(type),
  id(uid++),
  prev(NULL),
  next(NULL),
  parentScope(NULL),
  parentSymbol(NULL),
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


void BaseAST::verify() {

}


void BaseAST::clean() {

}


void BaseAST::print(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling print() on a Null AST");
  } else {
    INT_FATAL("print() is unimplemented for a subclass of BaseAST");
  }
}


void BaseAST::printDef(FILE* outfile) {
  INT_FATAL("printDef() is unimplemented for a subclass of BaseAST");
}


void BaseAST::println(FILE* outfile) {
  print(outfile);
  fprintf(outfile, "\n");
}


void BaseAST::codegen(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling codegen() on a Null AST");
  } else {
    INT_FATAL("codegen() is unimplemented for a subclass of BaseAST");
  }
}


void BaseAST::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to BaseAST::replaceChild(old, new)");
}


void BaseAST::callReplaceChild(BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to BaseAST::callReplaceChild(new)");
}


ASTContext BaseAST::getContext(void) {
  ASTContext context;
  INT_FATAL(this, "Unexpected call to BaseAST::getContext()");
  return context;
}


bool BaseAST::inTree(void) {
  INT_FATAL(this, "Unexpected call to BaseAST::inTree()");
  return false;
}


BaseAST* BaseAST::remove(void) {
  if (!this)
    return this;
  if (prev || next) {
    if (!prev || !next) {
      INT_FATAL("Ill-formed list in BaseAST::remove");
    }
    next->prev = prev;
    prev->next = next;
    next = NULL;
    prev = NULL;
  } else {
    callReplaceChild(NULL);
  }
  remove_help(this);
  return this;
}


void BaseAST::replace(BaseAST* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in BaseAST::replace");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in BaseAST::replace");
  if (prev || next) {
    if (!prev || !next) {
      INT_FATAL(this, "Ill-formed list in BaseAST::replace");
    }
    new_ast->prev = prev;
    new_ast->next = next;
    next->prev = new_ast;
    prev->next = new_ast;
    next = NULL;
    prev = NULL;
  } else {
    callReplaceChild(new_ast);
  }
  ASTContext context = getContext();
  remove_help(this);
  insert_help(new_ast, context.parentExpr, context.parentStmt,
              context.parentSymbol, context.parentScope);
}


void BaseAST::insertBefore(BaseAST* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in BaseAST::insertBefore");
  if (!prev)
    INT_FATAL(this, "Cannot call insertBefore on BaseAST not in a list");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in BaseAST::insertBefore");
  if (dynamic_cast<Symbol*>(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in BaseAST::insertBefore");
  new_ast->prev = prev;
  new_ast->next = this;
  prev->next = new_ast;
  prev = new_ast;
  sibling_insert_help(this, new_ast);
}


void BaseAST::insertAfter(BaseAST* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in BaseAST::insertAfter");
  if (!next)
    INT_FATAL(this, "Cannot call insertAfter on BaseAST not in a list");
  if (new_ast->prev || new_ast->next)
    INT_FATAL(new_ast, "Argument is in a list in BaseAST::insertAfter");
  if (dynamic_cast<Symbol*>(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in BaseAST::insertAfter");
  new_ast->prev = this;
  new_ast->next = next;
  next->prev = new_ast;
  next = new_ast;
  sibling_insert_help(this, new_ast);
}


char* BaseAST::stringLoc(void) {
  const int tmpBuffSize = 64;
  char tmpBuff[tmpBuffSize];

  snprintf(tmpBuff, tmpBuffSize, "%s:%d", filename, lineno);
  return stringcpy(tmpBuff);
}


void BaseAST::printLoc(FILE* outfile) {
  fprintf(outfile, "%s:%d", filename, lineno);
}


void BaseAST::addPragma(char* str) {
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(this)) {
    exprStmt->expr->addPragma(str);
  } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(this)) {
    defExpr->sym->addPragma(str);
  } else if (Symbol* sym = dynamic_cast<Symbol*>(this)) {
    sym->pragmas.add(str);
  }
}


void BaseAST::addPragmas(Vec<char*>* srcPragmas) {
  forv_Vec(char, srcPragma, *srcPragmas) {
    addPragma(srcPragma);
  }
}


void BaseAST::copyPragmas(BaseAST* ast) {
  if (Symbol* sym = dynamic_cast<Symbol*>(ast))
    addPragmas(&(sym->pragmas));
}


ModuleSymbol* BaseAST::getModule() {
  if (!this || dynamic_cast<UnresolvedSymbol*>(this))
    return NULL;
  if (ModuleSymbol* x = dynamic_cast<ModuleSymbol*>(this))
    return x;
  else if (Type* x = dynamic_cast<Type*>(this))
    return x->symbol->getModule();
  else if (Symbol* x = dynamic_cast<Symbol*>(this))
    return x->defPoint->getModule();
  else
    return parentSymbol->getModule();
}


FnSymbol* BaseAST::getFunction() {
  if (!this || dynamic_cast<UnresolvedSymbol*>(this))
    return NULL;
  if (ModuleSymbol* x = dynamic_cast<ModuleSymbol*>(this))
    return x->initFn;
  else if (FnSymbol* x = dynamic_cast<FnSymbol*>(this))
    return x;
  else if (Type* x = dynamic_cast<Type*>(this))
    return x->symbol->getFunction();
  else if (Symbol* x = dynamic_cast<Symbol*>(this))
    return x->defPoint->getFunction();
  else
    return parentSymbol->getFunction();
}


Symbol* BaseAST::lookup(char* name) {
  if (ModuleSymbol* a = dynamic_cast<ModuleSymbol*>(this))
    return a->modScope->lookup(name);
  return parentScope->lookup(name);
}

Symbol* BaseAST::lookup(BaseAST* ast) {
  if (SymExpr* a = dynamic_cast<SymExpr*>(ast))
    return lookup(a->var->name);
  INT_FATAL(ast, "Bad call to lookup");
  return NULL;
}

TypeSymbol* BaseAST::lookupType(char* name) {
  return dynamic_cast<TypeSymbol*>(lookup(name));
}

TypeSymbol* BaseAST::lookupType(BaseAST* ast) {
  return dynamic_cast<TypeSymbol*>(lookup(ast));
}

VarSymbol* BaseAST::lookupVar(char* name) {
  return dynamic_cast<VarSymbol*>(lookup(name));
}

VarSymbol* BaseAST::lookupVar(BaseAST* ast) {
  return dynamic_cast<VarSymbol*>(lookup(ast));
}

int compar_baseast(const void *ai, const void *aj) {
  BaseAST *i = *(BaseAST**)ai;
  BaseAST *j = *(BaseAST**)aj;
  if (i->id > j->id)
    return 1;
  else if (i->id < j->id)
    return -1;
  return 0;
}

char* astTypeName[AST_TYPE_END+1] = {
  "Stmt",
  "ExprStmt",
  "ReturnStmt",
  "BlockStmt",
  "CondStmt",
  "GotoStmt",

  "Expr",
  "SymExpr",
  "DefExpr",
  "CallExpr",
  "NamedExpr",

  "Symbol",
  "UnresolvedSymbol",
  "ModuleSymbol",
  "VarSymbol",
  "ArgSymbol",
  "TypeSymbol",
  "FnSymbol",
  "EnumSymbol",
  "LabelSymbol",

  "Type",
  "PrimitiveType",
  "FnType",
  "EnumType",
  "UserType",
  "ClassType",

  "BASE",

  "AST_TYPE_END"
};

int currentLineno = 0;
char* currentFilename = NULL;
char* currentTraversal = NULL;

#define AST_ADD_CHILD(_t, _m) if (((_t*)a)->_m) asts.add(((_t*)a)->_m)
#define AST_ADD_LIST(_t, _m, _mt)        \
  for_alist_sc(_mt, tmp, ((_t*)a)->_m) { \
    asts.add(tmp);                       \
  }

void
get_ast_children(BaseAST *a, Vec<BaseAST *> &asts) {
  switch (a->astType) {
  case STMT:
  case EXPR:
  case SYMBOL:
  case TYPE:
    break;
  case STMT_EXPR:
    AST_ADD_CHILD(ExprStmt, expr);
    break;
  case STMT_RETURN:
    AST_ADD_CHILD(ReturnStmt, expr);
    break;
  case STMT_BLOCK:
    AST_ADD_CHILD(BlockStmt, param_low);
    AST_ADD_CHILD(BlockStmt, param_high);
    AST_ADD_CHILD(BlockStmt, param_stride);
    AST_ADD_CHILD(BlockStmt, param_index);
    AST_ADD_LIST(BlockStmt, body, Stmt);
    break;
  case STMT_COND:
    AST_ADD_CHILD(CondStmt, condExpr);
    AST_ADD_CHILD(CondStmt, thenStmt);
    AST_ADD_CHILD(CondStmt, elseStmt);
    break;
  case STMT_GOTO:
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
    AST_ADD_LIST(CallExpr, argList, Expr);
    break;
  case EXPR_NAMED:
    AST_ADD_CHILD(NamedExpr, actual);
    break;
  case SYMBOL_UNRESOLVED:
    break;
  case SYMBOL_MODULE:
    AST_ADD_LIST(ModuleSymbol, stmts, Stmt);
    break;
  case SYMBOL_VAR:
    break;
  case SYMBOL_ARG:
    AST_ADD_CHILD(ArgSymbol, defaultExpr);
    AST_ADD_CHILD(ArgSymbol, variableExpr);
    break;
  case SYMBOL_TYPE:
    AST_ADD_CHILD(Symbol, type);
    break;
  case SYMBOL_FN:
    AST_ADD_LIST(FnSymbol, formals, DefExpr);
    AST_ADD_CHILD(FnSymbol, body);
    AST_ADD_CHILD(FnSymbol, where);
    AST_ADD_CHILD(FnSymbol, retExprType);
    break;
  case SYMBOL_ENUM:
    break;
  case SYMBOL_LABEL:
    break;
  case TYPE_PRIMITIVE:
    break;
  case TYPE_FN:
    break;
  case TYPE_ENUM:
    AST_ADD_LIST(EnumType, constants, DefExpr);
    break;
  case TYPE_USER:
    AST_ADD_CHILD(UserType, typeExpr);
    break;
  case TYPE_CLASS:
    AST_ADD_LIST(ClassType, fields, DefExpr);
    AST_ADD_LIST(ClassType, inherits, Expr);
    break;
  case BASE:
    INT_FATAL(a, "Unexpected case in get_ast_children (BASE)");
    break;
  case AST_TYPE_END:
    INT_FATAL(a, "Unexpected case in get_ast_children (AST_TYPE_END)");
    break;
  }
}
#undef AST_ADD_CHILD
#undef AST_ADD_LIST

SymScope* rootScope = NULL;

Vec<ModuleSymbol*> allModules;  // Contains all modules
Vec<ModuleSymbol*> userModules; // Contains user modules

void registerModule(ModuleSymbol* mod) {
  switch (mod->modtype) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_STANDARD:
    allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
  }
}
