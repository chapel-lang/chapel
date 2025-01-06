/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
#include "llvm/IR/DataLayout.h"
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
  GenInfo* info = gGenInfo;
  if (info->cfile) {
    // no action required.
  } else {
#ifdef HAVE_LLVM
    llvm::Type* llvmType = info->lvt->getType(this->symbol->cname);
    if (llvmType == nullptr) {
      if (this == dtVoid || this == dtNothing) {
        llvmType = llvm::Type::getVoidTy(gContext->llvmContext());
      } else {
        USR_FATAL_CONT(this, "could not find C type for %s",
                       this->symbol->cname);
        // fake it so we can continue and report more errors, if present
        llvmType = llvm::Type::getInt8Ty(gContext->llvmContext());
      }
      // cf. int64_t is added to lvt in addGlobalCDecl()
      info->lvt->addGlobalType(this->symbol->cname, llvmType, false);
    }
    INT_ASSERT(llvmType);
    // Assert that we have not set these yet.
    // Otherwise use this: if (this->symbol->hasLLVMType()) return;
    INT_ASSERT(this->symbol->llvmImplType == nullptr);
    INT_ASSERT(this->symbol->llvmAlignment == ALIGNMENT_UNINIT);
    // set them now
    this->symbol->llvmImplType = llvmType;
    this->symbol->llvmAlignment = ALIGNMENT_DEFER;
#endif
  }
}

void ConstrainedType::codegenDef() {
}

// TODO: See 'codegenFunctionTypeLLVM' for hints about what ABI stuff to
// do when code generating extern/export stuff. It's a mess in there!
void FunctionType::codegenDef() {
  auto info = gGenInfo;
  auto outfile = info->cfile;

  if (outfile) {
    INT_FATAL("The C backend is not supported yet!");
  } else {
    #ifdef HAVE_LLVM
    llvm::Type* returnTy = this->returnType()->symbol->getLLVMType();
    std::vector<llvm::Type*> argTys;

    for (int i = 0; i < numFormals(); i++) {
      auto formal = this->formal(i);
      llvm::Type* llvmTy = formal->type->symbol->getLLVMType();
      INT_ASSERT(llvmTy);
      argTys.push_back(llvmTy);
    }

    // Get the base type for the function. It is not a pointer.
    const bool isVarArgs = false;
    auto baseType = llvm::FunctionType::get(returnTy, argTys, isVarArgs);

    // TODO: With the new 'opaque pointer' strategy that LLVM is adopting,
    // it would be impossible to get the underlying type unless we carry
    // it around or have access to the Chapel FunctionType in contexts
    // where that information is important...
    // But this complicates the generator, because the type of a static
    // function (FnSymbol*) is a pointer to a function type, and the type
    // of a Chapel function type is the underlying function type.
    // For now, my workaround is just going to be to use a static map from
    // the pointer type to the function type. That way the LLVM type for a
    // Chapel FunctionType can still be a pointer, and everything lines up
    // nicely in the generator.
    bool ok = llvmMapUnderlyingFunctionType(this, baseType);
    INT_ASSERT(ok);

    // The final type is a pointer to the underlying function type.
    auto& layout = info->module->getDataLayout();
    auto addrSpace = layout.getAllocaAddrSpace();
    auto type = llvm::PointerType::get(baseType, addrSpace);

    if (! this->symbol->hasLLVMType()) {
      info->lvt->addGlobalType(this->symbol->cname, type, false);
      this->symbol->llvmImplType = type;
      this->symbol->llvmAlignment = ALIGNMENT_DEFER;
    }
    #endif
  }
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
    llvm::Type *type = info->lvt->getType(symbol->cname);

    if (type) {
      INT_ASSERT(symbol->llvmImplType == type);
      INT_ASSERT(symbol->llvmAlignment != ALIGNMENT_UNINIT);
    } else {
      PrimitiveType* ty = getIntegerType();
      type = ty->codegen().type;

      info->lvt->addGlobalType(symbol->cname, type, !is_signed(ty));
      symbol->llvmImplType = type;
      symbol->llvmAlignment = ALIGNMENT_DEFER;

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

#ifdef HAVE_LLVM

static Type* baseForLLVMPointer(TypeSymbol* origBase) {
  Type* result = origBase->type;
  if (result == dtVoid || result == dtNothing)
    // LLVM does not allow void*, see StructType::isValidElementType()
    // for typed pointers, use i8*
    // for untyped pointers, i8* is good enough. it does not matter the base type
    result = dtInt[INT_SIZE_8];
  return result;
}

//
// layout / alignment helpers: buildStructFields(), checkStructFields()
//

// If we end up relying on LLVM for the layout, which is a common case,
// we set 'structSize' to 0, 'structAlignment' to ALIGNMENT_DEFER,
// and do not fill 'aligns'.
static void buildStructFields(const llvm::DataLayout& DL,
                              AggregateType* ag, bool explicitPadding,
                              std::map<std::string, int>& GEPMap,
                              std::vector<llvm::Type*>& params,
                              std::vector<unsigned>& aligns,
                              uint64_t& structSize, int& structAlignment)
{
  GenInfo* info = gGenInfo;
  int      paramID = 0;
  uint64_t currSize = 0;
  int      currAlignment = ALIGNMENT_DEFER;
  INT_ASSERT(GEPMap.empty());  // we clear it if explicit padding is needed
  llvm::Type* padTy =
    explicitPadding ? llvm::Type::getInt8Ty(gContext->llvmContext()) : nullptr;

  // Performs updates when explicitPadding==false.
  // This avoids the need to compute fieldSize and fieldAlign.
  auto addFieldWhenNotEP = [&](const char* fieldName, llvm::Type* fieldType) {
    GEPMap.insert(std::pair<std::string, int>(fieldName, paramID));
    paramID++;
    params.push_back(fieldType);
  };

  // Performs updates when explicitPadding==true.
  // Assumes that padding has already been inserted as needed.
  auto addFieldWhenEP = [&](const char* fieldName, llvm::Type* fieldType,
                            uint64_t fieldSize, int fieldAlign) {
    GEPMap.insert(std::pair<std::string, int>(fieldName, paramID));
    paramID++;
    params.push_back(fieldType);
    aligns.push_back(fieldAlign);
    currSize += fieldSize;
    currAlignment = std::max(currAlignment, fieldAlign);
    if (fieldAlign > 16 && ag->aggregateTag == AGGREGATE_CLASS) {
      USR_FATAL_CONT(ag, "alignments > 16 are currently not implemented"
                         " for classes");
      USR_PRINT(ag, "alignment %d is requested by field '%s' of class '%s'",
                fieldAlign, fieldName, ag->symbol->name);
    }
  };

  // Inserts 'numBytes' of padding when explicitPadding==true.
  auto insertPadding = [&](int numBytes) {
    // GEPMap is unaffected
    paramID++;
    params.push_back(llvm::ArrayType::get(padTy, numBytes));
    aligns.push_back(1);
    currSize += numBytes;
    // currAlignment is unaffected
  };


  if (ag->aggregateTag == AGGREGATE_UNION) {
    //// a union ////
    const char* uidName = "_uid";
    llvm::Type* uidType = llvm::Type::getInt64Ty(info->module->getContext());
    if (explicitPadding)
      addFieldWhenEP(uidName, uidType, 8,
                     (int)DL.getABITypeAlign(uidType).value());
    else
      addFieldWhenNotEP(uidName, uidType);

    llvm::Type *largestType = NULL;
    uint64_t largestSize = 0;

    for_fields(field, ag) {
      llvm::Type* fieldType = field->type->symbol->getLLVMType();
      INT_ASSERT(fieldType);
      uint64_t fieldSize = DL.getTypeStoreSize(fieldType);

      if (fieldSize > largestSize) {
        largestType = fieldType;
        largestSize = fieldSize;
      }

      GEPMap.insert(std::pair<std::string, int>(field->cname, paramID));
      GEPMap.insert(std::pair<std::string, int>(
            std::string("_u.") + field->cname, paramID));

      // TODO account for field's alignment in case it is >8
    }

    llvm::StructType * st;
    // handle an empty union.
    if (largestType != nullptr) {
      st = llvm::StructType::get(largestType);
    } else {
      st = llvm::StructType::get(info->module->getContext());
    }

    const char* unionName = "_u";
    if (explicitPadding)
      addFieldWhenEP(unionName, st, largestSize,
                     (int)DL.getABITypeAlign(st).value());
    else
      addFieldWhenNotEP(unionName, st);

  } else if (ag->symbol->hasFlag(FLAG_OBJECT_CLASS)) {
    //// the RootClass ////
    llvm::Type* cidType = info->lvt->getType("chpl__class_id");
    INT_ASSERT(cidType);
    const char* cidName = "chpl__cid";
    if (explicitPadding)
      addFieldWhenEP(cidName, cidType, DL.getTypeStoreSize(cidType),
                     (int)DL.getABITypeAlign(cidType).value());
    else
      addFieldWhenNotEP(cidName, cidType);

  } else {
    //// a record or class ////

    for_fields(field, ag) {
      if (field->type == dtNothing || field->type == dtVoid) continue;

      TypeSymbol* fieldSym = field->type->symbol;

      llvm::Type* fieldType = field->hasFlag(FLAG_SUPER_CLASS) ?
        fieldSym->getLLVMStructureType() : fieldSym->getLLVMType();

      int fieldAlign = field->hasFlag(FLAG_SUPER_CLASS) ?
        fieldSym->getLLVMStructureAlignment() : fieldSym->getLLVMAlignment();

      if (! explicitPadding &&
          // skip getABITypeAlign() in the common case of deferred fieldAlign
          ! isDeferredAlignment(fieldAlign) &&
          fieldAlign > (int)DL.getABITypeAlign(fieldType).value())
      {
        // We had relied on LLVM to align our fields up until this field,
        // which LLVM may not align correctly. Restart with explicit padding.
        params.clear(); aligns.clear(); GEPMap.clear();
        buildStructFields(DL, ag, true, GEPMap, params, aligns,
                          structSize, structAlignment);
        return;
      }

      if (explicitPadding) {
        fieldAlign = fieldSym->getABIAlignment(fieldType);
        // add padding before the field if needed
        if (currSize % fieldAlign > 0)
          insertPadding(fieldAlign - currSize % fieldAlign);
      }

      if (explicitPadding)
        addFieldWhenEP(field->cname, fieldType,
                       DL.getTypeStoreSize(fieldType), fieldAlign);
      else
        addFieldWhenNotEP(field->cname, fieldType);

    } // for_fields

    if (params.empty()) {
      // add dummyFieldToAvoidWarning which also
      // stops us from doing 0-byte memory allocation
      // (comes up with ioNewline and --no-local RA)
      // TODO - don't ever allocate 0-byte structures
      // example: DefaultComparator
      if (explicitPadding)
        addFieldWhenEP("_dummyFieldToAvoidWarning", padTy, 1, 1);
      else
        addFieldWhenNotEP("_dummyFieldToAvoidWarning",
          llvm::Type::getInt32Ty(gContext->llvmContext())); // here padTy==0
    }
  } // if 'ag' is a class or record

  // add padding at end if needed
  if (explicitPadding) {
    if (currSize % currAlignment > 0)
      insertPadding(currAlignment - currSize % currAlignment);
  }

  structSize = currSize;
  structAlignment = currAlignment;
}

// Now that we have an llvm::StructType, we can check its layout:
// what we think vs. what LLVM thinks.
static void checkStructFields(const llvm::DataLayout& DL,
                              AggregateType* ag, llvm::StructType* stype,
                              std::vector<llvm::Type*>& params,
                              std::vector<unsigned>& aligns,
                              uint64_t structSize, int structAlignment)
{
  // For the release we should hide these behind 'if (fVerify)'
  // to reduce codegen time and to reduce the chance of a false alarm
  // on an architecture that we have not tested.
  if (structSize == 0) {
    INT_ASSERT(aligns.size() == 0);                   // we did not populate it
    INT_ASSERT(isDeferredAlignment(structAlignment)); // delegated to LLVM
  } else {
    INT_ASSERT(params.size() == aligns.size());  // we populated them together
    const llvm::StructLayout* layout = DL.getStructLayout(stype);
    // These are other ways to query the struct size - any of them useful?
    //  DL.getTypeAllocSize(stype) and layout->getSizeInBytes()
    INT_ASSERT(structSize == DL.getTypeStoreSize(stype));
    // verify that our alignment makes sense
    INT_ASSERT(structAlignment >= (int)DL.getABITypeAlign(stype).value());
    // check that all fields are sufficiently aligned
    for (size_t paramID = 0; paramID < params.size(); paramID++)
      INT_ASSERT(layout->getElementOffset(paramID) % aligns[paramID] == 0);
  }
}
#endif

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
  int structAlignment = ALIGNMENT_UNINIT;
#endif
  if (id == breakOnCodegenID) gdbShouldBreakHere();

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
      structAlignment = getField("x0")->type->getLLVMAlignment();
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
      structAlignment = ALIGNMENT_DEFER; // or pointer alignment
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
      llvm::Type* llBaseType = baseForLLVMPointer(base)->getLLVMType();
      type = llvm::PointerType::getUnqual(llBaseType);
      structAlignment = ALIGNMENT_DEFER; // or pointer alignment
#endif
    }
  } else if (symbol->hasFlag(FLAG_DATA_CLASS)) {
    TypeSymbol* base = getDataClassType(symbol);
    const char* baseType = base->cname;
    if( outfile ) {
      const char* constness =
          symbol->hasFlag(FLAG_C_PTRCONST_CLASS) ? "const " : "";
      fprintf(outfile, "typedef %s%s *%s;\n", constness, baseType,
              symbol->cname);
    } else {
#ifdef HAVE_LLVM
      llvm::Type* llBaseType = baseForLLVMPointer(base)->getLLVMType();
      type = llvm::PointerType::getUnqual(llBaseType);
      structAlignment = ALIGNMENT_DEFER; // or pointer alignment
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
      const llvm::DataLayout& DL = info->module->getDataLayout();
      std::vector<llvm::Type*> params;
      std::vector<unsigned> aligns; // only for assertions
      uint64_t structSize = 0;

      // this updates params, aligns, structSize, structAlignment
      buildStructFields(DL, this, false, GEPMap, params, aligns,
                        structSize, structAlignment);

      if( this->isWidePtrType() && fLLVMWideOpt ) {
        // Reach this branch when generating a wide/wide class as a
        // global pointer!
        unsigned globalAddressSpace = 0;

        globalAddressSpace = info->globalToWideInfo.globalSpace;

        Type* baseType = this->getField("addr")->type;
        llvm::Type* llBaseType = baseType->symbol->codegen().type;
        INT_ASSERT(llBaseType);
        llvm::Type *globalPtrTy = nullptr;

        if (isOpaquePointer(llBaseType)) {
#if HAVE_LLVM_VER >= 140
          // No need to compute the element type for an opaque pointer
          globalPtrTy = llvm::PointerType::get(gContext->llvmContext(),
                                               globalAddressSpace);
#endif
        } else {
#ifdef HAVE_LLVM_TYPED_POINTERS
          // Remove one level of indirection since the addr field
          // of a wide pointer is always a local address.
          llvm::Type* eltType = llBaseType->getPointerElementType();
          INT_ASSERT(eltType);
          globalPtrTy = llvm::PointerType::get(eltType, globalAddressSpace);
#endif
        }

        INT_ASSERT(globalPtrTy);
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

        // Is it a class or a record?
        // if it's a class, we update the existing type.
        // if it's a record, we make the new type now.
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
        // TODO: for the release, hide this check behind 'if (fVerify)'
        checkStructFields(DL, this, stype, params, aligns,
                          structSize, structAlignment);
      }
#endif
    }
  }

  if( !outfile ) {
#ifdef HAVE_LLVM
    if(! this->symbol->hasLLVMType()) {
      info->lvt->addGlobalType(this->symbol->cname, type, false);
      this->symbol->llvmImplType = type;
    } else {
      INT_ASSERT(this->symbol->llvmImplType == type);
    }
    INT_ASSERT(this->symbol->llvmAlignment == ALIGNMENT_UNINIT);
    INT_ASSERT(structAlignment != ALIGNMENT_UNINIT);
    this->symbol->llvmAlignment = llvmAlignmentOrDefer(structAlignment, type);
    // double-check, in case we missed this in buildStructFields
    if (structAlignment > 16 && aggregateTag == AGGREGATE_CLASS &&
        ! fatalErrorsEncountered())
      USR_FATAL_CONT(this,
        "alignment > 16 is currently not implemented for classes");
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
      info->lvt->addGlobalType(struct_name, st, false);

      llvm::PointerType* pt = llvm::PointerType::getUnqual(st);
      info->lvt->addGlobalType(symbol->cname, pt, false);
      symbol->llvmImplType = st;
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
