#ifndef _ASTUTIL_H_
#define _ASTUTIL_H_

#include "baseAST.h"
#include "alist.h"

class Type;
class FnSymbol;
class VarSymbol;
class ArgSymbol;
class CallExpr;
class Expr;

void normalize(BaseAST* ast);

// collect Stmts and Exprs in the AST and return them in vectors
void collect_asts(Vec<BaseAST*>* asts);
void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_asts_postorder(Vec<BaseAST*>* asts);
void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_top_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_stmts(Vec<Expr*>* exprs, Expr* expr);

// utility routines for clearing and resetting lineno and filename
void clear_file_info(BaseAST* baseAST);
void reset_file_info(BaseAST* baseAST, int lineno, const char* filename);

// compute call sites FnSymbol::calls
void compute_call_sites();

//
// compute Symbol::uses and Symbol::defs
//
void compute_sym_uses();
// builds the vectors for every variable/argument in the entire
// program

void compute_sym_uses(FnSymbol* fn);
// builds the vectors for every variable/argument in 'fn' and looks
// for uses and defs only in 'fn'

void compute_sym_uses(Vec<Symbol*>& set);
// builds the vectors for every variable/argument in 'set' and looks
// for uses and defs in the entire program; the vectors should be
// cleared before calling this variant

void compute_sym_uses(Vec<Symbol*>& set, Vec<BaseAST*>& asts);
// builds the vectors for every variable/argument in 'set' and looks
// for uses and defs only in 'asts'; the vectors should be cleared
// before calling this variant

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
