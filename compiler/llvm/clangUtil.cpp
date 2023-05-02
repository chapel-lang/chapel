/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "clangUtil.h"

#include <inttypes.h>

#include <cctype>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <regex>

#ifdef HAVE_LLVM
#include "clang/AST/GlobalDecl.h"

#include "clang/Basic/CodeGenOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/Version.h"
#include "clang/CodeGen/BackendUtil.h"
#include "clang/CodeGen/CodeGenABITypes.h"
#include "clang/CodeGen/ModuleBuilder.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Job.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/MacroInfo.h"
#include "clang/Lex/Preprocessor.h"

#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Linker/Linker.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/SubtargetFeature.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/AlwaysInliner.h"
#include "llvm/Transforms/IPO/Internalize.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/Cloning.h"

#if HAVE_LLVM_VER >= 140
#include "llvm/MC/TargetRegistry.h"
#else
#include "llvm/Support/TargetRegistry.h"
#endif

#if HAVE_LLVM_VER >= 90
#include "llvm/Support/CodeGen.h"
#endif

#ifdef HAVE_LLVM_RV
#include "rv/passes.h"
#endif

#endif

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "mli.h"
#include "mysystem.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "version.h"
#include "wellknown.h"

#include "codegen.h"

#include "build.h"

#include "llvmDebug.h"
#include "llvmVer.h"

#include "../../frontend/lib/immediates/prim_data.h"
#include "chpl/util/clang-integration.h"

#include "global-ast-vecs.h"

typedef Type ChapelType;

#ifndef HAVE_LLVM

void readExternC(void) {
  // Do nothing if we don't have LLVM support.
}

void cleanupExternC(void) {
  // Do nothing if we don't have LLVM support.
}

#else

using namespace clang;
using namespace llvm;
using namespace CodeGen;

#define GLOBAL_PTR_SPACE 100
#define WIDE_PTR_SPACE 101
#define GLOBAL_PTR_SIZE 128
#define GLOBAL_PTR_ABI_ALIGN 64
#define GLOBAL_PTR_PREF_ALIGN 64

#define LLVM_MODULE_NAME "root"

#include "llvmGlobalToWide.h"
#include "llvmAggregateGlobalOps.h"
#include "llvmDumpIR.h"

static void setupForGlobalToWide();
static void adjustLayoutForGlobalToWide();
static void setupModule();

fileinfo    gAllExternCode;

// forward declare
class CCodeGenConsumer;
class CCodeGenAction;

// This class stores information about the embedded clang compiler
// instance.
struct ClangInfo {

  bool parseOnly;

  std::string clangCC;
  std::string clangCXX;
  // arguments to C/C++ compiler
  // including -I for bundled paths
  // these will apply when compiling C files as well
  std::vector<std::string> clangCCArgs;

  // other arguments to C/C++ compiler, passed after the above ones.
  // these tend to be the arguments indicating what to compile etc.
  // and these arguments don't apply when compiling C files named
  // on the command line
  std::vector<std::string> clangOtherArgs;

  // arguments to the linker from CC/CXX variable overrides
  std::vector<std::string> clangLDArgs;

  // the following 3 are here to make sure of no memory errors
  // when clang has const char* pointers to these strings
  std::string clangexe;
  std::vector<std::string> driverArgs;
  std::vector<const char*> driverArgsCStrings;

  clang::CodeGenOptions codegenOptions;

  clang::CompilerInstance *Clang;

  // Once we get to code generation....
  clang::ASTContext *Ctx;

  clang::CodeGenerator *cCodeGen;
  CCodeGenAction *cCodeGenAction;

  // We stash the layout that Clang would like to use here.
  // With fLLVMWideOpt, this will be the layout that we
  // pass to the code generator even though we modify the
  // version in the module (to add global pointer types)
  // before running optimization.
  std::string asmTargetLayoutStr;

  int intSizeInBits;
  int longSizeInBits;
  int longLongSizeInBits;
  int charSizeInBits;
  int shortSizeInBits;
  int ptrSizeInBits;

  ClangInfo(
    std::string clangCcIn,
    std::string clangCxxIn,
    std::vector<std::string> clangCCArgsIn,
    std::vector<std::string> clangOtherArgsIn,
    std::vector<std::string> clangLDArgsIn,
    bool parseOnlyIn);
};

ClangInfo::ClangInfo(
    std::string clangCcIn,
    std::string clangCxxIn,
    std::vector<std::string> clangCCArgsIn,
    std::vector<std::string> clangOtherArgsIn,
    std::vector<std::string> clangLDArgsIn,
    bool parseOnlyIn)
       : parseOnly(parseOnlyIn),
         clangCC(std::move(clangCcIn)),
         clangCXX(std::move(clangCxxIn)),
         clangCCArgs(std::move(clangCCArgsIn)),
         clangOtherArgs(std::move(clangOtherArgsIn)),
         clangLDArgs(std::move(clangLDArgsIn)),
         codegenOptions(),
         Clang(nullptr),
         Ctx(nullptr),
         cCodeGen(nullptr), cCodeGenAction(nullptr),
         asmTargetLayoutStr(),
         intSizeInBits(0),
         longSizeInBits(0),
         longLongSizeInBits(0),
         charSizeInBits(0),
         shortSizeInBits(0),
         ptrSizeInBits(0)
{
}

static
VarSymbol *minMaxConstant(int nbits, bool isSigned, bool isMin)
{
  if( nbits == 8 && isSigned && isMin )
    return new_IntSymbol(INT8_MIN, INT_SIZE_8);
  else if( nbits == 8 && isSigned && !isMin )
    return new_IntSymbol(INT8_MAX, INT_SIZE_8);
  else if( nbits == 8 && !isSigned && isMin )
    return new_IntSymbol(0, INT_SIZE_8);
  else if( nbits == 8 && !isSigned && !isMin )
    return new_IntSymbol(UINT8_MAX, INT_SIZE_8);

  else if( nbits == 16 && isSigned && isMin )
    return new_IntSymbol(INT16_MIN, INT_SIZE_16);
  else if( nbits == 16 && isSigned && !isMin )
    return new_IntSymbol(INT16_MAX, INT_SIZE_16);
  else if( nbits == 16 && !isSigned && isMin )
    return new_IntSymbol(0, INT_SIZE_16);
  else if( nbits == 16 && !isSigned && !isMin )
    return new_IntSymbol(UINT16_MAX, INT_SIZE_16);

  else if( nbits == 32 && isSigned && isMin )
    return new_IntSymbol(INT32_MIN, INT_SIZE_32);
  else if( nbits == 32 && isSigned && !isMin )
    return new_IntSymbol(INT32_MAX, INT_SIZE_32);
  else if( nbits == 32 && !isSigned && isMin )
    return new_IntSymbol(0, INT_SIZE_32);
  else if( nbits == 32 && !isSigned && !isMin )
    return new_IntSymbol(UINT32_MAX, INT_SIZE_32);

  else if( nbits == 64 && isSigned && isMin )
    return new_IntSymbol(INT64_MIN, INT_SIZE_64);
  else if( nbits == 64 && isSigned && !isMin )
    return new_IntSymbol(INT64_MAX, INT_SIZE_64);
  else if( nbits == 64 && !isSigned && isMin )
    return new_IntSymbol(0, INT_SIZE_64);
  else if( nbits == 64 && !isSigned && !isMin )
    return new_IntSymbol(UINT64_MAX, INT_SIZE_64);

  else INT_ASSERT(0 && "Bad options for minMaxConstant");
  return NULL;
}

static
void addMinMax(const char* prefix, int nbits, bool isSigned)
{
  GenInfo* info = gGenInfo;
  LayeredValueTable *lvt = info->lvt.get();

  astlocT prevloc = currentAstLoc;

  currentAstLoc = astlocT(0, astr("<internal>"));

  const char* min_name = astr(prefix, "_MIN");
  const char* max_name = astr(prefix, "_MAX");

  if( isSigned ) {
    // only signed versions have a meaningful min.
    lvt->addGlobalVarSymbol(min_name, minMaxConstant(nbits, isSigned, true));
  }
  // but signed and unsigned both have a max
  lvt->addGlobalVarSymbol(max_name, minMaxConstant(nbits, isSigned, false));

  currentAstLoc = prevloc;
}

static
void addMinMax(ASTContext* Ctx, const char* prefix, clang::CanQualType qt)
{
  const clang::Type* ct = qt.getTypePtr();
  int nbits = Ctx->getTypeSize(ct);
  bool isSigned = ct->isSignedIntegerType();

  addMinMax(prefix, nbits, isSigned);
}

static
::Type* getIntWithBits(int nbits, bool unsigned_) {

  switch (nbits) {
    case 64:
      return unsigned_ ? dtUInt[INT_SIZE_64] : dtInt[INT_SIZE_64];
    case 32:
      return unsigned_ ? dtUInt[INT_SIZE_32] : dtInt[INT_SIZE_32];
    case 16:
      return unsigned_ ? dtUInt[INT_SIZE_16] : dtInt[INT_SIZE_16];
    case 8:
      return unsigned_ ? dtUInt[INT_SIZE_8] : dtInt[INT_SIZE_8];
    default:
      INT_FATAL("case not handled");
  }

  return NULL;
}

static
void setupCIntType(::Type*& type, int nbits, bool unsigned_) {
  if (type != NULL) {
    INT_ASSERT(get_width(type) == nbits);
    INT_ASSERT(is_signed(type) == !unsigned_);
  } else {
    type = getIntWithBits(nbits, unsigned_);
  }
}

static
void setupClangContext(GenInfo* info, ASTContext* Ctx)
{
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  clangInfo->Ctx = Ctx;

  // Set up some constants that depend on the Clang context.
  {
    clangInfo->intSizeInBits = Ctx->getTypeSize(Ctx->IntTy.getTypePtr());
    clangInfo->longSizeInBits = Ctx->getTypeSize(Ctx->LongTy.getTypePtr());
    clangInfo->longLongSizeInBits = Ctx->getTypeSize(Ctx->LongLongTy.getTypePtr());
    clangInfo->charSizeInBits = Ctx->getTypeSize(Ctx->CharTy.getTypePtr());
    clangInfo->shortSizeInBits = Ctx->getTypeSize(Ctx->ShortTy.getTypePtr());
    clangInfo->ptrSizeInBits = Ctx->getTypeSize(Ctx->VoidPtrTy.getTypePtr());

    setupCIntType(dt_c_int, clangInfo->intSizeInBits, false);
    setupCIntType(dt_c_uint, clangInfo->intSizeInBits, true);
    setupCIntType(dt_c_long, clangInfo->longSizeInBits, false);
    setupCIntType(dt_c_ulong, clangInfo->longSizeInBits, true);
    setupCIntType(dt_c_longlong, clangInfo->longLongSizeInBits, false);
    setupCIntType(dt_c_ulonglong, clangInfo->longLongSizeInBits, true);
    setupCIntType(dt_c_char, clangInfo->charSizeInBits,
                  !Ctx->CharTy.getTypePtr()->isSignedIntegerType());
    setupCIntType(dt_c_schar, clangInfo->charSizeInBits, false);
    setupCIntType(dt_c_uchar, clangInfo->charSizeInBits, true);
    setupCIntType(dt_c_short, clangInfo->shortSizeInBits, false);
    setupCIntType(dt_c_ushort, clangInfo->shortSizeInBits, true);
    setupCIntType(dt_c_intptr, clangInfo->ptrSizeInBits, false);
    setupCIntType(dt_c_uintptr, clangInfo->ptrSizeInBits, true);
    setupCIntType(dt_ssize_t, clangInfo->ptrSizeInBits, false);
    setupCIntType(dt_size_t, clangInfo->ptrSizeInBits, true);

    addMinMax(Ctx, "CHAR", Ctx->CharTy);
    addMinMax(Ctx, "SCHAR", Ctx->SignedCharTy);
    addMinMax(Ctx, "UCHAR", Ctx->UnsignedCharTy);
    addMinMax(Ctx, "SHRT", Ctx->ShortTy);
    addMinMax(Ctx, "USHRT", Ctx->UnsignedShortTy);
    addMinMax(Ctx, "INT", Ctx->IntTy);
    addMinMax(Ctx, "UINT", Ctx->UnsignedIntTy);
    addMinMax(Ctx, "LONG", Ctx->LongTy);
    addMinMax(Ctx, "ULONG", Ctx->UnsignedLongTy);
    addMinMax(Ctx, "LLONG", Ctx->LongLongTy);
    addMinMax(Ctx, "ULLONG", Ctx->UnsignedLongLongTy);
  }
}


static astlocT getClangDeclLocation(clang::Decl* d) {
  INT_ASSERT(gGenInfo->clangInfo->Ctx);
  SourceManager& srcm = gGenInfo->clangInfo->Ctx->getSourceManager();
  SourceLocation loc = d->getLocation();
  PresumedLoc ploc = srcm.getPresumedLoc(loc, true);
  const char* filename = astr(ploc.getFilename());
  unsigned lineno = ploc.getLine();
  astlocT ret(lineno, filename);
  return ret;
}



#if HAVE_LLVM_VER >= 150
typedef MacroInfo::const_tokens_iterator tokens_iterator;
#else
typedef MacroInfo::tokens_iterator tokens_iterator;
#endif

static const bool debugPrintMacros = false;

static void handleMacroExpr(const MacroInfo* inMacro,
                            const IdentifierInfo* origID,
                            tokens_iterator start,
                            tokens_iterator end,
                            VarSymbol*& varRet,
                            TypeDecl*& cTypeRet,
                            ValueDecl*& cValueRet,
                            const char*& cCastToTypeRet);

static void handleCallMacro(const IdentifierInfo* origID,
                            const MacroInfo* inMacro,
                            const MacroInfo* calledMacro) {
  if (inMacro->getNumTokens() != 6)
    return;
  if (calledMacro->getNumParams() != 2)
    return;
  if (calledMacro->getNumTokens() != 3)
    return;

  // expect 'LAPACK_GLOBAL' '(' 'actual1' ',' 'actual2' ')'
  Token actual1, actual2;
  int count = 0;
  for (tokens_iterator tok = inMacro->tokens_begin();
       tok != inMacro->tokens_end(); ++tok) {
    count++;
    if (count == 3)
      actual1 = *tok;
    if (count == 5)
      actual2 = *tok;
  }

  IdentifierInfo* formal1 = nullptr;
  IdentifierInfo* formal2 = nullptr;
  count = 0;
  for (MacroInfo::param_iterator param = calledMacro->param_begin();
       param != calledMacro->param_end(); ++param) {
    count++;
    if (count == 1)
      formal1 = *param;
    if (count == 2)
      formal2 = *param;
  }

  // expecting 'identifier' ## '_'
  count = 0;
  Token body1, body2, body3;
  for (tokens_iterator tok = calledMacro->tokens_begin();
       tok != calledMacro->tokens_end(); ++tok) {
    count++;
    if (count == 1) {
      if (tok->getKind() != tok::identifier)
        return;
      body1 = *tok;
    }
    if (count == 2) {
      if (tok->getKind() != tok::hashhash)
        return;
      body2 = *tok;
    }
    if (count == 3) {
      if (tok->getKind() != tok::identifier)
        return;
      body3 = *tok;
    }
  }

  llvm::StringRef lhsName;
  if (body1.getIdentifierInfo()->getName() == formal1->getName()) {
    lhsName = actual1.getIdentifierInfo()->getName();
  } else if (body1.getIdentifierInfo()->getName() == formal2->getName()) {
    lhsName = actual2.getIdentifierInfo()->getName();
  } else {
    return;
  }

  std::string replacement = lhsName.str() +
                            body3.getIdentifierInfo()->getName().str();

  GenInfo* info = gGenInfo;
  INT_ASSERT(info);

  info->lvt->addForwardName(origID->getName(), replacement.c_str());

  if (debugPrintMacros) {
    printf("replacing: %s with %s\n", origID->getName().str().c_str(),
                                      replacement.c_str());
  }
}

// Adds a mapping from id->getName() to a variable or CDecl to info->lvt
static
void handleMacro(const IdentifierInfo* id, const MacroInfo* macro)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  std::string name = id->getName().str();
  const bool debugPrint = debugPrintMacros;

  if (debugPrint) printf("Working on macro %s\n", name.c_str());

  //Handling only simple string or integer defines
  if (macro->getNumParams() > 0)
  {
    if( debugPrint) {
      printf("the macro takes arguments\n");
    }
    if (name == "LAPACK_GLOBAL") {
      info->lvt->addMacro(name, macro);
    } else {
      return; // TODO -- handle macro functions.
    }
  }

  VarSymbol* varRet = NULL;
  TypeDecl* cTypeRet = NULL;
  ValueDecl* cValueRet = NULL;
  const char* cCastToTypeRet = NULL;

  handleMacroExpr(macro, id,
                  macro->tokens_begin(), macro->tokens_end(),
                  varRet, cTypeRet, cValueRet, cCastToTypeRet);

  if( debugPrint ) {
    std::string s = std::string(id->getName());
    const char* kind = NULL;
    if( varRet ) kind = "var";
    if( cTypeRet ) kind = "cdecl type";
    if( cValueRet ) kind = "cdecl value";
    if( kind ) printf("%s: adding an %s to the lvt\n", s.c_str(), kind);
    if( cCastToTypeRet ) printf("with cast to %s\n", cCastToTypeRet);
  }
  if( varRet ) {
    info->lvt->addGlobalVarSymbol(id->getName(), varRet, cCastToTypeRet);
  }
  if( cTypeRet ) {
    info->lvt->addGlobalCDecl(id->getName(), cTypeRet);
  }
  if( cValueRet ) {
    info->lvt->addGlobalCDecl(id->getName(), cValueRet, cCastToTypeRet);
  }

}

static void removeMacroOuterParens(const MacroInfo* inMacro,
                                   tokens_iterator &start,
                                   tokens_iterator &end) {

  if (start == end)
    return;

  // Remove any number of outer parens e.g. (1), ((1)) -> 1
  int left_parens = 0;
  int right_parens = 0;
  for (tokens_iterator cur = end - 1;
       cur != start;
       --cur) {
    if(cur->getKind() == tok::r_paren) right_parens++;
    else break;
  }

  for (tokens_iterator cur = start;
       cur != end;
       ++cur) {
    if(cur->getKind() == tok::l_paren) left_parens++;
    else break;
  }

  int min_parens = (left_parens < right_parens) ? left_parens : right_parens;
  if (min_parens > 0) {
    tokens_iterator oldStart = start;
    tokens_iterator oldEnd = end;
    start = oldStart + min_parens;
    end = oldEnd - min_parens;
    INT_ASSERT(start != oldStart);
    INT_ASSERT(end != oldEnd);
  }
}

// finds a parenthesized expression at the start of start..end
// the expression does not necessarily cover the entire expression.
static bool findParenthesizedExpr(const MacroInfo* inMacro,
                                  tokens_iterator start,
                                  tokens_iterator end,
                                  tokens_iterator &pStart,
                                  tokens_iterator &pEnd) {
  int inparens = 0;
  for (tokens_iterator cur = start; cur != end; ++cur) {

    if (cur->getKind() == tok::l_paren) inparens++;
    if (cur->getKind() == tok::r_paren) inparens--;

    if (inparens==1 && cur->getKind() == tok::l_paren) {
      // just found the outermost (
      pStart = cur;
    } else if (inparens > 0) {
      // pass a parenthesized expression
    } else if (inparens==0 && cur->getKind() == tok::r_paren) {
      // just found the outermost )
      pEnd = cur+1;
      return true;
    } else {
      // Some other pattern, stop looking for a parenthesized expr.
      break;
    }
  }

  pStart = start;
  pEnd = start;
  return false;
}


// Returns a type/identifier/macro name or NULL if it was not handled
static const char* handleTypeOrIdentifierExpr(const MacroInfo* inMacro,
                                              const IdentifierInfo* origID,
                                              tokens_iterator start,
                                              tokens_iterator end,
                                              IdentifierInfo*& ii) {
  GenInfo* info = gGenInfo;
  ii = NULL;

  // handle things like 'unsigned int'
  int _unsigned = 0;
  int _signed = 0;
  int _long = 0;
  int _short = 0;
  int _int = 0;
  int _char = 0;
  int _float = 0;
  int _double = 0;

  removeMacroOuterParens(inMacro, start, end);

  while (start != end) {
    if (start->getKind() == tok::kw_unsigned) {
      _unsigned++;
      _signed = 0;
      ++start;
    } else if (start->getKind() == tok::kw_signed) {
      _signed++;
      _unsigned = 0;
      ++start;
    } else if (start->getKind() == tok::kw_long) {
      _long++;
      _short = 0;
      ++start;
    } else if (start->getKind() == tok::kw_short) {
      _short++;
      _long = 0;
      ++start;
    } else if (start->getKind() == tok::kw_int) {
      _int++;
      ++start;
    } else if (start->getKind() == tok::kw_char) {
      _char++;
      ++start;
    } else if (start->getKind() == tok::kw_float) {
      _float++;
      ++start;
    } else if (start->getKind() == tok::kw_double) {
      _double++;
      ++start;
    } else {
      break; // not signed / unsigned / long / short
    }
  }

  if (_unsigned == 0 && _signed == 0 &&
      _long == 0 && _short == 0 &&
      _int == 0 && _char == 0 && _float == 0 && _double == 0) {

    Token tok = *start; // the main token
    ++start;

    if (start != end) {
      /* return NULL unless it's a specific pattern we know how to handle */
      bool canHandle = false;
      if (tok.getKind() == tok::identifier) {
        IdentifierInfo* tokId = tok.getIdentifierInfo();
        if (info->lvt->getMacro(tokId->getName()))
          canHandle = true;
      }
      if (!canHandle)
        return NULL;
    }

    if (tok.getKind() == tok::identifier) {
      IdentifierInfo* tokId = tok.getIdentifierInfo();
      ii = tokId;
      if (const clang::MacroInfo* macro =
            info->lvt->getMacro(tokId->getName())) {
        INT_ASSERT(origID != NULL);
        handleCallMacro(origID, inMacro, macro);
      } else {
        return astr(tokId->getNameStart());
      }
    }
  } else {
    // Give up if we didn't handle all the tokens in the above loop
    if (start != end)
      return NULL;

    // Rule out cases that don't make sense
    // char -> only can add signed or unsigned
    if (_char > 0 &&
        (_short > 0 || _long > 0 || _int > 0 ||  _float > 0 || _double > 0))
      return NULL;
    // float/double -> only can add long or signed
    if ((_float > 0 || _double > 0) &&
        (_short > 0 || _char > 0 || _int > 0 || _unsigned > 0))
      return NULL;
    // can't have both float and double
    if (_float > 0 && _double > 0)
      return NULL;
    // can't have int int or char char
    if (_int > 1 || _char > 1)
      return NULL;
    // can't have both signed and unsigned
    if (_signed > 0 && _unsigned > 0)
      return NULL;
    // can't have both short and long
    if (_long > 0 && _short > 0)
      return NULL;
    // can't have short short or long long long
    if (_long > 2 || _short > 1)
      return NULL;

    const char* ret = NULL;

    if (_double > 0) {
      // signed double would be OK
      if (_long == 0) ret = "c_double";
      // one day this might work with long double
    } else if (_float > 0) {
      // signed float would be OK
      if (_long == 0) ret = "c_float";
    } else if (_unsigned > 0) {
      if (_long == 2) ret = "c_ulonglong";
      else if (_long == 1) ret = "c_ulong";
      else if (_short == 1) ret = "c_ushort";
      else if (_char == 1) ret = "c_uchar";
      else if (_long == 0 && _short == 0) ret = "c_uint";
    } else {
      if (_long == 2) ret = "c_longlong";
      else if (_long == 1) ret = "c_long";
      else if (_short == 1) ret = "c_short";
      else if (_signed > 0 && _char == 1) ret = "c_schar";
      else if (_signed == 0 && _char == 1) ret = "c_char";
      else if (_long == 0 && _short == 0) ret = "c_int";
    }

    if (ret != NULL)
      return astr(ret);
  }
  return NULL;
}

static const char* handleStringExpr(const MacroInfo* inMacro,
                                    tokens_iterator start,
                                    tokens_iterator end) {
  removeMacroOuterParens(inMacro, start, end);

  Token tok = *start; // the main token
  ++start;

  // Give up if there are any tokens beyond the main token
  if (start != end)
    return NULL;

  if (tok.getKind() == tok::string_literal) {
    std::string body = std::string(tok.getLiteralData(), tok.getLength());
    return astr(body.c_str());
  }

  return NULL;
}

static ::Type* getTypeForMacro(const char* name) {
  // Look for the saved types in dt_c_int etc.
  ::Type* t = getWellKnownTypeWithName(name);
  return t;
}

static bool handleNumericCastExpr(const MacroInfo* inMacro,
                                  tokens_iterator start,
                                  tokens_iterator end,
                                  Immediate* imm,
                                  const char*& cCastToTypeRet);
static bool handleNumericUnaryPrefixExpr(const MacroInfo* inMacro,
                                         tokens_iterator start,
                                         tokens_iterator end,
                                         Immediate* imm);
static bool handleNumericLiteralExpr(const MacroInfo* inMacro,
                                     tokens_iterator start,
                                     tokens_iterator end,
                                     Immediate* imm);
static bool handleNumericBinOpExpr(const MacroInfo* inMacro,
                                   tokens_iterator start,
                                   tokens_iterator end,
                                   Immediate* imm);

static bool handleNumericExpr(const MacroInfo* inMacro,
                              tokens_iterator start,
                              tokens_iterator end,
                              Immediate* imm,
                              const char*& cCastToTypeRet) {

  cCastToTypeRet = NULL;

  removeMacroOuterParens(inMacro, start, end);

  if (start == end)
    return false;

  if (handleNumericUnaryPrefixExpr(inMacro, start, end, imm))
    return true;

  if (handleNumericLiteralExpr(inMacro, start, end, imm))
    return true;

  if (handleNumericCastExpr(inMacro, start, end, imm, cCastToTypeRet))
    return true;

  if (handleNumericBinOpExpr(inMacro, start, end, imm))
    return true;

  return false;
}

static bool handleNumericCastExpr(const MacroInfo* inMacro,
                                  tokens_iterator start,
                                  tokens_iterator end,
                                  Immediate* imm,
                                  const char*& cCastToTypeRet) {

  if (start == end)
    return false;

  // Check for a cast like '(unsigned int) 12'
  tokens_iterator castStart = start;
  tokens_iterator castEnd = start;

  if (findParenthesizedExpr(inMacro, start, end, castStart, castEnd)) {
    if (castEnd == end)
      return false;

    const char* castTo = NULL;
    clang::IdentifierInfo* ii = NULL;
    castTo = handleTypeOrIdentifierExpr(inMacro, NULL, castStart, castEnd, ii);
    if (castTo == NULL)
      return false;
    start = castEnd;

    // Find the type to cast to
    // (handles things like macros, nested parens)
    VarSymbol* tmpVar = NULL;
    TypeDecl* tmpType = NULL;
    ValueDecl* tmpVal = NULL;
    const char* tmpCastToType = NULL;

    handleMacroExpr(inMacro, NULL, castStart, castEnd,
                    tmpVar, tmpType, tmpVal, tmpCastToType);

    if (tmpType == NULL || tmpCastToType != NULL)
      return false;

    cCastToTypeRet = astr(tmpType->getName().str().c_str());

    const char* rhsCastToTy = NULL;
    Immediate rhsImm;
    Immediate retImm;
    bool got = handleNumericExpr(inMacro, castEnd, end, &rhsImm, rhsCastToTy);

    if (got == false)
      return false;

    if (rhsCastToTy == NULL) {
      retImm = rhsImm;
    } else {
      ::Type* t = getTypeForMacro(tmpType->getName().str().c_str());
      if (t == NULL)
        return false;

      Immediate dstImm = getDefaultImmediate(t);
      coerce_immediate(gContext, &rhsImm, &dstImm);
      retImm = dstImm;
    }

    // Try handling the cast now if we can.
    // Put it off if the type isn't known yet (e.g. a typedef)
    ::Type* doCastToType = NULL;
    if (cCastToTypeRet != NULL) {
      doCastToType = getTypeForMacro(cCastToTypeRet);
    }

    if (doCastToType != NULL) {
      Immediate dstImm = getDefaultImmediate(doCastToType);
      coerce_immediate(gContext, &retImm, &dstImm);
      *imm = dstImm;
      cCastToTypeRet = NULL; // cast already handled
    } else {
      *imm = retImm;
    }
    return true;
  }
  return false;
}


static bool handleNumericUnaryPrefixExpr(const MacroInfo* inMacro,
                                         tokens_iterator start,
                                         tokens_iterator end,
                                         Immediate* imm) {
  if (start == end)
    return false;

  // handle prefix unary operators + and -
  if (start->getKind() == tok::plus ||
      start->getKind() == tok::minus) {

    if (start+1 == end)
      return false;

    Immediate rhsImm;
    const char* tmpCastToTy = NULL;
    bool got = handleNumericExpr(inMacro, start+1, end, &rhsImm, tmpCastToTy);
    if (got == false || tmpCastToTy != NULL)
      return false;

    int p = 0;
    switch (start->getKind()) {
      case tok::plus:   p = P_prim_plus;    break;
      case tok::minus:  p = P_prim_minus;   break;
      default:
        INT_FATAL("unhandled case");
    }

    fold_constant(gContext, p, &rhsImm, NULL, imm);
    return true;
  }

  return false;
}

static bool handleNumericLiteralExpr(const MacroInfo* inMacro,
                                     tokens_iterator start,
                                     tokens_iterator end,
                                     Immediate* imm) {
  if (start == end)
    return false;

  // handle a single numeric literal
  if (start->getKind() == tok::numeric_constant && start+1 == end) {
    Token tok = *start; // the main token
    VarSymbol* varRet = NULL;
    std::string numString;
    int hex;
    int isfloat;

    if (tok.getLiteralData() && tok.getLength()) {
      numString.append(tok.getLiteralData(), tok.getLength());
    }

    hex = 0;
    if (numString[0] == '0' && (numString[1] == 'x' || numString[1] == 'X')) {
      hex = 1;
    }

    // check that it begins with a digit
    if (!isdigit(numString[0]))
      return false;

    isfloat = 0;
    if (numString.find('.') != std::string::npos) {
      isfloat = 1;
    }
    // also check for exponent since e.g. 1e10 is a float.
    if (hex) {
      // C99 hex floats use p for exponent
      if (numString.find('p') != std::string::npos ||
          numString.find('P') != std::string::npos) {
        isfloat = 1;
      }
    } else {
      if (numString.find('e') != std::string::npos ||
          numString.find('E') != std::string::npos) {
        isfloat = 1;
      }
    }

    if (!isfloat) {
      IF1_int_type size = INT_SIZE_32;

      int _unsigned = 0;
      int _long = 0;

      for (int i = numString.length() - 1; i >= 0; i--) {
        if (tolower(numString[i]) == 'l') {
          numString[i] = '\0';
          _long++;
        } else if (tolower(numString[i]) == 'u') {
          numString[i] = '\0';
          _unsigned++;
        } else {
          break;
        }
      }

      ::Type* t = NULL;
      if (_unsigned > 1 || _long > 2) return false;
      if (_unsigned > 0) {
        if (_long == 2)      t = dt_c_ulonglong;
        else if (_long == 1) t = dt_c_ulong;
        else                 t = dt_c_uint;
      } else {
        if (_long == 2)      t = dt_c_longlong;
        else if (_long == 1) t = dt_c_long;
        else                 t = dt_c_int;
      }
      INT_ASSERT(t != NULL);

      if        (t == dtInt[INT_SIZE_64] || t == dtUInt[INT_SIZE_64]) {
        size = INT_SIZE_64;
      } else if (t == dtInt[INT_SIZE_32] || t == dtUInt[INT_SIZE_32]) {
        size = INT_SIZE_32;
      } else {
        INT_FATAL("unhandled case");
      }

      if (_unsigned > 0) {
        varRet = new_UIntSymbol(strtoul(numString.c_str(), NULL, 0), size);
      } else {
        varRet = new_IntSymbol(strtol(numString.c_str(), NULL, 0), size);
      }
    } else {
      IF1_float_type size = FLOAT_SIZE_64;

      if (tolower(numString[numString.length() - 1]) == 'l') {
        numString[numString.length() - 1] = '\0';
        // TODO: use long double support once it exists
      } else if (tolower(numString[numString.length() - 1]) == 'f') {
        numString[numString.length() - 1] = '\0';
        size = FLOAT_SIZE_32;
      }

      varRet = new_RealSymbol(numString.c_str(), size);
    }

    INT_ASSERT(varRet != NULL);
    *imm = *varRet->immediate;
    return true;
  }

  return false;
}

static bool handleNumericBinOpExpr(const MacroInfo* inMacro,
                                   tokens_iterator start,
                                   tokens_iterator end,
                                   Immediate* imm) {
  // handle select binary operators
  // this only works if the LHS and RHS are either:
  //  parenthesized expressions; or
  //  numeric constants
  bool lhsOk = false;
  tokens_iterator lhsStart = start;
  tokens_iterator lhsEnd = start;

  // find a LHS constant or parenthesized-expression
  if (start->getKind() == tok::numeric_constant) {
    lhsOk = true;
    lhsStart = start;
    lhsEnd = start+1;
  } else {
    lhsOk = findParenthesizedExpr(inMacro, start, end, lhsStart, lhsEnd);
  }

  if (lhsOk == false || lhsEnd == end || lhsEnd+1 == end )
    return false;

  bool rhsOk = false;
  tokens_iterator op = lhsEnd;
  tokens_iterator rhsStart = op + 1;
  tokens_iterator rhsEnd = rhsStart;

  // find a RHS constant or parenthesized-expression
  if (rhsStart->getKind() == tok::numeric_constant) {
    rhsOk = true;
    rhsEnd = rhsStart+1;
  } else {
    rhsOk = findParenthesizedExpr(inMacro, rhsStart, end, rhsStart, rhsEnd);
  }

  if (rhsOk == false)
    return false;

  // fail if it didn't cover everything
  if (lhsStart != start || rhsEnd != end)
    return false;

  // Compute the lhs and rhs immediates
  Immediate lhsImm;
  Immediate rhsImm;
  const char* lhsCastToTy = NULL;
  const char* rhsCastToTy = NULL;

  lhsOk = handleNumericExpr(inMacro, lhsStart, lhsEnd, &lhsImm, lhsCastToTy);
  rhsOk = handleNumericExpr(inMacro, rhsStart, rhsEnd, &rhsImm, rhsCastToTy);
  if (lhsOk == false || rhsOk == false)
    return false;
  if (lhsCastToTy != NULL || rhsCastToTy != NULL)
    return false;

  // Apply the binary operator to the immediate
  int p = 0;
  switch (op->getKind()) {
    case tok::lessless:   p = P_prim_lsh;    break;
    default:
      return false; // this operator not handled
  }

  fold_constant(gContext, p, &lhsImm, &rhsImm, imm);
  return true;
}

static void handleMacroExpr(const MacroInfo* inMacro,
                            const IdentifierInfo* origID,
                            tokens_iterator start,
                            tokens_iterator end,
                            VarSymbol*& varRet,
                            TypeDecl*& cTypeRet,
                            ValueDecl*& cValueRet,
                            const char*& cCastToTypeRet)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  Preprocessor &preproc = clangInfo->Clang->getPreprocessor();

  const bool debugPrint = debugPrintMacros;

  varRet = NULL;
  cTypeRet = NULL;
  cValueRet = NULL;

  if (start == end) {
    if( debugPrint) {
      printf("the macro is empty\n");
    }
    return;
  }

  if (debugPrint) {
    for (tokens_iterator cur = start;
         cur != end;
         ++cur) {
      Token t = *cur;
      printf("Found token type %i name %s\n", t.getKind(), t.getName());
    }
  }

  clang::IdentifierInfo* ii = NULL;
  const char* idName = handleTypeOrIdentifierExpr(inMacro, origID,
                                                  start, end, ii);
  if (idName != NULL) {
    if( debugPrint) {
      printf("id = %s\n", idName);
    }

    // Handle the case where the macro refers to something we've
    // already parsed in C
    varRet = info->lvt->getVarSymbol(idName);
    if( !varRet ) {
      info->lvt->getCDecl(idName, &cTypeRet, &cValueRet);
    }
    if( !varRet && !cTypeRet && !cValueRet && ii != NULL) {
      // Check to see if it's another macro.
      MacroInfo* otherMacro = preproc.getMacroInfo(ii);
      if( otherMacro && otherMacro != inMacro ) {
        // Handle the other macro to add it to the LVT under the new name
        // The recursive call will add it to the LVT
        if( debugPrint) printf("other macro\n");
        handleMacro(ii, otherMacro);
        // Get whatever was added in the recursive call
        // so that we can add it under the new name.
        varRet = info->lvt->getVarSymbol(idName);
        info->lvt->getCDecl(idName, &cTypeRet, &cValueRet);
      }
    }
    if( debugPrint ) {
      if( varRet ) printf("found var %s\n", varRet->cname);
      if( cTypeRet ) {
        std::string s = std::string(cTypeRet->getName());
        printf("found cdecl type %s\n", s.c_str());
      }
      if( cValueRet ) {
        std::string s = std::string(cValueRet->getName());
        printf("found cdecl value %s\n", s.c_str());
      }
    }
    return;
  }

  const char* str = handleStringExpr(inMacro, start, end);
  if (str != NULL) {
    if( debugPrint) printf("str = %s\n", str);
    varRet = new_CStringSymbol(str);
    return;
  }

  Immediate imm;
  const char* castToTy = NULL;
  if (handleNumericExpr(inMacro, start, end, &imm, castToTy)) {
    if (debugPrint) {
      printf("num = ");
      fprint_imm(stdout, imm, true);
      printf("\n");
    }
    varRet = new_ImmediateSymbol(&imm);
    cCastToTypeRet = castToTy;
    return;
  }
}


static
void readMacrosClang(void) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  LayeredValueTable *lvt = info->lvt.get();

  SET_LINENO(rootModule);

  // Pre-populate with important INTxx_MIN/MAX from stdint.h
  // because we have trouble reading these because they have
  // special stuff to get the right constant width, but they
  // are all known integer values.
  lvt->addGlobalVarSymbol("NULL", new_IntSymbol(0, INT_SIZE_64));

  // Add INT{8,16,32,64}_{MIN,MAX} and INT_MAX and friends.
  addMinMax("INT8", 8, true);
  addMinMax("UINT8", 8, false);
  addMinMax("INT16", 16, true);
  addMinMax("UINT16", 16, false);
  addMinMax("INT32", 32, true);
  addMinMax("UINT32", 32, false);
  addMinMax("INT64", 64, true);
  addMinMax("UINT64", 64, false);

  //printf("Running ReadMacrosAction\n");
  Preprocessor &preproc = clangInfo->Clang->getPreprocessor();
  // Identify macro-functions and macro-values.
  // Later, if we see a use of a macro-function, we can
  //  compile it to a static/inline function with args types based an use
  // how will we know the return type?
  //   expr->getType() stmt->getRetValue()->getType....
  //     ... add function wrapping macro with wrong type
  //         parse/analyze squelching errors; get the macro expression type;
  //         correct the type and recompile to LLVM
  //         See ClangExpressionParser.cpp in lldb which parses
  //          a C expression from a command line... we need to
  //          do something similar.
  for(Preprocessor::macro_iterator i = preproc.macro_begin();
      i != preproc.macro_end();
      i++) {

    handleMacro(i->first, i->second.getLatest()->getMacroInfo());
  }
};

// This ASTConsumer helps us to:
// 1: parse code only in certain configurations
// 2: Convert C code to LLVM IR in others
// 3: keep the code generator open until we finish generating Chapel code,
//    since we might need to code generate called functions.
// 4: get LLVM values for code generated C things (e.g. types, function ptrs)
class CCodeGenConsumer final : public ASTConsumer {
  private:
    GenInfo* info;
    clang::DiagnosticsEngine* Diags;
    clang::CodeGenerator* Builder;
    bool parseOnly;
    ASTContext* savedCtx;

    bool shouldHandleDecl(Decl* d) {
      if (gCodegenGPU) {
        //this decl must have __device__
        return d->hasAttr<CUDADeviceAttr>();
      }
      else {
        // this decl either doesn't have __device__, or if it has, it also has a
        // __host__
        return !d->hasAttr<CUDADeviceAttr>() || d->hasAttr<CUDAHostAttr>();
      }
    }

  public:
    CCodeGenConsumer()
      : ASTConsumer(),
        info(gGenInfo),
        Diags(&info->clangInfo->Clang->getDiagnostics()),
        Builder(NULL),
        parseOnly(info->clangInfo->parseOnly),
        savedCtx(NULL)
    {

      if (!parseOnly) {
        Builder = CreateLLVMCodeGen(
          *Diags,
          LLVM_MODULE_NAME,
#if HAVE_LLVM_VER >= 150
          &info->clangInfo->Clang->getVirtualFileSystem(),
#endif
          info->clangInfo->Clang->getHeaderSearchOpts(),
          info->clangInfo->Clang->getPreprocessorOpts(),
          info->clangInfo->codegenOptions,
          info->llvmContext);

        INT_ASSERT(Builder);
        INT_ASSERT(!info->module);
        info->module = Builder->GetModule();
        info->clangInfo->cCodeGen = Builder;

        // compute target triple, data layout
        setupModule();
      }
    }

    ~CCodeGenConsumer() override = default;

    // Start ASTVisitor Overrides
    void Initialize(ASTContext &Context) override {

      setupClangContext(info, &Context);

      if (parseOnly) return;

      // Call Initialize on the code generator
      // Note: this can call setDataLayout on the module!
      Builder->Initialize(Context);

      // Adjust the data layout again since it might have been overwritten.
      adjustLayoutForGlobalToWide();
    }

    void doHandleDecl(Decl* d) {
      if (TypedefDecl *td = dyn_cast<TypedefDecl>(d)) {
        const clang::Type *ctype= td->getUnderlyingType().getTypePtrOrNull();
        if(ctype != NULL) {
          info->lvt->addGlobalCDecl(td);
        }
      } else if (FunctionDecl *fd = dyn_cast<FunctionDecl>(d)) {
        if (shouldHandleDecl(d)) {
          info->lvt->addGlobalCDecl(fd);
        }
      } else if (VarDecl *vd = dyn_cast<VarDecl>(d)) {
        if (shouldHandleDecl(d)) {
          info->lvt->addGlobalCDecl(vd);
        }
      } else if (RecordDecl *rd = dyn_cast<RecordDecl>(d)) {
        info->lvt->addGlobalCDecl(rd);
      } else if (UsingDecl* ud = dyn_cast<UsingDecl>(d)) {
        for (auto shadow : ud->shadows()) {
          NamedDecl* nd = shadow->getTargetDecl();
          doHandleDecl(nd);
        }
      } else if (LinkageSpecDecl* ld = dyn_cast<LinkageSpecDecl>(d)) {
        // Handles extern "C" { }
        for (auto sub : ld->decls()) {
          doHandleDecl(sub);
        }
      } else if (ExternCContextDecl *ed = dyn_cast<ExternCContextDecl>(d)) {
        // TODO: is this an alternative extern "C"?
        // do we need to handle it?
        for (auto sub : ed->decls()) {
          doHandleDecl(sub);
        }
      }
    }

    // HandleTopLevelDecl - Handle the specified top-level declaration.
    // This is called by the parser to process every top-level Decl*.
    //
    // \returns true to continue parsing, or false to abort parsing.
    bool HandleTopLevelDecl(DeclGroupRef DG) override {

      if (Diags->hasErrorOccurred()) return true;

      for (DeclGroupRef::iterator I = DG.begin(), E = DG.end(); I != E; ++I) {
        doHandleDecl(*I);
      }

      if (parseOnly) return true;

      return Builder->HandleTopLevelDecl(DG);
    }

   // \brief This callback is invoked each time an inline (method or friend)
   // function definition in a class is completed.
    void HandleInlineFunctionDefinition(FunctionDecl *D) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
      Builder->HandleInlineFunctionDefinition(D);
    }

   void HandleInterestingDecl(DeclGroupRef D) override {
     if (Diags->hasErrorOccurred()) return;
     if (parseOnly) return;
     Builder->HandleInterestingDecl(D);
   }

   void HandleTranslationUnit(ASTContext &Context) override {
     // Don't call Builder->HandleTranslationUnit yet, so that we
     // can keep it open to codegen more later.
     savedCtx = &Context;
     INT_ASSERT(savedCtx == info->clangInfo->Ctx);
   }

   void HandleTagDeclDefinition(TagDecl *D) override {
      if (Diags->hasErrorOccurred()) return;

      // make a note of C globals
      if(EnumDecl *ed = dyn_cast<EnumDecl>(D)) {
         // Add the enum type
         info->lvt->addGlobalCDecl(ed);
         // Add the enum values
         for(EnumDecl::enumerator_iterator e = ed->enumerator_begin();
             e != ed->enumerator_end();
             e++) {
           info->lvt->addGlobalCDecl(*e); // & goes away with newer clang
         }
      } else if(RecordDecl *rd = dyn_cast<RecordDecl>(D)) {
         const clang::Type *ctype = rd->getTypeForDecl();

         if(ctype != NULL && rd->getDefinition() != NULL) {
           info->lvt->addGlobalCDecl(rd);
         }
      }
      if (parseOnly) return;
      Builder->HandleTagDeclDefinition(D);
    }

    void HandleTagDeclRequiredDefinition(const TagDecl *D) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
      Builder->HandleTagDeclRequiredDefinition(D);
    }

    void HandleCXXImplicitFunctionInstantiation(FunctionDecl *D) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
      Builder->HandleCXXImplicitFunctionInstantiation(D);
    }

    void HandleTopLevelDeclInObjCContainer(DeclGroupRef D) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
      Builder->HandleTopLevelDeclInObjCContainer(D);
    }

    void HandleImplicitImportDecl(ImportDecl *D) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
      Builder->HandleImplicitImportDecl(D);
    }

    void CompleteTentativeDefinition(VarDecl *D) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
      Builder->CompleteTentativeDefinition(D);
    }

    void AssignInheritanceModel(CXXRecordDecl *RD) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
      Builder->AssignInheritanceModel(RD);
    }

    void HandleCXXStaticMemberVarInstantiation(VarDecl *VD) override {
      if (Diags->hasErrorOccurred()) return;
      if (parseOnly) return;
       Builder->HandleCXXStaticMemberVarInstantiation(VD);
    }

    void HandleVTable(CXXRecordDecl *RD) override {
       if (Diags->hasErrorOccurred()) return;
       if (parseOnly) return;
       Builder->HandleVTable(RD);
     }

    ASTMutationListener *GetASTMutationListener() override {
      if (Builder) return Builder->GetASTMutationListener();
      return nullptr;
    }

    ASTDeserializationListener *GetASTDeserializationListener() override {
      if (Builder) return Builder->GetASTDeserializationListener();
      return nullptr;
    }

    void PrintStats() override {
      if (Builder) Builder->PrintStats();
    }

    bool shouldSkipFunctionBody(Decl *D) override {
      if (Builder) return Builder->shouldSkipFunctionBody(D);
      return true;
    }

    // End ASTVisitor overrides

    const Decl *GetDeclForMangledName(llvm::StringRef MangledName) {
      return Builder->GetDeclForMangledName(MangledName);
    }

    llvm::Constant *GetAddrOfGlobal(GlobalDecl decl) {
      return Builder->GetAddrOfGlobal(decl, false);
    }
};


class CCodeGenAction final : public ASTFrontendAction {
 public:
  CCodeGenAction() { }
 protected:
  std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override;
};

std::unique_ptr<ASTConsumer>
CCodeGenAction::CreateASTConsumer(CompilerInstance &CI, StringRef InFile) {
  CCodeGenConsumer* c = new CCodeGenConsumer();
  return std::unique_ptr<ASTConsumer>(c);
};

static void finishClang(ClangInfo* clangInfo){
  if( clangInfo->cCodeGen ) {
    // This should call Builder->Release()
    clangInfo->cCodeGen->HandleTranslationUnit(*clangInfo->Ctx);
  }
}

static void deleteClang(ClangInfo* clangInfo){
  if( clangInfo->cCodeGen ) {
    delete clangInfo->cCodeGen;
    clangInfo->cCodeGen = NULL;
  }
  delete clangInfo->Clang;
  clangInfo->Clang = NULL;
  delete clangInfo->cCodeGenAction;
  clangInfo->cCodeGenAction = NULL;
}

static void cleanupClang(ClangInfo* clangInfo)
{
  finishClang(clangInfo);
  deleteClang(clangInfo);
}

// Initialize LLVM targets if needed
static void initializeLlvmTargets() {
  static bool targetsInited = false;
  if (targetsInited == false) {
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();

    targetsInited = true;
  }
}

// Get a string corresponding to the LLVM target triple
// for the current configuration
static std::string getConfiguredTargetTriple() {
  // TODO: use a different triple when cross compiling
  // TODO: look at CHPL_TARGET_ARCH
  return llvm::sys::getDefaultTargetTriple();
}


void setupClang(GenInfo* info, std::string mainFile)
{
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  clangInfo->clangexe = clangInfo->clangCC;
  clangInfo->driverArgs.clear();

  clangInfo->driverArgs.push_back("<chapel clang driver invocation>");

  for( size_t i = 0; i < clangInfo->clangCCArgs.size(); ++i ) {
    clangInfo->driverArgs.push_back(clangInfo->clangCCArgs[i]);
  }
  for( size_t i = 0; i < clangInfo->clangOtherArgs.size(); ++i ) {
    clangInfo->driverArgs.push_back(clangInfo->clangOtherArgs[i]);
  }

  clangInfo->driverArgs.push_back("-c");
  // chpl - always compile rt file
  clangInfo->driverArgs.push_back(mainFile.c_str());

  if (!fLlvmCodegen)
    clangInfo->driverArgs.push_back("-fsyntax-only");

  if( printSystemCommands && developer ) {
    for( size_t i = 0; i < clangInfo->driverArgs.size(); i++ ) {
      printf("%s ", clangInfo->driverArgs[i].c_str());
    }
    printf("\n");
  }

  // Initialize LLVM targets so that the clang commands can know if the
  // target CPU supports vectorization, avx, etc, etc
  // Also important for generating assembly from this program.
  initializeLlvmTargets();
  std::string triple = getConfiguredTargetTriple();

  for (auto & arg : clangInfo->driverArgs) {
    clangInfo->driverArgsCStrings.push_back(arg.c_str());
  }

  // Create a compiler instance to handle the actual work.
  CompilerInstance* Clang = new CompilerInstance();
  auto diagOptions =
    chpl::util::wrapCreateAndPopulateDiagOpts(clangInfo->driverArgsCStrings);
  auto diagClient = new clang::TextDiagnosticPrinter(llvm::errs(),
                                                     &*diagOptions);
  auto clangDiags =
    clang::CompilerInstance::createDiagnostics(diagOptions.release(),
                                               diagClient,
                                               /* owned */ true);
  Clang->setDiagnostics(&*clangDiags);

  if (usingGpuLocaleModel()) {
    clangDiags->setSeverityForGroup(diag::Flavor::WarningOrError,
                                    "unknown-cuda-version",
                                    diag::Severity::Ignored);
  }
  clangInfo->Clang = Clang;

  clang::driver::Driver TheDriver(clangInfo->clangexe, triple, *clangDiags);

  //   SetInstallDir(argv, TheDriver);

  std::unique_ptr<clang::driver::Compilation> C(
      TheDriver.BuildCompilation(clangInfo->driverArgsCStrings));

  clang::driver::Command* job = NULL;

  if (usingGpuLocaleModel() == false) {
    // Not a CPU+GPU compilation, so just use first job.
    job = &*C->getJobs().begin();
  } else {
    // CPU+GPU compilation
    //  1st cc1 command is for the GPU
    //  2nd cc1 command is for the CPU
    for (auto &command : C->getJobs()) {
      bool isCC1 = false;
      for (const auto& arg : command.getArguments()) {
        if (0 == strcmp(arg, "-cc1")) {
          isCC1 = true;
          break;
        }
      }
      if (isCC1) {

        if (gCodegenGPU) {
          // For GPU, set job to 1st cc1 command
          if (job == NULL) job = &command;
        } else {
          // For CPU, set job to last cc1 command
          job = &command;
        }
      }
    }
  }


  if (job == NULL)
    USR_FATAL("Could not find cc1 command from clang driver");

  if( printSystemCommands && developer ) {
    printf("<internal clang cc> ");
    for ( const auto& a : job->getArguments() ) {
      printf("%s ", a);
    }
    printf("\n");
  }

  // Should this run
  // TheDriver.BuildCompilation
  // get a Compilation?
  //CompilerInvocation* CI =
  //  createInvocationFromCommandLine(clangArgs, clangInfo->Diags);

#if HAVE_LLVM_VER >= 100
  bool success = CompilerInvocation::CreateFromArgs(
            Clang->getInvocation(),
            job->getArguments(),
            *clangDiags);
#else
  bool success = CompilerInvocation::CreateFromArgs(
            Clang->getInvocation(),
            &job->getArguments().front(), (&job->getArguments().back())+1,
            *clangDiags);
#endif

  CompilerInvocation* CI = &Clang->getInvocation();

  INT_ASSERT(success);

  // Get the codegen options from the clang command line.
  clangInfo->codegenOptions = CI->getCodeGenOpts();

  // if --fast is given, we should be at least at -O3.
  if(fFastFlag && clangInfo->codegenOptions.OptimizationLevel < 3) {
    clangInfo->codegenOptions.OptimizationLevel = 3;
  }

  {
    // Make sure we include clang's internal header dir
    SmallString<128> P;
    SmallString<128> P2; // avoids a valgrind overlapping memcpy

    P = clangInfo->clangexe;
    // Remove /clang from foo/bin/clang
    P2 = sys::path::parent_path(P);
    // Remove /bin   from foo/bin
    P = sys::path::parent_path(P2);

    if( ! P.equals("") ) {
      // Get foo/lib/clang/<version>/
      sys::path::append(P, "lib");
      sys::path::append(P, "clang");
      sys::path::append(P, CLANG_VERSION_STRING);
    }
    CI->getHeaderSearchOpts().ResourceDir = std::string(P.str());
    sys::path::append(P, "include");
    CI->getHeaderSearchOpts().AddPath(
        P.str(), frontend::System,false, false);
  }

  // Create the compilers actual diagnostics engine.
  clangInfo->Clang->createDiagnostics();
  if (!clangInfo->Clang->hasDiagnostics())
    INT_FATAL("Bad diagnostics from clang");

  // Set llvm options
  {
    std::vector<std::string> vec;

    // Start with any -mllvm options from the Clang invocation
    auto clangMLLVM = CI->getFrontendOpts().LLVMArgs;
    for (auto & arg : clangMLLVM) {
      vec.push_back(arg);
    }

    // Then add any from --mllvm passed to Chapel
    if (llvmFlags != "") {
      //split llvmFlags by spaces
      splitStringWhitespace(llvmFlags, vec);
    }

    std::vector<const char*> Args;
    Args.push_back("chpl-llvm-opts");
    for (auto & i : vec) {
      Args.push_back(i.c_str());
    }
    Args.push_back(NULL);

    if (printSystemCommands && developer) {
      printf("# parsing llvm command line options: ");
      for (auto arg : Args) {
        if (arg != NULL)
          printf(" %s", arg);
      }
      printf("\n");
    }

    // reset previously parsed options because we might run this multiple
    // times when handling extern blocks
    llvm::cl::ResetAllOptionOccurrences();
    // parse the options
    llvm::cl::ParseCommandLineOptions(Args.size()-1, &Args[0]);
  }
}


// copied from clang's BackendUtil.cpp
static Optional<llvm::CodeModel::Model>
getCodeModel(const CodeGenOptions &CodeGenOpts) {
  unsigned CodeModel = llvm::StringSwitch<unsigned>(CodeGenOpts.CodeModel)
                           .Case("tiny", llvm::CodeModel::Tiny)
                           .Case("small", llvm::CodeModel::Small)
                           .Case("kernel", llvm::CodeModel::Kernel)
                           .Case("medium", llvm::CodeModel::Medium)
                           .Case("large", llvm::CodeModel::Large)
                           .Case("default", ~1u)
                           .Default(~0u);
  assert(CodeModel != ~0u && "invalid code model!");
  if (CodeModel == ~1u)
    return None;
  return static_cast<llvm::CodeModel::Model>(CodeModel);
}

// this function is substantially similar to clang's
// initTargetOptions from BackendUtil.cpp
static llvm::TargetOptions getTargetOptions(
    const clang::CodeGenOptions& CodeGenOpts,
    const clang::TargetOptions& TargetOpts) {

  llvm::TargetOptions Options;

  // Chapel is always multithreaded
  Options.ThreadModel = llvm::ThreadModel::POSIX;

  // Set float ABI type.
  assert((CodeGenOpts.FloatABI == "soft" || CodeGenOpts.FloatABI == "softfp" ||
          CodeGenOpts.FloatABI == "hard" || CodeGenOpts.FloatABI.empty()) &&
         "Invalid Floating Point ABI!");
  Options.FloatABIType =
      llvm::StringSwitch<llvm::FloatABI::ABIType>(CodeGenOpts.FloatABI)
          .Case("soft", llvm::FloatABI::Soft)
          .Case("softfp", llvm::FloatABI::Soft)
          .Case("hard", llvm::FloatABI::Hard)
          .Default(llvm::FloatABI::Default);


  // Set the floating point optimization level
  // see also code setting FastMathFlags
  // This uses ffloatOpt rather than using clang's LangOpts.
  if (ffloatOpt == 1) {
    // --no-ieee-float
    // Allow unsafe fast floating point optimization
    Options.UnsafeFPMath = 1; // e.g. FSIN instruction
    Options.NoInfsFPMath = 1;
    Options.NoNaNsFPMath = 1;
    Options.NoTrappingFPMath = 1;
    Options.NoSignedZerosFPMath = 1;
    Options.AllowFPOpFusion = llvm::FPOpFusion::Fast;
  } else if (ffloatOpt == 0) {
    // Target default floating point optimization
    Options.AllowFPOpFusion = llvm::FPOpFusion::Standard;
  } else if (ffloatOpt == -1) {
    // --ieee-float
    // Should this set targetOptions.HonorSignDependentRoundingFPMathOption ?
    // Allow fused multiply-adds
    Options.AllowFPOpFusion = llvm::FPOpFusion::Standard;
  }

#if HAVE_LLVM_VER >= 120
  Options.BinutilsVersion =
      llvm::TargetMachine::parseBinutilsVersion(CodeGenOpts.BinutilsVersion);
#endif

  Options.UseInitArray = CodeGenOpts.UseInitArray;
  Options.DisableIntegratedAS = CodeGenOpts.DisableIntegratedAS;
  Options.CompressDebugSections = CodeGenOpts.getCompressDebugSections();
  Options.RelaxELFRelocations = CodeGenOpts.RelaxELFRelocations;

  // Set EABI version.
  Options.EABIVersion = TargetOpts.EABIVersion;

  Options.ExceptionModel = llvm::ExceptionHandling::None;

  //Options.NoInfsFPMath = LangOpts.NoHonorInfs; -- set above
  //Options.NoNaNsFPMath = LangOpts.NoHonorNaNs; -- set above
  Options.NoZerosInBSS = CodeGenOpts.NoZeroInitializedInBSS;
  //Options.UnsafeFPMath = LangOpts.UnsafeFPMath; -- set above
#if HAVE_LLVM_VER <= 110
  Options.StackAlignmentOverride = CodeGenOpts.StackAlignment;
#endif

  Options.BBSections =
    llvm::StringSwitch<llvm::BasicBlockSection>(CodeGenOpts.BBSections)
        .Case("all", llvm::BasicBlockSection::All)
        .Case("labels", llvm::BasicBlockSection::Labels)
        .StartsWith("list=", llvm::BasicBlockSection::List)
        .Case("none", llvm::BasicBlockSection::None)
        .Default(llvm::BasicBlockSection::None);

  if (Options.BBSections == llvm::BasicBlockSection::List) {
    INT_FATAL("this clang configuration not supported");
  }

#if HAVE_LLVM_VER >= 120
  Options.EnableMachineFunctionSplitter = CodeGenOpts.SplitMachineFunctions;
#endif

  Options.FunctionSections = CodeGenOpts.FunctionSections;
  Options.DataSections = CodeGenOpts.DataSections;
#if LLVM_VERSION_MAJOR == 120
  // clang::CodeGenOptions::IgnoreXCOFFVisibility first appeared in
  // LLVM version 12 and then went away in version 13.
  Options.IgnoreXCOFFVisibility = CodeGenOpts.IgnoreXCOFFVisibility;
#endif
  Options.UniqueSectionNames = CodeGenOpts.UniqueSectionNames;
  Options.UniqueBasicBlockSectionNames =
      CodeGenOpts.UniqueBasicBlockSectionNames;
  Options.TLSSize = CodeGenOpts.TLSSize;
  Options.EmulatedTLS = CodeGenOpts.EmulatedTLS;
  Options.ExplicitEmulatedTLS = CodeGenOpts.ExplicitEmulatedTLS;
  Options.DebuggerTuning = CodeGenOpts.getDebuggerTuning();
  Options.EmitStackSizeSection = CodeGenOpts.StackSizeSection;
#if HAVE_LLVM_VER >= 130
  Options.StackUsageOutput = CodeGenOpts.StackUsageOutput;
#endif
  Options.EmitAddrsig = CodeGenOpts.Addrsig;
  Options.ForceDwarfFrameSection = CodeGenOpts.ForceDwarfFrameSection;
  Options.EmitCallSiteInfo = CodeGenOpts.EmitCallSiteInfo;
#if HAVE_LLVM_VER >= 120
  Options.EnableAIXExtendedAltivecABI = CodeGenOpts.EnableAIXExtendedAltivecABI;
#if HAVE_LLVM_VER < 140
  Options.PseudoProbeForProfiling = CodeGenOpts.PseudoProbeForProfiling;
  Options.ValueTrackingVariableLocations =
      CodeGenOpts.ValueTrackingVariableLocations;
#endif
#endif
  Options.XRayOmitFunctionIndex = CodeGenOpts.XRayOmitFunctionIndex;
#if HAVE_LLVM_VER >= 140
  Options.LoopAlignment = CodeGenOpts.LoopAlignment;
#endif

  Options.MCOptions.SplitDwarfFile = CodeGenOpts.SplitDwarfFile;
  Options.MCOptions.MCRelaxAll = CodeGenOpts.RelaxAll;
  Options.MCOptions.MCSaveTempLabels = CodeGenOpts.SaveTempLabels;
#if HAVE_LLVM_VER >= 150
  Options.MCOptions.MCUseDwarfDirectory =
    CodeGenOpts.NoDwarfDirectoryAsm ? llvm::MCTargetOptions::DisableDwarfDirectory
                                    : llvm::MCTargetOptions::EnableDwarfDirectory;
#else
  Options.MCOptions.MCUseDwarfDirectory = !CodeGenOpts.NoDwarfDirectoryAsm;
#endif
  Options.MCOptions.MCNoExecStack = CodeGenOpts.NoExecStack;
  Options.MCOptions.MCIncrementalLinkerCompatible =
      CodeGenOpts.IncrementalLinkerCompatible;
  Options.MCOptions.MCFatalWarnings = CodeGenOpts.FatalWarnings;
  Options.MCOptions.MCNoWarn = CodeGenOpts.NoWarn;
  Options.MCOptions.AsmVerbose = CodeGenOpts.AsmVerbose;
#if HAVE_LLVM_VER >= 120
  Options.MCOptions.Dwarf64 = CodeGenOpts.Dwarf64;
#endif
  Options.MCOptions.PreserveAsmComments = CodeGenOpts.PreserveAsmComments;
  Options.MCOptions.ABIName = TargetOpts.ABI;

  // consider setting Options.MCOptions.IASSearchPaths
  // if .include directives with integrated assembler are needed

  Options.MCOptions.Argv0 = CodeGenOpts.Argv0;
  Options.MCOptions.CommandLineArgs = CodeGenOpts.CommandLineArgs;
#if HAVE_LLVM_VER >= 130
  Options.DebugStrictDwarf = CodeGenOpts.DebugStrictDwarf;
#endif

  return Options;
}

static void setupModule()
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  if (clangInfo->parseOnly) return;

  INT_ASSERT(info->module);

#if HAVE_LLVM_VER >= 130
  clangInfo->asmTargetLayoutStr =
    clangInfo->Clang->getTarget().getDataLayoutString();
#else
  clangInfo->asmTargetLayoutStr =
    clangInfo->Clang->getTarget().getDataLayout().getStringRepresentation();
#endif

  // Set the target triple.
  const llvm::Triple &Triple =
    clangInfo->Clang->getTarget().getTriple();
  info->module->setTargetTriple(Triple.getTriple());

  // Always set the module layout. This works around an apparent bug in
  // clang or LLVM (trivial/deitz/test_array_low.chpl would print out the
  // wrong answer  because some i64s were stored at the wrong alignment).
  info->module->setDataLayout(clangInfo->asmTargetLayoutStr);

  adjustLayoutForGlobalToWide();

  // Set the TargetMachine
  std::string Err;
  const llvm::Target* Target = TargetRegistry::lookupTarget(Triple.str(), Err);
  if (!Target)
    USR_FATAL("Could not find LLVM target for %s: %s",
              Triple.str().c_str(), Err.c_str());


  const clang::TargetOptions & ClangOpts = clangInfo->Clang->getTargetOpts();
  const clang::CodeGenOptions& ClangCodeGenOpts = clangInfo->codegenOptions;

  std::string cpu = ClangOpts.CPU;
  std::vector<std::string> clangFeatures = ClangOpts.Features;
  std::string featuresString;
  if (!clangFeatures.empty()) {
    llvm::SubtargetFeatures features;
    for (const std::string &feature : clangFeatures)
      features.AddFeature(feature);
    featuresString = features.getString();
  }

  if (printSystemCommands && developer) {
    printf("# target features %s\n", featuresString.c_str());
  }

  // Set up the TargetOptions
  llvm::TargetOptions Options = getTargetOptions(ClangCodeGenOpts, ClangOpts);

  if (!fFastFlag)
    Options.EnableFastISel = 1;
  else {
    // things to consider:
    // EnableIPRA  -- InterProcedural Register Allocation (IPRA).
    // GuaranteedTailCallOpt -- guarantee tail call opt (may change fn ABI)
  }

  // If the clang compiler is configured to do PIC code generation
  // by default, we want to do PIC code generation even if
  // CHPL_LIB_PIC is not set, otherwise we get link errors on
  // such systems.
  // So, start with the PIC setting that clang is using based on the arguments.
  llvm::Reloc::Model relocModel = ClangCodeGenOpts.RelocationModel;

  // If CHPL_LIB_PIC=pic, make sure to compile as pic code
  if (strcmp(CHPL_LIB_PIC, "pic") == 0) {
    relocModel = llvm::Reloc::Model::PIC_;
  }

  // Choose the code model
  llvm::Optional<CodeModel::Model> codeModel = getCodeModel(ClangCodeGenOpts);

  llvm::CodeGenOpt::Level optLevel =
    fFastFlag ? llvm::CodeGenOpt::Aggressive : llvm::CodeGenOpt::None;

  // Create the target machine.
  info->targetMachine = Target->createTargetMachine(Triple.str(),
                                                    cpu,
                                                    featuresString,
                                                    Options,
                                                    relocModel,
                                                    codeModel,
                                                    optLevel);



  // TODO: set a module flag with the Chapel ABI version
  //   m->addModuleFlag(llvm::Module::Error, "Chapel Version", unsigned);

  // Also setup some basic TBAA metadata nodes.
  info->mdBuilder = new llvm::MDBuilder(info->module->getContext());
  // Create the TBAA root node and unions node.
  info->tbaaRootNode = info->mdBuilder->createTBAARoot("Chapel types");
  info->tbaaUnionsNode =
    info->mdBuilder->createTBAAScalarTypeNode("all unions", info->tbaaRootNode);
  // Initialize TBAA metadata for special C pointers.
  dtCVoidPtr->symbol->llvmTbaaTypeDescriptor =
    info->mdBuilder->createTBAAScalarTypeNode("C void ptr",
                                              info->tbaaUnionsNode);
  dtCVoidPtr->symbol->llvmTbaaAccessTag =
    info->mdBuilder->createTBAAStructTagNode(
      dtCVoidPtr->symbol->llvmTbaaTypeDescriptor,
      dtCVoidPtr->symbol->llvmTbaaTypeDescriptor,
      /*Offset=*/0);
  dtCVoidPtr->symbol->llvmConstTbaaAccessTag =
    info->mdBuilder->createTBAAStructTagNode(
      dtCVoidPtr->symbol->llvmTbaaTypeDescriptor,
      dtCVoidPtr->symbol->llvmTbaaTypeDescriptor,
      /*Offset=*/0,
      /*IsConstant=*/true);
  dtStringC->symbol->llvmTbaaTypeDescriptor =
    dtCVoidPtr->symbol->llvmTbaaTypeDescriptor;
  dtStringC->symbol->llvmTbaaAccessTag = dtCVoidPtr->symbol->llvmTbaaAccessTag;
  dtStringC->symbol->llvmConstTbaaAccessTag =
    dtCVoidPtr->symbol->llvmConstTbaaAccessTag;
  dtCFnPtr->symbol->llvmTbaaTypeDescriptor =
    info->mdBuilder->createTBAAScalarTypeNode(
      "C fn ptr", dtCVoidPtr->symbol->llvmTbaaTypeDescriptor);
  dtCFnPtr->symbol->llvmTbaaAccessTag =
    info->mdBuilder->createTBAAStructTagNode(
      dtCFnPtr->symbol->llvmTbaaTypeDescriptor,
      dtCFnPtr->symbol->llvmTbaaTypeDescriptor,
      /*Offset=*/0);
  dtCFnPtr->symbol->llvmConstTbaaAccessTag =
    info->mdBuilder->createTBAAStructTagNode(
      dtCFnPtr->symbol->llvmTbaaTypeDescriptor,
      dtCFnPtr->symbol->llvmTbaaTypeDescriptor,
      /*Offset=*/0,
      /*IsConstant=*/true);
}

void finishCodegenLLVM() {
  GenInfo* info = gGenInfo;

  // Codegen extra stuff for global-to-wide optimization.
  setupForGlobalToWide();

  // Finish up our cleanup optimizers...
  info->FPM_postgen->doFinalization();

  // We don't need our postgen function pass manager anymore.
  delete info->FPM_postgen;
  info->FPM_postgen = NULL;

  // Now finish any Clang code generation.
  finishClang(info->clangInfo);

  // Now overwrite the value of llvm.ident to show Chapel
  char version[128];
  char chapel_string[256];
  get_version(version, sizeof(version));
  snprintf(chapel_string, 256, "Chapel version %s", version);
  info->module->getNamedMetadata("llvm.ident")->setOperand(0,
    llvm::MDNode::get(info->module->getContext(),
      llvm::MDString::get(info->module->getContext(), chapel_string))
  );

  if(debug_info)debug_info->finalize();

  // Verify the LLVM module.
  if( developer ) {
    bool problems;
    problems = verifyModule(*info->module, &errs());
    //problems = false;
    if(problems) {
      INT_FATAL("LLVM module verification failed");
    }
  }
}

#ifdef HAVE_LLVM_RV
static void registerRVPasses(const llvm::PassManagerBuilder &Builder,
                             llvm::legacy::PassManagerBase &PM) {

  if (developer)
    printf("Adding RV passes\n");

  rv::addRVPasses(PM);
}
#endif

// This has code based on clang's EmitAssemblyHelper::CreatePasses
// in BackendUtil.cpp.
static
void configurePMBuilder(PassManagerBuilder &PMBuilder, bool forFunctionPasses, int optLevel=-1) {
  ClangInfo* clangInfo = gGenInfo->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenOptions &CodeGenOpts = clangInfo->codegenOptions;

  if (optLevel < 0)
    optLevel = CodeGenOpts.OptimizationLevel;

  if( fFastFlag ) {
    // TODO -- remove this assert
    INT_ASSERT(CodeGenOpts.OptimizationLevel >= 2);
  }

  if (optLevel <= 1) {
      bool InsertLifetimeIntrinsics = (CodeGenOpts.OptimizationLevel != 0 &&
                                       !CodeGenOpts.DisableLifetimeMarkers);
      // TODO: insert lifetime intrinsics if Coroutines are used
    PMBuilder.Inliner = createAlwaysInlinerLegacyPass(InsertLifetimeIntrinsics);
  } else {
    PMBuilder.Inliner = createFunctionInliningPass(
        CodeGenOpts.OptimizationLevel, CodeGenOpts.OptimizeSize,
        (!CodeGenOpts.SampleProfileFile.empty() &&
         CodeGenOpts.PrepareForThinLTO));
  }

  PMBuilder.OptLevel = optLevel;
  PMBuilder.SizeLevel = CodeGenOpts.OptimizeSize;
  PMBuilder.SLPVectorize = CodeGenOpts.VectorizeSLP;
  PMBuilder.LoopVectorize = CodeGenOpts.VectorizeLoop;
  PMBuilder.CallGraphProfile = !CodeGenOpts.DisableIntegratedAS;

  PMBuilder.DisableUnrollLoops = !CodeGenOpts.UnrollLoops;
  PMBuilder.LoopsInterleaved = CodeGenOpts.UnrollLoops;
  PMBuilder.MergeFunctions = CodeGenOpts.MergeFunctions;
#if HAVE_LLVM_VER < 150
#if HAVE_LLVM_VER > 60
  PMBuilder.PrepareForThinLTO = CodeGenOpts.PrepareForThinLTO;
#else
  PMBuilder.PrepareForThinLTO = CodeGenOpts.EmitSummaryIndex;
#endif
  PMBuilder.PrepareForLTO = CodeGenOpts.PrepareForLTO;
#endif
  PMBuilder.RerollLoops = CodeGenOpts.RerollLoops;

  if (gGenInfo->targetMachine)
    gGenInfo->targetMachine->adjustPassManager(PMBuilder);

  // Enable Region Vectorizer aka Outer Loop Vectorizer
#ifdef HAVE_LLVM_RV
  if (fRegionVectorizer && !fNoVectorize && !forFunctionPasses) {
    // This in copied from 'registerRVPasses'
    PMBuilder.addExtension(PassManagerBuilder::EP_VectorizerStart,
                           registerRVPasses);
  }
#endif

  // TODO: we might need to call TargetMachine's addEarlyAsPossiblePasses
}

void prepareCodegenLLVM()
{
  GenInfo *info = gGenInfo;

  llvm::legacy::FunctionPassManager *fpm = new llvm::legacy::FunctionPassManager(info->module);

  PassManagerBuilder PMBuilder;

  // Set up the optimizer pipeline.

  // Add the TransformInfo pass
  fpm->add(createTargetTransformInfoWrapperPass(
           info->targetMachine->getTargetIRAnalysis()));

  // Add the TargetLibraryInfo pass
  Triple TargetTriple(info->module->getTargetTriple());
  llvm::TargetLibraryInfoImpl TLII(TargetTriple);
  fpm->add(new TargetLibraryInfoWrapperPass(TLII));

  configurePMBuilder(PMBuilder, /*for function passes*/ true);
  PMBuilder.populateFunctionPassManager(*fpm);

  info->FPM_postgen = fpm;

  info->FPM_postgen->doInitialization();

  // Set the floating point optimization level
  // see also code setting targetOptions.UnsafeFPMath etc
  llvm::FastMathFlags FM;
  if (ffloatOpt == 1) {
    // --no-ieee-float
    // Enable all the optimization!
    FM.setFast();
    INT_ASSERT(FM.allowContract());
  } else if (ffloatOpt == 0) {
    // default
    // use a reasonable level of optimization
    FM.setAllowContract(true);
  } else if (ffloatOpt == -1) {
    // --ieee-float
    FM.setAllowContract(true);
  }
  if (gCodegenGPU == false) {
    info->irBuilder->setFastMathFlags(FM);
  }

  checkAdjustedDataLayout();
}

#if HAVE_LLVM_VER >= 140
static void handleErrorLLVM(void* user_data, const char* reason,
                            bool gen_crash_diag)
{
  INT_FATAL("llvm fatal error: %s", reason);
}
#else
static void handleErrorLLVM(void* user_data, const std::string& reason,
                            bool gen_crash_diag)
{
  INT_FATAL("llvm fatal error: %s", reason.c_str());
}
#endif

struct ExternBlockInfo {
  GenInfo* gen_info;
  fileinfo file;
  ExternBlockInfo() : gen_info(NULL), file() { }
 ~ExternBlockInfo() = default;
};

typedef std::set<ModuleSymbol*> module_set_t;
typedef module_set_t::iterator module_set_iterator_t;

module_set_t gModulesWithExternBlocks;

bool lookupInExternBlock(ModuleSymbol* module, const char* name,
                         clang::TypeDecl** cTypeOut,
                         clang::ValueDecl** cValueOut,
                         const char** cCastedToTypeOut,
                         ChapelType** chplTypeOut,
                         astlocT* astlocOut)
{
  if( ! module->extern_info ) return false;
  module->extern_info->gen_info->lvt->getCDecl(name, cTypeOut, cValueOut,
                                               cCastedToTypeOut,
                                               astlocOut);
  VarSymbol* var = module->extern_info->gen_info->lvt->getVarSymbol(name);
  if( var ) *chplTypeOut = var->typeInfo();
  return ( (*cTypeOut) || (*cValueOut) || (*chplTypeOut) );
}
bool alreadyConvertedExtern(ModuleSymbol* module, const char* name)
{
  return module->extern_info->gen_info->lvt->isAlreadyInChapelAST(name);
}
bool setAlreadyConvertedExtern(ModuleSymbol* module, const char* name)
{
  return module->extern_info->gen_info->lvt->markAddedToChapelAST(name);
}

static bool isTargetCpuValid(const char* targetCpu) {
  if (0 == strcmp(targetCpu, "native")) {
    return true;
  } else {
    initializeLlvmTargets();
    std::string triple = getConfiguredTargetTriple();
    std::string err;
    const llvm::Target* tgt = llvm::TargetRegistry::lookupTarget(triple, err);
    if (tgt == nullptr || !err.empty()) {
      return false;
    }
    bool targetCpuValid = false;
    auto ptr = tgt->createMCSubtargetInfo(triple, "", "");
    targetCpuValid = ptr->isCPUStringValid(CHPL_LLVM_TARGET_CPU);
    delete ptr;

    return targetCpuValid;
  }
}

static std::string generateClangGpuLangArgs() {
  std::string args = "";
  if (usingGpuLocaleModel()) {
    args += "-x ";
    switch (getGpuCodegenType()) {
      case GpuCodegenType::GPU_CG_NVIDIA_CUDA:
        args += "cuda";
        break;
      case GpuCodegenType::GPU_CG_AMD_HIP:
        args += "hip";
        break;
    }
  }
  return args;
}

// If we are parsing an extern block with clang, we might
// be configured to use a target compiler that is not clang.
// In that event, filter out everything but the -D and -I arguments,
// since other arguments might not be understood by clang.
//
// This could filter out more aggressively but we do need the
// paths and defines for the runtime headers to work.
static void addFilteredArgs(std::vector<std::string>& dst,
                            std::vector<std::string>& src) {
  bool clang = 0 == strcmp(CHPL_TARGET_COMPILER, "llvm") ||
               0 == strcmp(CHPL_TARGET_COMPILER, "clang");
  bool filter = !clang;

  if (filter) {
    for (size_t i = 0; i < src.size(); i++) {
      const auto& arg = src[i];
      const char* s = arg.c_str();
      if (startsWith(s, "-D") || startsWith(s, "-I")) {
        dst.push_back(arg);
        if (arg.size() == 2 && i+1 < src.size()) {
          // if it was just "-D" or "-I", add the next argument too
          i++;
          dst.push_back(src[i]);
        }
      }
    }

  } else {
    // add all of the arguments without filtering
    for (size_t i = 0; i < src.size(); i++) {
      dst.push_back(src[i]);
    }
  }
}

static void helpComputeClangArgs(std::string& clangCC,
                                 std::string& clangCXX,
                                 std::vector<std::string>& clangCCArgs,
                                 std::vector<std::string>& clangOtherArg,
                                 std::vector<std::string>& clangLDArgs) {
  // These warnings are _required_ to make sure the code Clang generates
  // when compiling the code in Chapel 'extern' blocks will play well
  // with our backend.
  const char* clangRequiredWarningFlags[] = {
    "-Wall",
    "-Werror",
    "-Wpointer-arith",
    "-Wwrite-strings",
    "-Wno-strict-aliasing",
    "-Wmissing-declarations",
    "-Wmissing-prototypes",
    "-Wstrict-prototypes",
    "-Wmissing-format-attribute",
    // clang can't tell which functions we use
    "-Wno-unused-function",
    NULL};

  const char* clang_debug = "-g";
  const char* clang_opt = "-O3";
  const char* clang_fast_float = "-ffast-math";
  const char* clang_ieee_float = "-fno-fast-math";

  BumpPtrAllocator A;
  StringSaver Saver(A);

  // get any args passed to CC/CXX and add them to the builtin clang invocation
  SmallVector<const char *, 0> split;
  llvm::cl::TokenizeGNUCommandLine(CHPL_LLVM_CLANG_C, Saver, split);
  // set clangCC / clangCXX to just the first argument
  for (size_t i = 0; i < split.size(); i++) {
    if (i == 0) {
      clangCC = split[i];
    } else {
      // arguments from CHPL_LLVM_CLANG_C to both CC args and LD args.
      clangCCArgs.push_back(split[i]);
      clangLDArgs.push_back(split[i]);
    }
  }
  split.clear();
  llvm::cl::TokenizeGNUCommandLine(CHPL_LLVM_CLANG_CXX, Saver, split);
  if (split.size() > 0) {
    clangCXX = split[0];
  }


  // add -fPIC if CHPL_LIB_PIC indicates we should.
  // otherwise, pic-or-not will be up to clang's default, which,
  // on many systems, is pic.
  if (strcmp(CHPL_LIB_PIC, "pic") == 0) {
    clangCCArgs.push_back("-fPIC");
  }

  // after arguments provided in CC/CXX
  // add include paths to anything builtin or in CHPL_HOME
  {
    std::vector<std::string> args;
    std::string dashI = "-I";

    // add -I$CHPL_HOME/modules/standard/
    // add -I$CHPL_HOME/modules/packages/
    args.push_back(dashI + CHPL_HOME + "/modules/standard");
    args.push_back(dashI + CHPL_HOME + "/modules/packages");

    // add arguments from CHPL_TARGET_BUNDLED_COMPILE_ARGS
    splitStringWhitespace(CHPL_TARGET_BUNDLED_COMPILE_ARGS, args);
    // Substitute $CHPL_HOME $CHPL_RUNTIME_LIB etc
    expandInstallationPaths(args);

    // add the arguments, filtering if parsing an extern block
    addFilteredArgs(clangCCArgs, args);
  }

  // add a -I. so we can find headers named on command line in same dir
  clangCCArgs.push_back("-I.");

  // add a -I for the generated code directory
  clangCCArgs.push_back(std::string("-I") + getIntermediateDirName());

  // Always add warnings flags
  for (int i = 0; clangRequiredWarningFlags[i]; i++) {
    clangCCArgs.push_back(clangRequiredWarningFlags[i]);
  }

  // Add debug flags
  if (debugCCode) {
    clangCCArgs.push_back(clang_debug);
    clangCCArgs.push_back("-DCHPL_DEBUG");
  }

  // Add optimize flags
  if (optimizeCCode) {
    clangCCArgs.push_back(clang_opt);
    clangCCArgs.push_back("-DCHPL_OPTIMIZE");
  }

  // Add specialization flags
  if (specializeCCode &&
      CHPL_TARGET_CPU_FLAG != NULL &&
      CHPL_LLVM_TARGET_CPU != NULL &&
      CHPL_TARGET_CPU_FLAG[0] != '\0' &&
      CHPL_LLVM_TARGET_CPU[0] != '\0' &&
      0 != strcmp(CHPL_TARGET_CPU_FLAG, "none") &&
      0 != strcmp(CHPL_LLVM_TARGET_CPU, "none") &&
      0 != strcmp(CHPL_LLVM_TARGET_CPU, "unknown")) {

    // Check that the requested CPU is valid
    bool targetCpuValid = isTargetCpuValid(CHPL_LLVM_TARGET_CPU);
    if (!targetCpuValid) {
      USR_WARN("Unknown target CPU %s -- not specializing",
               CHPL_LLVM_TARGET_CPU);
      std::string triple = getConfiguredTargetTriple();
      USR_PRINT("To see available CPU types, run "
                "%s --target=%s --print-supported-cpus",
                clangCC.c_str(), triple.c_str());

    } else {
      std::string march = "-m";
      march += CHPL_TARGET_CPU_FLAG;
      march += "=";
      march += CHPL_LLVM_TARGET_CPU;
      clangCCArgs.push_back(march);
    }
  }

  // Passing -ffast-math is important to get approximate versions
  // of cabs but it appears to slow down simple complex multiplication.
  if (ffloatOpt > 0) // --no-ieee-float
    clangCCArgs.push_back(clang_fast_float); // --ffast-math

  if (ffloatOpt < 0) // --ieee-float
    clangCCArgs.push_back(clang_ieee_float); // -fno-fast-math

  // Add include directories specified on the command line
  for_vector(const char, dirName, incDirs) {
    clangCCArgs.push_back(std::string("-I") + dirName);
  }

  // Add C compilation flags from the command line (--ccflags arguments)
  splitStringWhitespace(ccflags, clangCCArgs);

  // Library directories/files and ldflags are handled during linking later.
  // Skip this in multi-locale libraries because the C blob that is built
  // already defines this.
  if (!fMultiLocaleInterop) {
    clangCCArgs.push_back("-DCHPL_GEN_CODE");
  }

  // add -pthread since we will use pthreads
  clangCCArgs.push_back("-pthread");

  // add system compiler args from printchplenv
  {
    std::vector<std::string> args;
    splitStringWhitespace(CHPL_TARGET_SYSTEM_COMPILE_ARGS, args);
    // add the arguments, filtering if parsing an extern block
    addFilteredArgs(clangCCArgs, args);
  }
}

void computeClangArgs(std::vector<std::string>& clangCCArgs) {
  std::string clangCC, ignoredClangCXX;
  std::vector<std::string> ignoredOtherArgs, ignoredLDArgs;

  std::vector<std::string> gotCCArgs;

  helpComputeClangArgs(clangCC, ignoredClangCXX,
                       gotCCArgs,
                       ignoredOtherArgs, ignoredLDArgs);

  // append clangCC argument first
  if (clangCC == "") {
    clangCC = "clang";
  }
  clangCCArgs.push_back(clangCC);

  // append the other arguments
  clangCCArgs.insert(clangCCArgs.end(), gotCCArgs.begin(), gotCCArgs.end());
}

// if just_parse_filename != NULL, it is a file
// containing an extern block to parse only
// (and in that setting there is no need to work with the runtime).
void runClang(const char* just_parse_filename) {
  bool parseOnly = (just_parse_filename != NULL);
  static bool is_installed_fatal_error_handler = false;

  // find the path to clang and clang++
  std::string clangCC, clangCXX;
  std::vector<std::string> clangCCArgs;
  std::vector<std::string> clangOtherArgs;
  std::vector<std::string> clangLDArgs;
  helpComputeClangArgs(clangCC, clangCXX,
                       clangCCArgs, clangOtherArgs, clangLDArgs);

  // tell clang to use CUDA/AMD support
  if (usingGpuLocaleModel()) {
    // Need to pass this flag so atomics header will compile
    clangOtherArgs.push_back("--std=c++11");

    // Need to select CUDA/AMD mode in embedded clang to
    // activate the GPU target
    splitStringWhitespace(generateClangGpuLangArgs(), clangOtherArgs);

    std::string gpuArch = std::string("--offload-arch=") + CHPL_GPU_ARCH;
    clangOtherArgs.push_back(gpuArch);
  }

  // Always include sys_basic because it might change the
  // behaviour of macros!
  clangOtherArgs.push_back("-include");
  clangOtherArgs.push_back("sys_basic.h");

  if (!parseOnly) {
    if (usingGpuLocaleModel()) {
      //create a header file to include header files from the command line
      std::string genHeaderFilename;
      genHeaderFilename = genIntermediateFilename("command-line-includes.h");
      FILE* fp =  openfile(genHeaderFilename.c_str(), "w");
      if(usingGpuLocaleModel()) {
        // In some version of the CUDA headers they end up redefining
        // __noinline__, which is used as an attribute in gcc. This was
        // causing us to fail to compile Arkouda and so we undef it
        // here as a workaround
        fprintf(fp, "#undef __noinline__\n");
      }

      int filenum = 0;
      while (const char* inputFilename = nthFilename(filenum++)) {
        if (isCHeader(inputFilename)) {
          fprintf(fp, "%s%s%s\n", "#include \"", inputFilename,"\"");
        }
      }

      closefile(fp);
      clangOtherArgs.push_back("-include");
      clangOtherArgs.push_back(genHeaderFilename);
    }

    // Running clang to compile all runtime and extern blocks

    // Include header files from the command line.
    else {
      int filenum = 0;
      while (const char* inputFilename = nthFilename(filenum++)) {
        if (isCHeader(inputFilename)) {
          clangOtherArgs.push_back("-include");
          clangOtherArgs.push_back(inputFilename);
        }
      }
    }

    // Include header containing libc wrappers
    clangOtherArgs.push_back("-include");
    clangOtherArgs.push_back("llvm/chapel_libc_wrapper.h");

    // Include extern C blocks
    if( fAllowExternC && gAllExternCode.filename ) {
      clangOtherArgs.push_back("-include");
      clangOtherArgs.push_back(gAllExternCode.filename);
    }

    // Include a few extra things if generating a multi-locale library.
    if (fMultiLocaleInterop) {

      // Include the contents of the server bundle...
      clangOtherArgs.push_back("-include");
      INT_ASSERT(gMultiLocaleLibServerFile != NULL);
      clangOtherArgs.push_back(gMultiLocaleLibServerFile);

      // As well as the path to extra code for the client and server.
      std::string incPath = std::string("-I") + std::string(CHPL_HOME);
      incPath += "/runtime/etc/src/";
      clangOtherArgs.push_back(incPath);
    }
  } else {
    // Just running clang to parse the extern blocks for this module.
    clangOtherArgs.push_back("-include");
    clangOtherArgs.push_back(just_parse_filename);
  }

  if( printSystemCommands ) {
    if (parseOnly)
      printf("<internal clang parsing %s> ", just_parse_filename);
    else
      printf("<internal clang code generation> ");

    for( size_t i = 0; i < clangCCArgs.size(); i++ ) {
      printf("%s ", clangCCArgs[i].c_str());
    }
    for( size_t i = 0; i < clangOtherArgs.size(); i++ ) {
      printf("%s ", clangOtherArgs[i].c_str());
    }
    printf("\n");
  }


  // Initialize gGenInfo
  // Toggle LLVM code generation in our clang run;
  // turn it off if we just wanted to parse some C.
  if (parseOnly) {
    // TODO (dlongnecke): Always initialize outside of this function?
    gGenInfo = new GenInfo();
  } else {
    // Note that if we are calling 'runClang(NULL)' then the final gGenInfo
    // Should have already been initialized for us.
    INT_ASSERT(gGenInfo != NULL);
  }

  gGenInfo->lvt = std::make_unique<LayeredValueTable>();


  ClangInfo* clangInfo = NULL;
  clangInfo = new ClangInfo(clangCC, clangCXX,
                            clangCCArgs, clangOtherArgs, clangLDArgs,
                            parseOnly);

  gGenInfo->clangInfo = clangInfo;

  std::string home(CHPL_HOME);
  std::string rtmain = home + "/runtime/etc/rtmain.c";

  setupClang(gGenInfo, rtmain);

  if( fLlvmCodegen || fAllowExternC )
  {
    GenInfo *info = gGenInfo;

    // Install an LLVM Fatal Error Handler.
    if (!is_installed_fatal_error_handler) {
      is_installed_fatal_error_handler = true;
      install_fatal_error_handler(handleErrorLLVM);
    }

    // Run the Start Generation action
    // Now initialize a code generator...
    // this will enable us to ask for addresses of static (inline) functions
    // and cause them to be emitted eventually.
    // CCodeGenAction is defined above. It traverses the C AST
    // and does the code generation.
    clangInfo->cCodeGenAction = new CCodeGenAction();
    if (!clangInfo->Clang->ExecuteAction(*clangInfo->cCodeGenAction)) {
      if (parseOnly) {
        USR_FATAL("error running clang on extern block");
      } else {
        USR_FATAL("error running clang during code generation");
      }
    }
    if( ! parseOnly ) {
      // LLVM module should have been created by CCodeGenConsumer
      INT_ASSERT(gGenInfo->module);

      // Create a new IRBuilder
      gGenInfo->irBuilder = new llvm::IRBuilder<>(gGenInfo->module->getContext());

      // This seems to be needed, even though it is strange.
      // (otherwise we segfault in info->irBuilder->CreateGlobalString)

      // Some IRBuilder methods, codegenning a string,
      // need a basic block in order to get to the module
      // so we create a dummy function to code generate into
      llvm::Type * voidTy =  llvm::Type::getVoidTy(info->module->getContext());
      std::vector<llvm::Type*> args;
      llvm::FunctionType * FT = llvm::FunctionType::get(voidTy, args, false);
      Function * F =
        Function::Create(FT, Function::InternalLinkage,
                         "chplDummyFunction", info->module);
      llvm::BasicBlock *block =
        llvm::BasicBlock::Create(info->module->getContext(), "entry", F);
      info->irBuilder->SetInsertPoint(block);
    }
    // read macros. May call IRBuilder methods to codegen a string,
    // so needs to happen after we set the insert point.
    readMacrosClang();

    if( ! parseOnly ) {
      info->irBuilder->CreateRetVoid();
    }
  }
}

static
void saveExternBlock(ModuleSymbol* module, const char* extern_code)
{
  if( ! gAllExternCode.filename ) {
    openCFile(&gAllExternCode, "extern-code", "c");
    INT_ASSERT(gAllExternCode.fptr);
  }

  if( ! module->extern_info ) {
    // Figure out what file to place the C code into.
    module->extern_info = new ExternBlockInfo();
    const char* name = astr("extern_block_", module->cname);
    openCFile(&module->extern_info->file, name, "c");
    // Could put #ifndef/define/endif wrapper start here.
  }
  FILE* f = module->extern_info->file.fptr;
  INT_ASSERT(f);
  // Append the C code to that file.
  fputs(extern_code, f);
  // Always make sure it ends in a close semi (solves errors)
  fputs("\n;\n", f);
  // Add this module to the set of modules needing extern compilation.
  std::pair<module_set_iterator_t,bool> already_there;
  already_there = gModulesWithExternBlocks.insert(module);
  if( already_there.second ) {
    // A new element was added to the map ->
    //   first time we have worked with this module.
    // Add a #include of this module's extern block code to the
    //   global extern code file.
    fprintf(gAllExternCode.fptr,
           "#include \"%s\"\n", module->extern_info->file.filename);
  }
}


void readExternC(void) {
  // Handle extern C blocks.
  forv_Vec(ExternBlockStmt, eb, gExternBlockStmts) {
    // Figure out the parent module symbol.
    ModuleSymbol* module = eb->getModule();
    saveExternBlock(module, eb->c_code);
  }

  // Close extern_c_file.
  if( gAllExternCode.fptr ) closefile(&gAllExternCode);
  // Close any extern files for any modules we had generated code for.
  module_set_iterator_t it;
  for( it = gModulesWithExternBlocks.begin();
       it != gModulesWithExternBlocks.end();
       ++it ) {
    ModuleSymbol* module = *it;
    INT_ASSERT(module->extern_info);
    // Could put #ifndef/define/endif wrapper end here.
    closefile(&module->extern_info->file);
    // Now parse the extern C code for that module.
    runClang(module->extern_info->file.filename);
    // Now swap what went into the global layered value table
    // into the module's own layered value table.
    module->extern_info->gen_info = gGenInfo;
    gGenInfo = NULL;
  }
}

void cleanupExternC(void) {
  module_set_iterator_t it;
  for( it = gModulesWithExternBlocks.begin();
       it != gModulesWithExternBlocks.end();
       ++it ) {
    ModuleSymbol* module = *it;
    INT_ASSERT(module->extern_info);
    cleanupClang(module->extern_info->gen_info->clangInfo);
    delete module->extern_info->gen_info->clangInfo;
    delete module->extern_info->gen_info;
    delete module->extern_info;
    module->extern_info = nullptr;
    // Remove all ExternBlockStmts from this module.
    forv_Vec(ExternBlockStmt, eb, gExternBlockStmts) {
      eb->remove();
    }
    gExternBlockStmts.clear();
  }
}

llvm::Function* getFunctionLLVM(const char* name)
{
  GenInfo* info = gGenInfo;
  Function* fn = info->module->getFunction(name);
  if( fn ) return fn;

  GenRet got = info->lvt->getValue(name);
  if( got.val ) {
    fn = cast<Function>(got.val);
    return fn;
  }

  return NULL;
}

clang::FunctionDecl* getFunctionDeclClang(const char* name)
{
  GenInfo* info = gGenInfo;

  clang::TypeDecl* cType = NULL;
  clang::ValueDecl* cValue = NULL;

  info->lvt->getCDecl(name, &cType, &cValue);
  clang::FunctionDecl* FD = llvm::dyn_cast_or_null<clang::FunctionDecl>(cValue);
  return FD;
}

llvm::Type* getTypeLLVM(const char* name)
{
  GenInfo* info = gGenInfo;
#if HAVE_LLVM_VER >= 120
  llvm::Type* t = llvm::StructType::getTypeByName(info->llvmContext, name);
#else
  llvm::Type* t = info->module->getTypeByName(name);
#endif

  if( t ) return t;

  t = info->lvt->getType(name);
  if( t ) return t;

  return NULL;
}
// should support TypedefDecl,EnumDecl,RecordDecl
llvm::Type* codegenCType(const TypeDecl* td)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenerator* cCodeGen = clangInfo->cCodeGen;
  INT_ASSERT(cCodeGen);

  QualType qType;

  // handle TypedefDecl
  if( const TypedefNameDecl* tnd = dyn_cast<TypedefNameDecl>(td) ) {
    qType = tnd->getCanonicalDecl()->getUnderlyingType();
  } else if( const EnumDecl* ed = dyn_cast<EnumDecl>(td) ) {
    qType = ed->getCanonicalDecl()->getIntegerType();
  } else if( const RecordDecl* rd = dyn_cast<RecordDecl>(td) ) {
    RecordDecl *def = rd->getDefinition();
    if (def == nullptr) {
      // it's an opaque type - definition of fields not available
      qType=rd->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
    } else {
      qType=def->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
    }
  } else {
    INT_FATAL("Unknown clang type declaration");
  }
  return clang::CodeGen::convertTypeForMemory(cCodeGen->CGM(), qType);
}

llvm::Type* codegenCType(const clang::QualType& qType)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenerator* cCodeGen = clangInfo->cCodeGen;
  INT_ASSERT(cCodeGen);

  return clang::CodeGen::convertTypeForMemory(cCodeGen->CGM(), qType);
}

// should support FunctionDecl,VarDecl,EnumConstantDecl
GenRet codegenCValue(const ValueDecl *vd)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenerator* cCodeGen = clangInfo->cCodeGen;
  INT_ASSERT(cCodeGen);

  GenRet ret;

  if( info->cfile ) {
    ret.c = std::string(vd->getName());
    return ret;
  }

  if(const FunctionDecl *fd = dyn_cast<FunctionDecl>(vd)) {
    // It's a function decl.
    ret.val = cCodeGen->GetAddrOfGlobal(GlobalDecl(fd), false);
    ret.isLVPtr = GEN_VAL;
  } else if(const VarDecl *vard = dyn_cast<VarDecl>(vd)) {
    // It's a (global) variable decl
    ret.val = cCodeGen->GetAddrOfGlobal(GlobalDecl(vard), false);
    ret.isLVPtr = GEN_PTR;
  } else if(const EnumConstantDecl *ed = dyn_cast<EnumConstantDecl>(vd)) {
    // It's a constant enum value
    APInt v = ed->getInitVal();

    ret.isUnsigned = ! ed->getType()->hasSignedIntegerRepresentation();

    llvm::Type* type = NULL;
    type = clang::CodeGen::convertTypeForMemory(cCodeGen->CGM(), ed->getType());

    ret.val = ConstantInt::get(type, v);
    ret.isLVPtr = GEN_VAL;
  } else {
    INT_FATAL("Unknown clang value declaration");
  }
  return ret;
}

static std::map<const clang::Decl*, const char*> savedAnonDeclNames;

static
const char* makeNameForAnonDecl(const clang::Decl* decl)
{
  // This has to return the same value when applied to the same union
  SourceManager& srcm = gGenInfo->clangInfo->Ctx->getSourceManager();
  clang::SourceLocation loc = decl->getLocation();
  std::string filename = srcm.getFilename(loc).str();
  unsigned offset = srcm.getFileOffset(loc);
  std::string str = "chpl_anon_struct_" + filename + std::to_string(offset);

  const char* ret = astr(legalizeName(str.c_str()));
  savedAnonDeclNames[decl] = ret;
  return ret;
}
const char* getGeneratedAnonTypeName(const clang::RecordType* structType) {
  const clang::Decl* decl = structType->getDecl();
  const char* ret = savedAnonDeclNames[decl];
  INT_ASSERT(ret);
  return ret;
}

LayeredValueTable::LayeredValueTable(){
  layers.push_front(map_type());
}

void LayeredValueTable::addLayer(){
  layers.push_front(map_type());
}

void LayeredValueTable::removeLayer(){
  if(layers.size() != 1) {
    layers.pop_front();
  }
}


void LayeredValueTable::addValue(
    StringRef name, Value *value, uint8_t isLVPtr, bool isUnsigned) {
  Storage store;
  store.u.value = value;
  store.isLVPtr = isLVPtr;
  store.isUnsigned = isUnsigned;
  (layers.front())[name] = store;
}

void LayeredValueTable::addGlobalValue(
    StringRef name, Value *value, uint8_t isLVPtr, bool isUnsigned, ::Type* type) {
  Storage store;
  store.u.value = value;
  store.isLVPtr = isLVPtr;
  store.isUnsigned = isUnsigned;
  (layers.back())[name] = store;
}

void LayeredValueTable::addGlobalValue(StringRef name, GenRet gend) {
  addGlobalValue(name, gend.val, gend.isLVPtr, gend.isUnsigned, gend.chplType);
}

void LayeredValueTable::addGlobalType(StringRef name, llvm::Type *type, bool isUnsigned) {
  Storage store;
  store.u.type = type;
  store.isUnsigned = isUnsigned;
  /*fprintf(stderr, "Adding global type %s ", name.str().c_str());
  type->dump();
  fprintf(stderr, "\n");
  */
  (layers.back())[name] = store;
}

void LayeredValueTable::addGlobalCDecl(NamedDecl* cdecl) {
  llvm::StringRef name = "";

  // Certain C++ things such as constructors can have
  // special compound names. In this case getName() will
  // fail.
  if (cdecl->getIdentifier() != nullptr) {
    name = cdecl->getName();
  }

  if (name.empty()) {
    const char* name_astr = makeNameForAnonDecl(cdecl);
    name = name_astr;
  }

  addGlobalCDecl(name, cdecl);

  // Also file structs under 'struct struct_name'
  if(RecordDecl* rd = dyn_cast<RecordDecl>(cdecl)) {
    if(rd->isUnion()) {
      std::string sname = "union ";
      sname += name;
      addGlobalCDecl(sname, cdecl);
    } else if (rd->isStruct()) {
      std::string sname = "struct ";
      sname += name;
      addGlobalCDecl(sname, cdecl);
    }
  }
}

void LayeredValueTable::addGlobalCDecl(StringRef name, NamedDecl* cdecl, const char* castToType) {

  if(RecordDecl *rd = dyn_cast<RecordDecl>(cdecl)) {
    // For record decls, always use the completed definition
    // if it is available.
    // E.g., we might visit decls in this order:
    //   struct stat { ... }
    //   struct stat;

    RecordDecl* completed = rd->getDefinition();
    if (completed) {
      // Use the completed version below.
      cdecl = completed;
    }
  }

  // Add to an existing Storage record, so that it can store
  // both a type and a value (e.g. struct stat and function stat).
  Storage & store = (layers.back())[name];
  if (isa<TypeDecl>(cdecl)) store.u.cTypeDecl = cast<TypeDecl>(cdecl);
  if (isa<ValueDecl>(cdecl)) {
    store.u.cValueDecl = cast<ValueDecl>(cdecl);
    store.u.castChplVarTo = castToType;
  }

  store.astloc = getClangDeclLocation(cdecl);
}


void LayeredValueTable::addGlobalVarSymbol(llvm::StringRef name, VarSymbol* var, const char* castToType)
{
  Storage store;
  store.u.chplVar = var;
  store.u.castChplVarTo = castToType;
  (layers.back())[name] = store;
}

void LayeredValueTable::addBlock(StringRef name, llvm::BasicBlock *block) {
  Storage store;
  store.u.block = block;

  layer_iterator blockLayer = --layers.end();
  if(layers.size() > 1) {
    blockLayer = --blockLayer;
  }
  (*blockLayer)[name] = store;
}

void LayeredValueTable::addMacro(StringRef name,
                                 const clang::MacroInfo *macro) {
  Storage store;
  store.u.macro = macro;
  (layers.back())[name] = store;
}

void LayeredValueTable::addForwardName(llvm::StringRef name, const char* forwardName) {
  Storage store;
  store.u.forwardToName = astr(forwardName);
  (layers.back())[name] = store;
}

GenRet LayeredValueTable::getValue(StringRef name) {
  if(Storage *store = get(name)) {
    if (store->u.forwardToName) {
      store = get(store->u.forwardToName);
    }

    if( store->u.value ) {
      INT_ASSERT(isa<Value>(store->u.value));
      GenRet ret;
      ret.val = store->u.value;
      ret.isLVPtr = store->isLVPtr;
      ret.isUnsigned = store->isUnsigned;
      ret.chplType = store->chplType;
      return ret;
    }
    if( store->u.cValueDecl ) {
      INT_ASSERT(isa<ValueDecl>(store->u.cValueDecl));
      // we have a clang value decl.
      // maybe FunctionDecl,VarDecl,EnumConstantDecl

      // Convert it to an LLVM value
      // should support FunctionDecl,VarDecl,EnumConstantDecl
      GenRet ret = codegenCValue(store->u.cValueDecl);
      store->u.value = ret.val;
      store->isLVPtr = ret.isLVPtr;
      store->isUnsigned = ret.isUnsigned;

      return ret;
    }
    if( store->u.chplVar && isVarSymbol(store->u.chplVar) ) {
      VarSymbol* var = store->u.chplVar;
      GenRet ret = var; // code generate it!

      if (store->u.castChplVarTo) {
        ret = codegenCast(store->u.castChplVarTo, ret);
      }

      store->u.value = ret.val;
      store->isLVPtr = ret.isLVPtr;
      store->isUnsigned = ret.isUnsigned;

      return ret;
    }
  }
  GenRet ret;
  return ret;
}

llvm::BasicBlock *LayeredValueTable::getBlock(StringRef name) {
  if(Storage *store = get(name)) {
    if( store->u.block && isa<llvm::BasicBlock>(store->u.block) )
      return store->u.block;
  }
  return NULL;
}

llvm::Type *LayeredValueTable::getType(StringRef name, bool *isUnsigned) {
  if(Storage *store = get(name)) {
    if (isUnsigned != NULL) {
      *isUnsigned = store->isUnsigned;
    }

    if( store->u.type ) {
      INT_ASSERT(isa<llvm::Type>(store->u.type));
      return store->u.type;
    }
    if( store->u.cTypeDecl ) {
      INT_ASSERT(isa<TypeDecl>(store->u.cTypeDecl));
      // we have a clang type decl.
      // maybe TypedefDecl,EnumDecl,RecordDecl

      // Convert it to an LLVM type.
      store->u.type = codegenCType(store->u.cTypeDecl);
      const clang::Type *type = store->u.cTypeDecl->getTypeForDecl();
      if (type != NULL) {
        store->isUnsigned = type->isUnsignedIntegerOrEnumerationType();
      }
      return store->u.type;
    }
  }
  return NULL;
}

// Returns a type or a name decl for a given name
// Note that C can have a function and a type sharing the same name
// (e.g. stat/struct stat).
// Sets the output arguments to NULL if a type/value was not found.
// Either output argument can be NULL.
void LayeredValueTable::getCDecl(StringRef name, TypeDecl** cTypeOut,
    ValueDecl** cValueOut, const char** cCastedToTypeOut, astlocT* astlocOut) {

  if (cValueOut) *cValueOut = NULL;
  if (cTypeOut) *cTypeOut = NULL;
  if (cCastedToTypeOut) *cCastedToTypeOut = NULL;

  if(Storage *store = get(name)) {
    if (store->u.forwardToName) {
      store = get(store->u.forwardToName);
    }
    if( store->u.cValueDecl ) {
      INT_ASSERT(isa<ValueDecl>(store->u.cValueDecl));
      // we have a clang value decl.
      // maybe FunctionDecl,VarDecl,EnumConstantDecl

      if (cValueOut) *cValueOut = store->u.cValueDecl;
    }
    if( store->u.cTypeDecl ) {
      INT_ASSERT(isa<TypeDecl>(store->u.cTypeDecl));
      // we have a clang type decl.
      // maybe TypedefDecl,EnumDecl,RecordDecl

      if (cTypeOut) *cTypeOut = store->u.cTypeDecl;
    }
    if (store->u.castChplVarTo) {
      if (cCastedToTypeOut) *cCastedToTypeOut = store->u.castChplVarTo;
    }
    if (astlocOut) *astlocOut = store->astloc;
  }
}

bool LayeredValueTable::isCArray(StringRef cname) {
  clang::TypeDecl* cType = NULL;
  clang::ValueDecl* cValue = NULL;

  this->getCDecl(cname, &cType, &cValue);

  return (cValue && cValue->getType().getTypePtr()->isArrayType());
}

VarSymbol* LayeredValueTable::getVarSymbol(StringRef name) {
  if(Storage *store = get(name)) {
    if( store->u.chplVar && isVarSymbol(store->u.chplVar) ) {
      // we have a Chapel Var Symbol.
      // maybe immediate number or string, possibly variable reference.
      // These come from macros.
      return store->u.chplVar;
    }
  }
  return NULL;
}

const clang::MacroInfo* LayeredValueTable::getMacro(StringRef name) {
  if (Storage *store = get(name)) {
    if (store->u.macro) {
      INT_ASSERT(isa<clang::MacroInfo>(store->u.macro));
      return store->u.macro;
    }
  }
  return NULL;
}

LayeredValueTable::Storage* LayeredValueTable::get(StringRef name) {
  for(layer_iterator i = layers.begin(); i != layers.end(); ++i) {
    value_iterator j = i->find(name);

    if(j != i->end())
    {
      return &j->second;
    }
  }

  return NULL;
}

bool LayeredValueTable::isAlreadyInChapelAST(llvm::StringRef name)
{
  if(Storage *store = get(name)) {
    return store->addedToChapelAST;
  }
  return false;
}

bool LayeredValueTable::markAddedToChapelAST(llvm::StringRef name)
{
  if(Storage *store = get(name)) {
    if( store->addedToChapelAST ) return false;
    store->addedToChapelAST = true;
    return true;
  } else {
    // Otherwise, make a new entry.
    Storage toStore;
    toStore.addedToChapelAST = true;
    (layers.back())[name] = toStore;
    return true;
  }
}

void LayeredValueTable::swap(LayeredValueTable* other)
{
  this->layers.swap(other->layers);
}

int getCRecordMemberGEP(const char* typeName, const char* fieldName,
                        bool& isCArrayField)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenerator* cCodeGen = clangInfo->cCodeGen;
  INT_ASSERT(cCodeGen);

  TypeDecl* d = NULL;
  int ret;

  info->lvt->getCDecl(typeName, &d, NULL);
  INT_ASSERT(d);
  if( isa<TypedefDecl>(d) ) {
    TypedefDecl* td = cast<TypedefDecl>(d);
    const clang::Type* t = td->getUnderlyingType().getTypePtr();
    while( t->isPointerType() ) {
      t = t->getPointeeType().getTypePtr();
    }
    const RecordType* rt = t->getAsStructureType();
    if (rt == nullptr)
      rt = t->getAsUnionType();

    INT_ASSERT(rt);
    d = rt->getDecl();
  }
  INT_ASSERT(isa<RecordDecl>(d));
  RecordDecl* rec = cast<RecordDecl>(d);
  // Find the field decl.
  RecordDecl::field_iterator it;
  FieldDecl* field = NULL;
  for( it = rec->field_begin(); it != rec->field_end(); ++it ) {
    if( fieldName == it->getName() ) {
      field = *it;
      break;
    }
  }

  // TODO (dlongnecke): Try to move this to a spot before codegen? It would
  // also be cool if we could point this to the location in the source code
  // where the field is being accessed.
  if (!field) {
    auto fmt = "Definition of '%s.%s' not visible in external C code";
    USR_FATAL(fmt, typeName, fieldName);
  }

  INT_ASSERT(field);

  isCArrayField = field->getType()->isArrayType();

  ret = clang::CodeGen::getLLVMFieldNumber(cCodeGen->CGM(), rec, field);

  INT_ASSERT(ret >= 0);

  return ret;
}

static clang::CanQualType getClangType(::Type* t, bool makeRef) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::ASTContext *Ctx = clangInfo->Ctx;
  INT_ASSERT(Ctx);

  INT_ASSERT(!t->isWideRef());

  if (makeRef || t->isRef()) {
    ::Type* eltType = t->getValType();
    clang::CanQualType cTy = getClangType(eltType, false);
    // TODO: const qualify const refs
    cTy = Ctx->getPointerType(cTy);
    return cTy;
  }
  TypeSymbol* ts = t->symbol;
  if (ts->hasFlag(FLAG_C_PTR_CLASS) || ts->hasFlag(FLAG_DATA_CLASS)) {
    ::Type* eltType = getDataClassType(ts)->type;
    clang::CanQualType cTy = getClangType(eltType, false);
    // TODO: const qualify const C pointers
    cTy = Ctx->getPointerType(cTy);
    return cTy;
  }
  if (ts->hasFlag(FLAG_STAR_TUPLE)) {
    AggregateType* at = toAggregateType(t);
    Symbol* field = at->getField("x0");
    ::Type* eltType = field->type;
    clang::CanQualType cTy = getClangType(eltType, false);
    cTy = Ctx->getPointerType(cTy);
    return cTy;
  }

  if (t == dtVoid || t == dtNothing)
    return Ctx->VoidTy;
  // could match other builtin types like c_void_ptr or c_int here

  clang::TypeDecl* cTypeDecl = NULL;
  clang::ValueDecl* cValueDecl = NULL;
  const char* cCastedToType = NULL;
  const char* cname = t->symbol->cname;
  info->lvt->getCDecl(cname, &cTypeDecl, &cValueDecl, &cCastedToType);

  if (cCastedToType)
    USR_FATAL(t, "Cannot use macro with type cast in export function argument");

  if (cTypeDecl == NULL)
    USR_FATAL(t, "Could not find C type %s - "
                  "extern/export functions should only use extern types",
                   cname);

  clang::QualType cQualType = Ctx->getTypeDeclType(cTypeDecl);
  clang::CanQualType cTy = cQualType->getCanonicalTypeUnqualified();

  return cTy;
}

static clang::CanQualType getClangFormalType(ArgSymbol* formal) {
  ::Type* t = formal->type;

  bool ref = (formal->intent & INTENT_FLAG_REF) ||
             (formal->requiresCPtr() &&
              formal->type->getValType()->symbol->hasFlag(FLAG_TUPLE));

  if (formal->isWideRef())
    USR_FATAL(formal, "Cannot use wide reference in exported function");

  return getClangType(t, ref);
}

const clang::CodeGen::CGFunctionInfo& getClangABIInfoFD(clang::FunctionDecl* FD)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenerator* cCodeGen = clangInfo->cCodeGen;
  INT_ASSERT(cCodeGen);
  clang::CodeGen::CodeGenModule& CGM = cCodeGen->CGM();

  clang::CanQualType FTy = FD->getType()->getCanonicalTypeUnqualified();
  assert(llvm::isa<clang::FunctionType>(FTy));

  if (clang::CanQual<clang::FunctionNoProtoType> noProto =
      FTy.getAs<clang::FunctionNoProtoType>()) {
    return clang::CodeGen::arrangeFreeFunctionType(CGM, noProto);
  }

  clang::CanQual<clang::FunctionProtoType> proto =
             FTy.getAs<clang::FunctionProtoType>();
#if HAVE_LLVM_VER >= 90
  return clang::CodeGen::arrangeFreeFunctionType(CGM, proto);
#else
  return clang::CodeGen::arrangeFreeFunctionType(CGM, proto, FD);
#endif
}


const clang::CodeGen::CGFunctionInfo& getClangABIInfo(FnSymbol* fn) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenerator* cCodeGen = clangInfo->cCodeGen;
  INT_ASSERT(cCodeGen);
  clang::CodeGen::CodeGenModule& CGM = cCodeGen->CGM();

  // Lookup the clang AST for this function so we can
  // use the C ABI

  clang::TypeDecl* cType = NULL;
  clang::ValueDecl* cValue = NULL;
  const char* cCastedToType = NULL;

  info->lvt->getCDecl(fn->cname, &cType, &cValue, &cCastedToType);

  clang::FunctionDecl* FD = llvm::dyn_cast_or_null<clang::FunctionDecl>(cValue);
  if (FD != NULL) {
    // This is the typical case for extern functions.

    // This case could be supported if we need it to be
    if (cCastedToType)
      INT_FATAL(fn, "Cannot call casted macro for %s", fn->cname);

    return getClangABIInfoFD(FD);
  }

  // Otherwise, we should call arrangeFreeFunctionCall
  // with the various types, which must be extern types.
  // (An alternative strategy would be to generate the C headers
  //  for these types before creating this clang parser).
  llvm::SmallVector<clang::CanQualType,4> argTypesC;

  // Convert each formal to a Clang type.
  for_formals(formal, fn) {
    clang::CanQualType argTyC = getClangFormalType(formal);
    argTypesC.push_back(argTyC);
  }

  // Convert the return type
  bool retRef = false;
  if (fn->retTag == RET_REF || fn->retTag == RET_CONST_REF)
    retRef = true;
  if (fn->retType->isWideRef())
    USR_FATAL(fn, "Cannot return wide reference in exported function");

  clang::CanQualType retTyC = getClangType(fn->retType, retRef);

  auto extInfo = clang::FunctionType::ExtInfo();

  return clang::CodeGen::arrangeFreeFunctionCall(CGM, retTyC, argTypesC,
                                 extInfo, clang::CodeGen::RequiredArgs::All);
}

const clang::CodeGen::ABIArgInfo*
getCGArgInfo(const clang::CodeGen::CGFunctionInfo* CGI, int curCArg)
{

  // Don't try to use the calling convention code for variadic args.
  if ((unsigned) curCArg >= CGI->arg_size() && CGI->isVariadic())
    return NULL;

  const clang::CodeGen::ABIArgInfo* argInfo = NULL;
#if HAVE_LLVM_VER >= 100
  llvm::ArrayRef<clang::CodeGen::CGFunctionInfoArgInfo> a=CGI->arguments();
  argInfo = &a[curCArg].info;
#else
  int i = 0;
  for (auto &ii : CGI->arguments()) {
    if (i == curCArg) {
      argInfo = &ii.info;
      break;
    }
    i++;
  }
#endif

  return argInfo;
}

//
// Issue #19218 describes a bug encountered on darwin+arm64 machines where
// passing an extern record by value in LLVM resulted in incorrect assembly
// code.
//
// A fix is to use the `getSingleCGArgInfo` function to determine if Clang
// would pass such an extern record by indirect pointer, and match that
// behavior in our generated LLVM.
//
// This function returns true if we should use the ABI information from Clang
// to apply the fix.
//
bool useDarwinArmFix(::Type* type) {
  static bool isDarwin = strcmp(CHPL_TARGET_PLATFORM, "darwin") == 0;
  static bool isArm = strcmp(CHPL_TARGET_ARCH, "arm64") == 0;

  if (type != nullptr && isDarwin && isArm &&
      type->symbol->hasFlag(FLAG_EXTERN) &&
      isRecord(type)) {
    return true;
  }

  return false;
}

//
// Returns an ArgInfo for the given formal.
//
// The ArgInfo is acquired by manufacturing a function call in clang where the
// formal is the only argument.
//
// Note: this function exists as a workaround because `getClangABIInfo` only
// works if all of the function's formals are extern types.
// `getSingleCGArgInfo` is intended to be used in **any** situation where we
// are passing an extern record by value.
//
// For example, in the following function we need to pass `A` by indirect
// pointer, but `getClangABIInfo` cannot currently handle the formal `B`
// because its type is not extern.
//
//   proc helper(in A : externRec, B : chapelRec)
//
// TODO: Update `getClangABIInfo` to handle formal types and return types that
// are not extern types.
//
const clang::CodeGen::ABIArgInfo*
getSingleCGArgInfo(::Type* type) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenerator* cCodeGen = clangInfo->cCodeGen;
  INT_ASSERT(cCodeGen);
  clang::CodeGen::CodeGenModule& CGM = cCodeGen->CGM();

  llvm::SmallVector<clang::CanQualType,4> argTypesC;

  // Convert formal to a Clang type.
  clang::CanQualType argTyC = getClangType(type, false);
  argTypesC.push_back(argTyC);

  auto extInfo = clang::FunctionType::ExtInfo();

  auto& CGI = clang::CodeGen::arrangeFreeFunctionCall(CGM, argTyC, argTypesC,
                                 extInfo, clang::CodeGen::RequiredArgs::All);
  llvm::ArrayRef<clang::CodeGen::CGFunctionInfoArgInfo> a=CGI.arguments();
  const clang::CodeGen::ABIArgInfo* argInfo = NULL;
  argInfo = &a[0].info;

  return argInfo;
}

static unsigned helpGetCTypeAlignment(const clang::QualType& qType) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  unsigned alignInBits = clangInfo->Ctx->getTypeAlignIfKnown(qType);

  unsigned alignInBytes = alignInBits / 8;
  // round it up to a power of 2
  unsigned rounded = 1;
  while (rounded < alignInBytes) rounded *= 2;

  return rounded;
}

static unsigned helpGetCTypeAlignment(const clang::TypeDecl* td) {
  QualType qType;

  if (const TypedefNameDecl* tnd = dyn_cast<TypedefNameDecl>(td)) {
    qType = tnd->getCanonicalDecl()->getUnderlyingType();
  } else if (const EnumDecl* ed = dyn_cast<EnumDecl>(td)) {
    qType = ed->getCanonicalDecl()->getIntegerType();
  } else if (const RecordDecl* rd = dyn_cast<RecordDecl>(td)) {
    RecordDecl *def = rd->getDefinition();
    INT_ASSERT(def);
    qType=def->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
  } else {
    INT_FATAL("Unknown clang type declaration");
  }

  return helpGetCTypeAlignment(qType);
}
static unsigned helpGetAlignment(::Type* type) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);

  if (type->symbol->hasFlag(FLAG_EXTERN)) {
    clang::TypeDecl* cType = NULL;
    clang::ValueDecl* cVal = NULL;
    info->lvt->getCDecl(type->symbol->cname, &cType, &cVal);
    if (cType) {
      return helpGetCTypeAlignment(cType);
    }
  }

  // use the maximum alignment of all the fields
  unsigned maxAlign = 1;

  if (isRecord(type) || isUnion(type)) {
    AggregateType* at = toAggregateType(type);
    for_fields(field, at) {
      unsigned fieldAlign = helpGetAlignment(field->type);
      if (maxAlign < fieldAlign)
        maxAlign = fieldAlign;
    }
  }

  return maxAlign;
}

bool isCTypeUnion(const char* name) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  TypeDecl* d = nullptr;

  info->lvt->getCDecl(name, &d, nullptr);

  if (d == nullptr)
    return false;

  if (isa<TypedefDecl>(d)) {
    TypedefDecl* td = cast<TypedefDecl>(d);
    const clang::Type* t = td->getUnderlyingType().getTypePtr();
    return t->isUnionType();
  }
  if (isa<RecordDecl>(d)) {
    RecordDecl* rec = cast<RecordDecl>(d);
    return rec->isUnion();
  }
  return false;
}

#if HAVE_LLVM_VER >= 100
llvm::MaybeAlign getPointerAlign(int addrSpace) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  uint64_t align = clangInfo->Clang->getTarget().getPointerAlign(0);
  return llvm::MaybeAlign(align);
}
llvm::MaybeAlign getCTypeAlignment(const clang::TypeDecl* td) {
  unsigned rounded = helpGetCTypeAlignment(td);
  if (rounded > 1) {
    return llvm::MaybeAlign(rounded);
  } else {
    return llvm::MaybeAlign();
  }
}
llvm::MaybeAlign getCTypeAlignment(const clang::QualType& qt) {
  unsigned rounded = helpGetCTypeAlignment(qt);
  if (rounded > 1) {
    return llvm::MaybeAlign(rounded);
  } else {
    return llvm::MaybeAlign();
  }
}
llvm::MaybeAlign getAlignment(::Type* type) {
  unsigned rounded = helpGetAlignment(type);
  if (rounded > 1) {
    return llvm::MaybeAlign(rounded);
  } else {
    return llvm::MaybeAlign();
  }
}

#else
uint64_t getPointerAlign(int addrSpace) {
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  uint64_t align = clangInfo->Clang->getTarget().getPointerAlign(0);
  return align;
}
unsigned getCTypeAlignment(const clang::TypeDecl* td) {
  return helpGetCTypeAlignment(td);
}
unsigned getCTypeAlignment(const clang::QualType& qt) {
  return helpGetCTypeAlignment(qt);
}
unsigned getAlignment(::Type* type) {
  return helpGetAlignment(type);
}
#endif


bool isBuiltinExternCFunction(const char* cname)
{
  if( 0 == strcmp(cname, "sizeof") ) return true;
  else return false;
}

static
void addAggregateGlobalOps(const PassManagerBuilder &Builder,
    llvm::legacy::PassManagerBase &PM) {
  GenInfo* info = gGenInfo;
  if( fLLVMWideOpt ) {
    PM.add(createAggregateGlobalOpsOptPass(info->globalToWideInfo.globalSpace));
  }
}

static
void addGlobalToWide(const PassManagerBuilder &Builder,
    llvm::legacy::PassManagerBase &PM) {
  GenInfo* info = gGenInfo;
  if( fLLVMWideOpt ) {
    PM.add(createGlobalToWide(&info->globalToWideInfo, info->clangInfo->asmTargetLayoutStr));
  }
}

static
bool getIrDumpExtensionPoint(llvmStageNum_t s,
    PassManagerBuilder::ExtensionPointTy & dumpIrPoint)
{
  switch (s) {
    case llvmStageNum::EarlyAsPossible:
      dumpIrPoint = PassManagerBuilder::EP_EarlyAsPossible;
      return true;
    case llvmStageNum::ModuleOptimizerEarly:
      dumpIrPoint = PassManagerBuilder::EP_ModuleOptimizerEarly;
      return true;
    case llvmStageNum::LoopOptimizerEnd:
      dumpIrPoint = PassManagerBuilder::EP_LoopOptimizerEnd;
      return true;
    case llvmStageNum::ScalarOptimizerLate:
      dumpIrPoint = PassManagerBuilder::EP_ScalarOptimizerLate;
      return true;
    case llvmStageNum::OptimizerLast:
      dumpIrPoint = PassManagerBuilder::EP_OptimizerLast;
      return true;
    case llvmStageNum::VectorizerStart:
      dumpIrPoint = PassManagerBuilder::EP_VectorizerStart;
      return true;
    case llvmStageNum::EnabledOnOptLevel0:
      dumpIrPoint = PassManagerBuilder::EP_EnabledOnOptLevel0;
      return true;
    case llvmStageNum::Peephole:
      dumpIrPoint = PassManagerBuilder::EP_Peephole;
      return true;
    case llvmStageNum::NOPRINT:
    case llvmStageNum::NONE:
    case llvmStageNum::BASIC:
    case llvmStageNum::FULL:
    case llvmStageNum::EVERY:
    case llvmStageNum::ASM:
    case llvmStageNum::LAST:
      return false;
  }

  return false;
}

static
void addDumpIrPass(const PassManagerBuilder &Builder,
    llvm::legacy::PassManagerBase &PM) {
  PM.add(createDumpIrPass(llvmPrintIrStageNum));
}

static void linkBitCodeFile(const char *bitCodeFilePath) {
  GenInfo* info = gGenInfo;

  // load into new module
  llvm::SMDiagnostic err;
  auto bcLib = llvm::parseIRFile(bitCodeFilePath, err,
                                 info->llvmContext);

  // adjust it
  const llvm::Triple &Triple = info->clangInfo->Clang->getTarget().getTriple();
  bcLib->setTargetTriple(Triple.getTriple());
  bcLib->setDataLayout(info->clangInfo->asmTargetLayoutStr);

  // link
  llvm::Linker::linkModules(*info->module, std::move(bcLib),
                            llvm::Linker::Flags::LinkOnlyNeeded);
}

static std::string determineOclcVersionLib(std::string libPath) {
  std::string result;

  // Extract version number from CHPL_GPU_ARCH string (e.g. extract
  // the 908 from "gfx908")
  std::regex pattern("gfx(\\d+)");
  std::cmatch match;
  if (std::regex_search(CHPL_GPU_ARCH, match, pattern)) {
    result = libPath + "/oclc_isa_version_" + std::string(match[1]) + ".bc";
  } else {
    USR_FATAL("Unable to determine oclc version from CHPL_GPU_ARCH");
  }

  // Ensure file exists (and can be opened)
  std::ifstream file(result);
  if(!file.good()) {
    USR_FATAL("Unable to find or open ROCM device library file %s", result.c_str());
  }

  return result;
}

// See the directions in https://llvm.org/docs/NVPTXUsage.html#libdevice
static void linkGpuDeviceLibraries() {
  GenInfo* info = gGenInfo;

  // save external functions
  std::set<std::string> externals;
  for (auto it = info->module->begin() ; it!= info->module->end() ; ++it) {
    if (it->hasExternalLinkage()) {
      externals.insert(it->getGlobalIdentifier());
    }
  }

  if (getGpuCodegenType() == GpuCodegenType::GPU_CG_NVIDIA_CUDA) {
    linkBitCodeFile(CHPL_CUDA_LIBDEVICE_PATH);
  } else {
    // See <https://github.com/RadeonOpenCompute/ROCm-Device-Libs> for details
    // on what these various libraries are.
    auto libPath = CHPL_ROCM_PATH + std::string("/amdgcn/bitcode");
    linkBitCodeFile((libPath + "/hip.bc").c_str());
    linkBitCodeFile((libPath + "/ocml.bc").c_str());
    linkBitCodeFile((libPath + "/ockl.bc").c_str());
    linkBitCodeFile((libPath + "/oclc_daz_opt_off.bc").c_str());
    linkBitCodeFile((libPath + "/oclc_unsafe_math_off.bc").c_str());
    linkBitCodeFile((libPath + "/oclc_finite_only_off.bc").c_str());
    linkBitCodeFile((libPath + "/oclc_correctly_rounded_sqrt_on.bc").c_str());
    linkBitCodeFile((libPath + "/oclc_wavefrontsize64_on.bc").c_str());
    linkBitCodeFile(determineOclcVersionLib(libPath).c_str());
  }

  // internalize all functions that are not in `externals`
  llvm::InternalizePass iPass([&externals](const llvm::GlobalValue& gv) {
    return externals.count(gv.getGlobalIdentifier()) > 0;
  });
  iPass.internalizeModule(*info->module);
}

// If we're using the LLVM wide optimizations, we have to add
// some functions to call put/get into the Chapel runtime layers
// (the optimization is meant to be portable to other languages)
static
void setupForGlobalToWide(void) {
  if( ! fLLVMWideOpt ) return;

  GenInfo* ginfo = gGenInfo;
  GlobalToWideInfo* info = &ginfo->globalToWideInfo;

  info->localeIdType = ginfo->lvt->getType("chpl_localeID_t");
  assert(info->localeIdType);
  info->nodeIdType = ginfo->lvt->getType("c_nodeid_t");
  assert(info->nodeIdType);

  llvm::Function* getFn = getFunctionLLVM("chpl_gen_comm_get_ctl");
  INT_ASSERT(getFn);
  info->getFn = getFn;
  info->getFnType = getFn->getFunctionType();

  llvm::Function* putFn = getFunctionLLVM("chpl_gen_comm_put_ctl");
  INT_ASSERT(putFn);
  info->putFn = putFn;
  info->putFnType = putFn->getFunctionType();

  llvm::Function* getPutFn = getFunctionLLVM("chpl_gen_comm_getput");
  INT_ASSERT(getPutFn);
  info->getPutFn = getPutFn;
  info->getPutFnType = getPutFn->getFunctionType();

  llvm::Function* memsetFn = getFunctionLLVM("chpl_gen_comm_memset");
  INT_ASSERT(memsetFn);
  info->memsetFn = memsetFn;
  info->memsetFnType = memsetFn->getFunctionType();

  // Call these functions in a dummy externally visible
  // function which GlobalToWide should remove. We need to do that
  // in order to prevent the functions from being removed for
  // not having references when they are inline/internal linkage.
  // Our function here just returns a pointer to the i'th (i is the argument)
  // such function - and since it is marked externally visible, there
  // is no way that the compiler can completely remove the needed
  // runtime functions.
  const char* dummy = "chpl_wide_opt_dummy";
  if( getFunctionLLVM(dummy) ) INT_FATAL("dummy function already exists");

  llvm::Type* retType = llvm::Type::getInt8PtrTy(ginfo->module->getContext());
  llvm::Type* argType = llvm::Type::getInt64Ty(ginfo->module->getContext());
  llvm::Value* fval = ginfo->module->getOrInsertFunction(
                          dummy, retType, argType
#if HAVE_LLVM_VER < 90
                          );
#else
                          ).getCallee();
#endif
  llvm::Function* fn = llvm::dyn_cast<llvm::Function>(fval);

  // Mark the function as external so that it will not be removed
  fn->setLinkage(llvm::GlobalValue::ExternalLinkage);

  llvm::BasicBlock* block =
     llvm::BasicBlock::Create(ginfo->module->getContext(), "entry", fn);
  ginfo->irBuilder->SetInsertPoint(block);

  llvm::Value* fns[] = {info->getFn, info->putFn,
                        info->getPutFn, info->memsetFn, NULL};

  llvm::Value* ret = llvm::Constant::getNullValue(retType);
  llvm::Function::arg_iterator args = fn->arg_begin();
  llvm::Argument& llArg = *args;
  llvm::Value* arg = &llArg;
  ++args;

  for( int i = 0; fns[i]; i++ ) {
    llvm::Value* f = fns[i];
    llvm::Value* ptr = ginfo->irBuilder->CreatePointerCast(f, retType);
    llvm::Value* id = llvm::ConstantInt::get(argType, i);
    llvm::Value* eq = ginfo->irBuilder->CreateICmpEQ(arg, id);
    ret = ginfo->irBuilder->CreateSelect(eq, ptr, ret);
  }
  ginfo->irBuilder->CreateRet(ret);

  if (developer || fVerify) {
    llvm::verifyFunction(*fn, &errs());
  }

  info->hasPreservingFn = true;
  info->preservingFn = fn;
}
static
void adjustLayoutForGlobalToWide()
{
  if( ! fLLVMWideOpt ) return;

  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  if (clangInfo->parseOnly) return;

  std::string layout = clangInfo->asmTargetLayoutStr;

  INT_ASSERT(layout != "");

  char buf[200]; //needs to store up to 8 32-bit numbers in decimal

  // Add global pointer info to layout.
  snprintf(buf, sizeof(buf), "-p%u:%u:%u:%u-p%u:%u:%u:%u"
      /*"-ni:%u:%u"*/ /* non-integral pointers */,
      GLOBAL_PTR_SPACE,
      GLOBAL_PTR_SIZE, GLOBAL_PTR_ABI_ALIGN, GLOBAL_PTR_PREF_ALIGN,
      WIDE_PTR_SPACE, GLOBAL_PTR_SIZE, GLOBAL_PTR_ABI_ALIGN,
      GLOBAL_PTR_PREF_ALIGN /*, GLOBAL_PTR_SPACE, WIDE_PTR_SPACE*/);
  layout += buf;
  // Save the global address space we are using in info.
  info->globalToWideInfo.globalSpace = GLOBAL_PTR_SPACE;
  info->globalToWideInfo.wideSpace = WIDE_PTR_SPACE;
  info->globalToWideInfo.globalPtrBits = GLOBAL_PTR_SIZE;

  // Always set the module layout. This works around an apparent bug in
  // clang or LLVM (trivial/deitz/test_array_low.chpl would print out the
  // wrong answer  because some i64s were stored at the wrong alignment).
  info->module->setDataLayout(layout);

  checkAdjustedDataLayout();
}

void checkAdjustedDataLayout() {

  if( ! fLLVMWideOpt ) return;

  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  if (clangInfo->parseOnly) return;

  // Check that the data layout setting worked
  const llvm::DataLayout& dl = info->module->getDataLayout();
  llvm::Type* testTy = llvm::Type::getInt8PtrTy(info->module->getContext(),
                                                GLOBAL_PTR_SPACE);
  INT_ASSERT(dl.getTypeSizeInBits(testTy) == GLOBAL_PTR_SIZE);
}

static void handlePrintAsm(std::string dotOFile);
static void makeLLVMStaticLibrary(std::string moduleFilename,
                                  const char* tmpbinname,
                                  std::vector<std::string> dotOFiles);
static void makeLLVMDynamicLibrary(std::string useLinkCXX, std::string options,
                            std::string moduleFilename, const char* tmpbinname,
                            std::vector<std::string> dotOFiles,
                            std::vector<std::string> clangLDArgs);
static std::string buildLLVMLinkCommand(std::string useLinkCXX,
                                        std::string options,
                                        std::string moduleFilename,
                                        std::string maino,
                                        const char* tmpbinname,
                                        std::vector<std::string> dotOFiles,
                                        std::vector<std::string> clangLDArgs);
static void runLLVMLinking(std::string useLinkCXX, std::string options,
                           std::string moduleFilename, std::string maino,
                           const char* tmpbinname,
                           std::vector<std::string> dotOFiles,
                           std::vector<std::string> clangLDArgs);
static std::string getLibraryOutputPath();
static void moveGeneratedLibraryFile(const char* tmpbinname);
static void moveResultFromTmp(const char* resultName, const char* tmpbinname);

static llvm::CodeGenFileType getCodeGenFileType() {
  switch (getGpuCodegenType()) {
    case GpuCodegenType::GPU_CG_AMD_HIP:
      return llvm::CodeGenFileType::CGFT_ObjectFile;
    case GpuCodegenType::GPU_CG_NVIDIA_CUDA:
    default:
      return llvm::CodeGenFileType::CGFT_AssemblyFile;
  }
}

static void stripPtxDebugDirective(const std::string& artifactFilename) {
  std::string line;
  std::vector<std::string> lines;
  std::string prefix = ".target";
  std::string suffix = ", debug";
  {
    std::ifstream ptxFile(artifactFilename);
    while (std::getline(ptxFile, line)) {
      if (line.compare(0, prefix.size(), prefix) == 0 /* line.starts_with(".target") */ &&
          line.compare(line.size() - suffix.size(), suffix.size(), suffix) == 0 /* line.ends_with(", debug") */) {
        line.resize(line.size() - suffix.size());
      }
      lines.push_back(std::move(line));
    }
  }
  {
    std::ofstream ptxFile(artifactFilename);
    for (const auto& line : lines) {
      ptxFile << line << std::endl;
    }
  }

}

static void makeBinaryLLVMForCUDA(const std::string& artifactFilename,
                                  const std::string& ptxObjectFilename,
                                  const std::string& fatbinFilename) {
  if (myshell("which ptxas > /dev/null 2>&1", "Check to see if ptxas command can be found", true)) {
    USR_FATAL("Command 'ptxas' not found\n");
  }

  if (myshell("which fatbinary > /dev/null 2>&1", "Check to see if fatbinary command can be found", true)) {
    USR_FATAL("Command 'fatbinary' not found\n");
  }

  std::string ptxasFlags = "";

  if (fGpuPtxasEnforceOpt) {
    // When --gpu-ptxas-enforce-opt is set and --fast is used,
    // pass -O3 to ptxas even if -g is set. Clang's -g output
    // produces code with debugging directives incompatible
    // with -O3, so then strip those directives.

    ptxasFlags = fFastFlag ? "-O3" : "-O0";
    if (debugCCode) ptxasFlags += " -lineinfo";

    // Kind of a hack; manually turn
    //   .target sm_60, debug
    // into
    //   .target sm_60
    // because we can't configure clang to not force
    // full debug info.
    if (debugCCode && fFastFlag) {
      stripPtxDebugDirective(artifactFilename);
    }
  }

  // avoid warning about not statically knowing the stack size when recursive
  // functions are called from the kernel
  ptxasFlags += " --suppress-stack-size-warning ";
  std::string ptxCmd = std::string("ptxas -m64 --gpu-name ") + CHPL_GPU_ARCH +
                       " " + ptxasFlags + " " +
                       std::string(" --output-file ") +
                       ptxObjectFilename.c_str() +
                       " " + artifactFilename.c_str();

  mysystem(ptxCmd.c_str(), "PTX to  object file");

  if (strncmp(CHPL_GPU_ARCH, "sm_", 3) != 0 || strlen(CHPL_GPU_ARCH) != 5) {
    USR_FATAL("Unrecognized CUDA arch");
  }

  std::string computeCap = std::string("compute_") + CHPL_GPU_ARCH[3] +
                                                     CHPL_GPU_ARCH[4];
  std::string fatbinaryCmd = std::string("fatbinary -64 ") +
                             std::string("--create ") +
                             fatbinFilename.c_str() +
                             std::string(" --image=profile=") + CHPL_GPU_ARCH +
                             ",file=" + ptxObjectFilename.c_str() +
                             std::string(" --image=profile=") + computeCap +
                             ",file=" + artifactFilename.c_str();
  mysystem(fatbinaryCmd.c_str(), "object file to fatbinary");
}

static void makeBinaryLLVMForHIP(const std::string& artifactFilename,
                                 const std::string& outFilename,
                                 const std::string& fatbinFilename) {
  std::string lldCmd = std::string(gGpuSdkPath) +
                      "/llvm/bin/lld -flavor gnu" +
                       " --no-undefined -shared" +
                       " -plugin-opt=-amdgpu-internalize-symbols" +
                       " -plugin-opt=mcpu=gfx906 -plugin-opt=O3" +
                       " -plugin-opt=-amdgpu-early-inline-all=true" +
                       " -plugin-opt=-amdgpu-function-calls=false -o " +
                       outFilename + " " + artifactFilename;
  std::string bundlerCmd = std::string(gGpuSdkPath) +
                          "/llvm/bin/clang-offload-bundler" +
                           " -type=o -bundle-align=4096" +
                           " -targets=host-x86_64-unknown-linux," +
                           "hipv4-amdgcn-amd-amdhsa--" + CHPL_GPU_ARCH +
                           " -inputs=/dev/null," + outFilename +
                           " -outputs=" + fatbinFilename;

  mysystem(lldCmd.c_str(), "Device .o file to .out file");
  mysystem(bundlerCmd.c_str(), ".out file to fatbin file");
}

void makeBinaryLLVM(void) {

  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  std::string moduleFilename;
  std::string preOptFilename;
  std::string opt1Filename;
  std::string opt2Filename;
  std::string artifactFilename;
  std::string ptxObjectFilename;
  std::string outFilename;
  std::string fatbinFilename;

  if (gCodegenGPU == false) {
    moduleFilename = genIntermediateFilename("chpl__module.o");
    preOptFilename = genIntermediateFilename("chpl__module-nopt.bc");
    opt1Filename = genIntermediateFilename("chpl__module-opt1.bc");
    opt2Filename = genIntermediateFilename("chpl__module-opt2.bc");
  } else {
    moduleFilename = genIntermediateFilename("chpl__gpu_module.o");
    preOptFilename = genIntermediateFilename("chpl__gpu_module-nopt.bc");
    opt1Filename = genIntermediateFilename("chpl__gpu_module-opt1.bc");
    opt2Filename = genIntermediateFilename("chpl__gpu_module-opt2.bc");
    ptxObjectFilename = genIntermediateFilename("chpl__gpu_ptx.o");
    fatbinFilename = genIntermediateFilename("chpl__gpu.fatbin");
    outFilename = genIntermediateFilename("chpl__gpu.out");

    // In CUDA, we generate assembly and then assemble it. For
    // AMD, we generate an object file. Thus, we need to use
    // different file names.
    switch (getGpuCodegenType()) {
      case GpuCodegenType::GPU_CG_NVIDIA_CUDA:
        artifactFilename = genIntermediateFilename("chpl__gpu_ptx.s");
        break;
      case GpuCodegenType::GPU_CG_AMD_HIP:
        artifactFilename = genIntermediateFilename("chpl__gpu.o");
        break;
    }
  }

  if( saveCDir[0] != '\0' ) {
    std::error_code tmpErr;
    // Save the generated LLVM before optimization.
#if HAVE_LLVM_VER >= 120
    ToolOutputFile output (preOptFilename.c_str(), tmpErr, sys::fs::OF_None);
#else
    ToolOutputFile output (preOptFilename.c_str(), tmpErr, sys::fs::F_None);
#endif
    if (tmpErr)
      USR_FATAL("Could not open output file %s", preOptFilename.c_str());
#if HAVE_LLVM_VER < 70
    WriteBitcodeToFile(info->module, output.os());
#else
    WriteBitcodeToFile(*info->module, output.os());
#endif
    output.keep();
    output.os().flush();
  }

  // Handle --llvm-print-ir-stage=basic
#ifdef HAVE_LLVM
  if((llvmStageNum::BASIC == llvmPrintIrStageNum ||
      llvmStageNum::EVERY == llvmPrintIrStageNum)) {

    for (auto &F : info->module->functions()) {
      std::string str = F.getName().str();
      if (shouldLlvmPrintIrCName(str.c_str()))
        printLlvmIr(str.c_str(), &F, llvmStageNum::BASIC);
    }

    completePrintLlvmIrStage(llvmStageNum::BASIC);
  }
#endif


  // Open the output file
  std::error_code error;
#if HAVE_LLVM_VER >= 120
  llvm::sys::fs::OpenFlags flags = llvm::sys::fs::OF_None;
#else
  llvm::sys::fs::OpenFlags flags = llvm::sys::fs::F_None;
#endif

  static bool addedGlobalExts = false;
  if( ! addedGlobalExts ) {
    // Add IR dumping pass if necessary
    // point is initialized to a dummy value; it is set
    // in getIrDumpExtensionPoint.
    PassManagerBuilder::ExtensionPointTy point =
                  PassManagerBuilder::EP_EarlyAsPossible;

    if (getIrDumpExtensionPoint(llvmPrintIrStageNum, point)) {
      printf("Adding IR dump extension at %i\n", point);
      PassManagerBuilder::addGlobalExtension(point, addDumpIrPass);
    }

    if (llvmPrintIrStageNum == llvmStageNum::EVERY) {
      printf("; Adding IR dump extensions for all phases\n");
      for (int i = 0; i < llvmStageNum::LAST; i++) {
        llvmStageNum::llvmStageNum_t stage = (llvmStageNum::llvmStageNum_t) i;
        if (getIrDumpExtensionPoint(stage, point))
          PassManagerBuilder::addGlobalExtension(
              point,
              [stage] (const PassManagerBuilder &Builder,
                       llvm::legacy::PassManagerBase &PM) -> void {
                PM.add(createDumpIrPass(stage));
              });
      }

      // Put the print-stage-num back
      llvmPrintIrStageNum = llvmStageNum::EVERY;
    }

    addedGlobalExts = true;
  }

  // Create PassManager and run optimizations
  PassManagerBuilder PMBuilder;

  configurePMBuilder(PMBuilder, /* for function passes */ false);

  // Note, these global extensions currently only apply
  // to the module-level optimization (not the "basic" function
  // optimization we do immediately after generating LLVM IR).

  // Add the Global to Wide optimization if necessary.
  if (fLLVMWideOpt) {
    PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast, addAggregateGlobalOps);
    PMBuilder.addExtension(PassManagerBuilder::EP_OptimizerLast, addGlobalToWide);
    PMBuilder.addExtension(PassManagerBuilder::EP_EnabledOnOptLevel0, addGlobalToWide);
  }

  // Setup for and run LLVM optimization passes
  {
    adjustLayoutForGlobalToWide();

    llvm::legacy::PassManager mpm;
    llvm::legacy::PassManager mpm2;

    // Add the TransformInfo pass
    mpm.add(createTargetTransformInfoWrapperPass(
            info->targetMachine->getTargetIRAnalysis()));
    mpm2.add(createTargetTransformInfoWrapperPass(
             info->targetMachine->getTargetIRAnalysis()));

    // Add the TargetLibraryInfo pass
    Triple TargetTriple(info->module->getTargetTriple());
    llvm::TargetLibraryInfoImpl TLII(TargetTriple);
    mpm.add(new TargetLibraryInfoWrapperPass(TLII));
    mpm2.add(new TargetLibraryInfoWrapperPass(TLII));

    PMBuilder.populateModulePassManager(mpm);

    // Run the optimizations now!
    mpm.run(*info->module);

    if( saveCDir[0] != '\0' ) {
      // Save the generated LLVM after first chunk of optimization
      std::error_code tmpErr;
#if HAVE_LLVM_VER >= 120
      ToolOutputFile output1 (opt1Filename.c_str(),
                               tmpErr, sys::fs::OF_None);
#else
      ToolOutputFile output1 (opt1Filename.c_str(),
                               tmpErr, sys::fs::F_None);
#endif

      if (tmpErr)
        USR_FATAL("Could not open output file %s", opt1Filename.c_str());
#if HAVE_LLVM_VER < 70
      WriteBitcodeToFile(info->module, output1.os());
#else
      WriteBitcodeToFile(*info->module, output1.os());
#endif
      output1.keep();
      output1.os().flush();
    }


    if (fLLVMWideOpt) {
      // the GlobalToWide pass creates calls to inline functions, among
      // other things, that will need to be optimized. So run an additional
      // battery of optimizations now.

      PassManagerBuilder PMBuilder2;

      configurePMBuilder(PMBuilder2, false, /* opt level */ 1);
      // Should we disable vectorization since we did that?
      // Or run select few cleanup passes?
      // Inlining is definitely important here..

      PMBuilder2.populateModulePassManager(mpm2);

      // Reset the data layout.
      info->module->setDataLayout(clangInfo->asmTargetLayoutStr);

      // Run the optimizations now!
      mpm2.run(*info->module);

      if( saveCDir[0] != '\0' ) {
        // Save the generated LLVM after second chunk of optimization
        std::error_code tmpErr;
#if HAVE_LLVM_VER >= 120
        ToolOutputFile output2 (opt2Filename.c_str(),
                                 tmpErr, sys::fs::OF_None);
#else
        ToolOutputFile output2 (opt2Filename.c_str(),
                                 tmpErr, sys::fs::F_None);
#endif
        if (tmpErr)
          USR_FATAL("Could not open output file %s", opt2Filename.c_str());
#if HAVE_LLVM_VER < 70
        WriteBitcodeToFile(info->module, output2.os());
#else
        WriteBitcodeToFile(*info->module, output2.os());
#endif
        output2.keep();
        output2.os().flush();
      }
    }
  }

  // Handle --llvm-print-ir-stage=full
#ifdef HAVE_LLVM
  if((llvmStageNum::FULL == llvmPrintIrStageNum ||
      llvmStageNum::EVERY == llvmPrintIrStageNum)) {

    for (auto &F : info->module->functions()) {
      std::string str = F.getName().str();
      if (shouldLlvmPrintIrCName(str.c_str()))
        printLlvmIr(str.c_str(), &F, llvmStageNum::FULL);
    }

    completePrintLlvmIrStage(llvmStageNum::FULL);
  }
#endif

  // Make sure that we are generating PIC when we need to be.
  if (strcmp(CHPL_LIB_PIC, "pic") == 0) {
    INT_ASSERT(info->targetMachine->getRelocationModel()
        == llvm::Reloc::Model::PIC_);
  }

  // Emit the .o file for linking with clang
  // Setup and run LLVM passes to emit a .o file to outputOfile
  {

    bool disableVerify = !developer;

    if (gCodegenGPU == false) {
      llvm::raw_fd_ostream outputOfile(moduleFilename, error, flags);
      if (error || outputOfile.has_error())
        USR_FATAL("Could not open output file %s", moduleFilename.c_str());

#if HAVE_LLVM_VER >= 100
      llvm::CodeGenFileType FileType = llvm::CGFT_ObjectFile;
#else
      llvm::TargetMachine::CodeGenFileType FileType =
        llvm::TargetMachine::CGFT_ObjectFile;
#endif

      {
        llvm::legacy::PassManager emitPM;

        emitPM.add(createTargetTransformInfoWrapperPass(
                   info->targetMachine->getTargetIRAnalysis()));

#if HAVE_LLVM_VER > 60
        info->targetMachine->addPassesToEmitFile(emitPM, outputOfile,
                                                 nullptr,
                                                 FileType,
                                                 disableVerify);
#else
        info->targetMachine->addPassesToEmitFile(emitPM, outputOfile,
                                                 FileType,
                                                 disableVerify);
#endif

        emitPM.run(*info->module);

      }
      outputOfile.close();

      handlePrintAsm(moduleFilename);

    } else {

      auto artifactFileType = getCodeGenFileType();

      linkGpuDeviceLibraries();

      llvm::raw_fd_ostream outputArtifactFile(artifactFilename, error, flags);

      {

        llvm::legacy::PassManager emitPM;

        emitPM.add(createTargetTransformInfoWrapperPass(
                   info->targetMachine->getTargetIRAnalysis()));

        info->targetMachine->addPassesToEmitFile(emitPM, outputArtifactFile,
                                                 nullptr,
                                                 artifactFileType,
                                                 disableVerify);

        emitPM.run(*info->module);

      }

      outputArtifactFile.close();
      switch (getGpuCodegenType()) {
        case GpuCodegenType::GPU_CG_NVIDIA_CUDA:
          makeBinaryLLVMForCUDA(artifactFilename, ptxObjectFilename, fatbinFilename);
          break;
        case GpuCodegenType::GPU_CG_AMD_HIP:
          makeBinaryLLVMForHIP(artifactFilename, outFilename, fatbinFilename);
          break;
      }
    }
  }

  //finishClang is before the call to the debug finalize
  deleteClang(clangInfo);

  // Just make the .o file for GPU code
  if (gCodegenGPU) {
    return;
  }

  std::string options = "";

  std::string maino(CHPL_RUNTIME_LIB);
  maino += "/";
  maino += CHPL_RUNTIME_SUBDIR;
  maino += "/main.o";

  std::string clangCC = clangInfo->clangCC;
  std::string clangCXX = clangInfo->clangCXX;
  std::string useLinkCXX = clangCXX;

  if (CHPL_TARGET_LD != nullptr && CHPL_TARGET_LD[0] != '\0')
    useLinkCXX = CHPL_TARGET_LD;

  // start with arguments from CHPL_LLVM_CLANG_C unless
  // using a non-clang compiler to link
  std::vector<std::string> clangLDArgs = clangInfo->clangLDArgs;
  if (useLinkCXX != clangCXX)
    clangLDArgs.clear();

  // Add runtime libs arguments
  //readArgsFromFile(runtime_libs, clangLDArgs);

  // add the bundled link args from printchplenv
  splitStringWhitespace(CHPL_TARGET_BUNDLED_LINK_ARGS, clangLDArgs);

  // add the system link args from printchplenv
  splitStringWhitespace(CHPL_TARGET_SYSTEM_LINK_ARGS, clangLDArgs);

  // Grab extra dependencies for multilocale libraries if needed.
  if (fMultiLocaleInterop) {
    std::string cmd = std::string(CHPL_HOME);
    cmd += "/util/config/compileline --multilocale-lib-deps";
    std::string libs = runCommand(cmd);
    // Erase trailing newline.
    libs.erase(libs.size() - 1);
    clangLDArgs.push_back(libs);
  }

  // Substitute $CHPL_HOME $CHPL_RUNTIME_LIB etc
  expandInstallationPaths(clangLDArgs);


  std::vector<std::string> dotOFiles;

  // Gather C flags for compiling C files.
  std::string cargs;
  for( size_t i = 0; i < clangInfo->clangCCArgs.size(); ++i ) {
    cargs += " ";
    cargs += clangInfo->clangCCArgs[i];
  }

  std::string gpuArgs = "";
  if (usingGpuLocaleModel()) {
    gpuArgs = generateClangGpuLangArgs() + " -Wno-unknown-cuda-version";
  }

  int filenum = 0;
  while (const char* inputFilename = nthFilename(filenum++)) {
    if (isCSource(inputFilename)) {
      const char* objFilename = objectFileForCFile(inputFilename);
      std::string cmd = clangCC + " " + gpuArgs + " -c -o " + objFilename + " " +
                        inputFilename + " " + cargs;

      mysystem(cmd.c_str(), "Compile C File");
      dotOFiles.push_back(objFilename);
    } else if( isObjFile(inputFilename) ) {
      dotOFiles.push_back(inputFilename);
    }
  }

  // Note: we used to start 'options' with 'cargs' so that
  // we'd communicate -O3 -march=native e.g. to the "linker".
  // That was only important when we were emitting a .bc file
  // and currently we emit a .o.
  // If we decide to put it back, we might also need to
  // pass -Qunused-arguments or -Wno-error=unused-command-line-argument
  // to avoid unused argument errors for optimization flags.

  if(debugCCode) {
    options += " -g";
  }

  // We used to supply link args here *and* later on
  // in the link line. I think the later position is sufficient.

  // note: currently ldflags are not stored into clangLDArgs.
  // If they were, these lines would need to be removed.
  options += " ";
  options += ldflags;

  // We may need to add the -pthread flag here for the link step
  // if we start doing link-time optimization.  For now, leave it
  // out because its unnecessary inclusion causes a warning message
  // on Macs.

  // Now, if we're doing a multilocale build, we have to make a launcher.
  // For this reason, we create a makefile. codegen_makefile
  // also gives us the name of the temporary place to save
  // the generated program.
  fileinfo mainfile;
  mainfile.filename = "chpl__module.o";
  mainfile.pathname = moduleFilename.c_str();
  const char* tmpbinname = NULL;
  const char* tmpservername = NULL;

  if (fMultiLocaleInterop) {
    codegen_makefile(&mainfile, &tmpbinname, &tmpservername, true);
    INT_ASSERT(tmpservername);
    INT_ASSERT(tmpbinname);
  } else {
    codegen_makefile(&mainfile, &tmpbinname, NULL, true);
    INT_ASSERT(tmpbinname);
  }

  if (fLibraryCompile && !fMultiLocaleInterop) {
    switch (fLinkStyle) {
    // The default library link style for Chapel is _static_.
    case LS_DEFAULT:
    case LS_STATIC:
      makeLLVMStaticLibrary(moduleFilename, tmpbinname, dotOFiles);
      break;
    case LS_DYNAMIC:
      makeLLVMDynamicLibrary(useLinkCXX, options, moduleFilename, tmpbinname,
                             dotOFiles, clangLDArgs);
      break;
    default:
      INT_FATAL("Unsupported library link mode");
      break;
    }
  } else {
    const char* outbin = fMultiLocaleInterop ? tmpservername : tmpbinname;

    // Runs the LLVM link command for executables.
    runLLVMLinking(useLinkCXX, options, moduleFilename, maino, outbin,
                   dotOFiles, clangLDArgs);
  }

  // If we're not using a launcher, copy the program here
  if (0 == strcmp(CHPL_LAUNCHER, "none")) {

    if (fLibraryCompile) {
      moveGeneratedLibraryFile(tmpbinname);
    } else {
      moveResultFromTmp(executableFilename, tmpbinname);
    }

  } else {
    // Now run the makefile to move from tmpbinname to the proper program
    // name and to build a launcher (if necessary).
    const char* makeflags = printSystemCommands ? "-f " : "-s -f ";
    const char* makecmd = astr(astr(CHPL_MAKE, " "),
                               makeflags,
                               getIntermediateDirName(), "/Makefile");

    mysystem(makecmd, "Make Binary - Building Launcher and Copying");
  }
}

static void handlePrintAsm(std::string dotOFile) {
  if (llvmPrintIrStageNum == llvmStageNum::ASM ||
      llvmPrintIrStageNum == llvmStageNum::EVERY) {

    // Find llvm-objdump as a sibling to clang
    // note that if we have /path/to/clang-14, this logic
    // will loop for /path/to/llvm-objdump-14.
    //
    // If such suffixes do not turn out to matter in practice, it would
    // be nice to update this code to use sys::path::parent_path().
    std::vector<std::string> split;
    std::string llvmObjDump = "llvm-objdump";

    splitStringWhitespace(CHPL_LLVM_CLANG_C, split);
    if (split.size() > 0) {
      std::string tmp = split[0];
      const char* clang = "clang";
      auto pos = tmp.find(clang);
      if (pos != std::string::npos) {
        tmp.replace(pos, strlen(clang), "llvm-objdump");
        if (pathExists(tmp.c_str())) {
          llvmObjDump = tmp;
        }
      }
    }

    // Note: if we want to support GNU objdump, just need to use
    // --disassemble= instead of the LLVM flag --disassemble-symbols=
    // but this does not work with older GNU objdump versions.
    std::string disSymArg = "--disassemble-symbols=";

    static bool isDarwin = strcmp(CHPL_TARGET_PLATFORM, "darwin") == 0;
    if (isDarwin) {
      // The symbols in a Mach-O file has a leading _
      disSymArg += "_";
    }

    std::vector<std::string> names = gatherPrintLlvmIrCNames();
    for (const auto& name : names) {
      printf("\n\n# Disassembling symbol %s\n\n", name.c_str());
      fflush(stdout);
      std::vector<std::string> cmd;
      cmd.push_back(llvmObjDump);
      std::string arg = disSymArg; // e.g. --disassemble=
      arg += name;
      cmd.push_back(arg);
      cmd.push_back(dotOFile);

      mysystem(cmd, "disassemble a symbol",
               /* ignoreStatus */ true,
               /* quiet */ false);
    }
  }
}

static void makeLLVMStaticLibrary(std::string moduleFilename,
                                  const char* tmpbinname,
                                  std::vector<std::string> dotOFiles) {

  INT_ASSERT(fLibraryCompile);
  INT_ASSERT(fLinkStyle == LS_STATIC || fLinkStyle == LS_DEFAULT);

  std::string commandBase = "ar -c -r -s"; // Stolen from Makefile.static
  std::string command = commandBase + " " + tmpbinname + " " +  moduleFilename;

  for (size_t i = 0; i < dotOFiles.size(); i++) {
    command += " ";
    command += dotOFiles[i];
  }

  mysystem(command.c_str(), "Make Static Library - Linking");
}

static void makeLLVMDynamicLibrary(std::string useLinkCXX,
                                   std::string options,
                                   std::string moduleFilename,
                                   const char* tmpbinname,
                                   std::vector<std::string> dotOFiles,
                                   std::vector<std::string> clangLDArgs) {

  INT_ASSERT(fLibraryCompile && fLinkStyle == LS_DYNAMIC);

  // This is a clang++ flag, make the linker shut up about a missing "main".
  clangLDArgs.push_back("-shared");

#if defined(__APPLE__) && defined(__MACH__)
  {
    // TODO:
    // Right now, we check for __APPLE__ before adding Mac specific linker
    // args. What we would like to do is additionally detect what linker we
    // are using (not all linkers may support "-install_name", for example).
    //
    // Apple's default LD will attempt to load a dynamic library via the path
    // of the temporary copy (which was removed) unless we tell it to use the
    // final output path instead.
    std::string installName = "-Wl,-install_name," + getLibraryOutputPath();
    clangLDArgs.push_back(installName);
  }
#endif

  // No main object file for this call, since we're building a library.
  std::string command = buildLLVMLinkCommand(useLinkCXX, options,
                                             moduleFilename, "", tmpbinname,
                                             dotOFiles, clangLDArgs);

  mysystem(command.c_str(), "Make Dynamic Library - Linking");
}

static void moveResultFromTmp(const char* resultName, const char* tmpbinname) {
  std::error_code err;

  // rm -f hello
  if( printSystemCommands )
    printf("rm -f %s\n", resultName);

  err = llvm::sys::fs::remove(resultName);
  if (err) {
    USR_FATAL("removing file %s failed: %s\n",
              resultName,
              err.message().c_str());
  }
  // mv tmp/hello.tmp hello
  if( printSystemCommands )
    printf("mv %s %s\n", tmpbinname, resultName);

  err = llvm::sys::fs::rename(tmpbinname, resultName);
  if (err) {
    // But that might fail if /tmp is on a different filesystem.

    std::string mv("mv ");
    mv += tmpbinname;
    mv += " ";
    mv += resultName;

    mysystem(mv.c_str(), mv.c_str());

    /* For future LLVM,
       err = llvm::sys::fs::copy_file(tmpbinname, resultName);
       if (err) {
         USR_FATAL("copying file %s to %s failed: %s\n",
                   tmpbinname,
                   resultName,
                   err.message().c_str());
       }

       // and then set permissions, like mv
       auto maybePerms = llvm::sys::fs::getPermissions(tmpbinname);
       if (maybePerms.getError()) {
         USR_FATAL("reading permissions on %s failed: %s\n",
                   tmpbinname,
                   err.message().c_str());
       }
       err = llvm::sys::fs::setPermissions(resultName, *maybePerms);
       if (err) {
         USR_FATAL("setting permissions on %s failed: %s\n",
                   resultName,
                   err.message().c_str());
       }

       // and then remove the file, so it's like mv
       err = llvm::sys::fs::remove(tmpbinname);
       if (err) {
         USR_FATAL("removing file %s failed: %s\n",
                   tmpbinname,
                   err.message().c_str());
       }*/
  }
}

static std::string buildLLVMLinkCommand(std::string useLinkCXX,
                                        std::string options,
                                        std::string moduleFilename,
                                        std::string maino,
                                        const char* tmpbinname,
                                        std::vector<std::string> dotOFiles,
                                        std::vector<std::string> clangLDArgs) {
  // Run the linker. We always use a C++ compiler because some third-party
  // libraries are written in C++. Here we use clang++ or possibly a
  // linker override specified by the Makefiles (e.g. setting it to mpicxx)
  std::string command = useLinkCXX + " " + options + " " +
                        moduleFilename + " " + maino;

  // For dynamic linking, leave it alone.  For static, append -static.
  // See $CHPL_HOME/make/compiler/Makefile.clang (and keep this in sync
  // with it).
  // Note that in multi-locale interop we are building a server executable
  // that cannot be built with `-static`, because because it depends on
  // dynamic libraries. So even if the client library is being built as
  // static, the server cannot be.
  if (fLinkStyle == LS_STATIC && !fMultiLocaleInterop) {
    command += " -static";
  }

  command += " -o ";
  command += tmpbinname;

  for (size_t i = 0; i < dotOFiles.size(); i++) {
    command += " ";
    command += dotOFiles[i];
  }

  for (size_t i = 0; i < clangLDArgs.size(); ++i) {
    command += " ";
    command += clangLDArgs[i];
  }

  // Put user-requested libraries at the end of the compile line,
  // they should at least be after the .o files and should be in
  // order where libraries depend on libraries to their right.
  for_vector(const char, dirName, libDirs) {
    command += " -L";
    command += dirName;
  }

  for_vector(const char, libName, libFiles) {
    command += " -l";
    command += libName;
  }

  return command;
}

static void runLLVMLinking(std::string useLinkCXX, std::string options,
                           std::string moduleFilename, std::string maino,
                           const char* tmpbinname,
                           std::vector<std::string> dotOFiles,
                           std::vector<std::string> clangLDArgs) {

  // This code is general enough to use elsewhere, thus the move.
  std::string command = buildLLVMLinkCommand(useLinkCXX,
                                             options,
                                             moduleFilename,
                                             maino,
                                             tmpbinname,
                                             dotOFiles,
                                             clangLDArgs);

  mysystem(command.c_str(), "Make Binary - Linking");
}

static std::string getLibraryOutputPath() {
  // Need to reuse some of the stuff in codegen_makefile.  It doesn't save the
  // full filename that is used when in library mode, so we don't have an
  // alternative to making a modified version of executableFilename again
  std::string result;
  const char* exeExt = getLibraryExtension();
  const char* libraryPrefix = "";
  int libLength = strlen("lib");
  bool startsWithLib = strncmp(executableFilename, "lib", libLength) == 0;

  if (!startsWithLib) {
    libraryPrefix = "lib";
  }

  result += std::string(libDir) + "/" + libraryPrefix + executableFilename;
  result += std::string(exeExt);

  return result;
}

static void moveGeneratedLibraryFile(const char* tmpbinname) {
  std::string outputPath = getLibraryOutputPath();
  moveResultFromTmp(outputPath.c_str(), tmpbinname);
}

void print_clang(const clang::Type* t) {
  if (t == NULL)
    fprintf(stderr, "NULL");
  else
    t->dump();

  fprintf(stderr, "\n");
}

void print_clang(const clang::Decl* d) {
  if (d == NULL)
    fprintf(stderr, "NULL");
  else
    d->print(llvm::dbgs());

  fprintf(stderr, "\n");
}

void print_clang(const clang::TypeDecl* d) {
  if (d == NULL)
    fprintf(stderr, "NULL");
  else
    d->print(llvm::dbgs());

  fprintf(stderr, "\n");
}
void print_clang(const clang::ValueDecl* d) {
  if (d == NULL)
    fprintf(stderr, "NULL");
  else
    d->print(llvm::dbgs());

  fprintf(stderr, "\n");
}

#endif
