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

#define TYPE_EXTERN

#include "type.h"

#include "AggregateType.h"
#include "AstToText.h"
#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "intlimits.h"
#include "ipe.h"
#include "iterator.h"
#include "misc.h"
#include "passes.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"
#include "vec.h"

static bool isDerivedType(Type* type, Flag flag);

Type::Type(AstTag astTag, Symbol* iDefaultVal) : BaseAST(astTag) {
  symbol              = NULL;
  refType             = NULL;
  hasGenericDefaults  = false;
  defaultValue        = iDefaultVal;
  destructor          = NULL;
  isInternalType      = false;
  scalarPromotionType = NULL;
}

Type::~Type() {

}

void Type::verify() {
}

void Type::addSymbol(TypeSymbol* newSymbol) {
  symbol = newSymbol;
}

bool Type::inTree() {
  if (symbol)
    return symbol->inTree();
  else
    return false;
}


QualifiedType Type::qualType() {
  return QualifiedType(this);
}

// Are actuals of this type passed with const intent by default?
bool Type::isDefaultIntentConst() const {
  bool retval = true;

  // MPF 2017-03-09
  // It seems wrong to me that this returns true
  // for dtUnknown. However some parts of the compiler
  // currently rely on that behavior.

  if (symbol->hasFlag(FLAG_DEFAULT_INTENT_IS_REF) == true ||
      isReferenceType(this)                       == true ||
      isRecordWrappedType(this)                   == true)
    retval = false;

  return retval;
}

bool Type::isWidePtrType() const {
  if (symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS)) {
    // Workaround an ugly hack in insert wide references
    // which can make a wide _array record containing an "addr" record
    Type* baseType = this->getField("addr")->type;
    if (isReferenceType(baseType) || isClass(baseType) || baseType == dtNil)
      return true;
  }
  return false;
}

Symbol* Type::getField(const char* name, bool fatal) const {
  INT_FATAL(this, "getField not called on AggregateType");
  return NULL;
}

bool Type::hasDestructor() const {
  return destructor != NULL;
}

FnSymbol* Type::getDestructor() const {
  return destructor;
}

void Type::setDestructor(FnSymbol* fn) {
  destructor = fn;
}

const char* toString(Type* type) {
  const char* retval = NULL;

  if (type != NULL) {
    retval = type->getValType()->symbol->name;

  } else {
    retval = "null type";
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Qualifier and QualifiedType                                                 *
*                                                                             *
************************************** | *************************************/

const char* qualifierToStr(Qualifier q) {
    switch (q) {
      case QUAL_UNKNOWN:
        return "unknown";

      case QUAL_CONST:
        return "const";
      case QUAL_REF:
        return "ref";
      case QUAL_CONST_REF:
        return "const-ref";

      case QUAL_PARAM:
        return "param";

      case QUAL_VAL:
        return "val";
      case QUAL_NARROW_REF:
        return "narrow-ref";
      case QUAL_WIDE_REF:
        return "wide-ref";

      case QUAL_CONST_VAL:
        return "const-val";
      case QUAL_CONST_NARROW_REF:
        return "const-narrow-ref";
      case QUAL_CONST_WIDE_REF:
        return "const-wide-ref";
    }

    INT_FATAL("Unhandled Qualifier");
    return "UNKNOWN-QUAL";
}

bool QualifiedType::isRefType() const {
  return _type->symbol->hasFlag(FLAG_REF);
}

bool QualifiedType::isWideRefType() const {
  return _type->symbol->hasFlag(FLAG_WIDE_REF);
}

const char* QualifiedType::qualStr() const {
  if (isRefType())
    return qualifierToStr(QUAL_REF);

  if (isWideRefType())
    return qualifierToStr(QUAL_WIDE_REF);

  // otherwise
  return qualifierToStr(_qual);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

PrimitiveType::PrimitiveType(Symbol *init, bool internalType) :
  Type(E_PrimitiveType, init)
{
  isInternalType = internalType;
  gPrimitiveTypes.add(this);
}


PrimitiveType*
PrimitiveType::copyInner(SymbolMap* map) {
  //
  // If we're trying to make a copy of an internal Chapel primitive
  // type (say 'int'), that's a sign that something is wrong.  For
  // external primitive types, it should be OK to make such copies.
  // This may be desired/required if the extern type declaration is
  // local to a generic Chapel procedure for example and we're
  // creating multiple instantiations of that procedure, each of which
  // wants/needs its own local type symbol.  This exception may
  // suggest that external primitive types should really be
  // represented as their own ExternType class...
  //
  if (!symbol->hasFlag(FLAG_EXTERN)) {
    INT_FATAL(this, "Unexpected call to PrimitiveType::copyInner");
  }

  return new PrimitiveType(NULL);
}


void PrimitiveType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in PrimitiveType::replaceChild");
}


void PrimitiveType::verify() {
  Type::verify();
  if (astTag != E_PrimitiveType) {
    INT_FATAL(this, "Bad PrimitiveType::astTag");
  }
}

void PrimitiveType::printDocs(std::ostream *file, unsigned int tabs) {
  // Only print extern types.
  if (this->symbol->noDocGen()) {
    return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();
  *file << "type ";
  *file << this->symbol->name;
  *file << std::endl;

  // For .rst mode, put a line break after the .. data:: directive and
  // its description text.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->symbol->doc != NULL) {
    this->printDocsDescription(this->symbol->doc, file, tabs + 1);
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


std::string PrimitiveType::docsDirective() {
  if (!fDocsTextOnly) {
    return ".. type:: ";
  } else {
    return "";
  }
}


void PrimitiveType::accept(AstVisitor* visitor) {
  visitor->visitPrimType(this);
}

EnumType::EnumType() :
  Type(E_EnumType, NULL),
  constants(), integerType(NULL), minConstant(), maxConstant(),
  doc(NULL)
{
  gEnumTypes.add(this);
  constants.parent = this;
}


EnumType::~EnumType() { }


void EnumType::verify() {
  Type::verify();
  if (astTag != E_EnumType) {
    INT_FATAL(this, "Bad EnumType::astTag");
  }
  if (constants.parent != this)
    INT_FATAL(this, "Bad AList::parent in EnumType");
  for_alist(expr, constants) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad EnumType::constants::parentSymbol");
    if (!isDefExpr(expr))
      INT_FATAL(this, "Bad EnumType::constants - not a DefExpr");
  }
}


EnumType*
EnumType::copyInner(SymbolMap* map) {
  EnumType* copy = new EnumType();

  for_enums(def, this) {
    DefExpr* newDef = COPY_INT(def);
    newDef->sym->type = copy;
    copy->constants.insertAtTail(newDef);
  }
  copy->minConstant = this->minConstant;
  copy->maxConstant = this->maxConstant;;
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumType::replaceChild");
}


void EnumType::sizeAndNormalize() {
  bool first = true;
  bool issigned = false;
  int64_t v;
  uint64_t uv;
  int64_t min_v, max_v;
  uint64_t min_uv, max_uv;
  int num_bytes;
  uint64_t max;
  int64_t min;
  PrimitiveType* ret = NULL;
  Immediate* minImm = NULL;
  Immediate* maxImm = NULL;

  // First, look for negative numbers in the enum.
  // If there are any, we have to store all
  // the values in negative numbers.
  for_enums(constant, this) {
    // If this passes testing, much of the following can
    // be significantly simplified.
    INT_ASSERT(constant->init);

    if( constant->init ) {
      if( get_int( constant->init, &v ) ) {
        if( v < 0 ) {
          issigned = true;
        }
      } else if( get_uint( constant->init, &uv ) ) {
        // OK!
      } else {
        // If we get here, then the initializer does not have an immediate
        // value associated with it....
        SymExpr* sym = toSymExpr(constant->init);

        // We think that all params should have init values by now.
        INT_ASSERT(sym && !sym->symbol()->hasFlag(FLAG_PARAM));

        // So we're going to blame this on the user.
        USR_FATAL(constant,
                  "enumerator '%s' is not an integer param value",
                  constant->sym->name);
        // And unfortunately, if we get here, we don't know how to proceed,
        // which is why no USR_FATAL_CONT().
      }
    }
  }

  // Set initializers for all enum symbols and
  // compute the minimum and maximum values.
  v = 1;
  uv = 1;
  min_v = max_v = 1;
  min_uv = max_uv = 1;

  first = true;
  for_enums(constant, this) {
    SET_LINENO(constant);
    if( constant->init ) {
      // set v and uv to the initializer value
      // if the number doesn't fit in one of them, set it
      // to 1. That avoids overflow when we increment these below,
      // and won't confuse max/min calculations since any size
      // integer needs to be able to represent 1.
      if( get_int( constant->init, &v ) ) {
        if( v >= 0 ) uv = v;
        else uv = 1;
      } else if( get_uint( constant->init, &uv ) ) {
        if (uv <= (uint64_t)INT64_MAX) v = uv;
        else v = 1;
      }
    } else {
      // create initializer with v
      if( issigned ) {
        if( v >= INT32_MIN && v <= INT32_MAX ) {
          constant->init = new SymExpr(new_IntSymbol(v, INT_SIZE_32));
        } else {
          constant->init = new SymExpr(new_IntSymbol(v, INT_SIZE_64));
        }
      } else {
        if( uv <= UINT32_MAX ) {
          constant->init = new SymExpr(new_UIntSymbol(uv, INT_SIZE_32));
        } else {
          constant->init = new SymExpr(new_UIntSymbol(uv, INT_SIZE_64));
        }
      }
      parent_insert_help(constant, constant->init);
    }
    if( first ) {
      min_v = v;
      max_v = v;
      min_uv = uv;
      max_uv = uv;
      first = false;
    } else {
      if( min_v > v ) min_v = v;
      if( max_v < v ) max_v = v;
      if( min_uv > uv ) min_uv = uv;
      if( max_uv < uv ) max_uv = uv;

    }
    // Increment v for the next one, in case it is not set.
    v++;
    uv++;
  }

  // User error if the enum contains both negative values and
  // something needing a uint64.
  if (min_v < 0 && max_uv > (uint64_t)INT64_MAX)
    USR_FATAL(this,
              "this enum cannot be represented with a single integer type");

  num_bytes = 0;

  // Now figure out, based on min/max values, what integer
  // size we must use.
  if( issigned ) {
    int num_bytes_neg = 0;
    if( min_v >= INT8_MIN ) {
      num_bytes_neg = 1;
    } else if( min_v >= INT16_MIN ) {
      num_bytes_neg = 2;
    } else if( min_v >= INT32_MIN ) {
      num_bytes_neg = 4;
    } else {
      num_bytes_neg = 8;
    }

    if( max_v <= INT8_MAX ) {
      num_bytes = 1;
    } else if( max_v <= INT16_MAX ) {
      num_bytes = 2;
    } else if( max_v <= INT32_MAX ) {
      num_bytes = 4;
    } else {
      num_bytes = 8;
    }

    if( num_bytes < num_bytes_neg ) num_bytes = num_bytes_neg;
  } else {
    if( max_uv <= UINT8_MAX ) {
      num_bytes = 1;
    } else if( max_uv <= UINT16_MAX ) {
      num_bytes = 2;
    } else if( max_uv <= UINT32_MAX ) {
      num_bytes = 4;
    } else {
      num_bytes = 8;
    }
  }

  // Now figure out field min and max values.
  // and set et->integerType
  min = max = 0;

  IF1_int_type int_size = INT_SIZE_DEFAULT;

  if( num_bytes == 1 ) {
    int_size = INT_SIZE_8;
    if( issigned ) {
      max = INT8_MAX;
      min = INT8_MIN;
    } else {
      max = UINT8_MAX;
    }
  } else if( num_bytes == 2 ) {
    int_size = INT_SIZE_16;
    if( issigned ) {
      max = INT16_MAX;
      min = INT16_MIN;
    } else {
      max = UINT16_MAX;
    }
  } else if( num_bytes == 4 ) {
    int_size = INT_SIZE_32;
    if( issigned ) {
      max = INT32_MAX;
      min = INT32_MIN;
    } else {
      max = UINT32_MAX;
    }
  } else if( num_bytes == 8 ) {
    int_size = INT_SIZE_64;
    if( issigned ) {
      max = INT64_MAX;
      min = INT64_MIN;
    } else {
      max = UINT64_MAX;
    }
  }

  if (issigned)
    ret = dtInt[int_size];
  else
    ret = dtUInt[int_size];

  // At the end of it all, check that each enum
  // symbol fits into the assigned type.
  // This check is necessary because we might have
  // had the impossible-to-fit enum
  // because it has e.g. UINT64_MAX and INT64_MIN.
  for_enums(constant, this) {
    if( get_int( constant->init, &v ) ) {
      if( v < min || (v > 0 && (uint64_t)v > max) ) {
        INT_FATAL(constant, "Does not fit in enum integer type");
      }
    } else if( get_uint( constant->init, &uv ) ) {
      if( uv > max ) {
        INT_FATAL(constant, "Does not fit in enum integer type");
      }
    }
  }

  // Replace the immediates with one of the appropriate numeric type.
  // This is a way of normalizing the enum constants and simplifying
  // what follow-on passes need to deal with.
  for_enums(constant, this) {
    SET_LINENO(constant);
    INT_ASSERT(constant->init);

    if (ret == constant->init->typeInfo()) {
      // Nothing to do, constant already has appropriate type
    } else {
      bool have_v = true;
      bool have_uv = true;
      v = 0;
      uv = 0;
      // set v and uv to the initializer value
      if( get_int( constant->init, &v ) ) {
        if( v >= 0 ) uv = v;
        else have_uv = false;
      } else if( get_uint( constant->init, &uv ) ) {
        if (uv <= (uint64_t)INT64_MAX) v = uv;
        else have_v = false;
      }

      if( issigned ) {
        INT_ASSERT(have_v);
        constant->init->replace(new SymExpr(new_IntSymbol(v, int_size)));
      } else {
        INT_ASSERT(have_uv);
        constant->init->replace(new SymExpr(new_UIntSymbol(uv, int_size)));
      }
    }
  }

  first = true;

  min_v = max_v = 0;
  min_uv = max_uv = 0;

  // Set minConstant and maxConstant
  for_enums(constant, this) {
    SET_LINENO(constant);
    INT_ASSERT(constant->init);

    bool got = false;

    v = 0;
    uv = 0;
    if (issigned)
      got = get_int(constant->init, &v);
    else
      got = get_uint(constant->init, &uv);

    INT_ASSERT(got);

    Immediate* imm = getSymbolImmediate(constant->sym);

    if (first) {
      if (issigned) {
        min_v = v;
        max_v = v;
      } else {
        min_uv = uv;
        max_uv = uv;
      }
      minImm = imm;
      maxImm = imm;
      first = false;
    } else {
      if (issigned) {
        if (v < min_v) {
          min_v = v;
          minImm = imm;
        }
        if (v > max_v) {
          max_v = v;
          maxImm = imm;
        }
      } else {
        if (uv < min_uv) {
          min_uv = uv;
          minImm = imm;
        }
        if (uv > max_uv) {
          max_uv = uv;
          maxImm = imm;
        }
      }
    }
  }

  INT_ASSERT(minImm && maxImm);
  this->minConstant = *minImm;
  this->maxConstant = *maxImm;


  integerType = ret;
}

PrimitiveType* EnumType::getIntegerType() {
  INT_ASSERT(integerType);
  if( ! integerType ) {
    sizeAndNormalize();
  }
  return integerType;
}

Immediate* EnumType::getMinConstant() {
  return &minConstant;
}
Immediate* EnumType::getMaxConstant() {
  return &maxConstant;
}


void EnumType::accept(AstVisitor* visitor) {
  if (visitor->enterEnumType(this) == true) {

    for_alist(next_ast, constants) {
      next_ast->accept(visitor);
    }

    visitor->exitEnumType(this);
  }
}


void EnumType::printDocs(std::ostream *file, unsigned int tabs) {
  if (this->symbol->noDocGen()) {
    return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();
  *file << "enum ";
  AstToText info;
  info.appendEnumDecl(this);
  *file << info.text();
  *file << std::endl;

  // In rst mode, ensure there is an empty line between the enum signature and
  // its description or the next directive.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->doc != NULL) {
    this->printDocsDescription(this->doc, file, tabs + 1);
    *file << std::endl;

    // In rst mode, ensure there is an empty line between the enum description
    // and the next directive.
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


std::string EnumType::docsDirective() {
  if (fDocsTextOnly) {
    return "";
  } else {
    return ".. enum:: ";
  }
}





/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static PrimitiveType* createPrimitiveType(const char* name, const char* cname);
static PrimitiveType* createInternalType (const char* name, const char* cname);

static PrimitiveType* createType(const char* name,
                                 const char* cname,
                                 bool        internalType);

static VarSymbol*     createSymbol(PrimitiveType* primType, const char* name);

// Create new primitive type for integers.
// Specify name for now.
// Though it will probably be something like int1, int8, etc. in the end.
// In that case we can just specify the width (i.e., size).
#define INIT_PRIM_BOOL(name, width)                                             \
  dtBools[BOOL_SIZE_ ## width] = createPrimitiveType(name, "chpl_bool" #width); \
  dtBools[BOOL_SIZE_ ## width]->defaultValue = new_BoolSymbol( false, BOOL_SIZE_ ## width)

#define INIT_PRIM_INT( name, width)                                             \
  dtInt[INT_SIZE_ ## width] = createPrimitiveType (name, "int" #width "_t");    \
  dtInt[INT_SIZE_ ## width]->defaultValue = new_IntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIM_UINT( name, width)                                            \
  dtUInt[INT_SIZE_ ## width] = createPrimitiveType (name, "uint" #width "_t");  \
  dtUInt[INT_SIZE_ ## width]->defaultValue = new_UIntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIM_REAL( name, width)                                            \
  dtReal[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_real" #width);    \
  dtReal[FLOAT_SIZE_ ## width]->defaultValue = new_RealSymbol( "0.0", FLOAT_SIZE_ ## width)

#define INIT_PRIM_IMAG( name, width)                                            \
  dtImag[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_imag" #width);    \
  dtImag[FLOAT_SIZE_ ## width]->defaultValue = new_ImagSymbol( "0.0", FLOAT_SIZE_ ## width)

#define INIT_PRIM_COMPLEX( name, width)                                                   \
  dtComplex[COMPLEX_SIZE_ ## width]= createPrimitiveType (name, "_complex" #width);       \
  dtComplex[COMPLEX_SIZE_ ## width]->defaultValue = new_ComplexSymbol(                    \
                                  "_chpl_complex" #width "(0.0, 0.0)",                    \
                                   0.0, 0.0, COMPLEX_SIZE_ ## width);                     \
  dtComplex[COMPLEX_SIZE_ ## width]->GEPMap.insert(std::pair<std::string, int>("re", 0)); \
  dtComplex[COMPLEX_SIZE_ ## width]->GEPMap.insert(std::pair<std::string, int>("im", 1));

#define CREATE_DEFAULT_SYMBOL(primType, gSym, name)     \
  gSym = new VarSymbol (name, primType);                \
  gSym->addFlag(FLAG_CONST);                            \
  rootModule->block->insertAtTail(new DefExpr(gSym));   \
  primType->defaultValue = gSym


// This should probably be renamed since it creates primitive types, as
//  well as internal types and other types used in the generated code
void initPrimitiveTypes() {
  dtVoid                               = createInternalType ("void",     "void");

  dtBools[BOOL_SIZE_SYS]               = createPrimitiveType("bool",     "chpl_bool");
  dtInt[INT_SIZE_64]                   = createPrimitiveType("int",      "int64_t");
  dtReal[FLOAT_SIZE_64]                = createPrimitiveType("real",     "_real64");

  dtStringC                            = createPrimitiveType("c_string", "c_string" );

  dtString                             = new AggregateType(AGGREGATE_RECORD);

  gFalse                               = createSymbol(dtBools[BOOL_SIZE_SYS], "false");
  gTrue                                = createSymbol(dtBools[BOOL_SIZE_SYS], "true");

  gFalse->immediate                    = new Immediate;
  gFalse->immediate->v_bool            = false;
  gFalse->immediate->const_kind        = NUM_KIND_BOOL;
  gFalse->immediate->num_index         = BOOL_SIZE_SYS;

  gTrue->immediate                     = new Immediate;
  gTrue->immediate->v_bool             = true;
  gTrue->immediate->const_kind         = NUM_KIND_BOOL;
  gTrue->immediate->num_index          = BOOL_SIZE_SYS;

  //
  // Mark the "high water mark" for types that IPE relies on directly
  //
  if (fUseIPE == true) {
    ipeRootInit();
  }

  dtBools[BOOL_SIZE_SYS]->defaultValue = gFalse;
  dtInt[INT_SIZE_64]->defaultValue     = new_IntSymbol(0, INT_SIZE_64);
  dtReal[FLOAT_SIZE_64]->defaultValue  = new_RealSymbol("0.0", FLOAT_SIZE_64);
  dtStringC->defaultValue              = new_CStringSymbol("");

  dtBool                               = dtBools[BOOL_SIZE_SYS];

  uniqueConstantsHash.put(gFalse->immediate, gFalse);
  uniqueConstantsHash.put(gTrue->immediate,  gTrue);

  dtStringC->symbol->addFlag(FLAG_NO_CODEGEN);

  gTryToken = new VarSymbol("chpl__tryToken", dtBool);

  gTryToken->addFlag(FLAG_CONST);
  rootModule->block->insertAtTail(new DefExpr(gTryToken));

  dtNil = createInternalType ("_nilType", "_nilType");
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");

  // This type should not be visible past normalize.
  CREATE_DEFAULT_SYMBOL (dtVoid, gNoInit, "_gnoinit");

  dtUnknown = createInternalType ("_unknown", "_unknown");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_gunknown");
  gUnknown->addFlag(FLAG_TYPE_VARIABLE);

  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");

  dtValue = createInternalType("value", "_chpl_value");

  INIT_PRIM_BOOL("bool(1)", 1);
  INIT_PRIM_BOOL("bool(8)", 8);
  INIT_PRIM_BOOL("bool(16)", 16);
  INIT_PRIM_BOOL("bool(32)", 32);
  INIT_PRIM_BOOL("bool(64)", 64);

  INIT_PRIM_INT( "int(8)", 8);
  INIT_PRIM_INT( "int(16)", 16);
  INIT_PRIM_INT( "int(32)", 32);

  INIT_PRIM_UINT( "uint(8)", 8);
  INIT_PRIM_UINT( "uint(16)", 16);
  INIT_PRIM_UINT( "uint(32)", 32);
  INIT_PRIM_UINT( "uint", 64);          // default size

  INIT_PRIM_REAL( "real(32)", 32);

  INIT_PRIM_IMAG( "imag(32)", 32);
  INIT_PRIM_IMAG( "imag", 64);            // default size

  INIT_PRIM_COMPLEX( "complex(64)", 64);
  INIT_PRIM_COMPLEX( "complex", 128);       // default size

  // Could be == c_ptr(int(8)) e.g.
  // used in some runtime interfaces
  dtCVoidPtr   = createPrimitiveType("c_void_ptr", "c_void_ptr" );
  dtCVoidPtr->symbol->addFlag(FLAG_NO_CODEGEN);
  dtCVoidPtr->defaultValue = gOpaque;
  dtCFnPtr = createPrimitiveType("c_fn_ptr", "c_fn_ptr");
  dtCFnPtr->symbol->addFlag(FLAG_NO_CODEGEN);
  dtCFnPtr->defaultValue = gOpaque;
  CREATE_DEFAULT_SYMBOL(dtCVoidPtr, gCVoidPtr, "_nullVoidPtr");
  gCVoidPtr->cname = "NULL";
  gCVoidPtr->addFlag(FLAG_EXTERN);

  dtSymbol = createPrimitiveType( "symbol", "_symbol");

  dtFile = createPrimitiveType ("_file", "_cfile");
  dtFile->symbol->addFlag(FLAG_EXTERN);

  CREATE_DEFAULT_SYMBOL(dtFile, gFile, "NULL");
  gFile->addFlag(FLAG_EXTERN);

  dtOpaque = createPrimitiveType("opaque", "chpl_opaque");

  CREATE_DEFAULT_SYMBOL(dtOpaque, gOpaque, "_nullOpaque");
  gOpaque->cname = "NULL";
  gOpaque->addFlag(FLAG_EXTERN);

  dtTaskID = createPrimitiveType("chpl_taskID_t", "chpl_taskID_t");
  dtTaskID->symbol->addFlag(FLAG_NO_CODEGEN);

  CREATE_DEFAULT_SYMBOL(dtTaskID, gTaskID, "chpl_nullTaskID");

  dtSyncVarAuxFields = createPrimitiveType( "_sync_aux_t", "chpl_sync_aux_t");

  CREATE_DEFAULT_SYMBOL (dtSyncVarAuxFields, gSyncVarAuxFields, "_nullSyncVarAuxFields");
  gSyncVarAuxFields->cname = "NULL";

  dtSingleVarAuxFields = createPrimitiveType( "_single_aux_t", "chpl_single_aux_t");

  CREATE_DEFAULT_SYMBOL (dtSingleVarAuxFields, gSingleVarAuxFields, "_nullSingleVarAuxFields");
  gSingleVarAuxFields->cname = "NULL";

  dtAny = createInternalType ("_any", "_any");
  dtAny->symbol->addFlag(FLAG_GENERIC);

  dtIntegral = createInternalType ("integral", "integral");
  dtIntegral->symbol->addFlag(FLAG_GENERIC);

  dtAnyComplex = createInternalType("chpl_anycomplex", "complex");
  dtAnyComplex->symbol->addFlag(FLAG_GENERIC);

  dtNumeric = createInternalType ("numeric", "numeric");
  dtNumeric->symbol->addFlag(FLAG_GENERIC);

  dtIteratorRecord = createInternalType("_iteratorRecord", "_iteratorRecord");
  dtIteratorRecord->symbol->addFlag(FLAG_GENERIC);

  dtIteratorClass = createInternalType("_iteratorClass", "_iteratorClass");
  dtIteratorClass->symbol->addFlag(FLAG_GENERIC);

  dtMethodToken = createInternalType ("_MT", "_MT");

  CREATE_DEFAULT_SYMBOL(dtMethodToken, gMethodToken, "_mt");

  dtTypeDefaultToken = createInternalType("_TypeDefaultT", "_TypeDefaultT");

  CREATE_DEFAULT_SYMBOL(dtTypeDefaultToken, gTypeDefaultToken, "_typeDefaultT");

  dtModuleToken = createInternalType("tmodule=", "tmodule=");

  CREATE_DEFAULT_SYMBOL(dtModuleToken, gModuleToken, "module=");

  dtAnyEnumerated = createInternalType ("enumerated", "enumerated");
  dtAnyEnumerated->symbol->addFlag(FLAG_GENERIC);
}

static PrimitiveType* createPrimitiveType(const char* name, const char* cname) {
  return createType(name, cname, false);
}

static PrimitiveType* createInternalType(const char* name, const char* cname) {
  return createType(name, cname,  true);
}

static PrimitiveType*
createType(const char* name, const char* cname, bool internalType) {
  PrimitiveType* pt = new PrimitiveType(NULL, internalType);
  TypeSymbol*    ts = new TypeSymbol(name, pt);

  ts->cname = cname;

  // This prevents cleanAST() from sweeping these
  ts->addFlag(FLAG_GLOBAL_TYPE_SYMBOL);

  rootModule->block->insertAtTail(new DefExpr(ts));

  return pt;
}

static VarSymbol* createSymbol(PrimitiveType* primType, const char* name) {
  VarSymbol* retval = new VarSymbol(name, primType);

  retval->addFlag(FLAG_CONST);

  rootModule->block->insertAtTail(new DefExpr(retval));

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void initChplProgram(DefExpr* objectDef) {
  theProgram           = new ModuleSymbol("chpl__Program",
                                          MOD_INTERNAL,
                                          new BlockStmt());

  theProgram->filename = astr("<internal>");

  theProgram->addFlag(FLAG_NO_CODEGEN);

  theProgram->block->insertAtHead(objectDef);

  rootModule->block->insertAtTail(new DefExpr(theProgram));
}

// Appends a VarSymbol to the root module and gives it the bool immediate
// matching 'value'. For use in initCompilerGlobals.
static void setupBoolGlobal(VarSymbol* globalVar, bool value) {
  rootModule->block->insertAtTail(new DefExpr(globalVar));

  if (value) {
     globalVar->immediate = new Immediate;
    *globalVar->immediate = *gTrue->immediate;

  } else {
     globalVar->immediate = new Immediate;
    *globalVar->immediate = *gFalse->immediate;
  }
}

void initCompilerGlobals() {

  gBoundsChecking = new VarSymbol("boundsChecking", dtBool);
  gBoundsChecking->addFlag(FLAG_CONST);
  setupBoolGlobal(gBoundsChecking, !fNoBoundsChecks);

  gCastChecking = new VarSymbol("castChecking", dtBool);
  gCastChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gCastChecking, !fNoCastChecks);

  gDivZeroChecking = new VarSymbol("chpl_checkDivByZero", dtBool);
  gDivZeroChecking->addFlag(FLAG_PARAM);
  setupBoolGlobal(gDivZeroChecking, !fNoDivZeroChecks);

  gPrivatization = new VarSymbol("_privatization", dtBool);
  gPrivatization->addFlag(FLAG_PARAM);
  setupBoolGlobal(gPrivatization, !(fNoPrivatization || fLocal));

  gLocal = new VarSymbol("_local", dtBool);
  gLocal->addFlag(FLAG_PARAM);
  setupBoolGlobal(gLocal, fLocal);

  // defined and maintained by the runtime
  gNodeID = new VarSymbol("chpl_nodeID", dtInt[INT_SIZE_32]);
  gNodeID->addFlag(FLAG_EXTERN);
  rootModule->block->insertAtTail(new DefExpr(gNodeID));

  initForTaskIntents();
}

bool is_void_type(Type* t) {
  return t == dtVoid;
}

bool is_bool_type(Type* t) {
  return
    t == dtBools[BOOL_SIZE_1] ||
    t == dtBools[BOOL_SIZE_SYS] ||
    t == dtBools[BOOL_SIZE_8] ||
    t == dtBools[BOOL_SIZE_16] ||
    t == dtBools[BOOL_SIZE_32] ||
    t == dtBools[BOOL_SIZE_64];
}


bool is_int_type(Type *t) {
  return
    t == dtInt[INT_SIZE_32] ||
    t == dtInt[INT_SIZE_8] ||
    t == dtInt[INT_SIZE_16] ||
    t == dtInt[INT_SIZE_64];
}


bool is_uint_type(Type *t) {
  return
    t == dtUInt[INT_SIZE_32] ||
    t == dtUInt[INT_SIZE_8] ||
    t == dtUInt[INT_SIZE_16] ||
    t == dtUInt[INT_SIZE_64];
}

bool is_signed(Type *t) {
  if( is_int_type(t) ||
      is_real_type(t) ||
      is_imag_type(t) ||
      is_complex_type(t) ) return true;
  if( is_uint_type(t) ) return false;
  if( is_enum_type(t) ) {
    return is_signed(toEnumType(t)->getIntegerType());
  }
  return false;
}

bool is_real_type(Type *t) {
  return
    t == dtReal[FLOAT_SIZE_64] ||
    t == dtReal[FLOAT_SIZE_32];
}


bool is_imag_type(Type *t) {
  return
    t == dtImag[FLOAT_SIZE_64] ||
    t == dtImag[FLOAT_SIZE_32];
}


bool is_complex_type(Type *t) {
  return
    t == dtComplex[COMPLEX_SIZE_128] ||
    t == dtComplex[COMPLEX_SIZE_64];
}


bool is_enum_type(Type *t) {
  return toEnumType(t);
}


bool isLegalParamType(Type* t) {
  return (is_bool_type(t) ||
          is_int_type(t) ||
          is_uint_type(t) ||
          is_real_type(t) ||
          is_imag_type(t) ||
          is_enum_type(t) ||
          isString(t) ||
          t == dtStringC ||
          t == dtUnknown);
}

int get_width(Type *t) {
  if (t == dtBools[BOOL_SIZE_1] ||
      t == dtBools[BOOL_SIZE_SYS]) {
    return 1;
    // BLC: This is a lie, but one I'm hoping we can get away with
    // based on how this function is used
  }
  if (t == dtBools[BOOL_SIZE_8] ||
      t == dtInt[INT_SIZE_8] ||
      t == dtUInt[INT_SIZE_8])
    return 8;
  if (t == dtBools[BOOL_SIZE_16] ||
      t == dtInt[INT_SIZE_16] ||
      t == dtUInt[INT_SIZE_16])
    return 16;
  if (t == dtBools[BOOL_SIZE_32] ||
      t == dtInt[INT_SIZE_32] ||
      t == dtUInt[INT_SIZE_32] ||
      t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32])
    return 32;
  if (t == dtBools[BOOL_SIZE_64] ||
      t == dtInt[INT_SIZE_64] ||
      t == dtUInt[INT_SIZE_64] ||
      t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_64])
    return 64;
  if (t == dtComplex[COMPLEX_SIZE_128])
    return 128;
  INT_FATAL(t, "Unknown bit width");
  return 0;
}

// numbers between -2**width .. 2**width
// will fit exactly in a floating-point representation.
int get_mantissa_width(Type *t) {
  // FLOAT_SIZE_16 would have 11 bits of precision
  if (t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32] ||
      t == dtComplex[COMPLEX_SIZE_64])
    // mantissa for 32-bit float
    return 24;
  if (t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_128])
    // mantissa for 64-bit float
    return 53;
  INT_FATAL(t, "Unknown mantissa width");
  return 0;
}

int get_exponent_width(Type *t) {
  // FLOAT_SIZE_16 would have 5 bits of exponent
  if (t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32] ||
      t == dtComplex[COMPLEX_SIZE_64])
    // exponent bits for 32-bit float
    return 8;
  if (t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_128])
    // exponent bits for 64-bit float
    return 15;
  INT_FATAL(t, "Unknown exponent width");
  return 0;
}

bool isClass(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isClass();
  return false;
}

bool isClassOrNil(Type* t) {
  if (t == dtNil) return true;
  return isClass(t);
}

bool isRecord(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isRecord();
  return false;
}

bool isUnion(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isUnion();
  return false;
}

bool isReferenceType(const Type* t) {
  return t->symbol->hasFlag(FLAG_REF);
}

bool isRefCountedType(Type* t) {
  // We may eventually want to add a separate flag and provide users
  //  with an interface to declare reference counted types that will
  //  be "automatically" reference counted when needed

  // The set of reference counted types currently coincides with the set
  // of record-wrapped types, so we can reuse the flag set.
  return getRecordWrappedFlags(t->symbol).any();
}

bool isRecordWrappedType(const Type* t) {
  return getRecordWrappedFlags(t->symbol).any();
}

// Returns true if the given type is one which can be returned by one of the
// dsiNew*Dom() functions; false otherwise.
// This check is performed by looking to see if the given type derives from the
// BaseDom class.
bool isDomImplType(Type* type)
{
  return isDerivedType(type, FLAG_BASE_DOMAIN);
}

// Returns true if the given type is one which can be returned by
// dsiBuildArray() or similar function returning a "nude" array implementation;
// false otherwise.
// The test is actually performed by looking to see if the given type derives
// from the BaseArr class.
bool isArrayImplType(Type* type)
{
  return isDerivedType(type, FLAG_BASE_ARRAY);
}

bool isDistImplType(Type* type)
{
  return isDerivedType(type, FLAG_BASE_DIST);
}

static bool isDerivedType(Type* type, Flag flag)
{
  AggregateType* at     =  NULL;
  bool           retval = false;

  while ((at = toAggregateType(type)) != NULL && retval == false)
  {
    if (at->symbol->hasFlag(flag) == true)
      retval = true;
    else
      type   = at->dispatchParents.only();
  }

  return retval;
}

bool isSyncType(const Type* t) {
  return t->symbol->hasFlag(FLAG_SYNC);
}

bool isSingleType(const Type* t) {
  return t->symbol->hasFlag(FLAG_SINGLE);
}

bool isAtomicType(const Type* t) {
  return t->symbol->hasFlag(FLAG_ATOMIC_TYPE);
}

bool isRefIterType(Type* t) {
  Symbol* iteratorRecord = NULL;

  if (t->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
    AggregateType* at = toAggregateType(t);
    FnSymbol* getIterator = at->iteratorInfo->getIterator;
    iteratorRecord = getIterator->getFormal(1)->type->symbol;
  } else if (t->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    iteratorRecord = t->symbol;

  if (iteratorRecord)
    return iteratorRecord->hasFlag(FLAG_REF_ITERATOR_CLASS);
  return false;
}

bool isSubClass(Type* type, Type* baseType) {
  bool retval = false;

  if (type == baseType) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, pt, at->dispatchParents) {
      if (isSubClass(pt, baseType) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isDistClass(Type* type) {
  bool retval = false;

  if (type->symbol->hasFlag(FLAG_BASE_DIST) == true) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, pt, at->dispatchParents) {
      if (isDistClass(pt) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isDomainClass(Type* type) {
  bool retval = false;

  if (type->symbol->hasFlag(FLAG_BASE_DOMAIN) == true) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, pt, at->dispatchParents) {
      if (isDomainClass(pt) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isArrayClass(Type* type) {
  bool retval = false;

  if (type->symbol->hasFlag(FLAG_BASE_ARRAY) == true) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, t, at->dispatchParents) {
      if (isArrayClass(t) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

bool isString(Type* type) {
  bool retval = false;

  if (AggregateType* aggr = toAggregateType(type)) {
    retval = strcmp(aggr->symbol->name, "string") == 0;
  }

  return retval;
}

//
// Noakes 2016/02/29
//
// To support the merge of the string-as-rec branch we defined a
// function, isString(), which is only true of the record that was
// defined in the new implementation of String.  This predicate was
// applied in cullOverReferences and callDestructors to improve
// memory management for that particular record type.
//
// We seek to apply those routines to a wider set of record types but
// are not ready to apply them to range, tuple, and the reference-counted
// records.
//
// This shorter-term predicate, which has a slightly inelegant name, allows
// most record-like types to use the new business logic.
//
// In the longer term we plan to further broaden the cases that the new
// logic can handle and reduce the exceptions that are filtered out here.
//
//
//
// MPF    2016/09/15
// This function now includes tuples, distributions, domains, and arrays.
//
// Noakes 2017/03/02
// This function now includes range and atomics
//
bool isUserDefinedRecord(Type* type) {
  bool retval = false;

  if (AggregateType* aggr = toAggregateType(type)) {
    Symbol*     sym  = aggr->symbol;
    const char* name = sym->name;

    // Must be a record type
    if (aggr->aggregateTag != AGGREGATE_RECORD) {
      retval = false;

    // Not a RUNTIME_type
    } else if (sym->hasFlag(FLAG_RUNTIME_TYPE_VALUE) == true) {
      retval = false;

    // Not an iterator
    } else if (strncmp(name, "_ir_", 4)              ==    0) {
      retval = false;

    } else {
      retval = true;
    }
  }

  return retval;
}

Type* getNamedType(std::string name) {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if(name == ts->name || name == ts->cname) {
      return ts->type;
    }
  }

  return NULL;
}

// Do variables of the type 't' need capture for tasks?
bool needsCapture(Type* t) {
  INT_ASSERT(!isReferenceType(t)); // responsibility of the caller

  if (is_bool_type(t) ||
      is_int_type(t) ||
      is_uint_type(t) ||
      is_real_type(t) ||
      is_imag_type(t) ||
      is_complex_type(t) ||
      is_enum_type(t) ||
      t == dtStringC ||
      isClass(t) ||
      isRecord(t) ||
      isUnion(t) ||
      t == dtTaskID || // false?
      t == dtFile ||
      // TODO: Move these down to the "no" section.
      t == dtNil ||
      t == dtOpaque ||
      t->symbol->hasFlag(FLAG_EXTERN)) {
    return true;
  } else {
    // Ensure we have covered all types.
    INT_ASSERT(isRecordWrappedType(t) ||
               isSyncType(t)          ||
               isSingleType(t)        ||
               isAtomicType(t));
    return false;
  }
}

VarSymbol* resizeImmediate(VarSymbol* s, PrimitiveType* t)
{
  for( int i = 0; i < INT_SIZE_NUM; i++ ) {
    if( t == dtInt[i] ) {
      return new_IntSymbol(s->immediate->to_int(), (IF1_int_type) i);
    }
  }
  for( int i = 0; i < INT_SIZE_NUM; i++ ) {
    if( t == dtUInt[i] ) {
      return new_UIntSymbol(s->immediate->to_uint(), (IF1_int_type) i);
    }
  }
  return NULL;
}


/* After resolution, other passes can call isPOD
   in order to find out if a record type is POD.

   During resolution, one should call propagateNotPOD
   instead, so that the relevant calls can be resolved
   and POD fields can be properly handled.
 */
bool isPOD(Type* t)
{
  // things that aren't aggregate types are POD
  //   e.g. int, boolean, complex, etc
  if (!isAggregateType(t))
    return true;

  // sync/single and atomic types are not POD
  // but they should be marked with FLAG_NOT_POD
  // by propagateNotPOD in function resolution.

  // handle anything already marked
  if (t->symbol->hasFlag(FLAG_POD))
    return true;
  if (t->symbol->hasFlag(FLAG_NOT_POD))
    return false;

  // if we have not calculated POD-ness,
  // we should not be calling this function
  INT_ASSERT(false);
  return false;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool isPrimitiveScalar(Type* type) {
  bool retval = false;

  if (type == dtBools[BOOL_SIZE_8]         ||
      type == dtBools[BOOL_SIZE_16]        ||
      type == dtBools[BOOL_SIZE_32]        ||
      type == dtBools[BOOL_SIZE_64]        ||
      type == dtBools[BOOL_SIZE_SYS]       ||

      type == dtInt[INT_SIZE_8]            ||
      type == dtInt[INT_SIZE_16]           ||
      type == dtInt[INT_SIZE_32]           ||
      type == dtInt[INT_SIZE_64]           ||

      type == dtUInt[INT_SIZE_8]           ||
      type == dtUInt[INT_SIZE_16]          ||
      type == dtUInt[INT_SIZE_32]          ||
      type == dtUInt[INT_SIZE_64]          ||

      type == dtReal[FLOAT_SIZE_32]        ||
      type == dtReal[FLOAT_SIZE_64]        ||

      type == dtImag[FLOAT_SIZE_32]        ||
      type == dtImag[FLOAT_SIZE_64]) {

    retval = true;

  } else {
    retval = false;
  }

  return retval;
}

bool isNonGenericClass(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isClass()                    ==  true &&
        at->isGeneric()                  == false &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isNonGenericClassWithInitializers(Type* type) {
  bool retval = false;

  if (isNonGenericClass(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->initializerStyle == DEFINES_INITIALIZER) {
        retval = true;
      } else if (at->wantsDefaultInitializer()) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isGenericClass(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isClass()                    == true  &&
        at->isGeneric()                  == true  &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isGenericClassWithInitializers(Type* type) {
  bool retval = false;

  if (isGenericClass(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->initializerStyle == DEFINES_INITIALIZER) {
        retval = true;
      } else if (at->wantsDefaultInitializer()) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isClassWithInitializers(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isClass()                    == true  &&
        at->symbol->hasFlag(FLAG_EXTERN) == false &&
        (at->initializerStyle == DEFINES_INITIALIZER ||
         at->wantsDefaultInitializer())) {
      retval = true;
    }
  }

  return retval;
}

bool isNonGenericRecord(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isRecord()                   == true  &&
        at->isGeneric()                  == false &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isNonGenericRecordWithInitializers(Type* type) {
  bool retval = false;

  if (isNonGenericRecord(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->initializerStyle == DEFINES_INITIALIZER) {
        retval = true;
      } else if (at->wantsDefaultInitializer()) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isGenericRecord(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isRecord()                   == true  &&
        at->isGeneric()                  == true  &&
        at->symbol->hasFlag(FLAG_EXTERN) == false) {
      retval = true;
    }
  }

  return retval;
}

bool isGenericRecordWithInitializers(Type* type) {
  bool retval = false;

  if (isGenericRecord(type) == true) {
    if (AggregateType* at = toAggregateType(type)) {
      if (at->initializerStyle == DEFINES_INITIALIZER) {
        retval = true;
      } else if (at->wantsDefaultInitializer()) {
        retval = true;
      }
    }
  }

  return retval;
}

bool isRecordWithInitializers(Type* type) {
  bool retval = false;

  if (AggregateType* at = toAggregateType(type)) {
    if (at->isRecord()                   == true  &&
        at->symbol->hasFlag(FLAG_EXTERN) == false &&
        (at->initializerStyle == DEFINES_INITIALIZER ||
         at->wantsDefaultInitializer())) {
      retval = true;
    }
  }

  return retval;
}
