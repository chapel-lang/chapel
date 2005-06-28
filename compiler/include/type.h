#ifndef _TYPE_H_
#define _TYPE_H_


#include <stdio.h>
#include "baseAST.h"
#include "analysis.h"
#include "chplenum.h"
#include "vec.h"


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


class Type : public BaseAST {
 public:
  Symbol* symbol;
  Expr* defaultVal;
  FnSymbol *defaultConstructor;
  ASymbol *asymbol;
  Type* parentType;
  Vec<FnSymbol*> methods;
  Type *metaType;

  Type(astType_t astType, Expr* init_defaultVal);
  COPY_DEF(Type);
  void addSymbol(Symbol* newSymbol);
  Type *instantiate_generic(Map<BaseAST *, BaseAST *> &substitutions);
  Type *getMetaType();

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
  //E.g. an IndexType would be treated as TupleType in some situations.
  virtual Type* getType();

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
  AList<EnumSymbol>* valList;

  EnumType(AList<EnumSymbol>* init_valList);
  COPY_DEF(EnumType);

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

class IndexType;  // break circular forward reference

class DomainType : public Type {
 public:
  int numdims;
  Expr* parent;
  //upon creation, each domain creates an index type;
  //RED -- it seems useful to keep track of the init expr
  Expr* initExpr;  
  IndexType* idxType;

  DomainType(Expr* init_expr = NULL);
  DomainType(int init_numdims);
  COPY_DEF(DomainType);
  void computeRank(void);

  int rank(void);

  void traverseDefType(Traversal* traversal);

  void print(FILE* outfile);
  void codegenDef(FILE* outfile);

  virtual bool blankIntentImpliesRef(void);

  virtual bool hasDefaultWriteFunction(void);
  virtual AList<Stmt>* buildDefaultWriteFunctionBody(ParamSymbol* arg);
};

//Roxana -- Index should not by subtype of Domain
//class IndexType : public DomainType {
class IndexType : public Type {
 public:
  //the expression this index is instantiated from: e.g., index(2)
  //or
  //pointer to the domain, set to NULL until index(D) is used.
  //then domain is used for bounds check
  Expr* idxExpr;
  DomainType* domainType;
  //the type of the index: k-tuple for arithmetic domains, scalar type, or enum, record, union of scalar type
  //for indefinite and opaque domains.
  //taken from the domain it is associated with, or created anew otherwise
  Type* idxType;

  //IndexType();
  IndexType(Expr* init_expr = NULL);
  //IndexType(int init_numdims);
  IndexType(Type* init_idxType);
  COPY_DEF(IndexType);
  void codegenDef(FILE* outfile);
  
  void print(FILE* outfile);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseDefType(Traversal* traversal);
  Type* getType();
};


class UserType : public Type {
 public:
  Type* definition;

  UserType(Type* init_definition, Expr* init_defaultVal = NULL);
  COPY_DEF(UserType);
  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);

  Type* getType();
};


class LikeType : public Type {
 public:
  Expr* expr;

  LikeType(Expr* init_expr);
  COPY_DEF(LikeType);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class StructuralType : public Type {
 public:
  SymScope* structScope;
  AList<Stmt>* declarationList;
  StructuralType* parentStruct;

  Vec<VarSymbol*> fields;
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


class TupleType : public StructuralType {
 public:
  Vec<Type*> components;

  TupleType(void);
  COPY_DEF(TupleType);
  void traverseDefType(Traversal* traversal);
  void print(FILE* outfile);

  void addType(Type* additionalType);
};


class ArrayType : public Type {
 public:
  Expr* domain;
  DomainType* domainType;
  Type* elementType;

  ArrayType(Expr* init_domain, Type* init_elementType);
  COPY_DEF(ArrayType);

  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void traverseDefType(Traversal* traversal);

  int rank(void);

  void print(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenPrototype(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);

  virtual bool blankIntentImpliesRef(void);

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

class ExprType : public Type {
 public:
  Expr* expr;

  ExprType(Expr *init_expr = NULL);
  COPY_DEF(ExprType);
  void traverseDefType(Traversal* traversal);
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  virtual void print(FILE* outfile);
  void codegen(FILE* outfile);
};

class UnresolvedType : public Type {
 public:
  Vec<char*>* names;

  UnresolvedType(Vec<char*>* init_names);
  COPY_DEF(UnresolvedType);
  void print(FILE* outfile);
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

TYPE_EXTERN Type* dtAny;
TYPE_EXTERN Type* dtNumeric;
TYPE_EXTERN Type* dtObject;

// abstract base types
TYPE_EXTERN Type* dtTuple;
TYPE_EXTERN Type* dtIndex;
TYPE_EXTERN Type* dtDomain;
TYPE_EXTERN Type* dtArray;
TYPE_EXTERN Type* dtSequence;

// other funny types
TYPE_EXTERN Type* dtLocale;

// vector of built-in types
TYPE_EXTERN Vec<Type*> builtinTypes;


void initTypes(void);
void findInternalTypes(void);
Type *find_or_make_sum_type(Vec<Type *> *types);
int is_Scalar_Type(Type *t);
int is_Reference_Type(Type *t);
int is_Value_Type(Type *t);
Type *getMetaType(Type *t);

#endif
