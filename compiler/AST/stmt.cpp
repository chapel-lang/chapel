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


Stmt::Stmt(astType_t astType) :
  BaseAST(astType),
  parentSymbol(NULL),
  parentStmt(NULL),
  ainfo(NULL),
  pragmas(NULL)
{}


FnSymbol *Stmt::parentFunction() {
  ModuleSymbol *mod = dynamic_cast<ModuleSymbol*>(parentSymbol);
  if (mod)
    return mod->initFn;
  else
    return dynamic_cast<FnSymbol*>(parentSymbol);
}


Stmt* Stmt::copy(bool clone, Map<BaseAST*,BaseAST*>* map, Vec<BaseAST*>* update_list) {
  if (map == NULL) {
    map = new Map<BaseAST*,BaseAST*>();
  }
  Stmt* new_stmt = copyInternal(clone, map);
  if (update_list) {
    for (int j = 0; j < update_list->n; j++) {
      for (int i = 0; i < map->n; i++) {
        if (update_list->v[j] == map->v[i].key) {
          update_list->v[j] = map->v[i].value;
        }
      }
    }
  }
  TRAVERSE(new_stmt, new UpdateSymbols(map), true);
  return new_stmt;
}


Stmt* Stmt::copyInternal(bool clone, Map<BaseAST*,BaseAST*>* map) {
  if (!this) {
    return this;
  }

  Stmt* new_stmt = copyStmt(clone, map);

  new_stmt->lineno = lineno;
  new_stmt->filename = filename;
  new_stmt->pragmas = pragmas;
  if (!RunAnalysis::isRunning) {
    new_stmt->ainfo = ainfo;
  }
  if (map) {
    map->put(this, new_stmt);
  }
  return new_stmt;
}


Stmt* Stmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "copy not implemented for Stmt subclass");
  return NULL;
}


void Stmt::verify(void) {

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


void Stmt::callReplaceChild(BaseAST* new_ast) {
  if (parentStmt) {
    parentStmt->replaceChild(this, new_ast);
  } else {
    parentSymbol->replaceChild(this, new_ast);
  }
}


bool Stmt::hasPragma(char* str) {
  Pragma* pr = pragmas->first();
  while (pr) {
    if (!strcmp(pr->str, str)) {
      return true;
    }
    pr = pragmas->next();
  }
  return false;
}


void Stmt::addPragma(char* str) {
  if (pragmas) {
    pragmas->insertAtTail(new Pragma(copystring(str)));
  } else {
    pragmas = new AList<Pragma>(new Pragma(copystring(str)));
  }
}


NoOpStmt::NoOpStmt(void) :
  Stmt(STMT_NOOP)
{}


Stmt* NoOpStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new NoOpStmt();
}


void NoOpStmt::print(FILE* outfile) {
  fprintf(outfile, ";");
}


void NoOpStmt::codegen(FILE* outfile) {
  //  fprintf(outfile, "{}\n");
}


DefStmt::DefStmt(DefExpr* init_defExprls) :
  Stmt(STMT_DEF),
  defExprls(new AList<DefExpr>(init_defExprls))
{}


DefStmt::DefStmt(AList<DefExpr>* init_defExprls) :
  Stmt(STMT_DEF),
  defExprls(init_defExprls)
{}


Stmt* DefStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new DefStmt(defExprls->copy(clone, map));
}


void DefStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defExprls) {
    defExprls = dynamic_cast<AList<DefExpr>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in DefStmt::replaceChild(old, new)");
  }
}


void DefStmt::traverseStmt(Traversal* traversal) {
  defExprls->traverse(traversal, false);
}


void DefStmt::print(FILE* outfile) {
  defExprls->print(outfile, "\n");
}


void DefStmt::codegen(FILE* outfile) { /* Noop */ }


VarSymbol* DefStmt::varDef() {
  if (defExprls->isEmpty()) {
    return NULL;
  }
  return dynamic_cast<VarSymbol*>(defExprls->first()->sym);
}


FnSymbol* DefStmt::fnDef() {
  if (defExprls->isEmpty()) {
    return NULL;
  }
  return dynamic_cast<FnSymbol*>(defExprls->first()->sym);
}


bool DefStmt::definesTypes() {
  if (defExprls->isEmpty()) {
    return false;
  }
  for (DefExpr* defExpr = defExprls->first();
       defExpr;
       defExpr = defExprls->next()) {
    if (!dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      return false;
    }
  }
  return true;
}


bool DefStmt::definesFunctions() {
  if (defExprls->isEmpty()) {
    return false;
  }
  for (DefExpr* defExpr = defExprls->first();
       defExpr;
       defExpr = defExprls->next()) {
    if (!dynamic_cast<FnSymbol*>(defExpr->sym)) {
      return false;
    }
  }
  return true;
}


bool DefStmt::definesVariables() {
  if (defExprls->isEmpty()) {
    return false;
  }
  for (DefExpr* defExpr = defExprls->first();
       defExpr;
       defExpr = defExprls->next()) {
    if (!dynamic_cast<VarSymbol*>(defExpr->sym)) {
      return false;
    }
  }
  return true;
}


Vec<VarSymbol*>* DefStmt::varDefSet() {
  Vec<VarSymbol*>* var_set = new Vec<VarSymbol*>();
  DefExpr* def_expr = defExprls->first();
  while (def_expr) {
    VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym);
    var_set->set_add(var);
    def_expr = defExprls->next();
  }
  return var_set;
}


ExprStmt::ExprStmt(Expr* initExpr) :
  Stmt(STMT_EXPR),
  expr(initExpr) 
{ }


Stmt* ExprStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ExprStmt(expr->copyInternal(clone, map));
}


void ExprStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ExprStmt::replaceChild(old, new)");
  }
}


void ExprStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
}


void ExprStmt::print(FILE* outfile) {
  expr->print(outfile);
  fprintf(outfile, ";");
}


void ExprStmt::codegen(FILE* outfile) {
  expr->codegen(outfile);
  fprintf(outfile, ";");
}


ReturnStmt::ReturnStmt(Expr* initExpr) :
  ExprStmt(initExpr)
{
  astType = STMT_RETURN;
}


Stmt* ReturnStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ReturnStmt(expr->copyInternal(clone, map));
}


void ReturnStmt::print(FILE* outfile) {
  fprintf(outfile, "\n");
  fprintf(outfile, "return");
  if (expr) {
    fprintf(outfile, " ");
    expr->print(outfile);
  }
  fprintf(outfile, ";");
}


void ReturnStmt::codegen(FILE* outfile) {
  fprintf(outfile, "return");
  if (expr) {
    fprintf(outfile, " ");
    if (expr->isRef()) {
      FnSymbol *fn = parentFunction();
      if (!fn->_getter || !is_Value_Type(fn->retType))
        fprintf(outfile, "*");
    }
    expr->codegen(outfile);
  }
  fprintf(outfile, ";");
}


WithStmt::WithStmt(Expr* initExpr) :
  ExprStmt(initExpr)
{
  astType = STMT_WITH;
}


Stmt* WithStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new WithStmt(expr->copyInternal(clone, map));
}


void WithStmt::print(FILE* outfile) {
  fprintf(outfile, "with ");
  expr->print(outfile);
  fprintf(outfile, "\n");
}


void WithStmt::codegen(FILE* outfile) {
  INT_FATAL(this, "With statement encountered in codegen()");
}


StructuralType* WithStmt::getStruct(void) {
  if (Variable* var = dynamic_cast<Variable*>(expr)) {
    if (StructuralType* result = 
        dynamic_cast<StructuralType*>(var->var->type)) {
      return result;
    } else if (UnresolvedSymbol* unresolved = 
             dynamic_cast<UnresolvedSymbol*>(var->var)) {
      if (StructuralType* result = 
          dynamic_cast<StructuralType*>(Symboltable::lookup(unresolved->name)->type)) {
        return result;
      } else {
        INT_FATAL(this, "Bad with statement");
      }
    } else {
      INT_FATAL(this, "Bad with statement");
    }
  } else {
    INT_FATAL(this, "Bad with statement");
  }
  return NULL;
}


UseStmt::UseStmt(Expr* initExpr) :
  ExprStmt(initExpr)
{
  astType = STMT_USE;
}


Stmt* UseStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new UseStmt(expr->copyInternal(clone, map));
}


void UseStmt::print(FILE* outfile) {
  fprintf(outfile, "use ");
  expr->print(outfile);
  fprintf(outfile, "\n");
}


void UseStmt::codegen(FILE* outfile) {
  fprintf(outfile, "/* 'use ");
  expr->codegen(outfile);
  fprintf(outfile, "' was here */");
}


ModuleSymbol* UseStmt::getModule(void) {
  if (Variable* variable = dynamic_cast<Variable*>(expr)) {
    if (Symbol* symbol = variable->var) {
      if (ModuleSymbol* module =
          dynamic_cast<ModuleSymbol*>(Symboltable::lookup(symbol->name))) {
        return module;
      }
    }
  }
  return NULL;
}


BlockStmt::BlockStmt(AList<Stmt>* init_body, SymScope* init_scope) :
  Stmt(STMT_BLOCK),
  body(init_body),
  blkScope(init_scope)
{}


void BlockStmt::addBody(AList<Stmt>* init_body) {
  if (body->isEmpty()) {
    body->add(init_body);
  } else {
    INT_FATAL(this, "Adding a body to a for loop that already has one");
  }
}


void BlockStmt::setBlkScope(SymScope* init_blkScope) {
  blkScope = init_blkScope;
}


Stmt* BlockStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Symboltable::pushScope(SCOPE_LOCAL);
  AList<Stmt>* body_copy = body->copyInternal(true, map);
  SymScope* block_scope = Symboltable::popScope();
  BlockStmt* block_copy = new BlockStmt(body_copy, block_scope);
  block_scope->setContext(block_copy);
  return block_copy;
}


void BlockStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = dynamic_cast<AList<Stmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in BlockStmt::replaceChild(old, new)");
  }
}


void BlockStmt::traverseStmt(Traversal* traversal) {
  SymScope* prevScope = NULL;
  if (blkScope) {
    prevScope = Symboltable::setCurrentScope(blkScope);
  }
  body->traverse(traversal, false);
  if (blkScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void BlockStmt::print(FILE* outfile) {
  fprintf(outfile, "{\n");
  if (body) {
    body->print(outfile, "\n");
  fprintf(outfile, "\n");
  }
  fprintf(outfile, "}");
}

void BlockStmt::codegen(FILE* outfile) {
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
                             AList<Stmt>* init_body) :
  BlockStmt(init_body),
  isWhileDo(init_whileDo),
  condition(init_cond)
{
  astType = STMT_WHILELOOP;
}


Stmt* WhileLoopStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new WhileLoopStmt(isWhileDo, condition->copyInternal(clone, map), body->copyInternal(clone, map));
}


void WhileLoopStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == condition) {
    condition = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == body) {
    body = dynamic_cast<AList<Stmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in WhileLoopStmt::replaceChild(old, new)");
  }
}


void WhileLoopStmt::traverseStmt(Traversal* traversal) {
  if (isWhileDo) {
    TRAVERSE(condition, traversal, false);
    TRAVERSE(body, traversal, false);
  } else {
    TRAVERSE(body, traversal, false);
    TRAVERSE(condition, traversal, false);
  }
}


void WhileLoopStmt::print(FILE* outfile) {

  if (isWhileDo) {
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ") ");
    body->print(outfile);
  } else {
    fprintf(outfile, "do ");
    body->print(outfile);
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ")");
  }
}


void WhileLoopStmt::codegen(FILE* outfile) {

  if (isWhileDo) {
    fprintf(outfile, "while (");
    condition->codegen(outfile);
    fprintf(outfile, ") {\n");
    body->codegen(outfile);
    fprintf(outfile, "\n}");
  } else { 
    fprintf(outfile, "do {\n");
    body->codegen(outfile);
    fprintf(outfile, "\n} while (");
    condition->codegen(outfile);
    fprintf(outfile, ");\n");
  }
}


ForLoopStmt::ForLoopStmt(bool init_forall,
                         AList<DefExpr>* init_indices,
                         Expr* init_domain,
                         AList<Stmt>* body)
  : BlockStmt(body),
    forall(init_forall),
    indices(init_indices),
    domain(init_domain),
    indexScope(NULL)
{
  astType = STMT_FORLOOP;
}


void ForLoopStmt::setIndexScope(SymScope* init_indexScope) {
  indexScope = init_indexScope;
}


Stmt* ForLoopStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  AList<DefExpr>* indices_copy = indices->copyInternal(true, map);
  Expr* domain_copy = domain->copyInternal(true, map);
  AList<Stmt>* body_copy = body->copyInternal(true, map);
  SymScope* index_scope = Symboltable::popScope();
  ForLoopStmt* for_loop_stmt_copy =
    new ForLoopStmt(forall, indices_copy, domain_copy, body_copy);
  for_loop_stmt_copy->setIndexScope(index_scope);
  return for_loop_stmt_copy;
}


void ForLoopStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == indices) {
    indices = dynamic_cast<AList<DefExpr>*>(new_ast);
  } else if (old_ast == domain) {
    domain = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == body) {
    body = dynamic_cast<AList<Stmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ForLoopStmt::replaceChild(old, new)");
  }
}


void ForLoopStmt::traverseStmt(Traversal* traversal) {
  SymScope* prevScope = NULL;

  TRAVERSE(domain, traversal, false);
  if (indexScope) {
    prevScope = Symboltable::setCurrentScope(indexScope);
  }
  indices->traverse(traversal, false);
  TRAVERSE(body, traversal, false);
  if (indexScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void ForLoopStmt::print(FILE* outfile) {
  fprintf(outfile, "for");
  if (forall) {
    fprintf(outfile, "all");
  }
  fprintf(outfile, " ");
  indices->print(outfile);
  fprintf(outfile, " in ");
  domain->print(outfile);
  fprintf(outfile, " ");
  body->print(outfile);
}


void ForLoopStmt::codegen(FILE* outfile) {
  DefExpr* indices_def = indices->first();

  if (SeqType* seqType = dynamic_cast<SeqType*>(domain->typeInfo())) {
    fprintf(outfile, "{\n");
    indices_def->sym->codegenDef(outfile);
    fprintf(outfile, "_FOR");
    if (forall) {
      fprintf(outfile, "ALL");
    }
    fprintf(outfile, "_S(");
    indices_def->sym->codegen(outfile);
    fprintf(outfile, ", ");
    domain->codegen(outfile);
    fprintf(outfile, ", ");
    fprintf(outfile, "%s", seqType->types.v[0]->cname);
    fprintf(outfile, ") {\n");
    body->codegen(outfile);
    fprintf(outfile, "\n");
    fprintf(outfile, "}\n");
    fprintf(outfile, "}\n");
    return;
  }

  DefExpr* aVar = indices->first();
  fprintf(outfile, "{\n");
  int rank = 0;
  IndexType* index_type = NULL;
  
  //RED -- added support for generating code for IndexTypes which are tuples
  //This extra branching is ugly, but the only way to get around a strong bias
  //towards integer indices
  index_type = dynamic_cast<IndexType*>(aVar->sym->type);
  if (index_type){
    //aVar->codegen(outfile);
    indices_def->sym->codegenDef(outfile);
    fprintf(outfile, "\n");
    TupleType* tt = dynamic_cast<TupleType*>(index_type->getType());
    if (tt){
      rank = tt->components.n;
    }
    for (int i=0; i<rank; i++) {
      fprintf(outfile, "_FOR");
      if (forall) {
        fprintf(outfile, "ALL");
      }
      fprintf(outfile, "(");
      //aVar->sym->codegen(outfile);
      aVar->sym->codegen(outfile);
      fprintf(outfile, "._field%d", i + 1);
      
      fprintf(outfile, ", ");
      domain->codegen(outfile);
      fprintf(outfile, ", %d) {\n", i);
    }
  } else {    
    // TODO: Unify with VarDefStmt?  Have parser insert one here?
    // is it a challenge that we may not know the domain exprs at that point?
    while (aVar) {
      aVar->sym->codegenDef(outfile);
      rank++;

      aVar = indices->next();
    }
    fprintf(outfile, "\n");
  
    aVar = indices->first();

    Tuple* tuple = dynamic_cast<Tuple*>(domain);

    SimpleSeqExpr* seq = (tuple)
      ? dynamic_cast<SimpleSeqExpr*>(tuple->exprs->first())
      : dynamic_cast<SimpleSeqExpr*>(domain);

    if (seq) {
      for (int i=0; i<rank; i++) {
        fprintf(outfile, "_FOR");
        if (forall) {
          fprintf(outfile, "ALL");
        }
        fprintf(outfile, "_DIM");
        fprintf(outfile, "(");
        aVar->sym->codegen(outfile);
        fprintf(outfile, ", ");
        seq->lo->codegen(outfile);
        fprintf(outfile, ", ");
        seq->hi->codegen(outfile);
        fprintf(outfile, ", ");
        seq->str->codegen(outfile);
        fprintf(outfile, ") {\n");

        aVar = indices->next();
        if (tuple) {
          seq = dynamic_cast<SimpleSeqExpr*>(tuple->exprs->next());
        }
      }
    }
    else {
      for (int i=0; i<rank; i++) {
        fprintf(outfile, "_FOR");
        if (forall) {
          fprintf(outfile, "ALL");
        }
        fprintf(outfile, "(");
        aVar->sym->codegen(outfile);
        fprintf(outfile, ", ");
        domain->codegen(outfile);
        fprintf(outfile, ", %d) {\n", i);

        aVar = indices->next();
      }
    }
  }
  
  body->codegen(outfile);
  fprintf(outfile, "\n");

  for (int i=0; i<rank; i++) {
    fprintf(outfile, "}\n");
  }
  
  fprintf(outfile, "}\n");
}


CondStmt::CondStmt(Expr*  init_condExpr,
                   BlockStmt* init_thenStmt, 
                   BlockStmt* init_elseStmt) :
  Stmt(STMT_COND),
  condExpr(init_condExpr),
  thenStmt(init_thenStmt),
  elseStmt(init_elseStmt)
{
  if (condExpr->next || condExpr->prev) {
    INT_FATAL(this, "condExpr is in a list");
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


Stmt* CondStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new CondStmt(condExpr->copyInternal(clone, map),
                      dynamic_cast<BlockStmt*>(thenStmt->copyInternal(clone, map)),
                      dynamic_cast<BlockStmt*>(elseStmt->copyInternal(clone, map)));
}


void CondStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == condExpr) {
    condExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == thenStmt) {
    thenStmt = dynamic_cast<BlockStmt*>(new_ast);
  } else if (old_ast == elseStmt) {
    elseStmt = dynamic_cast<BlockStmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CondStmt::replaceChild(old, new)");
  }
}


void CondStmt::verify(void) {
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

void CondStmt::codegen(FILE* outfile) {
  fprintf(outfile, "if (");
  condExpr->codegen(outfile);
  fprintf(outfile, ") {\n");
  thenStmt->codegen(outfile);
  fprintf(outfile, "\n}");
  if (elseStmt) {
    fprintf(outfile, " else {\n");
    elseStmt->codegen(outfile);
    fprintf(outfile, "\n}");
  }
}


WhenStmt::WhenStmt(AList<Expr>* init_caseExprs, BlockStmt* init_doStmt) :
  Stmt(STMT_WHEN),
  caseExprs(init_caseExprs),
  doStmt(init_doStmt)
{ }


Stmt* WhenStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new WhenStmt(caseExprs->copyInternal(clone, map),
                      dynamic_cast<BlockStmt*>(doStmt->copyInternal(clone, map)));
}


void WhenStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == caseExprs) {
    caseExprs = dynamic_cast<AList<Expr>*>(new_ast);
  } else if (old_ast == doStmt) {
    doStmt = dynamic_cast<BlockStmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in WhenStmt::replaceChild(old, new)");
  }
}


void WhenStmt::traverseStmt(Traversal* traversal) {
  caseExprs->traverse(traversal, false);
  TRAVERSE(doStmt, traversal, false);
}


void WhenStmt::print(FILE* outfile) {
  fprintf(outfile, "when ");
  caseExprs->print(outfile);
  fprintf(outfile, " do ");
  doStmt->print(outfile);
}


void WhenStmt::codegen(FILE* outfile) {
  INT_FATAL(this, "WhenStmt::codegen encountered");
}


SelectStmt::SelectStmt(Expr* init_caseExpr, AList<WhenStmt>* init_whenStmts) :
  Stmt(STMT_SELECT),
  caseExpr(init_caseExpr),
  whenStmts(init_whenStmts)
{ }


Stmt* SelectStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new SelectStmt(caseExpr->copyInternal(clone, map),
                        whenStmts->copyInternal(clone, map));
}


void SelectStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == caseExpr) {
    caseExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == whenStmts) {
    whenStmts = dynamic_cast<AList<WhenStmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in SelectStmt::replaceChild(old, new)");
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


void SelectStmt::codegen(FILE* outfile) {
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
    whenStmt->doStmt->codegen(outfile);
  }
}


LabelStmt::LabelStmt(LabelSymbol* init_label, BlockStmt* init_stmt) :
  Stmt(STMT_LABEL),
  label(init_label),
  stmt(init_stmt)
{ }


Stmt* LabelStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new LabelStmt(label, dynamic_cast<BlockStmt*>(stmt->copyInternal(clone, map)));
}


void LabelStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == stmt) {
    stmt = dynamic_cast<BlockStmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in LabelStmt::replaceChild(old, new)");
  }
}


void LabelStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE_DEF(label, traversal, false);
  TRAVERSE(stmt, traversal, false);
}


void LabelStmt::print(FILE* outfile) {
  fprintf(outfile, "label ");
  label->print(outfile);
  fprintf(outfile, " ");
  stmt->print(outfile);
}


void LabelStmt::codegen(FILE* outfile) {
  label->codegen(outfile);
  fprintf(outfile, ":;\n");
  stmt->codegen(outfile);
  label->codegen(outfile);
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
  label(new UnresolvedSymbol(init_label)),
  goto_type(init_goto_type)
{
}


GotoStmt::GotoStmt(gotoType init_goto_type, Symbol* init_label) :
  Stmt(STMT_GOTO),
  label(init_label),
  goto_type(init_goto_type)
{
}


Stmt* GotoStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


void GotoStmt::codegen(FILE* outfile) {
  fprintf(outfile, "goto ");
  label->codegen(outfile);
  if (goto_type == goto_break) {
    fprintf(outfile, "_post");
  }
  fprintf(outfile, ";\n");
}
