#ifndef _callbacks_H_
#define _callbacks_H_

class Sym;
class Match;
class Fun;

class Callbacks : public gc {
public:
  virtual void compute_visible_functions() {}
  virtual void new_LUB_type(Sym *) = 0;
  virtual Sym *instantiate(Sym *, Map<Sym *, Sym *> &substitutions) { return 0; }
  virtual Sym *new_Sym(char *name) = 0;
  virtual Fun* order_wrapper(Match *) { return 0; }
  virtual Fun* coercion_wrapper(Match *) { return 0; }
  virtual Fun* default_wrapper(Match *) { return 0; }
  virtual Fun* instantiate_generic(Match *) { return 0; }
};

#endif
