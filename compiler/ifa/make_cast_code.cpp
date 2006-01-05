#include <stdio.h>
#define EXTERN
#define EXTERN_INIT(_x) = _x
#include "num.h"


/*
To be used to build the body of the function:

void coerce_immediate(from, to);

Compile with:

g++ -I../include make_cast_code.cpp 
*/

int
main() {
  FILE *fp = fopen("cast_code.cpp", "w");
  fprintf(fp, "switch (to->const_kind) {\n");
  fprintf(fp, "default: assert(!\"case\"); break;\n");
  for (int tt = 1; tt < 4; tt++) {
    fprintf(fp, "case %d: \n", tt);
    fprintf(fp, "switch (to->num_index) {\n");
    for (int tn = 0; tn < 8; tn++) {
      fprintf(fp, "case %d:\n", tn);
      if (!num_kind_string[tt][tn]) {
        fprintf(fp, "assert(!\"case\"); break;\n");
      } else {
        fprintf(fp, "switch (from->const_kind) {\n");
        fprintf(fp, "default: assert(!\"case\"); break;\n");
        for (int st = 1; st < 4; st++) {
          fprintf(fp, "case %d:\n", st);
          fprintf(fp, "switch (from->num_index) {\n");
          for (int sn = 0; sn < 8; sn++) {
            fprintf(fp, "case %d:\n", sn);
            if (!num_kind_string[st][sn]) {
              fprintf(fp, "assert(!\"case\"); break;\n");
            } else {
              fprintf(fp, "to->v_%s = (%s)from->v_%s; break;\n",
                      num_kind_string[tt][tn],
                      num_kind_string[tt][tn],
                      num_kind_string[st][sn]);
            }
          }
          fprintf(fp, "} break;\n");
        }
        fprintf(fp, "} break;\n");
      }
    }
    fprintf(fp, "} break;\n");
  }
  fprintf(fp, "}\n");
}


    
