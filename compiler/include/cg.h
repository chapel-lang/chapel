/* -*-Mode: c++;-*-
*/

#ifndef _cg_h_
#define _cg_h_

#include "fa.h"

void cg_print_c(FILE *fp, FA *fa, Fun *main);
void cg_write_c(FA *fa, Fun *main, char *filename);
int cg_compile(char *filename);

#endif
