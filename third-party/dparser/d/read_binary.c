#include "d.h"

static void 
read_chk(void* ptr, size_t size, size_t nmemb, FILE* fp, unsigned char **str) {
  if (fp) {
    if (fread(ptr, size, nmemb, fp) != nmemb) 
      d_fail("error reading binary tables\n");
  } else {
    memcpy(ptr, *str, size * nmemb);
    (*str) += size * nmemb;
  }
}

D_ParserTables *
read_binary_tables_internal(FILE *fp, unsigned char *str, 
			    D_ReductionCode spec_code, D_ReductionCode final_code) 
{
  BinaryTablesHead tables;
  int i;
  char *tables_buf, *strings_buf;

  read_chk(&tables, sizeof(BinaryTablesHead), 1, fp, &str);

  tables_buf = MALLOC(tables.tables_size + tables.strings_size);
  read_chk(tables_buf, sizeof(char), tables.tables_size, fp, &str);
  strings_buf = tables_buf + tables.tables_size;
  read_chk(strings_buf, sizeof(char), tables.strings_size, fp, &str);

  for (i=0; i<tables.n_relocs; i++) {
    int offset;
    void **ptr;
    int *intptr;
    read_chk((void*)&offset, sizeof(int), 1, fp, &str);
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
    read_chk((void*)&offset, sizeof(int), 1, fp, &str);
    *(void**)(tables_buf+offset) += (intptr_t)strings_buf;
  }
  if (fp)
    fclose(fp);

  return (D_ParserTables*)(tables_buf + tables.d_parser_tables_loc);
}

D_ParserTables *
read_binary_tables(char *file_name, 
		   D_ReductionCode spec_code, D_ReductionCode final_code) {
  FILE *fp = fopen(file_name, "rb");
  if (!fp)
    d_fail("error opening tables %s\n", file_name);
  return read_binary_tables_internal(fp, 0, spec_code, final_code);
}

D_ParserTables *
read_binary_tables_from_file(FILE *fp, 
		   D_ReductionCode spec_code, D_ReductionCode final_code) {
  return read_binary_tables_internal(fp, 0, spec_code, final_code);
}

D_ParserTables *
read_binary_tables_from_string(unsigned char *str, D_ReductionCode spec_code, D_ReductionCode final_code) {
  return read_binary_tables_internal(0, str, spec_code, final_code);
}
