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

// types contains the types of the actuals
// names contains the name if it is a named argument, otherwise NULL
// e.g.  foo(arg1=12, "hi");
//  types = integer, string
//  names = arg1, NULL
enum resolve_call_error_type {
  CALL_NO_ERROR,
  CALL_PARTIAL,
  CALL_AMBIGUOUS,
  CALL_UNKNOWN
};
extern resolve_call_error_type resolve_call_error;
FnSymbol*
resolve_call(BaseAST* ast,
             char *name,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names,
             PartialTag partialTag);

// collect FnSymbols in the AST and return them in vectors
void collect_functions(Vec<FnSymbol*>* functions);

// collect Stmts and Exprs in the AST and return them in vectors
void collect_asts(Vec<BaseAST*>* asts);
void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast);
void collect_asts_postorder(Vec<BaseAST*>* asts);
void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast);

// utility routines for clearing and resetting lineno and filename
void clear_file_info(BaseAST* baseAST);
void reset_file_info(BaseAST* baseAST, int lineno, char* filename);

// compute call sites FnSymbol::calls
void compute_call_sites();

// clear type and retTypes
void clear_type_info(BaseAST* baseAST);

// update symbols in ast with map
void update_symbols(BaseAST* ast, ASTMap* map);

#endif
