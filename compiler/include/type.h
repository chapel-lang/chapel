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
class FnCall;
class CondStmt;
class ASymbol;
class SymScope;
class VariableType;


class Type : public BaseAST {
 public:
  TypeSymbol* symbol;
  Expr* defaultVal;
  FnSymbol *defaultConstructor;
  ASymbol *asymbol;
  Type* parentType;
  Vec<FnSymbol*> methods;
  Type *metaType;
  bool isGeneric;
  Vec<VariableType*> typeVariables;
  Type *instantiatedFrom;
  Map<BaseAST*,BaseAST*> substitutions;

  Type(astType_t astType, Expr* init_defaultVal);
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

  virtual bool outParamNeedsPtr(void);
  virtual bool requiresCParamTmp(paramType intent);
  virtual bool blankIntentImpliesRef(void);
  virtual bool implementedUsingCVals(void);
  
  //RED: facility to treat a type like other than this, if needed.
  //virtual Type* getType();

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ParamSymbol* arg);
  virtual bool hasDefaultReadFunction(void);
  virtual AList<Stmt>* buildDefaultReadFunctionBody(ParamSymbol* arg);
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)


class FnType : public Type {
 public:
  FnType(void);
  virtual void codegen(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
};


class EnumType : public Type {
 public:
  AList<DefExpr>* constants; // EnumSymbols

  EnumType(AList<DefExpr>* init_constants);
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
  COPY_DEF(UserType);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);

  Type* getType();
};


class StructuralType : public Type {
 public:
  SymScope* structScope;
  AList<Stmt>* declarationList;
  StructuralType* parentStruct;

  Vec<Symbol*> fields;
  Vec<TypeSymbol*> types;

  StructuralType(astType_t astType, Expr* init_defaultVal = NULL);
  void addDeclarations(AList<Stmt>* newDeclarations,
                       Stmt* afterStmt = NULL);
  void setScope(SymScope* init_structScope);
  void copyGuts(StructuralType* copy_type, bool clone, Map<BaseAST*,BaseAST*>* map);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseDefType(Traversal* traversal);

  virtual void buildConstructorBody(AList<Stmt>* stmts, Symbol* _this, 
                                    AList<DefExpr>* arguments);

  virtual void codegenStartDefFields(FILE* outfile);
  virtual void codegenStopDefFields(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenStructName(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual void codegenMemberAccessOp(FILE* outfile);

  virtual bool blankIntentImpliesRef(void);
  virtual bool implementedUsingCVals(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ParamSymbol* arg);
};


class ClassType : public StructuralType {
 public:
  Vec<ClassType*> parentClasses;
  ClassType(astType_t astType = TYPE_CLASS);
  COPY_DEF(ClassType);
  virtual void codegenStructName(FILE* outfile);
  virtual void ClassType::codegenMemberAccessOp(FILE* outfile);

  virtual bool blankIntentImpliesRef(void);
  virtual bool implementedUsingCVals(void);
};


class RecordType : public StructuralType {
 public:
  bool isPattern;
  RecordType(void);
  COPY_DEF(RecordType);
};


enum unionCall {
  UNION_SET = 0,
  UNION_CHECK,
  UNION_CHECK_QUIET,

  NUM_UNION_CALLS
};


class UnionType : public StructuralType {
 public:
  EnumType* fieldSelector;

  UnionType();
  COPY_DEF(UnionType);

  void buildFieldSelector(void);
  FnCall* buildSafeUnionAccessCall(unionCall type, Expr* base, Symbol* field);
  void buildConstructorBody(AList<Stmt>* stmts, Symbol* _this, 
                            AList<DefExpr>* arguments);

  void codegenStartDefFields(FILE* outfile);
  void codegenStopDefFields(FILE* outfile);
  void codegenMemberAccessOp(FILE* outfile);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ParamSymbol* arg);
};


class MetaType : public Type {
 public:
  Type* base;

  void traverseDefType(Traversal* traversal);
  MetaType(Type* init_base);
};

class SumType : public Type {
 public:
  Vec<Type*> components;

  SumType(Type* init_type);
  void addType(Type* additionalType);
  virtual void codegenStructName(FILE* outfile);
  void codegenDef(FILE* outfile);
};

class VariableType : public Type {
 public:
  Type* type;

  VariableType(Type *init_type = NULL);
  COPY_DEF(VariableType);
  void traverseDefType(Traversal* traversal);
  void codegen(FILE* outfile);
};

class NilType : public Type {
 public:
  NilType(void);
  void codegen(FILE* outfile);
};


#ifndef TYPE_EXTERN
#define TYPE_EXTERN extern
#endif

// null-ish types
TYPE_EXTERN Type* dtUnknown;
TYPE_EXTERN Type* dtVoid;
TYPE_EXTERN Type* dtNil;

// built-in types
TYPE_EXTERN Type* dtBoolean;
TYPE_EXTERN Type* dtInteger;
TYPE_EXTERN Type* dtFloat;
TYPE_EXTERN Type* dtComplex;
TYPE_EXTERN Type* dtString;

TYPE_EXTERN Type* dtTuple;
TYPE_EXTERN Type* dtIndex;
TYPE_EXTERN Type* dtDomain;
TYPE_EXTERN Type* dtArray;
void findInternalTypes(void);

TYPE_EXTERN Type* dtAny;
TYPE_EXTERN Type* dtNumeric;
TYPE_EXTERN Type* dtObject;

// abstract base types
TYPE_EXTERN Type* dtSequence;

// other funny types
TYPE_EXTERN Type* dtLocale;

// vector of built-in types
TYPE_EXTERN Vec<Type*> builtinTypes;


void initTypes(void);
Type *find_or_make_sum_type(Vec<Type *> *types);
int is_Scalar_Type(Type *t);
int is_Reference_Type(Type *t);
int is_Value_Type(Type *t);
Type *getMetaType(Type *t);


#endif
