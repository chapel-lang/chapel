// utility routines for clearing and resetting lineno and filename
void clear_file_info(BaseAST* baseAST);
void reset_file_info(BaseAST* baseAST, int lineno, char* filename);

// compute call sites FnSymbol::calls
void compute_call_sites();
