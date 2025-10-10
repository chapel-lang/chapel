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

#include "llvmDebug.h"

#include "alist.h"
#include "global-ast-vecs.h"
#include "clangUtil.h"
#include "codegen.h"
#include "expr.h"
#include "genret.h"
#include "llvmVer.h"
#include "stringutil.h"
#include "symbol.h"
#include "vec.h"
#include "version.h"

#include <iostream>
#include <map>

#ifdef HAVE_LLVM
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

/*
LLVM provides a class called DIBuilder, you pass the LLVM module to this
class and it will attach the debug information to the LLVM code after the
call to finalize() on DIBuilder. The initialization happens in the
constructor of DebugData (inlined in the header). It is instanced during
codegen after setting up the LLVM module and put into debugInfo. If the
-g flag wasn't passed to the compiler then debugInfo is null. The
finalize call happens in finishCodegenLLVM().

As for all the additional data structures, all of that was built into LLVM
rather than me. The functions to create the debug information requires the
DI* types. It sort of makes sense to me, since it is building a DWARF tree
rather than building LLVM IR. It starts at the top level with the
createCompileUnit() call just before working through the modules and
attaching the simple types and building up more complex structures from
there. Getting things setup so that I can get a DISubroutine to pass to
SetCurrentDebugLocation() is needed to get information like line numbers
into the binary. So I think that the only new data structure that I'm
creating myself is the debug class itself.


See
http://llvm.org/docs/SourceLevelDebugging.html
for more information on LLVM debug information.
*/

// Note: All char strings must be the same size
// Very hacky, but completely legal and functional. Only issue is that file_name
// will have garbage from beyond full_path.
// garbage issue could be solved with scrubbing, if it matters.

char current_dir[128];
constexpr int RuntimeLang = 0;

struct DefinitionInfo {
  Symbol*             _sym;
  llvm::DIScope*    _scope;
  llvm::DIFile*      _file;
  unsigned int       _line;
  DefinitionInfo(): _sym(nullptr), _scope(nullptr), _file(nullptr), _line(0) {}
  DefinitionInfo(llvm::DIScope* scope,
                 llvm::DIFile*  file,
                 unsigned int   line):
    _sym(nullptr), _scope(scope), _file(file), _line(line) {}
  DefinitionInfo(DebugData*    debugData,
                 ModuleSymbol* defMod,
                 const char*   filename,
                 unsigned int  line):
    _sym(nullptr), _scope(debugData->getModuleScope(defMod)),
    _file(debugData->getFile(defMod, filename)), _line(line) {}
  DefinitionInfo(DebugData* debugData, Symbol* sym) : _sym(sym) {
    ModuleSymbol* defMod = sym->getModule();
    this->_scope = debugData->getModuleScope(defMod);
    this->_file = debugData->getFile(defMod, sym->fname());
    this->_line = sym->linenum();
  }

  Symbol* symbol() const { return _sym; }


  bool skipInfo() const {
    // always show the info for developer mode or if we don't know about the  symbol
    if (developer || !_sym)
      return false;

    // TODO: ideally we do this, so we can hide internal module names
    // but that messes up record wrapped types which have the same type name
    // as their _instance field (see toString in type.cpp)
    // this confuses the debugger because we have two types with the same name
    // if (auto ts = toTypeSymbol(_sym)) {
    //   if (ts->getModule()->modTag == MOD_INTERNAL)
    //     return true; // no scope for internal types
    // }
    // Instead, just selectively hide internal modules we know to be ok to hide
    static auto internalModulesToHide = std::array{
      astr("_root"),
      astr("chpl__Program"),
      astr("ChapelStringLiterals"),
      astr("ChapelBase"),
      astr("Atomics"),
      astr("ChapelSyncvar"),
      astr("ChapelTuple"),
      astr("ChapelRange"),
      astr("OwnedObject"),
      astr("SharedObject"),
      astr("String"),
    };
    if (_sym->getModule()->modTag == MOD_INTERNAL) {
      auto modName = _sym->getModule()->name;
      return std::find(internalModulesToHide.begin(),
                       internalModulesToHide.end(),
                       modName) != internalModulesToHide.end();
    }

    return false; // otherwise, show the info
  }

  llvm::DIScope* scope() const {
    return _scope;
  }
  llvm::DIScope* maybeScope() const {
    return !skipInfo() ? _scope : nullptr;
  }
  llvm::DIFile* file() const {
    return !skipInfo() ? _file : nullptr;
  }
  unsigned int line() const {
    return !skipInfo() ? _line : 0;
  }
};


// Ifdef'd to avoid unused warning, because its only usage has the same ifdef.
// If this gets used elsewhere the ifdef can be removed; besides the warning,
// there is nothing wrong with this code being included without
// HAVE_LLVM_TYPED_POINTERS.
#ifdef HAVE_LLVM_TYPED_POINTERS
static
std::string myGetTypeName(llvm::Type *ty) {
  std::string TypeName;
  llvm::raw_string_ostream TypeStream(TypeName);
  if(ty)
    ty->print(TypeStream);
  else
    TypeStream << "Printing <null> Type";
  TypeStream.flush();
  return TypeName;
}
#endif


static std::pair<llvm::DIType*, int>
removePointersAndQualifiers(llvm::DIType* N, int numLevels = -1) {
  auto res = N;
  int count = 0;
  while (llvm::isa<llvm::DIDerivedType>(res) &&
         (numLevels < 0 || count < numLevels)) {
    auto derived = llvm::cast<llvm::DIDerivedType>(res);
    res = derived->getBaseType();
    count++;
  }
  return std::make_pair(res, count);
}

void DebugData::finalize() {

  // if there are any types that weren't fully codegenned before, do it now
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->llvmDIForwardType) {
      auto oldDI = ts->llvmDIForwardType;
      ts->llvmDIForwardType = nullptr;
      auto newDI = getType(ts->type);

      // remove any pointers we added to the oldDI
      // then remove the same number of pointers from newDI
      auto [baseOldDI, numPtrs] =
        removePointersAndQualifiers(llvm::cast<llvm::DIType>(oldDI));
      auto [baseNewDI, _] = removePointersAndQualifiers(newDI, numPtrs);

      auto dibuilder = ts->getModule()->llvmDIBuilder;
      dibuilder->replaceTemporary(llvm::TempDIType(baseOldDI), baseNewDI);
    }
  }


  forv_Vec(ModuleSymbol, currentModule, allModules) {
    currentModule->llvmDIBuilder->finalize();
  }
}

void DebugData::createCompileUnit(ModuleSymbol* modSym,
                                  const char *file, const char *directory,
                                  const char *flags) {
  char version[128];
  char chapel_string[256];
  get_version(version, sizeof(version));
  snprintf(chapel_string, 256, "Chapel version %s", version);
  strncpy(current_dir, directory,sizeof(current_dir)-1);

  GenInfo* info = gGenInfo;
  modSym->llvmDIBuilder = new llvm::DIBuilder(*info->module);
  auto llvmFile = modSym->llvmDIBuilder->createFile(file, directory);
  modSym->llvmDICompileUnit =
    modSym->llvmDIBuilder->createCompileUnit(llvm::dwarf::DW_LANG_C99, /* Lang */
                                             llvmFile, /* File */
                                             chapel_string, /* Producer */
                                             this->optimized, /* isOptimized */
                                             flags, /* Flags */
                                             0 /* RV */ );
}

llvm::DIType* DebugData::maybeWrapTypeInPointer(llvm::DIType* N, Type* type) {
  auto res = N;
  // TODO: what about refs
  if (isUnmanagedClass(type) || isBorrowedClass(type)) {
    GenInfo* info = gGenInfo;
    const llvm::DataLayout& layout = info->module->getDataLayout();
    auto dibuilder = type->symbol->getModule()->llvmDIBuilder;
    // auto& Ctx = gContext->llvmContext();

    // FIXME: this information should be applied to the variable declaration
    // not the type itself
    // std::string ptrType;
    // if (isUnmanagedClass(type))
    //   ptrType = "unmanaged";
    // else if (isBorrowedClass(type))
    //   ptrType = "borrowed";
    // else if (isManagedPtrType(type))
    //   ptrType = "managed"; // TODO handle owned/shared separately
    // else if (type->symbol->hasFlag(FLAG_C_PTRCONST_CLASS))
    //   ptrType = "c_ptrConst";
    // else if (type->symbol->hasFlag(FLAG_C_PTR_CLASS))
    //   ptrType = "c_ptr";
    // else if (type->symbol->hasFlag(FLAG_DATA_CLASS))
    //   ptrType = "ddata";
    // else if (type->symbol->hasFlag(FLAG_REF))
    //   ptrType = "ref";
    // else
    //   ptrType = "unknown";

    // llvm::Metadata *Ops[2] = {
    //     llvm::MDString::get(Ctx, "chpl.ptrtype"),
    //     llvm::MDString::get(Ctx, ptrType)};
    // llvm::SmallVector<llvm::Metadata*, 1> Annots = {llvm::MDNode::get(Ctx, Ops)};
    // llvm::DINodeArray Annotations = dibuilder->getOrCreateArray(Annots);

    // todo: make this print nicer for for borrowed/unmanaged/ref?
    auto ptrN = dibuilder->createPointerType(
      res,
      layout.getPointerSizeInBits(),
      0, /* alignment */
      chpl::empty,
      type->symbol->name
      // toString(type, true)
      // TODO: toString messes up managed pointer pretty printing
      // Annotations
    );
    // auto N = dibuilder->createTypedef(ptrN, toString(type, true), nullptr, 0, nullptr);
    res = ptrN;
  }
  return res;
}

bool DebugData::shouldAddDebugInfoFor(Symbol* sym) {
  // in developer mode, add debug info for everything
  if (developer) return true;

  // skip temps
  if (sym->hasFlag(FLAG_TEMP)) return false;

  // skip non-user variables
  if (sym->hasFlag(FLAG_NO_USER_DEBUG_INFO)) return false;

  // default to adding debug info
  return true;
}

llvm::DIType* DebugData::constructTypeForAggregate(llvm::StructType* ty,
                                                   AggregateType* type) {

  const llvm::DataLayout& layout = gGenInfo->module->getDataLayout();

  const char* name = type->symbol->name;
  // toString(type, true);
  // TODO: toString messes up managed pointer pretty printing
  DefinitionInfo defInfo(this, type->symbol);
  auto dibuilder = type->symbol->getModule()->llvmDIBuilder;

  llvm::DIType* derivedFrom = nullptr;

  if (type->dispatchParents.length() > 0)
    derivedFrom = getType(type->dispatchParents.first());

  llvm::DIType* N = dibuilder->createReplaceableCompositeType(
    llvm::dwarf::DW_TAG_structure_type,
    name,
    defInfo.maybeScope(),
    defInfo.file(),
    defInfo.line(),
    RuntimeLang,
    !ty->isOpaque() ? layout.getTypeSizeInBits(ty) : 0,
    !ty->isOpaque() ? 8*layout.getABITypeAlign(ty).value() : 0
  );
  N = maybeWrapTypeInPointer(N, type);

  // N is added to the map (early) so that element search below can find it,
  // so as to avoid infinite recursion for structs that contain pointers to
  // their own type.
  type->symbol->llvmDIForwardType = N;

  if (!ty->isOpaque()) {
    llvm::SmallVector<llvm::Metadata *, 8> EltTys;

    auto slayout = layout.getStructLayout(ty);
    for_fields(field, type) {
      // field is a Symbol
      DefinitionInfo fieldDefInfo(this, field->getModule(),
                                  field->defPoint->fname(),
                                  field->defPoint->linenum());
      TypeSymbol* fts = field->type->symbol;
      llvm::Type* fty = fts->getLLVMType();

      if (!fty) {
        fty = getTypeLLVM(fts->cname);
        if (!fty) {
          if (developer || fVerify) {
            INT_FATAL("Unable to find LLVM type for field %s of type %s in struct %s",
                      field->cname, fts->cname, type->symbol->name);
          }
          return nullptr;
        }
      }

      llvm::DIType* fditype = getType(field->type);
      if (fditype == nullptr) {
        if (developer || fVerify) {
          INT_FATAL("Unable to find DIType for field %s of type %s in aggregate %s",
                    field->name, fts->name, type->symbol->name);
        }
        // if we can't determine the field type yet, create a forward decl
        // then later, the forward decl will be replaced with the actual type
        auto fieldTypeDefInfo = DefinitionInfo(this, fts->defPoint->getModule(),
                                               fts->defPoint->fname(),
                                               fts->defPoint->linenum());
        fditype = dibuilder->createReplaceableCompositeType(
          llvm::dwarf::DW_TAG_structure_type,
          fts->name,
          fieldTypeDefInfo.maybeScope(),
          fieldTypeDefInfo.file(),
          fieldTypeDefInfo.line(),
          RuntimeLang,
          layout.getTypeSizeInBits(fty),
          8*layout.getABITypeAlign(fty).value()
        );
        fditype = maybeWrapTypeInPointer(fditype, field->type);
        fts->llvmDIForwardType = fditype;
      }
      // if the field is "super", unwrap the pointer
      if (field->hasFlag(FLAG_SUPER_CLASS) &&
          fditype->getTag() == llvm::dwarf::DW_TAG_pointer_type) {
        auto pointeeType = llvm::cast<llvm::DIDerivedType>(fditype)->getBaseType();
        fditype = pointeeType;
      }

      bool unused;
      auto mty = dibuilder->createMemberType(
        fieldDefInfo.maybeScope(),
        field->name,
        fieldDefInfo.file(),
        fieldDefInfo.line(),
        layout.getTypeSizeInBits(fty),
        8*layout.getABITypeAlign(fty).value(),
        slayout->getElementOffsetInBits(type->getMemberGEP(field->cname, unused)),
        llvm::DINode::FlagZero,
        fditype);

      EltTys.push_back(mty);
    }

    // Now create the DItype for the struct
    N = dibuilder->createStructType(
      defInfo.maybeScope(),
      name,
      defInfo.file(),
      defInfo.line(),
      layout.getTypeSizeInBits(ty),
      8*layout.getABITypeAlign(ty).value(),
      llvm::DINode::FlagZero,
      derivedFrom,
      dibuilder->getOrCreateArray(EltTys)
    );
    N = maybeWrapTypeInPointer(N, type);
    if(type->symbol->llvmDIForwardType) {
      auto fwd = llvm::cast<llvm::DIType>(type->symbol->llvmDIForwardType);
      // remove any pointers we added to the fwd
      // then remove the same number of pointers from N
      auto [baseFwd, numPtrs] = removePointersAndQualifiers(fwd);
      auto [baseN, _] = removePointersAndQualifiers(N, numPtrs);

      dibuilder->replaceTemporary(llvm::TempDIType(baseFwd), baseN);
      type->symbol->llvmDIForwardType = nullptr;
    }
    type->symbol->llvmDIType = N;
  } // end of if(!Opaque)

  return N;
}

llvm::DIType* DebugData::constructTypeForPointer(llvm::Type* ty, Type* type) {
  CHPL_ASSERT(ty->isPointerTy());

  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  const char* name = type->symbol->name;
  DefinitionInfo defInfo(this, type->symbol);
  auto dibuilder = type->symbol->getModule()->llvmDIBuilder;

  if (type != type->getValType()) { // Add this condition to avoid segFault
    auto N = dibuilder->createPointerType(
      getType(type->getValType()), // it should return the pointee's DIType
      layout.getPointerSizeInBits(ty->getPointerAddressSpace()),
      0, /* alignment */
      chpl::empty,
      name);
    type->symbol->llvmDIType = N;
    return N;
  } else {
    if (type->astTag == E_PrimitiveType) {
#ifdef HAVE_LLVM_TYPED_POINTERS
      llvm::Type *PointeeTy = ty->getPointerElementType();
      // handle string, c_string, nil, opaque, raw_c_void_ptr
      if (PointeeTy->isIntegerTy()) {
        llvm::DIType* pteIntDIType; //create the DI-pointeeType
        pteIntDIType = dibuilder->createBasicType(
          myGetTypeName(PointeeTy),
          layout.getTypeSizeInBits(PointeeTy),
          llvm::dwarf::DW_ATE_unsigned);

        auto N = dibuilder->createPointerType(
          pteIntDIType,
          layout.getPointerSizeInBits(ty->getPointerAddressSpace()),
          0,
          chpl::empty,
          name);

        type->symbol->llvmDIType = N;
        return N;
      } else if (PointeeTy->isStructTy()) {
        // handle qio_channel_ptr_t, qio_file_ptr_t, syserr, _file
        auto pteStrDIType = dibuilder->createStructType(
          defInfo.maybeScope(),
          PointeeTy->getStructName(),
          defInfo.file(),
          defInfo.line(),
          (PointeeTy->isSized() ? layout.getTypeSizeInBits(PointeeTy) : 8), /* SizeInBits */
          (PointeeTy->isSized() ? 8*layout.getABITypeAlign(PointeeTy).value() : 8), /* AlignInBits */
          llvm::DINode::FlagZero,
          nullptr, /* DerivedFrom */
          nullptr /* Elements */
        );

        auto N = dibuilder->createPointerType(
          pteStrDIType,
          layout.getPointerSizeInBits(ty->getPointerAddressSpace()),
          0,
          chpl::empty,
          name);

        type->symbol->llvmDIType = N;
        return N;
      }
#else
      if (type->symbol->hasFlag(FLAG_EXTERN)) {
        // handle extern types
        // TODO: this should probably create forwarddecls?
        auto N = dibuilder->createPointerType(
          dibuilder->createUnspecifiedType(name),
          layout.getPointerSizeInBits(ty->getPointerAddressSpace()),
          0,
          chpl::empty,
          name);
        type->symbol->llvmDIType = N;
        return N;
      }
      if (developer || fVerify) {
        INT_FATAL("Unhandled debug info generation for pointer type: "
                  "%s\n\ttype->astTag=%i\n\tllvmImplType->getTypeID()=%i\n",
                  type->symbol->name, type->astTag, ty->getTypeID());
      }
      return nullptr;
#endif
    } else if (type->astTag == E_AggregateType) {
      // dealing with classes/records/unions
      AggregateType *this_class = (AggregateType *)type;

      // solve the data class: _ddata
      if (this_class->symbol->hasFlag(FLAG_DATA_CLASS)) {
        if (Type* vt = getDataClassType(this_class->symbol)->typeInfo()) {
          auto N = dibuilder->createPointerType(
            getType(vt),
            layout.getPointerSizeInBits(ty->getPointerAddressSpace()),
            0,
            chpl::empty,
            name);
          type->symbol->llvmDIType = N;
          return N;
        }
      } //Not sure whether we should directly return getType(vt)

      const char *struct_name = this_class->classStructName(true);
      if (llvm::Type* st = getTypeLLVM(struct_name)) {
        auto diType = constructTypeForAggregate(
          llvm::cast<llvm::StructType>(st), this_class);
        return diType;
      }
    } // end of astTag == E_AggregateTy
  } // end of else (type==type->getType)
  if (developer || fVerify) {
    INT_FATAL("Unhandled debug info generation for pointer type: "
              "%s\n\ttype->astTag=%i\n\tllvmImplType->getTypeID()=%i\n",
              type->symbol->name, type->astTag, ty->getTypeID());
  }
  return nullptr;
}

llvm::DIType* DebugData::constructTypeFromChplType(llvm::Type* ty, Type* type) {

  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  const char* name = type->symbol->name;
  DefinitionInfo defInfo(this, type->symbol);
  auto dibuilder = type->symbol->getModule()->llvmDIBuilder;


  if (type->isRef()) {
    auto valType = type->getValType();
    auto diType = valType ? getType(valType) : nullptr;
    if (!diType) {
      if (developer || fVerify) {
        INT_FATAL("Unable to find DIType for ref type %s ", type->symbol->name);
      }
      return nullptr;
    }
    auto N = dibuilder->createReferenceType(llvm::dwarf::DW_TAG_reference_type, diType);
    N = dibuilder->createTypedef(N, name, nullptr, 0, nullptr);
    type->symbol->llvmDIType = N;
    return N;
  } else if (type == dtObject) {
    llvm::SmallVector<llvm::Metadata *, 1> EltTys;
    llvm::Type* cidType = info->lvt->getType("chpl__class_id");
    auto cidDITy = dibuilder->createMemberType(
      defInfo.maybeScope(),
      "cid",
      defInfo.file(),
      defInfo.line(),
      layout.getTypeSizeInBits(cidType),
      8*layout.getABITypeAlign(cidType).value(),
      0, /* offset, assume its zero */
      llvm::DINode::FlagZero,
      getType(CLASS_ID_TYPE)
    );
    EltTys.push_back(cidDITy);
    // since dtOject has a single field, we can directly assume the size and alignent to
    // be the same as its single field
    llvm::DIType* N = dibuilder->createStructType(
      defInfo.maybeScope(),
      name,
      defInfo.file(),
      defInfo.line(),
      layout.getTypeSizeInBits(cidType), /* SizeInBits */
      8*layout.getABITypeAlign(cidType).value(), /* AlignInBits */
      llvm::DINode::FlagZero, /* Flags */
      nullptr,
      dibuilder->getOrCreateArray(EltTys) /* Elements */
    );
    N = maybeWrapTypeInPointer(N, type);
    type->symbol->llvmDIType = N;
    return N;
  } else if (isBoolType(type)) {
    auto size = layout.getTypeSizeInBits(ty);
    auto N = dibuilder->createBasicType(name, size, llvm::dwarf::DW_ATE_boolean);
    type->symbol->llvmDIType = N;
    return N;
  } else if (isIntType(type) || isUIntType(type)) {
    auto encoding = isSignedType(type) ? llvm::dwarf::DW_ATE_signed :
                                         llvm::dwarf::DW_ATE_unsigned;
    auto size = layout.getTypeSizeInBits(ty);
    llvm::DIType* N = dibuilder->createBasicType(name, size, encoding);
    N = dibuilder->createTypedef(N, name, nullptr, 0, nullptr);
    type->symbol->llvmDIType = N;
    return N;
  } else if (isRealType(type) || isImagType(type)) {
    // TODO: eventually, it would be nice to use DW_ATE_imaginary_float
    // for imaginary types, but lldb doesn't seem to understand that today
    auto encoding = llvm::dwarf::DW_ATE_float;
    auto size = layout.getTypeSizeInBits(ty);
    llvm::DIType* N = dibuilder->createBasicType(name, size, encoding);
    N = dibuilder->createTypedef(N, name, nullptr, 0, nullptr);
    type->symbol->llvmDIType = N;
    return N;
  } else if (isComplexType(type)) {
    // we could codegen this as DW_ATE_complex_float, but then we
    // wouldn't have the real and imaginary parts as members
    // but it would give us a pretty-printer for free
    llvm::SmallVector<llvm::Metadata *, 1> EltTys;
    auto fpSize = type == dtComplex[COMPLEX_SIZE_64] ? FLOAT_SIZE_32 :
                                                       FLOAT_SIZE_64;
    llvm::Type* reType = dtReal[fpSize]->getLLVMType();
    llvm::Type* imType = dtImag[fpSize]->getLLVMType();
    EltTys.push_back(dibuilder->createMemberType(
      defInfo.maybeScope(),
      "re",
      defInfo.file(),
      defInfo.line(),
      layout.getTypeSizeInBits(reType),
      8*layout.getABITypeAlign(reType).value(),
      0, /* offset, assume its zero */
      llvm::DINode::FlagZero,
      getType(dtReal[fpSize])
    ));
    EltTys.push_back(dibuilder->createMemberType(
      defInfo.maybeScope(),
      "im",
      defInfo.file(),
      defInfo.line(),
      layout.getTypeSizeInBits(imType),
      8*layout.getABITypeAlign(imType).value(),
      layout.getTypeSizeInBits(reType), /* offset, assume after re */
      llvm::DINode::FlagZero,
      getType(dtImag[fpSize])
    ));
    llvm::DIType* N = dibuilder->createStructType(
      defInfo.maybeScope(),
      name,
      defInfo.file(),
      defInfo.line(),
      layout.getTypeSizeInBits(ty), /* SizeInBits */
      8*layout.getABITypeAlign(ty).value(), /* AlignInBits */
      llvm::DINode::FlagZero, /* Flags */
      nullptr,
      dibuilder->getOrCreateArray(EltTys) /* Elements */
    );
    type->symbol->llvmDIType = N;
    return N;
  } else if (isEnumType(type)) {
    return constructTypeForEnum(ty, toEnumType(type));
  } else if (isAtomicType(type)) {
    AggregateType* at = toAggregateType(type);
    INT_ASSERT(at);
    Type* valType = nullptr;
    if (auto valTypeField = at->getSubstitution(astr("valType"))) {
      valType = valTypeField->type;
    } else if (type->symbol->name == astr("atomic bool") ||
               type->symbol->name == astr("AtomicBool")) {
      valType = dtBool;
    } else {
      if (developer || fVerify) {
        INT_FATAL("Unable to find valType for atomic type %s",
                  type->symbol->name);
      }
      return nullptr;
    }
    llvm::DIType* N = dibuilder->createQualifiedType(
      llvm::dwarf::DW_TAG_atomic_type, getType(valType)
    );
    N = dibuilder->createTypedef(N, name, nullptr, 0, nullptr);
    type->symbol->llvmDIType = N;
    return N;
  } else if (isSyncType(type)) {
    // TODO:
    return nullptr;
  } else if (type->symbol->hasFlag(FLAG_C_ARRAY)) {
    auto at = toAggregateType(type);
    INT_ASSERT(at);

    llvm::SmallVector<llvm::Metadata *> Subscripts;
    auto arraySize = at->cArrayLength();
    Subscripts.push_back(dibuilder->getOrCreateSubrange(0, arraySize));
    Type *elmType = at->cArrayElementType();
    auto elmDiType = getType(elmType);
    if (elmDiType == nullptr) {
      if (developer || fVerify) {
        INT_FATAL("Unable to find DIType for element type %s of array %s",
                  elmType->symbol->name, type->symbol->name);
      }
      return nullptr;
    }
    llvm::DIType* N = dibuilder->createArrayType(
      arraySize,
      8*layout.getABITypeAlign(ty).value(),
      elmDiType,
      dibuilder->getOrCreateArray(Subscripts));
    N = dibuilder->createTypedef(N, name, nullptr, 0, nullptr);
    type->symbol->llvmDIType = N;
    return N;
  } else if (type == dtStringC) {
    llvm::DIType* N = dibuilder->createPointerType(
      getType(dt_c_char),
      layout.getPointerSizeInBits(),
      0, /* alignment */
      chpl::empty,
      name);
    N = dibuilder->createQualifiedType(llvm::dwarf::DW_TAG_const_type, N);
    N = dibuilder->createTypedef(N, name, nullptr, 0, nullptr);
    type->symbol->llvmDIType = N;
    return N;
  } else if (type == dtNil || type == dtCFnPtr || type == dtCVoidPtr) {
    return dibuilder->createNullPtrType();
  } else if (toPrimitiveType(type) != nullptr &&
             type->symbol->hasFlag(FLAG_EXTERN)) {
    // if its a non-opaque struct, fill in the debug info based on those fields
    // if its an opaque struct, just create a forward decl
    // otherwise, create an unspecified type
    if (ty->isStructTy()) {
      // if (llvm::cast<llvm::StructType>(ty)->isOpaque()) {
        llvm::DIType* N = dibuilder->createForwardDecl(
          llvm::dwarf::DW_TAG_structure_type,
          name,
          defInfo.maybeScope(),
          defInfo.file(),
          defInfo.line(),
          RuntimeLang,
          0, /* SizeInBits */
          0  /* AlignInBits */
        );
        N = dibuilder->createTypedef(N, name, nullptr, 0, nullptr);
        type->symbol->llvmDIType = N;
        return N;
      // } else {
        // llvm::SmallVector<llvm::Metadata *, 8> EltTys;
        // auto slayout = layout.getStructLayout(llvm::cast<llvm::StructType>(ty));
        for (unsigned i = 0; i < ty->getStructNumElements(); i++) {
          // llvm::Type* fty = ty->getStructElementType(i);
          // EltTys.push_back(dibuilder->createMemberType(
          //   nullptr,
          //   "field", // TODO: we don't have the field name here
          //   nullptr,
          //   0,
          //   layout.getTypeSizeInBits(fty),
          //   8*layout.getABITypeAlign(fty).value(),
          //   slayout->getElementOffsetInBits(i),
          //   llvm::DINode::FlagZero,
          //   getType(fty)
          // ));
        }
      // }
    } else {
      return dibuilder->createUnspecifiedType(name);
    }
  }
  return nullptr;

}

static bool isNonCodegenType(Type * type) {
  if (type == dtVoid || type == dtNothing)
    return true;
  if (auto valType = type->getValType()) {
    if (valType == dtVoid || valType == dtNothing)
      return true;
  }
  return false;
}


llvm::DIType* DebugData::constructType(Type *type) {
  llvm::DIType *N = nullptr;

  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  llvm::Type* ty = type->symbol->getLLVMStructureType();
  const char* name = type->symbol->name;
  DefinitionInfo defInfo(this, type->symbol);
  auto dibuilder = type->symbol->getModule()->llvmDIBuilder;

  if (!ty) return nullptr;
  if (isNonCodegenType(type)) return nullptr;


  if (auto diTypeFromSpecialCase = constructTypeFromChplType(ty, type)) {
    type->symbol->llvmDIType = diTypeFromSpecialCase;
    return diTypeFromSpecialCase;
  }

  if (ty->isIntegerTy()) {
    auto encoding = isSignedType(type) ? llvm::dwarf::DW_ATE_signed :
                                         llvm::dwarf::DW_ATE_unsigned;
    N = dibuilder->createBasicType(name, layout.getTypeSizeInBits(ty), encoding);
    type->symbol->llvmDIType = N;
    return N;
  } else if (ty->isFloatingPointTy()) {
    auto encoding = llvm::dwarf::DW_ATE_float;
    N = dibuilder->createBasicType(name, layout.getTypeSizeInBits(ty), encoding);
    type->symbol->llvmDIType = N;
    return N;
  } else if (ty->isPointerTy()) {
    return constructTypeForPointer(ty, type);
  } else if (ty->isStructTy() && type->astTag == E_AggregateType) {
    return constructTypeForAggregate(
      llvm::cast<llvm::StructType>(ty), toAggregateType(type));
  } else if (ty->isStructTy() && type->astTag == E_PrimitiveType) {
    // Handle extern and opaque structs as a forward decl
    llvm::StructType* struct_type = llvm::cast<llvm::StructType>(ty);
    if (type->symbol->hasFlag(FLAG_EXTERN) || struct_type->isOpaque()) {
      auto typeSize = !struct_type->isOpaque() ? layout.getTypeSizeInBits(ty) : 0;
      auto typeAlign = !struct_type->isOpaque() ? 8*layout.getABITypeAlign(ty).value() : 0;
      N = dibuilder->createForwardDecl(
        llvm::dwarf::DW_TAG_structure_type,
        name,
        defInfo.maybeScope(),
        defInfo.file(),
        defInfo.line(),
        RuntimeLang,
        typeSize,
        typeAlign
      );
      type->symbol->llvmDIType = N;
      return N;
    } else {
      // TODO: create a struct type from the fields
      if (developer || fVerify) {
        INT_FATAL("Unhandled debug info generation for primitive struct type: "
                  "%s\n\ttype->astTag=%i\n\tllvmImplType->getTypeID()=%i\n",
                  type->symbol->name, type->astTag, ty->getTypeID());
      }
    }
  } else if (ty->isArrayTy() && type->astTag == E_AggregateType) {
    // c_array handled elsewhere
    INT_ASSERT(!type->symbol->hasFlag(FLAG_C_ARRAY));
    AggregateType *this_class = (AggregateType *)type;
    // Subscripts are "ranges" for each dimension of the array
    llvm::SmallVector<llvm::Metadata *, 4> Subscripts;
    int Asize = this_class->fields.length;
    // C-style language always has 1D for array, and index starts with "0"
    Subscripts.push_back(dibuilder->getOrCreateSubrange(0, Asize));
    Symbol *eleSym = toDefExpr(this_class->fields.head)->sym;
    Type *eleType = eleSym->type;
    if (getType(eleType) == nullptr) {
      if (developer || fVerify) {
        INT_FATAL("Unable to find DIType for element type %s of array %s",
                  eleType->symbol->name, type->symbol->name);
      }
      return nullptr;
    }
    N = dibuilder->createArrayType(
      Asize,
      8*layout.getABITypeAlign(ty).value(),
      getType(eleType),
      dibuilder->getOrCreateArray(Subscripts));

    type->symbol->llvmDIType = N;
    return N;
  }

  // We are unable for some reasons to find a debug type. This shouldn't be a
  // problem for a correct compilation, but it would be better to find a way to
  // deal with this case
  //
  if (developer || fVerify) {
    auto llvmType = type->symbol->getLLVMType();
    if (llvmType) {
      INT_FATAL("Unhandled debug info generation for type: "
                "%s\n\ttype->astTag=%i\n\tllvmImplType->getTypeID()=%i\n",
                type->symbol->name, type->astTag, llvmType->getTypeID());
    } else {
      INT_FATAL("Unhandled debug info generation for type: "
                "%s\n\ttype->astTag=%i\n\tllvmImplType is nullptr\n",
                type->symbol->name, type->astTag);
    }
  }

  return nullptr;
}

llvm::DIType* DebugData::constructTypeForEnum(llvm::Type* ty, EnumType* type) {
  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  const char* name = type->symbol->name;
  DefinitionInfo defInfo(this, type->symbol);
  auto dibuilder = type->symbol->getModule()->llvmDIBuilder;

  Type* bt = type->getIntegerType();
  auto diBT = getType(bt);
  if (!diBT) {
    if (developer || fVerify) {
      INT_FATAL("Unable to find base type for enum %s", name);
    }
    return nullptr;
  }
  llvm::SmallVector<llvm::Metadata *> Elements;
  for (auto [sym, var] : type->getConstantMap()) {
    INT_ASSERT(var && var->immediate);
    auto ev =
      dibuilder->createEnumerator(sym->name, var->immediate->aps_int());
    Elements.push_back(ev);
  }
  auto N = dibuilder->createEnumerationType(
    defInfo.maybeScope(),
    name,
    defInfo.file(),
    defInfo.line(),
    layout.getTypeSizeInBits(ty), /* SizeInBits */
    8*layout.getABITypeAlign(ty).value(), /* AlignInBits */
    dibuilder->getOrCreateArray(Elements),
    diBT,
#if LLVM_VERSION_MAJOR >= 18
   RuntimeLang,
#endif
    "", /* UniqueIdentifer */
    true /* isScoped */
  );
  type->symbol->llvmDIType = N;
  return N;
}

llvm::DIType* DebugData::getType(Type *type) {
  if (!type)
    return nullptr;

  if (type->symbol->llvmDIType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIType);

  if (type->symbol->llvmDIForwardType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIForwardType);

  constructType(type);

  if (type->symbol->llvmDIType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIType);

  if (type->symbol->llvmDIForwardType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIForwardType);

  return nullptr;
}

llvm::DIFile* DebugData::constructFile(ModuleSymbol* modSym, const char *fpath) {
  // Create strings for the directory and file.
  const char* last_slash;
  const char* file;
  const char* directory;

  last_slash = strrchr(fpath, '/');
  if( last_slash ) {
    file = astr(last_slash+1);
    directory = asubstr(fpath, last_slash);
  }
  else {
    file = fpath;
    directory = astr(current_dir);
  }

  return modSym->llvmDIBuilder->createFile(file, directory);
}

llvm::DIFile* DebugData::getFile(ModuleSymbol* modSym, const char *fpath) {
  // First, check to see if it's already a in our hashtable.
  if( this->filesByName.count(fpath) > 0 ) {
    return this->filesByName[fpath];
  }

  // Otherwise, construct the type, add it to the map,and return it
  llvm::DIFile* dif = constructFile(modSym, fpath);
  this->filesByName[fpath] = dif;
  return dif;
}

llvm::DINamespace* DebugData::constructModuleScope(ModuleSymbol* modSym)
{
  const char* fname = modSym->fname();
  llvm::DIFile* file = getFile(modSym, fname);
  return modSym->llvmDIBuilder->createNameSpace(file, /* Scope */
                                         modSym->name, /* Name */
                                         false /* ExportSymbols */
                                        );
}

llvm::DINamespace* DebugData::getModuleScope(ModuleSymbol* modSym)
{
  if( nullptr == modSym->llvmDINameSpace ) {
    modSym->llvmDINameSpace = constructModuleScope(modSym);
  }
  return llvm::cast_or_null<llvm::DINamespace>(modSym->llvmDINameSpace);
}

llvm::DISubroutineType* DebugData::getFunctionType(FnSymbol *function)
{
  ModuleSymbol* modSym = function->getModule();
  llvm::SmallVector<llvm::Metadata *,16> ret_arg_types;

  ret_arg_types.push_back(getType(function->retType));
  for_formals(arg, function) {
    ret_arg_types.push_back(getType(arg->type));
  }
  llvm::DITypeRefArray ret_arg_arr = modSym->llvmDIBuilder->getOrCreateTypeArray(ret_arg_types);
  return modSym->llvmDIBuilder->createSubroutineType(ret_arg_arr);
}

llvm::DISubprogram* DebugData::constructFunction(FnSymbol *function)
{
  const char *name = function->name;
  const char *cname = function->cname;
  ModuleSymbol* modSym = (ModuleSymbol*) function->defPoint->parentSymbol;
  DefinitionInfo defInfo(this, modSym, function->astloc.filename(), function->astloc.lineno());
  // Get the function using the cname since that is how it is
  // stored in the generated code. The name is just used within Chapel.

  llvm::DISubroutineType* function_type = getFunctionType(function);

  llvm::DISubprogram::DISPFlags SPFlags = llvm::DISubprogram::SPFlagDefinition;
  if (!function->hasFlag(FLAG_EXPORT))
    SPFlags |= llvm::DISubprogram::SPFlagLocalToUnit;
  if (optimized)
    SPFlags |= llvm::DISubprogram::SPFlagOptimized;

  llvm::DISubprogram* ret = modSym->llvmDIBuilder->createFunction(
    defInfo.maybeScope(),
    name,
    cname,
    defInfo.file(), defInfo.line(), function_type,
    defInfo.line(), /* beginning of scope we start */
    llvm::DINode::FlagZero,
    SPFlags
    );
  return ret;
}

llvm::DISubprogram* DebugData::getFunction(FnSymbol *function)
{
  if( nullptr == function->llvmDISubprogram ) {
    function->llvmDISubprogram = constructFunction(function);
  }
  return llvm::cast_or_null<llvm::DISubprogram>(function->llvmDISubprogram);
}


llvm::DIGlobalVariableExpression* DebugData::constructGlobalVariable(VarSymbol *gVarSym)
{
  const char *name = gVarSym->name;
  const char *cname = gVarSym->cname;

  DefinitionInfo defInfo(this, gVarSym);
  ModuleSymbol* modSym = gVarSym->getModule();
  defInfo._scope = modSym->llvmDICompileUnit;
  auto dibuilder = modSym->llvmDIBuilder;

  llvm::DIType* gVarSym_type = getType(gVarSym->type); // type is member of Symbol

  if(gVarSym_type)
    return dibuilder->createGlobalVariableExpression
     (
      defInfo.maybeScope(), /* Context */
      name, /* name */
      cname, /* linkage name */
      defInfo.file(), /* File */
      defInfo.line(), /* LineNo */
      gVarSym_type, /* Ty */
      !gVarSym->hasFlag(FLAG_EXPORT) /* is local to unit */
     );
  else {
    //return an Empty dbg node if the symbol type is unresolved
    return nullptr;
  }
}

llvm::DIGlobalVariableExpression* DebugData::getGlobalVariable(VarSymbol *gVarSym)
{
  if( nullptr == gVarSym->llvmDIGlobalVariable ) {
    gVarSym->llvmDIGlobalVariable = constructGlobalVariable(gVarSym);
  }
  return llvm::cast_or_null<llvm::DIGlobalVariableExpression>(
              gVarSym->llvmDIGlobalVariable);
}

llvm::DIVariable* DebugData::constructVariable(VarSymbol *varSym)
{
  const char *name = varSym->name;
  FnSymbol *funcSym = varSym->defPoint->getFunction();
  ModuleSymbol* modSym = varSym->getModule();
  DefinitionInfo defInfo(this, varSym);
  defInfo._scope = getFunction(funcSym);
  auto dibuilder = modSym->llvmDIBuilder;

  llvm::DIType* varSym_type = getType(varSym->type);

  if (!varSym_type) {
    if (developer || fVerify) {
      INT_FATAL("Unable to find DIType for variable %s of type %s in function %s",
                varSym->name, varSym->type->symbol->name, funcSym->name);
    }
    varSym_type = dibuilder->createNullPtrType();
  }

  llvm::DILocalVariable* localVariable = dibuilder->createAutoVariable(
    defInfo.scope(),
    name,
    defInfo.file(),
    defInfo.line(),
    varSym_type,
    true/*AlwaysPreserve, won't be removed when optimized*/
  ); //omit the  Flags and ArgNo

  // TODO: can I add inlined debug info here so I can get rid of FLAG_NO_USER_DEBUG_INFO
  auto& ctx = gGenInfo->irBuilder->getContext();
  dibuilder->insertDeclare(varSym->codegen().val, localVariable,
    dibuilder->createExpression(), llvm::DILocation::get(
      ctx, defInfo.line(), 0, defInfo.scope(), nullptr, false),
    gGenInfo->irBuilder->GetInsertBlock());
  return localVariable;

}

llvm::DIVariable* DebugData::getVariable(VarSymbol *varSym)
{
  if( nullptr == varSym->llvmDIVariable ){
    varSym->llvmDIVariable = constructVariable(varSym);
  }
  return llvm::cast_or_null<llvm::DIVariable>(varSym->llvmDIVariable);
}

llvm::DIVariable* DebugData::constructFormalArg(ArgSymbol *argSym, unsigned ArgNo)
{
  const char *name = argSym->name;
  FnSymbol *funcSym = nullptr;
  if(isFnSymbol(argSym->defPoint->parentSymbol))
    funcSym = (FnSymbol*)argSym->defPoint->parentSymbol;
  else
    printf("Couldn't find the function parent of param: %s!\n",name);

  DefinitionInfo defInfo(this, argSym);
  defInfo._scope = getFunction(funcSym);
  auto dibuilder = argSym->getModule()->llvmDIBuilder;

  llvm::DIType* argSym_type = getType(argSym->type);

  if (!argSym_type) {
    if (developer || fVerify) {
      INT_FATAL("Unable to find DIType for formal %s of type %s in function %s",
                argSym->name, argSym->type->symbol->name, funcSym->name);
    }
    argSym_type = dibuilder->createNullPtrType();
  }
  auto diParameterVariable = dibuilder->createParameterVariable(
    defInfo.scope(),
    name,
    ArgNo,
    defInfo.file(),
    defInfo.line(),
    argSym_type,
    true,/*AlwaysPreserve, won't be removed when optimized*/
    llvm::DINode::FlagZero /*Flags*/
    );
  auto Storage = gGenInfo->lvt->getValue(argSym->cname);
  auto& ctx = gGenInfo->irBuilder->getContext();
  dibuilder->insertDeclare(Storage.val, diParameterVariable,
    dibuilder->createExpression(), llvm::DILocation::get(
      ctx, defInfo.line(), 0, defInfo.scope(), nullptr, false),
    gGenInfo->irBuilder->GetInsertBlock());
  return diParameterVariable;
}

llvm::DIVariable* DebugData::getFormalArg(ArgSymbol *argSym, unsigned int ArgNo)
{
  if( nullptr == argSym->llvmDIFormal ){
    argSym->llvmDIFormal = constructFormalArg(argSym, ArgNo);
  }
  return llvm::cast_or_null<llvm::DIVariable>(argSym->llvmDIFormal);
}

#endif
