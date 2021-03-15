/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#ifdef HAVE_LLVM
#include "clang/AST/GlobalDecl.h"

// rely on CodeGenOptions.h being included from CompilerInstance.h
// if we need to change that, LLVM 6 named it
//   clang/Frontend/CodeGenOptions.h
// but LLVM 8 named it
//   clang/Basic/CodeGenOptions.h

#include "clang/Basic/Version.h"
#include "clang/Basic/TargetInfo.h"
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
#include "llvm/MC/SubtargetFeature.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

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

#include "../ifa/prim_data.h"

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
  std::vector<std::string> clangCCArgs;
  std::vector<std::string> clangOtherArgs;

  clang::CodeGenOptions codegenOptions;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagOptions;
  clang::TextDiagnosticPrinter* DiagClient;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagID;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> Diags;

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
    bool parseOnlyIn);
};

ClangInfo::ClangInfo(
    std::string clangCcIn,
    std::string clangCxxIn,
    std::vector<std::string> clangCCArgsIn,
    std::vector<std::string> clangOtherArgsIn,
    bool parseOnlyIn)
       : parseOnly(parseOnlyIn),
         clangCC(clangCcIn),
         clangCXX(clangCxxIn),
         clangCCArgs(clangCCArgsIn),
         clangOtherArgs(clangOtherArgsIn),
         codegenOptions(), diagOptions(nullptr),
         DiagClient(nullptr),
         DiagID(nullptr),
         Diags(nullptr),
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
  return nullptr;
}

static
void addMinMax(const char* prefix, int nbits, bool isSigned)
{
  GenInfo* info = gGenInfo;
  LayeredValueTable *lvt = info->lvt;

  astlocT prevloc = currentAstLoc;

  currentAstLoc.lineno = 0;
  currentAstLoc.filename = astr("<internal>");

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

  return nullptr;
}

static
void setupCIntType(::Type*& type, int nbits, bool unsigned_) {
  if (type != nullptr) {
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




static const bool debugPrintMacros = false;

static void handleMacroExpr(const MacroInfo* inMacro,
                            MacroInfo::tokens_iterator start,
                            MacroInfo::tokens_iterator end,
                            VarSymbol*& varRet,
                            TypeDecl*& cTypeRet,
                            ValueDecl*& cValueRet,
                            const char*& cCastToTypeRet);


// Adds a mapping from id->getName() to a variable or CDecl to info->lvt
static
void handleMacro(const IdentifierInfo* id, const MacroInfo* macro)
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  const bool debugPrint = debugPrintMacros;

  if (debugPrint) printf("Working on macro %s\n", id->getName().str().c_str());

  //Handling only simple string or integer defines
  if (macro->getNumParams() > 0)
  {
    if( debugPrint) {
      printf("the macro takes arguments\n");
    }
    return; // TODO -- handle macro functions.
  }

  VarSymbol* varRet = nullptr;
  TypeDecl* cTypeRet = nullptr;
  ValueDecl* cValueRet = nullptr;
  const char* cCastToTypeRet = nullptr;

  handleMacroExpr(macro,
                  macro->tokens_begin(), macro->tokens_end(),
                  varRet, cTypeRet, cValueRet, cCastToTypeRet);

  if( debugPrint ) {
    std::string s = std::string(id->getName());
    const char* kind = nullptr;
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
                                   MacroInfo::tokens_iterator &start,
                                   MacroInfo::tokens_iterator &end) {

  if (start == end)
    return;

  // Remove any number of outer parens e.g. (1), ((1)) -> 1
  int left_parens = 0;
  int right_parens = 0;
  for (MacroInfo::tokens_iterator cur = end - 1;
       cur != start;
       --cur) {
    if(cur->getKind() == tok::r_paren) right_parens++;
    else break;
  }

  for (MacroInfo::tokens_iterator cur = start;
       cur != end;
       ++cur) {
    if(cur->getKind() == tok::l_paren) left_parens++;
    else break;
  }

  int min_parens = (left_parens < right_parens) ? left_parens : right_parens;
  if (min_parens > 0) {
    MacroInfo::tokens_iterator oldStart = start;
    MacroInfo::tokens_iterator oldEnd = end;
    start = oldStart + min_parens;
    end = oldEnd - min_parens;
    INT_ASSERT(start != oldStart);
    INT_ASSERT(end != oldEnd);
  }
}

// finds a parenthesized expression at the start of start..end
// the expression does not necessarily cover the entire expression.
static bool findParenthesizedExpr(const MacroInfo* inMacro,
                                  MacroInfo::tokens_iterator start,
                                  MacroInfo::tokens_iterator end,
                                  MacroInfo::tokens_iterator &pStart,
                                  MacroInfo::tokens_iterator &pEnd) {
  int inparens = 0;
  for (MacroInfo::tokens_iterator cur = start; cur != end; ++cur) {

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


// Returns a type/identifier/macro name or nullptr if it was not handled
static const char* handleTypeOrIdentifierExpr(const MacroInfo* inMacro,
                                              MacroInfo::tokens_iterator start,
                                              MacroInfo::tokens_iterator end,
                                              IdentifierInfo*& ii) {
  ii = nullptr;

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

    // Give up if there are any tokens beyond the main token
    if (start != end)
      return nullptr;

    if (tok.getKind() == tok::identifier) {
      IdentifierInfo* tokId = tok.getIdentifierInfo();
      ii = tokId;
      return astr(tokId->getNameStart());
    }
  } else {
    // Give up if we didn't handle all the tokens in the above loop
    if (start != end)
      return nullptr;

    // Rule out cases that don't make sense
    // char -> only can add signed or unsigned
    if (_char > 0 &&
        (_short > 0 || _long > 0 || _int > 0 ||  _float > 0 || _double > 0))
      return nullptr;
    // float/double -> only can add long or signed
    if ((_float > 0 || _double > 0) &&
        (_short > 0 || _char > 0 || _int > 0 || _unsigned > 0))
      return nullptr;
    // can't have both float and double
    if (_float > 0 && _double > 0)
      return nullptr;
    // can't have int int or char char
    if (_int > 1 || _char > 1)
      return nullptr;
    // can't have both signed and unsigned
    if (_signed > 0 && _unsigned > 0)
      return nullptr;
    // can't have both short and long
    if (_long > 0 && _short > 0)
      return nullptr;
    // can't have short short or long long long
    if (_long > 2 || _short > 1)
      return nullptr;

    const char* ret = nullptr;

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

    if (ret != nullptr)
      return astr(ret);
  }
  return nullptr;
}

static const char* handleStringExpr(const MacroInfo* inMacro,
                                    MacroInfo::tokens_iterator start,
                                    MacroInfo::tokens_iterator end) {
  removeMacroOuterParens(inMacro, start, end);

  Token tok = *start; // the main token
  ++start;

  // Give up if there are any tokens beyond the main token
  if (start != end)
    return nullptr;

  if (tok.getKind() == tok::string_literal) {
    std::string body = std::string(tok.getLiteralData(), tok.getLength());
    return astr(body.c_str());
  }

  return nullptr;
}

static ::Type* getTypeForMacro(const char* name) {
  // Look for the saved types in dt_c_int etc.
  ::Type* t = getWellKnownTypeWithName(name);
  return t;
}

static bool handleNumericCastExpr(const MacroInfo* inMacro,
                                  MacroInfo::tokens_iterator start,
                                  MacroInfo::tokens_iterator end,
                                  Immediate* imm,
                                  const char*& cCastToTypeRet);
static bool handleNumericUnaryPrefixExpr(const MacroInfo* inMacro,
                                         MacroInfo::tokens_iterator start,
                                         MacroInfo::tokens_iterator end,
                                         Immediate* imm);
static bool handleNumericLiteralExpr(const MacroInfo* inMacro,
                                     MacroInfo::tokens_iterator start,
                                     MacroInfo::tokens_iterator end,
                                     Immediate* imm);
static bool handleNumericBinOpExpr(const MacroInfo* inMacro,
                                   MacroInfo::tokens_iterator start,
                                   MacroInfo::tokens_iterator end,
                                   Immediate* imm);

static bool handleNumericExpr(const MacroInfo* inMacro,
                              MacroInfo::tokens_iterator start,
                              MacroInfo::tokens_iterator end,
                              Immediate* imm,
                              const char*& cCastToTypeRet) {

  cCastToTypeRet = nullptr;

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
                                  MacroInfo::tokens_iterator start,
                                  MacroInfo::tokens_iterator end,
                                  Immediate* imm,
                                  const char*& cCastToTypeRet) {

  if (start == end)
    return false;

  // Check for a cast like '(unsigned int) 12'
  MacroInfo::tokens_iterator castStart = start;
  MacroInfo::tokens_iterator castEnd = start;

  if (findParenthesizedExpr(inMacro, start, end, castStart, castEnd)) {
    if (castEnd == end)
      return false;

    const char* castTo = nullptr;
    clang::IdentifierInfo* ii = nullptr;
    castTo = handleTypeOrIdentifierExpr(inMacro, castStart, castEnd, ii);
    if (castTo == nullptr)
      return false;
    start = castEnd;

    // Find the type to cast to
    // (handles things like macros, nested parens)
    VarSymbol* tmpVar = nullptr;
    TypeDecl* tmpType = nullptr;
    ValueDecl* tmpVal = nullptr;
    const char* tmpCastToType = nullptr;

    handleMacroExpr(inMacro, castStart, castEnd,
                    tmpVar, tmpType, tmpVal, tmpCastToType);

    if (tmpType == nullptr || tmpCastToType != nullptr)
      return false;

    cCastToTypeRet = astr(tmpType->getName().str().c_str());

    const char* rhsCastToTy = nullptr;
    Immediate rhsImm;
    Immediate retImm;
    bool got = handleNumericExpr(inMacro, castEnd, end, &rhsImm, rhsCastToTy);

    if (got == false)
      return false;

    if (rhsCastToTy == nullptr) {
      retImm = rhsImm;
    } else {
      ::Type* t = getTypeForMacro(tmpType->getName().str().c_str());
      if (t == nullptr)
        return false;

      Immediate dstImm = getDefaultImmediate(t);

      coerce_immediate(&rhsImm, &dstImm);

      retImm = dstImm;
    }

    // Try handling the cast now if we can.
    // Put it off if the type isn't known yet (e.g. a typedef)
    ::Type* doCastToType = nullptr;
    if (cCastToTypeRet != nullptr) {
      doCastToType = getTypeForMacro(cCastToTypeRet);
    }

    if (doCastToType != nullptr) {
      Immediate dstImm = getDefaultImmediate(doCastToType);
      coerce_immediate(&retImm, &dstImm);
      *imm = dstImm;
      cCastToTypeRet = nullptr; // cast already handled
    } else {
      *imm = retImm;
    }
    return true;
  }
  return false;
}


static bool handleNumericUnaryPrefixExpr(const MacroInfo* inMacro,
                                         MacroInfo::tokens_iterator start,
                                         MacroInfo::tokens_iterator end,
                                         Immediate* imm) {
  if (start == end)
    return false;

  // handle prefix unary operators + and -
  if (start->getKind() == tok::plus ||
      start->getKind() == tok::minus) {

    if (start+1 == end)
      return false;

    Immediate rhsImm;
    const char* tmpCastToTy = nullptr;
    bool got = handleNumericExpr(inMacro, start+1, end, &rhsImm, tmpCastToTy);
    if (got == false || tmpCastToTy != nullptr)
      return false;

    int p = 0;
    switch (start->getKind()) {
      case tok::plus:   p = P_prim_plus;    break;
      case tok::minus:  p = P_prim_minus;   break;
      default:
        INT_FATAL("unhandled case");
    }

    fold_constant(p, &rhsImm, nullptr, imm);
    return true;
  }

  return false;
}

static bool handleNumericLiteralExpr(const MacroInfo* inMacro,
                                     MacroInfo::tokens_iterator start,
                                     MacroInfo::tokens_iterator end,
                                     Immediate* imm) {
  if (start == end)
    return false;

  // handle a single numeric literal
  if (start->getKind() == tok::numeric_constant && start+1 == end) {
    Token tok = *start; // the main token
    VarSymbol* varRet = nullptr;
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

      ::Type* t = nullptr;
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
      INT_ASSERT(t != nullptr);

      if        (t == dtInt[INT_SIZE_64] || t == dtUInt[INT_SIZE_64]) {
        size = INT_SIZE_64;
      } else if (t == dtInt[INT_SIZE_32] || t == dtUInt[INT_SIZE_32]) {
        size = INT_SIZE_32;
      } else {
        INT_FATAL("unhandled case");
      }

      if (_unsigned > 0) {
        varRet = new_UIntSymbol(strtoul(numString.c_str(), nullptr, 0), size);
      } else {
        varRet = new_IntSymbol(strtol(numString.c_str(), nullptr, 0), size);
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

    INT_ASSERT(varRet != nullptr);
    *imm = *varRet->immediate;
    return true;
  }

  return false;
}

static bool handleNumericBinOpExpr(const MacroInfo* inMacro,
                                   MacroInfo::tokens_iterator start,
                                   MacroInfo::tokens_iterator end,
                                   Immediate* imm) {
  // handle select binary operators
  // this only works if the LHS and RHS are either:
  //  parenthesized expressions; or
  //  numeric constants
  bool lhsOk = false;
  MacroInfo::tokens_iterator lhsStart = start;
  MacroInfo::tokens_iterator lhsEnd = start;

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
  MacroInfo::tokens_iterator op = lhsEnd;
  MacroInfo::tokens_iterator rhsStart = op + 1;
  MacroInfo::tokens_iterator rhsEnd = rhsStart;

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
  const char* lhsCastToTy = nullptr;
  const char* rhsCastToTy = nullptr;

  lhsOk = handleNumericExpr(inMacro, lhsStart, lhsEnd, &lhsImm, lhsCastToTy);
  rhsOk = handleNumericExpr(inMacro, rhsStart, rhsEnd, &rhsImm, rhsCastToTy);
  if (lhsOk == false || rhsOk == false)
    return false;
  if (lhsCastToTy != nullptr || rhsCastToTy != nullptr)
    return false;

  // Apply the binary operator to the immediate
  int p = 0;
  switch (op->getKind()) {
    case tok::lessless:   p = P_prim_lsh;    break;
    default:
      return false; // this operator not handled
  }

  fold_constant(p, &lhsImm, &rhsImm, imm);
  return true;
}

static void handleMacroExpr(const MacroInfo* inMacro,
                            MacroInfo::tokens_iterator start,
                            MacroInfo::tokens_iterator end,
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

  varRet = nullptr;
  cTypeRet = nullptr;
  cValueRet = nullptr;

  if (start == end) {
    if( debugPrint) {
      printf("the macro is empty\n");
    }
    return;
  }

  if (debugPrint) {
    for (MacroInfo::tokens_iterator cur = start;
         cur != end;
         ++cur) {
      Token t = *cur;
      printf("Found token type %i\n", t.getKind());
    }
  }

  clang::IdentifierInfo* ii = nullptr;
  const char* idName = handleTypeOrIdentifierExpr(inMacro, start, end, ii);
  if (idName != nullptr) {
    if( debugPrint) {
      printf("id = %s\n", idName);
    }

    // Handle the case where the macro refers to something we've
    // already parsed in C
    varRet = info->lvt->getVarSymbol(idName);
    if( !varRet ) {
      info->lvt->getCDecl(idName, &cTypeRet, &cValueRet);
    }
    if( !varRet && !cTypeRet && !cValueRet && ii != nullptr) {
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
  if (str != nullptr) {
    if( debugPrint) printf("str = %s\n", str);
    varRet = new_CStringSymbol(str);
    return;
  }

  Immediate imm;
  const char* castToTy = nullptr;
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
  LayeredValueTable *lvt = info->lvt;

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

  public:
    CCodeGenConsumer()
      : ASTConsumer(),
        info(gGenInfo),
        Diags(info->clangInfo->Diags.get()),
        Builder(nullptr),
        parseOnly(info->clangInfo->parseOnly),
        savedCtx(nullptr)
    {

      if (!parseOnly) {
        Builder = CreateLLVMCodeGen(
          *Diags,
          LLVM_MODULE_NAME,
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
        if(ctype != nullptr) {
          info->lvt->addGlobalCDecl(td);
        }
      } else if (FunctionDecl *fd = dyn_cast<FunctionDecl>(d)) {
        info->lvt->addGlobalCDecl(fd);
      } else if (VarDecl *vd = dyn_cast<VarDecl>(d)) {
        info->lvt->addGlobalCDecl(vd);
      } else if (RecordDecl *rd = dyn_cast<RecordDecl>(d)) {
        if( rd->getName().size() > 0 ) {
          // Handle forward declaration for structs
          info->lvt->addGlobalCDecl(rd);
        }
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

         if(ctype != nullptr && rd->getDefinition() != nullptr) {
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
  clangInfo->Diags.reset();
  clangInfo->DiagID.reset();
}

static void deleteClang(ClangInfo* clangInfo){
  if( clangInfo->cCodeGen ) {
    delete clangInfo->cCodeGen;
    clangInfo->cCodeGen = nullptr;
  }
  delete clangInfo->Clang;
  clangInfo->Clang = nullptr;
  delete clangInfo->cCodeGenAction;
  clangInfo->cCodeGenAction = nullptr;
}

static void cleanupClang(ClangInfo* clangInfo)
{
  finishClang(clangInfo);
  deleteClang(clangInfo);
}

void setupClang(GenInfo* info, std::string mainFile)
{
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  std::string clangexe = clangInfo->clangCC;
  std::vector<const char*> clangArgs;

  clangArgs.push_back("<chapel clang driver invocation>");

  for( size_t i = 0; i < clangInfo->clangCCArgs.size(); ++i ) {
    clangArgs.push_back(clangInfo->clangCCArgs[i].c_str());
  }
  for( size_t i = 0; i < clangInfo->clangOtherArgs.size(); ++i ) {
    clangArgs.push_back(clangInfo->clangOtherArgs[i].c_str());
  }

  clangArgs.push_back("-c");
  clangArgs.push_back(mainFile.c_str()); // chpl - always compile rt file

  if (!fLlvmCodegen)
    clangArgs.push_back("-fsyntax-only");

  if( printSystemCommands && developer ) {
    for( size_t i = 0; i < clangArgs.size(); i++ ) {
      printf("%s ", clangArgs[i]);
    }
    printf("\n");
  }

  // Initialize LLVM targets so that the clang commands can know if the
  // target CPU supports vectorization, avx, etc, etc
  // Also important for generating assembly from this program.
  if (fLlvmCodegen) {
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();
  }

  // Create a compiler instance to handle the actual work.
  CompilerInstance* Clang = new CompilerInstance();
  Clang->createDiagnostics();

  clangInfo->diagOptions = new DiagnosticOptions();
  clangInfo->DiagClient= new TextDiagnosticPrinter(errs(),&*clangInfo->diagOptions);
  clangInfo->DiagID = new DiagnosticIDs();
  DiagnosticsEngine* Diags = nullptr;
  Diags = new DiagnosticsEngine(
      clangInfo->DiagID, &*clangInfo->diagOptions, clangInfo->DiagClient);
  clangInfo->Diags = Diags;
  clangInfo->Clang = Clang;

  clang::driver::Driver TheDriver(clangexe, llvm::sys::getDefaultTargetTriple(), *Diags);

  //   SetInstallDir(argv, TheDriver);

  std::unique_ptr<clang::driver::Compilation> C(TheDriver.BuildCompilation(clangArgs));

  clang::driver::Command* job = nullptr;

  if (localeUsesGPU() == false) {
    // Not a CPU+GPU compilation, so just use first job.
    job = &*C->getJobs().begin();
  } else {
    // CPU+GPU compilation
    //  1st cc1 command is for the GPU
    //  2nd cc1 command is for the CPU
    for (auto &command : C->getJobs()) {
      bool isCC1 = false;
      for (auto arg : command.getArguments()) {
        if (0 == strcmp(arg, "-cc1")) {
          isCC1 = true;
          break;
        }
      }
      if (isCC1) {

        if (gCodegenGPU) {
          // For GPU, set job to 1st cc1 command
          if (job == nullptr) job = &command;
        } else {
          // For CPU, set job to last cc1 command
          job = &command;
        }
      }
    }
  }


  if (job == nullptr)
    USR_FATAL("Could not find cc1 command from clang driver");

  if( printSystemCommands && developer ) {
    printf("<internal clang cc> ");
    for ( auto a : job->getArguments() ) {
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
            *Diags);
#else
  bool success = CompilerInvocation::CreateFromArgs(
            Clang->getInvocation(),
            &job->getArguments().front(), (&job->getArguments().back())+1,
            *Diags);
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

    P = clangexe;
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
    Args.push_back(nullptr);

    if (printSystemCommands && developer) {
      printf("# parsing llvm command line options: ");
      for (auto arg : Args) {
        if (arg != nullptr)
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

static void setupModule()
{
  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  if (clangInfo->parseOnly) return;

  INT_ASSERT(info->module);

  clangInfo->asmTargetLayoutStr =
    clangInfo->Clang->getTarget().getDataLayout().getStringRepresentation();

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
  llvm::TargetOptions targetOptions;
  targetOptions.ThreadModel = llvm::ThreadModel::POSIX;

  // Set the floating point optimization level
  // see also code setting FastMathFlags
  if (ffloatOpt == 1) {
    // --no-ieee-float
    // Allow unsafe fast floating point optimization
    targetOptions.UnsafeFPMath = 1; // e.g. FSIN instruction
    targetOptions.NoInfsFPMath = 1;
    targetOptions.NoNaNsFPMath = 1;
    targetOptions.NoTrappingFPMath = 1;
    targetOptions.NoSignedZerosFPMath = 1;
    targetOptions.AllowFPOpFusion = llvm::FPOpFusion::Fast;
  } else if (ffloatOpt == 0) {
    // Target default floating point optimization
    targetOptions.AllowFPOpFusion = llvm::FPOpFusion::Standard;
  } else if (ffloatOpt == -1) {
    // --ieee-float
    // Should this set targetOptions.HonorSignDependentRoundingFPMathOption ?
    // Allow fused multiply-adds
    targetOptions.AllowFPOpFusion = llvm::FPOpFusion::Standard;
  }

  if (!fFastFlag)
    targetOptions.EnableFastISel = 1;
  else {
    // things to consider:
    // EnableIPRA  -- InterProcedural Register Allocation (IPRA).
    // GuaranteedTailCallOpt -- guarantee tail call opt (may change fn ABI)
  }

  llvm::Reloc::Model relocModel = llvm::Reloc::Model::Static;

  if (strcmp(CHPL_LIB_PIC, "pic") == 0) {
    relocModel = llvm::Reloc::Model::PIC_;
  }

  // Choose the code model
  llvm::Optional<CodeModel::Model> codeModel = None;

  llvm::CodeGenOpt::Level optLevel =
    fFastFlag ? llvm::CodeGenOpt::Aggressive : llvm::CodeGenOpt::None;

  // Create the target machine.
  info->targetMachine = Target->createTargetMachine(Triple.str(),
                                                    cpu,
                                                    featuresString,
                                                    targetOptions,
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
  info->FPM_postgen = nullptr;

  // Now finish any Clang code generation.
  finishClang(info->clangInfo);

  // Now overwrite the value of llvm.ident to show Chapel
  char version[128];
  char chapel_string[256];
  get_version(version);
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

static
void configurePMBuilder(PassManagerBuilder &PMBuilder, bool forFunctionPasses, int optLevel=-1) {
  ClangInfo* clangInfo = gGenInfo->clangInfo;
  INT_ASSERT(clangInfo);
  clang::CodeGenOptions &opts = clangInfo->codegenOptions;

  if (optLevel < 0)
    optLevel = opts.OptimizationLevel;

  if( fFastFlag ) {
    // TODO -- remove this assert
    INT_ASSERT(opts.OptimizationLevel >= 2);
  }

  if (optLevel >= 1)
    PMBuilder.Inliner = createFunctionInliningPass(optLevel,
                                                   opts.OptimizeSize,
                                                   /*DisableInlineHotCallsite*/
                                                   false
                                                  );

  PMBuilder.OptLevel = optLevel;
  PMBuilder.SizeLevel = opts.OptimizeSize;
  PMBuilder.SLPVectorize = opts.VectorizeSLP;
  PMBuilder.LoopVectorize = opts.VectorizeLoop;

  PMBuilder.DisableUnrollLoops = !opts.UnrollLoops;
  PMBuilder.MergeFunctions = opts.MergeFunctions;
#if HAVE_LLVM_VER > 60
  PMBuilder.PrepareForThinLTO = opts.PrepareForThinLTO;
#else
  PMBuilder.PrepareForThinLTO = opts.EmitSummaryIndex;
#endif

  PMBuilder.PrepareForLTO = opts.PrepareForLTO;
  PMBuilder.RerollLoops = opts.RerollLoops;


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

static void handleErrorLLVM(void* user_data, const std::string& reason,
                            bool gen_crash_diag)
{
  INT_FATAL("llvm fatal error: %s", reason.c_str());
}

struct ExternBlockInfo {
  GenInfo* gen_info;
  fileinfo file;
  ExternBlockInfo() : gen_info(nullptr), file() { }
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


void runClang(const char* just_parse_filename) {
  static bool is_installed_fatal_error_handler = false;

  const char* clang_warn[] = {"-Wall", "-Werror", "-Wpointer-arith",
                              "-Wwrite-strings", "-Wno-strict-aliasing",
                              "-Wmissing-declarations", "-Wmissing-prototypes",
                              "-Wstrict-prototypes",
                              "-Wmissing-format-attribute",
                              // clang can't tell which functions we use
                              "-Wno-unused-function",
                              nullptr};
  const char* clang_debug = "-g";
  const char* clang_opt = "-O3";
  const char* clang_fast_float = "-ffast-math";
  const char* clang_ieee_float = "-fno-fast-math";

  std::vector<std::string> args;
  std::vector<std::string> clangCCArgs;
  std::vector<std::string> clangOtherArgs;

  // find the path to clang and clang++
  std::string llvm_install, clangCC, clangCXX;

  if (0 == strcmp(CHPL_LLVM, "system")) {
    clangCC = get_clang_cc();
    clangCXX = get_clang_cxx();
  } else if (0 == strcmp(CHPL_LLVM, "llvm") ||
             0 == strcmp(CHPL_LLVM, "bundled")) {
    llvm_install += CHPL_THIRD_PARTY;
    llvm_install += "/llvm/install/";
    llvm_install += CHPL_LLVM_UNIQ_CFG_PATH;
    llvm_install += "/bin/";
    clangCC = llvm_install + "clang";
    clangCXX = llvm_install + "clang++";
  } else {
    USR_FATAL("Unsupported CHPL_LLVM setting %s", CHPL_LLVM);
  }

  // read clang-sysroot-arguments
  std::string sysroot_arguments(CHPL_THIRD_PARTY);
  sysroot_arguments += "/llvm/install/";
  sysroot_arguments += CHPL_LLVM_UNIQ_CFG_PATH;
  sysroot_arguments += "/configured-clang-sysroot-arguments";

  // read arguments from configured-clang-sysroot-arguments
  // these might include a key -isysroot argument on Mac OS X
  readArgsFromFile(sysroot_arguments, args);

  // read arguments that we captured at compile time
  splitStringWhitespace(get_clang_sysroot_args(), args);

  std::string runtime_includes(CHPL_RUNTIME_LIB);
  runtime_includes += "/";
  runtime_includes += CHPL_RUNTIME_SUBDIR;
  runtime_includes += "/list-includes-and-defines";

  bool rtOk = readArgsFromFile(runtime_includes, args, /*errFatal*/ false);
  if (rtOk == false) {
    std::string runtime_dir(CHPL_RUNTIME_LIB);
    runtime_dir += "/";
    runtime_dir += CHPL_RUNTIME_SUBDIR;

    if (developer)
      USR_FATAL_CONT("Expected runtime library in %s", runtime_dir.c_str());

    const char* module_home = getenv("CHPL_MODULE_HOME");
    if (module_home) {
      USR_FATAL("The requested configuration is not included in the module. "
                "Please send the package maintainer the output of "
                "$CHPL_HOME/util/printchplenv and request support for this "
                "configuration.");
    } else {
      USR_FATAL("The runtime has not been built for this configuration. "
                "Check $CHPL_HOME/util/printchplenv and try rebuilding "
                "with $CHPL_MAKE from $CHPL_HOME.");
    }
  }

  // Remove -DCHPL_DEBUG, -DCHPL_OPTIMIZE, -DNDEBUG from the args
  // They are settings from the runtime build and we want to
  // use flags appropriate to the compilation instead of the runtime build
  std::vector<std::string>::iterator pos =
    std::find(args.begin(), args.end(), "-DCHPL_DEBUG");
  if (pos != args.end())
    args.erase(pos);

  pos = std::find(args.begin(), args.end(), "-DCHPL_OPTIMIZE");
  if (pos != args.end())
    args.erase(pos);


  pos = std::find(args.begin(), args.end(), "-DNDEBUG");
  if (pos != args.end())
    args.erase(pos);

  std::string dashImodules = "-I";
  dashImodules += CHPL_HOME;
  dashImodules += "/modules";

  args.push_back(dashImodules + "/standard");
  args.push_back(dashImodules + "/packages");

  // Substitute $CHPL_HOME $CHPL_RUNTIME_LIB etc
  expandInstallationPaths(args);

  if (compilingWithPrgEnv()) {
    std::string gather_prgenv(CHPL_HOME);
    gather_prgenv += "/util/config/gather-cray-prgenv-arguments.bash compile '";
    gather_prgenv += CHPL_TARGET_PLATFORM;
    gather_prgenv += "' '";
    gather_prgenv += CHPL_COMM;
    gather_prgenv += "' '";
    gather_prgenv += CHPL_COMM_SUBSTRATE;
    gather_prgenv += "' '";
    gather_prgenv += CHPL_AUX_FILESYS;
    gather_prgenv += "'";
    readArgsFromCommand(gather_prgenv, args);
  }

  if (ccwarnings) {
    for (int i = 0; clang_warn[i]; i++) {
      args.push_back(clang_warn[i]);
    }
  }

  if (debugCCode) {
    args.push_back(clang_debug);
    args.push_back("-DCHPL_DEBUG");
  }

  if (optimizeCCode) {
    args.push_back(clang_opt);
    args.push_back("-DCHPL_OPTIMIZE");
  }

  if (specializeCCode &&
      CHPL_TARGET_CPU_FLAG != nullptr &&
      CHPL_TARGET_BACKEND_CPU != nullptr &&
      CHPL_TARGET_CPU_FLAG[0] != '\0' &&
      CHPL_TARGET_BACKEND_CPU[0] != '\0' &&
      0 != strcmp(CHPL_TARGET_CPU_FLAG, "none") &&
      0 != strcmp(CHPL_TARGET_BACKEND_CPU, "none") &&
      0 != strcmp(CHPL_TARGET_BACKEND_CPU, "unknown")) {
    std::string march = "-m";
    march += CHPL_TARGET_CPU_FLAG;
    march += "=";
    march += CHPL_TARGET_BACKEND_CPU;
    args.push_back(march);
  }

  // Passing -ffast-math is important to get approximate versions
  // of cabs but it appears to slow down simple complex multiplication.
  if (ffloatOpt > 0) // --no-ieee-float
    args.push_back(clang_fast_float); // --ffast-math

  if (ffloatOpt < 0) // --ieee-float
    args.push_back(clang_ieee_float); // -fno-fast-math

  // Gather information from readargsfrom into clangArgs.

  // Note that these CC arguments will be saved in info->clangCCArgs
  // and will be used when compiling C files as well.
  bool saw_sysroot = false;
  for( size_t i = 0; i < args.size(); ++i ) {
    clangCCArgs.push_back(args[i]);
    if (args[i] == "-isysroot")
      saw_sysroot = true;
  }

  for_vector(const char, dirName, incDirs) {
    clangCCArgs.push_back(std::string("-I") + dirName);
  }
  clangCCArgs.push_back(std::string("-I") + getIntermediateDirName());
  if (saw_sysroot) {
    // Work around a bug in some versions of Clang that forget to
    // search /usr/local/include if there is a -isysroot argument.
    clangCCArgs.push_back(std::string("-I/usr/local/include"));
  }

  //split ccflags by spaces
  splitStringWhitespace(ccflags, clangCCArgs);

  clangCCArgs.push_back("-pthread");

  // library directories/files and ldflags are handled during linking later.

  clangCCArgs.push_back("-DCHPL_GEN_CODE");

  // tell clang to use CUDA support
  if (localeUsesGPU()) {
    // Need to pass this flag so atomics header will compile
    clangOtherArgs.push_back("--std=c++11");
    // Need to select CUDA mode in embedded clang to
    // activate the GPU target
    clangOtherArgs.push_back("-x");
    clangOtherArgs.push_back("cuda");
  }

  // Always include sys_basic because it might change the
  // behaviour of macros!
  clangOtherArgs.push_back("-include");
  clangOtherArgs.push_back("sys_basic.h");

  if (!just_parse_filename) {
    // Running clang to compile all runtime and extern blocks

    // Include header files from the command line.
    {
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
  } else {
    // Just running clang to parse the extern blocks for this module.
    clangOtherArgs.push_back("-include");
    clangOtherArgs.push_back(just_parse_filename);
  }

  if( printSystemCommands ) {
    if (just_parse_filename != nullptr)
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
  gGenInfo = new GenInfo();

  bool parseOnly = (just_parse_filename != nullptr);

  gGenInfo->lvt = new LayeredValueTable();


  ClangInfo* clangInfo = nullptr;
  clangInfo = new ClangInfo(clangCC, clangCXX,
                            clangCCArgs, clangOtherArgs,
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
      if (just_parse_filename) {
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
    gGenInfo = nullptr;
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

  return nullptr;
}

clang::FunctionDecl* getFunctionDeclClang(const char* name)
{
  GenInfo* info = gGenInfo;

  clang::TypeDecl* cType = nullptr;
  clang::ValueDecl* cValue = nullptr;

  info->lvt->getCDecl(name, &cType, &cValue);
  clang::FunctionDecl* FD = llvm::dyn_cast_or_null<clang::FunctionDecl>(cValue);
  return FD;
}

llvm::Type* getTypeLLVM(const char* name)
{
  GenInfo* info = gGenInfo;
  llvm::Type* t = info->module->getTypeByName(name);
  if( t ) return t;

  t = info->lvt->getType(name);
  if( t ) return t;

  return nullptr;
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
    INT_ASSERT(def);
    qType=def->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
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

    llvm::Type* type = nullptr;
    type = clang::CodeGen::convertTypeForMemory(cCodeGen->CGM(), ed->getType());

    ret.val = ConstantInt::get(type, v);
    ret.isLVPtr = GEN_VAL;
  } else {
    INT_FATAL("Unknown clang value declaration");
  }
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
    StringRef name, Value *value, uint8_t isLVPtr, bool isUnsigned) {
  Storage store;
  store.u.value = value;
  store.isLVPtr = isLVPtr;
  store.isUnsigned = isUnsigned;
  (layers.back())[name] = store;
}

void LayeredValueTable::addGlobalValue(StringRef name, GenRet gend) {
  addGlobalValue(name, gend.val, gend.isLVPtr, gend.isUnsigned);
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
  if (cdecl->getIdentifier() == nullptr) {
    // Certain C++ things such as constructors can have
    // special compound names. In this case getName() will
    // fail.
    return;
  }

  addGlobalCDecl(cdecl->getName(), cdecl);

  // Also file structs under 'struct struct_name'
  if(isa<RecordDecl>(cdecl)) {
    std::string sname = "struct ";
    sname += cdecl->getName();
    addGlobalCDecl(sname, cdecl);
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

GenRet LayeredValueTable::getValue(StringRef name) {
  if(Storage *store = get(name)) {
    if( store->u.value ) {
      INT_ASSERT(isa<Value>(store->u.value));
      GenRet ret;
      ret.val = store->u.value;
      ret.isLVPtr = store->isLVPtr;
      ret.isUnsigned = store->isUnsigned;
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
  return nullptr;
}

llvm::Type *LayeredValueTable::getType(StringRef name, bool *isUnsigned) {
  if(Storage *store = get(name)) {
    if (isUnsigned != nullptr) {
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
      if (type != nullptr) {
        store->isUnsigned = type->isUnsignedIntegerOrEnumerationType();
      }
      return store->u.type;
    }
  }
  return nullptr;
}

// Returns a type or a name decl for a given name
// Note that C can have a function and a type sharing the same name
// (e.g. stat/struct stat).
// Sets the output arguments to nullptr if a type/value was not found.
// Either output argument can be nullptr.
void LayeredValueTable::getCDecl(StringRef name, TypeDecl** cTypeOut,
    ValueDecl** cValueOut, const char** cCastedToTypeOut, astlocT* astlocOut) {

  if (cValueOut) *cValueOut = nullptr;
  if (cTypeOut) *cTypeOut = nullptr;
  if (cCastedToTypeOut) *cCastedToTypeOut = nullptr;

  if(Storage *store = get(name)) {
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
  clang::TypeDecl* cType = nullptr;
  clang::ValueDecl* cValue = nullptr;

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
  return nullptr;
}

LayeredValueTable::Storage* LayeredValueTable::get(StringRef name) {
  for(layer_iterator i = layers.begin(); i != layers.end(); ++i) {
    value_iterator j = i->find(name);

    if(j != i->end())
    {
      return &j->second;
    }
  }

  return nullptr;
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

  TypeDecl* d = nullptr;
  int ret;

  info->lvt->getCDecl(typeName, &d, nullptr);
  INT_ASSERT(d);
  if( isa<TypedefDecl>(d) ) {
    TypedefDecl* td = cast<TypedefDecl>(d);
    const clang::Type* t = td->getUnderlyingType().getTypePtr();
    while( t->isPointerType() ) {
      t = t->getPointeeType().getTypePtr();
    }
    const RecordType* rt = t->getAsStructureType();
    INT_ASSERT(rt);
    d = rt->getDecl();
    // getAsUnionType also available, but we don't support extern unions
  }
  INT_ASSERT(isa<RecordDecl>(d));
  RecordDecl* rec = cast<RecordDecl>(d);
  // Find the field decl.
  RecordDecl::field_iterator it;
  FieldDecl* field = nullptr;
  for( it = rec->field_begin(); it != rec->field_end(); ++it ) {
    if( fieldName == it->getName() ) {
      field = *it;
      break;
    }
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
    cTy = Ctx->getPointerType(cTy);
    return cTy;
  }
  TypeSymbol* ts = t->symbol;
  if (ts->hasFlag(FLAG_C_PTR_CLASS) || ts->hasFlag(FLAG_DATA_CLASS)) {
    ::Type* eltType = getDataClassType(ts)->type;
    clang::CanQualType cTy = getClangType(eltType, false);
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

  clang::TypeDecl* cTypeDecl = nullptr;
  clang::ValueDecl* cValueDecl = nullptr;
  const char* cCastedToType = nullptr;
  const char* cname = t->symbol->cname;
  info->lvt->getCDecl(cname, &cTypeDecl, &cValueDecl, &cCastedToType);

  if (cCastedToType)
    USR_FATAL(t, "Cannot use macro with type cast in export function argument");

  if (cTypeDecl == nullptr)
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

  clang::TypeDecl* cType = nullptr;
  clang::ValueDecl* cValue = nullptr;
  const char* cCastedToType = nullptr;

  info->lvt->getCDecl(fn->cname, &cType, &cValue, &cCastedToType);

  clang::FunctionDecl* FD = llvm::dyn_cast_or_null<clang::FunctionDecl>(cValue);
  if (FD != nullptr) {
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

  // Don't try to use the the calling convention code for variadic args.
  if ((unsigned) curCArg >= CGI->arg_size() && CGI->isVariadic())
    return nullptr;

  const clang::CodeGen::ABIArgInfo* argInfo = nullptr;
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
    clang::TypeDecl* cType = nullptr;
    clang::ValueDecl* cVal = nullptr;
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
                        info->getPutFn, info->memsetFn, nullptr};

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

  llvm::verifyFunction(*fn, &errs());

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

static void makeLLVMStaticLibrary(std::string moduleFilename,
                                  const char* tmpbinname,
                                  std::vector<std::string> dotOFiles);
static void makeLLVMDynamicLibrary(std::string useLinkCXX, std::string options,
                            std::string moduleFilename, const char* tmpbinname,
                            std::vector<std::string> dotOFiles,
                            std::vector<std::string> clangLDArgs,
                            bool sawSysroot);
static std::string buildLLVMLinkCommand(std::string useLinkCXX,
                                        std::string options,
                                        std::string moduleFilename,
                                        std::string maino,
                                        const char* tmpbinname,
                                        std::vector<std::string> dotOFiles,
                                        std::vector<std::string> clangLDArgs,
                                        bool sawSysroot);
static void runLLVMLinking(std::string useLinkCXX, std::string options,
                           std::string moduleFilename, std::string maino,
                           const char* tmpbinname,
                           std::vector<std::string> dotOFiles,
                           std::vector<std::string> clangLDArgs,
                           bool sawSysroot);
static std::string getLibraryOutputPath();
static void moveGeneratedLibraryFile(const char* tmpbinname);
static void moveResultFromTmp(const char* resultName, const char* tmpbinname);

void makeBinaryLLVM(void) {

  GenInfo* info = gGenInfo;
  INT_ASSERT(info);
  ClangInfo* clangInfo = info->clangInfo;
  INT_ASSERT(clangInfo);

  std::string moduleFilename;
  std::string preOptFilename;
  std::string opt1Filename;
  std::string opt2Filename;
  std::string asmFilename;
  std::string ptxObjectFilename;
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
    asmFilename = genIntermediateFilename("chpl__gpu_ptx.s");
    ptxObjectFilename = genIntermediateFilename("chpl__gpu_ptx.o");
    fatbinFilename = genIntermediateFilename("chpl__gpu.fatbin");
  }

  if( saveCDir[0] != '\0' ) {
    std::error_code tmpErr;
    // Save the generated LLVM before optimization.
    ToolOutputFile output (preOptFilename.c_str(),
                             tmpErr, sys::fs::F_None);
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
  llvm::sys::fs::OpenFlags flags = llvm::sys::fs::F_None;

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
      ToolOutputFile output1 (opt1Filename.c_str(),
                               tmpErr, sys::fs::F_None);
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
        ToolOutputFile output2 (opt2Filename.c_str(),
                                 tmpErr, sys::fs::F_None);
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

    } else {

      llvm::CodeGenFileType asmFileType =
        llvm::CodeGenFileType::CGFT_AssemblyFile;

      llvm::raw_fd_ostream outputASMfile(asmFilename, error, flags);

      {

        llvm::legacy::PassManager emitPM;

        emitPM.add(createTargetTransformInfoWrapperPass(
                   info->targetMachine->getTargetIRAnalysis()));

        info->targetMachine->addPassesToEmitFile(emitPM, outputASMfile,
                                                 nullptr,
                                                 asmFileType,
                                                 disableVerify);

        emitPM.run(*info->module);

      }

      outputASMfile.close();

      if (mysystem("which ptxas > /dev/null 2>&1", "Check to see if ptxas command can be found", true)) {
        USR_FATAL("Command 'ptxas' not found\n");
      }

      if (mysystem("which fatbinary > /dev/null 2>&1", "Check to see if fatbinary command can be found", true)) {
        USR_FATAL("Command 'fatbinary' not found\n");
      }


      std::string ptxCmd = std::string("ptxas -m64 --gpu-name ") +
                           std::string("sm_60 --output-file ") + ptxObjectFilename.c_str() +
                           " " + asmFilename.c_str();

      mysystem(ptxCmd.c_str(), "PTX to  object file");

      std::string fatbinaryCmd = std::string("fatbinary -64 ") +
                                 std::string("--create ") + fatbinFilename.c_str() +
                                 std::string(" --image=profile=sm_60,file=") + ptxObjectFilename.c_str() +
                                 std::string(" --image=profile=compute_60,file=") + asmFilename.c_str();

      mysystem(fatbinaryCmd.c_str(), "object file to fatbinary");

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

  std::string runtime_libs(CHPL_RUNTIME_LIB);
  runtime_libs += "/";
  runtime_libs += CHPL_RUNTIME_SUBDIR;
  runtime_libs += "/list-libraries";

  std::vector<std::string> runtimeArgs;
  readArgsFromFile(runtime_libs, runtimeArgs);

  std::vector<std::string> clangLDArgs;

  if (compilingWithPrgEnv()) {
    std::string gather_prgenv(CHPL_HOME);
    gather_prgenv += "/util/config/gather-cray-prgenv-arguments.bash link '";

    gather_prgenv += CHPL_TARGET_PLATFORM;
    gather_prgenv += "' '";
    gather_prgenv += CHPL_COMM;
    gather_prgenv += "' '";
    gather_prgenv += CHPL_COMM_SUBSTRATE;
    gather_prgenv += "' '";
    gather_prgenv += CHPL_AUX_FILESYS;
    gather_prgenv += "'";

    std::vector<std::string> gatheredArgs;
    readArgsFromCommand(gather_prgenv, gatheredArgs);

    if (fLinkStyle == LS_DEFAULT) {
      // check for indication that the PrgEnv defaults to dynamic linking
      bool defaultDynamic = false;
      for(size_t i = 0; i < gatheredArgs.size(); i++)
        if (gatheredArgs[i].find("-Wl,-Bdynamic") != std::string::npos)
          defaultDynamic = true;

      // Older Cray PrgEnv defaults to static linking.  If we are asking for
      // the default link type, and we don't find an explicit dynamic
      // flag in the gathered PrgEnv arguments, then force static linking
      // because LLVM's default (dynamic) is different from the PrgEnv
      // default (static).
      if (defaultDynamic == false)
        fLinkStyle = LS_STATIC;
    }

    // Replace -lchpl_lib_token with the runtime arguments
    // but don't add a redundant -lhugetlbfs because that
    // library is already included
    bool found = false;
    for(size_t i = 0; i < gatheredArgs.size(); ++i) {
      if (gatheredArgs[i] == "-lchpl_lib_token") {
        found = true;
        for(size_t j = 0; j < runtimeArgs.size(); ++j) {
          if (runtimeArgs[j] != "-lhugetlbfs")
            clangLDArgs.push_back(runtimeArgs[j]);
        }
      } else {
        clangLDArgs.push_back(gatheredArgs[i]);
      }
    }

    if (!found) INT_FATAL("could not find -lchpl_lib_token in gathered arguments");

  } else {
    clangLDArgs = runtimeArgs;
  }

  // Substitute $CHPL_HOME $CHPL_RUNTIME_LIB etc
  expandInstallationPaths(clangLDArgs);


  std::string runtime_ld_override(CHPL_RUNTIME_LIB);
  runtime_ld_override += "/";
  runtime_ld_override += CHPL_RUNTIME_SUBDIR;
  runtime_ld_override += "/override-ld";

  std::vector<std::string> ldOverride;
  readArgsFromFile(runtime_ld_override, ldOverride);
  // Substitute $CHPL_HOME $CHPL_RUNTIME_LIB etc
  expandInstallationPaths(ldOverride);

  std::string clangCC = clangInfo->clangCC;
  std::string clangCXX = clangInfo->clangCXX;
  std::string useLinkCXX = clangCXX;

  if (ldOverride.size() > 0)
    useLinkCXX = ldOverride[0];


  std::vector<std::string> dotOFiles;

  // Gather C flags for compiling C files.
  std::string cargs;
  for( size_t i = 0; i < clangInfo->clangCCArgs.size(); ++i ) {
    cargs += " ";
    cargs += clangInfo->clangCCArgs[i];
  }

  int filenum = 0;
  while (const char* inputFilename = nthFilename(filenum++)) {
    if (isCSource(inputFilename)) {
      const char* objFilename = objectFileForCFile(inputFilename);
      std::string cmd = clangCC + " -c -o " + objFilename + " " +
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

  std::vector<std::string> sysroot_args;
  std::string sysroot_arguments(CHPL_THIRD_PARTY);
  sysroot_arguments += "/llvm/install/";
  sysroot_arguments += CHPL_LLVM_UNIQ_CFG_PATH;
  sysroot_arguments += "/configured-clang-sysroot-arguments";

  readArgsFromFile(sysroot_arguments, sysroot_args);

  // add arguments from configured-clang-sysroot-arguments
  bool sawSysroot = false;
  for (auto &s : sysroot_args) {
    options += " ";
    options += s;
    if (s == "-isysroot")
      sawSysroot = true;
  }
  // add arguments that we captured at compile time
  options += " ";
  options += get_clang_sysroot_args();

  if(debugCCode) {
    options += " -g";
  }

  // We used to supply link args here *and* later on
  // in the link line. I think the later position is sufficient.
  /*
  for( size_t i = 0; i < clangLDArgs.size(); ++i ) {
    options += " ";
    options += clangLDArgs[i].c_str();
  }*/

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
  const char* tmpbinname = nullptr;

  codegen_makefile(&mainfile, &tmpbinname, true);
  INT_ASSERT(tmpbinname);

  if (fLibraryCompile) {
    switch (fLinkStyle) {
    // The default library link style for Chapel is _static_.
    case LS_DEFAULT:
    case LS_STATIC:
      makeLLVMStaticLibrary(moduleFilename, tmpbinname, dotOFiles);
      break;
    case LS_DYNAMIC:
      makeLLVMDynamicLibrary(useLinkCXX, options, moduleFilename, tmpbinname,
                             dotOFiles, clangLDArgs, sawSysroot);
      break;
    default:
      INT_FATAL("Unsupported library link mode");
      break;
    }
  } else {
    // Runs the LLVM link command for executables.
    runLLVMLinking(useLinkCXX, options, moduleFilename, maino, tmpbinname,
                   dotOFiles, clangLDArgs, sawSysroot);
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
                                   std::vector<std::string> clangLDArgs,
                                   bool sawSysroot) {

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
                                             dotOFiles, clangLDArgs,
                                             sawSysroot);

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
                                        std::vector<std::string> clangLDArgs,
                                        bool sawSysroot) {
  // Run the linker. We always use a C++ compiler because some third-party
  // libraries are written in C++. Here we use clang++ or possibly a
  // linker override specified by the Makefiles (e.g. setting it to mpicxx)
  std::string command = useLinkCXX + " " + options + " " +
                        moduleFilename + " " + maino;

  // For dynamic linking, leave it alone.  For static, append -static .
  // See $CHPL_HOME/make/compiler/Makefile.clang (and keep this in sync
  // with it).
  if (fLinkStyle == LS_STATIC) {
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

  if (sawSysroot) {
    // Work around a bug in some versions of Clang that forget to
    // search /usr/local/lib if there is a -isysroot argument.
    command += " -L/usr/local/lib";
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
                           std::vector<std::string> clangLDArgs,
                           bool sawSysroot) {

  // This code is general enough to use elsewhere, thus the move.
  std::string command = buildLLVMLinkCommand(useLinkCXX,
                                             options,
                                             moduleFilename,
                                             maino,
                                             tmpbinname,
                                             dotOFiles,
                                             clangLDArgs,
                                             sawSysroot);

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

void print_clang(clang::Decl* d) {
  if (d == nullptr)
    fprintf(stderr, "NULL");
  else
    d->print(llvm::dbgs());

  fprintf(stderr, "\n");
}

void print_clang(clang::TypeDecl* d) {
  if (d == nullptr)
    fprintf(stderr, "NULL");
  else
    d->print(llvm::dbgs());

  fprintf(stderr, "\n");
}
void print_clang(clang::ValueDecl* d) {
  if (d == nullptr)
    fprintf(stderr, "NULL");
  else
    d->print(llvm::dbgs());

  fprintf(stderr, "\n");
}

#endif
