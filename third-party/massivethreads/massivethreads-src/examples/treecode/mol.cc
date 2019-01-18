/* mol.cc */

#include "mol.h"

mol::mol(void)
{
  id = 0; mass = 0; chg = 0; 
  r.clr(); rv.clr(); ra.clr();
}

mol::mol(int _id, real _m, real _c, const vector& _r, const vector& _rv,
  const vector& _ra)
{
  id = _id; mass = _m; chg = _c; r = _r; rv = _rv, ra = _ra;
}

void mol::step(real dt)
{
  rv += ra * dt;
  r = rv * dt;
}
