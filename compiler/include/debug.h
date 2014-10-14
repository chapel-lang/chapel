#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "type.h"

#ifdef HAVE_LLVM

#include "llvmUtil.h"
#include "llvm/Support/Dwarf.h"
#include "llvm/DebugInfo.h"
#include "llvm/DIBuilder.h"

#include <vector>

//#define DW_LANG_chapel (llvm::dwarf::DW_LANG_lo_user+37)
#define DW_LANG_chapel llvm::dwarf::DW_LANG_lo_user
#endif

class debug_data
{
#ifdef HAVE_LLVM
 public:
  static bool can_debug() { return true; }
  debug_data(llvm::Module &m) : debug_factory(m){}
  void finalize(){debug_factory.finalize();}
  void create_compile_unit(const char *file, const char *directory, bool is_optimized, const char *flags);

  llvm::DIType get_type(Type *type);
  llvm::DIFile get_file(const char *file, const char *directory);
  llvm::DIFile get_file(const char *file);
  llvm::DIType get_int_type(const char *name, unsigned int bits);
  llvm::DIType get_real_type(const char *name, unsigned int bits);
  llvm::DIType get_array_type(unsigned int elements, llvm::DIType Type);
  //llvm::DIType get_struct_type(const char *name, const char *file_name, unsigned int line_number, std::vector<llvm::DIType>types);
  llvm::DIType get_struct_type(const AggregateType *struct_type);
  llvm::DINameSpace get_module_name(const char *name, const char *file_name, unsigned int line_number);
  llvm::DINameSpace get_module_name(const char *name);
  llvm::DISubprogram get_function(const FnSymbol *function);
  llvm::DIType get_function_type(const char *file, const std::vector<llvm::DIType>types);
  llvm::DISubprogram get_function(const char *name, const char *cname, const char *module_name, const char *file_name, unsigned int line_number, std::vector<llvm::DIType>parameters, llvm::Function *function);
 private:
  llvm::DIBuilder debug_factory;
  bool optimized;
  llvm::DIType get_type(const char *name, unsigned int bits, unsigned int encoding);
  std::vector<llvm::DIFile>files;
  std::vector<llvm::DIType>types;
  std::vector<llvm::DINameSpace>name_spaces;
  std::vector<llvm::DISubprogram>functions;
#else
  static bool can_debug() { return false; }
#endif
};

extern debug_data *debug_info;

#endif
