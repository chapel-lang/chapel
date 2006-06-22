#include <string.h>
#include <typeinfo>
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "../passes/runAnalysis.h"

static int inBlockStmt = 0;
bool printCppLineno = false;
bool printChplLineno = false;
bool inUserModule = false;
bool justStartedGeneratingFunction = false;

Stmt::Stmt(astType_t astType) :
  BaseAST(astType),
  parentStmt(NULL),
  ainfo(NULL)
{}


void Stmt::verify() {
  BaseAST::verify();
  if (!prev || !next) {
    if (!dynamic_cast<BlockStmt*>(this) &&
        !dynamic_cast<LabelStmt*>(parentStmt)) {
      INT_FATAL(this, "Statement is not in a list");
    }
  }
}


Stmt*
Stmt::copyInner(ASTMap* map) {
  INT_FATAL(this, "Illegal call to Stmt::copy");
  return NULL;
}


static char* priorFilename = "";
static int priorLineno = -1;

void Stmt::codegen(FILE* outfile) {
  if (lineno > 0) {
    if (printCppLineno) {
      fprintf(outfile, "/* ZLINE: %d %s */\n", lineno, filename);
    } 
    
    if (printChplLineno && inBlockStmt && inUserModule) {
      if (strcmp(filename, priorFilename) != 0 ||  
          justStartedGeneratingFunction) {
        fprintf(outfile, "chpl_input_filename = \"%s\";\n", filename);
        priorFilename = stringcpy(filename);
        justStartedGeneratingFunction = false;
      }
      if (lineno != priorLineno) {
        fprintf(outfile, "chpl_input_lineno = %d;\n", lineno);
        priorLineno = lineno;
      }
    }
  }
  codegenStmt(outfile);
}


void Stmt::codegenStmt(FILE* outfile) {
  INT_FATAL(this, "codegenStmt not implemented for Stmt subclass");
}


ASTContext Stmt::getContext(void) {
  ASTContext context;
  context.parentScope = parentScope;
  context.parentSymbol = parentSymbol;
  context.parentStmt = parentStmt;
  context.parentExpr = NULL;
  return context;
}


void Stmt::callReplaceChild(BaseAST* new_ast) {
  if (parentStmt) {
    parentStmt->replaceChild(this, new_ast);
  } else {
    parentSymbol->replaceChild(this, new_ast);
  }
}


ExprStmt::ExprStmt(Expr* initExpr) :
  Stmt(STMT_EXPR),
  expr(initExpr) 
{
  if (expr && expr->parentSymbol) {
    INT_FATAL(this, "ExprStmt initialized with expr already in tree");
  }
}


void ExprStmt::verify() {
  Stmt::verify();
  if (astType != STMT_EXPR) {
    INT_FATAL(this, "Bad ExprStmt::astType");
  }
}


ExprStmt*
ExprStmt::copyInner(ASTMap* map) {
  return new ExprStmt(COPY_INT(expr));
}


void ExprStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ExprStmt::replaceChild");
  }
}


void ExprStmt::print(FILE* outfile) {
  expr->print(outfile);
  fprintf(outfile, ";");
}


void ExprStmt::codegenStmt(FILE* outfile) {
  expr->codegen(outfile);
  fprintf(outfile, ";\n");
}

bool ExprStmt::noCodegen() {
  if (expr)
    return expr->noCodegen();
  else
    return false;
}

ReturnStmt::ReturnStmt(Expr* initExpr, bool init_yield) :
  Stmt(STMT_RETURN),
  expr(initExpr),
  yield(init_yield)
{
  if (initExpr == NULL)
    expr = new SymExpr(gVoid);
  if (expr && expr->parentSymbol)
    INT_FATAL(this, "ReturnStmt initialized with expr already in tree");
}

ReturnStmt::ReturnStmt(Symbol* initExpr, bool init_yield) :
  Stmt(STMT_RETURN),
  expr(new SymExpr(initExpr)),
  yield(init_yield)
{ }

ReturnStmt::ReturnStmt(char* initExpr, bool init_yield) :
  Stmt(STMT_RETURN),
  expr(new SymExpr(new UnresolvedSymbol(initExpr))),
  yield(init_yield)
{ }


void ReturnStmt::verify() {
  Stmt::verify();
  if (astType != STMT_RETURN) {
    INT_FATAL(this, "Bad ReturnStmt::astType");
  }
  if (!expr)
    INT_FATAL(this, "Return statement has no return expression.");
  if (normalized) {
    FnSymbol* fn = dynamic_cast<FnSymbol*>(parentSymbol);
    SymExpr* sym = dynamic_cast<SymExpr*>(expr);
    if (!fn)
      INT_FATAL(this, "Return statement is not in a function.");
    if (fn->body->body->last() != this)
      INT_FATAL(this, "Return statement is in middle of function.");
    if (!sym)
      INT_FATAL(this, "Return statement does not return a symbol.");
  }
}


ReturnStmt*
ReturnStmt::copyInner(ASTMap* map) {
  return new ReturnStmt(COPY_INT(expr), yield);
}


void ReturnStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ReturnStmt::replaceChild");
  }
}


void ReturnStmt::print(FILE* outfile) {
  fprintf(outfile, "\n");
  fprintf(outfile, yield ? "yield" : "return");
  if (expr) {
    fprintf(outfile, " ");
    expr->print(outfile);
  }
  fprintf(outfile, ";");
}


void ReturnStmt::codegenStmt(FILE* outfile) {
  if (yield)
    INT_FATAL(this, "Yield should be removed before codegen");
  fprintf(outfile, "return");
  if (!returnsVoid()) {
    fprintf(outfile, " ");
    expr->codegen(outfile);
  }
  fprintf(outfile, ";\n");
}


bool ReturnStmt::returnsVoid() {
  SymExpr* sym = dynamic_cast<SymExpr*>(expr);
  return sym && sym->var->type == dtVoid;
}


BlockStmt::BlockStmt(AList<Stmt>* init_body, BlockTag init_blockTag) :
  Stmt(STMT_BLOCK),
  blockTag(init_blockTag),
  body(init_body),
  blkScope(NULL),
  pre_loop(NULL),
  post_loop(NULL),
  param_low(NULL),
  param_high(NULL),
  param_index(NULL)
{}


BlockStmt::BlockStmt(Stmt* init_body, BlockTag init_blockTag) :
  Stmt(STMT_BLOCK),
  blockTag(init_blockTag),
  body(new AList<Stmt>(init_body)),
  blkScope(NULL),
  pre_loop(NULL),
  post_loop(NULL),
  param_low(NULL),
  param_high(NULL),
  param_index(NULL)
{}


void BlockStmt::verify() {
  Stmt::verify();
  if (astType != STMT_BLOCK) {
    INT_FATAL(this, "Bad BlockStmt::astType");
  }
}


BlockStmt*
BlockStmt::copyInner(ASTMap* map) {
  BlockStmt* _this = new BlockStmt(COPY_INT(body), blockTag);
  _this->param_low = COPY_INT(param_low);
  _this->param_high = COPY_INT(param_high);
  _this->param_index = COPY_INT(param_index);
  return _this;
}


void BlockStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = dynamic_cast<AList<Stmt>*>(new_ast);
  } else if (old_ast == param_low) {
    param_low = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == param_high) {
    param_high = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == param_index) {
    param_index = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in BlockStmt::replaceChild");
  }
}


void BlockStmt::print(FILE* outfile) {
  switch (blockTag) {
  case BLOCK_ATOMIC:
    fprintf(outfile, "atomic ");
    break;
  case BLOCK_COBEGIN:
    fprintf(outfile, "cobegin ");
  default:
    break;
  }
  fprintf(outfile, "{\n");
  if (body) {
    body->print(outfile, "\n");
  fprintf(outfile, "\n");
  }
  fprintf(outfile, "}");
}


void BlockStmt::codegenStmt(FILE* outfile) {
  static bool need_threadlib = false;

  fprintf(outfile, "{\n");
  inBlockStmt++;
  if (blkScope) {
    blkScope->codegen(outfile, "\n");
  }
  if (body) {
    if ((BLOCK_COBEGIN == blockTag) && (parallelPass)) {
      if (!need_threadlib) {
        addLibInfo ("-lpthread");
        need_threadlib = true;
      }
      int stmt_cnt;
      int num_stmts = body->length();
      // For now, assume all statements will be forked.

      // gen code for the function pointer array
      fprintf (outfile, "_chpl_threadfp_t fpv[%d] = {\n", num_stmts);
      stmt_cnt = 0;
      for_alist (Stmt, stmt, body) {
        if (ExprStmt *estmt=dynamic_cast<ExprStmt*>(stmt)) {
          if (CallExpr *cexpr=dynamic_cast<CallExpr*>(estmt->expr)) {
            if (SymExpr *sexpr=dynamic_cast<SymExpr*>(cexpr->baseExpr)) {
              fprintf (outfile, "(_chpl_threadfp_t)%s", sexpr->var->cname);
            } else {
              INT_FATAL(stmt, "cobegin codegen - call expr not a SymExpr");
            } 
          } else {
            INT_FATAL(stmt, "cobegin codegen - statement not a CallExpr");
          }
        } else {
          INT_FATAL(stmt, "cobegin codegen - statement not an ExprStmt");
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
      for_alist (Stmt, stmt, body) {
        if (ExprStmt *estmt=dynamic_cast<ExprStmt*>(stmt)) {
          if (CallExpr *cexpr=dynamic_cast<CallExpr*>(estmt->expr)) {
            Expr *actuals = cexpr->argList->first();
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

    } else {  // else, serial code gen
      body->codegen(outfile, "");
    }
  }
  fprintf(outfile, "}\n");
  inBlockStmt--;
}


void
BlockStmt::insertAtHead(BaseAST* ast) {
  body->insertAtHead(ast);
}


void
BlockStmt::insertAtTail(BaseAST* ast) {
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
  Stmt(STMT_COND),
  condExpr(iCondExpr),
  thenStmt(NULL),
  elseStmt(NULL)
{
  BlockStmt* bs;
  if (Stmt* s = dynamic_cast<Stmt*>(iThenStmt)) {
    thenStmt = new BlockStmt(s);
  } else if (AList<Stmt>* s = dynamic_cast<AList<Stmt>*>(iThenStmt)) {
    if (s->length() == 1 && (bs = dynamic_cast<BlockStmt*>(s->only()))) {
      bs->remove();
      thenStmt = bs;
    } else
      thenStmt = new BlockStmt(s);
  } else {
    INT_FATAL(iThenStmt, "Bad then-stmt passed to CondStmt constructor");
  }
  if (!iElseStmt)
    return;
  if (Stmt* s = dynamic_cast<Stmt*>(iElseStmt)) {
    elseStmt = new BlockStmt(s);
  } else if (AList<Stmt>* s = dynamic_cast<AList<Stmt>*>(iElseStmt)) {
    if (s->length() == 1 && (bs = dynamic_cast<BlockStmt*>(s->only()))) {
      bs->remove();
      elseStmt = bs;
    } else
      elseStmt = new BlockStmt(s);
  } else {
    INT_FATAL(iElseStmt, "Bad else-stmt passed to CondStmt constructor");
  }
}


void CondStmt::verify() {
  Stmt::verify();
  if (astType != STMT_COND) {
    INT_FATAL(this, "Bad CondStmt::astType");
  }

  if (!condExpr) {
    INT_FATAL(this, "CondStmt has no condExpr");
  }

  if (!thenStmt) {
    INT_FATAL(this, "CondStmt has no thenStmt");
  }

  if (condExpr->next || condExpr->prev) {
    INT_FATAL(this, "CondStmt::condExpr is a list");
  }

  if (thenStmt->next || thenStmt->prev) {
    INT_FATAL(this, "CondStmt::thenStmt is a list");
  }

  if (elseStmt && (elseStmt->next || elseStmt->prev)) {
    INT_FATAL(this, "CondStmt::elseStmt is a list");
  }
}


CondStmt*
CondStmt::copyInner(ASTMap* map) {
  return new CondStmt(COPY_INT(condExpr),
                      COPY_INT(thenStmt),
                      COPY_INT(elseStmt));
}


void CondStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
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


void CondStmt::print(FILE* outfile) {
  fprintf(outfile, "if (");
  condExpr->print(outfile);
  fprintf(outfile, ") ");
  thenStmt->print(outfile);
  if (elseStmt) {
    fprintf(outfile, " else ");
    elseStmt->print(outfile);
  }
}

void CondStmt::codegenStmt(FILE* outfile) {
  fprintf(outfile, "if (");
  condExpr->codegen(outfile);
  fprintf(outfile, ") ");
  thenStmt->codegenStmt(outfile);
  if (elseStmt) {
    fprintf(outfile, " else ");
    elseStmt->codegenStmt(outfile);
  }
}


WhenStmt::WhenStmt(AList<Expr>* init_caseExprs, BlockStmt* init_doStmt) :
  Stmt(STMT_WHEN),
  caseExprs(init_caseExprs),
  doStmt(init_doStmt)
{
  if (doStmt)
    doStmt = new BlockStmt(doStmt);
}


WhenStmt::WhenStmt(AList<Expr>* init_caseExprs, Stmt* init_doStmt) :
  Stmt(STMT_WHEN),
  caseExprs(init_caseExprs),
  doStmt(new BlockStmt(init_doStmt))
{ }


WhenStmt::WhenStmt(AList<Expr>* init_caseExprs, AList<Stmt>* init_doStmt) :
  Stmt(STMT_WHEN),
  caseExprs(init_caseExprs),
  doStmt(new BlockStmt(init_doStmt))
{ }


void WhenStmt::verify() {
  Stmt::verify();
  if (astType != STMT_WHEN) {
    INT_FATAL(this, "Bad WhenStmt::astType");
  }
}


WhenStmt*
WhenStmt::copyInner(ASTMap* map) {
  return new WhenStmt(COPY_INT(caseExprs), COPY_INT(doStmt));
}


void WhenStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == caseExprs) {
    caseExprs = dynamic_cast<AList<Expr>*>(new_ast);
  } else if (old_ast == doStmt) {
    doStmt = dynamic_cast<BlockStmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in WhenStmt::replaceChild");
  }
}


void WhenStmt::print(FILE* outfile) {
  fprintf(outfile, "when ");
  caseExprs->print(outfile);
  fprintf(outfile, " do ");
  doStmt->print(outfile);
}


void WhenStmt::codegenStmt(FILE* outfile) {
  INT_FATAL(this, "WhenStmt::codegenStmt encountered");
}


LabelStmt::LabelStmt(DefExpr* iDefLabel) :
  Stmt(STMT_LABEL),
  defLabel(iDefLabel),
  stmt(new BlockStmt())
{ }

LabelStmt::LabelStmt(Symbol* iDefLabel) :
  Stmt(STMT_LABEL),
  defLabel(new DefExpr(iDefLabel)),
  stmt(new BlockStmt())
{ }

LabelStmt::LabelStmt(char* iDefLabel) :
  Stmt(STMT_LABEL),
  defLabel(new DefExpr(new LabelSymbol(iDefLabel))),
  stmt(new BlockStmt())
{ }


void LabelStmt::verify() {
  Stmt::verify();
  if (astType != STMT_LABEL) {
    INT_FATAL(this, "Bad LabelStmt::astType");
  }

  if (!defLabel || !dynamic_cast<LabelSymbol*>(defLabel->sym)) {
    INT_FATAL(this, "LabelStmt does not define a LabelSymbol");
  }
}


LabelStmt*
LabelStmt::copyInner(ASTMap* map) {
  return new LabelStmt(COPY_INT(defLabel));
}


void LabelStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == stmt) {
    INT_FATAL("LabelStmt::stmt should not be used");
  } else if (old_ast == defLabel) {
    defLabel = dynamic_cast<DefExpr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in LabelStmt::replaceChild");
  }
}


void LabelStmt::print(FILE* outfile) {
  fprintf(outfile, "label ");
  defLabel->print(outfile);
  fprintf(outfile, " ");
  stmt->print(outfile);
}


void LabelStmt::codegenStmt(FILE* outfile) {
  defLabel->sym->codegen(outfile);
  fprintf(outfile, ":;\n");
}


char* LabelStmt::labelName(void) {
  return defLabel->sym->name;
}


GotoStmt::GotoStmt(gotoType init_goto_type) :
  Stmt(STMT_GOTO),
  label(NULL),
  goto_type(init_goto_type)
{
}


GotoStmt::GotoStmt(gotoType init_goto_type, char* init_label) :
  Stmt(STMT_GOTO),
  label(init_label ? new UnresolvedSymbol(init_label) : NULL),
  goto_type(init_goto_type)
{
}


GotoStmt::GotoStmt(gotoType init_goto_type, Symbol* init_label) :
  Stmt(STMT_GOTO),
  label(init_label),
  goto_type(init_goto_type)
{
}


void GotoStmt::verify() {
  Stmt::verify();
  if (astType != STMT_GOTO) {
    INT_FATAL(this, "Bad GotoStmt::astType");
  }
}


GotoStmt*
GotoStmt::copyInner(ASTMap* map) {
  return new GotoStmt(goto_type, label);
}


void GotoStmt::print(FILE* outfile) {
  switch (goto_type) {
  case goto_normal:
    fprintf(outfile, "goto ");
    break;
  case goto_break:
    fprintf(outfile, "break goto ");
    break;
  case goto_continue:
    fprintf(outfile, "continue goto ");
    break;
  }
  label->print(outfile);
  fprintf(outfile, "\n");
}


void GotoStmt::codegenStmt(FILE* outfile) {
  fprintf(outfile, "goto ");
  label->codegen(outfile);
  fprintf(outfile, ";\n");
}
