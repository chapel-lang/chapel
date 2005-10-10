#include "baseAST.h"
#include "symbol.h"

// collect things in the AST and return them in vectors
void collect_functions(Vec<FnSymbol*>* functions);

// utility routines for clearing and resetting lineno and filename
void clear_file_info(BaseAST* baseAST);
void reset_file_info(BaseAST* baseAST, int lineno, char* filename);

// compute call sites FnSymbol::calls
void compute_call_sites();
