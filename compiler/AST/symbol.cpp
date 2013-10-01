#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <cstdlib>
#include <inttypes.h>
#include <stdint.h>
#include "astutil.h"
#include "bb.h"
#include "build.h"
#include "files.h"
#include "iterator.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "passes.h"
#include "type.h"
#include "expr.h"

#include "intlimits.h"

#include "codegen.h"

//
// The function that represents the compiler-generated entry point
//
FnSymbol *chpl_gen_main = NULL;

ModuleSymbol* rootModule = NULL;
ModuleSymbol* theProgram = NULL;
ModuleSymbol* mainModule = NULL;
ModuleSymbol* baseModule = NULL;
ModuleSymbol* standardModule = NULL;
Symbol *gNil = NULL;
Symbol *gUnknown = NULL;
Symbol *gMethodToken = NULL;
Symbol *gTypeDefaultToken = NULL;
Symbol *gLeaderTag = NULL, *gFollowerTag = NULL;
Symbol *gModuleToken = NULL;
Symbol *gVoid = NULL;
Symbol *gFile = NULL;
Symbol *gStringC = NULL;
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
VarSymbol* gPrivatization = NULL;
VarSymbol* gLocal = NULL;
VarSymbol* gNodeID = NULL;
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


Symbol::Symbol(AstTag astTag, const char* init_name, Type* init_type) :
  BaseAST(astTag),
  name(astr(init_name)),
  cname(name),
  type(init_type),
  defPoint(NULL),
  flags()
{}


Symbol::~Symbol() {
}


void Symbol::verify() {
  if (defPoint && !defPoint->parentSymbol && !toModuleSymbol(this))
    INT_FATAL(this, "Symbol::defPoint is not in AST");
  if (defPoint && this != defPoint->sym)
    INT_FATAL(this, "Symbol::defPoint != Sym::defPoint->sym");
}


bool Symbol::inTree(void) {
  if (this == rootModule)
    return true;
  if (defPoint)
    return defPoint->inTree();
  else
    return false;
}


Type* Symbol::typeInfo(void) {
  return type;
}


bool Symbol::isConstant(void) {
  return false;
}


bool Symbol::isParameter(){
  return false;
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


FnSymbol* Symbol::getFnSymbol(void) {
  return NULL;
}


bool Symbol::hasFlag(Flag flag) {
  CHECK_FLAG(flag);
  return flags[flag];
}


void Symbol::addFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.set(flag);
}


void Symbol::copyFlags(Symbol* other) {
  flags |= other->flags;
}


void Symbol::removeFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.reset(flag);
}

bool Symbol::hasEitherFlag(Flag aflag, Flag bflag) {
  return hasFlag(aflag) || hasFlag(bflag);
}


bool Symbol::isImmediate() {
  return false;
}


VarSymbol::VarSymbol(const char *init_name,
                     Type    *init_type) :
  Symbol(E_VarSymbol, init_name, init_type),
  immediate(NULL),
  doc(NULL)
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
  newVarSymbol->cname = cname;
  INT_ASSERT(!newVarSymbol->immediate);
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in VarSymbol::replaceChild");
}


bool VarSymbol::isConstant(void) {
  return hasFlag(FLAG_CONST);
}


bool VarSymbol::isParameter(void){
  return hasFlag(FLAG_PARAM) || immediate;
}

#ifdef HAVE_LLVM
static
llvm::Value* codegenImmediateLLVM(Immediate* i)
{
  GenInfo* info = gGenInfo;
  llvm::Value* ret = NULL;

  switch(i->const_kind) {
    case NUM_KIND_UINT:
      switch(i->num_index) {
        case INT_SIZE_1:
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
        case INT_SIZE_1:
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
    case NUM_KIND_FLOAT:
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
      ret.isLVPtr = GEN_VAL;
      if (immediate->const_kind == CONST_KIND_STRING) {
        ret.c += '"';
        ret.c += immediate->v_string;
        ret.c += '"';
      } else if (immediate->const_kind == NUM_KIND_UINT &&
               immediate->num_index == INT_SIZE_1) {
        ret.c =  immediate->uint_value() ? "true" : "false";
      } else if (immediate->const_kind == NUM_KIND_INT) {
        int64_t iconst = immediate->int_value();
        if (iconst == (1ll<<63)) {
          ret.c = "-INT64(9223372036854775807) - INT64(1)";
        } else if (iconst <= -2147483648ll || iconst >= 2147483647ll) {
          ret.c = "INT64(" + int64_to_string(iconst) + ")";
        } else {
          ret.c = int64_to_string(iconst);
        }
      } else if (immediate->const_kind == NUM_KIND_UINT) {
        uint64_t uconst = immediate->uint_value();
        if( uconst <= (uint64_t) INT32_MAX ) {
          ret.c = uint64_to_string(uconst);
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


static void zeroInitializeRecord(FILE* outfile, ClassType* ct) {
  bool first = true;
  fprintf(outfile, "{");
  for_fields(field, ct) {
    if (!first) {
      fprintf(outfile, ", ");
    }
    if (ClassType* ct = toClassType(field->type)) {
      if (ct->classTag == CLASS_RECORD) {
        zeroInitializeRecord(outfile, ct);
      } else if (ct->classTag == CLASS_UNION) {
        fprintf(outfile, "{0, 0}");
      } else {
        fprintf(outfile, "NULL");
      }
    } else {
      fprintf(outfile, "0");
    }
    first = false;
  }
  fprintf(outfile, "}");
}


void VarSymbol::codegenDefC(bool global) {
  GenInfo* info = gGenInfo;
  if (this->hasFlag(FLAG_EXTERN))
    return;

  if (type == dtVoid)
    return;

  ClassType* ct = toClassType(type);
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
    if (ct->classTag == CLASS_CLASS) {
      if (isFnSymbol(defPoint->parentSymbol)) {
        str += " = NULL";  
      }
    } else if (ct->symbol->hasFlag(FLAG_WIDE) ||
               ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (isFnSymbol(defPoint->parentSymbol)) {
        if( widePointersStruct || isWideString(ct) ) {
          str += " = {{0,0},NULL}";
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
      if( hasFlag(FLAG_TYPE_VARIABLE) ) {
        llvm::Type* t = info->lvt->getType(cname);
        if( ! t ) {
          // TODO should be USR_FATAL
          USR_WARN(this, "Could not find extern def of type %s", cname);
        }
      } else {
        GenRet v = info->lvt->getValue(cname);
        if( ! v.val ) {
          // TODO should be USR_FATAL
          USR_WARN(this, "Could not find extern def of %s", cname);
        }
      }
    } else {
      // Anything that we don't have already
      //
      llvm::GlobalVariable *gVar = llvm::cast<llvm::GlobalVariable>(
          info->module->getOrInsertGlobal(cname, type->codegen().type));
      gVar->setInitializer(llvm::Constant::getNullValue(type->codegen().type));

      info->lvt->addGlobalValue(cname, gVar, GEN_PTR, ! is_signed(type) );
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

      info->lvt->addGlobalValue(cname, globalValue, GEN_VAL, ! is_signed(type));
    }
    llvm::Type *varType = type->codegen().type;
    llvm::Value *varAlloca = createTempVarLLVM(varType, cname);
    info->lvt->addValue(cname, varAlloca, GEN_PTR, ! is_signed(type));
    
    if(ClassType *ctype = toClassType(type)) {
      if(ctype->classTag == CLASS_CLASS ||
         ctype->symbol->hasFlag(FLAG_WIDE) ||
         ctype->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        if(isFnSymbol(defPoint->parentSymbol)) {
          info->builder->CreateStore(
              llvm::Constant::getNullValue(varType), varAlloca);
        }
      }
    }
#endif
  }
}


bool VarSymbol::isImmediate() {
  return immediate != NULL;
}


// describes the intent (for use in an English sentence)
const char* intentDescrName[INTENT_TYPE+1] = {
  "blank intent",
  "'in'",
  "'inout'",
  "'out'",
  "'const'",
  "'const in'",
  "'const ref'",
  "'ref'",
  "'param'",
  "'type'"
};


ArgSymbol::ArgSymbol(IntentTag iIntent, const char* iName, 
                     Type* iType, Expr* iTypeExpr,
                     Expr* iDefaultExpr, Expr* iVariableExpr) :
  Symbol(E_ArgSymbol, iName, iType),
  intent(iIntent),
  typeExpr(NULL),
  defaultExpr(NULL),
  variableExpr(NULL),
  instantiatedFrom(NULL),
  instantiatedParam(false),
  markedGeneric(false)
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
  ps->cname = cname;
  ps->instantiatedFrom = instantiatedFrom;
  ps->instantiatedParam = instantiatedParam;
  ps->markedGeneric = markedGeneric;
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
  // Don't pass global ptrs by ref
  if( fLLVMWideOpt && type->symbol->hasEitherFlag(FLAG_WIDE,FLAG_WIDE_CLASS) )
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


bool ArgSymbol::isConstant(void) {
  if (intent == INTENT_CONST_IN || intent == INTENT_CONST_REF) {
    return true;
  }
  return (intent == INTENT_BLANK || intent == INTENT_CONST) &&
    !isReferenceType(type) &&
    !isRecordWrappedType(type) /* array, domain, distribution */;
}


bool ArgSymbol::isParameter(void) {
  return (intent == INTENT_PARAM);
}


// describes this argument's intent (for use in an English sentence)
const char* ArgSymbol::intentDescrString(void) {
  return intentDescrName[intent];
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



TypeSymbol::TypeSymbol(const char* init_name, Type* init_type) :
  Symbol(E_TypeSymbol, init_name, init_type),
    llvmType(NULL),
    llvmTbaaNode(NULL), llvmConstTbaaNode(NULL),
    llvmTbaaStructNode(NULL), llvmConstTbaaStructNode(NULL),
    codegenned(false)
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
  new_type_symbol->cname = cname;
  if (this->hasFlag(FLAG_SYNC))
    new_type_symbol->addFlag(FLAG_SYNC);
  if (this->hasFlag(FLAG_SINGLE))
    new_type_symbol->addFlag(FLAG_SINGLE);
  if (this->hasFlag(FLAG_ATOMIC_TYPE))
    new_type_symbol->addFlag(FLAG_ATOMIC_TYPE);
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

  codegenned = true;

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
    llvm::Value* Ops[1];
    Ops[0] = llvm::MDString::get(ctx, "Chapel types");
    info->tbaaRootNode = llvm::MDNode::get(ctx, Ops);
  }

  // Set the llvmTbaaNode to non-NULL so that we can
  // avoid recursing.
  llvmTbaaNode = info->tbaaRootNode;

  ClassType* ct = toClassType(type);

  Type* superType = NULL;
  // Recursively generate the TBAA nodes for this type.
  if( ct ) {
    for_fields(field, ct) {
      ClassType* fct = toClassType(field->type);
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

  // Now create tbaa metadata, one for const and one for not.
  {
    llvm::Value* Ops[2];
    Ops[0] = llvm::MDString::get(ctx, cname);
    Ops[1] = parent;
    llvmTbaaNode = llvm::MDNode::get(ctx, Ops);
  }
  {
    llvm::Value* Ops[3];
    Ops[0] = llvm::MDString::get(ctx, cname);
    Ops[1] = constParent;
    Ops[2] = llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), 1);
    llvmConstTbaaNode = llvm::MDNode::get(ctx, Ops);
  }
  // Don't try to create tbaa.struct metadata for non-struct.
  if( isUnion(type) ||
      hasFlag(FLAG_STAR_TUPLE) ||
      hasFlag(FLAG_FIXED_STRING) ||
      hasFlag(FLAG_REF) ||
      hasFlag(FLAG_DATA_CLASS) ||
      hasEitherFlag(FLAG_WIDE,FLAG_WIDE_CLASS) ) {
    return;
  }

  if( ct ) {
    // Now create the tbaa.struct metadata nodes.
    llvm::SmallVector<llvm::Value*, 16> Ops;
    llvm::SmallVector<llvm::Value*, 16> ConstOps;

    const char* struct_name = ct->classStructName(true);
    llvm::Type* struct_type_ty = info->lvt->getType(struct_name);
    llvm::StructType* struct_type = NULL;
    INT_ASSERT(struct_type_ty);
    struct_type = llvm::dyn_cast<llvm::StructType>(struct_type_ty);
    INT_ASSERT(struct_type);

    for_fields(field, ct) {
      llvm::Type* fieldType = field->type->symbol->codegen().type;
      ClassType* fct = toClassType(field->type);
      if(fct && field->hasFlag(FLAG_SUPER_CLASS)) {
        fieldType = info->lvt->getType(fct->classStructName(true));
      }
      INT_ASSERT(fieldType);
      if( ct ) {
        unsigned gep = ct->getMemberGEP(field->cname);
        llvm::Constant* off = llvm::ConstantExpr::getOffsetOf(struct_type, gep);
        llvm::Constant* sz = llvm::ConstantExpr::getSizeOf(fieldType);
        Ops.push_back(off);
        Ops.push_back(sz);
        Ops.push_back(field->type->symbol->llvmTbaaNode);
        ConstOps.push_back(off);
        ConstOps.push_back(sz);
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
      // codegenMetadata(); TODO -- enable TBAA generation in the future.
    }
    ret.type = llvmType;
#endif
  }

  return ret;
}



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
  doc(NULL)
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
  if (normalized && !hasFlag(FLAG_AUTO_II)) {
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
  FnSymbol* copy = new FnSymbol(name);
  if (hasFlag(FLAG_CONSTRUCTOR))
    copy->addFlag(FLAG_CONSTRUCTOR);
  else if (hasFlag(FLAG_INIT_COPY_FN))
    copy->addFlag(FLAG_INIT_COPY_FN);
  else if (hasFlag(FLAG_AUTO_COPY_FN))
    copy->addFlag(FLAG_AUTO_COPY_FN);
  else if (hasFlag(FLAG_AUTO_DESTROY_FN))
    copy->addFlag(FLAG_AUTO_DESTROY_FN);
  if (hasFlag(FLAG_DONOR_FN))
    copy->addFlag(FLAG_DONOR_FN);
  for_formals(formal, this) {
    copy->insertFormalAtTail(COPY_INT(formal->defPoint));
  }
  copy->setter = COPY_INT(setter);
  copy->retType = retType;
  copy->where = COPY_INT(where);
  copy->body = COPY_INT(body);
  copy->thisTag = thisTag;
  copy->retTag = retTag;
  copy->retExprType = COPY_INT(retExprType);
  copy->cname = cname;
  copy->_this = _this;
  copy->_outer = _outer;
  copy->instantiatedFrom = instantiatedFrom;
  copy->instantiationPoint = instantiationPoint;
  return copy;
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

  if (hasFlag(FLAG_EXTERN) && !genExternPrototypes) return;
  if (hasFlag(FLAG_NO_PROTOTYPE)) return;
  if (hasFlag(FLAG_NO_CODEGEN)) return;

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


static void
codegenNullAssignments(FILE* outfile, const char* cname, ClassType* ct) {
  if (isClass(ct))
    fprintf(outfile, "%s = NULL;\n", cname);
  else {
    for_fields(field, ct) {
      if (ClassType* fct = toClassType(field->type)) {
        char buffer[1024];
        strcpy(buffer, cname);
        strcat(buffer, ".");
        strcat(buffer, field->cname);
        codegenNullAssignments(outfile, buffer, fct);
      }
    }
  }
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

    llvm::Function::arg_iterator ai = func->arg_begin();
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
      }
      ++ai;
    }

#endif
  }

  {
    Vec<BaseAST*> asts;
    collect_top_asts(body, asts);

    forv_Vec(BaseAST, ast, asts) {
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
      if(llvm::verifyFunction(*func, llvm::PrintMessageAction)){
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
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  SymExpr* sym = toSymExpr(ret->get(1));
  if (!sym)
    INT_FATAL(this, "function is not normal");
  return sym->var;
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
FnSymbol::numFormals() {
  return formals.length;
}


ArgSymbol*
FnSymbol::getFormal(int i) {
  return toArgSymbol(toDefExpr(formals.get(i))->sym);
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
          formal->markedGeneric ||
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
  return new EnumSymbol(name);
}

void EnumSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumSymbol::replaceChild");
}

bool EnumSymbol::isParameter(void) { return true; }

void EnumSymbol::codegenDef() { }

Immediate* EnumSymbol::getImmediate(void) {
  if (SymExpr* init = toSymExpr(defPoint->init)) {
    if (VarSymbol* initvar = toVarSymbol(init->var)) {
      return initvar->immediate;
    }
  }
  return NULL;
}


ModuleSymbol::ModuleSymbol(const char* iName, ModTag iModTag, BlockStmt* iBlock)
  : Symbol(E_ModuleSymbol, iName),
    modTag(iModTag),
    block(iBlock),
    initFn(NULL),
    filename(NULL),
    doc(NULL),
    extern_info(NULL)
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
  FnSymbol* fn1 = *(FnSymbol**)v1;
  FnSymbol* fn2 = *(FnSymbol**)v2;
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
  info->lineno = linenum();
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
  forv_Vec(FnSymbol, fn, fns) {
    fn->codegenDef();
  }
  flushStatements();
  return;
}
 
Vec<ClassType*> ModuleSymbol::getClasses() {
  Vec<ClassType*> classes;
  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          for_alist(expr2, fn->body->body) {
            if (DefExpr* def2 = toDefExpr(expr2))
              if (TypeSymbol* type = toTypeSymbol(def2->sym)) 
                if (ClassType* cl = toClassType(type->type)) {
                  classes.add(cl);
                }
          }
        }
      }
  }
  return classes;
}

Vec<VarSymbol*> ModuleSymbol::getConfigVars() {
  Vec<VarSymbol*> configs;
  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          for_alist(expr2, fn->body->body) {
            if (DefExpr* def2 = toDefExpr(expr2))
              if (VarSymbol* var = toVarSymbol(def2->sym)) {
                if (var->hasFlag(FLAG_CONFIG)) {
                  configs.add(var);
                }
              }
          }
        }
      }
  }
  return configs;
}

Vec<ModuleSymbol*> ModuleSymbol::getModules() {
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

Vec<FnSymbol*> ModuleSymbol::getFunctions() {
  Vec<FnSymbol*> fns;
  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (!genExternPrototypes &&
            (fn->hasFlag(FLAG_EXTERN) ||
             fn->hasFlag(FLAG_FUNCTION_PROTOTYPE)))
          continue;
        fns.add(fn);
        // The following additional overhead and that present in getConfigVars 
        // and getClasses is a result of the docs pass occurring before
        // the functions/configvars/classes are taken out of the module
        // initializer function and put on the same level as that function.
        // If and when that changes, the code encapsulated in this if
        // statement may be removed.
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          for_alist(expr2, fn->body->body) {
            if (DefExpr* def2 = toDefExpr(expr2))
              if (FnSymbol* fn2 = toFnSymbol(def2->sym)) {
                if (!genExternPrototypes &&
                    (fn->hasFlag(FLAG_EXTERN) ||
                     fn->hasFlag(FLAG_FUNCTION_PROTOTYPE)))
                  continue;
                fns.add(fn2);
              }
          }
        }
      }
  }
  return fns;
}

 
  
void ModuleSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == block) {
    block = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ModuleSymbol::replaceChild");
  }
}


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
  PrimitiveType* dtRetType = dtString;
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
  case BOOL_SIZE_8  : imm.v_bool = b; break;
  case BOOL_SIZE_16 : imm.v_bool = b; break;
  case BOOL_SIZE_32 : imm.v_bool = b; break;
  case BOOL_SIZE_64 : imm.v_bool = b; break;
    // case BOOL_SIZE_128: imm.v_bool = b; break;
  default:
    INT_FATAL( "unknown BOOL_SIZE");
  }
  imm.const_kind = NUM_KIND_UINT;
  imm.num_index = INT_SIZE_1;
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

VarSymbol *new_RealSymbol(const char *n, long double b, IF1_float_type size) {
  Immediate imm;
  switch (size) {
  case FLOAT_SIZE_32  : imm.v_float32  = b; break;
  case FLOAT_SIZE_64  : imm.v_float64  = b; break;
  default:
    INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = NUM_KIND_FLOAT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtReal[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->cname = astr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ImagSymbol(const char *n, long double b, IF1_float_type size) {
  Immediate imm;
  switch (size) {
  case FLOAT_SIZE_32  : imm.v_float32  = b; break;
  case FLOAT_SIZE_64  : imm.v_float64  = b; break;
  default:
    INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = NUM_KIND_IMAG;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtImag[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->cname = astr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
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
      return dtString;
    case NUM_KIND_UINT:
      return dtUInt[imm->num_index];
    case NUM_KIND_INT:
      return dtInt[imm->num_index];
    case NUM_KIND_FLOAT:
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
      name = "_tmp";
  }
  VarSymbol* vs = new VarSymbol(name, type);
  vs->addFlag(FLAG_TEMP);
  return vs;
}


VarSymbol* newTemp(Type* type) {
  return newTemp((const char*)NULL, type);
}
