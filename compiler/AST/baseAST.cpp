/*
 * Copyright 2004-2015 Cray Inc.
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

#include <ostream>
#include <sstream>
#include <string>

#include "baseAST.h"

#include "astutil.h"
#include "CForLoop.h"
#include "expr.h"
#include "ForLoop.h"
#include "IpeSymbol.h"
#include "log.h"
#include "ParamForLoop.h"
#include "parser.h"
#include "passes.h"
#include "runpasses.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "WhileStmt.h"

static void cleanModuleList();

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
  int kStmt = kCondStmt + kBlockStmt + kGotoStmt + kExternBlockStmt;
  int nExpr = nUnresolvedSymExpr + nSymExpr + nDefExpr + nCallExpr + nNamedExpr;
  int kExpr = kUnresolvedSymExpr + kSymExpr + kDefExpr + kCallExpr + kNamedExpr;
  int nSymbol = nModuleSymbol+nVarSymbol+nArgSymbol+nIpeSymbol+nTypeSymbol+nFnSymbol+nEnumSymbol+nLabelSymbol;
  int kSymbol = kModuleSymbol+kVarSymbol+kArgSymbol+kIpeSymbol+kTypeSymbol+kFnSymbol+kEnumSymbol+kLabelSymbol;
  int nType = nPrimitiveType+nEnumType+nAggregateType;
  int kType = kPrimitiveType+kEnumType+kAggregateType;

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
    fprintf(stderr, "    Type %9d  Prim  %9d  Enum %9d  Class %9d \n",
            nType, nPrimitiveType, nEnumType, nAggregateType);
  if (strstr(fPrintStatistics, "k") && strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %9dK Prim  %9dK Enum %9dK Class %9dK\n",
            kType, kPrimitiveType, kEnumType, kAggregateType);
  if (strstr(fPrintStatistics, "k") && !strstr(fPrintStatistics, "n"))
    fprintf(stderr, "    Type %6dK Prim  %6dK Enum %6dK Class %6dK\n",
            kType, kPrimitiveType, kEnumType, kAggregateType);
  last_nasts = nasts;
}

// for debugging purposes only
void trace_remove(BaseAST* ast, char flag) {
  // crash if deletedIdHandle is not initialized but deletedIdFilename is
  if (deletedIdON() == true) {
    fprintf(deletedIdHandle, "%d %c %p %d\n",
            currentPassNo, flag, ast, ast->id);
  }
  if (ast->id == breakOnDeleteID) {
    if (deletedIdON() == true) fflush(deletedIdHandle);
    gdbShouldBreakHere();
  }
  // There should never be an attempt to delete a global type.
  if (flag != 'z' && // At least, not before compiler shutdown.
      isPrimitiveType(ast) &&
      toPrimitiveType(ast)->symbol->hasFlag(FLAG_GLOBAL_TYPE_SYMBOL))
    INT_FATAL(ast, "Unexpected attempt to eviscerate a global type symbol.");
}

#define clean_gvec(type)                        \
  int i##type = 0;                              \
  forv_Vec(type, ast, g##type##s) {             \
    if (isAlive(ast) || isRootModuleWithType(ast, type)) { \
      g##type##s.v[i##type++] = ast;            \
    } else {                                    \
      trace_remove(ast, 'x');                   \
      delete ast; ast = 0;                      \
    }                                           \
  }                                             \
  g##type##s.n = i##type


static void clean_modvec(Vec<ModuleSymbol*>& modvec) {
  int aliveMods = 0;
  forv_Vec(ModuleSymbol, mod, modvec) {
    if (isAlive(mod) || isRootModuleWithType(mod, ModuleSymbol)) { 
      modvec.v[aliveMods++] = mod;            
    }                                           
  } 
  modvec.n = aliveMods;
}

void cleanAst() {
  cleanModuleList();
  //
  // clear back pointers to dead ast instances
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    for(int i = 0; i < ts->type->methods.n; i++) {
      FnSymbol* method = ts->type->methods.v[i];
      if (method && !isAliveQuick(method))
        ts->type->methods.v[i] = NULL;
      if (AggregateType* ct = toAggregateType(ts->type)) {
        if (ct->defaultInitializer && !isAliveQuick(ct->defaultInitializer))
          ct->defaultInitializer = NULL;
        if (ct->destructor && !isAliveQuick(ct->destructor))
          ct->destructor = NULL;
      }
    }
    for(int i = 0; i < ts->type->dispatchChildren.n; i++) {
      Type* type = ts->type->dispatchChildren.v[i];
      if (type && !isAlive(type))
        ts->type->dispatchChildren.v[i] = NULL;
    }
  }

  // check iterator-resume-label/goto data before nodes are free'd
  verifyNcleanRemovedIterResumeGotos();
  verifyNcleanCopiedIterResumeGotos();

  // clean the other module vectors, without deleting the ast instances (they
  // will be deleted with the clean_gvec call for ModuleSymbols.) 
  clean_modvec(allModules);
  clean_modvec(userModules);
 
  //
  // clean global vectors and delete dead ast instances
  //
  foreach_ast(clean_gvec);
}


// ModuleSymbols cache a pointer to their initialization function
// This pointer has to be zeroed out specially before the matching function
// definition is deleted from module body.
static void cleanModuleList()
{
  // Walk the module list.
  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    // Zero the initFn pointer if the function is now dead.
    if (mod->initFn && !isAlive(mod->initFn))
      mod->initFn = NULL;
  }
}


void destroyAst() {
  #define destroy_gvec(type)                    \
    forv_Vec(type, ast, g##type##s) {           \
      trace_remove(ast, 'z');                   \
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


int breakOnID = -1;
int breakOnDeleteID = -1;

int lastNodeIDUsed() {
  return uid - 1;
}


// This is here so that we can break on the creation of a particular
// BaseAST instance in gdb.
static void checkid(int id) {
  if (id == breakOnID) {
    gdbShouldBreakHere();
  }
}


BaseAST::BaseAST(AstTag type) :
  astTag(type),
  id(uid++),
  astloc(yystartlineno, yyfilename)
{
  checkid(id);
  if (astloc.filename) {
    // OK, set from yyfilename
  } else {
    if (currentAstLoc.filename) {
      astloc = currentAstLoc;
    } else {
      // neither yy* nor currentAstLoc are set
      INT_FATAL("no line number available");
    }
  }
}


const std::string BaseAST::tabText = "   ";


BaseAST::~BaseAST() { 
}

int BaseAST::linenum() const {
  return astloc.lineno; 
}

const char* BaseAST::fname() const {
  return astloc.filename; 
}

const char* BaseAST::stringLoc(void) const {
  const int tmpBuffSize = 256;
  char tmpBuff[tmpBuffSize];

  snprintf(tmpBuff, tmpBuffSize, "%s:%d", fname(), linenum());
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


Type* BaseAST::getValType() {
  Type* type = typeInfo();
  INT_ASSERT(type);
  if (type->symbol->hasFlag(FLAG_REF))
    return type->getField("_val")->type;
  else if (type->symbol->hasFlag(FLAG_WIDE_REF))
    return type->getField("addr")->getValType();
  else
    return type;
}

Type* BaseAST::getRefType() {
  Type* type = typeInfo();
  INT_ASSERT(type);
  if (type->symbol->hasFlag(FLAG_REF))
    return type;
  else if (type->symbol->hasFlag(FLAG_WIDE_REF))
    return type->getField("addr")->type;
  else
    return type->refType;
}

Type* BaseAST::getWideRefType() {
  Type* type = typeInfo();
  INT_ASSERT(type);
  if (type->symbol->hasFlag(FLAG_REF))
    return wideRefMap.get(type);
  else if (type->symbol->hasFlag(FLAG_WIDE_REF))
    return type;
  else
    return wideRefMap.get(type->getRefType());
}

const char* BaseAST::astTagAsString() const {
  const char* retval = "BaseAST??";

  switch (astTag) {
    case E_SymExpr:
      retval = "SymExpr";
      break;

    case E_UnresolvedSymExpr:
      retval = "UnresolvedSymExpr";
      break;

    case E_DefExpr:
      retval = "DefExpr";
      break;

    case E_CallExpr:
      retval = "CallExpr";
      break;

    case E_NamedExpr:
      retval = "NamedExpr";
      break;

    case E_BlockStmt:
      {
        // see AST_CHILDREN_CALL
        const BlockStmt* stmt = toConstBlockStmt(this);
        if (false) retval = "";
        else if (stmt->isCForLoop())     retval = "CForLoop";
        else if (stmt->isForLoop())      retval = "ForLoop";
        else if (stmt->isParamForLoop()) retval = "ParamForLoop";
        else if (stmt->isWhileDoStmt())  retval = "WhileDoStmt";
        else if (stmt->isDoWhileStmt())  retval = "DoWhileStmt";
        else retval = "BlockStmt";
      }
      break;

    case E_CondStmt:
      retval = "CondStmt";
      break;

    case E_GotoStmt:
      retval = "GotoStmt";
      break;

    case E_ExternBlockStmt:
      retval = "ExternBlockStmt";
      break;

    case E_ModuleSymbol:
      retval = "ModuleSymbol";
      break;

    case E_VarSymbol:
      retval = "VarSymbol";
      break;

    case E_ArgSymbol:
      retval = "ArgSymbol";
      break;

    case E_IpeSymbol:
      retval = "IpeSymbol";
      break;

    case E_TypeSymbol:
      retval = "TypeSymbol";
      break;

    case E_FnSymbol:
      retval = "FnSymbol";
      break;

    case E_EnumSymbol:
      retval = "EnumSymbol";
      break;

    case E_LabelSymbol:
      retval = "LabelSymbol";
      break;

    case E_PrimitiveType:
      retval = "PrimitiveType";
      break;

    case E_EnumType:
      retval = "EnumType";
      break;

    case E_AggregateType:
      retval = "AggregateType";
      break;
  }

  return retval;
}


void BaseAST::printTabs(std::ostream *file, unsigned int tabs) {
  for (int i = 0; i < tabs; i++) {
    *file << this->tabText;
  }
}


std::string BaseAST::docsDirective() {
  return "";
}


// This method is the same for several subclasses of BaseAST, so it is defined
// her on BaseAST. 'doc' is not defined as a member of BaseAST, so it must be
// taken as an argument here.
//
// TODO: Can BaseAST define a 'doc' member? What if `chpl --doc` went away and
//       `chpldoc` was compiled with a special #define (e.g. -DCHPLDOC) so the
//       'doc' member and all doc-related methods would only be available to
//       chpldoc? (thomasvandoren, 2015-02-21)
void BaseAST::printDocsDescription(const char *doc, std::ostream *file, unsigned int tabs) {
  if (doc != NULL) {
    std::stringstream sStream(ltrimAllLines(doc));
    std::string line;
    while (std::getline(sStream, line)) {
      this->printTabs(file, tabs);
      *file << line;
      *file << std::endl;
    }
  }
}


astlocT currentAstLoc(0,NULL);

Vec<ModuleSymbol*> userModules; // Contains user + main modules
Vec<ModuleSymbol*> allModules;  // Contains all modules

void registerModule(ModuleSymbol* mod) {
  switch (mod->modTag) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_STANDARD:
  case MOD_INTERNAL:
    if (strcmp(mod->name, "_root"))
      allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
  }
}

#define SUB_SYMBOL(x)                                   \
  do {                                                  \
    if (x)                                              \
      if (Symbol* y = map->get(x))                      \
        x = y;                                          \
  } while (0)

#define SUB_TYPE(x)                                     \
  do {                                                  \
    if (x)                                              \
      if (Symbol* y = map->get(x->symbol))              \
        x = y->type;                                    \
  } while (0)

void update_symbols(BaseAST* ast, SymbolMap* map) {
  if (SymExpr* sym_expr = toSymExpr(ast)) {
    SUB_SYMBOL(sym_expr->var);

  } else if (DefExpr* defExpr = toDefExpr(ast)) {
    SUB_TYPE(defExpr->sym->type);

  } else if (LoopStmt* ls = toLoopStmt(ast)) {
    LabelSymbol* breakLabel    = ls->breakLabelGet();
    LabelSymbol* continueLabel = ls->continueLabelGet();

    if (breakLabel != 0) {
      if (LabelSymbol* y = toLabelSymbol(map->get(breakLabel))) {
        ls->breakLabelSet(y);
      }
    }

    if (continueLabel != 0) {
      if (LabelSymbol* y = toLabelSymbol(map->get(continueLabel))) {
        ls->continueLabelSet(y);
      }
    }

  } else if (VarSymbol* ps = toVarSymbol(ast)) {
    SUB_TYPE(ps->type);

  } else if (FnSymbol* ps = toFnSymbol(ast)) {
    SUB_TYPE(ps->type);
    SUB_TYPE(ps->retType);
    SUB_SYMBOL(ps->_this);
    SUB_SYMBOL(ps->_outer);

  } else if (ArgSymbol* ps = toArgSymbol(ast)) {
    SUB_TYPE(ps->type);
  }

  AST_CHILDREN_CALL(ast, update_symbols, map);
}

GenRet baseASTCodegen(BaseAST* ast)
{
  GenRet ret = ast->codegen();
  ret.chplType = ast->typeInfo();
  ret.isUnsigned = ! is_signed(ret.chplType);
  return ret;
}

GenRet baseASTCodegenInt(int x)
{
  return baseASTCodegen(new_IntSymbol(x, INT_SIZE_64));
}

GenRet baseASTCodegenString(const char* str)
{
  return baseASTCodegen(new_StringSymbol(str));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool isLoopStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isLoopStmt()) ? true : false;
}

bool isWhileStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isWhileStmt()) ? true : false;
}

bool isWhileDoStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isWhileDoStmt()) ? true : false;
}

bool isDoWhileStmt(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isDoWhileStmt()) ? true : false;
}

bool isParamForLoop(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isParamForLoop()) ? true : false;
}

bool isForLoop(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isForLoop()) ? true : false;
}

bool isCForLoop(const BaseAST* a)
{
  const BlockStmt* stmt = toConstBlockStmt(a);

  return (stmt != 0 && stmt->isCForLoop()) ? true : false;
}

/************************************* | **************************************
*                                                                             *
* Definitions for astlocMarker                                                *
*                                                                             *
************************************** | *************************************/

// constructor, invoked upon SET_LINENO
astlocMarker::astlocMarker(astlocT newAstLoc)
  : previousAstLoc(currentAstLoc)
{
  //previousAstLoc = currentAstLoc;
  currentAstLoc = newAstLoc;
}

// constructor, for special occasions
astlocMarker::astlocMarker(int lineno, const char* filename)
  : previousAstLoc(currentAstLoc)
{
  currentAstLoc.lineno   = lineno;
  currentAstLoc.filename = astr(filename);
}

// destructor, invoked upon leaving SET_LINENO's scope
astlocMarker::~astlocMarker() {
  currentAstLoc = previousAstLoc;
}
