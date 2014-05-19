#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "view.h"

#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static int            log_indent     =     0;
static bool           log_need_space = false;

static void           log_ast(BaseAST* ast, FILE* file);
static void           log_ast_header(BaseAST* ast, FILE* file);
static void           log_ast_footer(BaseAST* ast, FILE* file);
static void           log_ast_symbol(FILE* file, Symbol* sym, bool def);
static void           log_ast_fnsymbol(FILE* file, FnSymbol* fn);
static void           printBlockID(FILE* fp, Expr* expr);
static AggregateType* structuralTypeSymbol(Symbol* s);

void dump_ast(const char* passName, int pass_num) {
  FILE* log_file;
  const char* filename;
  char numBuf[4];
  snprintf(numBuf, 4, "%02d", pass_num);

  forv_Vec(ModuleSymbol, mod, allModules) {
    if (*log_module &&  // is not NUL
        strcmp(log_module, mod->name))
      continue;

    filename = astr(mod->name, "_", numBuf, passName, ".ast");
    log_file = fopen(astr(log_dir, filename), "w");
    fprintf(log_file, "AST dump for %s after pass %s.\n", mod->name, passName);
    log_indent = 0; log_need_space = false;
    log_ast(mod, log_file);
    fclose(log_file);
  }
}

static inline void log_newline(FILE* file) {
  putc('\n', file);
  for (int i = 0; i < 2 * log_indent; ++i)
    putc(' ', file);
  log_need_space = false;
}

static inline void log_write(FILE* file, bool space_before, const char* text, bool space_after) {
  if (space_before && log_need_space)
    putc(' ', file);
  fputs(text, file);
  log_need_space = space_after;
}
  
static void
log_ast(BaseAST* ast, FILE* file) {
  if (Expr* expr = toExpr(ast))
    if (DefExpr* e = toDefExpr(expr))
      if (toModuleSymbol(e->sym))
        // Since we iterate modules at the top layer, don't visit nested modules here.
        return;
  if (Symbol* s = toSymbol(ast))
    if (toArgSymbol(s))
      // Don't show args; they are handled in log_ast_fnsymbol().
      return;

  log_ast_header(ast, file);
  AST_CHILDREN_CALL(ast, log_ast, file);
  log_ast_footer(ast, file);
}

static void
log_ast_header(BaseAST* ast, FILE* file) {
  if (Expr* expr = toExpr(ast)) {
    if (isBlockStmt(expr)) {
      log_newline(file);
      if (FnSymbol* fn = toFnSymbol(expr->parentSymbol))
        if (expr == fn->where)
          log_write(file, false, "where ", false);
      log_write(file, false, "{", true);
      printBlockID(file, expr);
      ++log_indent;
    } else if (GotoStmt* s = toGotoStmt(expr)) {
      switch (s->gotoTag) {
       default: break;
       case GOTO_NORMAL:        log_write(file, true, "goto", true);            break;
       case GOTO_BREAK:         log_write(file, true, "break", true);           break;
       case GOTO_CONTINUE:      log_write(file, true, "continue", true);        break;
       case GOTO_RETURN:        log_write(file, true, "gotoReturn", true);      break;
       case GOTO_GETITER_END:   log_write(file, true, "gotoGetiterEnd", true);  break;
       case GOTO_ITER_RESUME:   log_write(file, true, "gotoIterResume", true);  break;
       case GOTO_ITER_END:      log_write(file, true, "gotoIterEnd", true);     break;
      }
      if (SymExpr* label = toSymExpr(s->label))
        if (label->var != gNil) {
          log_ast_symbol(file, label->var, true);
        }
    } else if (toCondStmt(expr)) {
      log_newline(file);
      log_write(file, true, "if", true);
    } else {
      if (expr->getStmtExpr() && expr->getStmtExpr() == expr) {
        log_newline(file);
      }
      if (DefExpr* e = toDefExpr(expr)) {
        if (FnSymbol* fn = toFnSymbol(e->sym)) {
          log_write(file, true, "function", true);
          log_ast_fnsymbol(file, fn);
        } else if (structuralTypeSymbol(e->sym)) {
          if (DefExpr *def = toDefExpr( ast)) {
            if (def->sym->hasFlag(FLAG_SYNC))
              log_write(file, true, "sync", true);
            if (def->sym->hasFlag(FLAG_SINGLE))
              log_write(file, true, "single", true);
          }
          log_write(file, true, "type", true);
          log_ast_symbol(file, e->sym, true);
        } else if (toTypeSymbol(e->sym)) {
          log_write(file, true, "type", true);
          log_ast_symbol(file, e->sym, true);
        } else if (VarSymbol* vs=toVarSymbol(e->sym)) {
          if (vs->type->symbol->hasFlag(FLAG_SYNC))
            log_write(file, true, "sync", true);
          if (vs->type->symbol->hasFlag(FLAG_SINGLE))
            log_write(file, true, "single", true);
          log_write(file, true, "var", true);
          log_ast_symbol(file, e->sym, true);
        } else if (toArgSymbol(e->sym)) {
          // Argsymbols are handled in the function header.
        } else if (toLabelSymbol(e->sym)) {
          log_write(file, true, "label", true);
          log_ast_symbol(file, e->sym, true);
        } else if (toModuleSymbol(e->sym)) {
          log_write(file, true, "module", true);
          log_ast_symbol(file, e->sym, true);
        } else {
          log_write(file, true, "def", true);
          log_ast_symbol(file, e->sym, true);
        }
      } else if (VarSymbol* e = get_constant(expr)) {
        if (e->immediate) {
          const size_t bufSize = 128;
          char imm[bufSize];
          snprint_imm(imm, bufSize, *e->immediate);
          if (log_need_space) putc(' ', file);
          fprintf(file, "%s%s", imm, is_imag_type(e->type) ? "i" : "");
          log_need_space = true;
        } else {
          log_write(file, true, e->name, true);
        }
      } else if (SymExpr* e = toSymExpr(expr)) {
        log_ast_symbol(file, e->var, false);
      } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(expr)) {
        log_write(file, true, e->unresolved, true);
      } else if (NamedExpr* e = toNamedExpr(expr)) {
        fprintf(file, "(%s =", e->name);
        log_need_space = true;
      } else if (CallExpr* e = toCallExpr(expr)) {
        if (e->isResolved()) {
          if (e->isResolved()->hasFlag(FLAG_BEGIN_BLOCK))
            log_write(file, true, "begin", true);
          else if (e->isResolved()->hasFlag(FLAG_ON_BLOCK))
            log_write(file, true, "on", true);
        }
        if (fLogIds) {
          fprintf(file, "(%d ", e->id);
          log_need_space = false;
        } else {
          log_write(file, true, "(", false);
        }
        if (!e->primitive) {
          log_write(file, true, "call", true);
        } else {
          if (e->isPrimitive(PRIM_RETURN))
            log_write(file, true, "return", true);
          else if (e->isPrimitive(PRIM_YIELD))
            log_write(file, true, "yield", true);
          else {
            if (log_need_space) putc(' ', file);
            fprintf(file, "'%s'", e->primitive->name);
            log_need_space = true;
          }
        }
        if (e->partialTag)
          log_write(file, true, "(partial)", true);
      } else {
        if (log_need_space) putc(' ', file);
        fprintf(file, "(%s", expr->astTagAsString());
        log_need_space = true;
      }
    }
  }
}

static void
log_ast_footer(BaseAST* ast, FILE* file) {
  if (Expr* expr = toExpr(ast)) {
    if (toCallExpr(expr) || toNamedExpr(expr)) {
      log_write(file, false, ")", true);
    }

    if (toBlockStmt(expr)) {
      --log_indent;
      log_newline(file);
      log_write(file, false, "}", true);
      printBlockID(file, expr);
    }
  }
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

static void
log_ast_fnsymbol(FILE* file, FnSymbol* fn) {
  if (fn->_this && fn->_this->defPoint) {
    log_ast_symbol(file, fn->_this->type->symbol, false);
    log_write(file, false, ".", false);
  }
  log_ast_symbol(file, fn, true);
  log_write(file, false, "(", false);
  bool first = true;
  for_formals(formal, fn) {
    if (first) {
      first = false;
    } else {
      log_write(file, false, ",", true);
    }
    log_ast_symbol(file, formal, true);
    if (formal->typeExpr) {
      log_write(file, true, ":", true);
      if (BlockStmt* block = toBlockStmt(formal->typeExpr))
        log_ast(block->body.first(), file);
      else
        log_ast(formal->typeExpr, file);
    }
    if (formal->defaultExpr) {
      log_write(file, true, "=", true);
      if (BlockStmt* block = toBlockStmt(formal->defaultExpr))
        log_ast(block->body.first(), file);
      else
        log_ast(formal->defaultExpr, file);
    }
  }
  log_write(file, false, ")", true);
  if (fn->retTag == RET_VAR)
    log_write(file, true, "var", true);
  else if (fn->retTag == RET_PARAM)
    log_write(file, true, "param", true);
  else if (fn->retTag == RET_TYPE)
    log_write(file, true, "type", true);
  if (fn->retType && fn->retType->symbol) {
    log_write(file, true, ":", true);
    log_ast_symbol(file, fn->retType->symbol, false);
  }
}

static void printBlockID(FILE* fp, Expr* expr) {
  if (fdump_html_print_block_IDs)
    fprintf(fp, " %d", expr->id);
}

static AggregateType* structuralTypeSymbol(Symbol *s) {
  if (TypeSymbol *ts = toTypeSymbol(s))
    if (AggregateType *st = toAggregateType(ts->type))
      return st;
  return NULL;
}
