/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "AstDump.h"
#include "AstDumpToNode.h"
#include "CForLoop.h"
#include "CatchStmt.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "IfExpr.h"
#include "ImportStmt.h"
#include "iterator.h"
#include "log.h"
#include "LoopExpr.h"
#include "ParamForLoop.h"
#include "ResolutionCandidate.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "TryStmt.h"
#include "virtualDispatch.h"
#include "WhileStmt.h"

#include <inttypes.h>

int  debugShortLoc  = true;
bool whocalls_nview = false;

static void print_indent(int indent) {
  for (int i = 0; i < indent; i++) printf(" ");
}

static void print_on_its_own_line(int indent, const char* msg,
                                  bool newline = true) {
  if (newline) printf("\n");
  printf("%6c", ' ');
  print_indent(indent);
  printf("%s", msg);
}

static void
list_sym(const Symbol* sym, bool type = true) {
  if (const VarSymbol* var = toConstVarSymbol(sym)) {
    if (var->immediate) {
      if (var->immediate->const_kind == NUM_KIND_INT) {
        printf("%" PRId64 " ", var->immediate->int_value());
        return;
      } else if (var->immediate->const_kind == CONST_KIND_STRING) {
        printf("\"%s\" ", var->immediate->v_string);
        return;
      }
    }
  }
  if (isFnSymbol(sym)) {
    printf("fn ");
  } else if (const ArgSymbol* arg = toConstArgSymbol(sym)) {
    printf("arg intent-%s ", arg->intentDescrString());
  } else if (isTypeSymbol(sym)) {
    printf("type ");
  } else if (isInterfaceSymbol(sym)) {
    printf("interface ");
  }
  printf("%s", sym->name);
  printf("[%d]", sym->id);
  if (!type) {
    printf(" ");
  } else if (const FnSymbol* fn = toConstFnSymbol(sym)) {
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
block_explanation(const BaseAST* ast, const BaseAST* parentAst) {
  if (const ArgSymbol* parentArg = toConstArgSymbol(parentAst)) {
    if (ast == parentArg->typeExpr)
      return "  typeExpr=";
    if (ast == parentArg->defaultExpr)
      return "  defaultExpr=";
    if (ast == parentArg->variableExpr)
      return "  variableExpr=";
  }
  return "";
}

static const char*
forall_explanation_start(const BaseAST* ast, const BaseAST* parentAst) {
  if (const LoopExpr* fe = toConstLoopExpr(parentAst)) {
    if (ast == fe->iteratorExpr)
      return ") in( ";
    if (ast == fe->loopBody)
      return ") { ";
    if (ast == fe->cond)
      return "} if( ";
  }
  if (isDeferStmt(ast)) {
    return "defer\n";
  }
  return nullptr;
}

static void forallPreamble(const Expr* expr, const BaseAST* parentAst,
                           int indent) {
  if (const ForallStmt* pfs = toConstForallStmt(parentAst)) {
    if (expr == pfs->fRecIterIRdef) {
      print_on_its_own_line(indent, "fRecIterIRdef et al.\n");
    } else if (expr == pfs->loopBody()) {
      if (pfs->numShadowVars() == 0)
        print_on_its_own_line(indent, "with() do\n", !pfs->fRecIterIRdef);
      else
        print_on_its_own_line(indent, "do\n", false);
    }
  } else if (const ShadowVarSymbol* svar = toConstShadowVarSymbol(parentAst)) {
    if (expr == svar->outerVarSE                          ||
        ( expr == svar->initBlock() && !svar->outerVarSE ) )
      printf("\n");
  } else if (const InterfaceSymbol* isym = toConstInterfaceSymbol(parentAst)) {
    if (expr->list && expr->list->parent == isym)
      printf("\n");
  } else if (const ImplementsStmt* istm = toConstImplementsStmt(parentAst)) {
    if (expr == istm->implBody)
      printf("\n");
  }
}

static void forallPostamble(const Expr* expr, const ForallStmt* pfs,
                            int indent) {
  if (AList* list = expr->list) {
    if (list == &pfs->constInductionVariables()  ||
        list == &pfs->constIteratedExpressions() ) {
      if (expr != list->tail)
        printf("\n");
      if (expr == pfs->constInductionVariables().tail) {
        print_on_its_own_line(indent, pfs->zippered() ? "in zip\n" : "in\n");
      } else if (expr == pfs->constIteratedExpressions().tail &&
                 pfs->numShadowVars() > 0) {
        print_on_its_own_line(indent, "with\n");
      }
    }
  } else {
    if (expr == pfs->fRecIterIRdef        ||
        expr == pfs->fRecIterICdef        ||
        expr == pfs->fRecIterGetIterator  ||
        expr == pfs->fRecIterFreeIterator )
      printf("\n");

  }
}

static void usePostamble(const UseStmt* use, int indent) {
  if (use->isARename()) {
    printf("as %s ", use->getRename());
  }

  if (use->isPlainUse())
    return;

  if (use->hasExceptList()) {
    printf("except ");
  } else {
    printf("only ");
  }

  bool first = true;

  for_vector(const char, str, use->named) {
    if (first) {
      first = false;
    } else {
      printf(", ");
    }
    printf("%s", str);
  }

  for (std::map<const char*, const char*>::const_iterator it =
         use->renamed.begin(); it != use->renamed.end(); ++it) {
    if (first) {
      first = false;
    } else {
      printf(", ");
    }
    printf("%s as %s", it->second, it->first);
  }

  printf("\n");
}

static void importPostamble(const ImportStmt* import, int indent) {
  if (import->isARename()) {
    printf("as %s ", import->getRename());
  }

  if (import->providesUnqualifiedAccess()) {
    printf(". {");
    bool first = true;

    for_vector(const char, str, import->unqualified) {
      if (first) {
        first = false;
      } else {
        printf(", ");
      }
      printf("%s", str);
    }

    for (std::map<const char*, const char*>::const_iterator it =
           import->renamed.begin(); it != import->renamed.end(); ++it) {
      if (first) {
        first = false;
      } else {
        printf(", ");
      }
      printf("%s as %s", it->second, it->first);
    }
    printf("}\n");
  }
}

static bool
list_line(const Expr* expr, const BaseAST* parentAst) {
  if (expr->isStmt())
    return !*block_explanation(expr, parentAst);
  if (const CondStmt* cond = toConstCondStmt(parentAst))
    return cond->condExpr != expr;
  if (const GotoStmt* gts = toConstGotoStmt(parentAst))
    return gts->label != expr;
  if (const Expr* pExpr = toConstExpr(parentAst))
    return pExpr->isStmt() &&
      !isUseStmt(pExpr) && !isImportStmt(pExpr) && !isImplementsStmt(pExpr);
  if (isSymbol(parentAst))
    return true;
  return false;
}

static void
list_ast(const BaseAST* ast, const BaseAST* parentAst = nullptr, int indent = 0) {
  bool do_list_line = false;
  bool is_C_loop = false;
  bool empty_block = false;
  const char* block_explain = nullptr;
  if (const Expr* expr = toConstExpr(ast)) {
    forallPreamble(expr, parentAst, indent);
    do_list_line = !parentAst || list_line(expr, parentAst);
    if (do_list_line) {
      printf("%-7d ", expr->id);
      print_indent(indent);
    }
    if (const char* expl = forall_explanation_start(ast, parentAst))
      printf("%s", expl);
    if (const GotoStmt* e = toConstGotoStmt(ast)) {
      printf("goto %s ", gotoTagToString(e->gotoTag));
    } else if (const BlockStmt* block = toConstBlockStmt(ast)) {
      block_explain = block_explanation(ast, parentAst);
      const char* block_kind = ast->astTagAsString();
      if (!strcmp(block_kind, "BlockStmt")) block_kind = "";
      printf("%s{%s", block_explain, block_kind);
      if (block->isRealBlockStmt() && block->length() == 0)
        empty_block = true;
      printf("%s", empty_block ? " " : "\n");
    } else if (isCondStmt(ast)) {
      printf("if ");
    } else if (isIfExpr(ast)) {
      printf("IfExpr ");
    } else if (isForallStmt(ast)) {
      printf("forall\n");
    } else if (const CallExpr* e = toConstCallExpr(expr)) {
      if (e->isPrimitive(PRIM_BLOCK_C_FOR_LOOP))
          is_C_loop = true;
      if (e->primitive)
        printf("%s( ", e->primitive->name);
      else
        printf("call( ");
    } else if (const LoopExpr* e = toConstLoopExpr(expr)) {
      if (e->zippered) printf("zip ");
      printf("forall( ");
    } else if (const NamedExpr* e = toConstNamedExpr(expr)) {
      printf("%s = ", e->name);
    } else if (toConstDefExpr(expr)) {
      Symbol* sym = toConstDefExpr(expr)->sym;
      if (sym->type != nullptr) {
        printf("def %s ", sym->qualType().qualStr());
      } else {
        printf("def ");
      }
    } else if (const SymExpr* e = toConstSymExpr(expr)) {
      list_sym(e->symbol(), false);
    } else if (const UnresolvedSymExpr* e = toConstUnresolvedSymExpr(expr)) {
      printf("%s ", e->unresolved);
    } else if (isUseStmt(expr)) {
      printf("use ");
    } else if (isImportStmt(expr)) {
      printf("import ");
    } else if (isImplementsStmt(expr)) {
      printf("implements stmt ");
    } else if (isIfcConstraint(expr)) {
      if (parentAst == nullptr || !isImplementsStmt(parentAst))
        printf("ifc constraint (");
      else
        printf("(");
    }
  }

  if (const Symbol* sym = toConstSymbol(ast))
    list_sym(sym);

  bool early_newline = isFnSymbol(ast) || isModuleSymbol(ast);
  if (early_newline || is_C_loop)
    printf("\n");

  int new_indent = indent;

  if (isExpr(ast))
    if (do_list_line)
      new_indent = indent+2;

  AST_CHILDREN_CALL(ast, list_ast, ast, new_indent);

  if (const Expr* expr = toConstExpr(ast)) {
    bool parentIsCLoop = isCallExpr(parentAst) &&
      toConstCallExpr(parentAst)->isPrimitive(PRIM_BLOCK_C_FOR_LOOP);
    const CallExpr* parent_C_loop = parentIsCLoop ? toConstCallExpr(parentAst) :
      nullptr;
    if (isCallExpr(expr) || isIfcConstraint(expr)) {
      printf(") ");
    }
    if (isBlockStmt(ast)) {
      if (*block_explain)
        indent -= 2;
      if (!empty_block) {
        printf("%-7d ", expr->id);
        print_indent(indent);
      }
      if ((parent_C_loop && parent_C_loop->get(3) == expr) || *block_explain)
        printf("} ");
      else if (isDeferStmt(parentAst))
        printf("}"); // newline is coming
      else
        printf("}\n");
      if (isConstForallLoopBody(expr) && parentAst != nullptr) {
        print_indent(indent);
        printf("      end forall %d", parentAst->id);
      }
    } else if (const LoopExpr* e = toConstLoopExpr(expr)) {
      if (e->cond) printf(") ");
      else         printf("} ");
    } else if (const UseStmt* use = toConstUseStmt(expr)) {
      usePostamble(use, indent);
    } else if (const ImportStmt* import = toConstImportStmt(expr)) {
      importPostamble(import, indent);
    } else if (const CondStmt* cond = toConstCondStmt(parentAst)) {
      if (cond->condExpr == expr)
        printf("\n");
    } else if (const ForallStmt* pfs = toConstForallStmt(parentAst)) {
      forallPostamble(expr, pfs, indent);
    } else if (!isCondStmt(expr) && do_list_line) {
      const DefExpr* def = toConstDefExpr(expr);
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

// This version of aid*() does not exclude any id.
BaseAST* aid09(int id) {
  #define match_id(type)                        \
    forv_Vec(type, a, g##type##s) {             \
      if (a->id == id)                          \
        return a;                               \
    }
  foreach_ast(match_id);
  return nullptr;
}

static BaseAST* aidWithError(int id, const char* callerMsg) {
  if (aidIgnore(id)) {
    printf("%s\n", aidIgnoreError(callerMsg, id));
    return nullptr;
  } else {
    BaseAST* result = aid09(id);
    if (!result) printf("%s\n", aidNotFoundError(callerMsg, id));
    return result;
  }
}

BaseAST* aid(int id) {
  return aidWithError(id, "aid");
}

BaseAST* aid(BaseAST* ast) {
  return ast;
}

Expr* aidExpr(int id) {
  if (BaseAST* ast = aidWithError(id, "aidExpr"))
    return aidExpr(ast);
  else
    return nullptr;
}

Expr* aidExpr(BaseAST* ast) {
  if (Expr* expr = toExpr(ast))
    return expr;
  if (ast != nullptr)
    printf("<aidExpr: node %d is a %s, not an Expr>\n",
           ast->id, ast->astTagAsString());
  return nullptr;
}

// inTree(): a workaround for an lldb bug
bool inTree(BaseAST* ast) {
  return ast->inTree();
}
bool inTree(int id) {
  if (BaseAST* ast = aid(id)) {
    return inTree(ast);
  } else {
    printf("%s\n", aidNotFoundError("inTree", id));
    return false;
  }
}


void viewFlags(int id) {
  if (BaseAST* ast = aidWithError(id, "viewFlags"))
    viewFlags(ast);
}


static void view_sym(Symbol* sym, bool number, int mark) {
  putchar('\'');
  if (sym->id == mark)
    printf("***");

  if (isVarSymbol(sym) || isArgSymbol(sym)) {
    QualifiedType qual = sym->qualType();
    printf("%s ", qual.qualStr());
  }

  if (toFnSymbol(sym)) {
    printf("fn ");
  } else if (toArgSymbol(sym)) {
    printf("arg ");
  } else if (toTypeSymbol(sym)) {
    printf("type ");
  } else if (isInterfaceSymbol(sym)) {
    printf("interface ");
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
  if (sym->isKnownToBeGeneric())
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

    if (GotoStmt *gs= toGotoStmt(ast))
      printf(" %s", gotoTagToString(gs->gotoTag));

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
      view_sym(sym->symbol(), number, mark);
    } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
      printf(" '%s'", sym->unresolved);
    }
  }

  if (Symbol* sym = toSymbol(ast)) {
    view_sym(sym, number, mark);
  }

  if (DefExpr* def = toDefExpr(ast)) {
    printf(" ");
    if (ArgSymbol* arg = toArgSymbol(def->sym))
      printf("intent-%s ", arg->intentDescrString());
    if (ShadowVarSymbol* sv = toShadowVarSymbol(def->sym)) {
      printf("shadow-%s ", sv->intentDescrString());
    }

    writeFlags(stdout, def->sym);
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
  if (const BaseAST* ast = aidWithError(id, "list_view"))
    list_view(ast);
}

void list_view(const BaseAST* ast) {
  if (ast==nullptr) {
    printf("<NULL>");
  } else {
    if (isSymbol(ast))
      printf("%-7d ", ast->id);
    list_ast(ast);
  }
  printf("\n");
  fflush(stdout);
}

void list_view_noline(const BaseAST* ast) {
  if (ast==nullptr) {
    printf("<NULL>");
  } else {
    if (isSymbol(ast))
      printf("%-7d ", ast->id);
    list_ast(ast);
  }
  fflush(stdout);
}

void print_view(BaseAST* ast) {
  if (ast==nullptr) {
    printf("<NULL>");
  } else {
    type_print_view(ast);
    view_ast(ast);
  }
  printf("\n\n");
  fflush(stdout);
}

void print_view_noline(BaseAST* ast) {
  if (ast==nullptr) {
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
  if (ast==nullptr) {
    printf("<NULL>");
  } else {
    type_nprint_view(ast);
    view_ast(ast, true);
  }
  printf("\n\n");
  fflush(stdout);
}

void nprint_view_noline(BaseAST* ast) {
  if (ast==nullptr) {
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
  if (ast==nullptr) {
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
  if (ast==nullptr) {
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

  if (sym->isKnownToBeGeneric())
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
  const char* slash = (const char*) strrchr(longLoc, '/');
  return slash ? slash+1 : longLoc;
}

const char* debugLoc(BaseAST* ast) {
  return debugShortLoc ? shortLoc(ast) : stringLoc(ast);
}

// debugID: helper for printing IDs, opposite to aid()
int debugID(int id) {
  return id;
}
int debugID(BaseAST* ast) {
  return ast ? ast->id : -1;
}

// "debug summary": print key pieces of info
void debugSummary(int id) {
  if (BaseAST* ast = aid09(id))
    debugSummary(ast);
  else
    printf("%s\n", aidNotFoundError("debugSummary", id));
}
void debugSummary(BaseAST* ast) {
  if (ast)
    printf("%d   %s   %s\n", ast->id, ast->astTagAsString(), debugLoc(ast));
  else
    printf("<debugSummary: NULL>\n");
}
// these just print out the argument
void debugSummary(const char* str) {
  if (str)
    printf("\"%s\"\n", str);
  else
    printf("<debugSummary: NULL string>\n");
}
void debugSummary(bool b) {
  printf("%s\n", b ? "true" : "false");
}

// find the Parent Symbol
Symbol* debugParentSym(int id) {
  if (BaseAST* ast = aid09(id))
    return debugParentSym(ast);
  else {
    printf("%s\n", aidNotFoundError("debugParentSym", id));
    return nullptr;
  }
}
Symbol* debugParentSym(BaseAST* ast) {
  if (!ast)
    return nullptr;
  else if (Expr* expr = toExpr(ast))
    return expr->parentSymbol;
  else if (Symbol* sym = toSymbol(ast))
    return sym->defPoint ? sym->defPoint->parentSymbol : nullptr;
  else {
    printf("<debugParentSym: node %d is neither Expr nor Symbol>\n", ast->id);
    return nullptr;
  }
}

// find the Parent Expression
Expr* debugParentExpr(int id) {
  if (BaseAST* ast = aid09(id))
    return debugParentExpr(ast);
  else {
    printf("%s\n", aidNotFoundError("debugParentExpr", id));
    return nullptr;
  }
}
Expr* debugParentExpr(BaseAST* ast) {
  if (!ast)
    return nullptr;
  else if (Expr* expr = toExpr(ast))
    return expr->parentExpr;
  else if (Symbol* sym = toSymbol(ast))
    return sym->defPoint ? sym->defPoint->parentExpr : nullptr;
  else {
    printf("<debugParentExpr: node %d is neither Expr nor Symbol>\n", ast->id);
    return nullptr;
  }
}

//
// blockSummary: print minimal information about each statement in the block
//

void blockSummary(int id) {
  if (BaseAST* ast = aid09(id))
    blockSummary(ast);
  else
    printf("%s\n", aidNotFoundError("blockSummary", id));
}
void blockSummary(BaseAST* ast) {
  if (ast == nullptr)
    printf("<blockSummary: NULL>\n");
  else if (BlockStmt* block = toBlockStmt(ast))
    blockSummary(block, nullptr);
  else if (FnSymbol* fn = toFnSymbol(ast))
    blockSummary(fn->body, fn);
  else if (ModuleSymbol* mod = toModuleSymbol(ast))
    blockSummary(mod->block, mod);
  else if (DefExpr* def = toDefExpr(ast))
    blockSummary(def->sym);
  else
    printf("<blockSummary: node %d"
           " is not a BlockStmt/Fn/ModuleSymbol/DefExpr>\n", ast->id);
}
static const char* summarySymbolKind(Symbol* sym) {
  if (isFnSymbol(sym)) return "fn";
  if (isVarSymbol(sym)) return "var";
  if (isTypeSymbol(sym)) return "type";
  if (isModuleSymbol(sym)) return "module";
  if (isInterfaceSymbol(sym)) return "interface";
  return sym->astTagAsString();
}
static void summarySymbolPrint(Symbol* sym, const char* prefix = nullptr,
                               const char* suffix = nullptr) {
  printf("%s%s %s[%d] %s", prefix ? prefix : "",
         summarySymbolKind(sym), sym->name, sym->id, suffix ? suffix : "");
}
void blockSummary(BlockStmt* block, Symbol* sym) {
  printf("%9d { \n", block->id);
  for_alist(stmt, block->body) {
    printf("%9d    ", stmt->id);
    if (CallExpr* call = toCallExpr(stmt)) {
      if (PrimitiveOp* prim = call->primitive) {
        printf("'%s' ", prim->name);
      } else if (Expr* base = call->baseExpr) {
        if (SymExpr* se = toSymExpr(base))
          summarySymbolPrint(se->symbol(), "call ");
        else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(base))
          printf("call \"%s\" ", use->unresolved);
        else
          printf("call %s %d ", base->astTagAsString(), base->id);
      } else {
        // nothing
      }
    } else if (DefExpr* def = toDefExpr(stmt)) {
      printf("def ");
      if (Symbol* sym = def->sym) {
        summarySymbolPrint(sym);
      } else {
        // nothing
      }
    } else {
      printf("%s ", stmt->astTagAsString());
    }
    printf("\n");
  }
  printf("%9d }   %s\n", block->id, debugLoc(block));
  if (sym != nullptr)
    summarySymbolPrint(sym, "in ", "\n");
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
      if (val != nullptr && val != gNil) {
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
// vec_view: print the contents of a Vec or a std::vector
//

static void showFnSymbol(FnSymbol* fn) {
  printf("  %8d  %c%c  %s  %s\n", fn->id,
         // "g"eneric, "r"esolved, "G"eneric+resolved, " " neither
         fn->isResolved() ? (fn->isKnownToBeGeneric() ? 'G' : 'r') :
         (fn->isKnownToBeGeneric() ? 'g' : ' ') ,
         // "-" indicates not in tree
         fn->inTree() ? ' ' : '-',
         fn->name, debugLoc(fn));
}

static void showSymExpr(SymExpr* elm) {
  Symbol* sym = elm->symbol();
  if (sym)
    printf("  %8d  %s[%d]\n", elm->id, sym->name, sym->id);
  else
    printf("  %8d  <null symbol>\n", elm->id);
}

void vec_view(Vec<Symbol*,VEC_INTEGRAL_SIZE>* v) {
  vec_view(*v);
}

void vec_view(Vec<Symbol*,VEC_INTEGRAL_SIZE>& v)
{
  printf("Vec<Symbol> %d elm(s)\n", v.n);
  for (int i = 0; i < v.n; i++) {
    Symbol* elm = v.v[i];
    if (elm)
      printf("%3d  %8d  %s\n", i, elm->id, elm->name);
    else
      printf("%3d  <null>\n", i);
  }
}

void vec_view(Vec<FnSymbol*,VEC_INTEGRAL_SIZE>* v) {
  vec_view(*v);
}

void vec_view(Vec<FnSymbol*,VEC_INTEGRAL_SIZE>& v)
{
  printf("Vec<FnSymbol> %d elm(s)\n", v.n);
  for (int i = 0; i < v.n; i++) {
    FnSymbol* elm = v.v[i];
    if (elm)
      printf("%3d", i), showFnSymbol(elm);
    else
      printf("%3d  <null>\n", i);
  }
}

void vec_view(Vec<SymExpr*,VEC_INTEGRAL_SIZE>* v) {
  vec_view(*v);
}

void vec_view(Vec<SymExpr*,VEC_INTEGRAL_SIZE>& v)
{
  printf("Vec<SymExpr> %d elm(s)\n", v.n);
  for (int i = 0; i < v.n; i++) {
    SymExpr* elm = v.v[i];
    if (elm)
      printf("%3d", i), showSymExpr(elm);
    else
      printf("%3d  <null>\n", i);
  }
}

void vec_view(std::vector<Symbol*>* syms) {
  vec_view(*syms);
}

void vec_view(std::vector<Symbol*>& syms) {
  printf("vector<Symbol> %d elm(s)\n", (int)syms.size());
  for (int i = 0; i < (int)syms.size(); i++) {
    Symbol* elm = syms[i];
    if (elm)
      printf("%3d  %8d  %s\n", i, elm->id, elm->name);
    else
      printf("%3d  <null>\n", i);
  }
}

void vec_view(std::vector<FnSymbol*>* syms) {
  vec_view(*syms);
}

void vec_view(std::vector<FnSymbol*>& syms) {
  printf("vector<FnSymbol> %d elm(s)\n", (int)syms.size());
  for (int i = 0; i < (int)syms.size(); i++) {
    FnSymbol* elm = syms[i];
    if (elm)
      printf("%3d", i), showFnSymbol(elm);
    else
      printf("%3d  <null>\n", i);
  }
}

void vec_view(std::vector<SymExpr*>* syms) {
  vec_view(*syms);
}

void vec_view(std::vector<SymExpr*>& syms) {
  printf("vector<SymExpr> %d elm(s)\n", (int)syms.size());
  for (int i = 0; i < (int)syms.size(); i++) {
    SymExpr* elm = syms[i];
    if (elm)
      printf("%3d", i), showSymExpr(elm);
    else
      printf("%3d  <null>\n", i);
  }
}

static void showBlock(BlockStmt* block) {
  printf("  %8d  %s", block->id, debugLoc(block));
  if (FnSymbol* sym = toFnSymbol(block->parentSymbol)) {
    const char* msg = nullptr;
    if      (block == sym->body)                msg = "body";
    else if (block == sym->where)               msg = "where";
    else if (block == sym->lifetimeConstraints) msg = "lifetimeConstraints";
    else if (block == sym->retExprType)         msg = "retExprType";
    if (msg != nullptr)
      printf("  fn %s[%d] %s", sym->name, sym->id, msg);
  } else if (ModuleSymbol* sym = toModuleSymbol(block->parentSymbol)) {
    if (block == sym->block)
      printf("  module %s[%d] %s", sym->name, sym->id, "body");
  } else if (ArgSymbol* sym = toArgSymbol(block->parentSymbol)) {
    const char* msg = nullptr;
    if      (block == sym->typeExpr)     msg = "typeExpr";
    else if (block == sym->defaultExpr)  msg = "defaultExpr";
    else if (block == sym->variableExpr) msg = "variableExpr";
    if (msg != nullptr)
      printf("  arg %s[%d] %s", sym->name, sym->id, msg);
  } else if (ShadowVarSymbol* sym = toShadowVarSymbol(block->parentSymbol)) {
    const char* msg = nullptr;
    if      (block == sym->initBlock())   msg = "initBlock";
    else if (block == sym->deinitBlock()) msg = "deinitBlock";
    else if (block == sym->specBlock)     msg = "specBlock";
    if (msg != nullptr)
      printf("  svar %s[%d] %s", sym->name, sym->id, msg);
  }
  printf("\n");
}

void vec_view(Vec<BlockStmt*, VEC_INTEGRAL_SIZE>* v) {
  vec_view(*v);
}

void vec_view(Vec<BlockStmt*, VEC_INTEGRAL_SIZE>& v) {
  printf("Vec<BlockStmt> %d elm(s)\n", v.n);
  for (int i = 0; i < v.n; i++) {
    BlockStmt* elm = v.v[i];
    if (elm) printf("%3d", i), showBlock(elm);
    else     printf("%3d  <null>\n", i);
  }
}

void vec_view(std::vector<BlockStmt*>* v) {
  vec_view(*v);
}

void vec_view(std::vector<BlockStmt*>& v) {
  printf("vector<BlockStmt> %d elm(s)\n", (int)v.size());
  for (int i = 0; i < (int)v.size(); i++) {
    BlockStmt* elm = v[i];
    if (elm) printf("%3d", i), showBlock(elm);
    else     printf("%3d  <null>\n", i);
  }
}

void vec_view(Vec<ResolutionCandidate*, VEC_INTEGRAL_SIZE>* v) {
  vec_view(*v);
}

void vec_view(Vec<ResolutionCandidate*, VEC_INTEGRAL_SIZE>& v) {
  printf("Vec<ResolutionCandidate> %d elm(s)\n", v.n);
  for (int i = 0; i < v.n; i++) {
    ResolutionCandidate* elm = v.v[i];
    if (elm) {
      printf("%3d", i);
      if (elm->fn) showFnSymbol(elm->fn);
      else         printf("  RC.fn=<null>\n");
    }
    else printf("%3d  <null>\n", i);
  }
}

//
// set_view: print the contents of a std::set
//

void set_view(std::set<BlockStmt*>* bss) {
  set_view(*bss);
}

void set_view(std::set<BlockStmt*>& bss) {
  printf("set<BlockStmt> %d elm(s)\n", (int)bss.size());
  std::set<BlockStmt*>::iterator it = bss.begin();
  while (it != bss.end()) {
    debugSummary(*(it++));
  }
}

//
// typesWithName: print all TypeSymbols with the given name
//

void typesWithName(const char* name) {
  typesWithName(name, gTypeSymbols);
}

void typesWithName(const char* name, Vec<TypeSymbol*>& tyVec) {
  printf("typesWithName(\"%s\")\n", name);
  int count = 0, countNonNull = 0;
  forv_Vec(TypeSymbol, ty, tyVec) {
    if (ty) {
      countNonNull++;
      if (!strcmp(ty->name, name)) {
        count++;
        printf("  sym %8d   %s %d  %s\n", ty->id, ty->type->astTagAsString(),
               ty->type->id, debugLoc(ty));
      }
    }
  }
  printf("  = %d type(s) of %d\n", count, countNonNull);
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
        showFnSymbol(fn);
      }
    }
  }
  printf("  = %d function(s) of %d\n", count, countNonNull);
}

//
// whocalls: print all ways that the AST with the given 'id' is invoked
//

static void whocalls(int id, Symbol* sym);

void whocalls(BaseAST* ast) {
  if (!ast) {
    printf("whocalls: aborting: got NULL\n");
    return;
  }
  Symbol* sym = nullptr;
  if (SymExpr* se = toSymExpr(ast))
    sym = se->symbol();
  else if (DefExpr* def = toDefExpr(ast))
    sym = def->sym;
  else if (Symbol* symm = toSymbol(ast))
    sym = symm;

  if (sym == nullptr)
    printf("whocalls: aborting: need a SymExpr or DefExpr or Symbol\n");
  else
    whocalls(ast->id, sym);
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

void whocalls(int id) {
  whocalls(id, nullptr);
}

// 'sym' is used to print the function name
static void whocalls(int id, Symbol* sym) {
  if (sym == nullptr)
    printf("whocalls [%d]\n", id);
  else if (sym->id == id)
    printf("whocalls %s %s[%d]\n", sym->astTagAsString(), sym->name, id);
  else
    printf("whocalls [%d]  ignoring %s %s[%d]\n",
           id, sym->astTagAsString(), sym->name, sym->id);

  int callAll = 0, callMatch = 0, callNonTreeMatch = 0;
  forv_Vec(CallExpr, call, gCallExprs) {
    if (SymExpr* se = toSymExpr(call->baseExpr)) {
      callAll++;
      if (se->symbol()->id == id) {
        printf("  call %d  %s  %s\n", call->id,
               parentMsg(call, &callMatch, &callNonTreeMatch), debugLoc(call));
        if (whocalls_nview) nprint_view(call);
      }
    }
  }

  int forAll = 0, forMatch = 0, forNonTreeMatch = 0;
  forv_Vec(CallExpr, call, gCallExprs) {
    // todo: does this really happen?
    if (call->isPrimitive(PRIM_BLOCK_FOR_LOOP) && call->numActuals() >= 2) {
      forAll++;
      // check each step, just in case
      if (SymExpr* act2 = toSymExpr(call->get(2)))
        if (Symbol* ic = act2->symbol())
          if (AggregateType* ty = toAggregateType(ic->type))
            if (FnSymbol* init = ty->iteratorInfo->getIterator)
              if (ArgSymbol* form1 = init->getFormal(1))
                if (AggregateType* fty = toAggregateType(form1->type))
                  if (FnSymbol* iterator = fty->iteratorInfo->iterator)
                    if (iterator->id == id)
                      printf("  for-loop blockInfo %d  %s  %s\n",
                             call->id, parentMsg(call, &forMatch,
                               &forNonTreeMatch), debugLoc(call));
    }
  }

  int fItAll = 0, fItMatch = 0, fItNonTreeMatch = 0;
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (ForLoop* loop = toForLoop(block)) {
      fItAll++;
      if (SymExpr* it = loop->iteratorGet())
        if (Symbol* ic = it->symbol())
          if (FnSymbol* iterator = debugGetTheIteratorFn(ic))
            if (iterator->id == id)
              printf("  ForLoop %d  iterator sym %d  %s  %s\n",
                     loop->id, ic->id, parentMsg(loop, &fItMatch,
                       &fItNonTreeMatch), debugLoc(loop));
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

  int ftMatch = 0, ftAll = ftableVec.size();
  for (int i = 0; i < ftAll; i++) {
    if (ftableVec.begin()[i]->id == id) {
      ftMatch++;
      printf("  ftableVec[%d]\n", i);
    }
  }

  printf("  = %d of %d calls", callMatch, callAll);
  if (callNonTreeMatch) printf(", also %d not in tree", callNonTreeMatch);
  printf(".  %d of %d for-loops", forMatch+fItMatch, forAll+fItAll);
  int forNontree = forNonTreeMatch + fItNonTreeMatch;
  if (forNontree) printf(", also %d not in tree", forNontree);
  printf(".  %d of %d in VMT+FT.\n", vmtMatch+ftMatch, vmtAll+ftAll);
  printf("\n");
}

FnSymbol* debugGetTheIteratorFn(int id) {
  if (BaseAST* ast = aidWithError(id, "debugGetTheIteratorFn"))
    return debugGetTheIteratorFn(ast);
  else
    return nullptr;
}

FnSymbol* debugGetTheIteratorFn(BaseAST* ast) {
  if (!ast) {
    printf("<debugGetTheIteratorFn: NULL>\n");
    return nullptr;
  }
  else if (Symbol* sym = toSymbol(ast))
    return debugGetTheIteratorFn(sym->type);
  else if (SymExpr* se = toSymExpr(ast))
    return debugGetTheIteratorFn(se->symbol()->type);
  else if (Type* type = toType(ast))
    return debugGetTheIteratorFn(type);
  else if (ForLoop* fl = toForLoop(ast))
    return debugGetTheIteratorFn(fl);
  else {
    printf("<don't know how to get the iterator for node %s %d>\n",
           ast->astTagAsString(), ast->id);
    return nullptr;
  }
}
