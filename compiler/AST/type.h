#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdio.h>

class Symbol;
class Expr;

class Type : public Link {
 public:
  Symbol* name;

  Type();

  void addName(Symbol* newname);

  virtual void print(FILE* outfile);
  void printList(FILE* outfile) {}
};


class EnumType : public Type {
 public:
  Symbol* valList;

  EnumType(Symbol* init_valList);

  void print(FILE* outfile);
};


class DomainType : public Type {
 public:
  int rank;

  DomainType(int init_rank = 0);

  void print(FILE* outfile);
};


class ArrayType : public Type {
 public:
  Expr* domain;
  Type* elementType;

  ArrayType(Expr* init_domain, Type* init_elementType);

  void print(FILE* outfile);
};

extern Type* dtBoolean;
extern Type* dtInteger;
extern Type* dtFloat;

extern Type* dtTimer;
extern Type* dtLocale;

extern Symbol* pstBoolean;
extern Symbol* pstInteger;
extern Symbol* pstFloat;

extern Symbol* pstTimer;
extern Symbol* pstLocale;

void setupTypes(void);

#endif
