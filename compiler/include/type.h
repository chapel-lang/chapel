#ifndef _TYPE_H_
#define _TYPE_H_


#include <stdio.h>
#include "baseAST.h"
#include "chplenum.h"
#include "../ifa/num.h"

/*
  Things which must be changed if instance variables are added
  to Types:

  1. add variable to class and constructor
  2. copy variable in copyInner

*/

class Symbol;
class EnumSymbol;
class VarSymbol;
class TypeSymbol;
class ArgSymbol;
class FnSymbol;
class Expr;
class DefExpr;
class CallExpr;
class CondStmt;
class SymScope;


class Type : public BaseAST {
 public:
  Vec<Type*> dispatchParents; // dispatch hierarchy
  Vec<Type*> dispatchChildren; // dispatch hierarchy
  Type* scalarPromotionType;

  TypeSymbol* symbol;
  Symbol* defaultValue;
  FnSymbol *defaultConstructor;
  Vec<FnSymbol*> methods;
  bool isGeneric;
  Type *instantiatedFrom;
  Vec<Type*> *instantiatedWith;
  ASTMap substitutions;

  Type(astType_t astType, Symbol* init_defaultVal);
  virtual void verify(); 
  COPY_DEF(Type);
  void addSymbol(TypeSymbol* newSymbol);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual void codegenStringToType(FILE* outfile);
  virtual void codegenConfigVarRoutines(FILE* outfile);
  virtual void codegenDefaultFormat(FILE* outfile, bool isRead);

  virtual bool requiresCParamTmp(intentTag intent);
  virtual bool implementedUsingCVals(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg);
  virtual bool hasDefaultReadFunction(void);
  virtual AList<Stmt>* buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg);
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)


class FnType : public Type {
 public:
  FnType(void);
  virtual void verify(); 
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
};


class EnumType : public Type {
 public:
  AList<DefExpr>* constants; // EnumSymbols

  EnumType(AList<DefExpr>* init_constants);
  virtual void verify(); 
  COPY_DEF(EnumType);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenStringToType(FILE* outfile);
  void codegenConfigVarRoutines(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
  bool implementedUsingCVals(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg);
  virtual bool hasDefaultReadFunction(void);
  virtual AList<Stmt>* buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg);
};


class UserType : public Type {
 public:
  Expr* typeExpr;

  UserType(Expr* init_typeExpr);
  virtual void verify(); 
  COPY_DEF(UserType);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
};


enum ClassTag {
  CLASS_CLASS,
  CLASS_RECORD
};

class ClassType : public Type {
 public:
  ClassTag classTag;
  SymScope* structScope;
  AList<Stmt>* declarationList;
  bool isPattern;

  AList<Expr>* inherits; // used from parsing, sets dispatchParents

  Vec<Symbol*> fields;

  ClassType(ClassTag initClassTag);
  virtual void verify(); 
  COPY_DEF(ClassType);
  void addDeclarations(AList<Stmt>* newDeclarations,
                       Stmt* afterStmt = NULL);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  bool isNominalType();

  virtual void codegenDef(FILE* outfile);
  virtual void codegenStructName(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);

  virtual bool implementedUsingCVals(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg);
  virtual bool hasDefaultReadFunction(void);
  virtual AList<Stmt>* buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg);

};


class PrimitiveType : public Type {
 public:
  PrimitiveType(Symbol *init_defaultVal = NULL);
  virtual void verify(); 
};


#ifndef TYPE_EXTERN
#define TYPE_EXTERN extern
#endif

// internal types
TYPE_EXTERN Type* dtAny;
TYPE_EXTERN PrimitiveType* dtNil;
TYPE_EXTERN PrimitiveType* dtUnknown;
TYPE_EXTERN PrimitiveType* dtVoid;
TYPE_EXTERN PrimitiveType* dtObject;
TYPE_EXTERN PrimitiveType* dtValue;
TYPE_EXTERN PrimitiveType* dtMethodToken;
TYPE_EXTERN PrimitiveType* dtSetterToken;

// primitive types
TYPE_EXTERN PrimitiveType* dtBool;
TYPE_EXTERN PrimitiveType* dtInt[INT_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtUInt[INT_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtFloat[FLOAT_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtComplex[FLOAT_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtString;
TYPE_EXTERN PrimitiveType* dtSymbol;
TYPE_EXTERN PrimitiveType* dtFile; 
TYPE_EXTERN PrimitiveType* dtMutex;
TYPE_EXTERN PrimitiveType* dtMutex_p;
TYPE_EXTERN PrimitiveType* dtCondVar_p;        // condition variable

void initPrimitiveTypes(void);

bool is_int_type(Type*);
bool is_uint_type(Type*);
bool is_float_type(Type*);
bool is_complex_type(Type*);
#define is_arithmetic_type(t) (is_int_type(t) || is_uint_type(t) || is_float_type(t) || is_complex_type(t))
int  get_width(Type*);

#endif
