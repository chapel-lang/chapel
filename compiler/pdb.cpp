/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "if1.h"

PDB *pdb = 0;

PDB::PDB(IF1 *aif1) {
  if1 = aif1;
  pdb = this;
}

Sym *
PDB::find_global(char *s) {
  return if1_get_builtin(if1, s);
}

FA *
PDB::analyze(Fun *top) {
  FA *fa = new FA(this);
  if (fa->analyze(top) >= 0)
    return fa;
  return 0;
}
