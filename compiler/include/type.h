/*
 * Copyright 2004-2018 Cray Inc.
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
#include <set>
#include <vector>

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
class IteratorInfo;

class Type : public BaseAST {
public:
  virtual Type*          copy(SymbolMap* map      = NULL,
                              bool       internal = false)                 = 0;

  // Interface for BaseAST
  virtual GenRet         codegen();
  virtual bool           inTree();
  virtual QualifiedType  qualType();
  virtual void           verify();

  virtual void           codegenDef();
  virtual void           codegenPrototype();

  // only used for heterogeneous compilations in which we need to define
  // what our data structures are for the point of conversions
  virtual int            codegenStructure(FILE*       outfile,
                                          const char* baseoffset);

  virtual Symbol*        getField(const char* name, bool fatal = true)   const;

  void                   addSymbol(TypeSymbol* newSymbol);

  bool                   isDefaultIntentConst()                          const;
  bool                   isWidePtrType()                                 const;

  // get/set on the type destructor
  bool                   hasDestructor()                                 const;
  FnSymbol*              getDestructor()                                 const;
  void                   setDestructor(FnSymbol* fn);

  TypeSymbol*            symbol;

  // pointer to references for non-reference types
  AggregateType*         refType;

  Vec<FnSymbol*>         methods;

  // all generic fields have defaults
  bool                   hasGenericDefaults;

  Symbol*                defaultValue;

  // Used only in PrimitiveType; replace with flag?
  bool                   isInternalType;

  Type*                  scalarPromotionType;

  SymbolMap              substitutions;

  // Only used for LLVM.
  std::map<std::string, int> GEPMap;

protected:
                   Type(AstTag astTag, Symbol* init_defaultVal);
  virtual         ~Type();

private:
  virtual void     replaceChild(BaseAST* old_ast, BaseAST* new_ast) = 0;

  FnSymbol*        destructor;
};

#define forv_Type(_p, _v) forv_Vec(Type, _p, _v)

const char* toString(Type* type);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// a Qualifier allows the compiler to distinguish between
// different properties of a variable (const or ref-ness in particular)
// without changing its type to a ref or wide ref type.
enum Qualifier {
  // The abstract qualifiers
  QUAL_UNKNOWN,
  QUAL_CONST,
  QUAL_REF,
  QUAL_CONST_REF,
  QUAL_PARAM,

  // The concrete qualifiers

  // We expect we will need QUAL_PARAM
  // QUAL_TYPE has also been proposed
  // As we add those, we expect to add methods
  // e.g. isParam() to QualifiedType.

  // QUAL_VAL applies to local or global variables
  // as well as to compiler-introduced temporaries.
  // Something with Qualifier QUAL_VAL is mutable, but
  // something with Qualifier QUAL_CONST_VAL is not.
  QUAL_VAL,
  QUAL_NARROW_REF,
  QUAL_WIDE_REF,

  QUAL_CONST_VAL,
  QUAL_CONST_NARROW_REF,
  QUAL_CONST_WIDE_REF
};

const char* qualifierToStr(Qualifier q);

// A QualifiedType is basically a tuple of (qualifier, type).
// Shorter names, such as QualType and QualT have been proposed.
// A QualifiedType is only expected to be meaningful during and
// after resolution.
//
// For example
//   var aVar:int;
//   ref aRef = aVar;
//
//   SymExpr(aVar) and Symbol(aVar) have QualifiedType(int, QUAL_VAL)
//   SymExpr(aRef) and Symbol(aRef) have QualifiedType(int, QUAL_REF)
//
class QualifiedType {
public:

  // Static methods for working with Qualifier
  static bool qualifierIsConst(Qualifier q)
  {
    return (q == QUAL_CONST ||
            q == QUAL_CONST_REF ||
            q == QUAL_CONST_VAL ||
            q == QUAL_CONST_NARROW_REF ||
            q == QUAL_CONST_WIDE_REF);
  }

  static Qualifier qualifierToConst(Qualifier q)
  {
    switch (q) {
      case QUAL_CONST:
      case QUAL_CONST_REF:
      case QUAL_CONST_NARROW_REF:
      case QUAL_CONST_WIDE_REF:
      case QUAL_CONST_VAL:
      case QUAL_PARAM:
        // already const
        return q;
      case QUAL_UNKNOWN:
        return QUAL_CONST;
      case QUAL_REF:
        return QUAL_CONST_REF;
      case QUAL_VAL:
        return QUAL_CONST_VAL;
      case QUAL_NARROW_REF:
        return QUAL_CONST_NARROW_REF;
      case QUAL_WIDE_REF:
        return QUAL_CONST_WIDE_REF;
      // no default: update as Qualifier is updated
    }
    return QUAL_UNKNOWN;
  }

  // QualifiedType methods

  explicit QualifiedType(Type* type)
    : _type(type), _qual(QUAL_UNKNOWN)
  {
  }

  QualifiedType(Qualifier qual, Type* type)
    : _type(type), _qual(qual)
  {
  }

  QualifiedType(Type* type, Qualifier qual)
    : _type(type), _qual(qual)
  {
  }

  bool isAbstract() const {
    return (_qual == QUAL_UNKNOWN || _qual == QUAL_CONST ||
            _qual == QUAL_REF || _qual == QUAL_CONST_REF);
  }

  bool isVal() const {
    return (_qual == QUAL_VAL || _qual == QUAL_CONST_VAL);
  }

  bool isRef() const {
    return (_qual == QUAL_REF || _qual == QUAL_CONST_REF ||
            _qual == QUAL_NARROW_REF || _qual == QUAL_CONST_NARROW_REF ||
            isRefType());
  }

  bool isWideRef() const {
    return (_qual == QUAL_WIDE_REF || _qual == QUAL_CONST_WIDE_REF ||
            isWideRefType());
  }

  bool isRefOrWideRef() const {
    return isRef() || isWideRef();
  }

  bool isConst() const {
    return qualifierIsConst(_qual);
  }
  // TODO: isImmutable

  bool isRefType() const;

  bool isWideRefType() const;

  QualifiedType toRef() {
    return QualifiedType(QUAL_REF, _type->getValType());
  }

  QualifiedType toVal() {
    return QualifiedType(QUAL_VAL, _type->getValType());
  }

  QualifiedType toConst() {
    return QualifiedType(qualifierToConst(_qual), _type);
  }

  Type* type() const {
    return _type;
  }

  Qualifier getQual() const {
    return _qual;
  }

  const char* qualStr() const;

private:
  Type*      _type;
  Qualifier  _qual;
};

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class EnumType : public Type {
 public:
  AList constants; // EnumSymbols


  // what integer type contains all of this enum values?
  // if this is NULL it will just be recomputed when needed.
  PrimitiveType* integerType;
 private:
  Immediate minConstant;
  Immediate maxConstant;

 public:
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
  Immediate* getMinConstant();
  Immediate* getMaxConstant();

  virtual void printDocs(std::ostream *file, unsigned int tabs);

private:
  virtual std::string docsDirective();
};


/************************************* | **************************************
*                                                                             *
* A base type for union, class, and record.                                   *
*                                                                             *
************************************** | *************************************/

#include "AggregateType.h"

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

#ifndef TYPE_EXTERN
#define TYPE_EXTERN extern
#endif

// internal types
TYPE_EXTERN Type*             dtAny;
TYPE_EXTERN Type*             dtIteratorRecord;
TYPE_EXTERN Type*             dtIteratorClass;
TYPE_EXTERN Type*             dtIntegral;
TYPE_EXTERN Type*             dtAnyComplex;
TYPE_EXTERN Type*             dtNumeric;
TYPE_EXTERN Type*             dtAnyEnumerated;

TYPE_EXTERN PrimitiveType*    dtNil;
TYPE_EXTERN PrimitiveType*    dtUnknown;
TYPE_EXTERN PrimitiveType*    dtVoid;
TYPE_EXTERN PrimitiveType*    dtValue;
TYPE_EXTERN PrimitiveType*    dtMethodToken;
TYPE_EXTERN PrimitiveType*    dtTypeDefaultToken;
TYPE_EXTERN PrimitiveType*    dtModuleToken;

// primitive types
// Anything declared as PrimitiveType* can now also be declared as Type*
// This change was made to allow dtComplex to be represented by a record.
TYPE_EXTERN PrimitiveType*    dtBool;
TYPE_EXTERN PrimitiveType*    dtBools[BOOL_SIZE_NUM];
TYPE_EXTERN PrimitiveType*    dtInt[INT_SIZE_NUM];
TYPE_EXTERN PrimitiveType*    dtUInt[INT_SIZE_NUM];
TYPE_EXTERN PrimitiveType*    dtReal[FLOAT_SIZE_NUM];
TYPE_EXTERN PrimitiveType*    dtImag[FLOAT_SIZE_NUM];
TYPE_EXTERN PrimitiveType*    dtSymbol;
TYPE_EXTERN PrimitiveType*    dtFile;
TYPE_EXTERN PrimitiveType*    dtOpaque;
TYPE_EXTERN PrimitiveType*    dtTaskID;
TYPE_EXTERN PrimitiveType*    dtSyncVarAuxFields;
TYPE_EXTERN PrimitiveType*    dtSingleVarAuxFields;

TYPE_EXTERN PrimitiveType*    dtStringC; // the type of a C string (unowned)
TYPE_EXTERN PrimitiveType*    dtCVoidPtr; // the type of a C void* (unowned)
TYPE_EXTERN PrimitiveType*    dtCFnPtr;   // a C function pointer (unowned)

TYPE_EXTERN Type*             dtComplex[COMPLEX_SIZE_NUM];

TYPE_EXTERN Map<Type*, Type*> wideClassMap; // class -> wide class
TYPE_EXTERN Map<Type*, Type*> wideRefMap;   // reference -> wide reference

void     initPrimitiveTypes();
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
bool isLegalParamType(Type*);
int  get_width(Type*);
int  get_mantissa_width(Type*);
int  get_exponent_width(Type*);
bool isClass(Type* t);
bool isClassOrNil(Type* t);
bool isRecord(Type* t);
bool isUnion(Type* t);

bool isReferenceType(const Type* t);

bool isRefCountedType(Type* t);
bool isRecordWrappedType(const Type* t);
bool isDomImplType(Type* t);
bool isArrayImplType(Type* t);
bool isDistImplType(Type* t);
bool isSyncType(const Type* t);
bool isSingleType(const Type* t);
bool isAtomicType(const Type* t);
bool isRefIterType(Type* t);

bool isSubClass(Type* type, Type* baseType);
bool isDistClass(Type* type);
bool isDomainClass(Type* type);
bool isArrayClass(Type* type);

bool isString(Type* type);
bool isUserDefinedRecord(Type* type);

bool isPrimitiveScalar(Type* type);

bool isNonGenericClass (Type* type);
bool isNonGenericRecord(Type* type);

bool isNonGenericClassWithInitializers (Type* type);
bool isNonGenericRecordWithInitializers(Type* type);

bool isGenericClass (Type* type);
bool isGenericRecord(Type* type);

bool isGenericClassWithInitializers (Type* type);
bool isGenericRecordWithInitializers(Type* type);

bool isClassWithInitializers (Type* type);
bool isRecordWithInitializers(Type* type);

void registerTypeToStructurallyCodegen(TypeSymbol* type);
GenRet genTypeStructureIndex(TypeSymbol* typesym);
void codegenTypeStructures(FILE* hdrfile);
void codegenTypeStructureInclude(FILE* outfile);

Type* getNamedType(std::string name);

bool needsCapture(Type* t);
VarSymbol* resizeImmediate(VarSymbol* s, PrimitiveType* t);

bool isPOD(Type* t);

// defined in codegen.cpp
GenRet codegenImmediate(Immediate* i);




#define CLASS_ID_TYPE dtInt[INT_SIZE_32]
#define UNION_ID_TYPE dtInt[INT_SIZE_64]
#define SIZE_TYPE dtInt[INT_SIZE_64]
#define NODE_ID_TYPE dtInt[INT_SIZE_32]

#define is_arithmetic_type(t)                        \
        (is_int_type(t)        ||                    \
         is_uint_type(t)       ||                    \
         is_real_type(t)       ||                    \
         is_imag_type(t)       ||                    \
         is_complex_type(t))


#endif
