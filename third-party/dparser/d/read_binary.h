/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/

D_ParserTables *read_binary_tables(char *file_name, D_ReductionCode spec_code, D_ReductionCode final_code);
D_ParserTables *read_binary_tables_from_file(FILE *fp, D_ReductionCode spec_code, D_ReductionCode final_code);
D_ParserTables *read_binary_tables_from_string(unsigned char *buf, D_ReductionCode spec_code, D_ReductionCode final_code);

typedef struct BinaryTablesHead {
  int n_relocs;
  int n_strings;
  int d_parser_tables_loc;
  int tables_size;
  int strings_size;
} BinaryTablesHead;
