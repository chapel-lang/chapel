#ifndef _TYPE_H_
#define _TYPE_H_

#include <cstdio>
#include "baseAST.h"
#include "../ifa/num.h"

#include "genret.h"
#include <map>

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
class BlockStmt;
class ClassType;


class Type : public BaseAST {
 public:
  Vec<Type*> dispatchParents; // dispatch hierarchy
  Vec<Type*> dispatchChildren; // dispatch hierarchy
  Type* scalarPromotionType;

  TypeSymbol* symbol;
  Symbol* defaultValue;
  FnSymbol* initializer; // This is the compiler-supplied initializer function.
  FnSymbol* defaultTypeConstructor;
  FnSymbol* destructor;
  Vec<FnSymbol*> methods;
  bool hasGenericDefaults; // all generic fields have defaults
  Type *instantiatedFrom;
  SymbolMap substitutions;
  ClassType* refType;  // pointer to references for non-reference types
  bool isInternalType; // Used only in PrimitiveType; replace with flag?

  // Only used for LLVM.
  std::map<std::string, int> GEPMap;

  Type(AstTag astTag, Symbol* init_defaultVal);
  virtual ~Type();
  virtual Type* copy(SymbolMap* map = NULL, bool internal = false) = 0;
  virtual Type* copyInner(SymbolMap* map) = 0;
  virtual void replaceChild(BaseAST* old_ast, BaseAST* new_ast) = 0;

  virtual void verify(); 
  virtual bool inTree();
  virtual Type* typeInfo(void);

  void addSymbol(TypeSymbol* newSymbol);

  virtual GenRet codegen();
  virtual void codegenDef();
  virtual void codegenPrototype();

  // only used for heterogeneous compilations in which we need to define
  // what our data structures are for the point of conversions
  virtual int codegenStructure(FILE* outfile, const char* baseoffset);
  
  virtual Symbol* getField(const char* name, bool fatal=true);
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)

class EnumType : public Type {
 public:
  AList constants; // EnumSymbols

  // what integer type contains all of this enum values?
  // if this is NULL it will just be recomputed when needed.
  PrimitiveType* integerType;

  EnumType();
  ~EnumType();
  void verify(); 
  DECLARE_COPY(EnumType);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  void codegenDef();
  int codegenStructure(FILE* outfile, const char* baseoffset);

  // computes integerType and does the next=last+1 assignments.
  // This will only really work after the function resolution.
  void sizeAndNormalize();
  PrimitiveType* getIntegerType();
};


enum ClassTag {
  CLASS_CLASS,
  CLASS_RECORD,
  CLASS_UNION
};

class ClassType : public Type {
 public:
  ClassTag classTag;
  AList fields;
  AList inherits; // used from parsing, sets dispatchParents
  Symbol* outer;  // pointer to an outer class if this is an inner class
  const char *doc;

  ClassType(ClassTag initClassTag);
  ~ClassType();
  void verify(); 
  DECLARE_COPY(ClassType);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void addDeclarations(Expr* expr, bool tail = true);

  GenRet codegenClassStructType();
  void codegenDef();
  void codegenPrototype();
  const char* classStructName(bool standalone);
  int codegenStructure(FILE* outfile, const char* baseoffset);
  int codegenFieldStructure(FILE* outfile, bool nested, const char* baseoffset);

  int getMemberGEP(const char *name);

  int getFieldPosition(const char* name, bool fatal = true);
  Symbol* getField(const char* name, bool fatal = true);
  Symbol* getField(int i);
};


class PrimitiveType : public Type {
 public:
  PrimitiveType(Symbol *init_defaultVal = NULL, bool internalType=false);
  void verify(); 
  DECLARE_COPY(PrimitiveType);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef();
  int codegenStructure(FILE* outfile, const char* baseoffset);
};


#ifndef TYPE_EXTERN
#define TYPE_EXTERN extern
#endif

// internal types
TYPE_EXTERN Type* dtAny;
TYPE_EXTERN Type* dtIteratorRecord;
TYPE_EXTERN Type* dtIteratorClass;
TYPE_EXTERN Type* dtIntegral;
TYPE_EXTERN Type* dtAnyComplex;
TYPE_EXTERN Type* dtNumeric;
TYPE_EXTERN Type* dtAnyEnumerated;
TYPE_EXTERN PrimitiveType* dtNil;
TYPE_EXTERN PrimitiveType* dtUnknown;
TYPE_EXTERN PrimitiveType* dtVoid;
TYPE_EXTERN PrimitiveType* dtValue;
TYPE_EXTERN PrimitiveType* dtMethodToken;
TYPE_EXTERN PrimitiveType* dtTypeDefaultToken;
TYPE_EXTERN PrimitiveType* dtModuleToken;

// primitive types
// Anything declared as PrimitiveType* can now also be declared as Type*
// This change was made to allow dtComplex to be represented by a record.
TYPE_EXTERN PrimitiveType* dtBool;
TYPE_EXTERN PrimitiveType* dtBools[BOOL_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtInt[INT_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtUInt[INT_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtReal[FLOAT_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtImag[FLOAT_SIZE_NUM];
TYPE_EXTERN Type* dtComplex[COMPLEX_SIZE_NUM];
TYPE_EXTERN PrimitiveType* dtString;
TYPE_EXTERN PrimitiveType* dtSymbol;
TYPE_EXTERN PrimitiveType* dtFile; 
TYPE_EXTERN PrimitiveType* dtOpaque;
TYPE_EXTERN PrimitiveType* dtTaskID;
TYPE_EXTERN PrimitiveType* dtSyncVarAuxFields;
TYPE_EXTERN PrimitiveType* dtSingleVarAuxFields;
TYPE_EXTERN PrimitiveType* dtTaskList;

// a fairly special wide type
extern ClassType* wideStringType;

// Well-known types
TYPE_EXTERN ClassType* dtArray;
TYPE_EXTERN ClassType* dtReader;
TYPE_EXTERN ClassType* dtWriter;
TYPE_EXTERN ClassType* dtBaseArr;
TYPE_EXTERN ClassType* dtBaseDom;
TYPE_EXTERN ClassType* dtDist;
TYPE_EXTERN ClassType* dtTuple;
TYPE_EXTERN ClassType* dtLocale;
TYPE_EXTERN ClassType* dtLocaleID;
TYPE_EXTERN ClassType* dtMainArgument;

TYPE_EXTERN PrimitiveType* dtStringC; // the type of a C string.

// base object type (for all classes)
TYPE_EXTERN Type* dtObject;

TYPE_EXTERN Map<Type*,Type*> wideClassMap; // class -> wide class
TYPE_EXTERN Map<Type*,Type*> wideRefMap;   // reference -> wide reference

void initChplProgram(void);
void initPrimitiveTypes(void);
void initTheProgram(void);
void initCompilerGlobals(void);

bool is_void_type(Type*);
bool is_bool_type(Type*);
bool is_int_type(Type*);
bool is_uint_type(Type*);
bool is_signed(Type*);
bool is_real_type(Type*);
bool is_imag_type(Type*);
bool is_complex_type(Type*);
bool is_enum_type(Type*);
bool is_string_type(Type*);
#define is_arithmetic_type(t) (is_int_type(t) || is_uint_type(t) || is_real_type(t) || is_imag_type(t) || is_complex_type(t))
int  get_width(Type*);
bool isClass(Type* t);
bool isRecord(Type* t);
bool isUnion(Type* t);

bool isReferenceType(Type* t);

bool isRefCountedType(Type* t);
bool isRecordWrappedType(Type* t);
bool isSyncType(Type* t);
bool isAtomicType(Type* t);

bool isSubClass(Type* type, Type* baseType);
bool isDistClass(Type* type);
bool isDomainClass(Type* type);
bool isArrayClass(Type* type);

void registerTypeToStructurallyCodegen(TypeSymbol* type);
GenRet genTypeStructureIndex(TypeSymbol* typesym);
void codegenTypeStructures(FILE* hdrfile);
void codegenTypeStructureInclude(FILE* outfile);

Type* getNamedType(std::string name);

bool needsCapture(Type* t);
VarSymbol* resizeImmediate(VarSymbol* s, PrimitiveType* t);

// defined in codegen.cpp
GenRet codegenImmediate(Immediate* i);
#define CLASS_ID_TYPE dtInt[INT_SIZE_32]
#define UNION_ID_TYPE dtInt[INT_SIZE_64]
#define SIZE_TYPE dtInt[INT_SIZE_64]
#define LOCALE_TYPE dtLocale->typeInfo()
#define LOCALE_ID_TYPE dtLocaleID->typeInfo()
#define NODE_ID_TYPE dtInt[INT_SIZE_32]

#endif
