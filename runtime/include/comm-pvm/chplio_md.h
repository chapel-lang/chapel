#ifndef CHPLIO_MD_H
#define CHPLIO_MD_H

#include <stdio.h>

#undef printf
int chpl_pvm_fprintf(FILE* outfile, const char* format, ...)
  __attribute__((format(printf, 2, 3)));
int chpl_pvm_printf(const char* format, ...)
  __attribute__((format(printf, 1, 2)));


/* This is a little gimmick that is necessary due to the
   __attribute__s in this header and others */
#define printf PRINTF_DEF
#define PRINTF_DEF chpl_pvm_printf
#define fprintf chpl_pvm_fprintf

#endif
