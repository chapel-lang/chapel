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
  virtual void printDef(FILE* outfile);
};


class NullType : public Type {
 public:
  NullType(void);

  bool isNull(void) { return true; }
};


class EnumType : public Type {
 public:
  Symbol* valList;

  EnumType(Symbol* init_valList);

  void printDef(FILE* outfile);
};


class DomainType : public Type {
 public:
  int rank;

  DomainType(int init_rank = 0);

  void print(FILE* outfile);
};


class SubDomainType : public DomainType {
 public:
  Symbol* parent;

  SubDomainType(Symbol* init_parent);

  void print(FILE* outfile);
};


class IndexType : public DomainType {
 public:
  IndexType(int init_rank = 0);

  void print(FILE* outfile);
};


class SubIndexType : public SubDomainType {
 public:
  SubIndexType(Symbol* init_parent);

  void print(FILE* outfile);
};


class ArrayType : public Type {
 public:
  Expr* domain;
  Type* elementType;

  ArrayType(Expr* init_domain, Type* init_elementType);

  void print(FILE* outfile);
};


class UserType : public Type {
 public:
  Type* definition;

  UserType(Type* init_definition);

  void printDef(FILE* outfile);
};


// BLC: move these to standard prelude

extern Type* dtBoolean;
extern Type* dtInteger;
extern Type* dtFloat;

extern Type* dtTimer;
extern Type* dtLocale;

extern Type* dtUnknown;

extern Symbol* pstBoolean;
extern Symbol* pstInteger;
extern Symbol* pstFloat;

extern Symbol* pstTimer;
extern Symbol* pstLocale;

extern Symbol* pstUnknown;

void initType(void);

#endif
