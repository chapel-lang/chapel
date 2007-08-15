#include <string.h>
#include <typeinfo>
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"


bool printCppLineno = false;


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
  body(init_body),
  loopInfo(NULL),
  blkScope(NULL),
  pre_loop(NULL),
  post_loop(NULL)
{
  body->parent = this;
}


BlockStmt::BlockStmt(Expr* init_body, BlockTag init_blockTag) :
  Expr(STMT_BLOCK),
  blockTag(init_blockTag),
  body(new AList(init_body)),
  loopInfo(NULL),
  blkScope(NULL),
  pre_loop(NULL),
  post_loop(NULL)
{
  body->parent = this;
}


BlockStmt::~BlockStmt() {
  if (blkScope && blkScope->astParent == this)
    delete blkScope;
  delete body;
}


void BlockStmt::verify() {
  Expr::verify();
  if (astType != STMT_BLOCK) {
    INT_FATAL(this, "Bad BlockStmt::astType");
  }
  if (body->parent != this)
    INT_FATAL(this, "Bad AList::parent in BlockStmt");
}


BlockStmt*
BlockStmt::copyInner(ASTMap* map) {
  BlockStmt* _this = new BlockStmt(COPY_INT(body), blockTag);
  _this->loopInfo = COPY_INT(loopInfo);
  return _this;
}


void BlockStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == loopInfo)
    loopInfo = dynamic_cast<CallExpr*>(new_ast);
  else
    INT_FATAL(this, "Unexpected case in BlockStmt::replaceChild");
}


static void
codegenCobegin( FILE* outfile, AList *body) {
  int stmt_cnt;
  int num_stmts = body->length();
  // For now, assume all statements will be forked.

  // gen code for the function pointer array
  fprintf (outfile, "_chpl_threadfp_t fpv[%d] = {\n", num_stmts);
  stmt_cnt = 0;
  for_alist(stmt, body) {
    if (CallExpr *cexpr=dynamic_cast<CallExpr*>(stmt)) {
      if (SymExpr *sexpr=dynamic_cast<SymExpr*>(cexpr->baseExpr)) {
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
  for_alist(stmt, body) {
    if (CallExpr *cexpr=dynamic_cast<CallExpr*>(stmt)) {
      Expr *actuals = dynamic_cast<Expr*>(cexpr->argList->first());
      // pass exactly one class object containing ptrs to locals
      fprintf (outfile, "(_chpl_threadarg_t)");
      if (actuals) {
        /*
          FnSymbol *fnsym = cexpr->findFnSymbol();
          DefExpr  *formals = fnsym->formals->first();
          if (dynamic_cast<ArgSymbol*>(formals->sym)->requiresCPtr()) {
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
      
  fprintf (outfile, "_chpl_cobegin_wkspace_t wksp[%d];\n", num_stmts);
  fprintf (outfile, "_chpl_cobegin (%d, %s, %s, %s);\n",
           num_stmts, "fpv", "av", "wksp");

}


static void
codegenBegin( FILE* outfile, AList *body) {
  // Body should be one function call that we fork.
  if (body->length() != 1)
    INT_FATAL("begin codegen - expect only one function call");

  fprintf( outfile, "_chpl_begin( ");
  for_alist(stmt, body) {
    if (CallExpr *cexpr=dynamic_cast<CallExpr*>(stmt)) {
      if (SymExpr *sexpr=dynamic_cast<SymExpr*>(cexpr->baseExpr)) {
        fprintf (outfile, "(_chpl_threadfp_t) %s, ", sexpr->var->cname);
        fprintf (outfile, "(_chpl_threadarg_t) ");
        if (Expr *actuals = dynamic_cast<Expr*>(cexpr->argList->first())) {
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

  if (body) {
    if (blockTag == BLOCK_COBEGIN) {
      codegenCobegin( outfile, body);
    } else if (blockTag == BLOCK_BEGIN) {
      codegenBegin( outfile, body);
    } else {
      body->codegen(outfile, "");
    }
  }

  if (loopInfo && loopInfo->isPrimitive(PRIMITIVE_LOOP_DOWHILE)) {
    fprintf(outfile, "} while (");
    loopInfo->get(1)->codegen(outfile);
    fprintf(outfile, ");\n");
  } else if (this != getFunction()->body) {
    fprintf(outfile, "}");
    CondStmt* cond = dynamic_cast<CondStmt*>(parentExpr);
    if (!cond || !(cond->thenStmt == this && cond->elseStmt))
      fprintf(outfile, "\n");
  }
}


void
BlockStmt::insertAtHead(Expr* ast) {
  body->insertAtHead(ast);
}


void
BlockStmt::insertAtTail(Expr* ast) {
  body->insertAtTail(ast);
}


void
BlockStmt::insertAtHead(AList* ast) {
  body->insertAtHead(ast);
}


void
BlockStmt::insertAtTail(AList* ast) {
  body->insertAtTail(ast);
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


CondStmt::CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt) :
  Expr(STMT_COND),
  condExpr(iCondExpr),
  thenStmt(NULL),
  elseStmt(NULL)
{
  if (Expr* s = dynamic_cast<Expr*>(iThenStmt))
    thenStmt = new BlockStmt(s);
  else
    INT_FATAL(iThenStmt, "Bad then-stmt passed to CondStmt constructor");
  if (iElseStmt) {
    if (Expr* s = dynamic_cast<Expr*>(iElseStmt))
      elseStmt = new BlockStmt(s);
    else
      INT_FATAL(iElseStmt, "Bad else-stmt passed to CondStmt constructor");
  }
}


void CondStmt::verify() {
  Expr::verify();
  if (astType != STMT_COND) {
    INT_FATAL(this, "Bad CondStmt::astType");
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
    condExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == thenStmt) {
    thenStmt = dynamic_cast<BlockStmt*>(new_ast);
  } else if (old_ast == elseStmt) {
    elseStmt = dynamic_cast<BlockStmt*>(new_ast);
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


GotoStmt::GotoStmt(gotoType init_goto_type) :
  Expr(STMT_GOTO),
  label(NULL),
  goto_type(init_goto_type)
{
}


GotoStmt::GotoStmt(gotoType init_goto_type, const char* init_label) :
  Expr(STMT_GOTO),
  label(init_label ? new UnresolvedSymbol(init_label) : NULL),
  goto_type(init_goto_type)
{
}


GotoStmt::GotoStmt(gotoType init_goto_type, Symbol* init_label) :
  Expr(STMT_GOTO),
  label(init_label),
  goto_type(init_goto_type)
{
}


void GotoStmt::verify() {
  Expr::verify();
  if (astType != STMT_GOTO) {
    INT_FATAL(this, "Bad GotoStmt::astType");
  }
}


GotoStmt*
GotoStmt::copyInner(ASTMap* map) {
  return new GotoStmt(goto_type, label);
}


void GotoStmt::codegen(FILE* outfile) {
  codegenStmt(outfile, this);
  fprintf(outfile, "goto ");
  label->codegen(outfile);
  fprintf(outfile, ";\n");
}
