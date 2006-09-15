#ifndef _ASTUTIL_H_
#define _ASTUTIL_H_

#include "baseAST.h"
#include "chplenum.h"
#include "alist.h"

class Type;
class FnSymbol;
class VarSymbol;
class ArgSymbol;
class CallExpr;

void cleanup(Symbol* sym);
void scopeResolve(Symbol* sym);
void normalize(BaseAST* ast);
void build(FnSymbol* fn);

// collect FnSymbols in the AST and return them in vectors
void collect_functions(Vec<FnSymbol*>* functions);

// collect Stmts and Exprs in the AST and return them in vectors
void collect_asts(Vec<BaseAST*>* asts);
void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_asts(Vec<BaseAST*>* asts, AList<Stmt>* ast);
void collect_asts_postorder(Vec<BaseAST*>* asts);
void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_asts_postorder(Vec<BaseAST*>* asts, AList<Stmt>* ast);
void collect_top_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_top_asts(Vec<BaseAST*>* asts, AList<Stmt>* ast);

// utility routines for clearing and resetting lineno and filename
void clear_file_info(BaseAST* baseAST);
void reset_file_info(BaseAST* baseAST, int lineno, char* filename);
void reset_file_info(AList<Stmt>* baseAST, int lineno, char* filename);

// compute call sites FnSymbol::calls
void compute_call_sites();

// compute all SymExprs for a Symbol and store in Symbol::uses
void compute_sym_uses(BaseAST* ast = NULL);

// clear type and retTypes
void clear_type_info(BaseAST* baseAST);

// update symbols in ast with map
void update_symbols(BaseAST* ast, ASTMap* map);

// removes NamedExpr from AST
void remove_named_exprs();

void remove_static_actuals();
void remove_static_formals();

// replaces Fixup
BaseAST* parent_ast_wrap(BaseAST* parent, BaseAST* ast);
BaseAST* sibling_ast_wrap(BaseAST* sibling, BaseAST* ast);
void remove_help(BaseAST* ast);
void parent_insert_help(BaseAST* parent, BaseAST* ast);
void sibling_insert_help(BaseAST* sibling, BaseAST* ast);
void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Stmt* parentStmt,
                 Symbol* parentSymbol,
                 SymScope* parentScope);

ArgSymbol* actual_to_formal( Expr *a);

#endif
