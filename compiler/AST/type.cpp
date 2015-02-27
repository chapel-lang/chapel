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

#define TYPE_EXTERN

#include "type.h"

#include "astutil.h"
#include "build.h"
#include "codegen.h"
#include "expr.h"
#include "files.h"
#include "intlimits.h"
#include "ipe.h"
#include "misc.h"
#include "passes.h" // for isWideString
#include "stringutil.h"
#include "symbol.h"
#include "vec.h"

#include "AstVisitor.h"


Type::Type(AstTag astTag, Symbol* init_defaultVal) :
  BaseAST(astTag),

  symbol(NULL),
  refType(NULL),
  hasGenericDefaults(false),
  defaultValue(init_defaultVal),
  defaultInitializer(NULL),
  defaultTypeConstructor(NULL),
  destructor(NULL),
  isInternalType(false),
  instantiatedFrom(NULL),
  scalarPromotionType(NULL) {
}

Type::~Type() {

}

void Type::verify() {
}

void Type::addSymbol(TypeSymbol* newsymbol) {
  symbol = newsymbol;
}


bool Type::inTree() {
  if (symbol)
    return symbol->inTree();
  else
    return false;
}


Type* Type::typeInfo() {
  return this;
}


GenRet Type::codegen() {
  if (this == dtUnknown) {
    INT_FATAL(this, "Cannot generate unknown type");
  }
  return symbol->codegen();
}

void Type::codegenDef() {
  INT_FATAL(this, "Unexpected call to Type::codegenDef");
}


void Type::codegenPrototype() { }


int Type::codegenStructure(FILE* outfile, const char* baseoffset) {
  INT_FATAL(this, "Unexpected type in codegenStructure: %s", symbol->name);
  return 0;
}


Symbol* Type::getField(const char* name, bool fatal) {
  INT_FATAL(this, "getField not called on AggregateType");
  return NULL;
}


PrimitiveType::PrimitiveType(Symbol *init, bool internalType) :
  Type(E_PrimitiveType, init)
{
  isInternalType = internalType;
  gPrimitiveTypes.add(this);
}


PrimitiveType*
PrimitiveType::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Unexpected call to PrimitiveType::copyInner");
  return this;
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

void PrimitiveType::codegenDef() {
}

int PrimitiveType::codegenStructure(FILE* outfile, const char* baseoffset) {
  if (!isInternalType) {
    Symbol* cgsym = symbol;

    fprintf(outfile, "{CHPL_TYPE_%s, %s},\n", cgsym->cname, baseoffset);
  } else
    INT_FATAL(this, "Cannot codegen an internal type");
  return 1;
}

void PrimitiveType::accept(AstVisitor* visitor) {
  visitor->visitPrimType(this);
}

EnumType::EnumType() :
  Type(E_EnumType, NULL),
  constants(), integerType(NULL)
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
  }
}


EnumType*
EnumType::copyInner(SymbolMap* map) {
  EnumType* copy = new EnumType();
  for_enums(def, this)
    copy->constants.insertAtTail(COPY_INT(def));
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumType::replaceChild");
}


void EnumType::codegenDef() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;


  if( outfile ) {
    fprintf(outfile, "typedef enum {");
    bool first = true;
    for_enums(constant, this) {
      if (!first) {
        fprintf(outfile, ", ");
      }
      fprintf(outfile, "%s", constant->sym->codegen().c.c_str());
      if (constant->init) {
        fprintf(outfile, " = %s", constant->init->codegen().c.c_str());
      } else INT_FATAL("enum init not defined");
      first = false;
    }
    fprintf(outfile, "} ");
    fprintf(outfile, "%s", symbol->codegen().c.c_str());
    fprintf(outfile, ";\n");
  } else {
#ifdef HAVE_LLVM
    // Make sure that we've computed all of the enum values..
    PrimitiveType* ty = getIntegerType();
    llvm::Type *type;

    if(!(type = info->lvt->getType(symbol->cname))) {
      type = ty->codegen().type;
      info->lvt->addGlobalType(symbol->cname, type);

      for_enums(constant, this) {
        //llvm::Constant *initConstant;

        if(constant->init == NULL) INT_FATAL(this, "no constant->init");

        VarSymbol* s = toVarSymbol(toSymExpr(constant->init)->var);
        INT_ASSERT(s);
        INT_ASSERT(s->immediate);

        VarSymbol* sizedImmediate = resizeImmediate(s, ty);

        info->lvt->addGlobalValue(constant->sym->cname,
                                  sizedImmediate->codegen());
      }
    }
#endif
  }
  return;
}

int EnumType::codegenStructure(FILE* outfile, const char* baseoffset) {
  fprintf(outfile, "{CHPL_TYPE_enum, %s},\n", baseoffset);
  return 1;
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

  // First, look for negative numbers in the enum.
  // If there are any, we have to store all
  // the values in negative numbers.
  for_enums(constant, this) {
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
        INT_ASSERT(sym && !sym->var->hasFlag(FLAG_PARAM));

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
      if( get_int( constant->init, &v ) ) {
        if( v >= 0 ) uv = v;
        else uv = 1;
      } else if( get_uint( constant->init, &uv ) ) {
        v = uv;
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
      constant->init->parentExpr = constant;
      constant->init->parentSymbol = constant->parentSymbol;
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
    if( max_v <= UINT8_MAX ) {
      num_bytes = 1;
    } else if( max_v <= UINT16_MAX ) {
      num_bytes = 2;
    } else if( max_v <= UINT32_MAX ) {
      num_bytes = 4;
    } else {
      num_bytes = 8;
    }
  }

  // Now figure out field min and max values.
  // and set et->integerType
  min = max = 0;

  if( num_bytes == 1 ) {
    if( issigned ) {
      max = INT8_MAX;
      min = INT8_MIN;
      ret = dtInt[INT_SIZE_8];
    } else {
      max = UINT8_MAX;
      ret = dtUInt[INT_SIZE_8];
    }
  } else if( num_bytes == 2 ) {
    if( issigned ) {
      max = INT16_MAX;
      min = INT16_MIN;
      ret = dtInt[INT_SIZE_16];
    } else {
      max = UINT16_MAX;
      ret = dtUInt[INT_SIZE_16];
    }
  } else if( num_bytes == 4 ) {
    if( issigned ) {
      max = INT32_MAX;
      min = INT32_MIN;
      ret = dtInt[INT_SIZE_32];
    } else {
      max = UINT32_MAX;
      ret = dtUInt[INT_SIZE_32];
    }
  } else if( num_bytes == 8 ) {
    if( issigned ) {
      max = INT64_MAX;
      min = INT64_MIN;
      ret = dtInt[INT_SIZE_64];
    } else {
      max = UINT64_MAX;
      ret = dtUInt[INT_SIZE_64];
    }
  }

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

  integerType = ret;
}

PrimitiveType* EnumType::getIntegerType() {
  if( ! integerType ) {
    sizeAndNormalize();
  }
  return integerType;
}

void EnumType::accept(AstVisitor* visitor) {
  if (visitor->enterEnumType(this) == true) {

    for_alist(next_ast, constants) {
      next_ast->accept(visitor);
    }

    visitor->exitEnumType(this);
  }
}

AggregateType::AggregateType(AggregateTag initTag) :
  Type(E_AggregateType, NULL),
  aggregateTag(initTag),
  fields(),
  inherits(),
  outer(NULL),
  doc(NULL)
{
  if (aggregateTag == AGGREGATE_CLASS) { // set defaultValue to nil to keep it
                                 // from being constructed
    defaultValue = gNil;
  }
  methods.clear();
  fields.parent = this;
  inherits.parent = this;
  gAggregateTypes.add(this);
}


AggregateType::~AggregateType() { }


void AggregateType::verify() {
  Type::verify();
  if (astTag != E_AggregateType) {
    INT_FATAL(this, "Bad AggregateType::astTag");
  }
  if (aggregateTag != AGGREGATE_CLASS &&
      aggregateTag != AGGREGATE_RECORD &&
      aggregateTag != AGGREGATE_UNION)
    INT_FATAL(this, "Bad AggregateType::aggregateTag");
  if (fields.parent != this || inherits.parent != this)
    INT_FATAL(this, "Bad AList::parent in AggregateType");
  for_alist(expr, fields) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad AggregateType::fields::parentSymbol");
  }
  for_alist(expr, inherits) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad AggregateType::inherits::parentSymbol");
  }
}


AggregateType*
AggregateType::copyInner(SymbolMap* map) {
  AggregateType* copy_type = new AggregateType(aggregateTag);
  copy_type->outer = outer;
  for_alist(expr, fields)
    copy_type->fields.insertAtTail(COPY_INT(expr));
  for_alist(expr, inherits)
    copy_type->inherits.insertAtTail(COPY_INT(expr));
  for_fields(field, copy_type) {
    if (FnSymbol* fn = toFnSymbol(field))
      copy_type->methods.add(fn);
  }
  return copy_type;
}


static void
addDeclaration(AggregateType* ct, DefExpr* def, bool tail) {
  if (def->sym->hasFlag(FLAG_REF_VAR)) {
      USR_FATAL_CONT(def, "References cannot be members of classes or records yet.");
  }
  if (FnSymbol* fn = toFnSymbol(def->sym)) {
    ct->methods.add(fn);
    if (fn->_this) {
      // get the name used in the type binding clause
      // this is the way it comes from the parser (see fn_decl_stmt_inner)
      ArgSymbol* thisArg = toArgSymbol(fn->_this);  INT_ASSERT(thisArg);
      INT_ASSERT(thisArg->type == dtUnknown);
      BlockStmt* bs = thisArg->typeExpr;  INT_ASSERT(bs && bs->length() == 1);
      Expr* firstexpr = bs->body.first();  INT_ASSERT(firstexpr);
      UnresolvedSymExpr* sym = toUnresolvedSymExpr(firstexpr); INT_ASSERT(sym);
      const char* name = sym->unresolved;
      // ... then report it to the user
      USR_FATAL_CONT(fn->_this,
         "Type binding clauses ('%s.' in this case) are not supported in "
         "declarations within a class, record or union", name);
    } else {
      fn->_this = new ArgSymbol(fn->thisTag, "this", ct);
      fn->_this->addFlag(FLAG_ARG_THIS);
      fn->insertFormalAtHead(new DefExpr(fn->_this));
      fn->insertFormalAtHead(
          new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
      fn->addFlag(FLAG_METHOD);
      fn->addFlag(FLAG_METHOD_PRIMARY);
    }
  }

  if (VarSymbol* var = toVarSymbol(def->sym)) {
    // Identify VarSymbol as class/record member.
    var->makeField();
  }

  if (def->parentSymbol || def->list)
    def->remove();
  if (tail)
    ct->fields.insertAtTail(def);
  else
    ct->fields.insertAtHead(def);
}


void AggregateType::addDeclarations(Expr* expr, bool tail) {
  if (DefExpr* def = toDefExpr(expr)) {
    addDeclaration(this, def, tail);
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    for_alist(expr, block->body) {
      addDeclarations(expr, tail);
    }
  } else {
    INT_FATAL(expr, "unexpected case");
  }
}


void AggregateType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in AggregateType::replaceChild");
}

#define CLASS_STRUCT_PREFIX "chpl_"

//
// Construct the name of the struct used in the generated code to
// represent the object definition for a AggregateType.
//
// When 'standalone'== false, we generates an identifier designed to
// be used in combination with "struct "; true generates a name that's
// been typedef'd such that the 'struct ' is not required (i.e., it
// stands alone).
//
// By example, for a Chapel class named C, we'd generate:
//
//   typedef struct <classStructName(false)> {
//     ...class members here...
//   } <classStructName(true)>;
//  typedef <classStructName(true)>* C;
//
// which in the current implementation amounts to:
//
//   typedef struct chpl_C_s {
//     ...class members here...
//   } chpl_C_object;
//  typedef struct chpl_C_s* C;
//
// For records and unions (aggregateTag != AGGREGATE_CLASS), the pointer
// version above isn't used, so the original identifier is used to
// name the typedef for the struct itself.
//
const char* AggregateType::classStructName(bool standalone) {
  if (standalone) {
    const char* basename = symbol->cname;
    if (aggregateTag == AGGREGATE_CLASS) {
      //
      // For extern classes, we've traditionally required them to be
      // named _C; we could use a different naming convention, but
      // have never agreed upon one.
      //
      if (symbol->hasFlag(FLAG_EXTERN)) {
        return astr("_", basename);
      } else {
        // keep in sync with maxCNameAddedChars
        return astr(CLASS_STRUCT_PREFIX, basename, "_object");
      }
    } else
      return basename;
  } else {
    // keep in sync with maxCNameAddedChars
    return astr(CLASS_STRUCT_PREFIX, symbol->cname, "_s");
  }
}

GenRet AggregateType::codegenClassStructType()
{
  GenInfo* info = gGenInfo;
  GenRet ret;
  if(aggregateTag == AGGREGATE_CLASS) {
    if( info->cfile ) {
      ret.c = std::string(classStructName(true));
    } else {
#ifdef HAVE_LLVM
      ret.type = info->lvt->getType(classStructName(true));
#endif
    }
  } else {
    INT_FATAL("codegenClassStructType on non-class");
    ret = codegen();
  }
  return ret;
}

void AggregateType::codegenDef() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;

#ifdef HAVE_LLVM
  llvm::Type *type = NULL;
#endif

  if (symbol->hasFlag(FLAG_STAR_TUPLE)) {
    if( outfile ) {
      fprintf(outfile, "typedef ");
      fprintf(outfile, "%s", getField("x1")->type->codegen().c.c_str());
      fprintf(outfile, " %s", symbol->codegen().c.c_str());
      fprintf(outfile, "[%d];\n\n", fields.length);
      return;
    } else {
#ifdef HAVE_LLVM
      llvm::Type *elementType = getField("x1")->type->codegen().type;
      type = llvm::ArrayType::get(elementType, fields.length);
#endif
    }
  } else if (symbol->hasFlag(FLAG_REF)) {
    TypeSymbol* base = getField(1)->type->symbol;
    const char* baseType = base->cname;
    if( outfile ) {
      fprintf(outfile, "typedef %s *%s;\n", baseType, symbol->cname);
      return;
    } else {
#ifdef HAVE_LLVM
      llvm::Type* llBaseType;
      if( base->typeInfo() == dtVoid ) {
        llBaseType = llvm::IntegerType::getInt8Ty(info->module->getContext());
      } else {
        llBaseType = base->codegen().type;
      }
      INT_ASSERT(llBaseType);
      type = llvm::PointerType::getUnqual(llBaseType);
#endif
    }
  } else if (symbol->hasFlag(FLAG_DATA_CLASS)) {
    TypeSymbol* base = getDataClassType(symbol);
    const char* baseType = base->cname;
    if( outfile ) {
      fprintf(outfile, "typedef %s *%s;\n", baseType, symbol->cname);
    } else {
#ifdef HAVE_LLVM
      llvm::Type* llBaseType;
      if( base->typeInfo() == dtVoid ) {
        llBaseType = llvm::IntegerType::getInt8Ty(info->module->getContext());
      } else {
        llBaseType = base->codegen().type;
      }
      INT_ASSERT(llBaseType);
      type = llvm::PointerType::getUnqual(llBaseType);
#endif
    }
  } else {
    if( outfile ) {
      if( symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS) &&
          (! isWideString(this)) &&
          (! widePointersStruct ) ) {
        // Reach this branch when generating a wide/wide class as a
        // global pointer!
        Type* baseType = this->getField("addr")->type;
        GenRet c = baseType;

        // could use __attribute__(address_space(101))
        //  if we wanted to emit packed pointers in a different AS with clang.
        fprintf(outfile, "typedef %s * %s;\n",
            baseType->symbol->codegen().c.c_str(),
            this->classStructName(true));
      } else {
        fprintf(outfile, "typedef struct %s", this->classStructName(false));
        if (aggregateTag == AGGREGATE_CLASS && dispatchParents.n > 0) {
          /* Add a comment to class definitions listing super classes */
          bool first = true;
          fprintf(outfile, " /* : ");
          forv_Vec(Type, parent, dispatchParents) {
            if (parent) {
              if (!first) {
                fprintf(outfile, ", ");
              }
              fprintf(outfile, "%s", parent->symbol->codegen().c.c_str());
              first = false;
            }
          }
          fprintf(outfile, " */");
        }
        fprintf(outfile, " {\n");
        if (symbol->hasFlag(FLAG_OBJECT_CLASS) && aggregateTag == AGGREGATE_CLASS) {
          fprintf(outfile, "chpl__class_id chpl__cid;\n");
        } else if (aggregateTag == AGGREGATE_UNION) {
          fprintf(outfile, "int64_t _uid;\n");
          if (this->fields.length != 0)
            fprintf(outfile, "union {\n");
        } else if (this->fields.length == 0) {
          fprintf(outfile, "int dummyFieldToAvoidWarning;\n");
        }

        if (this->fields.length != 0) {
          for_fields(field, this) {
            field->codegenDef();
          }
        }
        flushStatements();

        if (aggregateTag == AGGREGATE_UNION) {
          if (this->fields.length != 0)
            fprintf(outfile, "} _u;\n");
        }
        fprintf(outfile, "} %s;\n\n", this->classStructName(true));
      }
    } else {
#ifdef HAVE_LLVM
      int paramID = 0;
      std::vector<llvm::Type*> params;

      if ((symbol->hasFlag(FLAG_OBJECT_CLASS) && aggregateTag == AGGREGATE_CLASS)) {
        llvm::Type* cidType = info->lvt->getType("chpl__class_id");
        INT_ASSERT(cidType);
        params.push_back(cidType);
        GEPMap.insert(std::pair<std::string, int>("chpl__cid", paramID++));
      } else if(aggregateTag == AGGREGATE_UNION) {
        params.push_back(llvm::Type::getInt64Ty(info->module->getContext()));
        GEPMap.insert(std::pair<std::string, int>("_uid", paramID++));
      }

      if(aggregateTag == AGGREGATE_UNION) {

        llvm::Type *largestType = NULL;
        uint64_t largestSize = 0;

        for_fields(field, this) {
          llvm::Type* fieldType = field->type->symbol->codegen().type;
          INT_ASSERT(fieldType);
          uint64_t fieldSize = info->targetData->getTypeStoreSize(fieldType);

          if(fieldSize > largestSize) {
            largestType = fieldType;
            largestSize = fieldSize;
          }

          GEPMap.insert(std::pair<std::string, int>(field->cname, paramID));
          GEPMap.insert(std::pair<std::string, int>(
                std::string("_u.") + field->cname, paramID));
        }

        llvm::StructType * st;
        // handle an empty union.
        if( largestType ) st = llvm::StructType::get(largestType, NULL);
        else st = llvm::StructType::get(info->module->getContext());
        params.push_back(st);
        GEPMap.insert(std::pair<std::string, int>("_u", paramID++));
      } else {
        if (this->fields.length == 0) {
          // add dummyFieldToAvoidWarning which also
          // stops us from doing 0-byte memory allocation
          // (comes up with ioNewline and --no-local RA)
          // TODO - don't ever allocate 0-byte structures
          params.push_back(llvm::Type::getInt32Ty(info->llvmContext));
        }
        for_fields(field, this) {
          llvm::Type* fieldType = field->type->symbol->codegen().type;
          AggregateType* ct = toAggregateType(field->type);
          if(ct && field->hasFlag(FLAG_SUPER_CLASS))
            fieldType = info->lvt->getType(ct->classStructName(true));
          INT_ASSERT(fieldType);
          params.push_back(fieldType);
          GEPMap.insert(std::pair<std::string, int>(field->cname, paramID++));
        }
      }

      // Is it a class or a record?
      // if it's a record, we make the new type now.
      // if it's a class, we update the existing type.
      if( symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS) &&
          (! isWideString(this)) &&
          (! widePointersStruct ) ) {
        // Reach this branch when generating a wide/wide class as a
        // global pointer!
        unsigned globalAddressSpace = 0;

        if( fLLVMWideOpt )
          globalAddressSpace = info->globalToWideInfo.globalSpace;

        Type* baseType = this->getField("addr")->type;
        llvm::Type* llBaseType = baseType->symbol->codegen().type;
        INT_ASSERT(llBaseType);
        llvm::Type *globalPtrTy = NULL;

        // Remove one level of indirection since the addr field
        // of a wide pointer is always a local address.
        llBaseType = llBaseType->getPointerElementType();
        INT_ASSERT(llBaseType);

        globalPtrTy = llvm::PointerType::get(llBaseType, globalAddressSpace);
        type = globalPtrTy; // set to use alternative address space ptr

        if( fLLVMWideOpt ) {
          // These are here so that the types are generated during codegen..
          if( ! info->globalToWideInfo.localeIdType ) {
            Type* localeType = LOCALE_ID_TYPE;
            info->globalToWideInfo.localeIdType = localeType->codegen().type;
          }
          if( ! info->globalToWideInfo.nodeIdType ) {
            Type* nodeType = NODE_ID_TYPE;
            info->globalToWideInfo.nodeIdType = nodeType->codegen().type;
          }
          // populateFunctions
          populateFunctionsForGlobalType(info->module, &info->globalToWideInfo, globalPtrTy);
        }
      } else {
        // Normal (wide or struct) code path.
        //
        // need to define _cname (for AGGREGATE_CLASS) or cname
        if(aggregateTag == AGGREGATE_CLASS) {
          const char* struct_name = this->classStructName(true);
          type = info->lvt->getType(struct_name);
          INT_ASSERT(type);
        } else {
          type = llvm::StructType::create(info->module->getContext(),
                                          symbol->cname);
        }


        llvm::StructType* stype = llvm::cast<llvm::StructType>(type);
        stype->setBody(params);

        if (aggregateTag == AGGREGATE_CLASS) {
          type = stype->getPointerTo();
        }
      }

#endif
    }
  }

  if( !outfile ) {
#ifdef HAVE_LLVM
    if( ! this->symbol->llvmType ) {
      info->lvt->addGlobalType(this->symbol->cname, type);
      this->symbol->llvmType = type;
    }
#endif
  }
}


void AggregateType::codegenPrototype() {
  GenInfo* info = gGenInfo;
  // Only generates prototypes for AGGREGATE_CLASS (ie a Chapel class,
  // not a record or wide pointer)
  if (aggregateTag == AGGREGATE_CLASS) {
    if( info->cfile ) {
      fprintf(info->cfile, "typedef struct %s* %s;\n",
              this->classStructName(false), symbol->cname);
    } else {
#ifdef HAVE_LLVM
      // ie _AggregateType
      const char* struct_name = this->classStructName(true);

      llvm::StructType* st;
      st = llvm::StructType::create(info->module->getContext(), struct_name);
      info->lvt->addGlobalType(struct_name, st);

      llvm::PointerType* pt = llvm::PointerType::getUnqual(st);
      info->lvt->addGlobalType(symbol->cname, pt);
      symbol->llvmType = pt;
#endif
    }
  }
  return;
}

int AggregateType::codegenStructure(FILE* outfile, const char* baseoffset) {
  switch (aggregateTag) {
  case AGGREGATE_CLASS:
    fprintf(outfile, "{CHPL_TYPE_CLASS_REFERENCE, %s},\n", baseoffset);
    return 1;
  case AGGREGATE_RECORD:
    return codegenFieldStructure(outfile, true, baseoffset);
  case AGGREGATE_UNION:
    INT_FATAL(this, "Don't know how to codegenStructure for unions yet");
    return 0;
  default:
    INT_FATAL(this, "Unexpected case in AggregateType::codegenStructure");
    return 0;
  }
}

void AggregateType::accept(AstVisitor* visitor) {
  if (visitor->enterAggrType(this) == true) {

    for_alist(next_ast, fields) {
      next_ast->accept(visitor);
    }

    for_alist(next_ast, inherits) {
      next_ast->accept(visitor);
    }

    visitor->exitAggrType(this);
  }
}

// BLC: I'm not understanding why special cases would need to be called
// out here
static const char* genUnderscore(Symbol* sym) {
  AggregateType* classtype = toAggregateType(sym->type);
  if (classtype && classtype->isClass() &&
      !sym->hasFlag(FLAG_REF)) {
    return "_";
  } else {
    return "";
  }
}


static const char* genChplTypeEnumString(TypeSymbol* typesym) {
  return astr("chpl_rt_type_id_", genUnderscore(typesym), typesym->cname);
}


static const char* genSizeofStr(TypeSymbol* typesym) {
  return astr("sizeof(", genUnderscore(typesym), typesym->cname, ")");
}


static const char* genNewBaseOffsetString(TypeSymbol* typesym, int fieldnum,
                                          const char* baseoffset, Symbol* field,
                                          AggregateType* classtype) {
  if (classtype->symbol->hasFlag(FLAG_STAR_TUPLE)) {
    char fieldnumstr[64];

    sprintf(fieldnumstr, "%d", fieldnum);

    return astr(baseoffset,
                " + (",
                fieldnumstr, "* ",
                genSizeofStr(field->type->symbol),
                ")");
  } else {
    return astr(baseoffset,
                " + offsetof(",
                genUnderscore(classtype->symbol),
                classtype->symbol->cname,
                ", ",
                classtype->isUnion() ? "_u." : "",
                field->cname, ")");
  }
}

int AggregateType::codegenFieldStructure(FILE*       outfile,
                                         bool        nested,
                                         const char* baseoffset) {
  // Handle ref types as pointers
  if (symbol->hasFlag(FLAG_REF)) {
    fprintf(outfile, "{CHPL_TYPE_CLASS_REFERENCE, 0},\n");
    fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
    return 1;
  }

  int totfields = 0;
  int locfieldnum = 0;

  for_fields(field, this) {
    const char* newbaseoffset = genNewBaseOffsetString(symbol,
                                                       locfieldnum,
                                                       baseoffset,
                                                       field,
                                                       this);
    int         numfields = field->type->codegenStructure(outfile, newbaseoffset);

    fprintf(outfile, " /* %s */\n", field->name);
    totfields += numfields;
    locfieldnum++;
  }
  if (!nested) {
    fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
    totfields += 1;
  }
  return totfields;
}

#ifdef HAVE_LLVM
extern int getCRecordMemberGEP(const char* typeName, const char* fieldName);
#endif

int AggregateType::getMemberGEP(const char *name) {
#ifdef HAVE_LLVM
  if( symbol->hasFlag(FLAG_EXTERN) ) {
    // We will cache the info in the local GEP map.
    std::map<std::string, int>::const_iterator GEPIdx = GEPMap.find(name);
    if(GEPIdx != GEPMap.end()) {
      return GEPIdx->second;
    }
    int ret = getCRecordMemberGEP(symbol->cname, name);
    GEPMap.insert(std::pair<std::string,int>(name, ret));
    return ret;
  } else {
    Vec<Type*> next, current;
    Vec<Type*>* next_p = &next, *current_p = &current;
    current_p->set_add(this);

    while (current_p->n != 0) {
      forv_Vec(Type, t, *current_p) {
        std::map<std::string,int>::const_iterator GEPIdx = t->GEPMap.find(name);
        if(GEPIdx != t->GEPMap.end()) {
          return GEPIdx->second;
        }

        forv_Vec(Type, parent, t->dispatchParents) {
          if (parent)
            next_p->set_add(parent);
        }
      }
      Vec<Type*>* temp = next_p;
      next_p = current_p;
      current_p = temp;
      next_p->clear();
    }

    const char *className = "<no name>";
    if (this->symbol) {
      className = this->symbol->name;
    }
    INT_FATAL(this, "no field '%s' in class '%s' in getField()",
              name, className);
  }
#endif
  return -1;
}


int AggregateType::getFieldPosition(const char* name, bool fatal) {
  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;

  current_p->set_add(this);

  int fieldPos = 0;

  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (AggregateType* ct = toAggregateType(t)) {
        for_fields(sym, ct) {
          if (!strcmp(sym->name, name)) {
            return fieldPos;
          }

          fieldPos++;
        }
        forv_Vec(Type, parent, ct->dispatchParents) {
          if (parent)
            next_p->set_add(parent);
        }
      }
    }
    Vec<Type*>* temp = next_p;
    next_p = current_p;
    current_p = temp;
    next_p->clear();
  }

  if (fatal) {
    const char *className = "<no name>";
    if (this->symbol) { // this is always true?
      className = this->symbol->name;
    }
    // TODO: report as a user error in certain cases
    INT_FATAL(this,
              "no field '%s' in class '%s' in getField()",
              name,
              className);
  }

  return -1;
}


Symbol* AggregateType::getField(const char* name, bool fatal) {
  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;
  current_p->set_add(this);
  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (AggregateType* ct = toAggregateType(t)) {
        for_fields(sym, ct) {
          if (!strcmp(sym->name, name))
            return sym;
        }
        forv_Vec(Type, parent, ct->dispatchParents) {
          if (parent)
            next_p->set_add(parent);
        }
      }
    }
    Vec<Type*>* temp = next_p;
    next_p = current_p;
    current_p = temp;
    next_p->clear();
  }
  if (fatal) {
    const char *className = "<no name>";
    if (this->symbol) { // this is always true?
      className = this->symbol->name;
    }
    // TODO: report as a user error in certain cases
    INT_FATAL(this, "no field '%s' in class '%s' in getField()",
              name, className);
  }
  return NULL;
}


Symbol* AggregateType::getField(int i) {
  return toDefExpr(fields.get(i))->sym;
}


void AggregateType::printDocs(std::ostream *file, unsigned int tabs) {
  // TODO: Include unions... (thomasvandoren, 2015-02-25)
  if (this->symbol->hasFlag(FLAG_NO_DOC) || this->isUnion()) {
    return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();
  *file << this->symbol->name;
  *file << this->docsSuperClass();
  *file << std::endl;

  // In rst mode, ensure there is an empty line between the class/record
  // signature and its description or the next directive.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->doc != NULL) {
    this->printDocsDescription(this->doc, file, tabs + 1);
    *file << std::endl;

    // In rst mode, ensure there is an empty line between the class/record
    // description and the next directive.
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


/*
 * Returns super class string for documentation. If super class exists, returns
 * ": <super class name>".
 */
std::string AggregateType::docsSuperClass() {
  if (this->inherits.length > 0) {
    std::vector<std::string> superClassNames;

    for_alist(expr, this->inherits) {
      if (UnresolvedSymExpr* use = toUnresolvedSymExpr(expr)) {
        superClassNames.push_back(use->unresolved);
      } else {
        INT_FATAL(expr, "Expected UnresolvedSymExpr for all member of inherits alist.");
      }
    }

    if (superClassNames.empty()) {
      return "";
    }

    // If there are super classes, join them into a single comma delimited
    // string prefixed with a colon.
    std::string superClasses = " : " + superClassNames.front();
    for (unsigned int i = 1; i < superClassNames.size(); i++) {
      superClasses += ", " + superClassNames.at(i);
    }
    return superClasses;
  } else {
    return "";
  }
}


std::string AggregateType::docsDirective() {
  if (fDocsTextOnly) {
    if (this->isClass()) {
      return "Class: ";
    } else if (this->isRecord()) {
      return "Record: ";
    }
  } else {
    if (this->isClass()) {
      return ".. class:: ";
    } else if (this->isRecord()) {
      return ".. record:: ";
    }
  }
  return "";
}


void initRootModule() {
  rootModule           = new ModuleSymbol("_root", MOD_INTERNAL, new BlockStmt());
  rootModule->filename = astr("<internal>");
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

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
  dtReal[FLOAT_SIZE_ ## width]->defaultValue = new_RealSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)

#define INIT_PRIM_IMAG( name, width)                                            \
  dtImag[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_imag" #width);    \
  dtImag[FLOAT_SIZE_ ## width]->defaultValue = new_ImagSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)

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
  dtVoid                               = createInternalType ("void", "void");

  dtBools[BOOL_SIZE_SYS]               = createPrimitiveType("bool", "chpl_bool");
  dtInt[INT_SIZE_64]                   = createPrimitiveType("int",  "int64_t");
  dtReal[FLOAT_SIZE_64]                = createPrimitiveType("real", "_real64");

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
  dtReal[FLOAT_SIZE_64]->defaultValue  = new_RealSymbol("0.0", 0.0, FLOAT_SIZE_64);

  dtBool                               = dtBools[BOOL_SIZE_SYS];

  uniqueConstantsHash.put(gFalse->immediate, gFalse);
  uniqueConstantsHash.put(gTrue->immediate,  gTrue);

  gTryToken = new VarSymbol("chpl__tryToken", dtBool);

  gTryToken->addFlag(FLAG_CONST);
  rootModule->block->insertAtTail(new DefExpr(gTryToken));

  //
  // IPE tries to run without the rest of the types
  //
  if (fUseIPE == true) {
    return;
  }

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

  dtStringC = createPrimitiveType( "c_string", "c_string" );
  dtStringC->defaultValue = new_StringSymbol("");
  dtStringC->symbol->addFlag(FLAG_NO_CODEGEN);

  dtStringCopy = createPrimitiveType( "c_string_copy", "c_string_copy" );
  dtStringCopy->defaultValue = gOpaque;
  dtStringCopy->symbol->addFlag(FLAG_NO_CODEGEN);

  CREATE_DEFAULT_SYMBOL(dtStringCopy, gStringCopy, "_nullString");
  gStringCopy->cname = "NULL";
  gStringCopy->addFlag(FLAG_EXTERN);

  dtString = createPrimitiveType( "string", "chpl_string");
  dtString->defaultValue = NULL;

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

  dtTaskList = createPrimitiveType( "_task_list", "chpl_task_list_p");
  dtTaskList->symbol->addFlag(FLAG_EXTERN);

  CREATE_DEFAULT_SYMBOL (dtTaskList, gTaskList, "_nullTaskList");
  gTaskList->cname = "NULL";

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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

DefExpr* defineObjectClass() {
  DefExpr* retval = 0;

  // The base object class looks like this:
  //
  //   class object {
  //     chpl__class_id chpl__cid;
  //   }
  //
  // chpl__class_id is an int32_t field identifying the classes
  //  in the program.  We never create the actual field within the
  //  IR (it is directly generated in the C code).  It might
  //  be the right thing to do, so I made an attempt at adding the
  //  field.  Unfortunately, we would need some significant changes
  //  throughout compilation, and it seemed to me that the it might result
  //  in possibly more special case code.
  //
  dtObject = new AggregateType(AGGREGATE_CLASS);

  retval   = buildClassDefExpr("object",
                               dtObject,
                               NULL,
                               new BlockStmt(),
                               FLAG_UNKNOWN,
                               NULL);

  retval->sym->addFlag(FLAG_OBJECT_CLASS);
  retval->sym->addFlag(FLAG_GLOBAL_TYPE_SYMBOL); // Prevents removal in pruneResovedTree().
  retval->sym->addFlag(FLAG_NO_OBJECT);

  return retval;
}

void initChplProgram(DefExpr* objectDef) {
  CallExpr* base = 0;
  CallExpr* std  = 0;

  theProgram           = new ModuleSymbol("chpl__Program", MOD_INTERNAL, new BlockStmt());
  theProgram->filename = astr("<internal>");

  theProgram->addFlag(FLAG_NO_USE_CHAPELSTANDARD);
  theProgram->addFlag(FLAG_NO_CODEGEN);

  base = new CallExpr(PRIM_USE, new UnresolvedSymExpr("ChapelBase"));
  std  = new CallExpr(PRIM_USE, new UnresolvedSymExpr("ChapelStandard"));

  theProgram->block->insertAtTail(base);

  // it may be better to add the following use after parsing
  // to simplify insertion of module guard sync var defs
  theProgram->block->insertAtTail(std);

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

bool is_string_type(Type *t) {
  return t == dtString;
}

int get_width(Type *t) {
  if (t == dtBools[BOOL_SIZE_SYS]) {
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


bool isClass(Type* t) {
  if (AggregateType* ct = toAggregateType(t))
    return ct->isClass();
  return false;
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

bool isReferenceType(Type* t) {
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

bool isRecordWrappedType(Type* t) {
  return getRecordWrappedFlags(t->symbol).any();
}

// Returns true if the given type is one which can be returned by one of the
// dsiNew*Dom() functions; false otherwise.
// This check is performed by looking to see if the given type derives from the
// BaseDom class.
 bool isDomImplType(Type* type)
{
  while (AggregateType* at = toAggregateType(type))
  {
    if (at->symbol->hasFlag(FLAG_BASE_DOMAIN))
      return true;
    // Lazily assume single-inheritance.
    type = at->dispatchParents.only();
  }
  return false;
}

// Returns true if the given type is one which can be returned by
// dsiBuildArray() or similar function returning a "nude" array implementation;
// false otherwise.
// The test is actually performed by looking to see if the given type derives
// from the BaseArr class.
 bool isArrayImplType(Type* type)
{
  while (AggregateType* at = toAggregateType(type))
  {
    if (at->symbol->hasFlag(FLAG_BASE_ARRAY))
      return true;
    // Lazily assume single-inheritance.
    type = at->dispatchParents.only();
  }
  return false;
}

bool isDistImplType(Type* type)
{
  while (AggregateType* at = toAggregateType(type))
  {
    if (at->symbol->hasFlag(FLAG_BASE_DIST))
      return true;
    // Lazily assume single-inheritance.
    type = at->dispatchParents.only();
  }
  return false;
}

bool isSyncType(Type* t) {
  return getSyncFlags(t->symbol).any();
}

bool isAtomicType(Type* t) {
  return t->symbol->hasFlag(FLAG_ATOMIC_TYPE);
}

bool isSubClass(Type* type, Type* baseType)
{
  if (type == baseType)
    return true;

  forv_Vec(Type, pt, type->dispatchParents)
    if (isSubClass(pt, baseType))
      return true;

  return false;
}

bool
isDistClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_DIST))
    return true;
  forv_Vec(Type, pt, type->dispatchParents)
    if (isDistClass(pt))
      return true;
  return false;
}

bool
isDomainClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_DOMAIN))
    return true;
  forv_Vec(Type, pt, type->dispatchParents)
    if (isDomainClass(pt))
      return true;
  return false;
}

bool
isArrayClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_ARRAY))
    return true;
  forv_Vec(Type, t, type->dispatchParents)
    if (isArrayClass(t))
      return true;
  return false;
}

static Vec<TypeSymbol*> typesToStructurallyCodegen;
static Vec<TypeSymbol*> typesToStructurallyCodegenList;

void registerTypeToStructurallyCodegen(TypeSymbol* type) {
  //  printf("registering chpl_rt_type_id_%s\n", type->cname);
  if (!typesToStructurallyCodegen.set_in(type)) {
    typesToStructurallyCodegenList.add(type);
    typesToStructurallyCodegen.set_add(type);
  }
}

GenRet genTypeStructureIndex(TypeSymbol* typesym) {
  GenInfo* info = gGenInfo;
  GenRet ret;
  if (fHeterogeneous) {
    // strings are special
    if (toPrimitiveType(typesym) == dtString) {
      if( info->cfile )
        ret.c = std::string("-") + typesym->cname;
      else
        INT_FATAL("TODO: genTypeStructureIndex llvm strings");
    } else {
      if( info->cfile )
        ret.c = genChplTypeEnumString(typesym);
      else {
#ifdef HAVE_LLVM
        ret = info->lvt->getValue(genChplTypeEnumString(typesym));
#endif
      }
    }
  } else {
    if( info->cfile )
      ret.c = "-1";
    else {
#ifdef HAVE_LLVM
      ret.val = llvm::ConstantInt::get(
          llvm::Type::getInt32Ty(info->module->getContext()), -1, true);
#endif
    }
  }
  ret.chplType = dtInt[INT_SIZE_32];
  return ret;
}

// Compare the cnames of different types alphabetically
static int compareCnames(const void* v1, const void* v2) {
  int retval;
  TypeSymbol* t1 = *(TypeSymbol* const *)v1;
  TypeSymbol* t2 = *(TypeSymbol* const *)v2;
  retval = strcmp(t1->cname, t2->cname);
  if (retval > 0)
    return 1;
  else if (retval < 0)
    return -1;
  else
    return 0;
}

#define TYPE_STRUCTURE_FILE "_type_structure.c"
static int maxFieldsPerType = 0;

void codegenTypeStructures(FILE* hdrfile) {
  fileinfo typeStructFile;
  openCFile(&typeStructFile, TYPE_STRUCTURE_FILE);
  FILE* outfile = typeStructFile.fptr;

  fprintf(outfile, "typedef enum {\n");
  forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
    fprintf(outfile, "%s,\n", genChplTypeEnumString(typesym));
  }
  fprintf(outfile, "chpl_num_rt_type_ids\n");
  fprintf(outfile, "} chpl_rt_types;\n\n");

  fprintf(outfile,
          "chpl_fieldType chpl_structType[][CHPL_MAX_FIELDS_PER_TYPE] = {\n");

  qsort(typesToStructurallyCodegenList.v,
        typesToStructurallyCodegenList.n,
        sizeof(typesToStructurallyCodegenList.v[0]),
        compareCnames);

  int num = 0;

  forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
    if (num) {
      fprintf(outfile, ",\n");
    }
    fprintf(outfile, "/* %s (%s) */\n", typesym->name, typesym->cname);
    fprintf(outfile, "{\n");
    if (AggregateType* classtype = toAggregateType(typesym->type)) {
      int numfields = classtype->codegenFieldStructure(outfile, false, "0");
      if (numfields > maxFieldsPerType) {
        maxFieldsPerType = numfields;
      }
    } else {
      typesym->type->codegenStructure(outfile, "0");
      fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
    }
    fprintf(outfile, "}");
    num++;
  }
  fprintf(outfile, "};\n\n");

  fprintf(outfile, "size_t chpl_sizeType[] = {\n");
  num = 0;
  forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
    if (num) {
      fprintf(outfile, ",\n");
    }
    fprintf(outfile, "%s", genSizeofStr(typesym));
    num++;
  }
  fprintf(outfile, "\n};\n\n");

  fprintf(outfile, "chplType chpl_getFieldType(int typeNum, int fieldNum) {\n");
  fprintf(outfile, "return chpl_structType[typeNum][fieldNum].type;\n");
  fprintf(outfile, "}\n\n");

  fprintf(outfile, "size_t chpl_getFieldOffset(int typeNum, int fieldNum) {\n");
  fprintf(outfile, "return chpl_structType[typeNum][fieldNum].offset;\n");
  fprintf(outfile, "}\n\n");

  fprintf(outfile, "size_t chpl_getFieldSize(int typeNum) {\n");
  fprintf(outfile, "return chpl_sizeType[typeNum];\n");
  fprintf(outfile, "}\n\n");
  closeCFile(&typeStructFile);
  fprintf(hdrfile, "#define CHPL_MAX_FIELDS_PER_TYPE %d\n", maxFieldsPerType);
  fprintf(hdrfile, "const int chpl_max_fields_per_type = %d;\n",
                   maxFieldsPerType);
}


void codegenTypeStructureInclude(FILE* outfile) {
  fprintf(outfile, "#include \"" TYPE_STRUCTURE_FILE "\"\n");
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
      is_string_type(t) ||
      t == dtStringC ||
      isClass(t) ||
      isRecord(t) ||
      isUnion(t) ||
      t == dtTaskID || // false?
      t == dtFile ||
      t == dtTaskList ||
      // TODO: Move these down to the "no" section.
      t == dtNil ||
      t == dtOpaque ||
      t->symbol->hasFlag(FLAG_EXTERN)) {
    return true;
  } else {
    // Ensure we have covered all types.
    INT_ASSERT(isRecordWrappedType(t) ||  // domain, array, or distribution
               isSyncType(t) ||
               isAtomicType(t));
    return false;
  }
}

VarSymbol* resizeImmediate(VarSymbol* s, PrimitiveType* t)
{
  for( int i = 0; i < INT_SIZE_NUM; i++ ) {
    if( t == dtInt[i] ) {
      return new_IntSymbol(s->immediate->int_value(), (IF1_int_type) i);
    }
  }
  for( int i = 0; i < INT_SIZE_NUM; i++ ) {
    if( t == dtUInt[i] ) {
      return new_UIntSymbol(s->immediate->uint_value(), (IF1_int_type) i);
    }
  }
  return NULL;
}

