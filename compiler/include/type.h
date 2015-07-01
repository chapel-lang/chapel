/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include "baseAST.h"

#include "alist.h"
#include "genret.h"

#include "../ifa/num.h"

#include <cstdio>
#include <map>

/*
  Things which must be changed if instance variables are added
  to Types:

  1. add variable to class and constructor
  2. copy variable in copyInner

*/

class AggregateType;
class ArgSymbol;
class BlockStmt;
class CallExpr;
class CondStmt;
class DefExpr;
class EnumSymbol;
class Expr;
class FnSymbol;
class Symbol;
class TypeSymbol;
class VarSymbol;

class Type : public BaseAST {
public:
  virtual Type*    copy(SymbolMap* map = NULL, bool internal = false) = 0;

  // Interface for BaseAST
  virtual GenRet   codegen();
  virtual bool     inTree();
  virtual Type*    typeInfo();
  virtual void     verify();

  virtual void     codegenDef();
  virtual void     codegenPrototype();

  // only used for heterogeneous compilations in which we need to define
  // what our data structures are for the point of conversions
  virtual int      codegenStructure(FILE* outfile, const char* baseoffset);

  virtual Symbol*  getField(const char* name, bool fatal = true);

  void             addSymbol(TypeSymbol* newSymbol);

  TypeSymbol*      symbol;
  AggregateType*   refType;  // pointer to references for non-reference types
  Vec<FnSymbol*>   methods;

  bool             hasGenericDefaults; // all generic fields have defaults

  Symbol*          defaultValue;
  FnSymbol*        defaultInitializer; // This is the compiler-supplied
                                       // default-initializer.
                                       // It provides initial values for the
                                       // fields in an aggregate type.
  FnSymbol*        defaultTypeConstructor;
  FnSymbol*        destructor;

  // Used only in PrimitiveType; replace with flag?
  bool             isInternalType;

  Type*            instantiatedFrom;
  Type*            scalarPromotionType;

  SymbolMap        substitutions;
  Vec<Type*>       dispatchChildren;   // dispatch hierarchy
  Vec<Type*>       dispatchParents;    // dispatch hierarchy

  // Only used for LLVM.
  std::map<std::string, int> GEPMap;

protected:
                   Type(AstTag astTag, Symbol* init_defaultVal);
  virtual         ~Type();

private:
  virtual void     replaceChild(BaseAST* old_ast, BaseAST* new_ast) = 0;
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)

class EnumType : public Type {
 public:
  AList constants; // EnumSymbols


  // what integer type contains all of this enum values?
  // if this is NULL it will just be recomputed when needed.
  PrimitiveType* integerType;

  const char* doc;

  EnumType();
  ~EnumType();
  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_COPY(EnumType);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);

  void codegenDef();
  int codegenStructure(FILE* outfile, const char* baseoffset);

  // computes integerType and does the next=last+1 assignments.
  // This will only really work after the function resolution.
  void sizeAndNormalize();
  PrimitiveType* getIntegerType();

  virtual void printDocs(std::ostream *file, unsigned int tabs);

private:
  virtual std::string docsDirective();
};


enum AggregateTag {
  AGGREGATE_CLASS,
  AGGREGATE_RECORD,
  AGGREGATE_UNION
};

class AggregateType : public Type {
 public:
  AggregateTag aggregateTag;
  AList fields;
  AList inherits; // used from parsing, sets dispatchParents
  Symbol* outer;  // pointer to an outer class if this is an inner class
  const char *doc;

  AggregateType(AggregateTag initTag);
  ~AggregateType();
  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_COPY(AggregateType);
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
  bool isClass() { return aggregateTag == AGGREGATE_CLASS; }
  bool isRecord() { return aggregateTag == AGGREGATE_RECORD; }
  bool isUnion() { return aggregateTag == AGGREGATE_UNION; }

  virtual void printDocs(std::ostream *file, unsigned int tabs);

private:
  virtual std::string docsDirective();
  std::string docsSuperClass();
};


class PrimitiveType : public Type {
 public:
  PrimitiveType(Symbol *init_defaultVal = NULL, bool internalType=false);
  void verify();
  virtual void    accept(AstVisitor* visitor);
  DECLARE_COPY(PrimitiveType);
  void replaceChild(BaseAST* old_ast, BaseAST* new_ast);
  void codegenDef();
  int codegenStructure(FILE* outfile, const char* baseoffset);

  virtual void printDocs(std::ostream *file, unsigned int tabs);

private:
  virtual std::string docsDirective();
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
extern AggregateType* wideStringType;

// Well-known types
TYPE_EXTERN AggregateType* dtArray;
TYPE_EXTERN AggregateType* dtReader;
TYPE_EXTERN AggregateType* dtWriter;
TYPE_EXTERN AggregateType* dtBaseArr;
TYPE_EXTERN AggregateType* dtBaseDom;
TYPE_EXTERN AggregateType* dtDist;
TYPE_EXTERN AggregateType* dtTuple;
TYPE_EXTERN AggregateType* dtLocale;
TYPE_EXTERN AggregateType* dtLocaleID;
TYPE_EXTERN AggregateType* dtMainArgument;

TYPE_EXTERN PrimitiveType* dtStringC; // the type of a C string (unowned)
TYPE_EXTERN PrimitiveType* dtStringCopy; // the type of a C string (owned)

// base object type (for all classes)
TYPE_EXTERN Type* dtObject;

TYPE_EXTERN Map<Type*,Type*> wideClassMap; // class -> wide class
TYPE_EXTERN Map<Type*,Type*> wideRefMap;   // reference -> wide reference
TYPE_EXTERN std::map<Type*, Type*> wideToNarrowRefMap; // ref_T -> ref_wide_T
TYPE_EXTERN std::map<Type*, Type*> wideValMap; // ref_T -> ref_wide_T

void     initRootModule();
void     initPrimitiveTypes();
DefExpr* defineObjectClass();
void     initChplProgram(DefExpr* objectDef);
void     initCompilerGlobals();

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
