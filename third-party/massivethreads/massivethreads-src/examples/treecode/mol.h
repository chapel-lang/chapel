/* mol.h */

#ifndef MOL_H
#define MOL_H

#include "defs.h"
#include "vector.h"

struct mol {
  int id;
  real mass;  // mass
  real chg;   // electric charge
  vector r;   // possition
  vector rv;  // velocity
  vector ra;  // acceleration

  mol(void);
  mol(int, real, real, const vector&, const vector&, const vector&);
  
  void step(real); // step forward with delta T
};

#endif /* MOL_H */
