#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdio.h>
#include "baseAST.h"
#include "vec.h"

class Symbol;
class EnumSymbol;
class VarSymbol;
class Expr;
class ASymbol;
class SymScope;
class FnDefStmt;
extern Expr* nilExpr;
extern Stmt* nilStmt;
extern Symbol* nilSymbol;
extern FnDefStmt* nilFnDefStmt;


enum paramType {
  PARAM_BLANK = 0,
  PARAM_IN,
  PARAM_INOUT,
  PARAM_OUT,
  PARAM_CONST,

  NUM_PARAM_TYPES
};


class Type : public BaseAST {
  TRAVERSABLE_TYPE(Type);
 public:
  Symbol* name;
  Expr* defaultVal;
  ASymbol *asymbol;

  Type(astType_t astType, Expr* init_defaultVal);
  void addName(Symbol* newname);
  virtual Type* copy(void);

  bool isNull(void);
  virtual bool isComplex(void);

  void traverseDef(Type* &_this, Traversal* traversal, bool atTop = true);
  virtual void traverseType(Traversal* traversal);
  virtual void traverseDefType(Traversal* traversal);

  virtual int rank(void);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenSafeInit(FILE* outfile);
  virtual void codegenStringToType(FILE* outfile);
  virtual void codegenIORoutines(FILE* outfile);
  virtual void codegenConfigVarRoutines(FILE* outfile);
  virtual void codegenDefaultFormat(FILE* outfile, bool isRead);
  virtual void codegenConstructors(FILE* outfile);

  virtual bool needsInit(void);
  virtual void generateInit(FILE* outfile, VarSymbol* var);

  virtual bool outParamNeedsPtr(void);
  virtual bool requiresCParamTmp(paramType intent);
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)

extern Type* nilType;

class EnumType : public Type {
  TRAVERSABLE_TYPE(EnumType);
 public:
  EnumSymbol* valList;

  EnumType(EnumSymbol* init_valList);
  virtual Type* copy(void);

  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenStringToType(FILE* outfile);
  void codegenIORoutines(FILE* outfile);
  void codegenConfigVarRoutines(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
};


class DomainType : public Type {
  TRAVERSABLE_TYPE(DomainType);
 public:
  int numdims;
  Expr* parent;

  DomainType(Expr* init_expr = nilExpr);
  DomainType(int init_numdims);
  virtual Type* copy(void);

  int rank(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


class IndexType : public DomainType {
  TRAVERSABLE_TYPE(IndexType);
 public:
  IndexType(Expr* init_expr = nilExpr);
  IndexType(int init_numdims);
  virtual Type* copy(void);

  void print(FILE* outfile);
};


class ArrayType : public Type {
  TRAVERSABLE_TYPE(ArrayType);
 public:
  Expr* domain;
  Type* elementType;

  ArrayType(Expr* init_domain, Type* init_elementType);
  virtual Type* copy(void);

  void traverseDefType(Traversal* traversal);

  int rank(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
  bool needsInit(void);
  void generateInit(FILE* outfile, VarSymbol* sym);
};


class UserType : public Type {
  TRAVERSABLE_TYPE(UserType);
 public:
  Type* definition;

  UserType(Type* init_definition, Expr* init_defaultVal = nilExpr);
  virtual Type* copy(void);

  bool isComplex(void);

  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenIORoutines(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
};


class ClassType;

extern ClassType* nilClassType;

class ClassType : public Type {
  TRAVERSABLE_TYPE(ClassType);
 public:
  bool value; /* true if this is a value class (aka record) */
  ClassType* parentClass;
  Stmt* definition;
  FnDefStmt* constructor;
  SymScope* classScope;
  
  ClassType(bool isValueClass,
            ClassType* init_parentClass = nilClassType, 
	    Stmt* init_definition = nilStmt, 
	    FnDefStmt* init_constructor = nilFnDefStmt,
	    SymScope* init_classScope = NULL);
  void addDefinition(Stmt* init_definition);
  void setClassScope(SymScope* init_classScope);
  virtual Type* copy(void);

  void traverseDefType(Traversal* traversal);

  bool isNull(void);

  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenIORoutines(FILE* outfile);
  void codegenConstructors(FILE* outfile);
};


class TupleType : public Type {
  TRAVERSABLE_TYPE(TupleType);
 public:
  Vec<Type*> components;

  TupleType(Type* init_type);
  void addType(Type* additionalType);
  virtual Type* copy(void);

  void traverseDefType(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
};

class UnresolvedType : public Type {
  TRAVERSABLE_TYPE(UnresolvedType);
 public:
  UnresolvedType(char* init_name);
  virtual Type* copy(void);
  void codegen(FILE* outfile);
};

#ifndef TYPE_EXTERN
#define TYPE_EXTERN extern
#endif

// null-ish types
TYPE_EXTERN Type* dtUnknown;
TYPE_EXTERN Type* dtVoid;

// built-in types
TYPE_EXTERN Type* dtBoolean;
TYPE_EXTERN Type* dtInteger;
TYPE_EXTERN Type* dtFloat;
TYPE_EXTERN Type* dtComplex;
TYPE_EXTERN Type* dtString;

// abstract base types
TYPE_EXTERN Type* dtTuple;
TYPE_EXTERN Type* dtIndex;
TYPE_EXTERN Type* dtDomain;
TYPE_EXTERN Type* dtArray;

// other funny types
TYPE_EXTERN Type* dtLocale;

// vector of built-in types
TYPE_EXTERN Vec<Type*> builtinTypes;


void initTypes(void);
void findInternalTypes(void);

#endif
