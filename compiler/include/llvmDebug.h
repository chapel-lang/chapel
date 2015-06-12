#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "type.h"

#ifdef HAVE_LLVM

#include "llvmUtil.h"
#include "llvm/Support/Dwarf.h"

#if HAVE_LLVM_VER >= 35
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DIBuilder.h"
#else
#include "llvm/DebugInfo.h"
#include "llvm/DIBuilder.h"
#endif


#include <vector>

//#define DW_LANG_chapel (llvm::dwarf::DW_LANG_lo_user+37)
#define DW_LANG_chapel llvm::dwarf::DW_LANG_lo_user

#if HAVE_LLVM_VER >= 37
// This should be renamed to DI versions
#define LLVM_DITYPE llvm::DIType*
#define LLVM_DIFILE llvm::DIFile*
#define LLVM_DINAMESPACE llvm::DINamespace*
#define LLVM_DISUBPROGRAM llvm::DISubprogram*
#define LLVM_DIGLOBALVARIABLE llvm::DIGlobalVariable*
#define LLVM_DIVARIABLE llvm::DIVariable*
#define LLVM_DITYPEARRAY llvm::DITypeRefArray
#define LLVM_DIARRAY llvm::DebugNodeArray

#define LLVM_DI_SUBROUTINE_TYPE llvm::DISubroutineType*

static inline LLVM_DITYPE toDITYPE(llvm::MDNode* N)
{
  return llvm::cast_or_null<llvm::DIType>(N);
}

static inline LLVM_DINAMESPACE toDINAMESPACE(llvm::MDNode* N)
{
  return llvm::cast_or_null<llvm::DINamespace>(N);
}

static inline LLVM_DISUBPROGRAM toDISUBPROGRAM(llvm::MDNode* N)
{
  return llvm::cast_or_null<llvm::DISubprogram>(N);
}

static inline LLVM_DIGLOBALVARIABLE toDIGLOBALVARIABLE(llvm::MDNode* N)
{
  return llvm::cast_or_null<llvm::DIGlobalVariable>(N);
}

static inline LLVM_DIVARIABLE toDIVARIABLE(llvm::MDNode* N)
{
  return llvm::cast_or_null<llvm::DIVariable>(N);
}




#else
#define LLVM_DITYPE llvm::DIType
#define LLVM_DIFILE llvm::DIFile
#define LLVM_DINAMESPACE llvm::DINameSpace
#define LLVM_DISUBPROGRAM llvm::DISubprogram
#define LLVM_DIGLOBALVARIABLE llvm::DIGlobalVariable
#define LLVM_DIVARIABLE llvm::DIVariable
#define LLVM_DITYPEARRAY llvm::DITypeArray
#define LLVM_DIARRAY llvm::DIArray

#if HAVE_LLVM_VER >= 36
#define LLVM_DI_SUBROUTINE_TYPE llvm::DISubroutineType
#else
#define LLVM_DI_SUBROUTINE_TYPE llvm::DICompositeType
#endif

static inline LLVM_DITYPE toDITYPE(llvm::MDNode* N)
{
  return llvm::DIType(N);
}

static inline LLVM_DINAMESPACE toDINAMESPACE(llvm::MDNode* N)
{
  return llvm::DINameSpace(N);
}

static inline LLVM_DISUBPROGRAM toDISUBPROGRAM(llvm::MDNode* N)
{
  return llvm::DISubprogram(N);
}

static inline LLVM_DIGLOBALVARIABLE toDIGLOBALVARIABLE(llvm::MDNode* N)
{
  return llvm::DIGlobalVariable(N);
}

static inline LLVM_DIVARIABLE toDIVARIABLE(llvm::MDNode* N)
{
  return llvm::DIVariable(N);
}



#endif


#endif

struct lessAstr {
  bool operator() (const char* lhs, const char* rhs) const {
    return strcmp(lhs,rhs) < 0;
  }
};

class debug_data
{
#ifdef HAVE_LLVM
 public:
  static bool can_debug() { return true; }
  debug_data(llvm::Module &m) : dibuilder(m){}
  void finalize();
  void create_compile_unit(const char *file, const char *directory, bool is_optimized, const char *flags);

  LLVM_DITYPE construct_type(Type *type);
  LLVM_DITYPE get_type(Type *type);

  LLVM_DIFILE construct_file(const char *file);
  LLVM_DIFILE get_file(const char *file);

  LLVM_DINAMESPACE construct_module_scope(ModuleSymbol* modSym);
  LLVM_DINAMESPACE get_module_scope(ModuleSymbol* modSym);

  LLVM_DI_SUBROUTINE_TYPE get_function_type(FnSymbol *function);
  LLVM_DISUBPROGRAM construct_function(FnSymbol *function);
  LLVM_DISUBPROGRAM get_function(FnSymbol *function);

  LLVM_DIGLOBALVARIABLE construct_global_variable(VarSymbol *gVarSym);
  LLVM_DIGLOBALVARIABLE get_global_variable(VarSymbol *gVarSym);
  LLVM_DIVARIABLE construct_variable(VarSymbol *varSym);
  LLVM_DIVARIABLE get_variable(VarSymbol *varSym);
  LLVM_DIVARIABLE construct_formal_arg(ArgSymbol *argSym, unsigned int ArgNo);
  LLVM_DIVARIABLE get_formal_arg(ArgSymbol *argSym, unsigned int ArgNo);

 private:
  llvm::DIBuilder dibuilder;
  bool optimized;
  //std::vector<llvm::DIFile>files;
  std::map<const char*,LLVM_DIFILE,lessAstr> filesByName;
  //std::vector<llvm::DIType>types;
  //std::map<const char*,llvm::DIType,lessAstr> typesByName;
  //std::vector<llvm::DINameSpace>name_spaces;
  //std::map<const char*,llvm::DINameSpace,lessAstr> modulesByName;
  //std::vector<llvm::DISubprogram>functions;
#else
  static bool can_debug() { return false; }
#endif
};

extern debug_data *debug_info;

#endif
