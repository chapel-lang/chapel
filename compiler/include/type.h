#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdio.h>
#include "baseAST.h"
#include "analysis.h"
#include "vec.h"

class Symbol;
class EnumSymbol;
class VarSymbol;
class TypeSymbol;
class FnSymbol;
class Expr;
class ASymbol;
class SymScope;


// neither of these really seem like they should be here
enum paramType {
  PARAM_BLANK = 0,
  PARAM_IN,
  PARAM_INOUT,
  PARAM_OUT,
  PARAM_CONST,
  PARAM_REF,

  NUM_PARAM_TYPES
};

enum ioCallType {
  IO_WRITE = 0, 
  IO_WRITELN, 
  IO_READ
};



class Type : public BaseAST {
 public:
  Symbol* symbol;
  Expr* defaultVal;
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
	  void print(FILE* outfile);
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


class ClassType : public Type {
 public:
  bool value; /* true if this is a value class (aka record) */
  bool union_value; /* true if this is a union */
  ClassType* parentClass;
  Stmt* constructor;
  SymScope* classScope;
  Stmt* declarationList;

  Vec<VarSymbol*> fields;
  Vec<FnSymbol*> methods;
  Vec<TypeSymbol*> types;
  
  ClassType(bool isValueClass,
	    bool isUnion,
            ClassType* init_parentClass = NULL, 
	    Stmt* init_constructor = NULL,
	    SymScope* init_classScope = NULL);
  void addDeclarations(Stmt* newDeclarations,
		       Stmt* afterStmt = NULL);
  void setClassScope(SymScope* init_classScope);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseDefType(Traversal* traversal);

  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  void codegenPrototype(FILE* outfile);
  virtual void codegenIOCall(FILE* outfile, ioCallType ioType, Expr* arg,
			     Expr* format = NULL);

  virtual bool blankIntentImpliesRef(void);
};


class SeqType : public ClassType {
 public:
  Type* elementType;
  ClassType* nodeType;

  SeqType::SeqType(Type* init_elementType,
		   ClassType* init_nodeClassType = NULL);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);
  void traverseDefType(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
  //void codegenPrototype(FILE* outfile);
  void codegenDefaultFormat(FILE* outfile, bool isRead);
  void buildImplementationClasses();
};


class TupleType : public Type {
 public:
  Vec<Type*> components;

  TupleType(Type* init_type);
  void addType(Type* additionalType);
  void rebuildDefaultVal(void);
  virtual Type* copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone);

  void traverseDefType(Traversal* traversal);
  void print(FILE* outfile);
  void codegen(FILE* outfile);
  void codegenDef(FILE* outfile);
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

TYPE_EXTERN Type* dtNumeric;
TYPE_EXTERN Type* dtObject;

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
Type *find_or_make_sum_type(Vec<Type *> *types);

#endif
