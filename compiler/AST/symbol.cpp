/*
 * Copyright 2004-2015 Cray Inc.
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

#include "astutil.h"
#include "stlUtil.h"
#include "bb.h"
#include "build.h"
#include "codegen.h"
#include "docsDriver.h"
#include "expr.h"
#include "files.h"
#include "intlimits.h"
#include "iterator.h"
#include "misc.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "type.h"

// LLVM debugging support
#include "llvmDebug.h"

#include "AstToText.h"
#include "AstVisitor.h"
#include "CollapseBlocks.h"

#include <cstdlib>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <stdint.h>

//
// The function that represents the compiler-generated entry point
//
FnSymbol *chpl_gen_main = NULL;

ModuleSymbol* rootModule = NULL;
ModuleSymbol* theProgram = NULL;
ModuleSymbol* mainModule = NULL;
ModuleSymbol* baseModule = NULL;
ModuleSymbol* standardModule = NULL;
ModuleSymbol* printModuleInitModule = NULL;
Symbol *gNil = NULL;
Symbol *gUnknown = NULL;
Symbol *gMethodToken = NULL;
Symbol *gTypeDefaultToken = NULL;
Symbol *gLeaderTag = NULL, *gFollowerTag = NULL, *gStandaloneTag = NULL;
Symbol *gModuleToken = NULL;
Symbol *gNoInit = NULL;
Symbol *gVoid = NULL;
Symbol *gFile = NULL;
Symbol *gStringC = NULL;
Symbol *gStringCopy = NULL;
Symbol *gOpaque = NULL;
Symbol *gTimer = NULL;
Symbol *gTaskID = NULL;
Symbol *gSyncVarAuxFields = NULL;
Symbol *gSingleVarAuxFields = NULL;
Symbol *gTaskList = NULL;

VarSymbol *gTrue = NULL;
VarSymbol *gFalse = NULL;
VarSymbol *gTryToken = NULL;
VarSymbol *gBoundsChecking = NULL;
VarSymbol *gCastChecking = NULL;
VarSymbol* gPrivatization = NULL;
VarSymbol* gLocal = NULL;
VarSymbol* gNodeID = NULL;
VarSymbol *gModuleInitIndentLevel = NULL;
FnSymbol *gPrintModuleInitFn = NULL;
FnSymbol* gChplHereAlloc = NULL;
FnSymbol* gChplHereFree = NULL;
Symbol *gCLine = NULL;
Symbol *gCFile = NULL;

Map<FnSymbol*,int> ftableMap;
Vec<FnSymbol*> ftableVec;

Map<Type*,Vec<FnSymbol*>*> virtualMethodTable;
Map<FnSymbol*,int> virtualMethodMap;
Map<FnSymbol*,Vec<FnSymbol*>*> virtualChildrenMap;
Map<FnSymbol*,Vec<FnSymbol*>*> virtualRootsMap;

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

Symbol::Symbol(AstTag astTag, const char* init_name, Type* init_type) :
  BaseAST(astTag),
  type(init_type),
  flags(),
  name(astr(init_name)),
  cname(name),
  defPoint(NULL)
{}


Symbol::~Symbol() {
}


void Symbol::verify() {
  if (defPoint && !defPoint->parentSymbol && !toModuleSymbol(this))
    INT_FATAL(this, "Symbol::defPoint is not in AST");
  if (defPoint && this != defPoint->sym)
    INT_FATAL(this, "Symbol::defPoint != Sym::defPoint->sym");
}


bool Symbol::inTree() {
  if (this == rootModule)
    return true;
  if (defPoint)
    return defPoint->inTree();
  else
    return false;
}


Type* Symbol::typeInfo() {
  return type;
}


bool Symbol::isConstant() const {
  return false;
}

bool Symbol::isConstValWillNotChange() const {
  return false;
}

bool Symbol::isParameter() const {
  return false;
}

bool Symbol::isRenameable() const {
  return !(hasFlag(FLAG_EXPORT) || hasFlag(FLAG_EXTERN));
}


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


FnSymbol* Symbol::getFnSymbol() {
  return NULL;
}


bool Symbol::hasFlag(Flag flag) const {
  CHECK_FLAG(flag);
  return flags[flag];
}


void Symbol::addFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.set(flag);
}


void Symbol::copyFlags(const Symbol* other) {
  flags |= other->flags;
}


void Symbol::removeFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.reset(flag);
}

bool Symbol::hasEitherFlag(Flag aflag, Flag bflag) const {
  return hasFlag(aflag) || hasFlag(bflag);
}


bool Symbol::isImmediate() const {
  return false;
}


/******************************** | *********************************
*                                                                   *
* Common base class for ArgSymbol and VarSymbol.                    *
* Also maintains a small amount of IPE specific state.              *
*                                                                   *
********************************* | ********************************/

LcnSymbol::LcnSymbol(AstTag      astTag,
                     const char* initName,
                     Type*       initType) :
  Symbol(astTag, initName, initType)
{
  mDepth  = -1;
  mOffset = -1;
}

LcnSymbol::~LcnSymbol()
{

}

void LcnSymbol::locationSet(int depth, int offset)
{
  mDepth  = depth;
  mOffset = offset;
}

int LcnSymbol::depth() const
{
  return mDepth;
}

int LcnSymbol::offset() const
{
  return mOffset;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

VarSymbol::VarSymbol(const char *init_name,
                     Type    *init_type) :
  LcnSymbol(E_VarSymbol, init_name, init_type),
  immediate(NULL),
  doc(NULL),
  isField(false)
#ifdef HAVE_LLVM
  ,
  llvmDIGlobalVariable(NULL),
  llvmDIVariable(NULL)
#endif
{
  gVarSymbols.add(this);
}


VarSymbol::~VarSymbol() {
  if (immediate)
    delete immediate;
}


void VarSymbol::verify() {
  Symbol::verify();
  if (astTag != E_VarSymbol)
    INT_FATAL(this, "Bad VarSymbol::astTag");
  if (!type)
    INT_FATAL(this, "VarSymbol::type is NULL");
}


VarSymbol*
VarSymbol::copyInner(SymbolMap* map) {
  VarSymbol* newVarSymbol = new VarSymbol(name, type);
  newVarSymbol->copyFlags(this);
  newVarSymbol->cname = cname;
  INT_ASSERT(!newVarSymbol->immediate);
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in VarSymbol::replaceChild");
}


bool VarSymbol::isConstant() const {
  return hasFlag(FLAG_CONST);
}


bool VarSymbol::isConstValWillNotChange() const {
  return hasFlag(FLAG_CONST);
}


bool VarSymbol::isParameter() const {
  return hasFlag(FLAG_PARAM) || immediate;
}


bool VarSymbol::isType() const {
  return hasFlag(FLAG_TYPE_VARIABLE);
}


std::string VarSymbol::docsDirective() {
  std::string result;
  if (fDocsTextOnly) {
    result = "";
  } else {
    // Global type aliases become type directives. Types that are also fields
    // could be generics, so let them be treated as regular fields (i.e. use
    // the attribute directive).
    if (this->isType() && !this->isField) {
      result = ".. type:: ";
    } else if (this->isField) {
      result = ".. attribute:: ";
    } else {
      result = ".. data:: ";
    }
  }
  return this->hasFlag(FLAG_CONFIG) ? result + "config " : result;
}


void VarSymbol::printDocs(std::ostream *file, unsigned int tabs) {
  if (this->hasFlag(FLAG_NO_DOC) || this->hasFlag(FLAG_SUPER_CLASS)) {
      return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();

  if (this->isType()) {
    *file << "type ";
  } else if (this->isConstant()) {
    *file << "const ";
  } else if (this->isParameter()) {
    *file << "param ";
  } else {
    *file << "var ";
  }

  AstToText info;
  info.appendVarDef(this);
  *file << info.text();

  *file << std::endl;

  // For .rst mode, put a line break after the .. data:: directive and
  // its description text.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->doc != NULL) {
    this->printDocsDescription(this->doc, file, tabs + 1);
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


/*
 * For docs, when VarSymbol is used for class fields, identify them as such by
 * calling this function.
 */
void VarSymbol::makeField() {
  this->isField = true;
}


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
              llvm::cast<llvm::StructType>(getTypeLLVM("complex(64)")),
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
              llvm::cast<llvm::StructType>(getTypeLLVM("complex(128)")),
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

        std::string newString = "";
        char nextChar;
        int pos = 0;

        while((nextChar = i->v_string[pos++]) != '\0') {
          if(nextChar != '\\') {
            newString += nextChar;
            continue;
          }

          nextChar = i->v_string[pos++];
          switch(nextChar) {
            case '\'':
            case '\"':
            case '?':
            case '\\':
              newString += nextChar;
              break;
            case 'a':
              newString += '\a';
              break;
            case 'b':
              newString += '\b';
              break;
            case 'f':
              newString += '\f';
              break;
            case 'n':
              newString += '\n';
              break;
            case 'r':
              newString += '\r';
              break;
            case 't':
              newString += '\t';
              break;
            case 'v':
              newString += '\v';
              break;
            case 'x':
              {
                char buf[3];
                long num;
                buf[0] = buf[1] = buf[2] = '\0';
                if( i->v_string[pos] ) buf[0] = i->v_string[pos++];
                if( i->v_string[pos] ) buf[1] = i->v_string[pos++];
                num = strtol(buf, NULL, 16);
                newString += (char) num;
              }
              break;
            default:
              INT_FATAL("Unknown C string escape");
              break;
          }
        }

        ret = info->builder->CreateGlobalString(newString);
      break;
  }

  return ret;
}
#endif

GenRet VarSymbol::codegen() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;
  ret.chplType = typeInfo();

  if( outfile ) {
    if (immediate) {
      ret.isLVPtr = GEN_VAL; //genret.h  GEN_VAL=0, read-only
      if (immediate->const_kind == CONST_KIND_STRING) {
        ret.c += '"';
        ret.c += immediate->v_string;
        ret.c += '"';
      } else if (immediate->const_kind == NUM_KIND_BOOL) {
        ret.c =  immediate->bool_value() ? "true" : "false";
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
        ret.c = '&';
        ret.c += cname;
        ret.isLVPtr = GEN_PTR;
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
              info->builder->CreateConstInBoundsGEP2_32(constString, 0, 0)));
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


void VarSymbol::codegenDefC(bool global) {
  GenInfo* info = gGenInfo;
  if (this->hasFlag(FLAG_EXTERN))
    return;

  if (type == dtVoid)
    return;

  AggregateType* ct = toAggregateType(type);
  std::string typestr =  (this->hasFlag(FLAG_SUPER_CLASS) ?
                          std::string(ct->classStructName(true)) :
                          type->codegen().c);

  //
  // a variable can be codegen'd as static if it is global and neither
  // exported nor external.
  //
  bool isStatic =  global && !hasFlag(FLAG_EXPORT) && !hasFlag(FLAG_EXTERN);

  std::string str = (isStatic ? "static " : "") + typestr + " " + cname;
  if (ct) {
    if (ct->isClass()) {
      if (isFnSymbol(defPoint->parentSymbol)) {
        str += " = NULL";
      }
    } else if (ct->symbol->hasFlag(FLAG_WIDE_REF) ||
               ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (isFnSymbol(defPoint->parentSymbol)) {
        if( widePointersStruct || isWideString(ct) ) {

          //
          // CHPL_LOCALEID_T_INIT is #defined in the chpl-locale-model.h
          // file in the runtime, for the selected locale model.
          //
          str += " = {CHPL_LOCALEID_T_INIT, NULL}";
        } else {
          str += " = ((wide_ptr_t) NULL)";
        }
      }
    }
  }
  info->cLocalDecls.push_back(str);
}

void VarSymbol::codegenGlobalDef() {
  GenInfo* info = gGenInfo;

  if( breakOnCodegenCname[0] &&
      0 == strcmp(cname, breakOnCodegenCname) ) {
    gdbShouldBreakHere();
  }

  if( info->cfile ) {
    codegenDefC(/*global=*/true);
  } else {
#ifdef HAVE_LLVM
    if(type == dtVoid) {
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

      //------------------added by Hui Zhang---------------------------//
      if(debug_info){
	debug_info->get_global_variable(this);
      }
      //---------------------------------------------------------------//
    }
#endif
  }
}

void VarSymbol::codegenDef() {
  GenInfo* info = gGenInfo;

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
                info->builder->CreateConstInBoundsGEP2_32(globalString, 0, 0)));
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
                info->builder->CreateConstInBoundsGEP1_32(globalString, 0)));
        }
      } else {
        globalValue->setInitializer(llvm::cast<llvm::Constant>(
              codegenImmediateLLVM(immediate)));
      }
//      //-------------added by Hui Zhang-------------------//
//      if(debug_info){
//	debug_info->get_global_variable(this);
//      }
      //---------------------------------------------------//
      info->lvt->addGlobalValue(cname, globalValue, GEN_VAL, ! is_signed(type));
    }
    llvm::Type *varType = type->codegen().type;
    //////////////////////////////////////////////
    //printf("Creating variable: %s\n",cname);
    /////////////////////////////////////////////
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
    //-------------added by Hui Zhang-------------------------//
    if(debug_info){
      debug_info->get_variable(this);
    }
    //--------------------------------------------------------//
#endif
  }
}


bool VarSymbol::isImmediate() const {
  return immediate != NULL;
}

void VarSymbol::accept(AstVisitor* visitor) {
  visitor->visitVarSym(this);
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

ArgSymbol::ArgSymbol(IntentTag iIntent, const char* iName,
                     Type* iType, Expr* iTypeExpr,
                     Expr* iDefaultExpr, Expr* iVariableExpr) :
  LcnSymbol(E_ArgSymbol, iName, iType),
  intent(iIntent),
  typeExpr(NULL),
  defaultExpr(NULL),
  variableExpr(NULL),
  instantiatedFrom(NULL)
#ifdef HAVE_LLVM
  ,
  llvmDIFormal(NULL)
#endif
{
  if (intentsResolved) {
    if (iIntent == INTENT_BLANK || iIntent == INTENT_CONST) {
      INT_FATAL(this, "You can't create an argument with blank/const intent once intents have been resolved; please be more specific");
      // NOTE: One way to be more specific is to use the blankIntentForType()/
      // constIntentForType() routines to map a (possibly unknown) type to
      // the intent that blank/const would use for that type.
    }
  }
  if (!iTypeExpr)
    typeExpr = NULL;
  else if (BlockStmt* block = toBlockStmt(iTypeExpr))
    typeExpr = block;
  else
    typeExpr = new BlockStmt(iTypeExpr, BLOCK_TYPE);
  if (!iDefaultExpr)
    defaultExpr = NULL;
  else if (BlockStmt* block = toBlockStmt(iDefaultExpr))
    defaultExpr = block;
  else
    defaultExpr = new BlockStmt(iDefaultExpr, BLOCK_SCOPELESS);
  if (!iVariableExpr)
    variableExpr = NULL;
  else if (BlockStmt* block = toBlockStmt(iVariableExpr))
    variableExpr = block;
  else
    variableExpr = new BlockStmt(iVariableExpr, BLOCK_SCOPELESS);
  gArgSymbols.add(this);
}


void ArgSymbol::verify() {
  Symbol::verify();
  if (astTag != E_ArgSymbol) {
    INT_FATAL(this, "Bad ArgSymbol::astTag");
  }
  if (typeExpr && typeExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::typeExpr::parentSymbol");
  if (defaultExpr && defaultExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::defaultExpr::parentSymbol");
  if (variableExpr && variableExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::variableExpr::parentSymbol");
  // ArgSymbols appear only in formal parameter lists.
  // If this one has a successor but the successor is not an argsymbol
  // the formal parameter list is corrupted.
  if (defPoint && defPoint->next &&
        (!toDefExpr(defPoint->next)->sym ||
         !toArgSymbol(toDefExpr(defPoint->next)->sym)))
    INT_FATAL(this, "Bad ArgSymbol::defPoint->next");
  if (intentsResolved) {
    if (intent == INTENT_BLANK || intent == INTENT_CONST) {
      INT_FATAL(this, "Arg '%s' (%d) has blank/const intent post-resolve", this->name, this->id);
    }
  }
}


ArgSymbol*
ArgSymbol::copyInner(SymbolMap* map) {
  ArgSymbol *ps = new ArgSymbol(intent, name, type, COPY_INT(typeExpr),
                                COPY_INT(defaultExpr), COPY_INT(variableExpr));
  ps->copyFlags(this);
  ps->cname = cname;
  ps->instantiatedFrom = instantiatedFrom;
  return ps;
}


void ArgSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == typeExpr)
    typeExpr = toBlockStmt(new_ast);
  else if (old_ast == defaultExpr)
    defaultExpr = toBlockStmt(new_ast);
  else if (old_ast == variableExpr)
    variableExpr = toBlockStmt(new_ast);
  else
    INT_FATAL(this, "Unexpected case in ArgSymbol::replaceChild");
}

bool argMustUseCPtr(Type* type) {
  if (isWideString(type))
    return false;
  if (isRecord(type) || isUnion(type))
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


bool ArgSymbol::isConstant() const {
  if (intent == INTENT_CONST_IN || intent == INTENT_CONST_REF) {
    return true;
  }
  return (intent == INTENT_BLANK || intent == INTENT_CONST) &&
    !isReferenceType(type) &&
    !isSyncType(type) &&
    !isRecordWrappedType(type) /* array, domain, distribution */;
}

bool ArgSymbol::isConstValWillNotChange() const {
  //
  // This is written to only be called post resolveIntents
  //
  assert (intent != INTENT_BLANK && intent != INTENT_CONST);
  return (intent == INTENT_CONST_IN);
}



bool ArgSymbol::isParameter() const {
  return (intent == INTENT_PARAM);
}


const char* retTagDescrString(RetTag retTag) {
  switch (retTag) {
    case RET_VALUE: return "value";
    case RET_REF:   return "ref";
    case RET_PARAM: return "param";
    case RET_TYPE:  return "type";
    default:        return "<unknown RetTag>";
  }
}


const char* modTagDescrString(ModTag modTag) {
  switch (modTag) {
    case MOD_INTERNAL:  return "internal";
    case MOD_STANDARD:  return "standard";
    case MOD_USER:      return "user";
    default:            return "<unknown ModTag>";
  }
}


// describes this argument's intent (for use in an English sentence)
const char* ArgSymbol::intentDescrString(void) {
  switch (intent) {
    case INTENT_BLANK: return "blank intent";
    case INTENT_IN: return "'in'";
    case INTENT_INOUT: return "'inout'";
    case INTENT_OUT: return "'out'";
    case INTENT_CONST: return "'const'";
    case INTENT_CONST_IN: return "'const in'";
    case INTENT_CONST_REF: return "'const ref'";
    case INTENT_REF: return "'ref'";
    case INTENT_PARAM: return "'param'";
    case INTENT_TYPE: return "'type'";
  }

  INT_FATAL(this, "unknown intent");
  return "unknown intent";
}

// describes the given intent (for use in an English sentence)
const char* intentDescrString(IntentTag intent) {
  switch (intent) {
    case INTENT_BLANK:     return "blank intent";
    case INTENT_IN:        return "'in' intent";
    case INTENT_INOUT:     return "'inout' intent";
    case INTENT_OUT:       return "'out' intent";
    case INTENT_CONST:     return "'const' intent";
    case INTENT_CONST_IN:  return "'const in' intent";
    case INTENT_CONST_REF: return "'const ref' intent";
    case INTENT_REF:       return "'ref' intent";
    case INTENT_PARAM:     return "'param' intent";
    case INTENT_TYPE:      return "'type' intent";
    default:               return "<unknown intent>";
  }
}


GenRet ArgSymbol::codegenType() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;
  if( outfile ) {
    ret.c = type->codegen().c;
    if (requiresCPtr())
      ret.c += "* const";
  } else {
#ifdef HAVE_LLVM
    llvm::Type *argType = type->codegen().type;
    if(requiresCPtr()) {
      argType = argType->getPointerTo();
    }
    ret.type = argType;
#endif
  }
  return ret;
}

GenRet ArgSymbol::codegen() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

  if( outfile ) {
    ret.c = '&';
    ret.c += cname;
    ret.isLVPtr = GEN_PTR;
  } else {
#ifdef HAVE_LLVM
    ret = info->lvt->getValue(cname);
#endif
  }

  if( requiresCPtr() ) {
    // Don't try to use chplType.
    ret.chplType = NULL;
    ret = codegenLocalDeref(ret);
  }

  ret.chplType = typeInfo();

  return ret;
}

void ArgSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterArgSym(this) == true) {

    if (typeExpr)
      typeExpr->accept(visitor);

    if (defaultExpr)
      defaultExpr->accept(visitor);

    if (variableExpr)
      variableExpr->accept(visitor);

    visitor->exitArgSym(this);
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

TypeSymbol::TypeSymbol(const char* init_name, Type* init_type) :
  Symbol(E_TypeSymbol, init_name, init_type),
#ifdef HAVE_LLVM
    llvmType(NULL),
    llvmTbaaNode(NULL), llvmConstTbaaNode(NULL),
    llvmTbaaStructNode(NULL), llvmConstTbaaStructNode(NULL),
    llvmDIType(NULL),
#endif
    doc(NULL)
{
  addFlag(FLAG_TYPE_VARIABLE);
  if (!type)
    INT_FATAL(this, "TypeSymbol constructor called without type");
  type->addSymbol(this);
  gTypeSymbols.add(this);
}


void TypeSymbol::verify() {
  Symbol::verify();
  if (astTag != E_TypeSymbol) {
    INT_FATAL(this, "Bad TypeSymbol::astTag");
  }
  if (type->symbol != this)
    INT_FATAL(this, "TypeSymbol::type->symbol != TypeSymbol");
}


TypeSymbol*
TypeSymbol::copyInner(SymbolMap* map) {
  Type* new_type = COPY_INT(type);
  TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
  new_type->addSymbol(new_type_symbol);
  new_type_symbol->copyFlags(this);
  new_type_symbol->cname = cname;
  return new_type_symbol;
}


void TypeSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in TypeSymbol::replaceChild");
}


void TypeSymbol::codegenPrototype() {
  if (!hasFlag(FLAG_EXTERN)) {
    type->codegenPrototype();
  }
}


void TypeSymbol::codegenDef() {
  GenInfo *info = gGenInfo;

  if( breakOnCodegenCname[0] &&
      0 == strcmp(cname, breakOnCodegenCname) ) {
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
  if( llvmTbaaNode ) return;

  GenInfo* info = gGenInfo;
  llvm::LLVMContext& ctx = info->module->getContext();
  // Create the TBAA root node if necessary.
  if( ! info->tbaaRootNode ) {
    LLVM_METADATA_OPERAND_TYPE* Ops[1];
    Ops[0] = llvm::MDString::get(ctx, "Chapel types");
    info->tbaaRootNode = llvm::MDNode::get(ctx, Ops);
  }

  // Set the llvmTbaaNode to non-NULL so that we can
  // avoid recursing.
  llvmTbaaNode = info->tbaaRootNode;

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
  llvm::MDNode* constParent = info->tbaaRootNode;
  if( superType ) {
    parent = superType->symbol->llvmTbaaNode;
    constParent = superType->symbol->llvmConstTbaaNode;
    INT_ASSERT( parent );
    INT_ASSERT( constParent );
  }

  // Ref and _ddata are really the same, and can conceivably
  // alias, so we normalize _ddata to be ref for the purposes of TBAA.
  if( hasFlag(FLAG_DATA_CLASS) ) {
    Type* eltType = getDataClassType(this)->typeInfo();
    Type* refType = getOrMakeRefTypeDuringCodegen(eltType);
    refType->symbol->codegenMetadata();
    this->llvmTbaaNode = refType->symbol->llvmTbaaNode;
    this->llvmConstTbaaNode = refType->symbol->llvmConstTbaaNode;
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
      LLVM_METADATA_OPERAND_TYPE* Ops[2];
      Ops[0] = llvm::MDString::get(ctx, cname);
      Ops[1] = parent;
      llvmTbaaNode = llvm::MDNode::get(ctx, Ops);
    }
    {
      LLVM_METADATA_OPERAND_TYPE* Ops[3];
      Ops[0] = llvm::MDString::get(ctx, cname);
      Ops[1] = constParent;
      Ops[2] = llvm_constant_as_metadata(
                   llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), 1));
      llvmConstTbaaNode = llvm::MDNode::get(ctx, Ops);
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
    llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE*, 16> Ops;
    llvm::SmallVector<LLVM_METADATA_OPERAND_TYPE*, 16> ConstOps;

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
      if( ct ) {
        unsigned gep = ct->getMemberGEP(field->cname);
        llvm::Constant* off = llvm::ConstantExpr::getOffsetOf(struct_type, gep);
        llvm::Constant* sz = llvm::ConstantExpr::getSizeOf(fieldType);
        Ops.push_back(llvm_constant_as_metadata(off));
        Ops.push_back(llvm_constant_as_metadata(sz));
        Ops.push_back(field->type->symbol->llvmTbaaNode);
        ConstOps.push_back(llvm_constant_as_metadata(off));
        ConstOps.push_back(llvm_constant_as_metadata(sz));
        ConstOps.push_back(field->type->symbol->llvmConstTbaaNode);
        llvmTbaaStructNode = llvm::MDNode::get(ctx, Ops);
        llvmConstTbaaStructNode = llvm::MDNode::get(ctx, ConstOps);
      }
    }
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
       codegenMetadata(); //TODO -- enable TBAA generation in the future.
    }
    ret.type = llvmType;
#endif
  }

  return ret;
}

void TypeSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterTypeSym(this) == true) {

    if (type)
      type->accept(visitor);

    visitor->exitTypeSym(this);
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

FnSymbol::FnSymbol(const char* initName) :
  Symbol(E_FnSymbol, initName),
  formals(),
  setter(NULL),
  retType(dtUnknown),
  where(NULL),
  retExprType(NULL),
  body(new BlockStmt()),
  thisTag(INTENT_BLANK),
  retTag(RET_VALUE),
  iteratorInfo(NULL),
  _this(NULL),
  _outer(NULL),
  instantiatedFrom(NULL),
  instantiationPoint(NULL),
  basicBlocks(NULL),
  calledBy(NULL),
  userString(NULL),
  valueFunction(NULL),
  codegenUniqueNum(1),
  doc(NULL),
  partialCopySource(NULL),
  retSymbol(NULL)
#ifdef HAVE_LLVM
  ,
  llvmDISubprogram(NULL)
#endif
{
  substitutions.clear();
  gFnSymbols.add(this);
  formals.parent = this;
}


FnSymbol::~FnSymbol() {
  if (iteratorInfo)
    delete iteratorInfo;
  BasicBlock::clear(this);
  delete basicBlocks; basicBlocks = 0;
  if (calledBy)
    delete calledBy;
}


void FnSymbol::verify() {
  Symbol::verify();
  if (astTag != E_FnSymbol) {
    INT_FATAL(this, "Bad FnSymbol::astTag");
  }
  if (_this && _this->defPoint->parentSymbol != this)
    INT_FATAL(this, "Each method must contain a 'this' declaration.");
  if (normalized) {
    CallExpr* last = toCallExpr(body->body.last());
    if (!last || !last->isPrimitive(PRIM_RETURN))
      INT_FATAL(this, "Last statement in normalized function is not a return");
  }
  if (formals.parent != this)
    INT_FATAL(this, "Bad AList::parent in FnSymbol");
  if (where && where->parentSymbol != this)
    INT_FATAL(this, "Bad FnSymbol::where::parentSymbol");
  if (retExprType && retExprType->parentSymbol != this)
    INT_FATAL(this, "Bad FnSymbol::retExprType::parentSymbol");
  if (body && body->parentSymbol != this)
    INT_FATAL(this, "Bad FnSymbol::body::parentSymbol");
}


FnSymbol* FnSymbol::getFnSymbol(void) {
  return this;
}


FnSymbol*
FnSymbol::copyInner(SymbolMap* map) {
  // Copy members that are common to innerCopy and partialCopy.
  FnSymbol* copy = this->copyInnerCore(map);

  // Copy members that weren't set by copyInnerCore.
  copy->setter      = COPY_INT(this->setter);
  copy->where       = COPY_INT(this->where);
  copy->body        = COPY_INT(this->body);
  copy->retExprType = COPY_INT(this->retExprType);
  copy->_this       = this->_this;

  return copy;
}


/** Copy over members common to both copyInner and partialCopy.
 *
 * \param map Map from symbols in the old function to symbols in the new one
 */
FnSymbol*
FnSymbol::copyInnerCore(SymbolMap* map) {
  FnSymbol* newFn = new FnSymbol(this->name);

  /* Copy the flags.
   *
   * TODO: See if it is necessary to copy flags both here and in the copy
   * method.
   */
  newFn->copyFlags(this);

  for_formals(formal, this) {
    newFn->insertFormalAtTail(COPY_INT(formal->defPoint));
  }

  // Copy members that are needed by both copyInner and partialCopy.
  newFn->partialCopySource  = this;
  newFn->astloc             = this->astloc;
  newFn->retType            = this->retType;
  newFn->thisTag            = this->thisTag;
  newFn->cname              = this->cname;
  newFn->_outer             = this->_outer;
  newFn->retTag             = this->retTag;
  newFn->instantiatedFrom   = this->instantiatedFrom;
  newFn->instantiationPoint = this->instantiationPoint;
  newFn->numPreTupleFormals = this->numPreTupleFormals;

  return newFn;
}

/** Copy just enough of the AST to get through filter candidate and
 *  disambiguate-by-match.
 *
 * This function selectively copies portions of the function's AST
 * representation.  The goal here is to copy exactly as many nodes as are
 * necessary to determine if a function is the best candidate for resolving a
 * call site and no more.  Special handling is necessary for the _this, where,
 * and retExprType members.  In addition, the return symbol needs to be made
 * available despite the fact that we have skipped copying the body.
 *
 * \param map Map from symbols in the old function to symbols in the new one
 */
FnSymbol* FnSymbol::partialCopy(SymbolMap* map) {
  FnSymbol* newFn = this->copyInnerCore(map);

  if (this->_this == NULL) {
    // Case 1: No _this pointer.
    newFn->_this = NULL;

  } else if (Symbol* replacementThis = map->get(this->_this)) {
    // Case 2: _this symbol is defined as one of the formal arguments.
    newFn->_this = replacementThis;

  } else {
    /*
     * Case 3: _this symbol is defined in the function's body.  A new symbol is
     * created.  This symbol will have to be used to replace some of the symbols
     * generated from copying the function's body during finalizeCopy.
     */
    newFn->_this           = this->_this->copy(map);
    newFn->_this->defPoint = new DefExpr(newFn->_this,
                                         COPY_INT(this->_this->defPoint->init),
                                         COPY_INT(this->_this->defPoint->exprType));
  }

  // Copy and insert the where clause if it is present.
  if (this->where != NULL) {
    newFn->where = COPY_INT(this->where);
    insert_help(newFn->where, NULL, newFn);
  }

  // Copy and insert the retExprType if it is present.
  if (this->retExprType != NULL) {
    newFn->retExprType = COPY_INT(this->retExprType);
    insert_help(newFn->retExprType, NULL, newFn);
  }

  /*
   * Because we are not copying the function's body we need to make the return
   * symbol available through other means.  To do this we first have to find
   * where the symbol is defined.  It may either be void, the _this symbol, a
   * formal symbol, or a symbol defined in the function's body.  In the last
   * case a new symbol and definition point have to be generated; the
   * finalizeCopy method will replace their corresponding nodes from the body
   * appropriately.
   */
  if (this->getReturnSymbol() == gVoid) {
    // Case 1: Function returns void.
    newFn->retSymbol = gVoid;

  } else if (this->getReturnSymbol() == this->_this) {
    // Case 2: Function returns _this.
    newFn->retSymbol = newFn->_this;

  } else if (Symbol* replacementRet = map->get(this->getReturnSymbol())) {
    // Case 3: Function returns a formal argument.
    newFn->retSymbol = replacementRet;

  } else {
    // Case 4: Function returns a symbol defined in the body.
    newFn->retSymbol = COPY_INT(this->getReturnSymbol());

    newFn->retSymbol->defPoint = new DefExpr(newFn->retSymbol,
                                             COPY_INT(this->getReturnSymbol()->defPoint->init),
                                             COPY_INT(this->getReturnSymbol()->defPoint->exprType));

    update_symbols(newFn->retSymbol, map);
  }

  // Add a map entry from this FnSymbol to the newly generated one.
  map->put(this, newFn);
  // Update symbols in the sub-AST as is appropriate.
  update_symbols(newFn, map);

  // Copy over the partialCopyMap, to be used later in finalizeCopy.
  newFn->partialCopyMap.copy(*map);

  /*
   * Add the PARTIAL_COPY flag so we will know if we need to instantiate its
   * body later.
   */
  newFn->addFlag(FLAG_PARTIAL_COPY);

  return newFn;
}

/** Finish copying the function's AST after a partial copy.
 *
 * This function finishes the work started by partialCopy.  This involves
 * copying the setter and body, and repairing some inconsistencies in the
 * copied body.
 *
 * \param map Map from symbols in the old function to symbols in the new one
 */
void FnSymbol::finalizeCopy(void) {
  if (this->hasFlag(FLAG_PARTIAL_COPY)) {

    // Make sure that the source has been finalized.
    this->partialCopySource->finalizeCopy();

    SET_LINENO(this);

    // Retrieve our old/new symbol map from the partial copy process.
    SymbolMap* map = &(this->partialCopyMap);

    this->setter = COPY_INT(this->partialCopySource->setter);

    /*
     * When we reach this point we will be in one of three scenarios:
     *  1) The function's body is empty and needs to be copied over from the
     *     copy source.
     *  2) The function's body has been replaced and we don't need to do
     *     anything else.
     *  3) The function has had varargs expanded and we need to copy over the
     *     added statements from the old block to a new copy of the body from
     *     the source.
     */
    if (this->hasFlag(FLAG_EXPANDED_VARARGS)) {
      // Alias the old body and make a new copy of the body from the source.
      BlockStmt* varArgNodes = this->body;
      this->body             = COPY_INT(this->partialCopySource->body);

      /*
       * Iterate over the statements that have been added to the function body
       * and add them to the new body.
       */
      for_alist_backward(node, varArgNodes->body) {
        remove_help(node, false);
        node->list = NULL;
        this->body->insertAtHead(node);
      }

      // Clean up blocks that aren't going to be used any more.
      gBlockStmts.remove(gBlockStmts.index(varArgNodes));
      delete varArgNodes;

      this->removeFlag(FLAG_EXPANDED_VARARGS);

    } else if (this->body->body.length == 0) {
      gBlockStmts.remove(gBlockStmts.index(this->body));
      delete this->body;

      this->body = COPY_INT(this->partialCopySource->body);
    }

    Symbol* replacementThis = map->get(this->partialCopySource->_this);

    /*
     * Two cases may arise here.  The first is when the _this symbol is defined
     * in the formal arguments.  In this case no additional work needs to be
     * done.  In the second case the function's _this symbol is defined in the
     * function's body.  In this case we need to repair the new/old symbol map
     * and replace the definition point in the body with our existing def point.
     */
    if (replacementThis != this->_this) {
      /*
       * In Case 2:
       * this->partialCopySource->_this := A
       * this->_this                    := B
       *
       * map[A] := C
       */

      // Set map[A] := B
      map->put(this->partialCopySource->_this, this->_this);
      // Set map[C] := B
      map->put(replacementThis, this->_this);

      // Replace the definition of _this in the body: def(C) -> def(B)
      replacementThis->defPoint->replace(this->_this->defPoint);
    }

    /*
     * Cases where the return symbol is gVoid or this->_this don't require any
     * additional actions.
     */
    if (this->retSymbol != gVoid && this->retSymbol != this->_this) {
      Symbol* replacementRet = map->get(this->partialCopySource->getReturnSymbol());

      if (replacementRet != this->retSymbol) {
        /*
         * We now know that retSymbol is defined in function's body.  We must
         * now replace the duplicate symbol and its definition point with the
         * ones generated in partialCopy.  This is the exact same process as
         * was done above for the _this symbol.
         */
        replacementRet->defPoint->replace(this->retSymbol->defPoint);

        map->put(this->partialCopySource->getReturnSymbol(), this->retSymbol);
        map->put(replacementRet, this->retSymbol);
      }
    }

    /*
     * Null out the return symbol so that future changes to the return symbol
     * will be reflected in calls to getReturnSymbol().
     */
    this->retSymbol = NULL;

    // Repair broken up-pointers.
    insert_help(this, this->defPoint, this->defPoint->parentSymbol);

    /*
     * Update all old symbols left in the function's AST with their appropriate
     * replacements.
     */
    update_symbols(this, map);

    // Clean up book keeping information.
    this->partialCopyMap.clear();
    this->partialCopySource = NULL;
    this->removeFlag(FLAG_PARTIAL_COPY);
  }
}


void FnSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = toBlockStmt(new_ast);
  } else if (old_ast == where) {
    where = toBlockStmt(new_ast);
  } else if (old_ast == setter) {
    setter = toDefExpr(new_ast);
  } else if (old_ast == retExprType) {
    retExprType = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in FnSymbol::replaceChild");
  }
}

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
        if (formal->defPoint == formals.head && hasFlag(FLAG_ON_BLOCK))
          continue; // do not print locale argument for on blocks
        if (count > 0)
          str += ", ";
        str += formal->codegenType().c;
        if( forHeader ) {
          str += " ";
          str += formal->cname;
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
      if (formal->defPoint == formals.head && hasFlag(FLAG_ON_BLOCK))
        continue; // do not print locale argument for on blocks
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
    fprintf(outfile, "/* %7d */ ", id);
  // Prepend function header with necessary __global__ declaration

  //
  // A function prototype can be labeled static if it is neither
  // exported nor external
  //
  if (!hasFlag(FLAG_EXPORT) && !hasFlag(FLAG_EXTERN)) {
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

  if( breakOnCodegenCname[0] &&
      0 == strcmp(cname, breakOnCodegenCname) ) {
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
    for_formals(arg, this) {
      if(arg->defPoint == formals.head && hasFlag(FLAG_ON_BLOCK)) {
        continue;
      }
      argumentTypes.push_back(arg->codegenType().type);
      argumentNames.push_back(arg->cname);
      numArgs++;
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

  if( breakOnCodegenCname[0] &&
      0 == strcmp(cname, breakOnCodegenCname) ) {
    gdbShouldBreakHere();
  }

  if( hasFlag(FLAG_NO_CODEGEN) ) return;

  info->cStatements.clear();
  info->cLocalDecls.clear();

  if( outfile ) {
    if (strcmp(saveCDir, "")) {
     if (const char* rawname = fname()) {
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

    llvm::BasicBlock *block =
      llvm::BasicBlock::Create(info->module->getContext(), "entry", func);

    info->builder->SetInsertPoint(block);

    info->lvt->addLayer();

    printf("LLVM GENERATING FUNCTION\n");
    if(debug_info) {
      printf("LLVM DEBUG_INFO BLOCK\n");
      llvm::DISubprogram dbgScope = debug_info->get_function(this);
      info->builder->SetCurrentDebugLocation(
        llvm::DebugLoc::get(linenum(),0,dbgScope));
    }

    llvm::Function::arg_iterator ai = func->arg_begin();
    unsigned int ArgNo = 1; //start from 1 to cooperate with createLocalVariable
    for_formals(arg, this) {
      if (arg->defPoint == formals.head && hasFlag(FLAG_ON_BLOCK))
        continue; // do not print locale argument for on blocks

      if (arg->requiresCPtr()){
        info->lvt->addValue(arg->cname, ai,  GEN_PTR, !is_signed(type));
      } else {
        GenRet gArg;
        gArg.val = ai;
        gArg.chplType = arg->typeInfo();
        GenRet tempVar = createTempVarWith(gArg);

        info->lvt->addValue(arg->cname, tempVar.val,
                            tempVar.isLVPtr, !is_signed(type));
	// Added by Hui: debug info for formal arguments
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
          if (fGenIDS && isVarSymbol(def->sym))
            genIdComment(def->sym->id);
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
      bool problems;
#if HAVE_LLVM_VER >= 35
      problems = llvm::verifyFunction(*func, &llvm::errs());
#else
      problems = llvm::verifyFunction(*func, llvm::PrintMessageAction);
#endif
      if( problems ) {
        INT_FATAL("LLVM function verification failed");
      }
    }
    // Now run the optimizations on that function.
    // (we handle checking fFastFlag, etc, when we set up FPM_postgen)
    // This way we can potentially keep the fn in cache while it
    // is simplified. The big optos happen later.
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

void
FnSymbol::insertAtHead(Expr* ast) {
  body->insertAtHead(ast);
}


void
FnSymbol::insertAtTail(Expr* ast) {
  body->insertAtTail(ast);
}


void
FnSymbol::insertAtHead(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtHead(new_Expr(format, args));
  va_end(args);
}


void
FnSymbol::insertAtTail(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtTail(new_Expr(format, args));
  va_end(args);
}


Symbol*
FnSymbol::getReturnSymbol() {
  if (this->retSymbol != NULL) {
    return this->retSymbol;

  } else {
    CallExpr* ret = toCallExpr(body->body.last());
    if (!ret || !ret->isPrimitive(PRIM_RETURN))
      INT_FATAL(this, "function is not normal");
    SymExpr* sym = toSymExpr(ret->get(1));
    if (!sym)
      INT_FATAL(this, "function is not normal");
    return sym->var;
  }
}


// Replace the return symbol with 'newRetSymbol',
// return the previous return symbol.
// If newRetType != NULL, also update fn->retType.
Symbol*
FnSymbol::replaceReturnSymbol(Symbol* newRetSymbol, Type* newRetType)
{
  // follows getReturnSymbol()
  CallExpr* ret = toCallExpr(this->body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  SymExpr* sym = toSymExpr(ret->get(1));
  if (!sym)
    INT_FATAL(this, "function is not normal");
  Symbol* prevRetSymbol = sym->var;

  // updating
  sym->var = newRetSymbol;
  this->retSymbol = newRetSymbol;
  if (newRetType)
    this->retType = newRetType;

  return prevRetSymbol;
}


void
FnSymbol::insertBeforeReturn(Expr* ast) {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  Expr* last = ret;
  if (DefExpr* def = toDefExpr(last->prev))
    if (toLabelSymbol(def->sym))
      last = last->prev; // label before return
  last->insertBefore(ast);
}


void
FnSymbol::insertBeforeReturnAfterLabel(Expr* ast) {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  ret->insertBefore(ast);
}


void
FnSymbol::insertBeforeDownEndCount(Expr* ast) {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  CallExpr* last = toCallExpr(ret->prev);
  if (!last || strcmp(last->isResolved()->name, "_downEndCount"))
    INT_FATAL(last, "Expected call to _downEndCount");
  last->insertBefore(ast);
}


void
FnSymbol::insertFormalAtHead(BaseAST* ast) {
  if (ArgSymbol* arg = toArgSymbol(ast))
    formals.insertAtHead(new DefExpr(arg));
  else if (DefExpr* def = toDefExpr(ast))
    formals.insertAtHead(def);
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtHead");
}


void
FnSymbol::insertFormalAtTail(BaseAST* ast) {
  if (ArgSymbol* arg = toArgSymbol(ast))
    formals.insertAtTail(new DefExpr(arg));
  else if (DefExpr* def = toDefExpr(ast))
    formals.insertAtTail(def);
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtTail");
}


int
FnSymbol::numFormals() const {
  return formals.length;
}


ArgSymbol*
FnSymbol::getFormal(int i) {
  return toArgSymbol(toDefExpr(formals.get(i))->sym);
}

void
FnSymbol::collapseBlocks() {
  CollapseBlocks visitor;

  body->accept(&visitor);
}

//
// returns 1 if generic
// returns 2 if they all have defaults
//
static int
hasGenericArgs(FnSymbol* fn) {
  bool isGeneric = false;
  bool hasGenericDefaults = true;
  for_formals(formal, fn) {
    if ((formal->type->symbol->hasFlag(FLAG_GENERIC) &&
         (!formal->type->hasGenericDefaults ||
          formal->hasFlag(FLAG_MARKED_GENERIC) ||
          formal == fn->_this ||
          formal->hasFlag(FLAG_IS_MEME))) ||
        formal->intent == INTENT_PARAM) {
      isGeneric = true;
      if (!formal->defaultExpr)
        hasGenericDefaults = false;
    }
  }
  if (isGeneric && !hasGenericDefaults)
    return 1;
  else if (isGeneric && hasGenericDefaults)
    return 2;
  else
    return 0;
}


// Tag the given function as generic.
// Returns true if there was a change, false otherwise.
bool FnSymbol::tag_generic() {
  if (hasFlag(FLAG_GENERIC))
    return false;  // Already generic, no change.

  if (int result = hasGenericArgs(this)) {
    // This function has generic arguments, so mark it as generic.
    addFlag(FLAG_GENERIC);

    // If the return type is not completely unknown (which is generic enough)
    // and this function is a type constructor function,
    // then mark its return type as generic.
    if (retType != dtUnknown && hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
      retType->symbol->addFlag(FLAG_GENERIC);
      if (result == 2)
        retType->hasGenericDefaults = true;
    }
    return true;
  }
  return false;
}

bool FnSymbol::isResolved() const {
  return hasFlag(FLAG_RESOLVED);
}

void FnSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterFnSym(this) == true) {

    for_alist(next_ast, formals) {
      next_ast->accept(visitor);
    }

    if (setter)
      setter->accept(visitor);

    if (body)
      body->accept(visitor);

    if (where)
      where->accept(visitor);

    if (retExprType) {
      retExprType->accept(visitor);
    }

    visitor->exitFnSym(this);
  }
}

// This function is a method on an aggregate type
bool FnSymbol::isMethod() const {
  return hasFlag(FLAG_METHOD);
}

// This function is a method on an aggregate type, defined within its
// declaration
bool FnSymbol::isPrimaryMethod() const {
  return hasFlag(FLAG_METHOD_PRIMARY);
}

// This function is a method on an aggregate type, defined outside its
// definition
bool FnSymbol::isSecondaryMethod() const {
  return isMethod() && !isPrimaryMethod();
}


// This function or method is an iterator (as opposed to a procedure).
bool FnSymbol::isIterator() const {
  return hasFlag(FLAG_ITERATOR_FN);
}


std::string FnSymbol::docsDirective() {
  if (fDocsTextOnly) {
    return "";
  }

  if (this->isMethod() && this->isIterator()) {
    return ".. itermethod:: ";
  } else if (this->isIterator()) {
    return ".. iterfunction:: ";
  } else if (this->isMethod()) {
    return ".. method:: ";
  } else {
    return ".. function:: ";
  }
}


void FnSymbol::printDocs(std::ostream *file, unsigned int tabs) {
  if (this->hasFlag(FLAG_NO_DOC)) {
    return;
  }

  // Print the rst directive, if one is needed.
  this->printTabs(file, tabs);
  *file << this->docsDirective();

  // Print export. Externs do not get a prefix, since the user doesn't
  // care whether it's an extern or not (they just want to use the function).
  // Inlines don't get a prefix for symmetry in modules like Math.chpl and
  // due to the argument that it's of negligible value in most cases.
  if (this->hasFlag(FLAG_EXPORT)) {
    *file << "export ";
  }

  // Print iter/proc.
  if (this->isIterator()) {
    *file << "iter ";
  } else {
    *file << "proc ";
  }

  // Print name and arguments.
  AstToText info;
  info.appendNameAndFormals(this);
  *file << info.text();

  // Print return intent, if one exists.
  switch (this->retTag) {
  case RET_REF:
    *file << " ref";
    break;
  case RET_PARAM:
    *file << " param";
    break;
  case RET_TYPE:
    *file << " type";
    break;
  default:
    break;
  }

  // Print return type.
  if (this->retExprType != NULL) {
    *file << ": ";
    this->retExprType->body.tail->prettyPrint(file);
  }
  *file << std::endl;

  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->doc != NULL) {
    this->printDocsDescription(this->doc, file, tabs + 1);
    *file << std::endl;
  }
}


/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

EnumSymbol::EnumSymbol(const char* init_name) :
  Symbol(E_EnumSymbol, init_name)
{
  gEnumSymbols.add(this);
}


void EnumSymbol::verify() {
  Symbol::verify();
  if (astTag != E_EnumSymbol) {
    INT_FATAL(this, "Bad EnumSymbol::astTag");
  }
}


EnumSymbol*
EnumSymbol::copyInner(SymbolMap* map) {
  EnumSymbol* copy = new EnumSymbol(this->name);
  copy->copyFlags(this);
  return copy;
}

void EnumSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumSymbol::replaceChild");
}

bool EnumSymbol::isParameter() const { return true; }

void EnumSymbol::codegenDef() { }

Immediate* EnumSymbol::getImmediate(void) {
  if (SymExpr* init = toSymExpr(defPoint->init)) {
    if (VarSymbol* initvar = toVarSymbol(init->var)) {
      return initvar->immediate;
    }
  }
  return NULL;
}

void EnumSymbol::accept(AstVisitor* visitor) {
  visitor->visitEnumSym(this);
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

ModuleSymbol::ModuleSymbol(const char* iName,
                           ModTag      iModTag,
                           BlockStmt*  iBlock)
  : Symbol(E_ModuleSymbol, iName),
    modTag(iModTag),
    block(iBlock),
    initFn(NULL),
    filename(NULL),
    doc(NULL),
#ifdef HAVE_LLVM
    extern_info(NULL),
    llvmDINameSpace(NULL),
#endif
    moduleNamePrefix("")
{

  block->parentSymbol = this;
  registerModule(this);
  gModuleSymbols.add(this);
}


ModuleSymbol::~ModuleSymbol() { }


void ModuleSymbol::verify() {
  Symbol::verify();

  if (astTag != E_ModuleSymbol) {
    INT_FATAL(this, "Bad ModuleSymbol::astTag");
  }

  if (block && block->parentSymbol != this)
    INT_FATAL(this, "Bad ModuleSymbol::block::parentSymbol");

  if (initFn && !toFnSymbol(initFn))
    INT_FATAL(this, "Bad ModuleSymbol::initFn");
}


ModuleSymbol*
ModuleSymbol::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Illegal call to ModuleSymbol::copy");

  return NULL;
}


static int compareLineno(const void* v1, const void* v2) {
  FnSymbol* fn1 = *(FnSymbol* const *)v1;
  FnSymbol* fn2 = *(FnSymbol* const *)v2;

  if (fn1->linenum() > fn2->linenum())
    return 1;

  else if (fn1->linenum() < fn2->linenum())
    return -1;

  else
    return 0;
}


void ModuleSymbol::codegenDef() {
  GenInfo* info = gGenInfo;

  info->filename = fname();
  info->lineno   = linenum();

  info->cStatements.clear();
  info->cLocalDecls.clear();

  Vec<FnSymbol*> fns;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (fn->hasFlag(FLAG_EXTERN) ||
            fn->hasFlag(FLAG_FUNCTION_PROTOTYPE))
          continue;

        fns.add(fn);
      }
  }

  qsort(fns.v, fns.n, sizeof(fns.v[0]), compareLineno);

#ifdef HAVE_LLVM
  if(debug_info && info->filename) {
    debug_info->get_module_scope(this);
  }
#endif

  forv_Vec(FnSymbol, fn, fns) {
    fn->codegenDef();
  }

  flushStatements();
}

// Collect the top-level classes for this Module.
//
// 2014/07/25 MDN.  This function is currently only called by
// docs.  Historically all of the top-level classes were buried
// inside the prototypical module initFn.
//
// Installing The initFn is being moved forward but there are
// still short periods of time when the classes will still be
// buried inside the module initFn.
//
// Hence this function is currently able to handle the before
// and after case.  The before case can be pulled out once the
// construction of the initFn is cleaned up.
//

Vec<AggregateType*> ModuleSymbol::getTopLevelClasses() {
  Vec<AggregateType*> classes;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr)) {

      if (TypeSymbol* type = toTypeSymbol(def->sym)) {
        if (AggregateType* cl = toAggregateType(type->type)) {
          classes.add(cl);
        }

      // Step in to the initFn
      } else if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          for_alist(expr2, fn->body->body) {
            if (DefExpr* def2 = toDefExpr(expr2)) {
              if (TypeSymbol* type = toTypeSymbol(def2->sym)) {
                if (AggregateType* cl = toAggregateType(type->type)) {
                  classes.add(cl);
                }
              }
            }
          }
        }
      }
    }
  }

  return classes;
}


void ModuleSymbol::printDocs(std::ostream *file, unsigned int tabs) {
  if (this->hasFlag(FLAG_NO_DOC)) {
    return;
  }

  // Print the module directive first, for .rst mode. This will associate the
  // Module: <name> title with the module. If the .. module:: directive comes
  // after the title, sphinx will complain about a duplicate id error.
  if (!fDocsTextOnly) {
    *file << ".. default-domain:: chpl" << std::endl << std::endl;
    *file << ".. module:: " << this->docsName() << std::endl;

    if (this->doc != NULL) {
      this->printTabs(file, tabs + 1);
      *file << ":synopsis: ";
      *file << firstNonEmptyLine(this->doc);
      *file << std::endl;
    }
    *file << std::endl;
  }

  this->printTabs(file, tabs);
  const char *moduleTitle = astr("Module: ", this->docsName().c_str());
  *file << moduleTitle << std::endl;

  if (!fDocsTextOnly) {
    int length = tabs * this->tabText.length() + strlen(moduleTitle);
    for (int i = 0; i < length; i++) {
      *file << "=";
    }
    *file << std::endl;
  }

  if (this->doc != NULL) {
    // Only print tabs for text only mode. The .rst prefers not to have the
    // tabs for module level comments and leading whitespace removed.
    unsigned int t = tabs;
    if (fDocsTextOnly) {
      t += 1;
    }

    this->printDocsDescription(this->doc, file, t);
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


/*
 * Append 'prefix' to existing module name prefix.
 */
void ModuleSymbol::addPrefixToName(std::string prefix) {
  this->moduleNamePrefix += prefix;
}


/*
 * Returns name of module, including any prefixes that have been set.
 */
std::string ModuleSymbol::docsName() {
  return this->moduleNamePrefix + this->name;
}


// This is intended to be called by getTopLevelConfigsVars and
// getTopLevelVariables, since the code for them would otherwise be roughly
// the same.

// It is also private to ModuleSymbols
//
// See the comment on getTopLevelFunctions() for the rationale behind the AST
// traversal
void ModuleSymbol::getTopLevelConfigOrVariables(Vec<VarSymbol *> *contain, Expr *expr, bool config) {
  if (DefExpr* def = toDefExpr(expr)) {

    if (VarSymbol* var = toVarSymbol(def->sym)) {
      if (var->hasFlag(FLAG_CONFIG) == config) {
        // The config status of the variable matches what we are looking for
        contain->add(var);
      }

    } else if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (fn->hasFlag(FLAG_MODULE_INIT)) {
        for_alist(expr2, fn->body->body) {
          if (DefExpr* def2 = toDefExpr(expr2)) {
            if (VarSymbol* var = toVarSymbol(def2->sym)) {
              if (var->hasFlag(FLAG_CONFIG) == config) {
                // The config status of the variable matches what we are
                // looking for
                contain->add(var);
              }
            }
          }
        }
      }
    }
  }
}

// Collect the top-level config variables for this Module.
Vec<VarSymbol*> ModuleSymbol::getTopLevelConfigVars() {
  Vec<VarSymbol*> configs;

  for_alist(expr, block->body) {
    getTopLevelConfigOrVariables(&configs, expr, true);
  }

  return configs;
}

// Collect the top-level variables that aren't configs for this Module.
Vec<VarSymbol*> ModuleSymbol::getTopLevelVariables() {
  Vec<VarSymbol*> variables;

  for_alist(expr, block->body) {
    getTopLevelConfigOrVariables(&variables, expr, false);
  }

  return variables;
}

// Collect the top-level functions for this Module.
//
// This one is similar to getTopLevelModules() and
// getTopLevelClasses() except that it collects any
// functions and then steps in to initFn if it finds it.
//
Vec<FnSymbol*> ModuleSymbol::getTopLevelFunctions(bool includeExterns) {
  Vec<FnSymbol*> fns;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (includeExterns == false &&
            (fn->hasFlag(FLAG_EXTERN) ||
             fn->hasFlag(FLAG_FUNCTION_PROTOTYPE))) {
          continue;
        }

        fns.add(fn);

        // The following additional overhead and that present in getConfigVars
        // and getClasses is a result of the docs pass occurring before
        // the functions/configvars/classes are taken out of the module
        // initializer function and put on the same level as that function.
        // If and when that changes, the code encapsulated in this if
        // statement may be removed.
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          for_alist(expr2, fn->body->body) {
            if (DefExpr* def2 = toDefExpr(expr2)) {
              if (FnSymbol* fn2 = toFnSymbol(def2->sym)) {
                if (includeExterns == false &&
                    (fn2->hasFlag(FLAG_EXTERN) ||
                     fn2->hasFlag(FLAG_FUNCTION_PROTOTYPE))) {
                  continue;
                }

                fns.add(fn2);
              }
            }
          }
        }
      }
    }
  }

  return fns;
}

Vec<ModuleSymbol*> ModuleSymbol::getTopLevelModules() {
  Vec<ModuleSymbol*> mods;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (ModuleSymbol* mod = toModuleSymbol(def->sym)) {
        if (strcmp(mod->defPoint->parentSymbol->name, name) == 0)
          mods.add(mod);
      }
  }

  return mods;
}

void ModuleSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == block) {
    block = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ModuleSymbol::replaceChild");
  }
}

void ModuleSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterModSym(this) == true) {

    if (block)
      block->accept(visitor);

    visitor->exitModSym(this);
  }
}

void ModuleSymbol::addDefaultUses() {
  if (modTag != MOD_INTERNAL) {
    UnresolvedSymExpr* modRef = 0;

    SET_LINENO(this);

    modRef = new UnresolvedSymExpr("ChapelStandard");
    block->insertAtHead(new CallExpr(PRIM_USE, modRef));

  // We don't currently have a good way to fetch the root module by name.
  // Insert it directly rather than by name
  } else if (this == baseModule) {
    SET_LINENO(this);

    block->moduleUseAdd(rootModule);
  }
}

//
// NOAKES 2014/07/22
//
// There is currently a problem in functionResolve that this function
// has a "temporary" work around for.

// There is somewhere within that code that believes the order of items in
// modUseList is an indicator of "dependence order" even though this list
// does not and cannot maintain that information.
//
// Fortunately there are currently no tests that expose this fallacy so
// long at ChapelStandard always appears first in the list
void ModuleSymbol::moduleUseAdd(ModuleSymbol* mod) {
  if (modUseList.index(mod) < 0) {
    if (mod == standardModule) {
      modUseList.insert(0, mod);
    } else {
      modUseList.add(mod);
    }
  }
}

// If the specified module is currently used by the target
// then remove the module from the use-state of this module
// but introduce references to the children of the module
// being dropped.
//
// At this time this is only used for deadCodeElimination and
// it is not clear if there will be other uses.
void ModuleSymbol::moduleUseRemove(ModuleSymbol* mod) {
  int index = modUseList.index(mod);

  if (index >= 0) {
    bool inBlock = block->moduleUseRemove(mod);

    modUseList.remove(index);

    // The dead module may have used other modules.  If so add them
    // to the current module
    forv_Vec(ModuleSymbol, modUsedByDeadMod, mod->modUseList) {
      if (modUseList.index(modUsedByDeadMod) < 0) {
        SET_LINENO(this);

        if (inBlock == true)
          block->moduleUseAdd(modUsedByDeadMod);

        modUseList.add(modUsedByDeadMod);
      }
    }
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

LabelSymbol::LabelSymbol(const char* init_name) :
  Symbol(E_LabelSymbol, init_name, NULL),
  iterResumeGoto(NULL)
{
  gLabelSymbols.add(this);
}


void LabelSymbol::verify() {
  Symbol::verify();
  if (astTag != E_LabelSymbol) {
    INT_FATAL(this, "Bad LabelSymbol::astTag");
  }
  if (GotoStmt* igs = iterResumeGoto) {
    if (!isAlive(igs))
      INT_FATAL(this, "label's iterResumeGoto is not in AST");
    if (igs->gotoTag != GOTO_ITER_RESUME)
      INT_FATAL(this, "label's iterResumeGoto has unexpected gotoTag");
    if (getGotoLabelSymbol(igs) != this)
      INT_FATAL(this,"label's iterResumeGoto does not point back to the label");
  }
}

LabelSymbol*
LabelSymbol::copyInner(SymbolMap* map) {
  LabelSymbol* copy = new LabelSymbol(name);
  copy->copyFlags(this);
  copy->cname = cname;
  if (iterResumeGoto) {
    MapElem<GotoStmt*,GotoStmt*>* rec =
      copiedIterResumeGotos.get_record(iterResumeGoto);
    if (rec) {
      // we gotta have the mapping because we handle each goto exactly once
      INT_ASSERT(rec->value);
      // update the copy
      copy->iterResumeGoto = rec->value;
      // indicate we are done with it
      rec->value = NULL;
      // printf("LabelSymbol-copy %d > %d  irg %d > %d\n", this->id, copy->id,
      //        iterResumeGoto->id, copy->iterResumeGoto->id);
    } else {
      // to be handled later - in GotoStmt::copyInner
      // printf("LabelSymbol-copy %d > %d  irg %d no action\n",
      //        this->id, copy->id, iterResumeGoto->id);
    }
  }
  return copy;
}

void LabelSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in LabelSymbol::replaceChild");
}

void LabelSymbol::codegenDef() { }

void LabelSymbol::accept(AstVisitor* visitor) {
  visitor->visitLabelSym(this);
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

static int literal_id = 1;
HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;

// Note that string immediate values are stored
// with C escapes - that is newline is 2 chars \ n
// so this function expects a string that could be in "" in C
VarSymbol *new_StringSymbol(const char *str) {
  Immediate imm;
  imm.const_kind = CONST_KIND_STRING;
  imm.v_string = astr(str);
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtStringC;
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}


VarSymbol* new_BoolSymbol(bool b, IF1_bool_type size) {
  Immediate imm;
  switch (size) {
  default:
    INT_FATAL( "unknown BOOL_SIZE");

  case BOOL_SIZE_1  :
  case BOOL_SIZE_SYS:
  case BOOL_SIZE_8  :
  case BOOL_SIZE_16 :
  case BOOL_SIZE_32 :
  case BOOL_SIZE_64 :
    break;
    // case BOOL_SIZE_128: imm.v_bool = b; break;
  }
  imm.v_bool = b;
  imm.const_kind = NUM_KIND_BOOL;
  imm.num_index = size;
  VarSymbol *s;
  PrimitiveType* dtRetType = dtBools[size];
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  return s;
}

VarSymbol *new_IntSymbol(int64_t b, IF1_int_type size) {
  Immediate imm;
  switch (size) {
  case INT_SIZE_8  : imm.v_int8   = b; break;
  case INT_SIZE_16 : imm.v_int16  = b; break;
  case INT_SIZE_32 : imm.v_int32  = b; break;
  case INT_SIZE_64 : imm.v_int64  = b; break;
    // case INT_SIZE_128: imm.v_int128 = b; break;
  default:
    INT_FATAL( "unknown INT_SIZE");
  }
  imm.const_kind = NUM_KIND_INT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtInt[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_UIntSymbol(uint64_t b, IF1_int_type size) {
  Immediate imm;
  switch (size) {
  case INT_SIZE_8  : imm.v_uint8   = b; break;
  case INT_SIZE_16 : imm.v_uint16  = b; break;
  case INT_SIZE_32 : imm.v_uint32  = b; break;
  case INT_SIZE_64 : imm.v_uint64  = b; break;
    // case INT_SIZE_128: imm.v_uint128 = b; break;
  default:
    INT_FATAL( "unknown INT_SIZE");
  }
  imm.const_kind = NUM_KIND_UINT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtUInt[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

static VarSymbol* new_FloatSymbol(const char* n, long double b,
                                  IF1_float_type size, IF1_num_kind kind,
                                  Type* type) {
  Immediate imm;
  switch (size) {
  case FLOAT_SIZE_32  : imm.v_float32  = b; break;
  case FLOAT_SIZE_64  : imm.v_float64  = b; break;
  default:
    INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = kind;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), type);
  rootModule->block->insertAtTail(new DefExpr(s));
  if (!strchr(n, '.') && !strchr(n, 'e') && !(strchr(n, 'E'))) {
    s->cname = astr(n, ".0");
  } else {
    s->cname = astr(n);
  }
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_RealSymbol(const char *n, long double b, IF1_float_type size) {
  return new_FloatSymbol(n, b, size, NUM_KIND_REAL, dtReal[size]);
}

VarSymbol *new_ImagSymbol(const char *n, long double b, IF1_float_type size) {
  return new_FloatSymbol(n, b, size, NUM_KIND_IMAG, dtImag[size]);
}

VarSymbol *new_ComplexSymbol(const char *n, long double r, long double i,
                             IF1_complex_type size) {
  Immediate imm;
  switch (size) {
  case COMPLEX_SIZE_64:
    imm.v_complex64.r  = r;
    imm.v_complex64.i  = i;
    break;
  case COMPLEX_SIZE_128:
    imm.v_complex128.r = r;
    imm.v_complex128.i = i;
    break;
  default:
    INT_FATAL( "unknown COMPLEX_SIZE for complex");
  }
  imm.const_kind = NUM_KIND_COMPLEX;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s) {
    return s;
  }
  Type* dtRetType = dtComplex[size];
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  s->cname = astr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

static Type*
immediate_type(Immediate *imm) {
  switch (imm->const_kind) {
    case CONST_KIND_STRING:
      return dtStringC;
    case NUM_KIND_BOOL:
      return dtBools[imm->num_index];
    case NUM_KIND_UINT:
      return dtUInt[imm->num_index];
    case NUM_KIND_INT:
      return dtInt[imm->num_index];
    case NUM_KIND_REAL:
      return dtReal[imm->num_index];
    case NUM_KIND_IMAG:
      return dtImag[imm->num_index];
    case NUM_KIND_COMPLEX:
      return dtComplex[imm->num_index];
    default:
      USR_FATAL("bad immediate type");
      break;
  }
  return NULL;
}

VarSymbol *new_ImmediateSymbol(Immediate *imm) {
  VarSymbol *s = uniqueConstantsHash.get(imm);
  if (s)
    return s;
  Type *t = immediate_type(imm);
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), t);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  const size_t bufSize = 512;
  char str[bufSize];
  const char* ss = str;
  if (imm->const_kind == CONST_KIND_STRING)
    ss = imm->v_string;
  else
    snprint_imm(str, bufSize, *imm);
  s->cname = astr(ss);
  *s->immediate = *imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

// enable locally-unique temp names?
bool localTempNames = false;

// used to number the temps uniquely to top-level statements
// (give them smaller numbers, for readability of AST printouts)
static int tempID = 1;

void resetTempID() {
  tempID = 1;
}

FlagSet getRecordWrappedFlags(Symbol* s) {
  static FlagSet mask;
  if (mask.none()) {
    // Initialize the function-scope static variable.
    mask.set(FLAG_ARRAY);
    mask.set(FLAG_DOMAIN);
    mask.set(FLAG_DISTRIBUTION);
  }

  return s->flags & mask;
}

FlagSet getSyncFlags(Symbol* s) {
  static FlagSet mask;
  if (mask.none()) {
    mask.set(FLAG_SYNC);
    mask.set(FLAG_SINGLE);
  }

  return s->flags & mask;
}

VarSymbol* newTemp(const char* name, Type* type) {
  if (!name) {
    if (localTempNames)
      name = astr("_t", istr(tempID++), "_");
    else
      name = "tmp";
  }
  VarSymbol* vs = new VarSymbol(name, type);
  vs->addFlag(FLAG_TEMP);
  return vs;
}


VarSymbol* newTemp(Type* type) {
  return newTemp((const char*)NULL, type);
}
