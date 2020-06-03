/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "type.h"

#include "AstToText.h"
#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "clangUtil.h"
#include "codegen.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "intlimits.h"
#include "iterator.h"
#include "LayeredValueTable.h"
#include "llvmVer.h"
#include "misc.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"
#include "vec.h"
#include "wellknown.h"

#ifdef HAVE_LLVM
#include "llvm/IR/Module.h"
#endif


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


void PrimitiveType::codegenDef() {
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
      }
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

      // Convert enums to constants with the user-specified immediate,
      // sized appropriately, when it exists.  When it doesn't, give
      // it the semi-arbitrary 0-based ordinal value (similar to what
      // the C back-end would do itself).  Note that once some enum
      // has a non-NULL constant->init, all subsequent ones should as
      // well.
      int order = 0;
      for_enums(constant, this) {
        //llvm::Constant *initConstant;

        VarSymbol* s;
        if (constant->init) {
          s = toVarSymbol(toSymExpr(constant->init)->symbol());
        } else {
          s = new_IntSymbol(order, INT_SIZE_64);
        }
        INT_ASSERT(s);
        INT_ASSERT(s->immediate);

        VarSymbol* sizedImmediate = resizeImmediate(s, ty);

        info->lvt->addGlobalValue(constant->sym->cname,
                                  sizedImmediate->codegen());
        order++;
      }
    }
#endif
  }
  return;
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
      // keep in sync with maxCNameAddedChars
      return astr(CLASS_STRUCT_PREFIX, basename, "_object");
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
      fprintf(outfile, "%s", getField("x0")->type->codegen().c.c_str());
      fprintf(outfile, " %s", symbol->codegen().c.c_str());
      fprintf(outfile, "[%d];\n\n", fields.length);
      return;
    } else {
#ifdef HAVE_LLVM
      llvm::Type *elementType = getField("x0")->type->codegen().type;
      type = llvm::ArrayType::get(elementType, fields.length);
#endif
    }
  } else if (symbol->hasFlag(FLAG_C_ARRAY)) {
    Type* eltType = cArrayElementType();
    int64_t sizeInt = cArrayLength();
    TypeSymbol* eltTS = eltType->symbol;
    const char* eltTypeCName = eltTS->cname;
    if( outfile ) {
      fprintf(outfile, "typedef ");
      fprintf(outfile, "%s", eltTypeCName);
      fprintf(outfile, " %s", symbol->codegen().c.c_str());
      fprintf(outfile, "[%ld];\n\n", (long int) sizeInt);
      return;
    } else {
#ifdef HAVE_LLVM
      llvm::Type *elementType = eltTS->type->codegen().type;
      type = llvm::ArrayType::get(elementType, sizeInt);
#endif
    }

  } else if (symbol->hasFlag(FLAG_REF)) {
    TypeSymbol* base = getField(1)->type->symbol;
    const char* baseType = base->cname;
    if( outfile ) {
      if (base->typeInfo() != dtNothing) {
        fprintf(outfile, "typedef %s *%s;\n", baseType, symbol->cname);
      }
      return;
    } else {
#ifdef HAVE_LLVM
      llvm::Type* llBaseType;
      if( base->typeInfo() == dtNothing ) {
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
      if( base->typeInfo() == dtNothing ) {
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
      fprintf(outfile, "typedef struct %s", this->classStructName(false));
      if (aggregateTag == AGGREGATE_CLASS && dispatchParents.n > 0) {
        /* Add a comment to class definitions listing super classes */
        bool first = true;
        fprintf(outfile, " /* : ");

        forv_Vec(AggregateType, parent, dispatchParents) {
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
        // TODO: remove and enforce at least 1 element in a union
        fprintf(outfile, "uint8_t dummyFieldToAvoidWarning;\n");
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
          uint64_t fieldSize = info->module->getDataLayout().getTypeStoreSize(fieldType);

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
        if( largestType ) {
          st = llvm::StructType::get(largestType
#if HAVE_LLVM_VER < 50
                                     , NULL
#endif
                                     );
        } else {
          st = llvm::StructType::get(info->module->getContext());
        }
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
      if( this->isWidePtrType() && fLLVMWideOpt ) {
        // Reach this branch when generating a wide/wide class as a
        // global pointer!
        unsigned globalAddressSpace = 0;

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
            Type* localeType = dtLocaleID->typeInfo();
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

int AggregateType::getMemberGEP(const char *name, bool &isCArrayField) {
#ifdef HAVE_LLVM
  if( symbol->hasFlag(FLAG_EXTERN) ) {
    // We will cache the info in the local GEP map.
    std::map<std::string, int>::const_iterator GEPIdx = GEPMap.find(name);
    if(GEPIdx != GEPMap.end()) {
      isCArrayField = isCArrayFieldMap[name];
      return GEPIdx->second;
    }
    int ret = getCRecordMemberGEP(symbol->cname, name, isCArrayField);
    GEPMap.insert(std::pair<std::string,int>(name, ret));
    isCArrayFieldMap.insert(std::pair<std::string,int>(name, isCArrayField));
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

        if (AggregateType* at = toAggregateType(t)) {
          forv_Vec(AggregateType, parent, at->dispatchParents) {
            if (parent)
              next_p->set_add(parent);
          }
        }
      }

      Vec<Type*>* temp = next_p;

      next_p    = current_p;
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
