#include "llvmDebug.h"

#include "stringutil.h"
#include "expr.h"
#include "codegen.h"
#include "symbol.h"
//#include "../main/version_num.h"
#include "vec.h"
#include "type.h"
#include "alist.h"
#include "version.h"

#ifdef HAVE_LLVM
#include "llvmUtil.h"
#include "llvm/ADT/ArrayRef.h"

/*
LLVM provides a class called DIBuilder, you pass the LLVM module to this
class and it will attach the debug information to the LLVM code after the
call to finalize() on DIBuilder. The initialization happens in the
constructor of debug_data (inlined in the header). It is instanced during
codegen after setting up the LLVM module and put into debug_info. If the
-g flag wasn't passed to the compiler then debug_info is null. The
finalize call happens in codegen() right after the finishClangGeneration()
call.
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
// Very hacky, but completely legal and functional. Only issue is that file_name will have garbage from beyond full_path.
// garbage issue could be solved with scrubbing, if it matters.

extern char *current_dir;
//const char *null_name = "<internal>";

void debug_data::create_compile_unit(const char *file, const char *directory, bool is_optimized, const char *flags)
{
  this->optimized = is_optimized;
  char version[128];
  char chapel_string[256];
  get_version(version);
  snprintf(chapel_string, 256, "Chapel version %s", version);
  //the blank string here is meant to be the compile flags, just not sure how to grab that right now.
  this->dibuilder.createCompileUnit(DW_LANG_chapel, file, directory, chapel_string, is_optimized, flags,0);
}

llvm::DIType debug_data::construct_type(Type *type)
{
  GenInfo* info = gGenInfo;
  LLVM_TARGET_DATA * layout = info->targetData;

  llvm::Type* ty = type->symbol->llvmType;
  //const char* cname = type->symbol->cname;
  const char* name = type->symbol->name;
  ModuleSymbol* defModule = type->symbol->getModule();
  const char* defFile = type->symbol->fname();
  int defLine = type->symbol->linenum();

  if(!ty) return llvm::DIType();
  if(ty->isIntegerTy()) {
    return this->dibuilder.createBasicType(
        name,
        layout->getTypeSizeInBits(ty),
        8*layout->getABITypeAlignment(ty),
        (is_signed(type))?
                      (llvm::dwarf::DW_ATE_signed):
                      (llvm::dwarf::DW_ATE_unsigned));
  } else if(ty->isFloatingPointTy()) {
    return this->dibuilder.createBasicType(
        name,
        layout->getTypeSizeInBits(ty),
        8*layout->getABITypeAlignment(ty),
        llvm::dwarf::DW_ATE_float);
  } else if(ty->isPointerTy()) {
    return this->dibuilder.createPointerType(
        get_type(type->getValType()),
        layout->getPointerSizeInBits(ty->getPointerAddressSpace()),
        0, /* alignment */
        name);
  } else if(ty->isStructTy() && type->astTag == E_AggregateType) {
    AggregateType *this_class = (AggregateType *)type;
    llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE *, 8> EltTys;
    llvm::DIType derivedFrom;
    if( type->dispatchParents.length() > 0 )
      derivedFrom = get_type(type->dispatchParents.first());

    const llvm::StructLayout* slayout = NULL;
    llvm::StructType* struct_type = llvm::cast<llvm::StructType>(ty);
    slayout = layout->getStructLayout(struct_type);

    for_fields(field, this_class) {
      // field is a Symbol
      const char* fieldDefFile = field->defPoint->fname();
      int fieldDefLine = field->defPoint->linenum();
      TypeSymbol* fts = field->type->symbol;
      llvm::Type* fty = fts->llvmType;

      llvm::DIType mty = this->dibuilder.createMemberType(
        get_module_scope(defModule),
        field->name,
        get_file(fieldDefFile),
        fieldDefLine,
        layout->getTypeSizeInBits(fty),
        8*layout->getABITypeAlignment(fty),
        slayout->getElementOffsetInBits(this_class->getMemberGEP(field->name)),
        0,
        get_type(field->type));
      EltTys.push_back(mty);
    }

    if(this_class->aggregateTag == AGGREGATE_RECORD) {
      return this->dibuilder.createStructType(
          get_module_scope(defModule),
          name,
          get_file(defFile),
          defLine,
          layout->getTypeSizeInBits(ty),
          8*layout->getABITypeAlignment(ty),
          0,
          derivedFrom,
          this->dibuilder.getOrCreateArray(EltTys));
    } else if(this_class->aggregateTag == AGGREGATE_CLASS) {
      return this->dibuilder.createStructType(
          get_module_scope(defModule),
          name,
          get_file(defFile),
          defLine,
          layout->getTypeSizeInBits(ty),
          8*layout->getABITypeAlignment(ty),
          0,
          derivedFrom,
          this->dibuilder.getOrCreateArray(EltTys));
    } else if(this_class->aggregateTag == AGGREGATE_UNION) {
      return this->dibuilder.createUnionType(
          get_module_scope(defModule),
          name,
          get_file(defFile),
          defLine,
          layout->getTypeSizeInBits(ty),
          8*layout->getABITypeAlignment(ty),
          0,
          this->dibuilder.getOrCreateArray(EltTys));
    }
  }

  printf("Unhandled type: %s\n\ttype->astTag=%i\n", type->symbol->name, type->astTag);
  if(type->symbol->llvmType) {
    printf("\tllvmType->getTypeID() = %i\n", type->symbol->llvmType->getTypeID());
  } else {
    printf("\tllvmType is NULL\n");
  }

  //return this->dibuilder.createUnspecifiedType(name);
  llvm::DIType ret;
  return ret;
}

llvm::DIType debug_data::get_type(Type *type)
{
  if( NULL == type->symbol->llvmDIType ) {
    type->symbol->llvmDIType = construct_type(type);
  }
  return llvm::DIType(type->symbol->llvmDIType);
}

llvm::DIFile debug_data::construct_file(const char *fpath)
{
  // Create strings for the directory and file.
  const char* last_slash;
  const char* file;
  const char* directory;

  last_slash = strrchr(fpath, '/');
  if( last_slash ) {
    file = astr(last_slash+1);
    directory = asubstr(fpath, last_slash);
  } else {
    file = fpath;
    directory = astr(current_dir);
  }

  return this->dibuilder.createFile(file, directory);
}

llvm::DIFile debug_data::get_file(const char *fpath)
{
  // First, check to see if it's already a in our hashtable.
  if( this->filesByName.count(fpath) > 0 ) {
    return this->filesByName[fpath];
  }

  // Otherwise, construct the type, add it to the map,
  // and then return it.
  llvm::DIFile dif = construct_file(fpath);
  this->filesByName[fpath] = dif;
  return dif;
}

llvm::DINameSpace debug_data::construct_module_scope(ModuleSymbol* modSym)
{
  const char* fname = modSym->fname();
  int line = modSym->linenum();
  llvm::DIFile file = get_file(fname);
  return this->dibuilder.createNameSpace(file, modSym->name, file, line);
}

llvm::DINameSpace debug_data::get_module_scope(ModuleSymbol* modSym)
{
  if( NULL == modSym->llvmDINameSpace ) {
    modSym->llvmDINameSpace = construct_module_scope(modSym);
  }
  return llvm::DINameSpace(modSym->llvmDINameSpace);
}

LLVM_DI_SUBROUTINE_TYPE debug_data::get_function_type(FnSymbol *function)
{
  const char *file_name = function->astloc.filename;
  llvm::DIFile file = get_file(file_name);
  llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE *,16> ret_arg_types;

  ret_arg_types.push_back(get_type(function->retType));
  for_formals(arg, function)
  {
    ret_arg_types.push_back(get_type(arg->type));
  }

#if HAVE_LLVM_VER >= 36
  llvm::DITypeArray ret_arg_arr = dibuilder.getOrCreateTypeArray(ret_arg_types);
  return this->dibuilder.createSubroutineType(file, ret_arg_arr);
#else
  llvm::DIArray ret_arg_arr = dibuilder.getOrCreateArray(ret_arg_types);
  return this->dibuilder.createSubroutineType(file, ret_arg_arr);
#endif
}

llvm::DISubprogram debug_data::construct_function(FnSymbol *function)
{
  const char *name = function->name;
  const char *cname = function->cname;
  ModuleSymbol* modSym = (ModuleSymbol*) function->defPoint->parentSymbol;
  const char *file_name = function->astloc.filename;
  int line_number = function->astloc.lineno;
  llvm::Function* llFunc = getFunctionLLVM(function->cname);

  llvm::DINameSpace module = get_module_scope(modSym);
  llvm::DIFile file = get_file(file_name);

  LLVM_DI_SUBROUTINE_TYPE function_type = get_function_type(function);

  printf("CONSTRUCTING DEBUG DATA\n");

  llvm::DISubprogram ret = this->dibuilder.createFunction(
      module, /* scope */
      name, /* name */
      cname, /* linkage name */
      file, line_number, function_type,
      !function->hasFlag(FLAG_EXPORT), /* is local to unit */
      true, /* is definition */
      line_number, /* beginning of scope we start */
      0, /* flags */
      optimized, /* isOptimized */
      llFunc);
  return ret;
}

llvm::DISubprogram debug_data::get_function(FnSymbol *function)
{
  if( NULL == function->llvmDISubprogram ) {
    function->llvmDISubprogram = construct_function(function);
  }
  return llvm::DISubprogram(function->llvmDISubprogram); 
}

// end if LLVM
#endif
