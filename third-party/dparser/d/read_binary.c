#include "d.h"

static void 
read_chk(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  if (fread(ptr, size, nmemb, stream) != nmemb) 
    d_fail("error reading binary tables\n");
}

D_ParserTables *
read_binary_tables(char *file_name, D_ReductionCode spec_code, D_ReductionCode final_code) {
  FILE *f;
  BinaryTablesHead tables;
  int i;
  char *tables_buf, *strings_buf;

  f = fopen(file_name, "rb");
  if (!f)
    d_fail("error opening tables %s\n", file_name);

  read_chk(&tables, sizeof(BinaryTablesHead), 1, f);

  tables_buf = MALLOC(tables.tables_size + tables.strings_size);
  read_chk(tables_buf, sizeof(char), tables.tables_size, f);
  strings_buf = tables_buf + tables.tables_size;
  read_chk(strings_buf, sizeof(char), tables.strings_size, f);

  for (i=0; i<tables.n_relocs; i++) {
    int offset;
    void **ptr;
    int *intptr;
    read_chk((void*)&offset, sizeof(int), 1, f);
    intptr = (int*)(tables_buf+offset);
    ptr = (void**)intptr;
    if (*intptr == -1) {
      *ptr = (void*)0;
    } else if (*intptr == -2) {
      *ptr = (void*)spec_code;
    } else if (*intptr == -3) {
      *ptr = (void*)final_code;
    } else {
      *ptr += (intptr_t)tables_buf;
    }
  }
  for (i=0; i<tables.n_strings; i++) {
    int offset;
    read_chk((void*)&offset, sizeof(int), 1, f);
    *(void**)(tables_buf+offset) += (intptr_t)strings_buf;
  }
  fclose(f);

  return (D_ParserTables*)(tables_buf + tables.d_parser_tables_loc);
}
