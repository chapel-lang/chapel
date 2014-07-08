/* treecode.h */

#ifndef TREECODE_H
#define TREECODE_H

#include "defs.h"
#include "utils.h"
#include "vector.h"
#include "mol.h"
#include "tree.h"
#include "opts.h"

// Simulation instance
struct simulation {
  int mol;   // number of molecules
  int steps; // number of simulation steps
  struct mol *mols;
};

void init(arguments &);

#endif /* TREECODE_H */
