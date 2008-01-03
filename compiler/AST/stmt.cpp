#include <string.h>
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"


void codegenStmt(FILE* outfile, Expr* stmt) {
  if (stmt->lineno > 0) {
    if (printCppLineno) {
      fprintf(outfile, "/* ZLINE: %d %s */\n", stmt->lineno, stmt->filename);
    } 
  }
}


BlockStmt::BlockStmt(AList* init_body, BlockTag init_blockTag) :
  Expr(STMT_BLOCK),
  blockTag(init_blockTag),
  body(),
  loopInfo(NULL),
  blkScope(NULL),
  pre_loop(NULL),
  post_loop(NULL)
{
  body.parent = this;
  body.insertAtTail(init_body);
}


BlockStmt::BlockStmt(Expr* init_body, BlockTag init_blockTag) :
  Expr(STMT_BLOCK),
  blockTag(init_blockTag),
  body(),
  loopInfo(NULL),
  blkScope(NULL),
  pre_loop(NULL),
  post_loop(NULL)
{
  body.parent = this;
  if (init_body)
    body.insertAtTail(init_body);
}


BlockStmt::~BlockStmt() {
  if (blkScope && blkScope->astParent == this)
    delete blkScope;
}


void BlockStmt::verify() {
  Expr::verify();
  if (astTag != STMT_BLOCK) {
    INT_FATAL(this, "Bad BlockStmt::astTag");
  }
  if (body.parent != this)
    INT_FATAL(this, "Bad AList::parent in BlockStmt");
}


BlockStmt*
BlockStmt::copyInner(ASTMap* map) {
  BlockStmt* _this = new BlockStmt();
  _this->blockTag = blockTag;
  for_alist(expr, body)
    _this->insertAtTail(COPY_INT(expr));
  _this->loopInfo = COPY_INT(loopInfo);
  return _this;
}


void BlockStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == loopInfo)
    loopInfo = toCallExpr(new_ast);
  else
    INT_FATAL(this, "Unexpected case in BlockStmt::replaceChild");
}


static void
codegenCobegin( FILE* outfile, AList* body) {
  int num_stmts = body->length();

  // if the cobegin is empty, exit early to avoid declaring 0-length
  // arrays which some compilers (PGI) don't like
  if (num_stmts == 0) {
    fprintf(outfile, "/* empty cobegin at */\n");
    return;
  }
  // For now, assume all statements will be forked.

  // gen code for the function pointer array
  fprintf (outfile, "_chpl_threadfp_t fpv[%d] = {\n", num_stmts);
  int stmt_cnt = 0;
  for_alist(stmt, *body) {
    if (CallExpr *cexpr=toCallExpr(stmt)) {
      if (SymExpr *sexpr=toSymExpr(cexpr->baseExpr)) {
        fprintf (outfile, "(_chpl_threadfp_t)%s", sexpr->var->cname);
      } else {
        INT_FATAL(stmt, "cobegin codegen - call expr not a SymExpr");
      } 
    } else {
      INT_FATAL(stmt, "cobegin codegen - statement not a CallExpr");
    }
    stmt_cnt++;
    if (stmt_cnt < num_stmts) {
      fprintf (outfile, ",\n");
    }
  }
  fprintf (outfile, "};\n");
      
  // ok, walk through again and gen code for the argument array
  fprintf (outfile, "_chpl_threadarg_t av[%d] = {\n", num_stmts);
  stmt_cnt = 0;
  for_alist(stmt, *body) {
    if (CallExpr *cexpr=toCallExpr(stmt)) {
      Expr *actuals = cexpr->get(1);
      // pass exactly one class object containing ptrs to locals
      fprintf (outfile, "(_chpl_threadarg_t)");
      if (actuals) {
        /*
          FnSymbol *fnsym = cexpr->findFnSymbol();
          DefExpr  *formals = fnsym->formals->first();
          if (toArgSymbol(formals->sym)->requiresCPtr()) {
          fprintf (outfile, "&");
          }
        */
        fprintf (outfile, "(");
        actuals->codegen (outfile);
        fprintf (outfile, ")");
      } else {
        fprintf (outfile, "NULL");
      }
    }
    stmt_cnt++;
    if (stmt_cnt < num_stmts) {
      fprintf (outfile, ",\n");
    }
  }
  fprintf (outfile, "};\n");

#if 0      
  fprintf (outfile, "_chpl_cobegin_wkspace_t wksp[%d];\n", num_stmts);
  fprintf (outfile, "_chpl_cobegin (%d, %s, %s, %s);\n",
           num_stmts, "fpv", "av", "wksp");
#endif
  fprintf (outfile, "_chpl_cobegin (%d, %s, %s);\n",
           num_stmts, "fpv", "av");

}


static void
codegenBegin( FILE* outfile, AList* body) {
  // Body should be one function call that we fork.
  if (body->length() != 1)
    INT_FATAL("begin codegen - expect only one function call");

  fprintf( outfile, "_chpl_begin( ");
  for_alist(stmt, *body) {
    if (CallExpr *cexpr=toCallExpr(stmt)) {
      if (SymExpr *sexpr=toSymExpr(cexpr->baseExpr)) {
        fprintf (outfile, "(_chpl_threadfp_t) %s, ", sexpr->var->cname);
        fprintf (outfile, "(_chpl_threadarg_t) ");
        if (Expr *actuals = cexpr->get(1)) {
          actuals->codegen (outfile);
        } else {
          fprintf( outfile, "NULL");
        }
      } else {
        INT_FATAL(stmt, "cobegin codegen - call expr not a SymExpr");
      } 
    } else {
      INT_FATAL(stmt, "cobegin codegen - statement not a CallExpr");
    }
  }
  fprintf (outfile, ");\n");
}


void BlockStmt::codegen(FILE* outfile) {
  codegenStmt(outfile, this);

  if (loopInfo) {
    if (loopInfo->isPrimitive(PRIMITIVE_LOOP_WHILEDO)) {
      fprintf(outfile, "while (");
      loopInfo->get(1)->codegen(outfile);
      fprintf(outfile, ") ");
    } else if (loopInfo->isPrimitive(PRIMITIVE_LOOP_DOWHILE)) {
      fprintf(outfile, "do ");
    } else if (loopInfo->isPrimitive(PRIMITIVE_LOOP_FOR)) {
      fprintf(outfile, "for (;");
      loopInfo->get(1)->codegen(outfile);
      fprintf(outfile, ";) ");
    } else if (loopInfo->isPrimitive(PRIMITIVE_LOOP_C_FOR)) {
      fprintf(outfile, "for (");
      loopInfo->get(1)->codegen(outfile);
      fprintf(outfile, " = ");
      if (loopInfo->get(2)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, "*");
      loopInfo->get(2)->codegen(outfile);
      fprintf(outfile, "; ");
      loopInfo->get(1)->codegen(outfile);
      fprintf(outfile, " <= ");
      if (loopInfo->get(3)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, "*");
      loopInfo->get(3)->codegen(outfile);
      fprintf(outfile, "; ");
      loopInfo->get(1)->codegen(outfile);
      fprintf(outfile, " += ");
      if (loopInfo->get(4)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, "*");
      loopInfo->get(4)->codegen(outfile);
      fprintf(outfile, ") ");
    }
  }

  if (this != getFunction()->body)
    fprintf(outfile, "{\n");

  if (blockTag == BLOCK_COBEGIN) {
    codegenCobegin(outfile, &body);
  } else if (blockTag == BLOCK_BEGIN) {
    codegenBegin(outfile, &body);
  } else {
    body.codegen(outfile, "");
  }

  if (loopInfo && loopInfo->isPrimitive(PRIMITIVE_LOOP_DOWHILE)) {
    fprintf(outfile, "} while (");
    loopInfo->get(1)->codegen(outfile);
    fprintf(outfile, ");\n");
  } else if (this != getFunction()->body) {
    fprintf(outfile, "}");
    CondStmt* cond = toCondStmt(parentExpr);
    if (!cond || !(cond->thenStmt == this && cond->elseStmt))
      fprintf(outfile, "\n");
  }
}


void
BlockStmt::insertAtHead(Expr* ast) {
  body.insertAtHead(ast);
}


void
BlockStmt::insertAtTail(Expr* ast) {
  body.insertAtTail(ast);
}


void
BlockStmt::insertAtHead(AList* ast) {
  body.insertAtHead(ast);
}


void
BlockStmt::insertAtTail(AList* ast) {
  body.insertAtTail(ast);
}


bool
BlockStmt::isLoop(void) {
  return
    blockTag == BLOCK_WHILE_DO ||
    blockTag == BLOCK_DO_WHILE ||
    blockTag == BLOCK_FOR ||
    blockTag == BLOCK_FORALL ||
    blockTag == BLOCK_ORDERED_FORALL;
}


int
BlockStmt::length(void) {
  return body.length();
}


CondStmt::CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt) :
  Expr(STMT_COND),
  condExpr(iCondExpr),
  thenStmt(NULL),
  elseStmt(NULL)
{
  if (Expr* s = toExpr(iThenStmt))
    thenStmt = new BlockStmt(s);
  else
    INT_FATAL(iThenStmt, "Bad then-stmt passed to CondStmt constructor");
  if (iElseStmt) {
    if (Expr* s = toExpr(iElseStmt))
      elseStmt = new BlockStmt(s);
    else
      INT_FATAL(iElseStmt, "Bad else-stmt passed to CondStmt constructor");
  }
}


Expr*
CondStmt::fold_cond_stmt() {
  Expr* result = NULL;
  if (SymExpr* cond = toSymExpr(condExpr)) {
    if (VarSymbol* var = toVarSymbol(cond->var)) {
      if (var->immediate &&
          var->immediate->const_kind == NUM_KIND_UINT &&
          var->immediate->num_index == INT_SIZE_1) {
        result = new CallExpr(PRIMITIVE_NOOP);
        this->insertBefore(result);
        if (var->immediate->v_bool == gTrue->immediate->v_bool) {
          Expr* then_stmt = thenStmt;
          then_stmt->remove();
          this->replace(then_stmt);
        } else if (var->immediate->v_bool == gFalse->immediate->v_bool) {
          Expr* else_stmt = elseStmt;
          if (else_stmt) {
            else_stmt->remove();
            this->replace(else_stmt);
          } else {
            this->remove();
          }
        }
      }
    }
  }
  return result;
}


void CondStmt::verify() {
  Expr::verify();
  if (astTag != STMT_COND) {
    INT_FATAL(this, "Bad CondStmt::astTag");
  }

  if (!condExpr) {
    INT_FATAL(this, "CondStmt has no condExpr");
  }

  if (!thenStmt) {
    INT_FATAL(this, "CondStmt has no thenStmt");
  }

  if (condExpr->list) {
    INT_FATAL(this, "CondStmt::condExpr is a list");
  }

  if (thenStmt->list) {
    INT_FATAL(this, "CondStmt::thenStmt is a list");
  }

  if (elseStmt && elseStmt->list) {
    INT_FATAL(this, "CondStmt::elseStmt is a list");
  }
}


CondStmt*
CondStmt::copyInner(ASTMap* map) {
  return new CondStmt(COPY_INT(condExpr),
                      COPY_INT(thenStmt),
                      COPY_INT(elseStmt));
}


void CondStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == condExpr) {
    condExpr = new_ast;
  } else if (old_ast == thenStmt) {
    thenStmt = toBlockStmt(new_ast);
  } else if (old_ast == elseStmt) {
    elseStmt = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CondStmt::replaceChild");
  }
}


void CondStmt::codegen(FILE* outfile) {
  codegenStmt(outfile, this);
  fprintf(outfile, "if (");
  condExpr->codegen(outfile);
  fprintf(outfile, ") ");
  thenStmt->codegen(outfile);
  if (elseStmt) {
    fprintf(outfile, " else ");
    elseStmt->codegen(outfile);
  }
}


GotoStmt::GotoStmt(GotoTag init_gotoTag) :
  Expr(STMT_GOTO),
  label(new SymExpr(gNil)),
  gotoTag(init_gotoTag)
{
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, const char* init_label) :
  Expr(STMT_GOTO),
  label(init_label ? new SymExpr(init_label) : new SymExpr(gNil)),
  gotoTag(init_gotoTag)
{
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, Symbol* init_label) :
  Expr(STMT_GOTO),
  label(new SymExpr(init_label)),
  gotoTag(init_gotoTag)
{
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, SymExpr* init_label) :
  Expr(STMT_GOTO),
  label(init_label),
  gotoTag(init_gotoTag)
{
  if (!init_label)
    INT_FATAL(this, "GotoStmt initialized with null label");

  if (init_label->parentSymbol)
    INT_FATAL(this, "GotoStmt initialized with label already in tree");
}


void GotoStmt::verify() {
  Expr::verify();
  if (astTag != STMT_GOTO) {
    INT_FATAL(this, "Bad GotoStmt::astTag");
  }
  if (!label)
    INT_FATAL(this, "GotoStmt has no label");
  if (label->list)
    INT_FATAL(this, "GotoStmt::label is a list");
}


GotoStmt*
GotoStmt::copyInner(ASTMap* map) {
  return new GotoStmt(gotoTag, label->copy());
}


void GotoStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == label) {
    label = toSymExpr(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in GotoStmt::replaceChild");
  }
}


void GotoStmt::codegen(FILE* outfile) {
  codegenStmt(outfile, this);
  fprintf(outfile, "goto ");
  label->codegen(outfile);
  fprintf(outfile, ";\n");
}
