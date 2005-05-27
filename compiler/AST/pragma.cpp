#include <string.h>
#include <typeinfo>
#include "link.h"
#include "pragma.h"
#include "map.h"


Pragma::Pragma(char *init_str) :
  BaseAST(PRAGMA)
{
  if (init_str) {
    str = dupstr(init_str);
  }
}

