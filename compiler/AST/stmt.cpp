#include <string.h>
#include <typeinfo>
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "symtab.h"
#include "../traversals/fixup.h"
#include "../traversals/updateSymbols.h"
#include "../passes/runAnalysis.h"

bool printCppLineno = false;
bool printChplLineno = false;
bool inFunction = false;
bool inUserModule = false;
bool justStartedGeneratingFunction = false;

Stmt::Stmt(astType_t astType) :
  BaseAST(astType),
  parentStmt(NULL),
  ainfo(NULL)
{}


void Stmt::verify(void) {
  INT_FATAL(this, "Stmt::verify() should never be called");
}


FnSymbol *Stmt::parentFunction() {
  ModuleSymbol *mod = dynamic_cast<ModuleSymbol*>(parentSymbol);
  if (mod)
    return mod->initFn;
  else
    return dynamic_cast<FnSymbol*>(parentSymbol);
}


Stmt*
Stmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
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
    
    if (printChplLineno && inFunction && inUserModule) {
      if (strcmp(filename, priorFilename) != 0 ||  
          justStartedGeneratingFunction) {
        fprintf(outfile, "_chpl_input_filename = \"%s\";\n", filename);
        priorFilename = copystring(filename);
        justStartedGeneratingFunction = false;
      }
      if (lineno != priorLineno) {
        fprintf(outfile, "_chpl_input_linenumber = %d;\n", lineno);
        priorLineno = lineno;
      }
    }
  }
  codegenStmt(outfile);
}


void Stmt::codegenStmt(FILE* outfile) {
  INT_FATAL(this, "codegenStmt not implemented for Stmt subclass");
}


void Stmt::traverse(Traversal* traversal, bool atTop) {
  SymScope* saveScope = NULL;
  if (atTop) {
    saveScope = Symboltable::setCurrentScope(parentScope);
  }
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->preProcessStmt(this);
  }
  if (atTop || traversal->exploreChildStmts) {
    traverseStmt(traversal);
  }
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->postProcessStmt(this);
  }
  if (atTop) {
    Symboltable::setCurrentScope(saveScope);
  }
}


void Stmt::traverseDef(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Attempt to traverse the definition of a statement");
}


void Stmt::traverseStmt(Traversal* traversal) {
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
  if (astType != STMT_EXPR) {
    INT_FATAL(this, "Bad ExprStmt::astType");
  }
}


ExprStmt*
ExprStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ExprStmt(COPY_INTERNAL(expr));
}


void ExprStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ExprStmt::replaceChild");
  }
}


void ExprStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
}


void ExprStmt::print(FILE* outfile) {
  expr->print(outfile);
  fprintf(outfile, ";");
}


void ExprStmt::codegenStmt(FILE* outfile) {
  expr->codegen(outfile);
  fprintf(outfile, ";");
}

bool ExprStmt::noCodegen() {
  if (expr)
    return expr->noCodegen();
  else
    return false;
}

ReturnStmt::ReturnStmt(Expr* initExpr, bool init_yield) :
  ExprStmt(initExpr),
  yield(init_yield)
{
  astType = STMT_RETURN;
}


void ReturnStmt::verify() {
  if (astType != STMT_RETURN) {
    INT_FATAL(this, "Bad ReturnStmt::astType");
  }
}


ReturnStmt*
ReturnStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ReturnStmt(COPY_INTERNAL(expr));
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
  if (yield) {
    INT_FATAL(this, "Yield should be removed before codegen");
  }
  fprintf(outfile, "return");
  if (expr) {
    fprintf(outfile, " ");
    expr->codegen(outfile);
  }
  fprintf(outfile, ";");
}


BlockStmt::BlockStmt(AList<Stmt>* init_body, blockStmtType init_blockType) :
  Stmt(STMT_BLOCK),
  blockType(init_blockType),
  body(init_body),
  blkScope(NULL)
{}


BlockStmt::BlockStmt(Stmt* init_body, blockStmtType init_blockType) :
  Stmt(STMT_BLOCK),
  blockType(init_blockType),
  body(new AList<Stmt>(init_body)),
  blkScope(NULL)
{}


void BlockStmt::verify() {
  if (astType != STMT_BLOCK) {
    INT_FATAL(this, "Bad BlockStmt::astType");
  }
}


BlockStmt*
BlockStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new BlockStmt(CLONE_INTERNAL(body), blockType);
}


void BlockStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = dynamic_cast<AList<Stmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in BlockStmt::replaceChild");
  }
}


void BlockStmt::traverseStmt(Traversal* traversal) {
  SymScope* saveScope = NULL;
  if (blkScope)
    saveScope = Symboltable::setCurrentScope(blkScope);
  TRAVERSE(body, traversal, false);
  if (saveScope)
    Symboltable::setCurrentScope(saveScope);
}


void BlockStmt::print(FILE* outfile) {
  switch (blockType) {
  case BLOCK_NORMAL:
    break;
  case BLOCK_ATOMIC:
    fprintf(outfile, "atomic ");
    break;
  case BLOCK_COBEGIN:
    fprintf(outfile, "cobegin ");
  }
  fprintf(outfile, "{\n");
  if (body) {
    body->print(outfile, "\n");
  fprintf(outfile, "\n");
  }
  fprintf(outfile, "}");
}


void BlockStmt::codegenStmt(FILE* outfile) {
  fprintf(outfile, "{\n");
  if (blkScope) {
    blkScope->codegen(outfile, "\n");
  }
  if (body) body->codegen(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "}");
}


WhileLoopStmt::WhileLoopStmt(bool init_whileDo,
                             Expr* init_cond,
                             BlockStmt* init_block) :
  Stmt(STMT_WHILELOOP),
  block(init_block),
  isWhileDo(init_whileDo),
  condition(init_cond)
{ }


WhileLoopStmt::WhileLoopStmt(bool init_whileDo,
                             Expr* init_cond,
                             Stmt* init_block) :
  Stmt(STMT_WHILELOOP),
  block(new BlockStmt(init_block)),
  isWhileDo(init_whileDo),
  condition(init_cond)
{ }


WhileLoopStmt::WhileLoopStmt(bool init_whileDo,
                             Expr* init_cond,
                             AList<Stmt>* init_block) :
  Stmt(STMT_WHILELOOP),
  block(new BlockStmt(init_block)),
  isWhileDo(init_whileDo),
  condition(init_cond)
{ }


void WhileLoopStmt::verify() {
  if (astType != STMT_WHILELOOP) {
    INT_FATAL(this, "Bad WhileLoopStmt::astType");
  }
}


WhileLoopStmt* 
WhileLoopStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new WhileLoopStmt(isWhileDo,
                           COPY_INTERNAL(condition),
                           COPY_INTERNAL(block));
}


void WhileLoopStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == condition) {
    condition = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == block) {
    block = dynamic_cast<BlockStmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in WhileLoopStmt::replaceChild");
  }
}


void WhileLoopStmt::traverseStmt(Traversal* traversal) {
  if (isWhileDo) {
    TRAVERSE(condition, traversal, false);
    TRAVERSE(block, traversal, false);
  } else {
    TRAVERSE(block, traversal, false);
    TRAVERSE(condition, traversal, false);
  }
}


void WhileLoopStmt::print(FILE* outfile) {

  if (isWhileDo) {
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ") ");
    block->print(outfile);
  } else {
    fprintf(outfile, "do ");
    block->print(outfile);
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ")");
  }
}


void WhileLoopStmt::codegenStmt(FILE* outfile) {

  if (isWhileDo) {
    fprintf(outfile, "while (");
    condition->codegen(outfile);
    fprintf(outfile, ") {\n");
    block->codegen(outfile);
    fprintf(outfile, "\n}");
  } else { 
    fprintf(outfile, "do {\n");
    block->codegen(outfile);
    fprintf(outfile, "\n} while (");
    condition->codegen(outfile);
    fprintf(outfile, ");\n");
  }
}


ForLoopStmt::ForLoopStmt(ForLoopStmtTag initForLoopStmtTag,
                         AList<DefExpr>* initIndices,
                         AList<Expr>* initIterators, 
                         BlockStmt* initInnerStmt) :
  Stmt(STMT_FORLOOP),
  forLoopStmtTag(initForLoopStmtTag),
  indices(initIndices),
  iterators(initIterators),
  innerStmt(initInnerStmt),
  indexScope(NULL)
{ }


void ForLoopStmt::verify() {
  if (astType != STMT_FORLOOP) {
    INT_FATAL(this, "Bad ForLoopStmt::astType");
  }
}


ForLoopStmt*
ForLoopStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ForLoopStmt(forLoopStmtTag,
                         CLONE_INTERNAL(indices),
                         CLONE_INTERNAL(iterators),
                         CLONE_INTERNAL(innerStmt));
}


void ForLoopStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == indices) {
    indices = dynamic_cast<AList<DefExpr>*>(new_ast);
  } else if (old_ast == iterators) {
    iterators = dynamic_cast<AList<Expr>*>(new_ast);
  } else if (old_ast == innerStmt) {
    innerStmt = dynamic_cast<BlockStmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ForLoopStmt::replaceChild");
  }
}


void ForLoopStmt::traverseStmt(Traversal* traversal) {
  SymScope* saveScope = NULL;
  if (indexScope)
    saveScope = Symboltable::setCurrentScope(indexScope);
  TRAVERSE(indices, traversal, false);
  TRAVERSE(iterators, traversal, false);
  TRAVERSE(innerStmt, traversal, false);
  if (saveScope)
    Symboltable::setCurrentScope(saveScope);
}


void ForLoopStmt::print(FILE* outfile) {
  switch (forLoopStmtTag) {
  case FORLOOPSTMT_FOR:
    fprintf(outfile, "for ");
    break;
  case FORLOOPSTMT_ORDEREDFORALL:
    fprintf(outfile, "ordered forall ");
    break;
  case FORLOOPSTMT_FORALL:
    fprintf(outfile, "forall ");
    break;
  }
  indices->print(outfile);
  fprintf(outfile, " in ");
  iterators->print(outfile);
  innerStmt->print(outfile);
}


void ForLoopStmt::codegenStmt(FILE* outfile) {
  DefExpr* index = indices->first();

  fprintf(outfile, "{\n");
  index->sym->codegenDef(outfile);
  switch (forLoopStmtTag) {
  case FORLOOPSTMT_FOR:
    fprintf(outfile, "_FOR_S(");
    break;
  case FORLOOPSTMT_ORDEREDFORALL:
    fprintf(outfile, "_FORALL_S(");
    break;
  case FORLOOPSTMT_FORALL:
    fprintf(outfile, "_FORALL_S(");
    break;
  }
  index->sym->codegen(outfile);
  fprintf(outfile, ", ");
  iterators->only()->codegen(outfile);
  fprintf(outfile, ", ");
  ClassType* seqType = dynamic_cast<ClassType*>(iterators->only()->typeInfo());
  fprintf(outfile, "%s", seqType->fields.v[1]->type->symbol->cname);
  fprintf(outfile, ", ");
  fprintf(outfile, "%ld", id);
  fprintf(outfile, ") {\n");
  innerStmt->codegen(outfile);
  fprintf(outfile, "\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "}\n");
}


CondStmt::CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt) :
  Stmt(STMT_COND),
  condExpr(iCondExpr),
  thenStmt(NULL),
  elseStmt(NULL)
{
  if (BlockStmt* s = dynamic_cast<BlockStmt*>(iThenStmt)) {
    thenStmt = s;
  } else if (Stmt* s = dynamic_cast<Stmt*>(iThenStmt)) {
    thenStmt = new BlockStmt(s);
  } else if (AList<Stmt>* s = dynamic_cast<AList<Stmt>*>(iThenStmt)) {
    thenStmt = new BlockStmt(s);
  } else {
    INT_FATAL(iThenStmt, "Bad then-stmt passed to CondStmt constructor");
  }
  if (!iElseStmt)
    return;
  if (BlockStmt* s = dynamic_cast<BlockStmt*>(iElseStmt)) {
    elseStmt = s;
  } else if (Stmt* s = dynamic_cast<Stmt*>(iElseStmt)) {
    elseStmt = new BlockStmt(s);
  } else if (AList<Stmt>* s = dynamic_cast<AList<Stmt>*>(iElseStmt)) {
    elseStmt = new BlockStmt(s);
  } else {
    INT_FATAL(iElseStmt, "Bad else-stmt passed to CondStmt constructor");
  }
}


void CondStmt::verify() {
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


//// DANGER //// See note below
void CondStmt::addElseStmt(BlockStmt* init_elseStmt) {
  if (elseStmt) {
    INT_FATAL(this, "overwriting existing else Stmt");
  }
  elseStmt = init_elseStmt;
  //SJD : this is not fixed up
}


CondStmt*
CondStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new CondStmt(COPY_INTERNAL(condExpr),
                      COPY_INTERNAL(thenStmt),
                      COPY_INTERNAL(elseStmt));
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


void CondStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(condExpr, traversal, false);
  TRAVERSE(thenStmt, traversal, false);
  TRAVERSE(elseStmt, traversal, false);
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
  fprintf(outfile, ") {\n");
  thenStmt->codegenStmt(outfile);
  fprintf(outfile, "\n}");
  if (elseStmt) {
    fprintf(outfile, " else {\n");
    elseStmt->codegenStmt(outfile);
    fprintf(outfile, "\n}");
  }
}


WhenStmt::WhenStmt(AList<Expr>* init_caseExprs, BlockStmt* init_doStmt) :
  Stmt(STMT_WHEN),
  caseExprs(init_caseExprs),
  doStmt(init_doStmt)
{ }


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
  if (astType != STMT_WHEN) {
    INT_FATAL(this, "Bad WhenStmt::astType");
  }
}


WhenStmt*
WhenStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new WhenStmt(COPY_INTERNAL(caseExprs),
                      COPY_INTERNAL(doStmt));
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


void WhenStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(caseExprs, traversal, false);
  TRAVERSE(doStmt, traversal, false);
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


SelectStmt::SelectStmt(Expr* init_caseExpr, AList<WhenStmt>* init_whenStmts) :
  Stmt(STMT_SELECT),
  caseExpr(init_caseExpr),
  whenStmts(init_whenStmts)
{ }


void SelectStmt::verify() {
  if (astType != STMT_SELECT) {
    INT_FATAL(this, "Bad SelectStmt::astType");
  }
}


SelectStmt*
SelectStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new SelectStmt(COPY_INTERNAL(caseExpr), COPY_INTERNAL(whenStmts));
}


void SelectStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == caseExpr) {
    caseExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == whenStmts) {
    whenStmts = dynamic_cast<AList<WhenStmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in SelectStmt::replaceChild");
  }
}


void SelectStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(caseExpr, traversal, false);
  whenStmts->traverse(traversal, false);
}


void SelectStmt::print(FILE* outfile) {
  fprintf(outfile, "select ");
  caseExpr->print(outfile);
  fprintf(outfile, " {\n");
  whenStmts->print(outfile);
  fprintf(outfile, "}\n");
}


void SelectStmt::codegenStmt(FILE* outfile) {
  bool firstWhenStmt = true;
  for (WhenStmt* whenStmt = whenStmts->first();
       whenStmt;
       whenStmt = whenStmts->next()) {
    if (!firstWhenStmt) {
      fprintf(outfile, " else ");
    } else {
      firstWhenStmt = false;
    }
    fprintf(outfile, "if (");
    if (!whenStmt->caseExprs->isEmpty()) {
      bool firstCaseExpr = true;
      for (Expr* expr = whenStmt->caseExprs->first();
           expr;
           expr = whenStmt->caseExprs->next()) {
        if (!firstCaseExpr) {
          fprintf(outfile, " || ");
        } else {
          firstCaseExpr = false;
        }
        fprintf(outfile, "(");
        caseExpr->codegen(outfile);
        fprintf(outfile, " == ");
        expr->codegen(outfile);
        fprintf(outfile, ")");
      }
      fprintf(outfile, ") ");
    } else {
      fprintf(outfile, "true) /* otherwise */ ");
    }
    whenStmt->doStmt->codegenStmt(outfile);
  }
}


LabelStmt::LabelStmt(DefExpr* initDefLabel, Stmt* initStmt) :
  Stmt(STMT_LABEL),
  defLabel(initDefLabel),
  stmt(initStmt)
{ }


void LabelStmt::verify() {
  if (astType != STMT_LABEL) {
    INT_FATAL(this, "Bad LabelStmt::astType");
  }

  if (!defLabel || !dynamic_cast<LabelSymbol*>(defLabel->sym)) {
    INT_FATAL(this, "LabelStmt does not define a LabelSymbol");
  }
}


LabelStmt*
LabelStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new LabelStmt(COPY_INTERNAL(defLabel), COPY_INTERNAL(stmt));
}


void LabelStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == stmt) {
    stmt = dynamic_cast<Stmt*>(new_ast);
  } else if (old_ast == defLabel) {
    defLabel = dynamic_cast<DefExpr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in LabelStmt::replaceChild");
  }
}


void LabelStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(defLabel, traversal, false);
  TRAVERSE(stmt, traversal, false);
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
  stmt->codegenStmt(outfile);
  defLabel->sym->codegen(outfile);
  fprintf(outfile, "_post:;\n");
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
  if (astType != STMT_GOTO) {
    INT_FATAL(this, "Bad GotoStmt::astType");
  }
}


GotoStmt*
GotoStmt::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new GotoStmt(goto_type, label);
}


void GotoStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(label, traversal, false);
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
  if (goto_type == goto_break) {
    fprintf(outfile, "_post");
  }
  fprintf(outfile, ";\n");
}
