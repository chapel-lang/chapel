{
#include "dparse_tables.h"

void mywhite(struct D_Parser *p, d_loc_t *loc, void **p_globals) {
  while (*loc->s == ' ') loc->s++;
}

}

${declare whitespace mywhite}
S: '\n' '\t' '\n';
