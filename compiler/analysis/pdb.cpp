#include "geysa.h"
#include "if1.h"
#include "fa.h"
#include "pdb.h"
#include "fun.h"

PDB *pdb = 0;

PDB::PDB(IF1 *aif1) {
  if1 = aif1;
  pdb = this;
  fa = new FA(this);
}

Sym *
PDB::find_global(char *s) {
  return if1_get_builtin(if1, s);
}

void 
PDB::add(Fun *f) { 
  f->id = funs.n; 
  funs.add(f); 
}

