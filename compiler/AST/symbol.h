#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "link.h"
#include "type.h"

class Symbol : public Link {
 public:
  char* name;
  Type* type;

  Symbol(char* init_name, Type* init_type);
  Symbol(char* init_name);

  void setType(Type* newType);

  void print(FILE*);
  void printWithType(FILE*);
  void printList(FILE*) {}
};

extern Symbol* pstSumReduce;

#endif
