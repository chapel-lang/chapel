#include <iostream>
#include <map>

#include "llvmDebug.h"
#include "stringutil.h"
#include "expr.h"
#include "codegen.h"
#include "symbol.h"
#include "vec.h"
#include "type.h"
#include "alist.h"
#include "version.h"

#ifdef HAVE_LLVM
#include "llvmUtil.h"
#include "llvm/ADT/ArrayRef.h"
#include "genret.h" 
#include "llvm/Support/Dwarf.h"
#include "llvm/Support/raw_os_ostream.h"
#include <iostream>
#include "llvm/ADT/DenseMap.h"
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

extern char *current_dir;
llvm::DenseMap<const Type *, llvm::MDNode *> myTypeDescriptors;

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

static
llvm::MDNode *myGetType(const Type *type) {
  typedef llvm::DenseMap<const Type*, llvm::MDNode *>::const_iterator TypeNodeIter;
  TypeNodeIter i = myTypeDescriptors.find(type);
  if(i != myTypeDescriptors.end())
    return i->second;
  return NULL;
}


void debug_data::finalize() {
  printf("IN FINALIZE\n");
  dibuilder.finalize();
}

void debug_data::create_compile_unit(const char *file, const char *directory, bool is_optimized, const char *flags)
{
  this->optimized = is_optimized;
  char version[128];
  char chapel_string[256];
  get_version(version);
  snprintf(chapel_string, 256, "Chapel version %s", version);
  
  this->dibuilder.createCompileUnit(DW_LANG_chapel, file, directory, chapel_string, is_optimized, flags,0);
}


LLVM_DITYPE debug_data::construct_type(Type *type)
{
  llvm::MDNode *N = myGetType(type);
  if(N) return toDITYPE(N);

  GenInfo* info = gGenInfo;
  LLVM_TARGET_DATA *layout = info->targetData; 
  
  llvm::Type* ty = type->symbol->llvmType;
  const char* name = type->symbol->name;
  ModuleSymbol* defModule = type->symbol->getModule();
  const char* defFile = type->symbol->fname();
  int defLine = type->symbol->linenum();
  
  if(!ty) {
#if HAVE_LLVM_VER >= 37
    return NULL;
#else
    return llvm::DIType();
#endif
  }
  if(ty->isIntegerTy()) {
    N = this->dibuilder.createBasicType(
      name,
      layout->getTypeSizeInBits(ty),
      8*layout->getABITypeAlignment(ty),
      (is_signed(type))? 
      (llvm::dwarf::DW_ATE_signed):
      (llvm::dwarf::DW_ATE_unsigned));

    myTypeDescriptors[type] = N;
    return toDITYPE(N);
  }

  else if(ty->isFloatingPointTy()) {
    N = this->dibuilder.createBasicType(
      name,
      layout->getTypeSizeInBits(ty),
      8*layout->getABITypeAlignment(ty),
      llvm::dwarf::DW_ATE_float);
    
    myTypeDescriptors[type] = N;
    return toDITYPE(N);
  }

  else if(ty->isPointerTy()) {
    if(type != type->getValType()) {//Add this condition to avoid segFault 
      N = this->dibuilder.createPointerType(
        get_type(type->getValType()),//it should return the pointee's DIType
        layout->getPointerSizeInBits(ty->getPointerAddressSpace()),
        0, /* alignment */
        name);

      myTypeDescriptors[type] = N;
      return toDITYPE(N);
    }
    else {
      if(type->astTag == E_PrimitiveType) {
        llvm::Type *PointeeTy = ty->getPointerElementType();
        // handle string, c_string, c_string_copy, nil, opaque, c_void_ptr
        if(PointeeTy->isIntegerTy()) {
          LLVM_DITYPE pteIntDIType; //create the DI-pointeeType
          pteIntDIType = this->dibuilder.createBasicType(
            myGetTypeName(PointeeTy), 
            layout->getTypeSizeInBits(PointeeTy),
            8*layout->getABITypeAlignment(PointeeTy),
            llvm::dwarf::DW_ATE_unsigned);

          N = this->dibuilder.createPointerType(
            pteIntDIType, 
            layout->getPointerSizeInBits(ty->getPointerAddressSpace()),
            0,
            name);

          myTypeDescriptors[type] = N;
          return toDITYPE(N);
        }
        // handle qio_channel_ptr_t, _task_list, qio_file_ptr_t, syserr, _file
        else if(PointeeTy->isStructTy()) {
          LLVM_DITYPE pteStrDIType; //create the DI-pointeeType
          pteStrDIType = this->dibuilder.createStructType(
            get_module_scope(defModule),
            PointeeTy->getStructName(),
            get_file(defFile),
            0,
            (PointeeTy->isSized()?
            layout->getTypeSizeInBits(PointeeTy):
            8),
            (PointeeTy->isSized()?
            8*layout->getABITypeAlignment(PointeeTy):
            8),
            0,
            toDITYPE(NULL),
#if HAVE_LLVM_VER >= 37
            NULL
#else
            llvm::DIArray(NULL)
#endif
            );
        
          N = this->dibuilder.createPointerType(
            pteStrDIType,
            layout->getPointerSizeInBits(ty->getPointerAddressSpace()),
            0,
            name);
        
          myTypeDescriptors[type] = N;
          return toDITYPE(N);
        }
      }
      else if(type->astTag == E_AggregateType) {
        // dealing with classes
        AggregateType *this_class = (AggregateType *)type;
        llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE *, 8> EltTys;
        LLVM_DITYPE derivedFrom;
        if( type->dispatchParents.length() > 0 )
          derivedFrom = get_type(type->dispatchParents.first());

        // solve the data class: _ddata
        if(this_class->symbol->hasFlag(FLAG_DATA_CLASS)) {
          Type* vt = getDataClassType(this_class->symbol)->typeInfo();
          if(vt) {
            N = this->dibuilder.createPointerType(
              get_type(vt),
              layout->getPointerSizeInBits(ty->getPointerAddressSpace()),
              0,
              name);
            
            myTypeDescriptors[type] = N;
            return toDITYPE(N);
          }
        } //Not sure whether we should directly return getType(vt)
        
        const llvm::StructLayout* slayout = NULL;
        const char *struct_name = this_class->classStructName(true);
        llvm::Type* st = getTypeLLVM(struct_name);
        if(st){
          llvm::StructType* struct_type = llvm::cast<llvm::StructType>(st);
          if(!struct_type->isOpaque()){
            N = this->dibuilder.createForwardDecl(
              llvm::dwarf::DW_TAG_structure_type, 
              name,
              get_module_scope(defModule),
              get_file(defFile),
              defLine,
              0, // RuntimeLang
              layout->getTypeSizeInBits(ty),
              8*layout->getABITypeAlignment(ty));
     
            //N is added to the map (early) so that element search below can find it,
            //so as to avoid infinite recursion for structs that contain pointers to
            //their own type.
            myTypeDescriptors[type] = N;

            slayout = layout->getStructLayout(struct_type); 
            for_fields(field, this_class) {
              // field is a Symbol
              const char* fieldDefFile = field->defPoint->fname();
              int fieldDefLine = field->defPoint->linenum();
              TypeSymbol* fts = field->type->symbol;
              llvm::Type* fty = fts->llvmType;
              LLVM_DITYPE mty; 
              //use the dummy type for 'BaseArr'
              mty = this->dibuilder.createMemberType(
                get_module_scope(defModule),
                field->name,
                get_file(fieldDefFile),
                fieldDefLine,
                layout->getTypeSizeInBits(fty),
                8*layout->getABITypeAlignment(fty),
                slayout->getElementOffsetInBits(this_class->getMemberGEP(field->cname)),
                0,
                get_type(field->type));    
              
              EltTys.push_back(mty);
            }

            // Now create the DItype for the struct
            N = this->dibuilder.createStructType(
              get_module_scope(defModule),
              name,
              get_file(defFile),
              defLine,
              layout->getTypeSizeInBits(ty),
              8*layout->getABITypeAlignment(ty),
              0, // RuntimeLang
              derivedFrom,
              this->dibuilder.getOrCreateArray(EltTys));
            
            return toDITYPE(N);
          }//end of if(!Opaque)
        }// end of if(st)
      } // end of astTag == E_AggregateTy
    } // end of else (type==type->getType)
  } // end of ty->isPointerTy()
  
  else if(ty->isStructTy() && type->astTag == E_AggregateType) {
    AggregateType *this_class = (AggregateType *)type;
    llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE *, 8> EltTys;
    LLVM_DITYPE derivedFrom;
    if( type->dispatchParents.length() > 0 )
      derivedFrom = get_type(type->dispatchParents.first());

    const llvm::StructLayout* slayout = NULL;
    llvm::StructType* struct_type = llvm::cast<llvm::StructType>(ty);
    slayout = layout->getStructLayout(struct_type);
    
    N = this->dibuilder.createForwardDecl(
      llvm::dwarf::DW_TAG_structure_type, 
      name,
      get_module_scope(defModule),
      get_file(defFile),
      defLine,
      0, // RuntimeLang
      layout->getTypeSizeInBits(ty),
      8*layout->getABITypeAlignment(ty));

    //N is added to the map (early) so that element search below can find it,
    //so as to avoid infinite recursion for structs that contain pointers to
    //their own type.
    myTypeDescriptors[type] = N;

    for_fields(field, this_class) {
      const char* fieldDefFile = field->defPoint->fname();
      int fieldDefLine = field->defPoint->linenum();
      TypeSymbol* fts = field->type->symbol; 
      llvm::Type* fty = fts->llvmType;

      if(!fty){
        fty = getTypeLLVM(fts->cname);
        if(!fty)
          printf("Error: %s has no llvm type\n",fts->name);
      }
      LLVM_DITYPE mty = this->dibuilder.createMemberType(
        get_module_scope(defModule),
        field->name,
        get_file(fieldDefFile),
        fieldDefLine,
        layout->getTypeSizeInBits(fty),
        8*layout->getABITypeAlignment(fty),
        slayout->getElementOffsetInBits(this_class->getMemberGEP(field->cname)),
        0,
        get_type(field->type));

      EltTys.push_back(mty);
    }

    if(this_class->aggregateTag == AGGREGATE_RECORD) {
      N = this->dibuilder.createStructType(
        get_module_scope(defModule),
        name,
        get_file(defFile),
        defLine,
        layout->getTypeSizeInBits(ty),
        8*layout->getABITypeAlignment(ty),
        0,
        derivedFrom,
        this->dibuilder.getOrCreateArray(EltTys));
    
      myTypeDescriptors[type] = N;
      return toDITYPE(N);
    } 
    else if(this_class->aggregateTag == AGGREGATE_CLASS) {
      N = this->dibuilder.createStructType(
        get_module_scope(defModule),
        name,
        get_file(defFile),
        defLine,
        layout->getTypeSizeInBits(ty),
        8*layout->getABITypeAlignment(ty),
        0,
        derivedFrom,
        this->dibuilder.getOrCreateArray(EltTys));
    
      myTypeDescriptors[type] = N;
      return toDITYPE(N);
    } 
    else if(this_class->aggregateTag == AGGREGATE_UNION) {
      N = this->dibuilder.createUnionType(
        get_module_scope(defModule),
        name,
        get_file(defFile),
        defLine,
        layout->getTypeSizeInBits(ty),
        8*layout->getABITypeAlignment(ty),
        0,
        this->dibuilder.getOrCreateArray(EltTys));
        
      myTypeDescriptors[type] = N;
      return toDITYPE(N);
    }
  }

  else if(ty->isArrayTy() && type->astTag == E_AggregateType) {
    AggregateType *this_class = (AggregateType *)type;
    // Subscripts are "ranges" for each dimention of the array
    llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE *, 4> Subscripts;
    int Asize = this_class->fields.length;
    // C-style language always has 1D for array, and index starts with "0"
    Subscripts.push_back(this->dibuilder.getOrCreateSubrange(0, Asize));
    Symbol *eleSym = toDefExpr(this_class->fields.head)->sym;
    Type *eleType = eleSym->type;
    N = this->dibuilder.createArrayType(
      Asize, 
      8*layout->getABITypeAlignment(ty), 
      get_type(eleType),  
      this->dibuilder.getOrCreateArray(Subscripts)); 
      
    myTypeDescriptors[type] = N;
    return toDITYPE(N);
  }

  printf("Unhandled type: %s\n\ttype->astTag=%i\n", type->symbol->name, type->astTag);
  if(type->symbol->llvmType) {
    printf("\tllvmType->getTypeID() = %i\n", type->symbol->llvmType->getTypeID());
  }
  else {
    printf("\tllvmType is NULL\n");
  }

  LLVM_DITYPE ret;
#if HAVE_LLVM_VER >= 37
  ret = NULL;
#endif

  return ret;
}

LLVM_DITYPE debug_data::get_type(Type *type)
{
  if( NULL == type->symbol->llvmDIType ) {
    type->symbol->llvmDIType = construct_type(type);
  }
  return toDITYPE(type->symbol->llvmDIType);
}

LLVM_DIFILE debug_data::construct_file(const char *fpath)
{
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

  return this->dibuilder.createFile(file, directory);
}

LLVM_DIFILE debug_data::get_file(const char *fpath)
{
  // First, check to see if it's already a in our hashtable.
  if( this->filesByName.count(fpath) > 0 ) {
    return this->filesByName[fpath];
  }

  // Otherwise, construct the type, add it to the map,and return it
  LLVM_DIFILE dif = construct_file(fpath);
  this->filesByName[fpath] = dif;
  return dif;
}

LLVM_DINAMESPACE debug_data::construct_module_scope(ModuleSymbol* modSym)
{
  const char* fname = modSym->fname();
  int line = modSym->linenum();
  LLVM_DIFILE file = get_file(fname);
  return this->dibuilder.createNameSpace(file, modSym->name, file, line);
}

LLVM_DINAMESPACE debug_data::get_module_scope(ModuleSymbol* modSym)
{
  if( NULL == modSym->llvmDINameSpace ) {
    modSym->llvmDINameSpace = construct_module_scope(modSym);
  }
  return toDINAMESPACE(modSym->llvmDINameSpace);
}

LLVM_DI_SUBROUTINE_TYPE debug_data::get_function_type(FnSymbol *function)
{
  const char *file_name = function->astloc.filename;
  LLVM_DIFILE file = get_file(file_name);
  llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE *,16> ret_arg_types;

  ret_arg_types.push_back(get_type(function->retType));
  for_formals(arg, function)
  {
    ret_arg_types.push_back(get_type(arg->type));
  } 
#if HAVE_LLVM_VER >= 37
  llvm::DITypeRefArray ret_arg_arr = dibuilder.getOrCreateTypeArray(ret_arg_types);
  return this->dibuilder.createSubroutineType(file, ret_arg_arr);
#elif HAVE_LLVM_VER >= 36
  llvm::DITypeArray ret_arg_arr = dibuilder.getOrCreateTypeArray(ret_arg_types);
  return this->dibuilder.createSubroutineType(file, ret_arg_arr);
#else
  llvm::DIArray ret_arg_arr = dibuilder.getOrCreateArray(ret_arg_types);
  return this->dibuilder.createSubroutineType(file, ret_arg_arr);
#endif
}

LLVM_DISUBPROGRAM debug_data::construct_function(FnSymbol *function)
{
  const char *name = function->name;
  const char *cname = function->cname;
  ModuleSymbol* modSym = (ModuleSymbol*) function->defPoint->parentSymbol;
  const char *file_name = function->astloc.filename;
  int line_number = function->astloc.lineno;
  // Get the function using the cname since that is how it is
  // stored in the generated code. The name is just used within Chapel.
  llvm::Function* llFunc = getFunctionLLVM(function->cname);

  LLVM_DINAMESPACE module = get_module_scope(modSym);
  LLVM_DIFILE file = get_file(file_name);

  LLVM_DI_SUBROUTINE_TYPE function_type = get_function_type(function);

  printf("CONSTRUCTING FUNCTION DEBUG DATA on %s\n",name);

  LLVM_DISUBPROGRAM ret = this->dibuilder.createFunction(
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

LLVM_DISUBPROGRAM debug_data::get_function(FnSymbol *function)
{
  if( NULL == function->llvmDISubprogram ) {
    function->llvmDISubprogram = construct_function(function);
  }
  return toDISUBPROGRAM(function->llvmDISubprogram); 
}


LLVM_DIGLOBALVARIABLE debug_data::construct_global_variable(VarSymbol *gVarSym)
{
  GenInfo *info = gGenInfo; 
  const char *name = gVarSym->name;
  const char *cname = gVarSym->cname;
  const char *file_name = gVarSym->astloc.filename;
  int line_number = gVarSym->astloc.lineno;
  
  LLVM_DIFILE file = get_file(file_name);
  LLVM_DITYPE gVarSym_type = get_type(gVarSym->type); // type is member of Symbol
  GenRet got = info->lvt->getValue(cname); //?use cname since get_function uses it?

#if HAVE_LLVM_VER >= 36
  llvm::Constant *llVal = NULL;
  if( got.val ) {
    llVal = llvm::cast<llvm::Constant>(got.val);
  }
#else
  llvm::Value *llVal = NULL;
  llVal = got.val;  
#endif

  if(gVarSym_type)
    return this->dibuilder.createGlobalVariable(
#if HAVE_LLVM_VER >= 36
      file,
#endif
      name, /* name */
      cname, /* linkage name */
      file, line_number, gVarSym_type, 
      !gVarSym->hasFlag(FLAG_EXPORT), /* is local to unit */
      llVal); /* must be llvm::Constant since LLVM 3.6 */
  ///////////////////////////////////////////////
  //else 
    //printf("For this unsolved GV: type-name = %s astTag = %i\n",gVarSym->type->symbol->name, gVarSym->type->astTag);
  
  else {
    LLVM_DIGLOBALVARIABLE ret;
    //return an Empty dbg node if the symbol type is unresolved
#if HAVE_LLVM_VER >= 37
    ret = NULL;
#endif
    return ret;
  }
}

LLVM_DIGLOBALVARIABLE debug_data::get_global_variable(VarSymbol *gVarSym)
{
  if( NULL == gVarSym->llvmDIGlobalVariable ) {
    gVarSym->llvmDIGlobalVariable = construct_global_variable(gVarSym);
  }
  return toDIGLOBALVARIABLE(gVarSym->llvmDIGlobalVariable);
}

LLVM_DIVARIABLE debug_data::construct_variable(VarSymbol *varSym)
{
  const char *name = varSym->name;
  const char *file_name = varSym->astloc.filename;
  int line_number = varSym->astloc.lineno;
  FnSymbol *funcSym = NULL;
  if(isFnSymbol(varSym->defPoint->parentSymbol))
    funcSym = (FnSymbol*)varSym->defPoint->parentSymbol;//TODO:if parent is a block
  else 
    printf("Couldn't find the function parent of variable: %s!\n",name);
  
  LLVM_DISUBPROGRAM scope = get_function(funcSym);  
  LLVM_DIFILE file = get_file(file_name);
  LLVM_DITYPE varSym_type = get_type(varSym->type);
      
  if(varSym_type)
    return this->dibuilder.createLocalVariable(
      llvm::dwarf::DW_TAG_auto_variable, /*Tag*/
      scope, /* Scope */
      name, /*Name*/
      file, /*File*/
      line_number, /*Lineno*/
      varSym_type, /*Type*/
      true/*AlwaysPreserve, won't be removed when optimized*/
      ); //omit the  Flags and ArgNo
  else {
    //Empty dbg node if the symbol type is unresolved
    LLVM_DIVARIABLE ret;
#if HAVE_LLVM_VER >= 37
    ret = NULL;
#endif
    return ret;
  }
}

LLVM_DIVARIABLE debug_data::get_variable(VarSymbol *varSym)
{
  if( NULL == varSym->llvmDIVariable ){
    varSym->llvmDIVariable = construct_variable(varSym);
  }
  return toDIVARIABLE(varSym->llvmDIVariable);
}

LLVM_DIVARIABLE debug_data::construct_formal_arg(ArgSymbol *argSym, unsigned ArgNo)
{
  const char *name = argSym->name;
  const char *file_name = argSym->astloc.filename;
  int line_number = argSym->astloc.lineno;
  FnSymbol *funcSym = NULL;
  if(isFnSymbol(argSym->defPoint->parentSymbol))
    funcSym = (FnSymbol*)argSym->defPoint->parentSymbol;
  else 
    printf("Couldn't find the function parent of param: %s!\n",name);
  
  LLVM_DISUBPROGRAM scope = get_function(funcSym);  
  LLVM_DIFILE file = get_file(file_name);
  LLVM_DITYPE argSym_type = get_type(argSym->type);
      
  if(argSym_type)
    return this->dibuilder.createLocalVariable(
      llvm::dwarf::DW_TAG_arg_variable, /*Tag: formal arg to a function */
      scope, /* Scope */
      name, /*Name*/
      file, /*File*/
      line_number, /*Lineno*/
      argSym_type, /*Type*/
      true,/*AlwaysPreserve, won't be removed when optimized*/
      0,  /*Flags*/
      ArgNo
      ); 
  else {
    //Empty dbg node if the symbol type is unresolved
    LLVM_DIVARIABLE ret;
#if HAVE_LLVM_VER >= 37
    ret = NULL;
#endif
    return ret;
  }
}

LLVM_DIVARIABLE debug_data::get_formal_arg(ArgSymbol *argSym, unsigned int ArgNo)
{
  if( NULL == argSym->llvmDIFormal ){
    argSym->llvmDIFormal = construct_formal_arg(argSym, ArgNo);
  }
  return toDIVARIABLE(argSym->llvmDIFormal);
}

#endif
