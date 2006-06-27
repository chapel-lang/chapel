#ifndef _ASTUTIL_H_
#define _ASTUTIL_H_

#include "baseAST.h"
#include "chplenum.h"

class Type;
class FnSymbol;
class VarSymbol;
class CallExpr;

void cleanup(BaseAST* ast);
void scopeResolve(BaseAST* ast);
void normalize(BaseAST* ast);
void build(BaseAST* ast);

// types contains the types of the actuals
// names contains the name if it is a named argument, otherwise NULL
// e.g.  foo(arg1=12, "hi");
//  types = int, string
//  names = arg1, NULL
enum resolve_call_error_type {
  CALL_NO_ERROR,
  CALL_PARTIAL,
  CALL_AMBIGUOUS,
  CALL_UNKNOWN
};
extern resolve_call_error_type resolve_call_error;
extern Vec<FnSymbol*> resolve_call_error_candidates;
FnSymbol*
resolve_call(BaseAST* ast,
             char *name,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names,
             PartialTag partialTag,
             FnSymbol *fnSymbol = NULL);
void resolve_type_expr(BaseAST* base);

// collect FnSymbols in the AST and return them in vectors
void collect_functions(Vec<FnSymbol*>* functions);

// collect Stmts and Exprs in the AST and return them in vectors
void collect_asts(Vec<BaseAST*>* asts);
void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_asts_postorder(Vec<BaseAST*>* asts);
void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_top_asts(Vec<BaseAST*>* asts, BaseAST* ast);

// utility routines for clearing and resetting lineno and filename
void clear_file_info(BaseAST* baseAST);
void reset_file_info(BaseAST* baseAST, int lineno, char* filename);

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

void simplify_nested_moves();

void remove_static_actuals();
void remove_static_formals();

// replaces Fixup
void remove_help(BaseAST* ast);
void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Stmt* parentStmt,
                 Symbol* parentSymbol,
                 SymScope* parentScope);

#endif
