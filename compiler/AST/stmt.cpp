#include <string.h>
#include <typeinfo>
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "../symtab/symlink.h"
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


Stmt* Stmt::copyList(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone, Vec<BaseAST*>* update_list) {
  if (map == NULL) {
    map = new Map<BaseAST*,BaseAST*>();
  }
  Stmt* newStmtList = copyListInternal(clone, map, analysis_clone);
  if (update_list) {
    for (int j = 0; j < update_list->n; j++) {
      for (int i = 0; i < map->n; i++) {
        if (update_list->v[j] == map->v[i].key) {
          update_list->v[j] = map->v[i].value;
        }
      }
    }
  }
  TRAVERSE_LS(newStmtList, new UpdateSymbols(map), true);
  return newStmtList;
}


Stmt* Stmt::copy(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone, Vec<BaseAST*>* update_list) {
  if (map == NULL) {
    map = new Map<BaseAST*,BaseAST*>();
  }
  Stmt* new_stmt = copyInternal(clone, map, analysis_clone);
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


Stmt* Stmt::copyListInternal(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Stmt* newStmtList = NULL;
  Stmt* oldStmt = this;

  while (oldStmt) {
    newStmtList = appendLink(newStmtList, 
                             oldStmt->copyInternal(clone, map, analysis_clone));
    
    oldStmt = nextLink(Stmt, oldStmt);
  }

  return newStmtList;
}


Stmt* Stmt::copyInternal(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  if (!this) {
    return this;
  }

  Stmt* new_stmt = copyStmt(clone, map, analysis_clone);

  new_stmt->lineno = lineno;
  new_stmt->filename = filename;
  //new_stmt->pragmas = pragmas;
  if (!RunAnalysis::isRunning) {
    new_stmt->ainfo = ainfo;
  }
  if (analysis_clone) {
    analysis_clone->clone(this, new_stmt);
  }
  if (map) {
    map->put(this, new_stmt);
  }
  return new_stmt;
}


Stmt* Stmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  INT_FATAL(this, "copy not implemented for Stmt subclass");
  return NULL;
}


void Stmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to Stmt::replaceChild(old, new)");
}


void Stmt::traverse(Traversal* traversal, bool atTop) {
  if (traversal->processTop || !atTop) {
    traversal->preProcessStmt(this);
  }
  if (atTop || traversal->exploreChildStmts) {
    traverseStmt(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessStmt(this);
  }
}


void Stmt::traverseDef(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Attempt to traverse the definition of a statement");
}


void Stmt::traverseStmt(Traversal* traversal) {
}


static void call_replace_child(Stmt* old_stmt, Stmt* new_stmt) {
  if (old_stmt->parentStmt) {
    old_stmt->parentStmt->replaceChild(old_stmt, new_stmt);
  } else {
    old_stmt->parentSymbol->replaceChild(old_stmt, new_stmt);
  }
}


Stmt* Stmt::head(void) {
  ILink* first = this;
  while (first->prev) {
    first = first->prev;
  }
  Stmt* head = dynamic_cast<Stmt*>(first);
  if (!head) {
    INT_FATAL(this, "Ill-formed statement list found in Stmt::head");
  }
  return head;
}


Stmt* Stmt::tail(void) {
  ILink* last = this;
  while (last->next) {
    last = last->next;
  }
  Stmt* tail = dynamic_cast<Stmt*>(last);
  if (!last) {
    INT_FATAL(this, "Ill-formed statement list found in Stmt::last");
  }
  return tail;
}


void Stmt::replace(Stmt* new_stmt) {
  Stmt* first = new_stmt->head();
  Stmt* last = new_stmt->tail();

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal replace, new_stmt is not head of list");
  }

  last->next = next;
  if (next) {
    next->prev = last;
  }
  first->prev = prev;
  if (!prev) {
    call_replace_child(this, first);
  } else {
    prev->next = first;
  }

  call_fixup(this);
}


void Stmt::insertBefore(Stmt* new_stmt) {
  Stmt* first = new_stmt->head();
  Stmt* last = new_stmt->tail();

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal insertBefore, new_stmt is not head of list");
  }

  if (!prev) {
    call_replace_child(this, first);
  } else {
    prev->next = first;
  }
  first->prev = prev;
  prev = last;
  last->next = this;

  call_fixup(this);
}


void Stmt::insertAfter(Stmt* new_stmt) {
  Stmt* first = new_stmt->head();
  Stmt* last = new_stmt->tail();

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal insertAfter, new_stmt is not head of list");
  }

  if (next) {
    next->prev = last;
  }
  last->next = next;
  next = first;
  first->prev = this;

  call_fixup(this);
}


void Stmt::append(ILink* new_stmt) {
  if (!new_stmt) {
    return;
  }

  Stmt* first = dynamic_cast<Stmt*>(new_stmt->head());

  if (first != new_stmt) {
    INT_FATAL(this, "Illegal append, new_stmt is not head of list");
  }

  Stmt* append_point = dynamic_cast<Stmt*>(this->tail());

  append_point->next = first;
  first->prev = append_point;
}


Stmt* Stmt::extract(void) {
  Stmt* next_stmt = dynamic_cast<Stmt*>(next);
  if (prev) {
    prev->next = next;
  } else {
    call_replace_child(this, next_stmt);
  }
  if (next_stmt) {
    next->prev = prev;
  }
  return this;
}


NoOpStmt::NoOpStmt(void) :
  Stmt(STMT_NOOP)
{}


Stmt* NoOpStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
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
  defExprls(init_defExprls)
{ }


Stmt* DefStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new DefStmt(dynamic_cast<DefExpr*>(defExprls->copy(clone, map, analysis_clone)));
}


void DefStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defExprls) {
    defExprls = dynamic_cast<DefExpr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in DefStmt::replaceChild(old, new)");
  }
}


void DefStmt::traverseStmt(Traversal* traversal) {
  TRAVERSE_LS(defExprls, traversal, false);
}


void DefStmt::print(FILE* outfile) {
  defExprls->printList(outfile, "\n");
}


void DefStmt::codegen(FILE* outfile) { /* Noop */ }


VarSymbol* DefStmt::varDef() {
  return dynamic_cast<VarSymbol*>(defExprls->sym);
}


FnSymbol* DefStmt::fnDef() {
  return dynamic_cast<FnSymbol*>(defExprls->sym);
}


TypeSymbol* DefStmt::typeDef() {
  return dynamic_cast<TypeSymbol*>(defExprls->sym);
}


ModuleSymbol* DefStmt::moduleDef() {
  return dynamic_cast<ModuleSymbol*>(defExprls->sym);
}


Vec<VarSymbol*>* DefStmt::varDefSet() {
  Vec<VarSymbol*>* var_set = new Vec<VarSymbol*>();
  DefExpr* def_expr = defExprls;
  while (def_expr) {
    VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym);
    while (var) {
      var_set->set_add(var);
      var = nextLink(VarSymbol, var);
    }
    def_expr = nextLink(DefExpr, def_expr);
  }
  return var_set;
}


ExprStmt::ExprStmt(Expr* initExpr) :
  Stmt(STMT_EXPR),
  expr(initExpr) 
{ }


Stmt* ExprStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new ExprStmt(expr->copyInternal(clone, map, analysis_clone));
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


Stmt* ReturnStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new ReturnStmt(expr->copyInternal(clone, map, analysis_clone));
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
    expr->codegen(outfile);
  }
  fprintf(outfile, ";");
}


WithStmt::WithStmt(Expr* initExpr) :
  ExprStmt(initExpr)
{
  astType = STMT_WITH;
}


Stmt* WithStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new WithStmt(expr->copyInternal(clone, map, analysis_clone));
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


Stmt* UseStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new UseStmt(expr->copyInternal(clone, map, analysis_clone));
}


void UseStmt::print(FILE* outfile) {
  fprintf(outfile, "use ");
  expr->print(outfile);
  fprintf(outfile, "\n");
}


void UseStmt::codegen(FILE* outfile) {
  INT_FATAL(this, "Use statement encountered in codegen()");
}


BlockStmt::BlockStmt(Stmt* init_body) :
  Stmt(STMT_BLOCK),
  body(init_body),
  blkScope(NULL)
{ }


void BlockStmt::addBody(Stmt* init_body) {
  if (!body) {
    body = init_body;
  } else {
    INT_FATAL(this, "Adding a body to a for loop that already has one");
  }
}


void BlockStmt::setBlkScope(SymScope* init_blkScope) {
  blkScope = init_blkScope;
}


Stmt* BlockStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Symboltable::pushScope(SCOPE_LOCAL);
  Stmt* body_copy = body->copyListInternal(true, map, analysis_clone);
  SymScope* block_scope = Symboltable::popScope();
  BlockStmt* block_copy = new BlockStmt(body_copy);
  block_copy->setBlkScope(block_scope);
  block_scope->setContext(block_copy);
  return block_copy;
}


void BlockStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = dynamic_cast<Stmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in BlockStmt::replaceChild(old, new)");
  }
}


void BlockStmt::traverseStmt(Traversal* traversal) {
  SymScope* prevScope = NULL;
  if (blkScope) {
    prevScope = Symboltable::setCurrentScope(blkScope);
  }
  TRAVERSE_LS(body, traversal, false);
  if (blkScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void BlockStmt::print(FILE* outfile) {
  fprintf(outfile, "{\n");
  if (body) {
    body->printList(outfile, "\n");
  fprintf(outfile, "\n");
  }
  fprintf(outfile, "}");
}

void BlockStmt::codegen(FILE* outfile) {
  fprintf(outfile, "{\n");
  if (blkScope) {
    blkScope->codegen(outfile, "\n");
  }
  if (body) body->codegenList(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "}");
}


WhileLoopStmt::WhileLoopStmt(bool init_whileDo, 
                             Expr* init_cond, 
                             Stmt* init_body) 
  : BlockStmt(init_body), 
    isWhileDo(init_whileDo), 
    condition(init_cond) 
{
  astType = STMT_WHILELOOP;
}


Stmt* WhileLoopStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new WhileLoopStmt(isWhileDo, condition->copyInternal(clone, map, analysis_clone), body->copyInternal(clone, map, analysis_clone));
}


void WhileLoopStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == condition) {
    condition = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == body) {
    body = dynamic_cast<Stmt*>(new_ast);
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
                         Expr* init_indices,
                         Expr* init_domain,
                         Stmt* body)
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


Stmt* ForLoopStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  Expr* indices_copy = indices->copyListInternal(true, map, analysis_clone);
  Expr* domain_copy = domain->copyInternal(true, map, analysis_clone);
  Stmt* body_copy = body->copyInternal(true, map, analysis_clone);
  SymScope* index_scope = Symboltable::popScope();
  ForLoopStmt* for_loop_stmt_copy =
    new ForLoopStmt(forall, indices_copy, domain_copy, body_copy);
  for_loop_stmt_copy->setIndexScope(index_scope);
  return for_loop_stmt_copy;
}


void ForLoopStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == indices) {
    indices = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == domain) {
    domain = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == body) {
    body = dynamic_cast<Stmt*>(new_ast);
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
  TRAVERSE_LS(indices, traversal, false);
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
  indices->printList(outfile);
  fprintf(outfile, " in ");
  domain->print(outfile);
  fprintf(outfile, " ");
  body->print(outfile);
}


void ForLoopStmt::codegen(FILE* outfile) {
  DefExpr* indices_def = dynamic_cast<DefExpr*>(indices);

  if (indices && !indices_def) {
    INT_FATAL(this, "Indices in ForLoopStmt not defined in DefExpr");
  }

  if (dynamic_cast<SeqType*>(domain->typeInfo())) {
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
    domain->typeInfo()->codegen(outfile);
    fprintf(outfile, ") {\n");
    body->codegen(outfile);
    fprintf(outfile, "\n");
    fprintf(outfile, "}\n");
    fprintf(outfile, "}\n");
    return;
  }

  DefExpr* aVar = indices_def;
  fprintf(outfile, "{\n");
  int rank = 0;

  // TODO: Unify with VarDefStmt?  Have parser insert one here?
  // is it a challenge that we may not know the domain exprs at that point?
  while (aVar) {
    aVar->sym->codegenDef(outfile);
    rank++;

    aVar = nextLink(DefExpr, aVar);
  }
  fprintf(outfile, "\n");
  
  aVar = indices_def;

  Tuple* tuple = dynamic_cast<Tuple*>(domain);

  SimpleSeqExpr* seq = (tuple)
    ? dynamic_cast<SimpleSeqExpr*>(tuple->exprs)
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

      aVar = nextLink(DefExpr, aVar);
      seq = nextLink(SimpleSeqExpr, seq);
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

      aVar = nextLink(DefExpr, aVar);
    }
  }

  body->codegen(outfile);
  fprintf(outfile, "\n");

  for (int i=0; i<rank; i++) {
    fprintf(outfile, "}\n");
  }
  
  fprintf(outfile, "}\n");
}


CondStmt::CondStmt(Expr*  init_condExpr, Stmt* init_thenStmt, 
                   Stmt* init_elseStmt) :
  Stmt(STMT_COND),
  condExpr(init_condExpr),
  thenStmt(init_thenStmt),
  elseStmt(init_elseStmt)
{ }


void CondStmt::addElseStmt(Stmt* init_elseStmt) {
  if (elseStmt) {
    INT_FATAL(this, "overwriting existing else Stmt");
  }
  elseStmt = init_elseStmt;
}


Stmt* CondStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new CondStmt(condExpr->copyInternal(clone, map, analysis_clone), thenStmt->copyInternal(clone, map, analysis_clone), elseStmt->copyInternal(clone, map, analysis_clone));
}


void CondStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == condExpr) {
    condExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == thenStmt) {
    thenStmt = dynamic_cast<Stmt*>(new_ast);
  } else if (old_ast == elseStmt) {
    elseStmt = dynamic_cast<Stmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CondStmt::replaceChild(old, new)");
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



LabelStmt::LabelStmt(LabelSymbol* init_label, Stmt* init_stmt) :
  Stmt(STMT_LABEL),
  label(init_label),
  stmt(init_stmt)
{ }


Stmt* LabelStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new LabelStmt(label, stmt->copyInternal(clone, map, analysis_clone));
}


void LabelStmt::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == stmt) {
    stmt = dynamic_cast<Stmt*>(new_ast);
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


Stmt* GotoStmt::copyStmt(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
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
