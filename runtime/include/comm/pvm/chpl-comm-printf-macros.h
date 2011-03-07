#ifndef _comm_printf_macros_h_
#define _comm_printf_macros_h_

#ifndef LAUNCHER

#include <stdio.h>
#include <stdarg.h>

/* undef printf in order to permit it to be used in the following
   attribute annotations */
#undef printf

int chpl_pvm_fprintf(FILE* outfile, const char* format, ...)
  __attribute__((format(printf, 2, 3)));
int chpl_pvm_printf(const char* format, ...)
  __attribute__((format(printf, 1, 2)));
int chpl_pvm_vfprintf(FILE* stream, const char* format, va_list ap)
  __attribute__((format(printf, 2, 0)));

/* This is a little gimmick that is necessary due to the
   __attribute__s in this header and others */
#define printf PRINTF_DEF
#define PRINTF_DEF chpl_pvm_printf

#define fprintf chpl_pvm_fprintf
#define vfprintf chpl_pvm_vfprintf

#else

#define PRINTF_DEF printf

#endif

#endif
