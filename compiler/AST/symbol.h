#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "link.h"
#include "type.h"

enum varType {
  VAR_NORMAL,
  VAR_CONFIG,
  VAR_STATE
};

class Symbol : public Link {
 public:
  bool isVar;
  varType varClass;
  char* name;
  Type* type;

  Symbol(char* init_name, Type* init_type);
  Symbol(char* init_name);

  void setType(Type* newType);
  void setIsVar(bool init_isVar);
  void setVarClass(varType init_varClass);

  void print(FILE*);
  void printWithType(FILE*);
  void printList(FILE*);
};

// BLC: Get rid of this
extern Symbol* pstSumReduce;

#endif
