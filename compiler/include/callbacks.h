/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _callbacks_H_
#define _callbacks_H_

class Callbacks : public gc {
public:
  virtual void new_SUM_type(Sym *) = 0;
  virtual Sym *new_Sym(char *name) = 0;
};

#endif
