/* vector.h */

#ifndef VECTOR_H
#define VECTOR_H

#include "defs.h"

struct vector {
  real x;
  real y;
  real z;
  
  vector(void);
  vector(real, real, real);

  void clr(void);
  void set(real, real, real);
  void add(const vector&);
  void sub(const vector&);
  void mul(const vector&);
  void div(const vector&);

  void operator=(const vector&);
};

vector operator+(const vector&, const vector&);
vector operator-(const vector&, const vector&);
vector operator*(const vector&, const vector&);
vector operator/(const vector&, const vector&);
void operator+=(vector&, const vector&);
vector operator*(const vector&, real);

#endif /* VECTOR_H */
