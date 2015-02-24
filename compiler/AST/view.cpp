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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "view.h"

#include "CForLoop.h"
#include "expr.h"
#include "ForLoop.h"
#include "log.h"
#include "ParamForLoop.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "WhileStmt.h"
#include "AstDump.h"
#include "AstDumpToNode.h"

#include <inttypes.h>

int  debugShortLoc  = true;
bool whocalls_nview = false;

static void
list_sym(Symbol* sym, bool type = true) {
  if (VarSymbol* var = toVarSymbol(sym)) {
    if (var->immediate) {
      if (var->immediate->const_kind == NUM_KIND_INT) {
        printf("%"PRId64" ", var->immediate->int_value());
        return;
      } else if (var->immediate->const_kind == CONST_KIND_STRING) {
        printf("\"%s\" ", var->immediate->v_string);
        return;
      }
    }
  }
  if (toFnSymbol(sym)) {
    printf("fn ");
  } else if (toArgSymbol(sym)) {
    printf("arg ");
  } else if (toTypeSymbol(sym)) {
    printf("type ");
  }
  printf("%s", sym->name);
  printf("[%d]", sym->id);
  if (!type) {
    printf(" ");
  } else if (FnSymbol* fn = toFnSymbol(sym)) {
    printf(":%s", fn->retType->symbol->name);
    printf("[%d] ", fn->retType->symbol->id);
  } else if (sym->type && sym->type->symbol) {
    printf(":%s", sym->type->symbol->name);
    printf("[%d] ", sym->type->symbol->id);
  } else {
    printf(" ");
  }
}


static const char*
block_explanation(BaseAST* ast, BaseAST* parentAst) {
  if (ArgSymbol* parentArg = toArgSymbol(parentAst)) {
    if (ast == parentArg->typeExpr)
      return "  typeExpr=";
    if (ast == parentArg->defaultExpr)
      return "  defaultExpr=";
    if (ast == parentArg->variableExpr)
      return "  variableExpr=";
  }
  return "";
}

static bool
list_line(Expr* expr, BaseAST* parentAst) {
  if (expr->isStmt())
    return !*block_explanation(expr, parentAst);
  if (CondStmt* cond = toCondStmt(parentAst)) {
    if (cond->condExpr == expr)
      return false;
  }
  if (Expr* pExpr = toExpr(parentAst))
   if (pExpr->isStmt())
    return true;
  if (isSymbol(parentAst))
    return true;
  return false;
}

static void
list_ast(BaseAST* ast, BaseAST* parentAst = NULL, int indent = 0) {
  bool do_list_line = false;
  bool is_C_loop = false;
  const char* block_explain = NULL;
  if (Expr* expr = toExpr(ast)) {
    do_list_line = !parentAst || list_line(expr, parentAst);
    if (do_list_line) {
      printf("%-7d ", expr->id);
      for (int i = 0; i < indent; i++)
        printf(" ");
    }
    if (GotoStmt* e = toGotoStmt(ast)) {
      printf("goto ");
      if (SymExpr* label = toSymExpr(e->label)) {
        if (label->var != gNil) {
          list_ast(e->label, ast, indent+1);
        }
      } else {
        list_ast(e->label, ast, indent+1);
      }
    } else if (toBlockStmt(ast)) {
      block_explain = block_explanation(ast, parentAst);
      printf("%s{\n", block_explain);
    } else if (toCondStmt(ast)) {
      printf("if ");
    } else if (CallExpr* e = toCallExpr(expr)) {
      if (e->isPrimitive(PRIM_BLOCK_C_FOR_LOOP))
          is_C_loop = true;
      if (e->primitive)
        printf("%s( ", e->primitive->name);
      else
        printf("call( ");
    } else if (NamedExpr* e = toNamedExpr(expr)) {
      printf("%s = ", e->name);
    } else if (toDefExpr(expr)) {
      printf("def ");
    } else if (SymExpr* e = toSymExpr(expr)) {
      list_sym(e->var, false);
    } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(expr)) {
      printf("%s ", e->unresolved);
    }
  }

  if (Symbol* sym = toSymbol(ast))
    list_sym(sym);

  bool early_newline = toFnSymbol(ast) || toModuleSymbol(ast); 
  if (early_newline || is_C_loop)
    printf("\n");

  int new_indent = indent;

  if (isExpr(ast))
    if (do_list_line)
      new_indent = indent+2;

  AST_CHILDREN_CALL(ast, list_ast, ast, new_indent);

  if (Expr* expr = toExpr(ast)) {
    CallExpr* parent_C_loop = NULL;
    if (CallExpr* call = toCallExpr(parentAst))
      if (call->isPrimitive(PRIM_BLOCK_C_FOR_LOOP))
        parent_C_loop = call;
    if (toCallExpr(expr)) {
      printf(") ");
    }
    if (toBlockStmt(ast)) {
      printf("%-7d ", expr->id);
      if (*block_explain)
        indent -= 2;
      for (int i = 0; i < indent; i++)
        printf(" ");
      if ((parent_C_loop && parent_C_loop->get(3) == expr) || *block_explain)
        printf("} ");
      else
        printf("}\n");
    } else if (CondStmt* cond = toCondStmt(parentAst)) {
      if (cond->condExpr == expr)
        printf("\n");
    } else if (!toCondStmt(expr) && do_list_line) {
      DefExpr* def = toDefExpr(expr);
      if (!(def && early_newline))
        if (!parent_C_loop)
          printf("\n");
    }
  }
}


// If I invoke nprint_view et al. in aid(1) by accident,
// the debugger prints the entire program, which is too much.
// This safeguards against that.
// As of this writing, id >= 6 is OK; we use 9 to be slightly conservative.
static bool aidIgnore(int id) { return id < 9; }

static const char*
aidErrorMessage(const char* callerMsg, int id, const char* errorMsg) {
  const int tmpBuffSize = 256;
  static char tmpBuff[tmpBuffSize];
  snprintf(tmpBuff, tmpBuffSize, "<%s%s""the ID %d %s>",
           callerMsg ? callerMsg : "", callerMsg ? ": " : "",
           id, errorMsg);
  return tmpBuff;
}
static const char* aidNotFoundError(const char* callerMsg, int id) {
  return aidErrorMessage(callerMsg, id, "does not correspond to an AST node");
}
static const char* aidIgnoreError(const char* callerMsg, int id) {
  return aidErrorMessage(callerMsg, id,
                         " is small, use aid09(id) to examine it");
}

// This version of aid*() does not exlude any id.
BaseAST* aid09(int id) {
  #define match_id(type)                        \
    forv_Vec(type, a, g##type##s) {             \
      if (a->id == id)                          \
        return a;                               \
    }
  foreach_ast(match_id);
  return NULL;
}

static BaseAST* aidWithError(int id, const char* callerMsg) {
  if (aidIgnore(id)) {
    printf("%s\n", aidIgnoreError(callerMsg, id));
    return NULL;
  } else {
    BaseAST* result = aid09(id);
    if (!result) printf("%s\n", aidNotFoundError(callerMsg, id));
    return result;
  }
}

BaseAST* aid(int id) {
  return aidWithError(id, "aid");
}


void viewFlags(int id) {
  if (BaseAST* ast = aidWithError(id, "viewFlags"))
    viewFlags(ast);
}


static void view_sym(Symbol* sym, bool number, int mark) {
  putchar('\'');
  if (sym->id == mark)
    printf("***");
  if (toFnSymbol(sym)) {
    printf("fn ");
  } else if (toArgSymbol(sym)) {
    printf("arg ");
  } else if (toTypeSymbol(sym)) {
    printf("type ");
  }
  printf("%s", sym->name);
  if (number)
    printf("[%d]", sym->id);
  if (FnSymbol* fn = toFnSymbol(sym)) {
    printf(":%s", fn->retType->symbol->name);
    if (number)
      printf("[%d]", fn->retType->symbol->id);
  } else if (sym->type && sym->type->symbol) {
    printf(":%s", sym->type->symbol->name);
    if (number)
      printf("[%d]", sym->type->symbol->id);
  }
  if (sym->hasFlag(FLAG_GENERIC))
    putchar('?');
  putchar('\'');
}


static void
view_ast(BaseAST* ast, bool number = false, int mark = -1, int indent = 0) {
  if (!ast)
    return;
  if (Expr* expr = toExpr(ast)) {
    printf("\n");
    for (int i = 0; i < indent; i++)
      printf(" ");
    printf("(");
    if (ast->id == mark)
      printf("***");
    if (number)
      printf("%d ", ast->id);
    printf("%s", expr->astTagAsString());

    if (isBlockStmt(expr))
      if (FnSymbol* fn = toFnSymbol(expr->parentSymbol))
        if (expr == fn->where)
          printf(" where");

    if (GotoStmt *gs= toGotoStmt(ast)) {
      printf( " ");
      view_ast(gs->label, number, mark, indent+1);
    }

    if (CallExpr* call = toCallExpr(expr))
      if (call->primitive)
        printf(" %s", call->primitive->name);

    if (NamedExpr* named = toNamedExpr(expr))
      printf(" \"%s\"", named->name);

    if (toDefExpr(expr))
      printf(" ");

    int64_t i;
    const char *str;
    if (get_int(expr, &i)) {
      printf(" %" PRId64, i);
    } else if (get_string(expr, &str)) {
      printf(" \"%s\"", str);
    }

    if (SymExpr* sym = toSymExpr(expr)) {
      printf(" ");
      view_sym(sym->var, number, mark);
    } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
      printf(" '%s'", sym->unresolved);
    }
  }

  if (Symbol* sym = toSymbol(ast)) {
    view_sym(sym, number, mark);
  }

  AST_CHILDREN_CALL(ast, view_ast, number, mark, indent+2);

  if (toExpr(ast))
    printf(")");
}

// complement view_ast(), which does not do justice to types
static void type_nprint_view(BaseAST* ast) {
  if (Type* type = toType(ast))
    if (Symbol* ts = type->symbol)
      printf(":%s[%d]\n", ts->name, type->id);
}

static void type_print_view(BaseAST* ast) {
  if (Type* type = toType(ast))
    if (Symbol* ts = type->symbol) {
      printf(":%s", ts->name);
      if (type->symbol->hasFlag(FLAG_GENERIC))
        putchar('?');
      putchar('\n');
    }
}

void list_view(int id) {
  if (BaseAST* ast = aidWithError(id, "list_view"))
    list_view(ast);
}

void list_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    if (toSymbol(ast))
      printf("%-7d ", ast->id);
    list_ast(ast);
  }
  printf("\n");
  fflush(stdout);
}

void list_view_noline(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    if (toSymbol(ast))
      printf("%-7d ", ast->id);
    list_ast(ast);
  }
  fflush(stdout);
}

void print_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_print_view(ast);
    view_ast(ast);
  }
  printf("\n\n");
  fflush(stdout);
}

void print_view_noline(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_print_view(ast);
    view_ast(ast);
  }
  fflush(stdout);
}

void nprint_view(int id) {
  if (BaseAST* ast = aidWithError(id, "nprint_view"))
    nprint_view(ast);
}

void nprint_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_nprint_view(ast);
    view_ast(ast, true);
  }
  printf("\n\n");
  fflush(stdout);
}

void nprint_view_noline(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_nprint_view(ast);
    view_ast(ast, true);
  }
  fflush(stdout);
}


void iprint_view(int id) {
  if (BaseAST* ast = aidWithError(id, "iprint_view"))
    nprint_view(ast);
}


void mark_view(BaseAST* ast, int id) {
  view_ast(ast, false, id);
  printf("\n\n");
  fflush(stdout);
}


// feel free to propose a better name
void astDump_view(int id) {
  if (BaseAST* ast = aidWithError(id, "astDump_view"))
    astDump_view(ast);
}

void astDump_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    AstDump logger(stdout);
    ast->accept(&logger);
  }
  printf("\n\n");
  fflush(stdout);
}


// feel free to propose a better name
void astDumpToNode_view(int id) {
  if (BaseAST* ast = aidWithError(id, "astDumpToNode_view"))
    astDumpToNode_view(ast);
}

void astDumpToNode_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    AstDumpToNode logger(stdout);
    ast->accept(&logger);
  }
  printf("\n\n");
  fflush(stdout);
}


static bool log_need_space;

static void log_ast_symbol(FILE* file, Symbol* sym, bool def);

static inline void log_write(FILE* file, bool space_before, const char* text, bool space_after) {
  if (space_before && log_need_space)
    putc(' ', file);
  fputs(text, file);
  log_need_space = space_after;
}

static void
log_ast_symbol(FILE* file, Symbol* sym, bool def) {
  if (log_need_space) putc(' ', file);
  log_need_space = false;

  if (def)
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      log_need_space = true;
      switch (arg->intent) {
       case INTENT_IN:      fprintf(file, "in");        break;
       case INTENT_INOUT:   fprintf(file, "inout");     break;
       case INTENT_OUT:     fprintf(file, "out");       break;
       case INTENT_CONST:   fprintf(file, "const");     break;
       case INTENT_CONST_IN:  fprintf(file, "const in");  break;
       case INTENT_CONST_REF: fprintf(file, "const ref"); break;
       case INTENT_REF:     fprintf(file, "ref");       break;
       case INTENT_PARAM:   fprintf(file, "param");     break;
       case INTENT_TYPE:    fprintf(file, "type");      break;
      default: log_need_space = false; break;
      }
      log_write(file, true, "arg", true);
    }

  log_write(file, true, sym->name, true);

  if (fLogIds)
    fprintf(file, "[%d]", sym->id);

  if (def &&
      !toTypeSymbol(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    log_write(file, false, ":", false);
    log_ast_symbol(file, sym->type->symbol, false);
  }

  if (sym->hasFlag(FLAG_GENERIC))
    log_write(file, false, "?", false);

  log_need_space = true;
}


//
// stringLoc, shortLoc, debugLoc: return "file:line", where 'file' is:
//  - stringLoc: AST's fname()
//  - shortLoc: just the file part of that (no path)
//  - debugLoc: one of the above, depending on --debug-short-loc
// NB may return the same static buffer - limit to one call per printf().
// Cf. no restrictions on BaseAST::stringLoc().
//

#define locBuffSize 256
static char locBuff[locBuffSize];

const char* stringLoc(int id) {
  BaseAST* ast = aid09(id);
  return ast ? stringLoc(ast) : aidNotFoundError("stringLoc", id);
}

const char* shortLoc(int id) {
  BaseAST* ast = aid09(id);
  return ast ? shortLoc(ast) : aidNotFoundError("shortLoc", id);
}

const char* debugLoc(int id) {
  return debugShortLoc ? shortLoc(id) : stringLoc(id);
}

const char* stringLoc(BaseAST* ast) {
  if (!ast)
    return "<no node provided>";

  snprintf(locBuff, locBuffSize, "%s:%d", ast->fname(), ast->linenum());
  return locBuff;
}

const char* shortLoc(BaseAST* ast) {
  if (!ast)
    return "<no node provided>";

  const char* longLoc = stringLoc(ast);
  const char* slash = (const char*) rindex(longLoc, '/');
  return slash ? slash+1 : longLoc;
}

const char* debugLoc(BaseAST* ast) {
  return debugShortLoc ? shortLoc(ast) : stringLoc(ast);
}

// helper for printing IDs
int debugID(int id) {
  return id;
}
int debugID(BaseAST* ast) {
  return ast ? ast->id : -1;
}


//
// map_view: print the contents of a SymbolMap
//
void map_view(SymbolMap* map) {
  map_view(*map);
}

void map_view(SymbolMap& map) {
  printf("SymbolMap at %p\n", &map);
  int cnt = 0;
  bool temp_log_need_space = log_need_space;
  log_need_space = true;
  
  form_Map(SymbolMapElem, elm, map) {
    Symbol* key = elm->key;
    Symbol* val = elm->value;
    if (key || val) {
      cnt++;
      printf(" ");
      if (key) {
        if (!fLogIds) printf(" [%d]", key->id);
        log_ast_symbol(stdout, key, true);
      } else {
        printf("NULL");
      }
      if (val != NULL && val != gNil) {
        printf("  => ");
        if (!fLogIds) printf(" [%d]", val->id);
        log_ast_symbol(stdout, val, true);
      } else {
        // nothing
      }
      printf("\n");
    }
  }
  printf("  %d elm(s)\n", cnt);
  log_need_space = temp_log_need_space;
}


//
// vec_view: print the contents of a Vec.
// todo: add STL versions
//
void vec_view(Vec<Symbol*,VEC_INTEGRAL_SIZE>* v) {
  vec_view(*v);
}

void vec_view(Vec<Symbol*,VEC_INTEGRAL_SIZE>& v)
{
  printf("Vec<Symbol> %d elm(s)\n", v.n);
  for (int i = 0; i < v.n; i++) {
    Symbol* elm = v.v[i];
    if (elm)
      printf("%3d %8d  %s\n", i, elm->id, elm->name);
    else
      printf("%3d <null>\n", i);
  }
}

void vec_view(Vec<FnSymbol*,VEC_INTEGRAL_SIZE>* v) {
  vec_view(*v);
}

void vec_view(Vec<FnSymbol*,VEC_INTEGRAL_SIZE>& v)
{
  printf("Vec<FnSymbol> %d elm(s)\n", v.n);
  for (int i = 0; i < v.n; i++) {
    Symbol* elm = v.v[i];
    if (elm)
      printf("%3d %8d  %s\n", i, elm->id, elm->name);
    else
      printf("%3d <null>\n", i);
  }
}

//
// fnsWithName: print all FnSymbols with the given name
//
void fnsWithName(const char* name) {
  fnsWithName(name, gFnSymbols);
}

void fnsWithName(const char* name, Vec<FnSymbol*,VEC_INTEGRAL_SIZE>& fnVec) {
  printf("fnsWithName(\"%s\")\n", name);
  int count = 0, countNonNull = 0;
  forv_Vec(FnSymbol, fn, fnVec) {
    if (fn) {
      countNonNull++;
      if (!strcmp(fn->name, name)) {
        count++;
        printf("  %d  %s\n", fn->id, debugLoc(fn));
      }
    }
  }
  printf("  %d function(s) of %d\n", count, countNonNull);
}

//
// whocalls: print all CallExprs whose baseExpr is the given SymExpr or Symbol
//
void whocalls(BaseAST* ast) {
  if (!ast) {
    printf("whocalls: aborting: got NULL\n");
    return;
  }
  printf("whocalls(%s[%d])\n", ast->astTagAsString(), ast->id);
  if (SymExpr* se = toSymExpr(ast)) {
    whocalls(se->var->id);
  } else if (isSymbol(ast)) {
    whocalls(ast->id);
  } else {
    printf("whocalls: aborting: need a SymExpr or Symbol\n");
  }
}

static char* parentMsg(Expr* expr, int* cntInTreeP, int* cntNonTreeP) {
  static char result[128];
  if (expr->inTree()) {
    (*cntInTreeP)++;
    sprintf(result, "psym %d", expr->parentSymbol->id);
  } else {
    (*cntNonTreeP)++;
    sprintf(result, "<not in tree>");
  }
  return result;
}

// 'id' better be a Symbol
void whocalls(int id) {
  int callAll = 0, callMatch = 0, callNonTreeMatch = 0;
  forv_Vec(CallExpr, call, gCallExprs) {
    if (SymExpr* se = toSymExpr(call->baseExpr)) {
      callAll++;
      if (se->var->id == id) {
        printf("  call %d  %s  %s\n", call->id,
               parentMsg(call, &callMatch, &callNonTreeMatch), debugLoc(call));
        if (whocalls_nview) nprint_view(call);
      }
    }
  }

  int forAll = 0, forMatch = 0, forNonTreeMatch = 0;
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_BLOCK_FOR_LOOP) && call->numActuals() >= 2) {
      forAll++;
      // check each step, just in case
      if (SymExpr* act2 = toSymExpr(call->get(2)))
        if (Symbol* ic = act2->var)
          if (Type* ty = ic->type)
            if (FnSymbol* init = ty->defaultInitializer)
              if (ArgSymbol* form1 = init->getFormal(1))
                if (Type* fty = form1->type)
                  if (FnSymbol* iterator = fty->defaultInitializer)
                    if (iterator->id == id)
                      printf("  for-loop blockInfo %d  %s  %s\n",
                             call->id, parentMsg(call, &forMatch,
                               &forNonTreeMatch), debugLoc(call));
    }
  }

  int vmtMatch = 0, vmtAll = 0;
  for (int i = 0; i < virtualMethodTable.n; i++) {
    if (virtualMethodTable.v[i].key) {
      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
        vmtAll++;
        if (virtualMethodTable.v[i].value->v[j]->id == id) {
          vmtMatch++;
          printf("  VMT[%d][%d]\n", i, j);
        }
      }
    }
  }

  int ftMatch = 0, ftAll = ftableVec.n;
  for (int i = 0; i < ftAll; i++) {
    if (ftableVec.v[i]->id == id) {
      ftMatch++;
      printf("  ftableVec[%d]\n", i);
    }
  }

  printf("  %d of %d calls", callMatch, callAll);
  if (callNonTreeMatch) printf(", also %d not in tree", callNonTreeMatch);
  printf(".  %d of %d for-loops", forMatch, forAll);
  if (forNonTreeMatch) printf(", also %d not in tree", forNonTreeMatch);
  printf(".  %d of %d in VMT.  %d of %d in FT.\n",
         vmtMatch, vmtAll, ftMatch, ftAll);
}
