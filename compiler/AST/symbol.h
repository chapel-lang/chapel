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
  char* name;

  Symbol(char* init_name);

  void print(FILE*);
  void printList(FILE*);
};


class VarSymbol : public Symbol {
 public:
  bool isVar;
  varType varClass;
  Type* type;

  VarSymbol(char* init_name, Type* init_type = dtUnknown);
  
  void setType(Type* newType);
  void setIsVar(bool init_isVar);
  void setVarClass(varType init_varClass);

  void printWithType(FILE*);
};


class TypeSymbol : public Symbol {
 public:
  Type* definition;

  TypeSymbol(char* init_name, Type* init_definition);
};


// BLC: Get rid of this
extern Symbol* pstSumReduce;

#endif
