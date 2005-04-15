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

  Type(astType_t astType, Expr* init_defaultVal);
  void addSymbol(Symbol* newSymbol);

  virtual bool isComplex(void);

  Type* copy(bool clone = false, Map<BaseAST*,BaseAST*>* map = NULL, CloneCallback* analysis_clone = NULL);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  Type *instantiate_generic(Map<Type *, Type *> &substitutions);

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
  virtual void codegenIORoutines(FILE* outfile);
  virtual void codegenConfigVarRoutines(FILE* outfile);
  virtual void codegenDefaultFormat(FILE* outfile, bool isRead);
  virtual void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg,
                             Expr* format = NULL);

  virtual bool outParamNeedsPtr(void);
  virtual bool requiresCParamTmp(paramType intent);
  virtual bool blankIntentImpliesRef(void);
  virtual bool implementedUsingCVals(void);
  
  //RED: facility to treat a type like other than this, if needed.
  //E.g. an IndexType would be treated as TupleType in some situations.
  virtual Type* getType();
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)


class EnumType : public Type {
 public:
  EnumSymbol* valList;

  EnumType(EnumSymbol* init_valList);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenStringToType(FILE* outfile);
  void codegenIORoutines(FILE* outfile);
  void codegenConfigVarRoutines(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
  bool implementedUsingCVals(void);
};


class DomainType : public Type {
 public:
  int numdims;
  Expr* parent;
  //upon creation, each domain creates an index type;
  Type* idxType;

  DomainType(Expr* init_expr = NULL);
  DomainType(int init_numdims);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  int rank(void);

  void print(FILE* outfile);
  void codegenDef(FILE* outfile);
  virtual void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg,
                             Expr* format = NULL);

  virtual bool blankIntentImpliesRef(void);
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
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  void codegenDef(FILE* outfile);
  void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg, 
                     Expr* format);
  
  void print(FILE* outfile);
  void traverseDefType(Traversal* traversal);
  Type* getType();
};


class UserType : public Type {
 public:
  Type* definition;

  UserType(Type* init_definition, Expr* init_defaultVal = NULL);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  bool isComplex(void);

  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenIORoutines(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
};


class LikeType : public Type {
 public:
  Expr* expr;

  LikeType(Expr* init_expr);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  bool isComplex(void);

  void traverseDefType(Traversal* traversal);

  void printDef(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
};


class StructuralType : public Type {
 public:
  Stmt* constructor;
  SymScope* structScope;
  Stmt* declarationList;
  StructuralType* parentStruct;

  Vec<VarSymbol*> fields;
  Vec<FnSymbol*> methods;
  Vec<TypeSymbol*> types;

  StructuralType(astType_t astType, Expr* init_defaultVal = NULL);
  void addDeclarations(Stmt* newDeclarations,
                       Stmt* afterStmt = NULL);
  void setScope(SymScope* init_structScope);
  void copyGuts(StructuralType* copy_type, bool clone, Map<BaseAST*,BaseAST*>* map, 
                CloneCallback* analysis_clone);

  void traverseDefType(Traversal* traversal);

  virtual Stmt* buildConstructorBody(Stmt* stmts, Symbol* _this, ParamSymbol* arguments);
  virtual Stmt* buildIOBodyStmtsHelp(Stmt* bodyStmts, ParamSymbol* thisArg);
  virtual Stmt* buildIOBodyStmts(ParamSymbol* thisArg);

  virtual void codegen(FILE* outfile);
  virtual void codegenStartDefFields(FILE* outfile);
  virtual void codegenStopDefFields(FILE* outfile);
  virtual void codegenDef(FILE* outfile);
  virtual void codegenStructName(FILE* outfile);
  virtual void codegenPrototype(FILE* outfile);
  virtual void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg,
                             Expr* format = NULL);
  virtual void codegenMemberAccessOp(FILE* outfile);

  virtual bool blankIntentImpliesRef(void);
  virtual bool implementedUsingCVals(void);
};


class ClassType : public StructuralType {
 public:
  ClassType(astType_t astType = TYPE_CLASS);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  virtual Stmt* buildIOBodyStmts(ParamSymbol* thisArg);

  virtual void codegenStructName(FILE* outfile);
  virtual void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg,
                             Expr* format = NULL);
  virtual void ClassType::codegenMemberAccessOp(FILE* outfile);

  virtual bool blankIntentImpliesRef(void);
  virtual bool implementedUsingCVals(void);
};


class RecordType : public StructuralType {
 public:
  RecordType(void);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
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
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void buildFieldSelector(void);
  FnCall* buildSafeUnionAccessCall(unionCall type, Expr* base, Symbol* field);
  CondStmt* buildUnionFieldIO(CondStmt* prevStmt, VarSymbol* field, ParamSymbol* thisArg);
  virtual Stmt* buildIOBodyStmtsHelp(Stmt* bodyStmts, ParamSymbol* thisArg);
  Stmt* buildConstructorBody(Stmt* stmts, Symbol* _this, ParamSymbol* arguments);

  void codegenStartDefFields(FILE* outfile);
  void codegenStopDefFields(FILE* outfile);
  void codegenMemberAccessOp(FILE* outfile);
};


class TupleType : public Type {
 public:
  Vec<Type*> components;

  TupleType(void);
  void addType(Type* additionalType);
  void rebuildDefaultVal(void);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseDefType(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg, Expr* format);
};


class SeqType : public ClassType {
 public:
  Type* elementType;

  SeqType::SeqType(Type* init_elementType);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  void traverseDefType(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  //void codegenPrototype(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
  void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg, Expr* format);
  bool implementedUsingCVals(void);
  static SeqType* createSeqType(char* new_seq_name, Type* init_elementType);
};


class ArrayType : public Type {
 public:
  Expr* domain;
  DomainType* domainType;
  Type* elementType;

  ArrayType(Expr* init_domain, Type* init_elementType);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseDefType(Traversal* traversal);

  int rank(void);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenPrototype(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);

  virtual bool blankIntentImpliesRef(void);
};


class SumType : public Type {
 public:
  Vec<Type*> components;

  SumType(Type* init_type);
  void addType(Type* additionalType);
};

class VariableType : public Type {
 public:
  Type* type;
  VariableType(Type *init_type = NULL);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  void codegen(FILE* outfile);
};

class UnresolvedType : public Type {
 public:
  UnresolvedType(char* init_symbol);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
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

#endif
