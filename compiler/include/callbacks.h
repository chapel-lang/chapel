#ifndef _callbacks_H_
#define _callbacks_H_

class Sym;
class Match;
class Fun;

class Callbacks : public gc {
public:
  virtual void new_LUB_type(Sym *) = 0;
  virtual Sym *new_Sym(char *name) = 0;
  virtual Fun* build(Match *) = 0;
};

#endif
