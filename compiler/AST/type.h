#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdio.h>

class Symbol;
class EnumSymbol;
class Expr;
class ASymbol;

class Type : public BaseAST {
 public:
  Symbol* name;
  ASymbol *asymbol;

  Type(astType_t astType);
  void addName(Symbol* newname);

  bool isNull(void);

  void traverse(Traversal* traversal);
  virtual void traverseType(Traversal* traversal);

  virtual int rank(void);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenIORoutines(FILE* outfile);
  virtual void codegenDefaultFormat(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
};
#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)

extern Type* nilType;


class EnumType : public Type {
 public:
  EnumSymbol* valList;

  EnumType(EnumSymbol* init_valList);

  void traverseType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenIORoutines(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
};


class DomainType : public Type {
 public:
  int numdims;

  DomainType(int init_numdims = 0);

  int rank(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class SubDomainType : public DomainType {
 public:
  Symbol* parent;

  SubDomainType(Symbol* init_parent);

  void print(FILE* outfile);
  int getSymbols(Vec<BaseAST *> &asts);
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

  void traverseType(Traversal* traversal);

  int rank(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile);
  int getExprs(Vec<BaseAST *> &asts);
  int getTypes(Vec<BaseAST *> &asts);
};


class UserType : public Type {
 public:
  Type* definition;

  UserType(Type* init_definition);

  void traverse(Traversal* traversal);

  void printDef(FILE* outfile);
  int getTypes(Vec<BaseAST *> &asts);
};


class ClassType;

extern ClassType* nilClassType;

class ClassType : public Type {
 public:
  ClassType* parentClass;
  
  ClassType(ClassType* init_parentClass = nilClassType);

  bool isNull(void);

  void print(FILE* outfile);
  int getTypes(Vec<BaseAST *> &asts);
};



// BLC: move these to standard prelude

extern Type* dtVoid;
extern Type* dtBoolean;
extern Type* dtInteger;
extern Type* dtFloat;
extern Type* dtString;

extern Type* dtTimer;
extern Type* dtLocale;

extern Type* dtUnknown;

extern Symbol* pstVoid;
extern Symbol* pstBoolean;
extern Symbol* pstInteger;
extern Symbol* pstFloat;
extern Symbol* pstString;

extern Symbol* pstTimer;
extern Symbol* pstLocale;

extern Symbol* pstUnknown;

void initType(void);

#endif
