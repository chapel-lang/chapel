#ifndef _callbacks_H_
#define _callbacks_H_

#include "vec.h"
#include "map.h"

class Sym;
class Match;
class Fun;
class ATypeViolation;

class Callbacks : public gc {
public:
  virtual void finalize_functions() {}
  virtual Sym *make_LUB_type(Sym *s) { return s; }
  virtual Sym *instantiate(Sym *, Map<Sym *, Sym *> &substitutions) { return 0; }
  virtual Sym *formal_to_generic(Sym *s) { return s; }
  virtual Sym *new_Sym(char *name) = 0;
  virtual Fun* order_wrapper(Match *) { return 0; }
  virtual Fun* coercion_wrapper(Match *) { return 0; }
  virtual Fun* default_wrapper(Match *) { return 0; }
  virtual Fun* instantiate_generic(Match *) { return 0; }
  virtual void report_analysis_errors(Vec<ATypeViolation*> &type_violations) { }
};

#endif
