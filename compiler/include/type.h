#ifndef _TYPE_H_
#define _TYPE_H_


#include <stdio.h>
#include "baseAST.h"
#include "analysis.h"
#include "chplenum.h"
#include "vec.h"

/*
  Things which must be changed if instance variablers are added
  to Types:

  1. add variable to class and constructor
  2. copy variable in copyInner

*/

class Symbol;
class EnumSymbol;
class VarSymbol;
class TypeSymbol;
class ParamSymbol;
class FnSymbol;
class Expr;
class DefExpr;
class CallExpr;
class CondStmt;
class ASymbol;
class SymScope;
class VariableType;


class Type : public BaseAST {
 public:
  Vec<Type*> typeParents;     // type hierarchy
  Vec<Type*> dispatchParents; // dispatch hierarchy

  TypeSymbol* symbol;
  Expr* defaultValue;
  FnSymbol *defaultConstructor;
  ASymbol *asymbol;
  Vec<FnSymbol*> methods;
  Type *metaType;
  bool isGeneric;
  Vec<VariableType*> variableTypeSymbols;
  Type *instantiatedFrom;
  Map<BaseAST*,BaseAST*> substitutions;

  Type(astType_t astType, Expr* init_defaultVal);
  virtual void verify(void); 
  COPY_DEF(Type);
  void addSymbol(TypeSymbol* newSymbol);
  Type *instantiate_generic(Map<BaseAST *, BaseAST *> &substitutions);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  virtual void traverseType(Traversal* traversal);
  virtual void traverseDefType(Traversal* traversal);

  virtual int rank(void);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual void codegenSafeInit(FILE* outfile);
  virtual void codegenStringToType(FILE* outfile);
  virtual void codegenConfigVarRoutines(FILE* outfile);
  virtual void codegenDefaultFormat(FILE* outfile, bool isRead);

  virtual bool requiresCParamTmp(paramType intent);
  virtual bool implementedUsingCVals(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ParamSymbol* arg);
  virtual bool hasDefaultReadFunction(void);
  virtual AList<Stmt>* buildDefaultReadFunctionBody(ParamSymbol* arg);
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)


class FnType : public Type {
 public:
  FnType(void);
  virtual void verify(void); 
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
};


class EnumType : public Type {
 public:
  AList<DefExpr>* constants; // EnumSymbols

  EnumType(AList<DefExpr>* init_constants);
  virtual void verify(void); 
  COPY_DEF(EnumType);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenStringToType(FILE* outfile);
  void codegenConfigVarRoutines(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
  bool implementedUsingCVals(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ParamSymbol* arg);
  virtual bool hasDefaultReadFunction(void);
  virtual AList<Stmt>* buildDefaultReadFunctionBody(ParamSymbol* arg);
};


class UserType : public Type {
 public:
  Expr* defExpr;
  Type* defType;

  UserType(Type* init_defType, Expr* init_defaultVal = NULL);
  UserType(Expr* init_defExpr, Expr* init_defaultVal = NULL);
  UserType(Expr* init_defExpr, Type* init_defType, Expr* init_defaultVal = NULL);
  virtual void verify(void); 
  COPY_DEF(UserType);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
};


enum ClassTag {
  CLASS_CLASS,
  CLASS_VALUECLASS,
  CLASS_RECORD,
  CLASS_UNION
};

enum unionCall {
  UNION_SET = 0,
  UNION_CHECK,
  UNION_CHECK_QUIET,

  NUM_UNION_CALLS
};

class ClassType : public Type {
 public:
  ClassTag classTag;
  SymScope* structScope;
  AList<Stmt>* declarationList;
  bool isPattern;
  EnumType* fieldSelector;

  Vec<Symbol*> fields;
  Vec<TypeSymbol*> types;

  ClassType(ClassTag initClassTag);
  virtual void verify(void); 
  COPY_DEF(ClassType);
  void addDeclarations(AList<Stmt>* newDeclarations,
                       Stmt* afterStmt = NULL);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseDefType(Traversal* traversal);

  bool isNominalType();

  virtual void buildConstructorBody(AList<Stmt>* stmts, Symbol* _this, 
                                    AList<DefExpr>* arguments);

  virtual void codegenStartDefFields(FILE* outfile);
  virtual void codegenStopDefFields(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenStructName(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual void codegenMemberAccessOp(FILE* outfile);

  virtual bool implementedUsingCVals(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ParamSymbol* arg);

  void ClassType::buildFieldSelector(void);
  CallExpr* ClassType::buildSafeUnionAccessCall(unionCall type, Expr* base, 
                                                Symbol* field);


};


class MetaType : public Type {
 public:
  Type* base;

  void traverseDefType(Traversal* traversal);
  MetaType(Type* init_base);
  virtual void verify(void); 
};

class SumType : public Type {
 public:
  Vec<Type*> components;

  SumType();
  virtual void verify(void); 
  void addType(Type* additionalType);
  virtual void codegenStructName(FILE* outfile);
  void codegenDef(FILE* outfile);
};

class VariableType : public Type {
 public:
  Type* type;

  VariableType(Type *init_type = NULL);
  virtual void verify(void); 
  COPY_DEF(VariableType);
  void traverseDefType(Traversal* traversal);
  void codegen(FILE* outfile);
};

class PrimitiveType : public Type {
 public:
  PrimitiveType(Expr *init_defaultVal = NULL);
  virtual void verify(void); 
};


#ifndef TYPE_EXTERN
#define TYPE_EXTERN extern
#endif

// null-ish types
TYPE_EXTERN PrimitiveType* dtUnknown;
TYPE_EXTERN PrimitiveType* dtVoid;
TYPE_EXTERN PrimitiveType* dtNil;

// primitive types
TYPE_EXTERN PrimitiveType* dtBoolean;
TYPE_EXTERN PrimitiveType* dtInteger;
TYPE_EXTERN PrimitiveType* dtFloat;
TYPE_EXTERN PrimitiveType* dtComplex;
TYPE_EXTERN PrimitiveType* dtString;

// internal types
TYPE_EXTERN Type* dtTuple;
TYPE_EXTERN Type* dtIndex;
TYPE_EXTERN Type* dtDomain;
TYPE_EXTERN Type* dtArray;
void findInternalTypes(void);

// sum-ish types
TYPE_EXTERN Type* dtAny;
TYPE_EXTERN Type* dtNumeric;
TYPE_EXTERN Type* dtObject;

// abstract base types
TYPE_EXTERN Type* dtSequence;

// other funny types
TYPE_EXTERN Type* dtLocale;

// vector of built-in types
TYPE_EXTERN Vec<Type*> builtinTypes;

TYPE_EXTERN FnSymbol* initConfigFn;
TYPE_EXTERN FnSymbol* unionSetFn;
TYPE_EXTERN FnSymbol* unionCheckFn;
TYPE_EXTERN FnSymbol* unionQuietCheckFn;

void initType(void);
Type *find_or_make_sum_type(Vec<Type *> *types);
int is_Scalar_Type(Type *t);
int is_Reference_Type(Type *t);
int is_Value_Type(Type *t);
Type *getMetaType(Type *t);


#endif
