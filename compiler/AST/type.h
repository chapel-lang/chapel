#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdio.h>
#include "vec.h"

class Symbol;
class EnumSymbol;
class VarSymbol;
class Expr;
class ASymbol;
extern Expr* nilExpr;

class Type : public BaseAST {
 public:
  Symbol* name;
  ASymbol *asymbol;

  Type(astType_t astType);
  void addName(Symbol* newname);

  bool isNull(void);

  void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseType(Traversal* traversal);

  virtual int rank(void);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenIORoutines(FILE* outfile);
  virtual void codegenDefaultFormat(FILE* outfile);
  virtual bool needsInit(void);
  virtual void generateInit(FILE* outfile, VarSymbol* var);
  int getSymbols(Vec<BaseAST *> &asts);
};
#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)

extern Type* nilType;
extern Vec<Type*> builtinTypes;


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
  Expr* parent;

  DomainType(Expr* init_expr = nilExpr);
  DomainType(int init_numdims);

  int rank(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class IndexType : public DomainType {
 public:
  IndexType(Expr* init_expr = nilExpr);

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
  Stmt* definition;
  ClassType* parentClass;
  
  ClassType(ClassType* init_parentClass = nilClassType);
  void addDefinition(Stmt* init_definition);

  bool isNull(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenIORoutines(FILE* outfile);
  bool needsInit(void);
  void generateInit(FILE* outfile, VarSymbol* var);

  int getTypes(Vec<BaseAST *> &asts);
  int getStmts(Vec<BaseAST *> &asts);
};


class TupleType : public Type {
 public:
  Vec<Type*> components;

  TupleType(Type* init_type);
  void addType(Type* additionalType);

  void traverseType(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


extern Type* dtUnknown;
extern Type* dtVoid;
extern Type* dtBoolean;
extern Type* dtInteger;
extern Type* dtFloat;
extern Type* dtComplex;
extern Type* dtString;
extern Type* dtLocale;
extern Type* dtTimer;


void initType(void);

#endif
