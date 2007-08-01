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
class Expr;

void scopeResolve(Symbol* sym);
void normalize(BaseAST* ast);

// collect FnSymbols in the AST and return them in vectors
void collect_functions(Vec<FnSymbol*>* functions);

// collect Stmts and Exprs in the AST and return them in vectors
void collect_asts(Vec<BaseAST*>* asts);
void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_asts(Vec<BaseAST*>* asts, AList* ast);
void collect_asts_postorder(Vec<BaseAST*>* asts);
void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_asts_postorder(Vec<BaseAST*>* asts, AList* ast);
void collect_top_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_top_asts(Vec<BaseAST*>* asts, AList* ast);
void collect_stmts(Vec<Expr*>* exprs, Expr* expr);

// utility routines for clearing and resetting lineno and filename
void clear_file_info(BaseAST* baseAST);
void reset_file_info(BaseAST* baseAST, int lineno, const char* filename);
void reset_file_info(AList* baseAST, int lineno, const char* filename);

// compute call sites FnSymbol::calls
void compute_call_sites();

// compute all SymExprs for a Symbol and store in Symbol::uses
void compute_sym_uses(BaseAST* ast = NULL);

// clear type and retTypes
void clear_type_info(BaseAST* baseAST);

// update symbols in ast with map
void update_symbols(BaseAST* ast, ASTMap* map);

// replaces Fixup
void remove_help(BaseAST* ast);
void parent_insert_help(BaseAST* parent, Expr* ast);
void sibling_insert_help(BaseAST* sibling, BaseAST* ast);
void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Symbol* parentSymbol,
                 SymScope* parentScope);

ArgSymbol* actual_to_formal( Expr *a);

#endif
