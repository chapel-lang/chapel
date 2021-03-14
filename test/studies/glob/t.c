/*#include "globhelp.h"*/
#include <glob.h>
#include <stdio.h>
#include <wordexp.h>

#define prt(nm) printf("%s = 0x%d\n", #nm, nm)

int main() {
  prt(GLOB_ERR);
  prt(GLOB_MARK);
  prt(GLOB_NOSORT);
  prt(GLOB_DOOFFS);
  prt(GLOB_NOCHECK);
  prt(GLOB_APPEND);
  prt(GLOB_NOESCAPE);
  prt(WRDE_APPEND);
  prt(WRDE_DOOFFS);
  prt(WRDE_NOCMD);
  prt(WRDE_REUSE);

  int i;
  glob_t out;
  int glb = glob("*.[c|h]",0, NULL, &out);

  for (i = 0; i < out.gl_pathc; i++) {
    printf("%s\n", out.gl_pathv[i]);
  }
  return 0;
}
