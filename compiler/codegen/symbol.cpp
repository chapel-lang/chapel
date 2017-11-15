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
#include "intlimits.h"
#include "iterator.h"
#include "LayeredValueTable.h"
#include "llvmDebug.h"
#include "llvmUtil.h"
#include "misc.h"
#include "optimizations.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "type.h"
#include "resolution.h"

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

#include "llvm/Support/raw_os_ostream.h"
#endif

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

char llvmPrintIrName[FUNC_NAME_MAX+1] = "";
char llvmPrintIrStage[FUNC_NAME_MAX+1] = "";
const char *llvmPrintIrCName;
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

#ifdef HAVE_LLVM
void printLlvmIr(llvm::Function *func, llvmStageNum_t numStage) {
  if(func) {
    llvm::raw_os_ostream stdOut(std::cout);
    std::cout << "; " << "LLVM IR representation of " << llvmPrintIrName
              << " function after " << llvmStageNameFromLlvmStageNum(numStage) << " optimization stage";
    func->print(stdOut);
  }
}
#endif

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
        case BOOL_SIZE_1:
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
      }
      break;
    case CONST_KIND_STRING:
      // Note that string immediate values are stored
      // with C escapes - that is newline is 2 chars \ n
      // so we have to convert to a sequence of bytes
      // for LLVM (the C backend can just print it out).
      std::string newString = unescapeString(i->v_string, NULL);
      ret = info->builder->CreateGlobalString(newString);
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

  if( outfile ) {
    // dtString immediates don't actually codegen as immediates, we just use
    // them for param string functionality.
    if (immediate && ret.chplType != dtString) {
      ret.isLVPtr = GEN_VAL;
      if (immediate->const_kind == CONST_KIND_STRING) {
        ret.c += '"';
        ret.c += immediate->v_string;
        ret.c += '"';
      } else if (immediate->const_kind == NUM_KIND_BOOL) {
        std::string bstring = (immediate->bool_value())?"true":"false";
        const char* castString = "(";
        switch (immediate->num_index) {
        case BOOL_SIZE_1:
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
      } else {
        ret.c = cname; // in C, all floating point literals are (double)
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
      voidPtr.val = llvm::Constant::getNullValue(info->builder->getInt8PtrTy());
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
      if( got.val ) {
        return got;
      }
    }

    if(isImmediate()) {
      ret.isLVPtr = GEN_VAL;
      if(immediate->const_kind == CONST_KIND_STRING) {
        if(llvm::Value *value = info->module->getNamedGlobal(name)) {
          ret.val = value;
          ret.isLVPtr = GEN_PTR;
          return ret;
        }
        llvm::Value *constString = codegenImmediateLLVM(immediate);
        llvm::GlobalVariable *globalValue =
          llvm::cast<llvm::GlobalVariable>(
              info->module->getOrInsertGlobal
                  (name, info->builder->getInt8PtrTy()));
        globalValue->setConstant(true);
        globalValue->setInitializer(llvm::cast<llvm::Constant>(
              info->builder->CreateConstInBoundsGEP2_32(
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
      voidPtr.val = llvm::Constant::getNullValue(info->builder->getInt8PtrTy());
      voidPtr.chplType = typeInfo();
      return voidPtr;
    }
#endif
  }

  INT_FATAL("Could not code generate %s - "
            "perhaps it is a complex macro?", cname);
  return ret;
}

GenRet VarSymbol::codegen() {
  return codegenVarSymbol(true);
}

void VarSymbol::codegenDefC(bool global, bool isHeader) {
  GenInfo* info = gGenInfo;
  if (this->hasFlag(FLAG_EXTERN))
    return;

  if (type == dtVoid)
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

  if(fIncrementalCompilation) {
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
    if(type == dtVoid || !isHeader) {
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
  if (type == dtVoid)
    return;

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
                info->builder->CreateConstInBoundsGEP2_32(
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
                info->builder->CreateConstInBoundsGEP1_32(
                  NULL, globalString, 0)));
        }
      } else {
        globalValue->setInitializer(llvm::cast<llvm::Constant>(
              codegenImmediateLLVM(immediate)));
      }
      info->lvt->addGlobalValue(cname, globalValue, GEN_VAL, ! is_signed(type));
    }
    llvm::Type *varType = type->codegen().type;
    llvm::Value *varAlloca = createTempVarLLVM(varType, cname);
    info->lvt->addValue(cname, varAlloca, GEN_PTR, ! is_signed(type));

    if(AggregateType *ctype = toAggregateType(type)) {
      if(ctype->isClass() ||
         ctype->symbol->hasFlag(FLAG_WIDE_REF) ||
         ctype->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        if(isFnSymbol(defPoint->parentSymbol)) {
          info->builder->CreateStore(
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
  if (isUnion(type))
    return true;
  if (isRecord(type) &&
      // TODO: why are ref types being created with AGGREGATE_RECORD?
      !type->symbol->hasFlag(FLAG_REF) &&
      !type->symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS))
    return true;
  return false;
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

GenRet ArgSymbol::codegenType() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

  Type* useType = getArgSymbolCodegenType(this);

  if( outfile ) {
    ret.c = useType->codegen().c;
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
      printf("No type '%s'/'%s' found\n", cname, name);
      INT_FATAL(this, "No type found");
    }

    llvmType = type;
    if(debug_info) debug_info->get_type(this->type);
#endif
  }
}

void TypeSymbol::codegenMetadata() {
#ifdef HAVE_LLVM
  // Don't do anything if we've already visited this type.
  if( llvmTbaaTypeDescriptor ) return;

  GenInfo* info = gGenInfo;
  llvm::LLVMContext& ctx = info->module->getContext();
  // Create the TBAA root node if necessary.
  if( ! info->tbaaRootNode ) {
    llvm::Metadata* Ops[1];
    Ops[0] = llvm::MDString::get(ctx, "Chapel types");
    info->tbaaRootNode = llvm::MDNode::get(ctx, Ops);
  }

  // Set the llvmTbaaNode to non-NULL so that we can
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
      }
      field->type->symbol->codegenMetadata();
    }
  }

  llvm::MDNode* parent = info->tbaaRootNode;
  if( superType ) {
    parent = superType->symbol->llvmTbaaTypeDescriptor;
    INT_ASSERT( parent );
  }

  // Ref and _ddata are really the same, and can conceivably
  // alias, so we normalize _ddata to be ref for the purposes of TBAA.
  if( hasFlag(FLAG_DATA_CLASS) ) {
    Type* eltType = getDataClassType(this)->typeInfo();
    Type* refType = getOrMakeRefTypeDuringCodegen(eltType);
    refType->symbol->codegenMetadata();
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
  if( is_bool_type(type) || is_int_type(type) || is_uint_type(type) ||
      is_real_type(type) || is_imag_type(type) || is_enum_type(type) ||
      isClass(type) || hasEitherFlag(FLAG_REF,FLAG_WIDE_REF) ||
      hasEitherFlag(FLAG_DATA_CLASS,FLAG_WIDE_CLASS) ) {
    // Now create tbaa metadata, one for const and one for not.
    {
      llvm::Metadata* Ops[2];
      Ops[0] = llvm::MDString::get(ctx, cname);
      Ops[1] = parent;
      llvmTbaaTypeDescriptor = llvm::MDNode::get(ctx, Ops);
    }
    {
      llvm::Metadata* Ops[3];
      Ops[0] = llvmTbaaTypeDescriptor;
      Ops[1] = llvmTbaaTypeDescriptor;
      Ops[2] = llvm::ConstantAsMetadata::get(
                   llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), 0));
      llvmTbaaAccessTag = llvm::MDNode::get(ctx, Ops);
    }
    {
      llvm::Metadata* Ops[4];
      Ops[0] = llvmTbaaTypeDescriptor;
      Ops[1] = llvmTbaaTypeDescriptor;
      Ops[2] = llvm::ConstantAsMetadata::get(
                   llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), 0));
      Ops[3] = llvm::ConstantAsMetadata::get(
                   llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), 1));
      llvmConstTbaaAccessTag = llvm::MDNode::get(ctx, Ops);
    }
  }

  // Don't try to create tbaa.struct metadata for non-struct.
  if( isUnion(type) ||
      hasFlag(FLAG_STAR_TUPLE) ||
      hasFlag(FLAG_REF) ||
      hasFlag(FLAG_DATA_CLASS) ||
      hasEitherFlag(FLAG_WIDE_REF,FLAG_WIDE_CLASS) ) {
    return;
  }

  if( ct ) {
    // Now create the tbaa.struct metadata nodes.
    llvm::SmallVector<llvm::Metadata*, 16> Ops;
    llvm::SmallVector<llvm::Metadata*, 16> ConstOps;

    const char* struct_name = ct->classStructName(true);
    llvm::Type* struct_type_ty = info->lvt->getType(struct_name);
    llvm::StructType* struct_type = NULL;
    INT_ASSERT(struct_type_ty);
    struct_type = llvm::dyn_cast<llvm::StructType>(struct_type_ty);
    INT_ASSERT(struct_type);

    for_fields(field, ct) {
      llvm::Type* fieldType = field->type->symbol->codegen().type;
      AggregateType* fct = toAggregateType(field->type);
      if(fct && field->hasFlag(FLAG_SUPER_CLASS)) {
        fieldType = info->lvt->getType(fct->classStructName(true));
      }
      INT_ASSERT(fieldType);
      unsigned gep = ct->getMemberGEP(field->cname);
      llvm::Constant* off = llvm::ConstantExpr::getOffsetOf(struct_type, gep);
      llvm::Constant* sz = llvm::ConstantExpr::getSizeOf(fieldType);
      Ops.push_back(llvm::ConstantAsMetadata::get(off));
      Ops.push_back(llvm::ConstantAsMetadata::get(sz));
      Ops.push_back(field->type->symbol->llvmTbaaAccessTag);
      ConstOps.push_back(llvm::ConstantAsMetadata::get(off));
      ConstOps.push_back(llvm::ConstantAsMetadata::get(sz));
      ConstOps.push_back(field->type->symbol->llvmConstTbaaAccessTag);
    }
    llvmTbaaStructCopyNode = llvm::MDNode::get(ctx, Ops);
    llvmConstTbaaStructCopyNode = llvm::MDNode::get(ctx, ConstOps);
  }
#endif
}

GenRet TypeSymbol::codegen() {
  GenInfo *info = gGenInfo;
  GenRet ret;
  ret.chplType = typeInfo();
  if( info->cfile ) {
    ret.c = cname;
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

// forHeader == true when generating the C header.
GenRet FnSymbol::codegenFunctionType(bool forHeader) {
  GenInfo* info = gGenInfo;
  GenRet ret;

  ret.chplType = typeInfo();
  INT_ASSERT(ret.chplType == dtUnknown); //just documenting the current state

  if( info->cfile ) {
    // Cast to right function type.
    std::string str;
    str += retType->codegen().c.c_str();
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
    llvm::Type *returnType;
    std::vector<llvm::Type *> argumentTypes;

    int count = 0;
    for_formals(formal, this) {
      if (formal->hasFlag(FLAG_NO_CODEGEN))
        continue; // do not print locale argument, end count, dummy class
      argumentTypes.push_back(formal->codegenType().type);
      count++;
    }

    //Void type handled here since LLVM complains about a
    //void type defined in a module
    if( 0 == strcmp("void", retType->symbol->name) ) {
      returnType = llvm::Type::getVoidTy(info->module->getContext());
    } else {
      returnType = retType->codegen().type;
    }
    // now cast to correct function type
    llvm::FunctionType* fnType =
      llvm::FunctionType::get(returnType,
                              argumentTypes,
                              /* is var arg */ false);
    ret.type = fnType;
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
    fngen.val = info->builder->CreateBitCast(fnPtr.val, ptrToFnType);
#endif
  }
  return fngen;
}

void FnSymbol::codegenPrototype() {
  GenInfo *info = gGenInfo;

  if (hasFlag(FLAG_EXTERN))       return;
  if (hasFlag(FLAG_NO_PROTOTYPE)) return;
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
    std::vector<llvm::Type *> argumentTypes;
    std::vector<const char *> argumentNames;

    int numArgs = 0;
    std::vector<int> refArgs;
    for_formals(arg, this) {
      if (arg->hasFlag(FLAG_NO_CODEGEN))
        continue; // do not print locale argument, end count, dummy class
      argumentTypes.push_back(arg->codegenType().type);
      argumentNames.push_back(arg->cname);

      arg->codegenType();
      numArgs++;
      // LLVM Arguments indices in function API are 1-based not 0-based
      // that's why we push numArgs after increment
      if(arg->isRef())
        refArgs.push_back(numArgs);
    }

    llvm::FunctionType *type = llvm::cast<llvm::FunctionType>(
        this->codegenFunctionType(false).type);

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
      if((int)existing->arg_size() != numArgs) {
        INT_FATAL(this,
                  "Redefinition of a function with different number of args");
      }
      if(type != existing->getFunctionType()) {
        INT_FATAL(this, "Redefinition of a function with different arg types");
      }

      return;
    }

    // No other function with the same name exists.
    llvm::Function *func =
      llvm::Function::Create(
          type,
          hasFlag(FLAG_EXPORT) ? llvm::Function::ExternalLinkage
                               : llvm::Function::InternalLinkage,
          cname,
          info->module);

    for(auto argNumber : refArgs)
      func->addAttribute(argNumber, llvm::Attribute::NonNull);

    int argID = 0;
    for(llvm::Function::arg_iterator ai = func->arg_begin();
        ai != func->arg_end();
        ai++) {
      ai->setName(argumentNames[argID++]);
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
    func = getFunctionLLVM(cname);

    if(llvmPrintIrStageNum != llvmStageNum::NOPRINT
            && strcmp(llvmPrintIrName, name) == 0) {
        func->addFnAttr(llvm::Attribute::NoInline);
        llvmPrintIrCName = cname;
    }
    if (fNoInline)
      func->addFnAttr(llvm::Attribute::NoInline);

    llvm::BasicBlock *block =
      llvm::BasicBlock::Create(info->module->getContext(), "entry", func);

    info->builder->SetInsertPoint(block);

    info->lvt->addLayer();

    if(debug_info) {
      llvm::DISubprogram* dbgScope = debug_info->get_function(this);
      info->builder->SetCurrentDebugLocation(
        llvm::DebugLoc::get(linenum(),0,dbgScope));
    }

    llvm::Function::arg_iterator ai = func->arg_begin();
    unsigned int ArgNo = 1; //start from 1 to cooperate with createLocalVariable
    for_formals(arg, this) {
      if (arg->hasFlag(FLAG_NO_CODEGEN))
        continue; // do not print locale argument, end count, dummy class

      if (arg->requiresCPtr()){
        llvm::Argument& llArg = *ai;
        info->lvt->addValue(arg->cname, &llArg,  GEN_PTR, !is_signed(type));
      } else {
        GenRet gArg;
        llvm::Argument& llArg = *ai;
        gArg.val = &llArg;
        gArg.chplType = arg->typeInfo();
        GenRet tempVar = createTempVarWith(gArg);

        info->lvt->addValue(arg->cname, tempVar.val,
                            tempVar.isLVPtr, !is_signed(type));
        // debug info for formal arguments
        if(debug_info){
          debug_info->get_formal_arg(arg, ArgNo);
        }
      }
      ++ai;
      ArgNo++;
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
        llvmPrintIrStageNum == llvmStageNum::EVERY)
            && strcmp(llvmPrintIrName, name) == 0)
        printLlvmIr(func, llvmStageNum::NONE);

    // Now run the optimizations on that function.
    // (we handle checking fFastFlag, etc, when we set up FPM_postgen)
    // This way we can potentially keep the fn in cache while it
    // is simplified. The big optos happen later.

    // (note, in particular, the default pass manager's
    //  populateFunctionPassManager does not include vectorization)
    info->FPM_postgen->run(*func);
    if((llvmPrintIrStageNum == llvmStageNum::BASIC ||
        llvmPrintIrStageNum == llvmStageNum::EVERY)
            && strcmp(llvmPrintIrName, name) == 0)
        printLlvmIr(func, llvmStageNum::BASIC);
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
    ret.val = getFunctionLLVM(cname);
    if( ! ret.val ) {
      if( hasFlag(FLAG_EXTERN) ) {
        if( isBuiltinExternCFunction(cname) ) {
          // it's OK.
        } else {
          USR_FATAL("Could not find C function for %s; "
                    " perhaps it is missing or is a macro?", cname);
        }
      } else {
        INT_FATAL("Missing LLVM function for %s", cname);
      }
    }
#endif
  }
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
