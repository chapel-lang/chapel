#include "expr.h"
#include "codegen.h"
#include "symbol.h"
#include "../main/version.h"
#include "vec.h"
#include "type.h"
#include "alist.h"

#ifdef HAVE_LLVM
#include "llvmUtil.h"
#include "llvm/ADT/ArrayRef.h"

// Note: All char strings must be the same size
// Very hacky, but completely legal and functional. Only issue is that file_name will have garbage from beyond full_path.
// garbage issue could be solved with scrubbing, if it matters.

extern char *current_dir;
char *null_name = "<internal>";

void shred_path(const char *full_path, char *file_name, char *path_name, int size)
{
  if(full_path == NULL)
  {
    memcpy(path_name, null_name, strlen(null_name)); 
  } else {
    memcpy(path_name, full_path, size);
  }
  char *last_slash = strrchr(path_name, '/');
  if(last_slash == NULL)
  {
    memcpy(file_name, path_name, size);
    memcpy(path_name, current_dir, 3);
  } else {
    memcpy(file_name, last_slash+1,size);
    last_slash[1]='\0';
  }
}

llvm::DIFile debug_data::get_file(const char *file)
{
  char file_name[128];
  char path_name[128];
  shred_path(file,file_name,path_name, 128);
  return debug_info->get_file(file_name, path_name);
}

void debug_data::create_compile_unit(const char *file, const char *directory, bool is_optimized, const char *flags)
{
  this->optimized = is_optimized;
  char version[128];
  char chapel_string[256];
  get_version(version);
  snprintf(chapel_string, 256, "Chapel version %s", version);
  //the blank string here is meant to be the compile flags, just not sure how to grab that right now.
  this->debug_factory.createCompileUnit(DW_LANG_chapel, file, directory, chapel_string, is_optimized, flags,0);
}

llvm::DIType debug_data::get_type(const char *name, unsigned int bits, unsigned int encoding)
{
  llvm::StringRef this_name(name);
  for(std::vector<llvm::DIType>::iterator idx = this->types.begin(); idx != this->types.end(); idx++)
  {
    if(idx->getName() == this_name)
    {
      assert(idx->getName() == this_name && idx->getSizeInBits() == bits);
      return *idx;
    }
  }
  llvm::DIType new_type = this->debug_factory.createBasicType(name, bits, bits, encoding);
  assert(new_type.Verify());
  this->types.push_back(new_type);
  return new_type;
}

llvm::DIFile debug_data::get_file(const char *file, const char *directory)
{
  llvm::StringRef file_name = llvm::StringRef(file);
  llvm::StringRef directory_name = llvm::StringRef(directory);
  for(std::vector<llvm::DIFile>::iterator current_file = this->files.begin(); current_file != this->files.end(); current_file++)
  {
    if(current_file->getFilename() == file_name && current_file->getDirectory() == directory_name)
    {
      return *current_file;
    }
  }
  return this->debug_factory.createFile(file_name, directory_name);
}

llvm::DIType debug_data::get_struct_type(const ClassType *struct_type)
{
  char file_name[128];
  char path_name[128];
  //shread_path(struct_type->astloc.filename, file_name, path_name, 128);
  //llvm::DINameSpace = get_module_name(stuff, struct_type->astloc.filename, struct_type->astloc.lineno);
}

llvm::DIType debug_data::get_type(Type *type)
{
  if(type->symbol->llvmType == NULL) return llvm::DIType();
  if(type->symbol->llvmType->isIntegerTy()) {
    return get_int_type(type->symbol->name, type->symbol->llvmType->getIntegerBitWidth());
  } else if(type->symbol->llvmType->isFloatingPointTy()) {
    return get_real_type(type->symbol->name, type->symbol->llvmType->getPrimitiveSizeInBits());
  } else if(type->astTag == E_ClassType) {
    ClassType *this_class = (ClassType *)type;
    if(this_class->classTag == CLASS_RECORD) return get_struct_type(this_class);
  }

  printf("Unhandled type: %s\n\ttype->astTag=%i\n", type->symbol->name, type->astTag);
  if(type->symbol->llvmType) {
    printf("\tllvmType->getTypeID() = %i\n", type->symbol->llvmType->getTypeID());
  } else {
    printf("\tllvmType is NULL\n");
  }

  return llvm::DIType();
}

llvm::DIType debug_data::get_int_type(const char *name, unsigned int bits)
{
  return get_type(name, bits, llvm::dwarf::DW_ATE_signed);
}

llvm::DIType debug_data::get_real_type(const char *name, unsigned int bits)
{
  return get_type(name, bits, llvm::dwarf::DW_ATE_float);
}

llvm::DIType debug_data::get_array_type(unsigned int elements, llvm::DIType Type)
{
}

llvm::DINameSpace debug_data::get_module_name(const char *name)
{
  assert(name != NULL);
  llvm::StringRef module_name = llvm::StringRef(name);
  llvm::DINameSpace ret_val;
  for(std::vector<llvm::DINameSpace>::iterator current_module = this->name_spaces.begin(); current_module != this->name_spaces.end(); current_module++)
  {
    if(current_module->getName() == module_name)
    {
      ret_val =  *current_module;
    }
  }
  return ret_val;
}

llvm::DINameSpace debug_data::get_module_name(const char *name, const char *file_name, unsigned int line_number)
{
  //assert(name != NULL);
  // resolved_name needs to stick around. StringRef only points to it, it doesn't keep a copy of it. If leaking this matters it can be freed in a destructor.
  char *resolved_name = malloc(128);
  if(name == NULL)
  {
    printf("New name using default name\n");
    char new_name[128];
    char path_name[128];
    shread_path(file_name, new_name, path_name, 128);
    char *last_dot = strrchr(new_name,'.');
    assert(strcmp(last_dot,".chpl")==0);
    last_dot[0]='\0';
    memcpy(resolved_name,new_name,128);
  } else {
    memcpy(resolved_name,name,128);
  }

  llvm::StringRef module_name = llvm::StringRef(resolved_name);
  for(std::vector<llvm::DINameSpace>::iterator current_module = this->name_spaces.begin(); current_module != this->name_spaces.end(); current_module++)
  {
    if(current_module->getName() == module_name)
    {
      return *current_module;
    }
  }
  llvm::DIFile file = get_file(file_name);
  llvm::DINameSpace ret= this->debug_factory.createNameSpace(file, module_name, file, line_number);
  this->name_spaces.push_back(ret);
  return ret;
}

llvm::DIType debug_data::get_function_type(const char *file, const std::vector<llvm::DIType>types)
{
  llvm::DIFile my_file = get_file(file);
  llvm::SmallVector<llvm::Value *,16> my_types;

  for(int idx=0; idx < types.size(); idx++)
  {
    my_types.push_back(types[idx]);
  }

  llvm::DIArray my_array = this->debug_factory.getOrCreateArray(my_types);
  return this->debug_factory.createSubroutineType(my_file, my_array);
}

llvm::DISubprogram debug_data::get_function(const char *name, const char *cname, const char *module_name, const char *file_name, unsigned int line_number, std::vector<llvm::DIType>parameters, llvm::Function *function)
{
  llvm::DINameSpace module = get_module_name(module_name);
  llvm::DIFile file = get_file(file_name);
  llvm::DIType function_type = get_function_type(file_name, parameters);
  return this->debug_factory.createFunction(module, name, cname, file, line_number, function_type, true, true, line_number, 0, false);
}

llvm::DISubprogram debug_data::get_function(const FnSymbol *function)
{
  std::vector<llvm::DIType> parameters;
  parameters.push_back(get_type(function->retType));
  for_formals(arg, function)
  {
    parameters.push_back(get_type(arg->type));
  }
  return get_function(function->name, function->cname, function->defPoint->parentSymbol->name, function->astloc.filename, function->astloc.lineno, parameters, getFunctionLLVM(function->cname));
}

// end if LLVM
#endif
