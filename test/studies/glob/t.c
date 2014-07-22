#include "globhelp.h"

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
  return 0;
}
