/*
 * Copyright 2004-2017 Cray Inc.
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

#include "codegen.h"
#include "clangSupport.h"

#include "build.h"

#include "llvmDebug.h"

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

#define GLOBAL_PTR_SPACE 100
#define WIDE_PTR_SPACE 101
#define GLOBAL_PTR_SIZE 64
#define GLOBAL_PTR_ABI_ALIGN 64
#define GLOBAL_PTR_PREF_ALIGN 64

#include "llvmGlobalToWide.h"
#include "llvmAggregateGlobalOps.h"

// TODO - add functionality to clang so that we don't
// have to have what are basically copies of
// ModuleBuilder.cpp
// ( and BackendUtil.cpp but we used PassManagerBuilder::addGlobalExtension)
//
// This one is not normally included by clang clients
// and not normally installed in the include directory.
//
// Q. Could we instead call methods on clang::CodeGenerator subclass of
// ASTConsumer such as HandleTopLevelDecl to achieve what we want?
// We would have a different AST visitor for populating the LVT.
//
// It is likely that we can leave the C parser "open" somehow and then
// add statements to it at the end.
// BUT we couldn't call EmitDeferredDecl.
//
//
#include "CodeGenModule.h"
#include "CGRecordLayout.h"
#include "CGDebugInfo.h"
#include "clang/CodeGen/BackendUtil.h"

static void setupForGlobalToWide();

fileinfo    gAllExternCode;
fileinfo    gChplCompilationConfig;

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
void setupClangContext(GenInfo* info, ASTContext* Ctx)
{
  std::string layout;

  info->Ctx = Ctx;
  if( ! info->parseOnly ) {
    info->module->setTargetTriple(
        info->Ctx->getTargetInfo().getTriple().getTriple());

    // Also setup some basic TBAA metadata nodes.
    llvm::LLVMContext& cx = info->module->getContext();
    // Create the TBAA root node
    {
      LLVM_METADATA_OPERAND_TYPE* Ops[1];
      Ops[0] = llvm::MDString::get(cx, "Chapel types");
      info->tbaaRootNode = llvm::MDNode::get(cx, Ops);
    }
  }

  info->targetLayout = info->Ctx->getTargetInfo().getTargetDescription();
  layout = info->targetLayout;

  if( fLLVMWideOpt && ! info->parseOnly ) {
    char buf[200]; //needs to store up to 8 32-bit numbers in decimal

    assert(GLOBAL_PTR_SIZE == GLOBAL_PTR_BITS);

    // Add global pointer info to layout.
    snprintf(buf, sizeof(buf), "-p%u:%u:%u:%u-p%u:%u:%u:%u", GLOBAL_PTR_SPACE, GLOBAL_PTR_SIZE, GLOBAL_PTR_ABI_ALIGN, GLOBAL_PTR_PREF_ALIGN, WIDE_PTR_SPACE, GLOBAL_PTR_SIZE, GLOBAL_PTR_ABI_ALIGN, GLOBAL_PTR_PREF_ALIGN);
    layout += buf;
    // Save the global address space we are using in info.
    info->globalToWideInfo.globalSpace = GLOBAL_PTR_SPACE;
    info->globalToWideInfo.wideSpace = WIDE_PTR_SPACE;
  }
  // Always set the module layout. This works around an apparent bug in
  // clang or LLVM (trivial/deitz/test_array_low.chpl would print out the
  // wrong answer  because some i64s were stored at the wrong alignment).
  if( info->module ) info->module->setDataLayout(layout);

  info->targetData =
    new LLVM_TARGET_DATA(info->Ctx->getTargetInfo().getTargetDescription());
  if( ! info->parseOnly ) {
    info->cgBuilder = new CodeGen::CodeGenModule(*Ctx,
#if HAVE_LLVM_VER >= 37
        info->Clang->getHeaderSearchOpts(),
        info->Clang->getPreprocessorOpts(),
#endif
                              info->codegenOptions,
                              *info->module,
                              *info->targetData, *info->Diags);
  }


  // Set up some  constants that depend on the Clang context.
  {
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


// Adds a mapping from id->getName() to a variable or CDecl to info->lvt
static
void handleMacro(const IdentifierInfo* id, const MacroInfo* macro)
{
  GenInfo* info = gGenInfo;
  Preprocessor &preproc = info->Clang->getPreprocessor();
  VarSymbol* varRet = NULL;
  TypeDecl* cTypeRet = NULL;
  ValueDecl* cValueRet = NULL;

  const bool debugPrint = false;

  if( debugPrint) printf("Adding macro %s\n", id->getName().str().c_str());

  //Handling only simple string or integer defines
  if(macro->getNumArgs() > 0) {
    if( debugPrint) {
      printf("the macro takes arguments\n");
    }
    return; // TODO -- handle macro functions.
  }

  // Check that we have a single token surrounded by any
  // number of parens. ie 1, (1), ((1))
  Token tok; // the main token.
  size_t left_parens = 0;
  size_t right_parens = 0;
  ssize_t ntokens = macro->getNumTokens();
  ssize_t t_idx;
  bool negate = false;
  if( ntokens > 0 ) {
      MacroInfo::tokens_iterator ti = macro->tokens_end() - 1;
      for( t_idx = ntokens - 1; t_idx >= 0; t_idx-- ) {
        tok = *ti;
        if(tok.getKind() == tok::r_paren) right_parens++;
        else break;
        --ti;
      }
  }

  {
    MacroInfo::tokens_iterator ti = macro->tokens_begin();
    for( t_idx = 0; t_idx < ntokens; t_idx++ ) {
      tok = *ti;
      if(tok.getKind() == tok::l_paren) left_parens++;
      else if(tok.getKind() == tok::minus) {
        negate = true;
        ntokens--;
      } else break;
      ++ti;
    }
  }
  if( left_parens == right_parens &&
      ntokens - left_parens - right_parens == 1 ) {
    // OK!
  } else {
    if( debugPrint) {
      printf("the following macro is too complicated or empty:\n");
    }
    return; // we don't handle complicated expressions like A+B
  }


  switch(tok.getKind()) {
    case tok::numeric_constant: {
      std::string numString;
      int hex;
      int isfloat;
      if( negate ) numString.append("-");

      if (tok.getLiteralData() && tok.getLength()) {
        numString.append(tok.getLiteralData(), tok.getLength());
      }

      if( debugPrint) printf("num = %s\n", numString.c_str());

      hex = 0;
      if( numString[0] == '0' && (numString[1] == 'x' || numString[1] == 'X'))
      {
        hex = 1;
      }

      isfloat = 0;
      if(numString.find('.') != std::string::npos) {
        isfloat = 1;
      }
      // also check for exponent since e.g. 1e10 is a float.
      if( hex ) {
        // C99 hex floats use p for exponent
        if(numString.find('p') != std::string::npos ||
           numString.find('P') != std::string::npos) {
          isfloat = 1;
        }
      } else {
        if(numString.find('e') != std::string::npos ||
           numString.find('E') != std::string::npos) {
          isfloat = 1;
        }
      }

      if( !isfloat ) {
        IF1_int_type size = INT_SIZE_32;

        if(tolower(numString[numString.length() - 1]) == 'l') {
          numString[numString.length() - 1] = '\0';
          size = INT_SIZE_64;
        }

        if(tolower(numString[numString.length() - 1]) == 'u') {
          numString[numString.length() - 1] = '\0';
          varRet = new_UIntSymbol(strtoul(numString.c_str(), NULL, 0), size);
        }
        else {
          varRet = new_IntSymbol(strtol(numString.c_str(), NULL, 0), size);
        }
      }
      else {
        IF1_float_type size = FLOAT_SIZE_64;

        if(tolower(numString[numString.length() - 1]) == 'l') {
          numString[numString.length() - 1] = '\0';
        }

        varRet = new_RealSymbol(numString.c_str(), size);
      }
      break;
    }
    case tok::string_literal: {
      std::string body = std::string(tok.getLiteralData(), tok.getLength());
      if( debugPrint) printf("str = %s\n", body.c_str());
      varRet = new_CStringSymbol(body.c_str());
      break;
    }
    case tok::identifier: {
      IdentifierInfo* tokId = tok.getIdentifierInfo();
      std::string idName = tokId->getName();
      if( debugPrint) {
        printf("id = %s\n", idName.c_str());
      }

      // Handle the case where the macro refers to something we've
      // already parsed in C
      varRet = info->lvt->getVarSymbol(idName);
      if( !varRet ) {
        info->lvt->getCDecl(idName, &cTypeRet, &cValueRet);
      }
      if( !varRet && !cTypeRet && !cValueRet ) {
        // Check to see if it's another macro.
        MacroInfo* otherMacro = preproc.getMacroInfo(tokId);
        if( otherMacro && otherMacro != macro ) {
          // Handle the other macro to add it to the LVT under the new name
          // The recursive call will add it to the LVT
          if( debugPrint) printf("other macro\n");
          handleMacro(tokId, otherMacro);
          // Get whatever was added in the recursive call
          // so that we can add it under the new name.
          varRet = info->lvt->getVarSymbol(idName);
          info->lvt->getCDecl(idName, &cTypeRet, &cValueRet);
        }
      }
      if( debugPrint ) {
        if( varRet ) printf("found var %s\n", varRet->cname);
        if( cTypeRet ) {
          std::string s = cTypeRet->getName();
          printf("found cdecl type %s\n", s.c_str());
        }
        if( cValueRet ) {
          std::string s = cValueRet->getName();
          printf("found cdecl value %s\n", s.c_str());
        }
      }
      break;
    }
    default:
      break;
  }

  if( debugPrint ) {
    std::string s = id->getName();
    const char* kind = NULL;
    if( varRet ) kind = "var";
    if( cTypeRet ) kind = "cdecl type";
    if( cValueRet ) kind = "cdecl value";
    if( kind ) printf("%s: adding an %s to the lvt\n", s.c_str(), kind);
  }
  if( varRet ) {
    info->lvt->addGlobalVarSymbol(id->getName(), varRet);
  }
  if( cTypeRet ) {
    info->lvt->addGlobalCDecl(id->getName(), cTypeRet);
  }
  if( cValueRet ) {
    info->lvt->addGlobalCDecl(id->getName(), cValueRet);
  }

}




static
void readMacrosClang(void) {
  GenInfo* info = gGenInfo;
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
  Preprocessor &preproc = info->Clang->getPreprocessor();
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

#if HAVE_LLVM_VER >= 37
    handleMacro(i->first, i->second.getLatest()->getMacroInfo());
#elif HAVE_LLVM_VER >= 33
    handleMacro(i->first, i->second->getMacroInfo());
#else
    handleMacro(i->first, i->second);
#endif
  }
};

// We need a way to:
// 1: parse code only
// 2: keep the code generator open until we finish generating Chapel code,
//    since we might need to code generate called functions.
// 3: append to the target description
// 4: get LLVM values for code generated C things (e.g. types, function ptrs)
//
// This code is boiler-plate code mostly copied from ModuleBuilder.cpp - see
// http://clang.llvm.org/doxygen/ModuleBuilder_8cpp_source.html
// Note that ModuleBuilder.cpp is from the clang project and distributed
// under a BSD-like license.
//
// As far as we know, there is no public API for clang that
// would allow us the level of control we need over code generation.
// The portions that are not copied are delineated by
// comments indicating that they are custom to Chapel.
class CCodeGenConsumer : public ASTConsumer {
  private:
    GenInfo* info;
    unsigned HandlingTopLevelDecls;
    SmallVector<CXXMethodDecl *, 8> DeferredInlineMethodDefinitions;

    struct HandlingTopLevelDeclRAII {
      CCodeGenConsumer &Self;
      HandlingTopLevelDeclRAII(CCodeGenConsumer &Self) : Self(Self) {
        ++Self.HandlingTopLevelDecls;
      }
      ~HandlingTopLevelDeclRAII() {
        if (--Self.HandlingTopLevelDecls == 0)
          Self.EmitDeferredDecls();
      }
    };
  public:
    CCodeGenConsumer()
      : ASTConsumer(), info(gGenInfo), HandlingTopLevelDecls(0) {
    }

    virtual ~CCodeGenConsumer() { }

    // these macros help us to copy and paste the code from ModuleBuilder.
#define Ctx (info->Ctx)
#define Diags (* info->Diags)
#define Builder (info->cgBuilder)
#define CodeGenOpts (info->codegenOptions)

    // mostly taken from ModuleBuilder.cpp

     /// ASTConsumer override:
     // Initialize - This is called to initialize the consumer, providing
     // the ASTContext.
    virtual void Initialize(ASTContext &Context) LLVM_CXX_OVERRIDE {
      // This does setTargetTriple, setDataLayout, initialize targetData
      // and cgBuilder.
      setupClangContext(info, &Context);

      for (size_t i = 0, e = CodeGenOpts.DependentLibraries.size(); i < e; ++i)
        HandleDependentLibrary(CodeGenOpts.DependentLibraries[i]);
    }

    // ASTConsumer override:
    // HandleCXXStaticMemberVarInstantiation - Tell the consumer that
    // this variable has been instantiated.
    virtual void HandleCXXStaticMemberVarInstantiation(VarDecl *VD) LLVM_CXX_OVERRIDE {
      // Custom to Chapel
      if( info->parseOnly ) return;
      // End custom to Chapel

      if (Diags.hasErrorOccurred())
        return;

      Builder->HandleCXXStaticMemberVarInstantiation(VD);
    }

    // ASTConsumer override:
    // 
    // HandleTopLevelDecl - Handle the specified top-level declaration.
    // This is called by the parser to process every top-level Decl*.
    //
    // \returns true to continue parsing, or false to abort parsing.
    virtual bool HandleTopLevelDecl(DeclGroupRef DG) LLVM_CXX_OVERRIDE {
       if (Diags.hasErrorOccurred())
         return true;

        HandlingTopLevelDeclRAII HandlingDecl(*this);

      // Make sure to emit all elements of a Decl.
      for (DeclGroupRef::iterator I = DG.begin(), E = DG.end(); I != E; ++I) {
        // Custom to Chapel
        if(TypedefDecl *td = dyn_cast<TypedefDecl>(*I)) {
          const clang::Type *ctype= td->getUnderlyingType().getTypePtrOrNull();
          //printf("Adding typedef %s\n", td->getNameAsString().c_str());
          if(ctype != NULL) {
            info->lvt->addGlobalCDecl(td);
          }
        } else if(FunctionDecl *fd = dyn_cast<FunctionDecl>(*I)) {
          info->lvt->addGlobalCDecl(fd);
        } else if(VarDecl *vd = dyn_cast<VarDecl>(*I)) {
          info->lvt->addGlobalCDecl(vd);
        } else if(clang::RecordDecl *rd = dyn_cast<RecordDecl>(*I)) {
          if( rd->getName().size() > 0 ) {
            // Handle forward declaration for structs
            info->lvt->addGlobalCDecl(rd);
          }
        }
        if( info->parseOnly ) continue;
        // End custom to Chapel

        Builder->EmitTopLevelDecl(*I);
      }

      return true;
    }

    // ModuleBuilder.cpp has EmitDeferredDecls but that's not in ASTConsumer.
    void EmitDeferredDecls() {
       if (DeferredInlineMethodDefinitions.empty())
         return;

       // Emit any deferred inline method definitions. Note that more deferred
       // methods may be added during this loop, since ASTConsumer callbacks
       // can be invoked if AST inspection results in declarations being added.
       HandlingTopLevelDeclRAII HandlingDecl(*this);
       for (unsigned I = 0; I != DeferredInlineMethodDefinitions.size(); ++I)
         Builder->EmitTopLevelDecl(DeferredInlineMethodDefinitions[I]);
       DeferredInlineMethodDefinitions.clear();
    }

   // ASTConsumer override:
   // \brief This callback is invoked each time an inline method
   // definition is completed.
   virtual void HandleInlineMethodDefinition(CXXMethodDecl *D) LLVM_CXX_OVERRIDE {
      if (Diags.hasErrorOccurred())
        return;

      assert(D->doesThisDeclarationHaveABody());

      // We may want to emit this definition. However, that decision might be
      // based on computing the linkage, and we have to defer that in case we
      // are inside of something that will change the method's final linkage,
      // e.g.
      //   typedef struct {
      //     void bar();
      //     void foo() { bar(); }
      //   } A;
      DeferredInlineMethodDefinitions.push_back(D);

      // Provide some coverage mapping even for methods that aren't emitted.
      // Don't do this for templated classes though, as they may not be
      // instantiable.
      if (!D->getParent()->getDescribedClassTemplate())
        Builder->AddDeferredUnusedCoverageMapping(D);
    }

     // skipped ASTConsumer HandleInterestingDecl
     // HandleTagDeclRequiredDefinition
     // HandleCXXImplicitFunctionInstantiation
     // HandleTopLevelDeclInObjCContainer
     // HandleImplicitImportDecl
     // GetASTMutationListener
     // GetASTDeserializationListener
     // PrintStats
     // shouldSkipFunctionBody

    // ASTConsumer override:
    // HandleTagDeclDefinition - This callback is invoked each time a TagDecl
    // to (e.g. struct, union, enum, class) is completed. This allows the
    // client hack on the type, which can occur at any point in the file
    // (because these can be defined in declspecs).
    virtual void HandleTagDeclDefinition(TagDecl *D) LLVM_CXX_OVERRIDE {
      if (Diags.hasErrorOccurred())
        return;

      // Custom to Chapel - make a note of C globals
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
      if( info->parseOnly ) return;
      // End Custom to Chapel

      Builder->UpdateCompletedType(D);

      // For MSVC compatibility, treat declarations of static data members with
      // inline initializers as definitions.
      if (Ctx->getLangOpts().MSVCCompat) {
        for (Decl *Member : D->decls()) {
          if (VarDecl *VD = dyn_cast<VarDecl>(Member)) {
            if (Ctx->isMSStaticDataMemberInlineDefinition(VD) &&
                Ctx->DeclMustBeEmitted(VD)) {
              Builder->EmitGlobal(VD);
            }
          }
        }
      }
    }

    // ASTConsumer override:
    // \brief This callback is invoked the first time each TagDecl is required
    // to be complete.
    virtual void HandleTagDeclRequiredDefinition(const TagDecl *D) LLVM_CXX_OVERRIDE {
      if (Diags.hasErrorOccurred())
        return;

      if( info->parseOnly ) return;

      if (CodeGen::CGDebugInfo *DI = Builder->getModuleDebugInfo())
        if (const RecordDecl *RD = dyn_cast<RecordDecl>(D))
          DI->completeRequiredType(RD);
    }


     // ASTConsumer override:
     // HandleTranslationUnit - This method is called when the ASTs for
     // entire translation unit have been parsed.
     virtual void HandleTranslationUnit(ASTContext &Context) LLVM_CXX_OVERRIDE {
       if (Diags.hasErrorOccurred()) {
         if(Builder)
           Builder->clear();
         return;
       }

       /* custom to Chapel -
          we don't release the builder now, because
          we want to add a bunch of uses of functions
          that may not have been codegened yet.

          Instead, we call this in cleanupClang.
       if (Builder)
         Builder->Release();
       */
     }

     // ASTConsumer override:
     //
     // CompleteTentativeDefinition - Callback invoked at the end of a
     // translation unit to notify the consumer that the given tentative
     // definition should be completed.
     //
     // The variable declaration
     // itself will be a tentative definition. If it had an incomplete
     // array type, its type will have already been changed to an array
     // of size 1. However, the declaration remains a tentative
     // definition and has not been modified by the introduction of an
     // implicit zero initializer.
     virtual void CompleteTentativeDefinition(VarDecl *D) LLVM_CXX_OVERRIDE {
       if (Diags.hasErrorOccurred())
         return;

       // Custom to Chapel
       if( info->parseOnly ) return;
       // End Custom to Chapel
       
       Builder->EmitTentativeDefinition(D);
     }

     // ASTConsumer override:
     // \brief Callback involved at the end of a translation unit to
     // notify the consumer that a vtable for the given C++ class is
     // required.
     //
     // \param RD The class whose vtable was used.
     virtual void HandleVTable(CXXRecordDecl *RD
#if HAVE_LLVM_VER < 37
         , bool DefinitionRequired
#endif
         ) LLVM_CXX_OVERRIDE {
       if (Diags.hasErrorOccurred())
         return;

       // Custom to Chapel
       if( info->parseOnly ) return;
       // End Custom to Chapel

       Builder->EmitVTable(RD
#if HAVE_LLVM_VER < 37
           , DefinitionRequired
#endif
           );
     }
     
     // ASTConsumer override:
     //
     // \brief Handle a pragma that appends to Linker Options.  Currently
     // this only exists to support Microsoft's #pragma comment(linker,
     // "/foo").
     void HandleLinkerOptionPragma(llvm::StringRef Opts) override {
       Builder->AppendLinkerOptions(Opts);
     }

     // HandleLinkerOptionPragma
     // ASTConsumer override:
     // \brief Handle a pragma that emits a mismatch identifier and value to
     // the object file for the linker to work with.  Currently, this only
     // exists to support Microsoft's #pragma detect_mismatch.
     virtual void HandleDetectMismatch(llvm::StringRef Name,
                                               llvm::StringRef Value) LLVM_CXX_OVERRIDE {
       Builder->AddDetectMismatch(Name, Value);
     }

     // ASTConsumer override:
     // \brief Handle a dependent library created by a pragma in the source.
     /// Currently this only exists to support Microsoft's
     /// #pragma comment(lib, "/foo").
     virtual void HandleDependentLibrary(llvm::StringRef Lib) LLVM_CXX_OVERRIDE {
       Builder->AddDependentLib(Lib);
     }

    // undefine macros we created to help with ModuleBuilder
#undef Ctx
#undef Diags
#undef Builder
#undef CodeGenOpts

};


#if HAVE_LLVM_VER >= 36
#define CREATE_AST_CONSUMER_RETURN_TYPE std::unique_ptr<ASTConsumer>
#else
#define CREATE_AST_CONSUMER_RETURN_TYPE ASTConsumer*
#endif

class CCodeGenAction : public ASTFrontendAction {
 public:
  CCodeGenAction() { }
 protected:
  virtual CREATE_AST_CONSUMER_RETURN_TYPE CreateASTConsumer(
      CompilerInstance &CI, StringRef InFile);
};

CREATE_AST_CONSUMER_RETURN_TYPE CCodeGenAction::CreateASTConsumer(
    CompilerInstance &CI, StringRef InFile) {
#if HAVE_LLVM_VER >= 36
  return std::unique_ptr<ASTConsumer>(new CCodeGenConsumer());
#else
  return new CCodeGenConsumer();
#endif
};

static void finishClang(GenInfo* info){
  if( info->cgBuilder ) {
    info->cgBuilder->Release();
  }
  info->Diags.reset();
  info->DiagID.reset();
}

static void deleteClang(GenInfo* info){
  if( info->cgBuilder ) {
    delete info->cgBuilder;
    info->cgBuilder = NULL;
  }
  delete info->targetData;
  delete info->Clang;
  info->Clang = NULL;
  delete info->cgAction;
  info->cgAction = NULL;
}

static void cleanupClang(GenInfo* info)
{
  finishClang(info);
  deleteClang(info);
}

void setupClang(GenInfo* info, std::string mainFile)
{
  std::string clangexe = info->clangCC;
  std::vector<const char*> clangArgs;
  for( size_t i = 0; i < info->clangCCArgs.size(); ++i ) {
    clangArgs.push_back(info->clangCCArgs[i].c_str());
  }
  for( size_t i = 0; i < info->clangLDArgs.size(); ++i ) {
    clangArgs.push_back(info->clangLDArgs[i].c_str());
  }
  for( size_t i = 0; i < info->clangOtherArgs.size(); ++i ) {
    clangArgs.push_back(info->clangOtherArgs[i].c_str());
  }

  if (llvmCodegen) {
    clangArgs.push_back("-emit-llvm");
  }

  //clangArgs.push_back("-c");
  clangArgs.push_back(mainFile.c_str()); // chpl - always compile rt file

  info->diagOptions = new DiagnosticOptions();
  info->DiagClient= new TextDiagnosticPrinter(errs(),&*info->diagOptions);
  info->DiagID = new DiagnosticIDs();
#if HAVE_LLVM_VER >= 32
  info->Diags = new DiagnosticsEngine(
      info->DiagID, &*info->diagOptions, info->DiagClient);
#else
  info->Diags = new DiagnosticsEngine(info->DiagID, info->DiagClient);
#endif

  CompilerInvocation* CI =
    createInvocationFromCommandLine(clangArgs, info->Diags);

  // Get the codegen options from the clang command line.
  info->codegenOptions = CI->getCodeGenOpts();

  // if --fast is given, we should be at least at -O3.
  if(fFastFlag && info->codegenOptions.OptimizationLevel < 3) {
    info->codegenOptions.OptimizationLevel = 3;
  }

  {
    // Make sure we include clang's internal header dir
#if HAVE_LLVM_VER >= 34
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
    CI->getHeaderSearchOpts().ResourceDir = P.str();
    sys::path::append(P, "include");
#else
    sys::Path P(clangexe);
    if (!P.isEmpty()) {
      P.eraseComponent();  // Remove /clang from foo/bin/clang
      P.eraseComponent();  // Remove /bin   from foo/bin

      // Get foo/lib/clang/<version>/
      P.appendComponent("lib");
      P.appendComponent("clang");
      P.appendComponent(CLANG_VERSION_STRING);
    }
    CI->getHeaderSearchOpts().ResourceDir = P.str();
    sys::Path P2(P);
    P.appendComponent("include");
#endif
#if HAVE_LLVM_VER >= 33
    CI->getHeaderSearchOpts().AddPath(
        P.str(), frontend::System,false, false);
#else
    CI->getHeaderSearchOpts().AddPath(
        P.str(), frontend::System,false, false, false, true, false);
#endif
  }

  // Create a compiler instance to handle the actual work.
  info->Clang = new CompilerInstance();
  info->Clang->setInvocation(CI);

  // Save the TargetOptions and LangOptions since these
  // are used during machine code generation.
  info->clangTargetOptions = info->Clang->getTargetOpts();
  info->clangLangOptions = info->Clang->getLangOpts();

  // Create the compilers actual diagnostics engine.
  // Create the compilers actual diagnostics engine.
#if HAVE_LLVM_VER >= 33
  info->Clang->createDiagnostics();
#else
  info->Clang->createDiagnostics(int(clangArgs.size()),&clangArgs[0]);
#endif
  if (!info->Clang->hasDiagnostics())
    INT_FATAL("Bad diagnostics from clang");
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
  finishClang(info);

  if(debug_info)debug_info->finalize();

  // Verify the LLVM module.
  if( developer ) {
    bool problems;
#if HAVE_LLVM_VER >= 35
    problems = verifyModule(*info->module, &errs());
    //problems = false;
#else
    problems = verifyModule(*info->module, PrintMessageAction);
#endif
    if(problems) {
      INT_FATAL("LLVM module verification failed");
    }
  }
}

void prepareCodegenLLVM()
{
  GenInfo *info = gGenInfo;

  LEGACY_FUNCTION_PASS_MANAGER *fpm = new LEGACY_FUNCTION_PASS_MANAGER(info->module);

  PassManagerBuilder PMBuilder;

  // Set up the optimizer pipeline.
  // Start with registering info about how the
  // target lays out data structures.
#if HAVE_LLVM_VER >= 37
  // We already set the data layout in setupClangContext
  // don't need to do anything else.
#elif HAVE_LLVM_VER >= 36
  // We already set the data layout in setupClangContext
  fpm->add(new DataLayoutPass());
#elif HAVE_LLVM_VER >= 35
  fpm->add(new DataLayoutPass(info->module));
#else
  fpm->add(new DataLayout(info->module));
#endif

  if( fFastFlag ) {
    PMBuilder.OptLevel = 2;
    PMBuilder.populateFunctionPassManager(*fpm);
  }

  info->FPM_postgen = fpm;

  info->FPM_postgen->doInitialization();
}

#if HAVE_LLVM_VER >= 33
static void handleErrorLLVM(void* user_data, const std::string& reason,
                            bool gen_crash_diag)
#else
static void handleErrorLLVM(void* user_data, const std::string& reason)
#endif
{
  INT_FATAL("llvm fatal error: %s", reason.c_str());
}

struct ExternBlockInfo {
  GenInfo* gen_info;
  fileinfo file;
  ExternBlockInfo() : gen_info(NULL), file() { }
  ~ExternBlockInfo() { }
};

typedef std::set<ModuleSymbol*> module_set_t;
typedef module_set_t::iterator module_set_iterator_t;

module_set_t gModulesWithExternBlocks;

bool lookupInExternBlock(ModuleSymbol* module, const char* name,
                         clang::TypeDecl** cTypeOut,
                         clang::ValueDecl** cValueOut,
                         ChapelType** chplTypeOut)
{
  if( ! module->extern_info ) return false;
  module->extern_info->gen_info->lvt->getCDecl(name, cTypeOut, cValueOut);
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

  /* TODO -- note that clang/examples/clang-interpreter/main.cpp 
             includes an example for getting the executable path,
             so that we could automatically set CHPL_HOME. */
  std::string home(CHPL_HOME);
  std::string compileline = home + "/util/config/compileline";
  compileline += " COMP_GEN_WARN="; compileline += istr(ccwarnings);
  compileline += " COMP_GEN_DEBUG="; compileline += istr(debugCCode);
  compileline += " COMP_GEN_OPT="; compileline += istr(optimizeCCode);
  compileline += " COMP_GEN_SPECIALIZE="; compileline += istr(specializeCCode);
  compileline += " COMP_GEN_FLOAT_OPT="; compileline += istr(ffloatOpt);

  std::string readargsfrom;

  if( !llvmCodegen && just_parse_filename ) {
    // We're handling an extern block and not using the LLVM backend.
    // Don't ask for any compiler-specific C flags.
    readargsfrom = compileline + " --llvm"
                                 " --clang"
                                 " --clang-sysroot-arguments"
                                 " --includes-and-defines";
  } else {
    // We're parsing extern blocks AND any parts of the runtime
    // in order to prepare for an --llvm compilation.
    // Use compiler-specific flags for clang-included.
    readargsfrom = compileline + " --llvm"
                                 " --clang"
                                 " --clang-sysroot-arguments"
                                 " --cflags"
                                 " --includes-and-defines";
  }
  std::vector<std::string> args;
  std::vector<std::string> clangCCArgs;
  std::vector<std::string> clangLDArgs;
  std::vector<std::string> clangOtherArgs;
  std::string clangCC, clangCXX;

  // Gather information from readargsfrom into clangArgs.
  readArgsFromCommand(readargsfrom.c_str(), args);
  if( args.size() < 2 ) USR_FATAL("Could not find runtime dependencies for --llvm build");

  clangCC = args[0];
  clangCXX = args[1];

  // Note that these CC arguments will be saved in info->clangCCArgs
  // and will be used when compiling C files as well.
  for( size_t i = 2; i < args.size(); ++i ) {
    clangCCArgs.push_back(args[i]);
  }
 
  forv_Vec(const char*, dirName, incDirs) {
    clangCCArgs.push_back(std::string("-I") + dirName);
  }
  clangCCArgs.push_back(std::string("-I") + getIntermediateDirName());

  //split ccflags by spaces
  std::stringstream ccArgsStream(ccflags);
  std::string ccArg;
  while(ccArgsStream >> ccArg)
      clangCCArgs.push_back(ccArg);

  clangCCArgs.push_back("-pthread");

  // libFlag and ldflags are handled during linking later.

  clangCCArgs.push_back("-DCHPL_GEN_CODE");

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
    // Include extern C blocks
    if( externC && gAllExternCode.filename ) {
      clangOtherArgs.push_back("-include");
      clangOtherArgs.push_back(gAllExternCode.filename);
    }
  } else {
    // Just running clang to parse the extern blocks for this module.
    clangOtherArgs.push_back("-include");
    clangOtherArgs.push_back(just_parse_filename);
  }

  if( printSystemCommands ) {
    printf("<internal clang> ");
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
  gGenInfo = new GenInfo(clangCC, clangCXX,
                         compileline, clangCCArgs, clangLDArgs, clangOtherArgs,
                         just_parse_filename != NULL);

  if( llvmCodegen || externC )
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
    info->cgAction = new CCodeGenAction();
    if (!info->Clang->ExecuteAction(*info->cgAction)) {
      if (just_parse_filename) {
        USR_FATAL("error running clang on extern block");
      } else {
        USR_FATAL("error running clang during code generation");
      }
    }

    if( ! info->parseOnly ) {
      // This seems to be needed, even though it is strange.
      // (otherwise we segfault in info->builder->CreateGlobalString)
      
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
      info->builder->SetInsertPoint(block);
    }
    // read macros. May call IRBuilder methods to codegen a string,
    // so needs to happen after we set the insert point.
    readMacrosClang();

    if( ! info->parseOnly ) {
      info->builder->CreateRetVoid();
    }
  }
}

static
void saveExternBlock(ModuleSymbol* module, const char* extern_code)
{
  if( ! gAllExternCode.filename ) {
    openCFile(&gAllExternCode, "extern-code", "c");
    INT_ASSERT(gAllExternCode.fptr);

    // Allow code in extern block to use malloc/calloc/realloc/free
    // Note though that e.g. strdup or other library routines that
    // allocate memory might still be an issue...
    fprintf(gAllExternCode.fptr, "#include \"chpl-mem-no-warning-macros.h\"\n");
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
    cleanupClang(module->extern_info->gen_info);
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

  return NULL;
}

llvm::Type* getTypeLLVM(const char* name)
{
  GenInfo* info = gGenInfo;
  llvm::Type* t = info->module->getTypeByName(name);
  if( t ) return t;

  t = info->lvt->getType(name);
  if( t ) return t;

  return NULL;
}
// should support TypedefDecl,EnumDecl,RecordDecl
llvm::Type* codegenCType(const TypeDecl* td)
{
  GenInfo* info = gGenInfo;
  CodeGen::CodeGenTypes & cdt = info->cgBuilder->getTypes();
  QualType qType;

  // handle TypedefDecl
  if( const TypedefNameDecl* tnd = dyn_cast<TypedefNameDecl>(td) ) {
    qType = tnd->getCanonicalDecl()->getUnderlyingType();
    // had const Type *ctype = td->getUnderlyingType().getTypePtrOrNull();
    //could also do:
    //  qType =
    //   tnd->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
  } else if( const EnumDecl* ed = dyn_cast<EnumDecl>(td) ) {
    qType = ed->getCanonicalDecl()->getIntegerType();
    // could also use getPromotionType()
    //could also do:
    //  qType =
    //   tnd->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
  } else if( const RecordDecl* rd = dyn_cast<RecordDecl>(td) ) {
    RecordDecl *def = rd->getDefinition();
    INT_ASSERT(def);
    qType=def->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
  } else {
    INT_FATAL("Unknown clang type declaration");
  }
  return cdt.ConvertTypeForMem(qType);
}

// should support FunctionDecl,VarDecl,EnumConstantDecl
GenRet codegenCValue(const ValueDecl *vd)
{
  GenInfo* info = gGenInfo;
  GenRet ret;

  if( info->cfile ) {
    ret.c = vd->getName();
    return ret;
  }

  if(const FunctionDecl *fd = dyn_cast<FunctionDecl>(vd)) {
    // It's a function decl.
    ret.val = info->cgBuilder->GetAddrOfFunction(fd);
    ret.isLVPtr = GEN_VAL;
  } else if(const VarDecl *vard = dyn_cast<VarDecl>(vd)) {
    // It's a (global) variable decl
    ret.val = info->cgBuilder->GetAddrOfGlobalVar(vard);
    ret.isLVPtr = GEN_PTR;
  } else if(const EnumConstantDecl *ed = dyn_cast<EnumConstantDecl>(vd)) {
    // It's a constant enum value
    APInt v = ed->getInitVal();

    ret.isUnsigned = ! ed->getType()->hasSignedIntegerRepresentation();
    CodeGen::CodeGenTypes & cdt = info->cgBuilder->getTypes();
    llvm::Type* type = cdt.ConvertTypeForMem(ed->getType());
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

void LayeredValueTable::addGlobalType(StringRef name, llvm::Type *type) {
  Storage store;
  store.u.type = type;
  /*fprintf(stderr, "Adding global type %s ", name.str().c_str());
  type->dump();
  fprintf(stderr, "\n");
  */
  (layers.back())[name] = store;
}

void LayeredValueTable::addGlobalCDecl(NamedDecl* cdecl) {
  addGlobalCDecl(cdecl->getName(), cdecl);

  // Also file structs under 'struct struct_name'
  if(isa<RecordDecl>(cdecl)) {
    std::string sname = "struct ";
    sname += cdecl->getName();
    addGlobalCDecl(sname, cdecl);
  }
}

void LayeredValueTable::addGlobalCDecl(StringRef name, NamedDecl* cdecl) {

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
  if (isa<ValueDecl>(cdecl)) store.u.cValueDecl = cast<ValueDecl>(cdecl);
}


void LayeredValueTable::addGlobalVarSymbol(llvm::StringRef name, VarSymbol* var)
{
  Storage store;
  store.u.chplVar = var;
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
      return codegenCValue(store->u.cValueDecl);
    }
    if( store->u.chplVar && isVarSymbol(store->u.chplVar) ) {
      VarSymbol* var = store->u.chplVar;
      GenRet ret = var; // code generate it!
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

llvm::Type *LayeredValueTable::getType(StringRef name) {
  if(Storage *store = get(name)) {
    if( store->u.type ) {
      INT_ASSERT(isa<llvm::Type>(store->u.type));
      return store->u.type;
    }
    if( store->u.cTypeDecl ) {
      INT_ASSERT(isa<TypeDecl>(store->u.cTypeDecl));
      // we have a clang type decl.
      // maybe TypedefDecl,EnumDecl,RecordDecl

      // Convert it to an LLVM type.
      return codegenCType(store->u.cTypeDecl);
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
    ValueDecl** cValueOut) {

  if (cValueOut) *cValueOut = NULL;
  if (cTypeOut) *cTypeOut = NULL;

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
  }
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

int getCRecordMemberGEP(const char* typeName, const char* fieldName)
{
  GenInfo* info = gGenInfo;
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
    INT_ASSERT(rt);
    d = rt->getDecl();
    // getAsUnionType also available, but we don't support extern unions
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
  INT_ASSERT(field);
  ret=info->cgBuilder->getTypes().getCGRecordLayout(rec).getLLVMFieldNo(field);
  return ret;
}

bool isBuiltinExternCFunction(const char* cname)
{
  if( 0 == strcmp(cname, "sizeof") ) return true;
  else return false;
}

static
void addAggregateGlobalOps(const PassManagerBuilder &Builder,
    LEGACY_PASS_MANAGER &PM) {
  GenInfo* info = gGenInfo;
  if( fLLVMWideOpt ) {
    PM.add(createAggregateGlobalOpsOptPass(info->globalToWideInfo.globalSpace));
  }
}

static
void addGlobalToWide(const PassManagerBuilder &Builder,
    LEGACY_PASS_MANAGER &PM) {
  GenInfo* info = gGenInfo;
  if( fLLVMWideOpt ) {
    PM.add(createGlobalToWide(&info->globalToWideInfo, info->targetLayout));
  }
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

  info->addrFn = getFunctionLLVM("chpl_wide_ptr_get_address");
  INT_ASSERT(info->addrFn);
  info->locFn = getFunctionLLVM("chpl_wide_ptr_read_localeID");
  INT_ASSERT(info->locFn);
  info->nodeFn = getFunctionLLVM("chpl_wide_ptr_get_node");
  INT_ASSERT(info->nodeFn);
  info->makeFn = getFunctionLLVM("chpl_return_wide_ptr_loc_ptr");
  INT_ASSERT(info->makeFn);
  info->getFn = getFunctionLLVM("chpl_gen_comm_get_ctl");
  INT_ASSERT(info->getFn);
  info->putFn = getFunctionLLVM("chpl_gen_comm_put_ctl");
  INT_ASSERT(info->putFn);
  info->getPutFn = getFunctionLLVM("chpl_gen_comm_getput");
  INT_ASSERT(info->getPutFn);
  info->memsetFn = getFunctionLLVM("chpl_gen_comm_memset");
  INT_ASSERT(info->memsetFn);

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
                          dummy, retType, argType, NULL);
  llvm::Function* fn = llvm::dyn_cast<llvm::Function>(fval);

  // Mark the function as external so that it will not be removed
  fn->setLinkage(llvm::GlobalValue::ExternalLinkage);

  llvm::BasicBlock* block = 
     llvm::BasicBlock::Create(ginfo->module->getContext(), "entry", fn);
  ginfo->builder->SetInsertPoint(block);

  llvm::Constant* fns[] = {info->addrFn, info->locFn, info->nodeFn,
                           info->makeFn, info->getFn, info->putFn,
                           info->getPutFn, info->memsetFn, NULL};

  llvm::Value* ret = llvm::Constant::getNullValue(retType);
  llvm::Function::arg_iterator args = fn->arg_begin();
  llvm::Value* arg = args++;

  for( int i = 0; fns[i]; i++ ) {
    llvm::Constant* f = fns[i];
    llvm::Value* ptr = ginfo->builder->CreatePointerCast(f, retType); 
    llvm::Value* id = llvm::ConstantInt::get(argType, i);
    llvm::Value* eq = ginfo->builder->CreateICmpEQ(arg, id);
    ret = ginfo->builder->CreateSelect(eq, ptr, ret);
  }
  ginfo->builder->CreateRet(ret);

#if HAVE_LLVM_VER >= 35
  llvm::verifyFunction(*fn, &errs());
#endif

  info->preservingFn = fn;
}


void makeBinaryLLVM(void) {
#if HAVE_LLVM_VER >= 36
  std::error_code errorInfo;
#else
  std::string errorInfo;
#endif

  GenInfo* info = gGenInfo;

  std::string moduleFilename = genIntermediateFilename("chpl__module.bc");
  std::string preOptFilename = genIntermediateFilename("chpl__module-nopt.bc");

  if( saveCDir[0] != '\0' ) {
    // Save the generated LLVM before optimization.
    tool_output_file output (preOptFilename.c_str(),
                             errorInfo,
#if HAVE_LLVM_VER >= 34
                             sys::fs::F_None
#else
                             raw_fd_ostream::F_Binary
#endif
                             );
    WriteBitcodeToFile(info->module, output.os());
    output.keep();
    output.os().flush();
  }

  tool_output_file output (moduleFilename.c_str(),
                           errorInfo,
#if HAVE_LLVM_VER >= 34
                             sys::fs::F_None
#else
                             raw_fd_ostream::F_Binary
#endif
                           );
 
  static bool addedGlobalExts = false;
  if( ! addedGlobalExts ) {
    // Add the Global to Wide optimization if necessary.
    PassManagerBuilder::addGlobalExtension(PassManagerBuilder::EP_ScalarOptimizerLate, addAggregateGlobalOps);
    PassManagerBuilder::addGlobalExtension(PassManagerBuilder::EP_ScalarOptimizerLate, addGlobalToWide);
    PassManagerBuilder::addGlobalExtension(PassManagerBuilder::EP_EnabledOnOptLevel0, addGlobalToWide);
    addedGlobalExts = true;
  }

  EmitBackendOutput(*info->Diags, info->codegenOptions,
                    info->clangTargetOptions, info->clangLangOptions,
#if HAVE_LLVM_VER >= 35
                    info->Ctx->getTargetInfo().getTargetDescription(),
#endif
                    info->module, Backend_EmitBC, &output.os());
  output.keep();
  output.os().flush();

  //finishClang is before the call to the debug finalize
  deleteClang(info);

  std::string options = "";

  std::string home(CHPL_HOME);
  std::string compileline = info->compileline;
  compileline += " --llvm"
                 " --clang"
                 " --main.o"
                 " --clang-sysroot-arguments"
                 " --libraries";
  std::vector<std::string> args;
  readArgsFromCommand(compileline.c_str(), args);
  INT_ASSERT(args.size() >= 1);

  std::string clangCC = args[0];
  std::string clangCXX = args[1];
  std::string maino = args[2];
  std::vector<std::string> dotOFiles;

  // Gather C flags for compiling C files.
  std::string cargs;
  for( size_t i = 0; i < info->clangCCArgs.size(); ++i ) {
    cargs += " ";
    cargs += info->clangCCArgs[i];
  }

  // Compile any C files.
  {
    // Start with configuration settings
    const char* inputFilename = gChplCompilationConfig.pathname;
    const char* objFilename = objectFileForCFile(inputFilename);

    mysystem(astr(clangCC.c_str(),
                  " -c -o ",
                  objFilename,
                  " ",
                  inputFilename,
                  cargs.c_str()),
               "Compile C File");

    dotOFiles.push_back(objFilename);
  }

  int filenum = 0;
  while (const char* inputFilename = nthFilename(filenum++)) {
    if (isCSource(inputFilename)) {
      const char* objFilename = objectFileForCFile(inputFilename);
      mysystem(astr(clangCC.c_str(),
                    " -c -o ", objFilename,
                    " ", inputFilename, cargs.c_str()),
               "Compile C File");
      dotOFiles.push_back(objFilename);
    } else if( isObjFile(inputFilename) ) {
      dotOFiles.push_back(inputFilename);
    }
  }

  // Start linker options with C args
  // This is important to get e.g. -O3 -march=native
  // since with LLVM we are doing link-time optimization.
  // We know it's OK to include -I (e.g.) since we're calling
  // clang++ to link so that it can optimize the .bc files.
  options = cargs;

  if(debugCCode) {
    options += " -g";
  }

  options += " ";
  options += ldflags;

  options += " -pthread";

  // Now, if we're doing a multilocale build, we have to make a launcher.
  // For this reason, we create a makefile. codegen_makefile
  // also gives us the name of the temporary place to save
  // the generated program.
  fileinfo mainfile;
  mainfile.filename = "chpl__module.bc";
  mainfile.pathname = moduleFilename.c_str();
  const char* tmpbinname = NULL;

  codegen_makefile(&mainfile, &tmpbinname, true);
  INT_ASSERT(tmpbinname);

  // Run the linker. We always use clang++ because some third-party
  // libraries are written in C++. With the C backend, this switcheroo
  // is accomplished in the Makefiles somewhere
  std::string command = clangCXX + " " + options + " " +
                        moduleFilename + " " + maino +
                        " -o " + tmpbinname;
  for( size_t i = 0; i < dotOFiles.size(); i++ ) {
    command += " ";
    command += dotOFiles[i];
  }

  // 0 is clang, 1 is clang++, 2 is main.o
  for(size_t i = 3; i < args.size(); ++i) {
    command += " ";
    command += args[i];
  }

  // Put user-requested libraries at the end of the compile line,
  // they should at least be after the .o files and should be in
  // order where libraries depend on libraries to their right.
  for (int i=0; i<numLibFlags; i++) {
    command += " ";
    command += libFlag[i];
  }

  if( printSystemCommands ) {
    printf("%s\n", command.c_str());
  }
  mysystem(command.c_str(), "Make Binary - Linking");

  // Now run the makefile to move from tmpbinname to the proper program
  // name and to build a launcher (if necessary).
  const char* makeflags = printSystemCommands ? "-f " : "-s -f ";
  const char* makecmd = astr(astr(CHPL_MAKE, " "),
                             makeflags,
                             getIntermediateDirName(), "/Makefile");

  if( printSystemCommands ) {
    printf("%s\n", makecmd);
  }

  mysystem(makecmd, "Make Binary - Building Launcher and Copying");
}

#endif
