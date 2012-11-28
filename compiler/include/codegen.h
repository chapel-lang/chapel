#ifndef CODEGEN_H
#define CODEGEN_H

#include <list>
#include <map>
#include <string>
#include <vector>

#ifdef HAVE_LLVM
// need llvm::Value, BasicBlock, Type, and
// a bunch of clang stuff.
#include "llvm/Module.h"
#include "llvm/Value.h"
#include "llvmUtil.h"
#include "llvm/Target/TargetData.h"
#include "clangSupport.h"
#endif

#include "genret.h"

#ifdef HAVE_LLVM
// should support TypedefDecl,EnumDecl,RecordDecl
llvm::Type* codegenCType(const clang::TypeDecl* td);
// should support FunctionDecl,VarDecl,EnumConstantDecl
GenRet codegenCValue(const clang::ValueDecl *vd);

// forward declare.
namespace clang {
  namespace CodeGen {
    class CodeGenModule;
  }
}
class CCodeGenAction;

/* The LayeredValueTable stores
 * symbols we got from clang or symbol we have generated
 * in LLVM and might need to refer to in the future.
 * That includes local variables, functions, globals, types...
 */
class LayeredValueTable
{
  private:
    struct Storage {
      // note: this following nested struct should be a union
      // since only one of the following is necessary?
      struct s_u {
        llvm::Value *value;
        llvm::BasicBlock *block;
        llvm::Type *type;
        // Note that clang will cache clang->llvm for types, at least
        // It would be possible for us to cache everything here
        // but that is probably redundant.
        clang::NamedDecl *cdecl;
      } u;
      int8_t isLVPtr;
      bool isUnsigned;
      Storage() {
        u.value = NULL;
        u.block = NULL;
        u.type = NULL;
        u.cdecl = NULL;
      }
    };
   
    typedef std::list<std::map<std::string, Storage> >::iterator layer_iterator;
    typedef std::map<std::string, Storage>::iterator value_iterator;
    
    std::list<std::map<std::string, Storage> > layers;
  
  public:
    LayeredValueTable();
    void addLayer();
    void removeLayer();
    void addValue(std::string name, llvm::Value *value, uint8_t isLVPtr, bool isUnsigned);
    void addGlobalValue(std::string name, llvm::Value *value, uint8_t isLVPtr, bool isUnsigned);
    void addGlobalValue(std::string name, GenRet gend);
    void addGlobalType(std::string name, llvm::Type *type);
    void addGlobalCDecl(clang::NamedDecl* cdecl);
    void addBlock(std::string name, llvm::BasicBlock *block);
    GenRet getValue(std::string name);
    llvm::BasicBlock *getBlock(std::string name);
    llvm::Type *getType(std::string name);
    clang::NamedDecl* getCDecl(std::string name);
 
  private:
    Storage* get(std::string name);
};

#endif

/* GenInfo is meant to be a global variable which stores
 * the code generator state - e.g. FILE* to print C to
 * or LLVM module in which to generate.
 */
struct GenInfo {
  // If we're generating C, this is the FILE* to print to
  FILE* cfile;
  // When generating C, sometimes the code generator needs
  // to introduce a temporary variable. When it does,
  // it saves them here.
  std::vector<std::string> cLocalDecls;
  // When generating C code *internal to a function*, we
  // need to possibly insert some temporaries above the code
  // for the function, so we can't print the function body
  // until after we have generated the entire function.
  // So, we accumulate statements here and when we finish
  // code generating the function, we print out
  // all of cLocalDecls and all of the cStatements.
  std::vector<std::string> cStatements;

  // We're always generating code for some AST element
  // This is the astloc for the statement under consideration
  // Normally, it refers to a statement
  int lineno;
  const char* filename;

#ifdef HAVE_LLVM
  // If we're generating LLVM, the following are available
  llvm::Module *module;
  llvm::IRBuilder<> *builder;
  LayeredValueTable *lvt;

  // Clang Stuff
  std::string clangInstallDir;
  std::string compileline;
  std::vector<std::string> clangCCArgs;
  std::vector<std::string> clangLDArgs;
  std::vector<std::string> clangOtherArgs;

  clang::CodeGenOptions codegenOptions;
  clang::DiagnosticOptions diagOptions;
  clang::TextDiagnosticPrinter* DiagClient;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagID;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> Diags;

  /*clang::driver::Driver *TheDriver;
  clang::driver::Compilation *C;
  clang::CompilerInvocation *CI;*/
  clang::CompilerInstance *Clang;

  // Once we get to code generation....
  std::string moduleName;
  llvm::LLVMContext llvmContext;
  clang::ASTContext *Ctx;
  llvm::TargetData *targetData;
  clang::CodeGen::CodeGenModule *cgBuilder;
  CCodeGenAction *cgAction;

  // When using a function, just use cgModule->GetAddrOfFunction,
  // which will cause cgModule to emit it on Builder->Release.
  //
#endif

  // defined in passes/codegen.cpp
  GenInfo(std::string clangInstallDirIn,
          std::string compilelineIn,
          std::vector<std::string> clangCCArgs,
          std::vector<std::string> clangLDArgs,
          std::vector<std::string> clangOtherArgs);
};


extern GenInfo* gGenInfo;
extern int gMaxVMT;

#ifdef HAVE_LLVM
llvm::Function* getFunctionLLVM(const char* name);
llvm::Type* getTypeLLVM(const char* name);
int getCRecordMemberGEP(const char* typeName, const char* fieldName);
#endif


bool isBuiltinExternCFunction(const char* cname);

std::string numToString(int64_t num);
std::string int64_to_string(int64_t i);
std::string uint64_to_string(uint64_t i);
void genComment(const char* comment, bool push=false);
void genIdComment(int id);
void flushStatements(void);


#endif //CODEGEN_H
