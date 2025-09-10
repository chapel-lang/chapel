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
constructor of debug_data (inlined in the header). It is instanced during
codegen after setting up the LLVM module and put into debug_info. If the
-g flag wasn't passed to the compiler then debug_info is null. The
finalize call happens in finishCodegenLLVM().

As for all the additional data structures, all of that was built into LLVM
rather than me. The functions to create the debug information requires the
DI* types. It sort of makes sense to me, since it is building a DWARF tree
rather than building LLVM IR. It starts at the top level with the
create_compile_unit() call just before working through the modules and
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
llvm::DenseMap<const Type *, llvm::MDNode *> myTypeDescriptors;

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


void debug_data::finalize() {
  forv_Vec(ModuleSymbol, currentModule, allModules) {
    currentModule->llvmDIBuilder->finalize();
  }
}

void debug_data::create_compile_unit(
  ModuleSymbol* modSym,
  const char *file, const char *directory, bool is_optimized, const char *flags
) {
  this->optimized = is_optimized;
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
                                             is_optimized, /* isOptimized */
                                             flags, /* Flags */
                                             0 /* RV */ );
}


llvm::DIType* debug_data::construct_type_for_aggregate(
  llvm::StructType* ty, AggregateType* type
) {

  if(ty->isOpaque())
    return nullptr;

  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  const char* name = type->symbol->name;
  ModuleSymbol* defModule = type->symbol->getModule();
  const char* defFile = type->symbol->fname();
  int defLine = type->symbol->linenum();

  auto dibuilder = defModule->llvmDIBuilder;

  llvm::DIType* derivedFrom = nullptr;

  if (type->dispatchParents.length() > 0)
    derivedFrom = get_type(type->dispatchParents.first());

  auto N = dibuilder->createForwardDecl(
    llvm::dwarf::DW_TAG_structure_type,
    name,
    get_module_scope(defModule),
    get_file(defModule, defFile),
    defLine,
    0, // RuntimeLang
    !ty->isOpaque() ? layout.getTypeSizeInBits(ty) : 0,
    !ty->isOpaque() ? 8*layout.getABITypeAlign(ty).value() : 0
  );

  //N is added to the map (early) so that element search below can find it,
  //so as to avoid infinite recursion for structs that contain pointers to
  //their own type.
  type->symbol->llvmDIForwardType = N;

  if (!ty->isOpaque()) {
    llvm::SmallVector<llvm::Metadata *, 8> EltTys;

    auto slayout = layout.getStructLayout(ty);
    for_fields(field, type) {
      // field is a Symbol
      const char* fieldDefFile = field->defPoint->fname();
      int fieldDefLine = field->defPoint->linenum();
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

      llvm::DIType* fditype = get_type(field->type);
      if(fditype == nullptr) {
        // if we can't determine the field type yet, create a forward decl
        // then later, the forward decl will be replaced with the actual type
        // TODO: this creates crashes with arrays, why? Possibly bad interaction
        // with ddata?
        // fditype = dibuilder->createForwardDecl(
        //   llvm::dwarf::DW_TAG_structure_type,
        //   fts->name,
        //   get_module_scope(fts->defPoint->getModule()),
        //   get_file(fts->defPoint->getModule(), fts->defPoint->fname()),
        //   fts->defPoint->linenum(),
        //   0, // RuntimeLang
        //   layout.getTypeSizeInBits(fty),
        //   8*layout.getABITypeAlign(fty).value()
        // );
        fditype = dibuilder->createNullPtrType();
      }

      bool unused;
      auto mty = dibuilder->createMemberType(
        get_module_scope(defModule),
        field->name,
        get_file(defModule, fieldDefFile),
        fieldDefLine,
        layout.getTypeSizeInBits(fty),
        8*layout.getABITypeAlign(fty).value(),
        slayout->getElementOffsetInBits(type->getMemberGEP(field->cname, unused)),
        llvm::DINode::FlagZero,
        fditype);

      EltTys.push_back(mty);
    }

    // Now create the DItype for the struct
    N = dibuilder->createStructType(
      get_module_scope(defModule), /* Scope */
      name, /* Name */
      get_file(defModule, defFile), /* File */
      defLine, /* LineNumber */
      layout.getTypeSizeInBits(ty), /* SizeInBits */
      8*layout.getABITypeAlign(ty).value(), /* AlignInBits */
      llvm::DINode::FlagZero, /* Flags */
      derivedFrom, /* DerivedFrom */
      dibuilder->getOrCreateArray(EltTys) /* Elements */
    );
    type->symbol->llvmDIForwardType = nullptr;
    type->symbol->llvmDIType = N;
  } // end of if(!Opaque)

  return llvm::cast_or_null<llvm::DIType>(N);
}

llvm::DIType* debug_data::construct_type_for_pointer(llvm::Type* ty, Type* type) {
  CHPL_ASSERT(ty->isPointerTy());

  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  const char* name = type->symbol->name;
  ModuleSymbol* defModule = type->symbol->getModule();
#ifdef HAVE_LLVM_TYPED_POINTERS
  const char* defFile = type->symbol->fname();
#endif

  auto dibuilder = defModule->llvmDIBuilder;

  if (type != type->getValType()) { // Add this condition to avoid segFault
    auto N = dibuilder->createPointerType(
      get_type(type->getValType()), // it should return the pointee's DIType
      layout.getPointerSizeInBits(ty->getPointerAddressSpace()),
      0, /* alignment */
      chpl::empty,
      name);
    type->symbol->llvmDIType = N;
    return llvm::cast_or_null<llvm::DIType>(N);
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
        return llvm::cast_or_null<llvm::DIType>(N);
      } else if (PointeeTy->isStructTy()) {
        // handle qio_channel_ptr_t, qio_file_ptr_t, syserr, _file
        auto pteStrDIType = dibuilder->createStructType(
          get_module_scope(defModule), /* Scope */
          PointeeTy->getStructName(), /* Name */
          get_file(defModule, defFile), /* File */
          0, /* LineNumber */
          (PointeeTy->isSized()?
          layout.getTypeSizeInBits(PointeeTy):
          8), /* SizeInBits */
          (PointeeTy->isSized()?
          8*layout.getABITypeAlign(PointeeTy).value():
          8), /* AlignInBits */
          llvm::DINode::FlagZero, /* Flags */
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
        return llvm::cast_or_null<llvm::DIType>(N);
      }
#else
      return nullptr;
#endif
    } else if (type->astTag == E_AggregateType) {
      // dealing with classes/records/unions
      AggregateType *this_class = (AggregateType *)type;

      // solve the data class: _ddata
      if (this_class->symbol->hasFlag(FLAG_DATA_CLASS)) {
        if (Type* vt = getDataClassType(this_class->symbol)->typeInfo()) {
          auto N = dibuilder->createPointerType(
            get_type(vt),
            layout.getPointerSizeInBits(ty->getPointerAddressSpace()),
            0,
            chpl::empty,
            name);
          type->symbol->llvmDIType = N;
          return llvm::cast_or_null<llvm::DIType>(N);
        }
      } //Not sure whether we should directly return getType(vt)

      const char *struct_name = this_class->classStructName(true);
      if (llvm::Type* st = getTypeLLVM(struct_name)) {
        auto diType = construct_type_for_aggregate(
          llvm::cast<llvm::StructType>(st), this_class);
        return diType;
      }
    } // end of astTag == E_AggregateTy
  } // end of else (type==type->getType)
  return nullptr;
}

llvm::DIType* debug_data::construct_type(Type *type) {
  llvm::MDNode *N = nullptr;
  // if(N) return llvm::cast_or_null<llvm::DIType>(N);

  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  llvm::Type* ty = type->symbol->getLLVMStructureType();
  const char* name = type->symbol->name;
  ModuleSymbol* defModule = type->symbol->getModule();
  const char* defFile = type->symbol->fname();
  int defLine = type->symbol->linenum();

  auto dibuilder = defModule->llvmDIBuilder;

  if (!ty) return nullptr;

  if (ty->isIntegerTy()) {
    // TODO: special handling for enums
    if (type->astTag == E_EnumType) {
      return construct_type_for_enum(ty, toEnumType(type));
    } else {
      auto encoding = is_bool_type(type) ? llvm::dwarf::DW_ATE_boolean :
                     (is_signed(type)    ? llvm::dwarf::DW_ATE_signed :
                                          llvm::dwarf::DW_ATE_unsigned);
      N = dibuilder->createBasicType(
        name, /* Name */
        layout.getTypeSizeInBits(ty), /* SizeInBits */
        encoding);
      type->symbol->llvmDIType = N;
      return llvm::cast_or_null<llvm::DIType>(N);
    }
  } else if (ty->isFloatingPointTy()) {
    N = dibuilder->createBasicType(
      name,
      layout.getTypeSizeInBits(ty),
      llvm::dwarf::DW_ATE_float);
    type->symbol->llvmDIType = N;
    return llvm::cast_or_null<llvm::DIType>(N);
  } else if (ty->isPointerTy()) {
    return construct_type_for_pointer(ty, type);
  } else if (ty->isStructTy() && type->astTag == E_AggregateType) {
    return construct_type_for_aggregate(
      llvm::cast<llvm::StructType>(ty), toAggregateType(type));
  } else if (ty->isStructTy() && type->astTag == E_PrimitiveType &&
             type->symbol->hasFlag(FLAG_EXTERN)) {
    // Handle extern structs
    llvm::StructType* struct_type = llvm::cast<llvm::StructType>(ty);

    N = dibuilder->createForwardDecl(
      llvm::dwarf::DW_TAG_structure_type,
      name,
      get_module_scope(defModule),
      get_file(defModule, defFile),
      defLine,
      0, // RuntimeLang
      !struct_type->isOpaque() ? layout.getTypeSizeInBits(ty) : 0,
      !struct_type->isOpaque() ? 8*layout.getABITypeAlign(ty).value() : 0
    );
    type->symbol->llvmDIType = N;
    return llvm::cast_or_null<llvm::DIType>(N);
  } else if (ty->isArrayTy() && type->astTag == E_AggregateType) {
    if (type->symbol->hasFlag(FLAG_C_ARRAY))
      return nullptr;
    AggregateType *this_class = (AggregateType *)type;
    // Subscripts are "ranges" for each dimension of the array
    llvm::SmallVector<llvm::Metadata *, 4> Subscripts;
    int Asize = this_class->fields.length;
    // C-style language always has 1D for array, and index starts with "0"
    Subscripts.push_back(dibuilder->getOrCreateSubrange(0, Asize));
    Symbol *eleSym = toDefExpr(this_class->fields.head)->sym;
    Type *eleType = eleSym->type;
    if (get_type(eleType) == nullptr)
      return nullptr;
    N = dibuilder->createArrayType(
      Asize,
      8*layout.getABITypeAlign(ty).value(),
      get_type(eleType),
      dibuilder->getOrCreateArray(Subscripts));

    type->symbol->llvmDIType = N;
    return llvm::cast_or_null<llvm::DIType>(N);
  }

  if (type == dtVoid || type == dtNothing) {
    return nullptr;
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

llvm::DIType* debug_data::construct_type_for_enum(llvm::Type* ty, EnumType* type) {
  GenInfo* info = gGenInfo;
  const llvm::DataLayout& layout = info->module->getDataLayout();

  const char* name = type->symbol->name;
  ModuleSymbol* defModule = type->symbol->getModule();
  const char* defFile = type->symbol->fname();
  int defLine = type->symbol->linenum();

  auto dibuilder = defModule->llvmDIBuilder;

  Type* bt = type->getIntegerType();
  auto diBT = get_type(bt);
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
    get_module_scope(defModule),
    name,
    get_file(defModule, defFile),
    defLine,
    layout.getTypeSizeInBits(ty), /* SizeInBits */
    8*layout.getABITypeAlign(ty).value(), /* AlignInBits */
    dibuilder->getOrCreateArray(Elements),
    diBT,
#if LLVM_VERSION_MAJOR >= 18
    0 /* RuntimeLang */,
#endif
    "", /* UniqueIdentifer */
    true /* isScoped */
  );
  type->symbol->llvmDIType = N;
  return llvm::cast_or_null<llvm::DIType>(N);
}

llvm::DIType* debug_data::get_type(Type *type) {
  if (!type)
    return nullptr;

  if (type->symbol->llvmDIType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIType);

  if (type->symbol->llvmDIForwardType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIForwardType);

  construct_type(type);

  if (type->symbol->llvmDIType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIType);

  if (type->symbol->llvmDIForwardType)
    return llvm::cast_or_null<llvm::DIType>(type->symbol->llvmDIForwardType);

  return nullptr;
}

llvm::DIFile* debug_data::construct_file(ModuleSymbol* modSym, const char *fpath) {
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

llvm::DIFile* debug_data::get_file(ModuleSymbol* modSym, const char *fpath) {
  // First, check to see if it's already a in our hashtable.
  if( this->filesByName.count(fpath) > 0 ) {
    return this->filesByName[fpath];
  }

  // Otherwise, construct the type, add it to the map,and return it
  llvm::DIFile* dif = construct_file(modSym, fpath);
  this->filesByName[fpath] = dif;
  return dif;
}

llvm::DINamespace* debug_data::construct_module_scope(ModuleSymbol* modSym)
{
  const char* fname = modSym->fname();
  llvm::DIFile* file = get_file(modSym, fname);
  return modSym->llvmDIBuilder->createNameSpace(file, /* Scope */
                                         modSym->name, /* Name */
                                         false /* ExportSymbols */
                                        );
}

llvm::DINamespace* debug_data::get_module_scope(ModuleSymbol* modSym)
{
  if( nullptr == modSym->llvmDINameSpace ) {
    modSym->llvmDINameSpace = construct_module_scope(modSym);
  }
  return llvm::cast_or_null<llvm::DINamespace>(modSym->llvmDINameSpace);
}

llvm::DISubroutineType* debug_data::get_function_type(FnSymbol *function)
{
  ModuleSymbol* modSym = function->getModule();
  llvm::SmallVector<llvm::Metadata *,16> ret_arg_types;

  ret_arg_types.push_back(get_type(function->retType));
  for_formals(arg, function)
  {
    ret_arg_types.push_back(get_type(arg->type));
  }
  llvm::DITypeRefArray ret_arg_arr = modSym->llvmDIBuilder->getOrCreateTypeArray(ret_arg_types);
  return modSym->llvmDIBuilder->createSubroutineType(ret_arg_arr);
}

llvm::DISubprogram* debug_data::construct_function(FnSymbol *function)
{
  const char *name = function->name;
  const char *cname = function->cname;
  ModuleSymbol* modSym = (ModuleSymbol*) function->defPoint->parentSymbol;
  const char *file_name = function->astloc.filename();
  int line_number = function->astloc.lineno();
  // Get the function using the cname since that is how it is
  // stored in the generated code. The name is just used within Chapel.

  llvm::DINamespace* module = get_module_scope(modSym);
  llvm::DIFile* file = get_file(modSym, file_name);

  llvm::DISubroutineType* function_type = get_function_type(function);

  llvm::DISubprogram::DISPFlags SPFlags = llvm::DISubprogram::SPFlagDefinition;
  if (!function->hasFlag(FLAG_EXPORT))
    SPFlags |= llvm::DISubprogram::SPFlagLocalToUnit;
  if (optimized)
    SPFlags |= llvm::DISubprogram::SPFlagOptimized;

  llvm::DISubprogram* ret = modSym->llvmDIBuilder->createFunction(
    module, /* scope */
    name, /* name */
    cname, /* linkage name */
    file, line_number, function_type,
    line_number, /* beginning of scope we start */
    llvm::DINode::FlagZero, /* flags */
    SPFlags /* subprogram flags */
    );
  return ret;
}

llvm::DISubprogram* debug_data::get_function(FnSymbol *function)
{
  if( nullptr == function->llvmDISubprogram ) {
    function->llvmDISubprogram = construct_function(function);
  }
  return llvm::cast_or_null<llvm::DISubprogram>(function->llvmDISubprogram);
}


llvm::DIGlobalVariableExpression* debug_data::construct_global_variable(VarSymbol *gVarSym)
{
  const char *name = gVarSym->name;
  const char *cname = gVarSym->cname;
  const char *file_name = gVarSym->astloc.filename();
  int line_number = gVarSym->astloc.lineno();

  ModuleSymbol* modSym = gVarSym->getModule();
  auto dibuilder = modSym->llvmDIBuilder;
  auto compileUnit = modSym->llvmDICompileUnit;

  llvm::DIFile* file = get_file(modSym, file_name);
  llvm::DIType* gVarSym_type = get_type(gVarSym->type); // type is member of Symbol

  if(gVarSym_type)
    return dibuilder->createGlobalVariableExpression
     (
      compileUnit, /* Context */
      name, /* name */
      cname, /* linkage name */
      file, /* File */
      line_number, /* LineNo */
      gVarSym_type, /* Ty */
      !gVarSym->hasFlag(FLAG_EXPORT) /* is local to unit */
     );
  else {
    //return an Empty dbg node if the symbol type is unresolved
    return nullptr;
  }
}

llvm::DIGlobalVariableExpression* debug_data::get_global_variable(VarSymbol *gVarSym)
{
  if( nullptr == gVarSym->llvmDIGlobalVariable ) {
    gVarSym->llvmDIGlobalVariable = construct_global_variable(gVarSym);
  }
  return llvm::cast_or_null<llvm::DIGlobalVariableExpression>(
              gVarSym->llvmDIGlobalVariable);
}

llvm::DIVariable* debug_data::construct_variable(VarSymbol *varSym)
{
  const char *name = varSym->name;
  const char *file_name = varSym->astloc.filename();
  int line_number = varSym->astloc.lineno();
  FnSymbol *funcSym = varSym->defPoint->getFunction();
  ModuleSymbol* modSym = varSym->getModule();

  llvm::DISubprogram* scope = get_function(funcSym);
  llvm::DIFile* file = get_file(modSym, file_name);
  llvm::DIType* varSym_type = get_type(varSym->type);

  if(varSym_type) {
    llvm::DILocalVariable* localVariable = modSym->llvmDIBuilder->createAutoVariable(
      scope, /* Scope */
      name, /*Name*/
      file, /*File*/
      line_number, /*Lineno*/
      varSym_type, /*Type*/
      true/*AlwaysPreserve, won't be removed when optimized*/
      ); //omit the  Flags and ArgNo

    modSym->llvmDIBuilder->insertDeclare(varSym->codegen().val, localVariable,
      modSym->llvmDIBuilder->createExpression(), llvm::DILocation::get(
        scope->getContext(), line_number, 0, scope, nullptr, false),
      gGenInfo->irBuilder->GetInsertBlock());
    return localVariable;
  }
  else {
    //Empty dbg node if the symbol type is unresolved
    return nullptr;
  }
}

llvm::DIVariable* debug_data::get_variable(VarSymbol *varSym)
{
  if( nullptr == varSym->llvmDIVariable ){
    varSym->llvmDIVariable = construct_variable(varSym);
  }
  return llvm::cast_or_null<llvm::DIVariable>(varSym->llvmDIVariable);
}

llvm::DIVariable* debug_data::construct_formal_arg(ArgSymbol *argSym, unsigned ArgNo)
{
  const char *name = argSym->name;
  const char *file_name = argSym->astloc.filename();
  int line_number = argSym->astloc.lineno();
  FnSymbol *funcSym = nullptr;
  if(isFnSymbol(argSym->defPoint->parentSymbol))
    funcSym = (FnSymbol*)argSym->defPoint->parentSymbol;
  else
    printf("Couldn't find the function parent of param: %s!\n",name);
  ModuleSymbol* modSym = argSym->getModule();

  llvm::DISubprogram* scope = get_function(funcSym);
  llvm::DIFile* file = get_file(modSym, file_name);
  llvm::DIType* argSym_type = get_type(argSym->type);

  if(argSym_type) {
    auto diParameterVariable = modSym->llvmDIBuilder->createParameterVariable(
      scope, /* Scope */
      name, /*Name*/
      ArgNo, /* ArgNo */
      file, /*File*/
      line_number, /*Lineno*/
      argSym_type, /*Type*/
      true,/*AlwaysPreserve, won't be removed when optimized*/
      llvm::DINode::FlagZero /*Flags*/
      );
    auto Storage = gGenInfo->lvt->getValue(argSym->cname);
    modSym->llvmDIBuilder->insertDeclare(Storage.val, diParameterVariable,
      modSym->llvmDIBuilder->createExpression(), llvm::DILocation::get(
        scope->getContext(), line_number, 0, scope, nullptr, false),
      gGenInfo->irBuilder->GetInsertBlock());
    return diParameterVariable;
  } else {
    //Empty dbg node if the symbol type is unresolved
    return nullptr;
  }
}

llvm::DIVariable* debug_data::get_formal_arg(ArgSymbol *argSym, unsigned int ArgNo)
{
  if( nullptr == argSym->llvmDIFormal ){
    argSym->llvmDIFormal = construct_formal_arg(argSym, ArgNo);
  }
  return llvm::cast_or_null<llvm::DIVariable>(argSym->llvmDIFormal);
}
#endif
