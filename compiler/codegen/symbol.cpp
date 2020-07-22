/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "symbol.h"

#include "AstToText.h"
#include "bb.h"
#include "AstVisitor.h"
#include "astutil.h"
#include "build.h"
#include "clangUtil.h"
#include "codegen.h"
#include "CollapseBlocks.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "fixupExports.h"
#include "intlimits.h"
#include "iterator.h"
#include "LayeredValueTable.h"
#include "library.h"
#include "llvmDebug.h"
#include "llvmExtractIR.h"
#include "llvmUtil.h"
#include "misc.h"
#include "optimizations.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "type.h"
#include "resolution.h"
#include "wellknown.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include <inttypes.h>
#include <stdint.h>

#ifdef HAVE_LLVM
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Target/TargetIntrinsicInfo.h"
#include "clang/CodeGen/CGFunctionInfo.h"
#endif

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

// these are sets of astrs
static std::set<const char*> llvmPrintIrNames;
static std::set<const char*> llvmPrintIrCNames;

llvmStageNum_t llvmPrintIrStageNum = llvmStageNum::NOPRINT;

const char* llvmStageName[llvmStageNum::LAST] = {
  "", //llvmStageNum::NOPRINT
  "none", //llvmStageNum::NONE
  "basic", //llvmStageNum::BASIC
  "full", //llvmStageNum::FULL
  "every", //llvmStageNum::EVERY
  "early-as-possible",
  "module-optimizer-early",
  "loop-optimizer-end",
  "scalar-optimizer-late",
  "optimizer-last",
  "vectorizer-start",
  "enabled-on-opt-level0",
  "peephole",
};

const char *llvmStageNameFromLlvmStageNum(llvmStageNum_t stageNum) {
  if(stageNum < llvmStageNum::LAST)
    return llvmStageName[stageNum];
  else
    return NULL;
}

llvmStageNum_t llvmStageNumFromLlvmStageName(const char* stageName) {
  for(int i = 0; i < llvmStageNum::LAST; i++)
    if(strcmp(llvmStageName[i], stageName) == 0)
      return static_cast<llvmStageNum_t>(i);
  return llvmStageNum::NOPRINT;
}

void addNameToPrintLlvmIr(const char* name) {
  llvmPrintIrNames.insert(astr(name));
}
void addCNameToPrintLlvmIr(const char* name) {
  llvmPrintIrCNames.insert(astr(name));
}

bool shouldLlvmPrintIrName(const char* name) {
  if (llvmPrintIrNames.empty())
    return false;

  return llvmPrintIrNames.count(astr(name));
}

bool shouldLlvmPrintIrCName(const char* name) {
  if (llvmPrintIrNames.empty())
    return false;

  return llvmPrintIrCNames.count(astr(name));
}

bool shouldLlvmPrintIrFn(FnSymbol* fn) {
  return shouldLlvmPrintIrName(fn->name) || shouldLlvmPrintIrCName(fn->cname);
}

#ifdef HAVE_LLVM

static std::set<const llvm::GlobalValue*> funcsToPrint;
static llvmStageNum_t partlyPrintedStage = llvmStageNum::NOPRINT;

void printLlvmIr(const char* name, llvm::Function *func, llvmStageNum_t numStage) {
  if(func) {
    std::cout << "; " << "LLVM IR representation of " << name
              << " function after " << llvmStageNameFromLlvmStageNum(numStage)
              << " optimization stage\n" << std::flush;
    if (!(numStage == llvmStageNum::BASIC ||
          numStage == llvmStageNum::FULL)) {
      // Basic and full can happen module-at-a-time due to current
      // compiler structure. For the others, we can't save the Function*,
      // so just print out multiple modules if there are multiple functions.
      std::set<const llvm::GlobalValue*> funcs;
      funcs.insert(func);
      extractAndPrintFunctionsLLVM(&funcs);
    } else {
      funcsToPrint.insert(func);
      partlyPrintedStage = numStage;
    }
  }
}
#endif

void completePrintLlvmIrStage(llvmStageNum_t numStage) {
#ifdef HAVE_LLVM
  extractAndPrintFunctionsLLVM(&funcsToPrint);
  partlyPrintedStage = llvmStageNum_t::NOPRINT;
  funcsToPrint.clear();
#endif
}


void preparePrintLlvmIrForCodegen() {
  if (llvmPrintIrNames.empty() && llvmPrintIrCNames.empty())
    return;
  if (llvmPrintIrStageNum == llvmStageNum::NOPRINT)
    return;

  // Gather the cnames for the functions in names
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (shouldLlvmPrintIrFn(fn)) {
      addCNameToPrintLlvmIr(fn->cname);
    }
  }

  // Extend cnames with the cnames of task functions
  bool changed;
  do {
    changed = false;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (shouldLlvmPrintIrCName(fn->cname)) {
        std::vector<CallExpr*> calls;
        collectFnCalls(fn, calls);

        for_vector(CallExpr, call, calls) {
          if (FnSymbol* calledFn = call->resolvedFunction()) {
            if (isTaskFun(calledFn) ||
                calledFn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK)) {
              if (!shouldLlvmPrintIrFn(calledFn)) {
                addCNameToPrintLlvmIr(calledFn->cname);
                changed = true;
              }
            }
          }
        }
      }
    }
  } while (changed);
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet Symbol::codegen() {
  GenInfo* info = gGenInfo;
  GenRet ret;
  if( info->cfile ) ret.c = cname;
  return ret;
}


void Symbol::codegenDef() {
  INT_FATAL(this, "Unanticipated call to Symbol::codegenDef");
}


void Symbol::codegenPrototype() { }

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

#ifdef HAVE_LLVM
static
llvm::Value* codegenImmediateLLVM(Immediate* i)
{
  GenInfo* info = gGenInfo;
  llvm::Value* ret = NULL;

  switch(i->const_kind) {
    case NUM_KIND_BOOL:
      switch(i->num_index) {
        case BOOL_SIZE_SYS:
        case BOOL_SIZE_8:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt8Ty(info->module->getContext()),
              i->bool_value());
          break;
        case BOOL_SIZE_16:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt16Ty(info->module->getContext()),
              i->bool_value());
          break;
        case BOOL_SIZE_32:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt32Ty(info->module->getContext()),
              i->bool_value());
          break;
        case BOOL_SIZE_64:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt64Ty(info->module->getContext()),
              i->bool_value());
          break;
      }
      break;
    case NUM_KIND_UINT:
      switch(i->num_index) {
        case INT_SIZE_8:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt8Ty(info->module->getContext()),
              i->uint_value());
          break;
        case INT_SIZE_16:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt16Ty(info->module->getContext()),
              i->uint_value());
          break;
        case INT_SIZE_32:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt32Ty(info->module->getContext()),
              i->uint_value());
          break;
        case INT_SIZE_64:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt64Ty(info->module->getContext()),
              i->uint_value());
          break;
      }
      break;
    case NUM_KIND_COMMID:
      ret = llvm::ConstantInt::get(
          llvm::Type::getInt64Ty(info->module->getContext()),
          i->commid_value(),
          true);
      break;
    case NUM_KIND_INT:
      switch(i->num_index) {
        case INT_SIZE_8:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt8Ty(info->module->getContext()),
              i->int_value(),
              true);
          break;
        case INT_SIZE_16:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt16Ty(info->module->getContext()),
              i->int_value(),
              true);
          break;
        case INT_SIZE_32:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt32Ty(info->module->getContext()),
              i->int_value(),
              true);
          break;
        case INT_SIZE_64:
          ret = llvm::ConstantInt::get(
              llvm::Type::getInt64Ty(info->module->getContext()),
              i->int_value(),
              true);
          break;
      }
      break;
    case NUM_KIND_REAL:
    case NUM_KIND_IMAG:
      switch(i->num_index) {
        case FLOAT_SIZE_32:
          ret = llvm::ConstantFP::get(
              llvm::Type::getFloatTy(info->module->getContext()),
              i->v_float32);
          break;
        case FLOAT_SIZE_64:
          ret = llvm::ConstantFP::get(
              llvm::Type::getDoubleTy(info->module->getContext()),
              i->v_float64);
          break;
        default:
          INT_ASSERT("unsupported floating point width");
      }
      break;
    case NUM_KIND_COMPLEX:
      switch(i->num_index) {
        case COMPLEX_SIZE_64: {
          std::vector<llvm::Constant *> elements(2);
          elements[0] = llvm::ConstantFP::get(
              llvm::Type::getFloatTy(info->module->getContext()),
              i->v_complex64.r);
          elements[1] = llvm::ConstantFP::get(
              llvm::Type::getFloatTy(info->module->getContext()),
              i->v_complex64.i);
          ret = llvm::ConstantStruct::get(
              llvm::cast<llvm::StructType>(getTypeLLVM("_complex64")),
              elements);
          break;
        }
        case COMPLEX_SIZE_128: {
          std::vector<llvm::Constant *> elements(2);
          elements[0] = llvm::ConstantFP::get(
              llvm::Type::getDoubleTy(info->module->getContext()),
              i->v_complex128.r);
          elements[1] = llvm::ConstantFP::get(
              llvm::Type::getDoubleTy(info->module->getContext()),
              i->v_complex128.i);
          ret = llvm::ConstantStruct::get(
              llvm::cast<llvm::StructType>(getTypeLLVM("_complex128")),
              elements);
          break;
        }
        default:
          INT_ASSERT("unsupported complex floating point width");
      }
      break;
    case CONST_KIND_STRING:
      // Note that string immediate values are stored
      // with C escapes - that is newline is 2 chars \ n
      // so we have to convert to a sequence of bytes
      // for LLVM (the C backend can just print it out).
      std::string newString = unescapeString(i->v_string, NULL);
      ret = info->irBuilder->CreateGlobalString(newString);
      break;
  }

  return ret;
}
#endif

GenRet VarSymbol::codegenVarSymbol(bool lhsInSetReference) {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;
  ret.chplType = typeInfo();

  if (id == breakOnCodegenID)
    gdbShouldBreakHere();

  if( outfile ) {
    // dtString immediates don't actually codegen as immediates, we just use
    // them for param string functionality.
    if (immediate && ret.chplType != dtString && ret.chplType != dtBytes) {
      ret.isLVPtr = GEN_VAL;
      if (immediate->const_kind == CONST_KIND_STRING) {
        ret.c += '"';
        ret.c += immediate->v_string;
        ret.c += '"';
      } else if (immediate->const_kind == NUM_KIND_BOOL) {
        std::string bstring = (immediate->bool_value())?"true":"false";
        const char* castString = "(";
        switch (immediate->num_index) {
        case BOOL_SIZE_SYS:
        case BOOL_SIZE_8:
          castString = "UINT8(";
          break;
        case BOOL_SIZE_16:
          castString = "UINT16(";
          break;
        case BOOL_SIZE_32:
          castString = "UINT32(";
          break;
        case BOOL_SIZE_64:
          castString = "UINT64(";
          break;
        default:
          INT_FATAL("Unexpected immediate->num_index: %d\n", immediate->num_index);
        }
        ret.c = castString + bstring + ")";
      } else if (immediate->const_kind == NUM_KIND_INT) {
        int64_t iconst = immediate->int_value();
        if (iconst == (1ll<<63)) {
          ret.c = "-INT64(9223372036854775807) - INT64(1)";
        } else if (iconst <= -2147483648ll || iconst >= 2147483647ll) {
          ret.c = "INT64(" + int64_to_string(iconst) + ")";
        } else {
          const char* castString = "(";
          switch (immediate->num_index) {
          case INT_SIZE_8:
            castString = "INT8(";
            break;
          case INT_SIZE_16:
            castString = "INT16(";
            break;
          case INT_SIZE_32:
            castString = "INT32(";
            break;
          case INT_SIZE_64:
            castString = "INT64(";
            break;
          default:
            INT_FATAL("Unexpected immediate->num_index: %d\n", immediate->num_index);
          }

          ret.c = castString + int64_to_string(iconst) + ")";
        }
      } else if (immediate->const_kind == NUM_KIND_UINT) {
        uint64_t uconst = immediate->uint_value();
        if( uconst <= (uint64_t) INT32_MAX ) {
          const char* castString = "(";
          switch (immediate->num_index) {
          case INT_SIZE_8:
            castString = "UINT8(";
            break;
          case INT_SIZE_16:
            castString = "UINT16(";
            break;
          case INT_SIZE_32:
            castString = "UINT32(";
            break;
          case INT_SIZE_64:
            castString = "UINT64(";
            break;
          default:
            INT_FATAL("Unexpected immediate->num_index: %d\n", immediate->num_index);
          }
          ret.c = castString + uint64_to_string(uconst) + ")";
        } else {
          ret.c = "UINT64(" + uint64_to_string(uconst) + ")";
        }
      } else if (immediate->const_kind == NUM_KIND_COMMID) {
        int64_t iconst = immediate->commid_value();
        if (iconst == (1ll<<63)) {
          ret.c = "-COMMID(9223372036854775807) - COMMID(1)";
        } else {
          INT_ASSERT(immediate->num_index == INT_SIZE_64);
          ret.c = "COMMID(" + int64_to_string(iconst) + ")";
        }
      } else if (immediate->const_kind == NUM_KIND_REAL ||
                 immediate->const_kind == NUM_KIND_IMAG) {
        double value = immediate->real_value();
        const char* castString = NULL;
        switch (immediate->num_index) {
        case FLOAT_SIZE_32:
          castString = "REAL32(";
          break;
        case FLOAT_SIZE_64:
          castString = "REAL64(";
          break;
        default:
          INT_FATAL("Unexpected immediate->num_index");
        }

        ret.c = castString + real_to_string(value) + ")";
      } else if (immediate->const_kind == NUM_KIND_COMPLEX) {

        IF1_float_type flType = FLOAT_SIZE_32;
        const char* chplComplexN = NULL;

        switch(immediate->num_index) {
          case COMPLEX_SIZE_64:
            flType = FLOAT_SIZE_32;
            chplComplexN = "_chpl_complex64";
            break;
          case COMPLEX_SIZE_128:
            flType = FLOAT_SIZE_64;
            chplComplexN = "_chpl_complex128";
            break;
          default:
            INT_ASSERT("unsupported complex floating point width");
        }

        Immediate r_imm = getDefaultImmediate(dtReal[flType]);
        Immediate i_imm = getDefaultImmediate(dtImag[flType]);

        // get the real and imaginary parts
        coerce_immediate(immediate, &r_imm);
        coerce_immediate(immediate, &i_imm);

        VarSymbol* r = new_ImmediateSymbol(&r_imm);
        VarSymbol* i = new_ImmediateSymbol(&i_imm);

        ret = codegenCallExpr(chplComplexN,
                              new SymExpr(r),
                              new SymExpr(i));

        ret.chplType = typeInfo();

      } else {
        INT_FATAL("Unexpected immediate type");
      }
    } else {
      // not immediate
      // is it a constant extern? If it is, it might be for example
      // an enum or #define'd value, in which case taking the address
      // of it is simply nonsense. Therefore, we code generate
      // extern const symbols as GEN_VAL (ie not an lvalue).
      if( hasFlag(FLAG_CONST) && hasFlag(FLAG_EXTERN) ) {
        ret.isLVPtr = GEN_VAL;
        ret.c = cname;
      } else {
        QualifiedType qt = qualType();
        if (lhsInSetReference) {
          ret.c = '&';
          ret.c += cname;
          ret.isLVPtr = GEN_PTR;
          if (qt.isRef() && !qt.isRefType())
            ret.chplType = getOrMakeRefTypeDuringCodegen(typeInfo());
          else if (qt.isWideRef() && !qt.isWideRefType()) {
            Type* refType = getOrMakeRefTypeDuringCodegen(typeInfo());
            ret.chplType = getOrMakeWideTypeDuringCodegen(refType);
          }
        } else {
          if (qt.isRef() && !qt.isRefType()) {
            ret.c = cname;
            ret.isLVPtr = GEN_PTR;
          } else if(qt.isWideRef() && !qt.isWideRefType()) {
            ret.c = cname;
            ret.isLVPtr = GEN_WIDE_PTR;
          } else {
            ret.c = '&';
            ret.c += cname;
            ret.isLVPtr = GEN_PTR;
          }
        }
      }
      // Print string contents in a comment if developer mode
      // and savec is set.
      if (developer &&
          0 != strcmp(saveCDir, "") &&
          immediate &&
          ret.chplType == dtString &&
          immediate->const_kind == CONST_KIND_STRING) {
        if (strstr(immediate->v_string, "/*") ||
            strstr(immediate->v_string, "*/")) {
          // Don't emit comment b/c string contained comment character.
        } else {
          ret.c += " /* \"";
          ret.c += immediate->v_string;
          ret.c += "\" */";
        }
      }
    }
    return ret;
  } else {
#ifdef HAVE_LLVM

    // for LLVM

    // Handle extern type variables.
    if( hasFlag(FLAG_EXTERN) && isType() ) {
      // code generate the type.
      GenRet got = typeInfo();
      return got;
    }

    // for nil, generate a void pointer of chplType dtNil
    // to allow LLVM pointer cast
    // e.g. T = ( (locale) (nil) );
    //
    // We would just compare against dtNil, but in some cases
    // the code generator needs to assign e.g.
    //   _ret:dtNil = nil
    if( typeInfo() == dtNil && 0 == strcmp(cname, "nil") ) {
      GenRet voidPtr;
      voidPtr.val = llvm::Constant::getNullValue(info->irBuilder->getInt8PtrTy());
      voidPtr.chplType = dtNil;
      return voidPtr;
    }

    if (typeInfo() == dtBool){
      // since "true" and "false" are read into the LVT during ReadMacrosAction
      // they will generate an LLVM value of type i32 instead of i8
      if (0 == strcmp(cname, "false")){
        GenRet boolVal = new_UIntSymbol(0, INT_SIZE_8)->codegen();
        return boolVal;
      }
      if (0 == strcmp(cname, "true")){
        GenRet boolVal = new_UIntSymbol(1, INT_SIZE_8)->codegen();
        return boolVal;
      }
    }

    if(!isImmediate()) {
      // check LVT for value
      GenRet got = info->lvt->getValue(cname);
      got.chplType = typeInfo();
      // extern C arrays might be declared with type c_ptr(eltType)
      // (which is a lie but works OK in C). In that event, generate
      // a pointer to the first element when the variable is used.
      if (got.val &&
          hasFlag(FLAG_EXTERN) &&
          getValType()->symbol->hasFlag(FLAG_C_PTR_CLASS) &&
          info->lvt->isCArray(cname)) {
        got.val = info->irBuilder->CreateStructGEP(NULL, got.val, 0);
        got.isLVPtr = GEN_VAL;
      }
      if (got.val)
        return got;
    }

    if(isImmediate()) {
      ret.isLVPtr = GEN_VAL;
      if(immediate->const_kind == CONST_KIND_STRING) {
        if(llvm::Value *value = info->module->getNamedGlobal(cname)) {
          ret.val = value;
          ret.isLVPtr = GEN_PTR;
          return ret;
        }
        llvm::Value *constString = codegenImmediateLLVM(immediate);
        llvm::GlobalVariable *globalValue =
          llvm::cast<llvm::GlobalVariable>(
              info->module->getOrInsertGlobal
                  (name, info->irBuilder->getInt8PtrTy()));
        globalValue->setConstant(true);
        globalValue->setInitializer(llvm::cast<llvm::Constant>(
              info->irBuilder->CreateConstInBoundsGEP2_32(
                NULL, constString, 0, 0)));
        ret.val = globalValue;
        ret.isLVPtr = GEN_PTR;
      } else {
        ret.val = codegenImmediateLLVM(immediate);
      }

      return ret;
    }

    if(std::string(cname) == "0") {
      // Chapel compiler should not make these.
      INT_FATAL(" zero value BOO ");
      return ret;
    } else if (std::string(cname) == "NULL") {
      GenRet voidPtr;
      voidPtr.val = llvm::Constant::getNullValue(info->irBuilder->getInt8PtrTy());
      voidPtr.chplType = typeInfo();
      return voidPtr;
    }
#endif
  }

  USR_FATAL(this->defPoint, "Could not find C variable %s - "
            "perhaps it is a complex macro?", cname);
  return ret;
}

GenRet VarSymbol::codegen() {
  return codegenVarSymbol(true);
}

void VarSymbol::codegenDefC(bool global, bool isHeader) {
  GenInfo* info = gGenInfo;
  if (this->hasFlag(FLAG_EXTERN) && !this->hasFlag(FLAG_GENERATE_SIGNATURE))
    return;

  if (type == dtNothing || type == dtVoid)
    return;

  AggregateType* ct = toAggregateType(type);
  QualifiedType qt = qualType();

  if (qt.isRef() && !qt.isRefType()) {
    Type* refType = getOrMakeRefTypeDuringCodegen(type);
    ct = toAggregateType(refType);
  }
  if (qt.isWideRef() && !qt.isWideRefType()) {
    Type* refType = getOrMakeRefTypeDuringCodegen(type);
    Type* wideType = getOrMakeWideTypeDuringCodegen(refType);
    ct = toAggregateType(wideType);
  }

  Type* useType = type;
  if (ct) useType = ct;

  std::string typestr =  (this->hasFlag(FLAG_SUPER_CLASS) ?
                          std::string(toAggregateType(useType)->classStructName(true)) :
                          useType->codegen().c);

  //
  // a variable can be codegen'd as static if it is global and neither
  // exported nor external.
  //
  std::string str;

  if(fIncrementalCompilation || (this->hasFlag(FLAG_EXTERN) &&
                                 this->hasFlag(FLAG_GENERATE_SIGNATURE))) {
    bool addExtern =  global && isHeader;
    str = (addExtern ? "extern " : "") + typestr + " " + cname;
  } else {
    bool isStatic =  global && !hasFlag(FLAG_EXPORT) && !hasFlag(FLAG_EXTERN);
    str = (isStatic ? "static " : "") + typestr + " " + cname;
  }

  if (ct) {
    if (ct->isClass()) {
      if (isFnSymbol(defPoint->parentSymbol)) {
        str += " = NULL";
      }
    } else if (ct->symbol->hasFlag(FLAG_WIDE_REF) ||
               ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (isFnSymbol(defPoint->parentSymbol)) {
        //
        // CHPL_LOCALEID_T_INIT is #defined in the chpl-locale-model.h
        // file in the runtime, for the selected locale model.
        //
        str += " = {CHPL_LOCALEID_T_INIT, NULL}";
      }
    }
  }

  if (fGenIDS)
    str = idCommentTemp(this) + str;
  if (printCppLineno && !isHeader && !isTypeSymbol(defPoint->parentSymbol))
    str = zlineToString(this) + str;

  info->cLocalDecls.push_back(str);
}

void VarSymbol::codegenGlobalDef(bool isHeader) {
  GenInfo* info = gGenInfo;

  if( id == breakOnCodegenID ||
      (breakOnCodegenCname[0] &&
       0 == strcmp(cname, breakOnCodegenCname)) ) {
    gdbShouldBreakHere();
  }

  if( info->cfile ) {
    codegenDefC(/*global=*/true, isHeader);
  } else {
#ifdef HAVE_LLVM
    if(type == dtNothing || !isHeader) {
      return;
    }

    if( this->hasFlag(FLAG_EXTERN) ) {
      // Make sure that it already exists in the layered value table.
      if( isType() ) {
        llvm::Type* t = info->lvt->getType(cname);
        if( ! t ) {
          // TODO should be USR_FATAL
          USR_WARN(this, "Could not find extern def of type %s", cname);
        }
      } else {
        GenRet v = info->lvt->getValue(cname);
        if( ! v.val ) {
          // TODO should be USR_FATAL
          // Commenting out to prevent problems with S_IRWXU and friends
          // USR_WARN(this, "Could not find extern def of %s", cname);
        }
      }
    } else {
      bool existing;

      existing = (info->module->getNamedValue(cname) != NULL);

      if( existing )
        INT_FATAL(this, "Redefinition of a global variable %s", cname);

      // Now, create a global variable with appropriate linkage.
      llvm::Type* llTy = type->codegen().type;
      INT_ASSERT(llTy);

      llvm::GlobalVariable *gVar =
        new llvm::GlobalVariable(
            *info->module,
            llTy,
            false, /* is constant */
            hasFlag(FLAG_EXPORT) ? llvm::GlobalVariable::ExternalLinkage
                                 : llvm::GlobalVariable::InternalLinkage,
            llvm::Constant::getNullValue(llTy), /* initializer, */
            cname);
      info->lvt->addGlobalValue(cname, gVar, GEN_PTR, ! is_signed(type) );

      gVar->setDSOLocal(true);

      if(debug_info){
        debug_info->get_global_variable(this);
      }
    }
#endif
  }
}

void VarSymbol::codegenDef() {
  GenInfo* info = gGenInfo;

  if (id == breakOnCodegenID)
    gdbShouldBreakHere();

  // Local variable symbols should never be
  // generated for extern or void types
  if (this->hasFlag(FLAG_EXTERN))
    return;
  if (type == dtNothing || type == dtVoid)
    return;

  // Check sizes for c_array
  if (type->symbol->hasFlag(FLAG_C_ARRAY)) {
    int64_t sizeInt = toAggregateType(type)->cArrayLength();
    if (sizeInt > INT_MAX)
        USR_FATAL(type->symbol->instantiationPoint, "c_array is too large");
  }

  if( info->cfile ) {
    codegenDefC();
  } else {
#ifdef HAVE_LLVM
    if(isImmediate()) {
      llvm::GlobalVariable *globalValue = llvm::cast<llvm::GlobalVariable>(
          info->module->getOrInsertGlobal(cname, type->codegen().type));
      globalValue->setConstant(true);

      if(immediate->const_kind == CONST_KIND_STRING) {
        if(llvm::Value *constString = codegenImmediateLLVM(immediate)) {
          llvm::GlobalVariable *globalString =
            llvm::cast<llvm::GlobalVariable>(constString);
          globalValue->setInitializer(llvm::cast<llvm::Constant>(
                info->irBuilder->CreateConstInBoundsGEP2_32(
                  NULL, globalString, 0, 0)));
        } else {
          llvm::GlobalVariable *globalString =
            new llvm::GlobalVariable(
                *info->module,
                llvm::IntegerType::getInt8Ty(info->module->getContext()),
                true,
                llvm::GlobalVariable::PrivateLinkage,
                NULL,
                "string");
          globalString->setInitializer(llvm::Constant::getNullValue(
                llvm::IntegerType::getInt8Ty(info->module->getContext())));
          globalValue->setInitializer(llvm::cast<llvm::Constant>(
                info->irBuilder->CreateConstInBoundsGEP1_32(
                  NULL, globalString, 0)));
        }
      } else {
        globalValue->setInitializer(llvm::cast<llvm::Constant>(
              codegenImmediateLLVM(immediate)));
      }
      info->lvt->addGlobalValue(cname, globalValue, GEN_VAL, ! is_signed(type));
    }
    llvm::Type *varType = type->codegen().type;
    llvm::Value *varAlloca = createVarLLVM(varType, cname);
    info->lvt->addValue(cname, varAlloca, GEN_PTR, ! is_signed(type));

    if(AggregateType *ctype = toAggregateType(type)) {
      if(ctype->isClass() ||
         ctype->symbol->hasFlag(FLAG_WIDE_REF) ||
         ctype->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        if(isFnSymbol(defPoint->parentSymbol)) {
          info->irBuilder->CreateStore(
              llvm::Constant::getNullValue(varType), varAlloca);
        }
      }
    }
    if(debug_info){
      debug_info->get_variable(this);
    }
#endif
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

bool argMustUseCPtr(Type* type) {
  // no additional c pointer indirection needed for ref/wide ref types
  if (type->symbol->hasFlag(FLAG_REF) ||
      type->symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS))
    return false;

  bool recordNotRangeNotExtern = isRecord(type) &&
                                 !type->symbol->hasFlag(FLAG_RANGE) &&
                                 !type->symbol->hasFlag(FLAG_EXTERN);

  return recordNotRangeNotExtern ||
         isUnion(type);
}

bool ArgSymbol::requiresCPtr(void) {
  /* This used to be true for INTENT_REF, but that is handled with the "_ref"
     class and we don't need to generate a pointer for it directly */
  if (hasFlag(FLAG_ARG_THIS)) {
      if (is_complex_type(type))
        return true;
  }
  return argMustUseCPtr(type);
}

static Type* getArgSymbolCodegenType(ArgSymbol* arg) {
  QualifiedType q = arg->qualType();
  Type* useType = q.type();

  if (q.isRef() && !q.isRefType())
    useType = getOrMakeRefTypeDuringCodegen(useType);

  if (q.isWideRef() && !q.isWideRefType()) {
    Type* refType = getOrMakeRefTypeDuringCodegen(useType);
    useType = getOrMakeWideTypeDuringCodegen(refType);
  }
  return useType;
}

// Alter the C code type to avoid _ref_, c_ptr_ prefixes and instead use a
// type that C understands without additional information (e.g. instead of
// _ref_int64_t, just int64_t *).  But only for exported symbols like the
// return type of exported functions, or arguments of those functions.
//
// TODO: apply to _ddata as well?
static std::string
transformTypeForPointer(Type* type) {
  if (type->symbol->hasFlag(FLAG_REF)) {
    Type* referenced = type->getValType();
    return referenced->codegen().c + " *";

  } else if (type->symbol->hasFlag(FLAG_C_PTR_CLASS)) {
    Type* pointedTo = getDataClassType(type->symbol)->typeInfo();
    return pointedTo->codegen().c + " *";
  }
  std::string typeName = type->codegen().c;
  return typeName;
}

GenRet ArgSymbol::codegenType() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

  Type* useType = getArgSymbolCodegenType(this);

  if( outfile ) {
    std::string argType = transformTypeForPointer(useType);
    ret.c = argType;
  } else {
#ifdef HAVE_LLVM
    llvm::Type *argType = useType->codegen().type;
    ret.type = argType;
#endif
  }
  ret.chplType = useType;
  return ret;
}

GenRet ArgSymbol::codegen() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

  ret.chplType = this->type;

  if( outfile ) {
    QualifiedType qt = qualType();
    ret.c = '&';
    ret.c += cname;
    ret.isLVPtr = GEN_PTR;
    if (qt.isRef() && !qt.isRefType())
      ret.chplType = getOrMakeRefTypeDuringCodegen(typeInfo());
    else if (qt.isWideRef() && !qt.isWideRefType()) {
      Type* refType = getOrMakeRefTypeDuringCodegen(typeInfo());
      ret.chplType = getOrMakeWideTypeDuringCodegen(refType);
    }
    /*
    // BHARSH TODO: Is this still necessary?
    if (q.isRef() && !q.isRefType()) {
      ret.c = cname;
      ret.isLVPtr = GEN_PTR;
    } else if(q.isWideRef() && !q.isWideRefType()) {
      ret.c = cname;
      ret.isLVPtr = GEN_WIDE_PTR;
    } else {
      ret.c = '&';
      ret.c += cname;
      ret.isLVPtr = GEN_PTR;
    }
    */
  } else {
#ifdef HAVE_LLVM
    ret = info->lvt->getValue(cname);
#endif
  }

  // BHARSH TODO: Is this still necessary?
  //if( requiresCPtr() ) {
  //  // Don't try to use chplType.
  //  ret.chplType = NULL;
  //  ret = codegenLocalDeref(ret);
  //}

  //ret.chplType = this->type;

  return ret;
}

static std::string getFortranTypeName(Type* type, Symbol* sym) {
  static std::set<Symbol*> warnedSymbols;
  std::string typeName = fortranTypeNames[type->symbol];

  if (typeName.empty()) {
    if (warnedSymbols.count(sym) == 0) {
      // TODO: Maybe issue an error instead?
      USR_WARN(sym->defPoint, "Unknown Fortran type generating interface for C type: %s", type->symbol->cname);
      warnedSymbols.insert(sym);
    }
    return type->symbol->cname;
  } else {
    return typeName;
  }
}

static std::string getFortranKindName(Type* type, Symbol* sym) {
  static std::set<Symbol*> warnedSymbols;
  std::string kindName = fortranKindNames[type->symbol];

  if (kindName.empty()) {
    if (warnedSymbols.count(sym) == 0) {
      // TODO: Maybe issue an error instead?
      USR_WARN(sym->defPoint, "Unknown Fortran KIND generating interface for C type: %s", type->symbol->cname);
      warnedSymbols.insert(sym);
    }
    return type->symbol->cname;
  } else {
    return kindName;
  }
}

std::string ArgSymbol::getPythonType(PythonFileType pxd) {
  Type* t = getArgSymbolCodegenType(this);

  if (t->getValType() == dtExternalArray &&
      (pxd == PYTHON_PYX || pxd == C_PYX)
      && exportedArrayElementType[this] != NULL) {
    // Allow python declarations to accept anything iterable to translate to
    // an array, instead of limiting to a specific Python type
    return "";
  } else if (t->symbol->hasFlag(FLAG_REF) &&
             t->getValType() == dtOpaqueArray &&
             (pxd == PYTHON_PYX || pxd == C_PYX)) {
    return "ChplOpaqueArray ";
  } else if (pxd == C_PYX && t->getValType() == exportTypeChplByteBuffer) {
    //
    // For now, bytes uses an arg check in the body of the routine to ensure
    // that the argument is bytes.
    //
    // TODO: Better place to put this?
    //
    return "";
  } else {
    return getPythonTypeName(t, pxd) + " ";
  }
}

std::string ArgSymbol::getPythonDefaultValue() {
  std::string defaultVal = exportedDefaultValues[this];
  if (defaultVal != "") {
    return "= " + defaultVal;
  } else {
    return "";
  }
}

static std::string pythonArgToStringC(ArgSymbol* as) {
  std::string strname = as->cname;
  std::string res = "\tcdef const char* chpl_" + strname + " = " + strname;
  res += "\n";
  return res;
}

//
// Translate a Python argument with the 'bytes' type into a Chapel 'bytes',
// or a Python argument with the 'str' type into a Chapel 'string'. Both
// Python types are shuttled into Chapel via a 'chpl_byte_buffer', but we
// do emit checks (for now) to restrict the Python argument type to 'bytes'
// or 'str', accordingly.
//
static std::string pythonArgToByteBuffer(ArgSymbol* as) {
  std::string strname = as->cname;

  Type* origt = getUnwrappedArg(as)->type->getValType();
  INT_ASSERT(origt == dtBytes || origt == dtString);

  std::string chapelType = "Chapel bytes";
  std::string pythonType = "bytes";

  if (origt == dtString) {
    chapelType = "Chapel string";
    pythonType = "str";
  }

  std::string res;

  //
  // Generate a Python TypeError if the Python type does not match the
  // Chapel type (string or bytes).
  //
  res += "\tif type(" + strname + ") != " + pythonType + ":\n";
  res += "\t\traise TypeError(\"Expected \'" + pythonType;
  res += "\' in conversion to \'" + chapelType;
  res += "\', found \" + str(type(" + strname + ")))\n";

  // Python strings need to encode themselves into a bytes first.
  if (origt == dtString) {
    res += "\t" + strname + " = " + strname + ".encode(\'utf-8\')\n";
  }

  // Get the size of the bytes buffer.
  std::string argsize = "size_" + strname;
  res += "\tcdef size_t " + argsize + " = len(" + strname + ")\n";

  // Get a handle to the bytes buffer.
  std::string argdata = "data_" + strname;
  res += "\tcdef char* " + argdata + " = " + strname + "\n";

  // Declare a struct by value on the stack.
  std::string wrapval = "chpl_" + strname + "_val";

  // Create a chpl_byte_buffer struct that represents a shallow copy.
  res += "\tcdef chpl_byte_buffer " + wrapval + "\n";
  res += "\t" + wrapval + ".isOwned = 0\n";
  res += "\t" + wrapval + ".data = " + argdata + "\n";
  res += "\t" + wrapval + ".size = " + argsize + "\n";

  // The final result is a copy of the stack allocated struct.
  res += "\tcdef chpl_byte_buffer chpl_" + strname;
  res += " = " + wrapval + "\n";

  return res;
}

static std::string pythonArgToExternalArray(ArgSymbol* as) {
  std::string strname = as->cname;

  if (Symbol* eltType = exportedArrayElementType[as]) {
    // The element type will be recorded in the exportedArrayElementType map
    // if this arg's type was originally a Chapel array instead of explicitly
    // an external array.  If we have the element type, that means we need to
    // do a translation in the python wrapper.
    std::string typeStr = getPythonTypeName(eltType->type, PYTHON_PYX);
    std::string typeStrCDefs = getPythonTypeName(eltType->type, C_PYX);

    // Create the memory needed to store the contents of what was passed to us
    // E.g. cdef chpl_external_array chpl_foo =
    //          chpl_make_external_array(sizeof(element type), len(foo))
    std::string res = "\tcdef chpl_external_array chpl_" + strname;
    res += " = chpl_make_external_array(sizeof(" + typeStrCDefs + "), len(";
    res += strname + "))\n";

    // Copy the contents over.
    // E.g. for i in range(len(foo)):
    //         (<element type*>chpl_foo.elts)[i] = foo[i]
    res += "\tfor i in range(len(" + strname + ")):\n";
    res += "\t\t(<" + typeStrCDefs + "*>chpl_" + strname + ".elts)[i] = ";
    res += strname + "[i]\n";

    return res;
  }
  return "";
}

//
// Handle opaque arrays. Opaque arrays have a Python representation that
// stores the C contents in a field named 'val'.
//
static std::string pythonArgToOpaqueArray(ArgSymbol* as) {
  std::string strname = as->cname;
  std::string res = "\tchpl_" + strname + " = &" + strname + ".val\n";
  return res;
}

//
// Pass a pointer to a 'numpy.ndarray' buffer or a 'ctypes._Pointer' into
// Chapel code. In both cases we already know the size of underlying
// elements.
//
static std::string pythonArgToChapelArrayOrPtr(ArgSymbol* as) {
  Type* t = getArgSymbolCodegenType(as);
  std::string strname = as->cname;

  // Lydia TODO 12/04/18: Might be good to use a template where we can
  // replace all instances of a placeholder with the argument name instead of
  // of writing all of the code out in this chunky, unclear fashion.  Might
  // be worth considering doing for the other 'pythonArgTo...' functions
  // as well.
  std::string res = "\tcdef ";
  std::string typeStr = "";
  std::string typeStrCDefs = "";
  if (t->symbol->hasFlag(FLAG_REF)) {
    typeStr = getPythonTypeName(t->getValType(), PYTHON_PYX);
    typeStrCDefs = getPythonTypeName(t->getValType(), C_PYX);
  } else {
    typeStr = getPythonTypeName(getDataClassType(t->symbol)->typeInfo(),
                                PYTHON_PYX);
    typeStrCDefs = getPythonTypeName(getDataClassType(t->symbol)->typeInfo(),
                                     C_PYX);
  }
  res += typeStrCDefs + " * chpl_" + strname + "\n";
  res += "\tcdef numpy.ndarray[" + typeStrCDefs;
  res += ", ndim=1, mode = 'c'] chpl_tmp_" + strname + "\n"; // for numpy case
  res += "\tcdef intptr_t chpl_tmp2_" + strname; // for ctypes.pointer case
  // If sent a numpy array, pass in a pointer to the array
  res += "\n\tif type(" + strname + ") == numpy.ndarray:\n";
  res += "\t\tchpl_tmp_" + strname + " = numpy.ascontiguousarray(";
  res += strname + ", dtype = " + typeStr + ")\n";
  res += "\t\tchpl_" + strname + " = <" + typeStrCDefs + "*> chpl_tmp_";
  res += strname + ".data\n";
  // Otherwise, check if type is ctypes._Pointer.  Note that this means we
  // cannot send in ctypes.byref, but I don't think Cython allows that right
  // now anyways
  // Note: this is a lot more work than we would like it to be, but we think
  // it is the best possible at the moment.  Unless Cython decides to support
  // ctypes directly, then these conversions will not be necessary
  res += "\telif isinstance(" + strname + ", ctypes._Pointer):\n";
  res += "\t\tpython_" + strname + " = ctypes.addressof(" + strname;
  res += ".contents)\n";
  res += "\t\tchpl_tmp2_" + strname + " = <intptr_t>python_" + strname;
  res += "\n\t\tchpl_" + strname + " = <" + typeStrCDefs + "*> chpl_tmp2_";
  res += strname + "\n";
  // TODO: support ctypes arrays as well
  // Otherwise, throw a type error because we've been passed something that
  // won't work (and we can't assign a Python object into a C one without
  // knowing what the type is)
  res += "\telse:\n";
  res += "\t\traise TypeError(\"" + strname + " is of unsupported type\")\n";
  return res;
}

// Some Python type instances need to be translated into C level type instances.
// Generate code to perform that translation when this is the case
std::string ArgSymbol::getPythonArgTranslation() {
  Type* t = getArgSymbolCodegenType(this);
  Type* valType = t->getValType();

  if (t == dtStringC) {
    return pythonArgToStringC(this);
  } else if (valType == exportTypeChplByteBuffer) {
    return pythonArgToByteBuffer(this);
  } else if (valType == dtExternalArray) {
    return pythonArgToExternalArray(this);
  } else if (t->symbol->hasFlag(FLAG_REF) && valType == dtOpaqueArray) {
    return pythonArgToOpaqueArray(this);
  } else if (t->symbol->hasEitherFlag(FLAG_C_PTR_CLASS, FLAG_REF)) {
    return pythonArgToChapelArrayOrPtr(this);
  }
  return "";
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

void TypeSymbol::codegenPrototype() {
  if (!hasFlag(FLAG_EXTERN)) {
    type->codegenPrototype();
  }
}


void TypeSymbol::codegenDef() {
  GenInfo *info = gGenInfo;

  if( id == breakOnCodegenID ||
      (breakOnCodegenCname[0] &&
       0 == strcmp(cname, breakOnCodegenCname)) ) {
    gdbShouldBreakHere();
  }

  if (!hasFlag(FLAG_EXTERN)) {
    type->codegenDef();
  }

  this->addFlag(FLAG_CODEGENNED);

  if( info->cfile ) {
    // no action required.
  } else {
#ifdef HAVE_LLVM
    llvm::Type *type = info->lvt->getType(cname);

    if(type == NULL) {
      USR_FATAL(this, "Could not find C type for %s", cname);
    }

    llvmType = type;
    if(debug_info) debug_info->get_type(this->type);
#endif
  }
}

void TypeSymbol::codegenMetadata() {
#ifdef HAVE_LLVM
  // Don't do anything if we've already visited this type,
  // or the type is void so we don't need metadata.
  if (llvmTbaaTypeDescriptor || type == dtNothing) return;

  GenInfo* info = gGenInfo;
  INT_ASSERT(info->tbaaRootNode);

  // Set the llvmTbaaTypeDescriptor to non-NULL so that we can
  // avoid recursing.
  llvmTbaaTypeDescriptor = info->tbaaRootNode;

  AggregateType* ct = toAggregateType(type);

  Type* superType = NULL;
  // Recursively generate the TBAA nodes for this type.
  if( ct ) {
    for_fields(field, ct) {
      AggregateType* fct = toAggregateType(field->type);
      if(fct && field->hasFlag(FLAG_SUPER_CLASS)) {
        superType = field->type;
        field->type->symbol->codegenMetadata();
      } else if (!isClass(type)) {
        field->type->symbol->codegenMetadata();
      }
    }
  }

  llvm::MDNode* parent = info->tbaaUnionsNode;
  if( superType ) {
    parent = superType->symbol->llvmTbaaTypeDescriptor;
  } else {
    llvm::Type *ty = NULL;
    if (llvmType) {
      ty = llvmType;
    } else if (hasFlag(FLAG_EXTERN)) {
      ty = info->lvt->getType(cname);
    } else {
      ty = this->codegen().type;
    }
    if (ty && llvm::isa<llvm::PointerType>(ty)) {
      parent = dtCVoidPtr->symbol->llvmTbaaTypeDescriptor;
    }
  }
  INT_ASSERT(parent && parent != info->tbaaRootNode);

  // Ref and _ddata are really the same, and can conceivably
  // alias, so we normalize _ddata to be ref for the purposes of TBAA.
  if( hasFlag(FLAG_DATA_CLASS) ) {
    Type* eltType = getDataClassType(this)->typeInfo();
    Type* refType = getOrMakeRefTypeDuringCodegen(eltType);
    refType->symbol->codegenMetadata();
    INT_ASSERT(refType->symbol->llvmTbaaTypeDescriptor != info->tbaaRootNode);
    this->llvmTbaaTypeDescriptor = refType->symbol->llvmTbaaTypeDescriptor;
    this->llvmTbaaAccessTag = refType->symbol->llvmTbaaAccessTag;
    this->llvmConstTbaaAccessTag = refType->symbol->llvmConstTbaaAccessTag;
    return;
  }

  // Only things that aren't 'struct'-like should have simple TBAA
  // metadata. If they can alias with their fields, we don't do simple TBAA.
  // Integers, reals, bools, enums, references, wide pointers
  // count as one thing. Records, strings, complexes should not
  // get simple TBAA (they can get struct tbaa).
  if (isUnion(type) || (isRecord(type) && ct->numFields() == 0)) {
    // This is necessary due to the design of LLVM TBAA metadata.
    // The preferred situation would be to allow each union to have
    // multiple parents, corresponding to its members.  The way TBAA
    // metadata is designed, this can only be achieved with a struct
    // type descriptor.  However, struct type descriptors do not
    // support multiple members at the same offset, so that doesn't work.
    // The only other way to handle unions correctly, given the limitations
    // of LLVM's TBAA metadata design, is to make all unions ancestors
    // of everything, as we do here.  Clang does this as well.
    //
    // This means we don't get any help with alias disambiguation on
    // unions.  We still need to supply a TBAA type descriptor for them,
    // though, in case they appear as a member of a class or record.
    //
    // Records that have no Chapel IR fields, but may have LLVM IR fields,
    // are treated as unions because we don't know what Chapel type the
    // LLVM fields are referring to.  (There is no backward mapping.)
    llvmTbaaTypeDescriptor = info->tbaaUnionsNode;
  } else if (is_complex_type(type)) {
    codegenCplxMetadata();
    INT_ASSERT(llvmTbaaAggTypeDescriptor);
    llvmTbaaTypeDescriptor = llvmTbaaAggTypeDescriptor;
  } else if (!ct || hasFlag(FLAG_STAR_TUPLE) ||
             isClass(type) || hasEitherFlag(FLAG_REF,FLAG_WIDE_REF) ||
             hasEitherFlag(FLAG_DATA_CLASS,FLAG_WIDE_CLASS)) {
    if (is_imag_type(type)) {
      // At present, imaginary often aliases with real,
      // so make real the parent of imaginary.
      INT_ASSERT(type == dtImag[FLOAT_SIZE_32] ||
                 type == dtImag[FLOAT_SIZE_64]);
      TypeSymbol *re;
      if (type == dtImag[FLOAT_SIZE_32]) {
        re = dtReal[FLOAT_SIZE_32]->symbol;
      } else {
        re = dtReal[FLOAT_SIZE_64]->symbol;
      }
      re->codegenMetadata();
      parent = re->llvmTbaaTypeDescriptor;
    }
    llvmTbaaTypeDescriptor =
      info->mdBuilder->createTBAAScalarTypeNode(cname, parent);
  } else if (isRecord(type)) {
    codegenAggMetadata();
    if (llvmTbaaAggTypeDescriptor)
      llvmTbaaTypeDescriptor = llvmTbaaAggTypeDescriptor;
  }
  if (llvmTbaaTypeDescriptor && llvmTbaaTypeDescriptor != info->tbaaRootNode) {
    // Create tbaa access tags, one for const and one for not.
    // The createTBAAStructTagNode() method works for both scalars
    // and aggregates, referencing the whole object.
    llvmTbaaAccessTag =
      info->mdBuilder->createTBAAStructTagNode(llvmTbaaTypeDescriptor,
                                               llvmTbaaTypeDescriptor,
                                               /*Offset=*/0);
    llvmConstTbaaAccessTag =
      info->mdBuilder->createTBAAStructTagNode(llvmTbaaTypeDescriptor,
                                               llvmTbaaTypeDescriptor,
                                               /*Offset=*/0,
                                               /*IsConstant=*/true);
  }
#endif
}

void TypeSymbol::codegenCplxMetadata() {
#ifdef HAVE_LLVM
  // At present, complex types are not records, so we have to
  // build their struct type descriptors and tbaa.struct access tags
  // manually, using knowledge of the contents of complex types.
  GenInfo* info = gGenInfo;
  llvm::LLVMContext& ctx = info->module->getContext();
  const llvm::DataLayout& dl = info->module->getDataLayout();

  INT_ASSERT(type == dtComplex[COMPLEX_SIZE_64] ||
             type == dtComplex[COMPLEX_SIZE_128]);

  TypeSymbol *re, *im;
  if (type == dtComplex[COMPLEX_SIZE_64]) {
    re = dtReal[FLOAT_SIZE_32]->symbol;
    im = dtImag[FLOAT_SIZE_32]->symbol;
  } else {
    re = dtReal[FLOAT_SIZE_64]->symbol;
    im = dtImag[FLOAT_SIZE_64]->symbol;
  }
  re->codegenMetadata();
  im->codegenMetadata();

  uint64_t fieldSize = dl.getTypeStoreSize(re->llvmType);
  llvm::Type *int64Ty = llvm::Type::getInt64Ty(ctx);
  llvm::ConstantAsMetadata *zero =
    info->mdBuilder->createConstant(llvm::ConstantInt::get(int64Ty, 0));
  llvm::ConstantAsMetadata *fsz =
    info->mdBuilder->createConstant(llvm::ConstantInt::get(int64Ty,
                                                             fieldSize));

  llvm::Metadata *TypeOps[5];
  TypeOps[0] = llvm::MDString::get(ctx,cname);
  TypeOps[1] = re->llvmTbaaTypeDescriptor;
  TypeOps[2] = zero;  // offset
  TypeOps[3] = im->llvmTbaaTypeDescriptor;
  TypeOps[4] = fsz;   // offset
  llvmTbaaAggTypeDescriptor = llvm::MDNode::get(ctx, TypeOps);

  llvm::Metadata *CopyOps[6];
  CopyOps[0] = zero;  // offset
  CopyOps[1] = fsz;   // size
  CopyOps[2] = re->llvmTbaaAccessTag;
  CopyOps[3] = fsz;   // offset
  CopyOps[4] = fsz;   // size
  CopyOps[5] = im->llvmTbaaAccessTag;
  llvmTbaaStructCopyNode = llvm::MDNode::get(ctx, CopyOps);

  llvm::Metadata *ConstCopyOps[6];
  ConstCopyOps[0] = zero;  // offset
  ConstCopyOps[1] = fsz;   // size
  ConstCopyOps[2] = re->llvmConstTbaaAccessTag;
  ConstCopyOps[3] = fsz;   // offset
  ConstCopyOps[4] = fsz;   // size
  ConstCopyOps[5] = im->llvmConstTbaaAccessTag;
  llvmConstTbaaStructCopyNode = llvm::MDNode::get(ctx, ConstCopyOps);
#endif
}

void TypeSymbol::codegenAggMetadata() {
#ifdef HAVE_LLVM
  // Don't do anything if we've already visited this type.
  if (llvmTbaaAggTypeDescriptor) return;

  GenInfo* info = gGenInfo;

  // Set the llvmTbaaAggTypeDescriptor to non-NULL so that we can
  // avoid recursing.
  llvmTbaaAggTypeDescriptor = info->tbaaRootNode;

  llvm::LLVMContext& ctx = info->module->getContext();
  const llvm::DataLayout& dl = info->module->getDataLayout();
  AggregateType *ct = toAggregateType(type);
  INT_ASSERT(ct);

  // Create the TBAA struct type descriptors and tbaa.struct metadata nodes.
  llvm::SmallVector<llvm::Metadata*, 16> TypeOps;
  llvm::SmallVector<llvm::Metadata*, 16> CopyOps;
  llvm::SmallVector<llvm::Metadata*, 16> ConstCopyOps;

  const char *struct_name = ct->classStructName(true);
  llvm::Type *struct_type_ty = info->lvt->getType(struct_name);
  if (isClass(type) && !struct_type_ty)
    return;
  llvm::StructType *struct_type = NULL;
  INT_ASSERT(struct_type_ty);
  struct_type = llvm::dyn_cast<llvm::StructType>(struct_type_ty);
  INT_ASSERT(struct_type);

  TypeOps.push_back(llvm::MDString::get(ctx, struct_name));

  llvm::Type *int64Ty = llvm::Type::getInt64Ty(ctx);
  if (ct == dtObject) {
    // Special case because we never create object's actual field within
    // the Chapel IR.  Change this if defineObjectClass() changes.
    //
    llvm::Type *fieldType = CLASS_ID_TYPE->symbol->codegen().type;
    INT_ASSERT(CLASS_ID_TYPE->symbol->llvmTbaaTypeDescriptor &&
               CLASS_ID_TYPE->symbol->llvmTbaaTypeDescriptor !=
               info->tbaaRootNode);
    llvm::Constant *off = llvm::ConstantInt::get(int64Ty, 0);
    TypeOps.push_back(CLASS_ID_TYPE->symbol->llvmTbaaTypeDescriptor);
    TypeOps.push_back(llvm::ConstantAsMetadata::get(off));

    llvm::Constant *sz =
      llvm::ConstantInt::get(int64Ty, dl.getTypeStoreSize(fieldType));
    CopyOps.push_back(llvm::ConstantAsMetadata::get(off));
    CopyOps.push_back(llvm::ConstantAsMetadata::get(sz));
    CopyOps.push_back(CLASS_ID_TYPE->symbol->llvmTbaaAccessTag);
    ConstCopyOps.push_back(llvm::ConstantAsMetadata::get(off));
    ConstCopyOps.push_back(llvm::ConstantAsMetadata::get(sz));
    ConstCopyOps.push_back(CLASS_ID_TYPE->symbol->llvmConstTbaaAccessTag);
  } else {
    for_fields(field, ct) {
      if (field->type == dtNothing)
        continue;

      llvm::Type *fieldType = NULL;
      AggregateType *fct = toAggregateType(field->type);
      bool is_super = false;
      if (fct && field->hasFlag(FLAG_SUPER_CLASS)) {
        is_super = true;
        fieldType = info->lvt->getType(fct->classStructName(true));
        field->type->symbol->codegenAggMetadata();
      } else if (field->type->symbol->hasFlag(FLAG_EXTERN)) {
        fieldType = info->lvt->getType(field->type->symbol->cname);
      } else {
        fieldType = field->type->symbol->codegen().type;
      }
      INT_ASSERT(fieldType);
      uint64_t store_size = dl.getTypeStoreSize(fieldType);
      if (store_size > 0) {
        bool unused;
        unsigned fieldno = ct->getMemberGEP(field->cname, unused);
        uint64_t byte_offset =
          dl.getStructLayout(struct_type)->getElementOffset(fieldno);
        llvm::Constant *off = llvm::ConstantInt::get(int64Ty, byte_offset);
        if (is_super) {
          INT_ASSERT(field->type->symbol->llvmTbaaAggTypeDescriptor &&
                     field->type->symbol->llvmTbaaAggTypeDescriptor !=
                     info->tbaaRootNode);
          TypeOps.push_back(field->type->symbol->llvmTbaaAggTypeDescriptor);
        } else {
          INT_ASSERT(field->type->symbol->llvmTbaaTypeDescriptor &&
                     field->type->symbol->llvmTbaaTypeDescriptor !=
                     info->tbaaRootNode);
          TypeOps.push_back(field->type->symbol->llvmTbaaTypeDescriptor);
        }
        TypeOps.push_back(llvm::ConstantAsMetadata::get(off));

        llvm::Constant *sz = llvm::ConstantInt::get(int64Ty, store_size);
        CopyOps.push_back(llvm::ConstantAsMetadata::get(off));
        CopyOps.push_back(llvm::ConstantAsMetadata::get(sz));
        CopyOps.push_back(is_super ?
                          field->type->symbol->llvmTbaaAggAccessTag :
                          field->type->symbol->llvmTbaaAccessTag);
        ConstCopyOps.push_back(llvm::ConstantAsMetadata::get(off));
        ConstCopyOps.push_back(llvm::ConstantAsMetadata::get(sz));
        ConstCopyOps.push_back(is_super ?
                               field->type->symbol->llvmConstTbaaAggAccessTag :
                               field->type->symbol->llvmConstTbaaAccessTag);
      }
    }
  }
  if (CopyOps.size() > 0) {
    llvmTbaaAggTypeDescriptor = llvm::MDNode::get(ctx, TypeOps);
    llvmTbaaStructCopyNode = llvm::MDNode::get(ctx, CopyOps);
    llvmConstTbaaStructCopyNode = llvm::MDNode::get(ctx, ConstCopyOps);
  }
  if (llvmTbaaAggTypeDescriptor &&
      llvmTbaaAggTypeDescriptor != info->tbaaRootNode) {
    // Create tbaa access tags, one for const and one for not.
    llvmTbaaAggAccessTag =
      info->mdBuilder->createTBAAStructTagNode(llvmTbaaAggTypeDescriptor,
                                               llvmTbaaAggTypeDescriptor,
                                               /*Offset=*/0);
    llvmConstTbaaAggAccessTag =
      info->mdBuilder->createTBAAStructTagNode(llvmTbaaAggTypeDescriptor,
                                               llvmTbaaAggTypeDescriptor,
                                               /*Offset=*/0,
                                               /*IsConstant=*/true);
  }
#endif
}

GenRet TypeSymbol::codegen() {
  GenInfo *info = gGenInfo;
  GenRet ret;
  ret.chplType = type;

  // Should not be code generating non-canonical class pointers
  // (these are replaced with canonical ones after resolution)
  if (isDecoratedClassType(type))
    INT_FATAL("attempting to code generate a managed class type");

  if( info->cfile ) {
    if (this == dtNothing->symbol) {
      ret.c = dtVoid->codegen().c;
    } else {
      ret.c = cname;
    }
  } else {
#ifdef HAVE_LLVM
    if( ! llvmType ) {
      // If we don't have an LLVM type yet, the type hasn't been
      // code generated, so code generate it now. This can get called
      // when adding types partway through code generation.
      codegenDef();
      // codegenMetadata(); //TODO -- enable TBAA generation in the future.
    }
    ret.type = llvmType;
#endif
  }

  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

#ifdef HAVE_LLVM

static void pushAllFieldTypesRecursively(const char* name,
                                         Type* t,
                                         std::vector<llvm::Type*>& argTys,
                                         std::vector<const char*>& argNames) {
  if (isRecord(t)) {
    if (AggregateType* at = toAggregateType(t)) {
      for_fields(field, at) {
        pushAllFieldTypesRecursively(astr(name, "_", field->name),
                                     field->type,
                                     argTys,
                                     argNames);
      }
    }
  } else {
    llvm::Type* ty = t->codegen().type;
    argTys.push_back(ty);
    argNames.push_back(name);
  }
}

static llvm::FunctionType* codegenFunctionTypeLLVM(FnSymbol* fn,
                                             llvm::AttributeList& attrs,
                                             std::vector<const char*>& argNames)
{
  // This function is inspired by clang's CodeGenTypes::GetFunctionType
  // and CodeGenModule::ConstructAttributeList

  llvm::LLVMContext& ctx = gGenInfo->llvmContext;
  const llvm::DataLayout& layout = gGenInfo->module->getDataLayout();
  const clang::CodeGen::CGFunctionInfo* CGI = NULL;

  if (fn->hasFlag(FLAG_EXPORT)) {
    CGI = &getClangABIInfo(fn);
  }

  unsigned int stackSpace = layout.getAllocaAddrSpace();

  llvm::Type* chapelReturnTy; // Chapel return type as an llvm type
  llvm::Type* returnTy;
  std::vector<llvm::Type *> argTys;

  // Void type handled here since LLVM complains about a
  // void type defined in a module
  if (fn->retType == dtVoid || fn->retType == dtNothing) {
    returnTy = llvm::Type::getVoidTy(ctx);
  } else {
    returnTy = fn->retType->codegen().type;

    if (fn->hasFlag(FLAG_LLVM_RETURN_NOALIAS)) {
      // Add NoAlias on return for allocator-like functions
      llvm::AttrBuilder b;
      b.addAttribute(llvm::Attribute::NoAlias);
      attrs = attrs.addAttributes(ctx, llvm::AttributeList::ReturnIndex, b);
    }
  }

  chapelReturnTy = returnTy;

  int curCArg = 0;

  if (CGI) {

    const clang::CodeGen::ABIArgInfo& returnInfo = CGI->getReturnInfo();

    switch (returnInfo.getKind()) {
      case clang::CodeGen::ABIArgInfo::Kind::Indirect:
      case clang::CodeGen::ABIArgInfo::Kind::Ignore:
      {
        returnTy = llvm::Type::getVoidTy(ctx);
        break;
      }

      case clang::CodeGen::ABIArgInfo::Kind::Direct:
      {
        llvm::AttrBuilder b;
        if (returnInfo.getInReg())
          b.addAttribute(llvm::Attribute::InReg);
        attrs = attrs.addAttributes(ctx, llvm::AttributeList::ReturnIndex, b);

        returnTy = returnInfo.getCoerceToType();
        break;
      }
      case clang::CodeGen::ABIArgInfo::Kind::Extend:
      {
        bool isSigned = returnInfo.isSignExt();

        llvm::AttrBuilder b;
        if (isSigned)
          b.addAttribute(llvm::Attribute::SExt);
        else
          b.addAttribute(llvm::Attribute::ZExt);

        if (returnInfo.getInReg())
          b.addAttribute(llvm::Attribute::InReg);

        attrs = attrs.addAttributes(ctx, llvm::AttributeList::ReturnIndex, b);

        returnTy = returnInfo.getCoerceToType();
        break;
      }
      case clang::CodeGen::ABIArgInfo::Kind::InAlloca:
      {
        if (returnInfo.getInAllocaSRet()) {
          returnTy = llvm::PointerType::get(returnTy, stackSpace);
        } else {
          returnTy = llvm::Type::getVoidTy(ctx);
        }
        break;
      }
      case clang::CodeGen::ABIArgInfo::Kind::CoerceAndExpand:
      {
        returnTy = returnInfo.getUnpaddedCoerceAndExpandType ();
        break;
      }
      case clang::CodeGen::ABIArgInfo::Kind::Expand:
        INT_FATAL("Invalid ABI kind for return argument");
        break;
      // No default -> compiler warning if more added
    }

    // Add type for sret argument
    if (returnInfo.isIndirect()) {
      if (returnInfo.isSRetAfterThis()) {
        INT_FATAL("not handled"); // replace existing sret argument?
        curCArg++; // a guess
      }

      // returnTy is void, so use chapelReturnTy
      argTys.push_back(llvm::PointerType::get(chapelReturnTy, stackSpace));
      argNames.push_back("indirect_return");

      // Adjust attributes for sret argument
      llvm::AttrBuilder b;
      b.addAttribute(llvm::Attribute::StructRet);
      b.addAttribute(llvm::Attribute::NoAlias);
      if (returnInfo.getInReg())
        b.addAttribute(llvm::Attribute::InReg);
      b.addAlignmentAttr(returnInfo.getIndirectAlign().getQuantity());
      attrs = attrs.addAttributes(ctx, argTys.size(), b);
    }
    // Add type for inalloca argument
    if (CGI->usesInAlloca()) {
      auto argStruct = CGI->getArgStruct();
      argTys.push_back(argStruct->getPointerTo());
      argNames.push_back("inalloca_arg");

      // Adjust attributes for inalloca argument
      llvm::AttrBuilder b;
      b.addAttribute(llvm::Attribute::InAlloca);
      attrs = attrs.addAttributes(ctx, argTys.size(), b);
    }
  }

  for_formals(formal, fn) {
    const clang::CodeGen::ABIArgInfo* argInfo = NULL;
    if (CGI) {
      argInfo = getCGArgInfo(CGI, curCArg);
    }

    if (formal->hasFlag(FLAG_NO_CODEGEN))
      continue; // do not print locale argument, end count, dummy class

    if (argInfo) {
      if (argInfo->isIgnore() || argInfo->isInAlloca())
        continue; // ignore - inalloca handled separately
    }

    llvm::Type* argTy = formal->codegenType().type;

    if (argInfo) {
      if (llvm::Type* paddingTy = argInfo->getPaddingType()) {
        // Emit padding argument
        argTys.push_back(paddingTy);
        argNames.push_back(astr(formal->cname, ".padding"));

        // Adjust attributes for padding argument
        if (argInfo->getPaddingInReg()) {
          llvm::AttrBuilder b;
          b.addAttribute(llvm::Attribute::InReg);
          attrs = attrs.addAttributes(ctx, argTys.size(), b);
        }
      }

      switch (argInfo->getKind()) {
        case clang::CodeGen::ABIArgInfo::Kind::Ignore:
        case clang::CodeGen::ABIArgInfo::Kind::InAlloca:
          break;

        case clang::CodeGen::ABIArgInfo::Kind::Indirect:
        {
          // Emit indirect argument
          argTys.push_back(llvm::PointerType::get(argTy, stackSpace));
          argNames.push_back(astr(formal->cname, ".indirect"));

          // Adjust attributes for indirect argument
          llvm::AttrBuilder b;
          if (argInfo->getInReg()) {
            b.addAttribute(llvm::Attribute::InReg);
          }
          if (argInfo->getIndirectByVal()) {
#if HAVE_LLVM_VER >= 90
            b.addByValAttr(argTy);
#else
            b.addAttribute(llvm::Attribute::ByVal);
#endif
          }
          clang::CharUnits align = argInfo->getIndirectAlign();
          if (argInfo->getIndirectByVal()) {
            b.addAlignmentAttr(align.getQuantity());
          }
          attrs = attrs.addAttributes(ctx, argTys.size(), b);
          break;
        }

        case clang::CodeGen::ABIArgInfo::Kind::Extend:
        case clang::CodeGen::ABIArgInfo::Kind::Direct:
        {
          // flatten out structs to scalars if possible
          llvm::Type *toTy = argInfo->getCoerceToType();
          llvm::StructType *sTy = llvm::dyn_cast<llvm::StructType>(toTy);
          if (sTy && argInfo->isDirect() && argInfo->getCanBeFlattened()) {
            int nElts = sTy->getNumElements();
            for (int i = 0; i < nElts; i++) {
              // Emit flattened argument
              argTys.push_back(sTy->getElementType(i));
              argNames.push_back(astr(formal->cname, ".", istr(i)));
              // Adjust attributes
              llvm::AttrBuilder b;
              if (argInfo->isExtend()) {
                if (argInfo->isSignExt()) b.addAttribute(llvm::Attribute::SExt);
                else                      b.addAttribute(llvm::Attribute::ZExt);
              }
              if (argInfo->getInReg()) {
                b.addAttribute(llvm::Attribute::InReg);
              }
              attrs = attrs.addAttributes(ctx, argTys.size(), b);
            }
          } else {
            // Emit argument
            argTys.push_back(toTy);
            const char* name = formal->cname;
            if (argTy != toTy)
              name = astr(name, ".coerce");
            argNames.push_back(name);
            // Adjust attributes
            llvm::AttrBuilder b;
            if (formal->isRef() && argTy == toTy) {
              b.addAttribute(llvm::Attribute::NonNull);
              llvm::Type* valType = formal->getValType()->codegen().type;
              int64_t sz = getTypeSizeInBytes(layout, valType);
              b.addDereferenceableAttr(sz);
            }
            if (argInfo->isExtend()) {
              if (argInfo->isSignExt()) b.addAttribute(llvm::Attribute::SExt);
              else                      b.addAttribute(llvm::Attribute::ZExt);
            }
            if (argInfo->getInReg()) {
              b.addAttribute(llvm::Attribute::InReg);
            }
            attrs = attrs.addAttributes(ctx, argTys.size(), b);
          }
          break;
        }

        case clang::CodeGen::ABIArgInfo::Kind::CoerceAndExpand:
        {
          int i = 0;
          for (auto ty : argInfo->getCoerceAndExpandTypeSequence()) {
            argTys.push_back(ty);
            argNames.push_back(astr(formal->cname, istr(i)));
            i++;
          }
          break;
        }
        case clang::CodeGen::ABIArgInfo::Kind::Expand:
        {
          // TODO: check this for complex
          // TODO: should this be applying to C types not Chapel ones?
          pushAllFieldTypesRecursively(formal->name,
                                       getArgSymbolCodegenType(formal),
                                       argTys, argNames);
          break;
        }
      }

    } else {
      argTys.push_back(argTy);
      argNames.push_back(formal->cname);
      if(formal->isRef()) {
        llvm::AttrBuilder b;
        b.addAttribute(llvm::Attribute::NonNull);
        llvm::Type* valType = formal->getValType()->codegen().type;
        int64_t sz = getTypeSizeInBytes(layout, valType);
        b.addDereferenceableAttr(sz);
        attrs = attrs.addAttributes(ctx, argTys.size(), b);
      }
    }

    curCArg++;
  }

  llvm::FunctionType* fnType =
    llvm::FunctionType::get(returnTy, argTys, /* is var arg */ false);

  return fnType;
}

#endif

// forHeader == true when generating the C header.
GenRet FnSymbol::codegenFunctionType(bool forHeader) {
  GenInfo* info = gGenInfo;
  GenRet ret;

  ret.chplType = typeInfo();
  INT_ASSERT(ret.chplType == dtUnknown); //just documenting the current state

  if( info->cfile ) {
    // Cast to right function type.
    std::string str;

    std::string retString = transformTypeForPointer(retType);
    str += retString.c_str();
    if( forHeader ) {
      str += " ";
      str += cname;
    } else str += "(*)";
    str += "(";
    if(numFormals() == 0) {
      str += "void";
    } else {
      int count = 0;
      for_formals(formal, this) {
        if (formal->hasFlag(FLAG_NO_CODEGEN))
          continue; // do not print locale argument, end count, dummy class
        if (count > 0)
          str += ",\n";
        str += formal->codegenType().c;
        if( forHeader ) {
          str += " ";
          str += formal->cname;
        }
        if (fGenIDS) {
          str += " ";
          str += idCommentTemp(formal);
        }
        count++;
      }
    }
    str += ")";
    ret.c = str;
  } else {
#ifdef HAVE_LLVM
    llvm::AttributeList argAttrs; // and return attrs
    std::vector<const char*> argNames;
    llvm::FunctionType* fTy = codegenFunctionTypeLLVM(this,
                                                      argAttrs,
                                                      argNames);
    INT_ASSERT(argNames.size() == fTy->getNumParams());
    ret.type = fTy;
#endif
  }
  return ret;
}

void FnSymbol::codegenHeaderC(void) {
  FILE* outfile = gGenInfo->cfile;
  if (fGenIDS)
    fprintf(outfile, "%s", idCommentTemp(this));

  if (!fIncrementalCompilation && !hasFlag(FLAG_EXPORT) && !hasFlag(FLAG_EXTERN)) {
    fprintf(outfile, "static ");
  }
  fprintf(outfile, "%s", codegenFunctionType(true).c.c_str());
}

GenRet FnSymbol::codegenCast(GenRet fnPtr) {
  GenInfo *info = gGenInfo;
  GenRet fngen;
  GenRet t = codegenFunctionType(false);
  if( info->cfile ) {
    // Cast to right function type.
    std::string str;
    str += "((";
    str += t.c;
    str  += ")";
    str += fnPtr.c;
    str += ")";
    fngen.c = str;
  } else {
#ifdef HAVE_LLVM
    // now cast to correct function type
    llvm::FunctionType* fnType = llvm::cast<llvm::FunctionType>(t.type);
    llvm::PointerType *ptrToFnType = llvm::PointerType::get(fnType, 0);
    fngen.val = info->irBuilder->CreateBitCast(fnPtr.val, ptrToFnType);
#endif
  }
  return fngen;
}

void FnSymbol::codegenPrototype() {
  GenInfo *info = gGenInfo;

  if (hasFlag(FLAG_EXTERN) && !hasFlag(FLAG_GENERATE_SIGNATURE)) return;
  if (hasFlag(FLAG_NO_CODEGEN))   return;

  if( id == breakOnCodegenID ||
      (breakOnCodegenCname[0] &&
       0 == strcmp(cname, breakOnCodegenCname)) ) {
    gdbShouldBreakHere();
  }

  if( info->cfile ) {
    // In C, we don't need to generate prototypes for external
    // functions, since these prototypes will presumably be
    // present in some C header file.
    codegenHeaderC();
    fprintf(info->cfile, ";\n");
  } else {
#ifdef HAVE_LLVM
    std::vector<const char*> argNames;
    llvm::AttributeList argAttrs;
    llvm::FunctionType *fTy = codegenFunctionTypeLLVM(this,
                                                      argAttrs,
                                                      argNames);

    llvm::Function *existing;

    // Look for the function in the LayeredValueTable
    // or in the module.
    existing = getFunctionLLVM(cname);

    // Check to see if another function already exists.
    if( existing ) {
      // other function with the same name exists!
      // check that the prototype matches.
      if(!existing->empty()) {
        INT_FATAL(this, "Redefinition of a function");
      }
      if(existing->arg_size() != argNames.size()) {
        INT_FATAL(this,
                  "Redefinition of a function with different number of args");
      }
      if(fTy != existing->getFunctionType()) {
        INT_FATAL(this, "Redefinition of a function with different arg types");
      }

      return;
    }

    llvm::Function::LinkageTypes linkage = llvm::Function::InternalLinkage;
    if (hasFlag(FLAG_EXPORT))
      linkage = llvm::Function::ExternalLinkage;

    // No other function with the same name exists.
    llvm::Function *func = llvm::Function::Create(fTy, linkage, cname,
                                                  info->module);

    func->setDSOLocal(true);

    func->setAttributes(argAttrs);

    int argID = 0;
    for(llvm::Function::arg_iterator ai = func->arg_begin();
        ai != func->arg_end();
        ai++) {
      ai->setName(argNames[argID]);
      argID++;
    }

#endif
  }
  return;
}


void FnSymbol::codegenDef() {
  GenInfo *info = gGenInfo;
  FILE* outfile = info->cfile;
#ifdef HAVE_LLVM
  llvm::Function *func = NULL;
#endif

  if( id == breakOnCodegenID ||
      (breakOnCodegenCname[0] &&
       0 == strcmp(cname, breakOnCodegenCname)) ) {
    gdbShouldBreakHere();
  }

  if( hasFlag(FLAG_NO_CODEGEN) ) return;

  info->cStatements.clear();
  info->cLocalDecls.clear();

  if( outfile ) {
    if (strcmp(saveCDir, "")) {
     if (const char* rawname = fname()) {
      zlineToFileIfNeeded(this, outfile);
      const char* name = strrchr(rawname, '/');
      name = name ? name + 1 : rawname;
      fprintf(outfile, "/* %s:%d */\n", name, linenum());
     }
    }

    codegenHeaderC();

    fprintf(outfile, " {\n");
  } else {
#ifdef HAVE_LLVM
    llvm::IRBuilder<>* irBuilder = info->irBuilder;
    const llvm::DataLayout& layout = info->module->getDataLayout();
    llvm::LLVMContext &ctx = info->llvmContext;

    unsigned int stackSpace = layout.getAllocaAddrSpace();

    func = getFunctionLLVM(cname);

    // Mark functions to dump as no-inline so they actually exist
    // after optimization
    if(llvmPrintIrStageNum != llvmStageNum::NOPRINT &&
       shouldLlvmPrintIrFn(this)) {
        func->addFnAttr(llvm::Attribute::NoInline);
    }
    // Also mark no-inline if the flag was set
    if (fNoInline)
      func->addFnAttr(llvm::Attribute::NoInline);

    if (this->hasFlag(FLAG_LLVM_READNONE))
      func->addFnAttr(llvm::Attribute::ReadNone);

    if (this->hasFlag(FLAG_FUNCTION_TERMINATES_PROGRAM)) {
      func->addFnAttr(llvm::Attribute::NoReturn);
    }

    if (specializeCCode) {
      // Add target-cpu and target-features metadata
      // We could also get this from clang::CompilerInvocation getTargetOpts
      llvm::StringRef TargetCPU = info->targetMachine->getTargetCPU();
      llvm::StringRef TargetFeatures = info->targetMachine->getTargetFeatureString();
      func->addFnAttr("target-cpu", TargetCPU);
      func->addFnAttr("target-features", TargetFeatures);
    }

    llvm::BasicBlock *block =
      llvm::BasicBlock::Create(info->module->getContext(), "entry", func);

    info->irBuilder->SetInsertPoint(block);

    info->lvt->addLayer();

    if(debug_info) {
      llvm::DISubprogram* dbgScope = debug_info->get_function(this);
      info->irBuilder->SetCurrentDebugLocation(
        llvm::DebugLoc::get(linenum(),0,dbgScope));
    }

    // ABI support in this function is inspired by clang's
    //   CodeGenFunction::EmitFunctionProlog

    const clang::CodeGen::CGFunctionInfo* CGI = NULL;
    if (this->hasFlag(FLAG_EXPORT)) {
      CGI = &getClangABIInfo(this);
      info->currentFunctionABI = CGI;
    }

    if (CGI) {
      const clang::CodeGen::ABIArgInfo &returnInfo = CGI->getReturnInfo();
      // Adjust attributes based on return ABI info
      if (returnInfo.isInAlloca() || returnInfo.isIndirect()) {
        func->removeFnAttr(llvm::Attribute::ReadOnly);
        func->removeFnAttr(llvm::Attribute::ReadNone);
      }

      if (returnInfo.isInAlloca())
        INT_FATAL("TODO");

    }

    llvm::Function::arg_iterator ai = func->arg_begin();
    int curCArg = 0;
    for_formals(arg, this) {
      const clang::CodeGen::ABIArgInfo* argInfo = NULL;
      if (CGI) {
        argInfo = getCGArgInfo(CGI, curCArg);
      }

      if (arg->hasFlag(FLAG_NO_CODEGEN))
        continue; // do not print locale argument, end count, dummy class

      if (argInfo) {
        if (argInfo->isIgnore())
          continue; // ignore - inalloca handled separately
        if (argInfo->isInAlloca())
          INT_FATAL("TODO");
      }

      if (argInfo && (argInfo->isIndirect() || argInfo->isInAlloca())) {
        func->removeFnAttr(llvm::Attribute::ReadOnly);
        func->removeFnAttr(llvm::Attribute::ReadNone);
      }

      Type* argType = arg->typeInfo();
      llvm::Type* chapelArgTy = argType->codegen().type;

      if (arg->requiresCPtr() || (argInfo && argInfo->isIndirect())) {
        // consume the next LLVM argument
        llvm::Argument& llArg = *ai++;
        info->lvt->addValue(arg->cname, &llArg,  GEN_PTR, !is_signed(argType));

      } else if (argInfo) {

        // handle a direct or extend argument

        if (!llvm::isa<llvm::StructType>(argInfo->getCoerceToType()) &&
            argInfo->getCoerceToType() == chapelArgTy &&
            argInfo->getDirectOffset() == 0) {
          // handle a simple direct argument

          // consume the next LLVM argument
          llvm::Value* val = &*ai++;
          // make sure the argument has the correct type
          if (val->getType() != argInfo->getCoerceToType())
            val = convertValueToType(val, argInfo->getCoerceToType(),
                                     is_signed(argType), true);

          if (val->getType() != chapelArgTy)
            val = convertValueToType(val, chapelArgTy,
                                     is_signed(argType), true);

          info->lvt->addValue(arg->cname, val,  GEN_VAL, !is_signed(argType));
        } else {
          // handle a more complex direct argument
          // (possibly in multiple registers)

          llvm::StructType *sTy = llvm::dyn_cast<llvm::StructType>(argInfo->getCoerceToType());

          if (argInfo->isDirect() && argInfo->getCanBeFlattened() && sTy &&
              sTy->getNumElements() > 1) {

            // handle a complex direct argument with multiple registers

            // Create a temp variable to store into
            GenRet tmp = createTempVar(arg->typeInfo());
            llvm::Value* ptr = tmp.val;
            llvm::Type* ptrEltTy = chapelArgTy;
            llvm::Type* i8PtrTy = irBuilder->getInt8PtrTy();
            llvm::Type* coercePtrTy = llvm::PointerType::get(sTy, stackSpace);

            // handle offset
            if (unsigned offset = argInfo->getDirectOffset()) {
              ptr = irBuilder->CreatePointerCast(ptr, i8PtrTy);
              ptr = irBuilder->CreateConstInBoundsGEP1_32(i8PtrTy, ptr, offset);
              ptr = irBuilder->CreatePointerCast(ptr, coercePtrTy);
              ptrEltTy = sTy;
            }

            // Store into the temp variable
            uint64_t srcSize = layout.getTypeAllocSize(sTy);
            llvm::Type* dstTy = ptrEltTy;
            uint64_t dstSize = layout.getTypeAllocSize(dstTy);

            llvm::Value* storeAdr = NULL;
            if (srcSize <= dstSize) {
              storeAdr = irBuilder->CreatePointerCast(ptr, coercePtrTy);
            } else {
              storeAdr = makeAllocaAndLifetimeStart(irBuilder, layout, ctx,
                                                    sTy, "coerce");
            }

            unsigned nElts = sTy->getNumElements();
            for (unsigned i = 0; i < nElts; i++) {
              // consume the next LLVM argument
              llvm::Value* val = &*ai++;
              // store it into the addr
              llvm::Value* eltPtr = irBuilder->CreateStructGEP(storeAdr, i);
              irBuilder->CreateStore(val, eltPtr);
            }

            // if we allocated a temporary, memcpy from it to the main var
            //
            if (srcSize > dstSize) {
              irBuilder->CreateMemCpy(ptr,
#if HAVE_LLVM_VER >= 100
                                      llvm::MaybeAlign(),
#else
                                      0,
#endif
                                      storeAdr,
#if HAVE_LLVM_VER >= 100
                                      llvm::MaybeAlign(),
#else
                                      0,
#endif
                                      dstSize);
            }

            info->lvt->addValue(arg->cname, tmp.val,
                                tmp.isLVPtr, tmp.isUnsigned);
          } else {
            // mainly just needing to convert the type

            // consume the next LLVM argument
            llvm::Value* val = &*ai++;

            // make sure the argument has the correct type
            if (val->getType() != argInfo->getCoerceToType())
              val = convertValueToType(val, argInfo->getCoerceToType(),
                                       is_signed(argType), true);

            if (val->getType() != chapelArgTy)
              val = convertValueToType(val, chapelArgTy,
                                       is_signed(argType), true);

            GenRet gArg;
            gArg.val = val;
            gArg.chplType = arg->typeInfo();

            GenRet tempVar = createTempVarWith(gArg);
            info->lvt->addValue(arg->cname, tempVar.val,
                                tempVar.isLVPtr, tempVar.isUnsigned);
          }
        }

      } else {
        // No ABI info is available

        // consume the LLVM argument
        llvm::Argument* llArg = &*ai++;

        GenRet gArg;
        gArg.val = llArg;
        gArg.chplType = arg->typeInfo();
        GenRet tempVar = createTempVarWith(gArg);

        info->lvt->addValue(arg->cname, tempVar.val,
                            tempVar.isLVPtr, tempVar.isUnsigned);
        // debug info for formal arguments
        if(debug_info){
          debug_info->get_formal_arg(arg, curCArg+1);
        }
      }
      curCArg++;
    }

    // if --gen-ids is enabled, add metadata mapping the
    // function back to Chapel AST id
    if (fGenIDS) {
      llvm::LLVMContext& ctx = info->llvmContext;

      llvm::Type *int64Ty = llvm::Type::getInt64Ty(ctx);
      llvm::Constant* c = llvm::ConstantInt::get(int64Ty, this->id);
      llvm::ConstantAsMetadata* aid = llvm::ConstantAsMetadata::get(c);

      llvm::MDNode* node = llvm::MDNode::get(ctx, aid);

      func->setMetadata("chpl.ast.id", node);
    }
#endif
  }

  {
    std::vector<BaseAST*> asts;
    collect_top_asts(body, asts);

    for_vector(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast))
        if (!toTypeSymbol(def->sym)) {
          def->sym->codegenDef();
          flushStatements();
        }
    }
  }

  body->codegen();
  flushStatements();
#ifdef HAVE_LLVM
  info->currentStackVariables.clear();
  info->currentFunctionABI = NULL;
#endif

  if( outfile ) {
    fprintf(outfile, "}\n\n");
  } else {
#ifdef HAVE_LLVM
    info->lvt->removeLayer();
    if( developer ) {
      bool problems = false;
      // Debug info generation creates metadata nodes that won't be
      // finished until the whole codegen is complete and finalize
      // is called.
      if( ! debug_info )
        problems = llvm::verifyFunction(*func, &llvm::errs());
      if( problems ) {
        INT_FATAL("LLVM function verification failed");
      }
    }

    if((llvmPrintIrStageNum == llvmStageNum::NONE ||
        llvmPrintIrStageNum == llvmStageNum::EVERY) &&
       shouldLlvmPrintIrFn(this))
        printLlvmIr(name, func, llvmStageNum::NONE);

    // Now run the optimizations on that function.
    // (we handle checking fFastFlag, etc, when we set up FPM_postgen)
    // This way we can potentially keep the fn in cache while it
    // is simplified. The big optos happen later.

    // (note, in particular, the default pass manager's
    //  populateFunctionPassManager does not include vectorization)
    info->FPM_postgen->run(*func);
#endif
  }

  return;
}

GenRet FnSymbol::codegen() {
  GenInfo *info = gGenInfo;
  GenRet ret;
  if( info->cfile ) ret.c = cname;
  else {
#ifdef HAVE_LLVM
    if (cname[0] == 'l' &&
        cname[1] == 'l' &&
        cname[2] == 'v' &&
        cname[3] == 'm' &&
        cname[4] == '.') {
      // Find intrinsic.

      Type* formalType = getFormal(1)->type;
      GenRet ty = formalType;
      INT_ASSERT(ty.type);
      llvm::Type* Types[] = {ty.type};

      const llvm::TargetIntrinsicInfo *TII = info->targetMachine->getIntrinsicInfo();
      llvm::Intrinsic::ID ID = llvm::Function::lookupIntrinsicID(cname);
      if (ID == llvm::Intrinsic::not_intrinsic && TII)
        ID = static_cast<llvm::Intrinsic::ID>(TII->lookupName(cname));
      ret.val = llvm::Intrinsic::getDeclaration(info->module, ID, Types);
      if (!ret.val) {
        USR_FATAL("Could not find LLVM intrinsic %s", cname);
      }
    } else {
      ret.val = getFunctionLLVM(cname);
      if( ! ret.val ) {
        if( hasFlag(FLAG_EXTERN) ) {
          if( isBuiltinExternCFunction(cname) ) {
            // it's OK.
          } else {
            USR_FATAL(this->defPoint,
                      "Could not find C function for %s; "
                      " perhaps it is missing or is a macro?", cname);
          }
        } else {
          INT_FATAL("Missing LLVM function for %s", cname);
        }
      }
    }
#endif
  }
  return ret;
}

void FnSymbol::codegenFortran(int indent) {
  GenInfo *info = gGenInfo;
  int beginIndent = indent;

  if (!hasFlag(FLAG_EXPORT)) return;
  if (hasFlag(FLAG_NO_CODEGEN)) return;

  if (info->cfile) {
    FILE* outfile = info->cfile;
    if (fGenIDS)
      fprintf(outfile, "%*s! %d\n", indent, "", this->id);
    const char* subOrProc = retType != dtVoid ? "function" : "subroutine";
    fprintf(outfile, "%*s%s %s(", indent, "", subOrProc, this->cname);
    bool first = true;

    // print the list of formal names
    for_formals(formal, this) {
      if (formal->hasFlag(FLAG_NO_CODEGEN))
        continue;
      if (!first) fprintf(outfile, ", ");

      // My Fortran compiler doesn't like names with leading underscores
      if (formal->cname[0] == '_')
        fprintf(outfile, "chpl");

      fprintf(outfile, "%s", formal->cname);
      first = false;
    }
    fprintf(outfile, ") bind(C, name=\"%s\")\n", this->cname);

    indent += 2;
    // build a unique set of type kinds to import
    std::set<std::string> uniqueKindNames;
    bool foundUnsignedInt = false;
    for_formals(formal, this) {
      if (formal->hasFlag(FLAG_NO_CODEGEN))
        continue;
      uniqueKindNames.insert(getFortranKindName(formal->type, formal));
      if (is_uint_type(formal->type)) {
        foundUnsignedInt = true;
      }
    }
    if (retType != dtVoid) {
      uniqueKindNames.insert(getFortranKindName(retType, this));
      if (is_uint_type(retType)) {
        foundUnsignedInt = true;
      }
    }

    if (foundUnsignedInt) {
      USR_WARN(this->defPoint, "Fortran does not support unsigned integers. Using signed integer instead.");
    }

    // print "import <c_type_name>" for each required type
    // Don't import anything for '_ref_CFI_cdesc_t_chpl' which is the Fortran
    // array type for array interoperability.
    if (!uniqueKindNames.empty() &&
        (uniqueKindNames.size() > 1 ||
         uniqueKindNames.count("_ref_CFI_cdesc_t_chpl") == 0)) {
      fprintf(outfile, "%*simport ", indent, "");
      first = true;
      for (std::set<std::string>::iterator kindName = uniqueKindNames.begin();
           kindName != uniqueKindNames.end(); ++kindName) {
        if (!strcmp(kindName->c_str(), "_ref_CFI_cdesc_t_chpl")) continue;
        if (!first) {
          fprintf(outfile, ", ");
        }
        fprintf(outfile, "%s", kindName->c_str());
        first = false;
      }
      fprintf(outfile, "\n");
    }

    // print type declarations for each formal
    for_formals(formal, this) {
      if (formal->hasFlag(FLAG_NO_CODEGEN))
        continue;
      const char* prefix = formal->cname[0] == '_' ? "chpl" : "";
      const bool isRef = formal->intent & INTENT_FLAG_REF;
      const char* valueString = isRef ? "" : ", value";
      std::string typeName = getFortranTypeName(formal->type, formal);
      std::string kindName = getFortranKindName(formal->type, formal);

      // declare arrays specially instead of just using the record type
      if (kindName == "_ref_CFI_cdesc_t_chpl") {
        fprintf(outfile, "%*sTYPE(*) :: %s(..)\n", indent, "", formal->cname);
      } else {
        fprintf(outfile, "%*s%s(kind=%s)%s :: %s%s\n", indent, "",
                typeName.c_str(), kindName.c_str(),
                valueString, prefix, formal->cname);
      }
    }

    // print type declaration for the return type
    if (retType != dtVoid) {
      fprintf(outfile, "%*s%s(kind=%s) :: %s\n", indent, "", getFortranTypeName(retType, this).c_str(), getFortranKindName(retType, this).c_str(), this->cname);
    }
    indent -= 2;
    fprintf(outfile, "%*send %s %s\n\n", indent, "", subOrProc, this->cname);
  } else {
    INT_FATAL("no file named to generate Fortran interface into");
  }
  INT_ASSERT(indent == beginIndent);
}

// Supports the creation of a python module with --library-python
void FnSymbol::codegenPython(PythonFileType pxd) {
  GenInfo *info = gGenInfo;

  if (!hasFlag(FLAG_EXPORT)) return;
  if (hasFlag(FLAG_NO_CODEGEN)) return;

  // Should I add the break-on-codegen-id stuff here?

  if (info->cfile) {
    FILE* outfile = info->cfile;
    if (fGenIDS)
      fprintf(outfile, "%s", idCommentTemp(this));

    if (pxd == C_PXD) {
      fprintf(outfile, "\t%s;\n", codegenPXDType().c.c_str());
    } else if (pxd == PYTHON_PYX) {
      fprintf(outfile, "\n%s", codegenPYXType().c.c_str());
    } else {
      INT_FATAL("python file type not handled");
    }
  } else {
    // TODO: LLVM stuff
  }
}

// Supports the creation of a python module with --library-python
GenRet FnSymbol::codegenPXDType() {
  GenInfo* info = gGenInfo;
  GenRet ret;

  ret.chplType = typeInfo();

  if (info->cfile) {
    // Cast to right function type.
    std::string str;

    std::string retString = getPythonTypeName(retType, C_PXD);
    str += retString.c_str();
    str += " ";
    str += cname;
    str += "(";

    if (numFormals() != 0) {
      int count = 0;
      for_formals(formal, this) {
        if (formal->hasFlag(FLAG_NO_CODEGEN))
          continue; // do not print locale argument, end count, dummy class
        if (count > 0)
          str += ", ";
        str += formal->getPythonType(C_PXD);
        str += formal->cname;
        if (fGenIDS) {
          str += " ";
          str += idCommentTemp(formal);
        }
        count++;
      }
    } // pxd files do not take void as an argument list, just close the parens
    str += ")";
    ret.c = str;

  } else {
    // TODO: LLVM stuff
  }

  return ret;
}

static void pythonRetByteBuffer(FnSymbol* fn, std::string& funcCall,
                                std::string& returnStmt) {

  // The raw result of the routine call, a "chpl_byte_buffer".
  funcCall += "cdef chpl_byte_buffer rv = ";

  // Unpack a handle to the buffer and the buffer size.
  returnStmt += "\tcdef char* rdata = rv.data\n";
  returnStmt += "\tcdef Py_ssize_t rsize = rv.size\n";

  //
  // Create a new Python bytes that is a copy of the Chapel bytes. Note
  // that this call creates a copy of the string.
  //
  returnStmt += "\tret = PyBytes_FromStringAndSize(rdata, rsize)\n";

  // This will free the "chpl_byte_buffer" buffer if required.
  returnStmt += "\tchpl_byte_buffer_free(rv)\n";

  Type* origt = getUnwrappedRetType(fn)->getValType();
  INT_ASSERT(origt != NULL);

  // The only two types that can be mapped to "chpl_byte_buffer".
  INT_ASSERT(origt == dtBytes || origt == dtString);

  //
  // If the original Chapel routine's return type is a string, then
  // decode the Python bytes into a Python UTF-8 string.
  //
  if (origt == dtString) {
    returnStmt += "\tret = ret.decode(\'utf-8\')\n";
  }
}

static void pythonRetExternalArray(FnSymbol* fn, std::string& funcCall,
                                   std::string& returnStmt) {
  INT_ASSERT(fn->retType == dtExternalArray);

  Symbol* eltTypeSym = exportedArrayElementType[fn];
  if (eltTypeSym == NULL) { return; }

  Type* eltType = eltTypeSym->type;

  funcCall += "cdef chpl_external_array ret_arr = ";

  std::string typeStr = getPythonTypeName(eltType, PYTHON_PYX);
  std::string typeStrCDefs = getPythonTypeName(eltType, C_PYX);

  //
  // Create the numpy array to return. The form looks like:
  //
  //  cdef numpy.ndarray [C element type, ndim=1] ret =
  //    numpy.zeros(shape = ret_arr.num_elts, dtype = (numpy dtype))
  //
  std::string res;
  res += "\tcdef numpy.ndarray [" + typeStrCDefs + ", ndim=1] ret";
  res += " = numpy.zeros(shape = ret_arr.num_elts, dtype = " + typeStr;
  res += ")\n";

  if (eltType == dtBytes || eltType == dtString) {

    //
    // TODO: Add this to a function we can call with a flag for string or
    // bytes? Can we also do the same for the opaque array code?
    //
    //    cdef chpl_byte_buffer rv
    //    for i in range(ret_arr.num_elts):
    //      rv = (<chpl_byte_buffer*>ret_arr.elts)[i]
    //      slot = PyBytes_FromStringAndSize(rv.data, rv.size)
    //      chpl_byte_buffer_free(rv)
    //      # Only execute this line if type is dtString:
    //      slot = slot.decode('utf-8')
    //      ret[i] = slot
    //    chpl_free_external_array(ret_arr)
    //
    res += "\tcdef chpl_byte_buffer rv\n";
    res += "\tfor i in range(ret_arr.num_elts):\n";
    res += "\t\trv = (<chpl_byte_buffer*>ret_arr.elts)[i]\n";
    res += "\t\tslot = PyBytes_FromStringAndSize(rv.data, rv.size)\n";
    res += "\t\tchpl_byte_buffer_free(rv)\n";

    if (eltType == dtString) {
      res += "\t\tslot = slot.decode(\'utf-8\')\n";
    }

    res += "\t\tret[i] = slot\n";
  } else {
    // Populate it with the contents we return (which translated C types into
    // Python types)
    res += "\tfor i in range(ret_arr.num_elts):\n";
    res += "\t\tret[i] = (<" + typeStrCDefs + "*>ret_arr.elts)[i]\n";
  }

  // Free the returned array now that its contents have been stored elsewhere
  res += "\tchpl_free_external_array(ret_arr)\n";
  returnStmt += res;
}

static void pythonRetOpaqueArray(FnSymbol* fn, std::string& funcCall,
                                 std::string& returnStmt) {
  funcCall += "ret = ChplOpaqueArray()\n\t";
  funcCall += "ret.setVal(";
}


// Supports the creation of a python module with --library-python
GenRet FnSymbol::codegenPYXType() {
  GenRet ret;

  ret.chplType = typeInfo();

  // Function header
  std::string header = "def ";
  header += cname;
  header += "(";

  // Translation of any arguments with Python-specific types
  std::string argTranslate = "";
  // Call to the wrapped function
  std::string funcCall = "\t";
  // Return statement, if applicable
  std::string returnStmt = "";
  if (retType != dtVoid) {
    if (retType == exportTypeChplByteBuffer) {
      pythonRetByteBuffer(this, funcCall, returnStmt);
    } else if (retType == dtExternalArray) {
      pythonRetExternalArray(this, funcCall, returnStmt);
    } else if (retType == dtOpaqueArray) {
      pythonRetOpaqueArray(this, funcCall, returnStmt);
    } else {
      funcCall += "ret = ";
    }
    returnStmt += "\treturn ret\n";
  }
  funcCall += "chpl_";
  funcCall += cname;
  funcCall += "(";

  if (numFormals() != 0) {
    int count = 0;
    for_formals(formal, this) {
      if (formal->hasFlag(FLAG_NO_CODEGEN))
        continue; // do not print locale argument, end count, dummy class
      if (count > 0) {
        header += ", ";
        funcCall += ", ";
      }

      std::string argType = formal->getPythonType(C_PYX);
      header += argType;
      header += formal->cname;
      if (fGenIDS) {
        header += " ";
        header += idCommentTemp(formal);
      }

      header += formal->getPythonDefaultValue();

      std::string curArgTranslate = formal->getPythonArgTranslation();
      if (curArgTranslate != "") {
        argTranslate += curArgTranslate;
        if (argType == "" && formal->type->getValType() == dtExternalArray) {
          // And we need to clean it up when we are done with it
          std::string oldRetStmt = returnStmt;
          returnStmt = "\tchpl_free_external_array(chpl_";
          returnStmt += formal->cname;
          returnStmt += ")\n" + oldRetStmt;
        }
        funcCall += "chpl_";
      }
      funcCall += formal->cname;
      count++;
    }

  } // pyx files do not take void as an argument list, just close the parens
  header += "):\n";
  if (retType == dtOpaqueArray)
    funcCall += ")";
  funcCall += ")\n";
  ret.c = header + argTranslate + funcCall + returnStmt;

  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

void EnumSymbol::codegenDef() { }

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

static int compareLineno(void* v1, void* v2) {
  FnSymbol* fn1 = (FnSymbol*)v1;
  FnSymbol* fn2 = (FnSymbol*)v2;

  return fn1->linenum() < fn2->linenum();
}

void ModuleSymbol::codegenDef() {
  GenInfo* info = gGenInfo;

  info->filename = fname();
  info->lineno   = linenum();
  commIDMap[info->filename] = 0;

  info->cStatements.clear();
  info->cLocalDecls.clear();

  std::vector<FnSymbol*> fns;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (fn->hasFlag(FLAG_EXTERN))
          continue;

        fns.push_back(fn);
      }
  }

  std::sort(fns.begin(), fns.end(), compareLineno);

#ifdef HAVE_LLVM
  if(debug_info && info->filename) {
    debug_info->get_module_scope(this);
  }
#endif

  for_vector(FnSymbol, fn, fns) {
    fn->codegenDef();
  }

  flushStatements();
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

void LabelSymbol::codegenDef() { }

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/
