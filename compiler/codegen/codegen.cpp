/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "codegen.h"

#include "astutil.h"
#include "chplmath.h"
#include "clangBuiltinsWrappedSet.h"
#include "clangUtil.h"
#include "config.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "insertLineNumbers.h"
#include "library.h"
#include "llvmDebug.h"
#include "llvmUtil.h"
#include "LayeredValueTable.h"
#include "mli.h"
#include "mysystem.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"
#include "virtualDispatch.h"

#ifdef HAVE_LLVM
// Include relevant LLVM headers
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#endif

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

#include <algorithm>
#include <cctype>

#include <cstring>
#include <cstdio>
#include <vector>

// function prototypes
static bool compareSymbol(const void* v1, const void* v2);

// Global so that we don't have to pass around
// to all of the codegen() routines
GenInfo* gGenInfo   =  0;
int      gMaxVMT    = -1;
int      gStmtCount =  0;

std::map<std::string, int> commIDMap;


// ensure these two produce consistent output
std::string zlineToString(BaseAST* ast) {
  return "/* ZLINE: " + numToString(ast->linenum())
         + " " + ast->fname() + " */\n";
}
void zlineToFileIfNeeded(BaseAST* ast, FILE* outfile) {
  if (printCppLineno)
    fprintf(outfile, "%s", zlineToString(ast).c_str());
}

static char idCommentBuffer[32];

const char* idCommentTemp(BaseAST* ast) {
  sprintf(idCommentBuffer, "/* %7d */ ", ast->id);
  return idCommentBuffer;
}


static const char*
subChar(Symbol* sym, const char* ch, const char* x) {
  char* tmp = (char*)malloc(ch-sym->cname+1);
  strncpy(tmp, sym->cname, ch-sym->cname);
  tmp[ch-sym->cname] = '\0';
  sym->cname = astr(tmp, x, ch+1);
  free(tmp);
  return sym->cname;
}

static void legalizeName(Symbol* sym) {
  if (!sym->isRenameable())
    return;
  for (const char* ch = sym->cname; *ch != '\0'; ch++) {
    switch (*ch) {
    case '>': ch = subChar(sym, ch, "_GREATER_"); break;
    case '<': ch = subChar(sym, ch, "_LESS_"); break;
    case '=':
      {

        /* To help generated code readability, we'd like to convert =
           into "ASSIGN" and == into "EQUALS".  Unfortunately, because
           of the character-at-a-time approach taken here combined
           with the fact that subChar() returns a completely new
           string on every call, the way I implemented this is a bit
           ugly (in part because I didn't want to spend the time to
           reimplement this whole function -BLC */

        static const char* equalsStr = "_EQUALS_";
        static int equalsLen = strlen(equalsStr);

        if (*(ch+1) == '=') {
          // If we're in the == case, replace the first = with EQUALS
          ch = subChar(sym, ch, equalsStr);
        } else {
          if ((ch-equalsLen >= sym->cname) &&
              strncmp(ch-equalsLen, equalsStr, equalsLen) == 0) {
            // Otherwise, if the thing preceding this '=' is the
            // string _EQUALS_, we must have been the second '=' and
            // we should just replace ourselves with an underscore to
            // make things legal.
            ch = subChar(sym, ch, "_");
          } else {
            // Otherwise, this must have simply been a standalone '='
            ch = subChar(sym, ch, "_ASSIGN_");
          }
        }
        break;
    }
    case '*': ch = subChar(sym, ch, "_ASTERISK_"); break;
    case '/': ch = subChar(sym, ch, "_SLASH_"); break;
    case '%': ch = subChar(sym, ch, "_PERCENT_"); break;
    case '+': ch = subChar(sym, ch, "_PLUS_"); break;
    case '-': ch = subChar(sym, ch, "_HYPHEN_"); break;
    case '^': ch = subChar(sym, ch, "_CARET_"); break;
    case '&': ch = subChar(sym, ch, "_AMPERSAND_"); break;
    case '|': ch = subChar(sym, ch, "_BAR_"); break;
    case '!': ch = subChar(sym, ch, "_EXCLAMATION_"); break;
    case '#': ch = subChar(sym, ch, "_POUND_"); break;
    case '?': ch = subChar(sym, ch, "_QUESTION_"); break;
    case '$': ch = subChar(sym, ch, "_DOLLAR_"); break;
    case '~': ch = subChar(sym, ch, "_TILDE_"); break;
    case '.': ch = subChar(sym, ch, "_DOT_"); break;
    case ' ': ch = subChar(sym, ch, "_SPACE_"); break;
    default: break;
    }
  }

  // Add chpl_ to operator names.
  if ((sym->cname[0] == '_' &&
      (sym->cname[1] == '_' || (sym->cname[1] >= 'A' && sym->cname[1] <= 'Z'))))
  {
    sym->cname = astr("chpl__", sym->cname);
  }

  // Append number of array dimensions to polly_array_index
  // It helps Polly Optimizer to select the correct function
  if (strcmp("polly_array_index",sym->name) == 0){
    int numDims = (toFnSymbol(sym)->numFormals() - 1) / 2;
    sym->cname = astr("polly_array_index_",istr(numDims));
  }

}

static void
genGlobalDefClassId(const char* cname, int id, bool isHeader) {
  GenInfo* info = gGenInfo;
  const char* id_type_name = "chpl__class_id";
  std::string name("chpl__cid_");
  name += cname;

  if( info->cfile ) {
    if(isHeader)
      fprintf(info->cfile, "extern const %s %s;\n",
                      id_type_name, name.c_str());
    else
      fprintf(info->cfile, "const %s %s = %d;\n",
                      id_type_name, name.c_str(), id);
  } else {
#ifdef HAVE_LLVM
    if (!isHeader)
      return;
    GenRet id_type_g = CLASS_ID_TYPE->codegen();
    llvm::Type *id_type = id_type_g.type;
    llvm::GlobalVariable * gv = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(name, id_type));
    gv->setInitializer(info->irBuilder->getInt32(id));
    gv->setConstant(true);
    info->lvt->addGlobalValue(name, gv, GEN_PTR, ! is_signed(CLASS_ID_TYPE));
#endif
  }
}
static void
genGlobalString(const char* cname, const char* value) {
  GenInfo* info = gGenInfo;
  if( info->cfile ) {
    fprintf(info->cfile, "const char* %s = \"%s\";\n", cname, value);
  } else {
#ifdef HAVE_LLVM
    llvm::GlobalVariable *globalString = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(
          cname, llvm::IntegerType::getInt8PtrTy(info->module->getContext())));
    globalString->setInitializer(llvm::cast<llvm::GlobalVariable>(
          new_CStringSymbol(value)->codegen().val)->getInitializer());
    globalString->setConstant(true);
    info->lvt->addGlobalValue(cname, globalString, GEN_PTR, true);
#endif
  }
}
static void
genGlobalInt(const char* cname, int value, bool isHeader) {
  GenInfo* info = gGenInfo;
  if( info->cfile ) {
    if(isHeader)
      fprintf(info->cfile, "extern const int %s;\n", cname);
    else
    fprintf(info->cfile, "const int %s = %d;\n", cname, value);
  } else {
#ifdef HAVE_LLVM
    llvm::GlobalVariable *globalInt = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(
          cname, llvm::IntegerType::getInt32Ty(info->module->getContext())));
    globalInt->setInitializer(info->irBuilder->getInt32(value));
    globalInt->setConstant(true);
    info->lvt->addGlobalValue(cname, globalInt, GEN_PTR, false);
#endif
  }
}

static void genGlobalInt32(const char *cname, int value) {
  GenInfo *info = gGenInfo;
  if (info->cfile) {
    fprintf(info->cfile, "const int32_t %s = %d;\n", cname, value);
  } else {
#ifdef HAVE_LLVM
    llvm::GlobalVariable *globalInt =
        llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal(
            cname, llvm::IntegerType::getInt32Ty(info->module->getContext())));
    globalInt->setInitializer(info->irBuilder->getInt32(value));
    globalInt->setConstant(true);
    info->lvt->addGlobalValue(cname, globalInt, GEN_PTR, false);
#endif
  }
}

static bool
isObjectOrSubclass(Type* t)
{
  if (AggregateType* ct = toAggregateType(t))
    if (!isReferenceType(ct) && isClass(ct) &&
        (ct == dtObject || ct->symbol->hasFlag(FLAG_OBJECT_CLASS) ||
         !ct->symbol->hasFlag(FLAG_NO_OBJECT)))
      return true;

  return false;
}

static void
genClassIDs(std::vector<TypeSymbol*> & typeSymbol, bool isHeader) {
  genComment("Class Type Identification Numbers");

  forv_Vec(TypeSymbol, ts, typeSymbol) {
    if (AggregateType* ct = toAggregateType(ts->type)) {
      if (isObjectOrSubclass(ct)) {
        int id = ct->classId;
        INT_ASSERT(id != 0);
        genGlobalDefClassId(ts->cname, id, isHeader);
      }
    }
  }
}

struct compareSymbolFunctor {
  // This is really operator less-than
  bool operator() (const Symbol* a, const Symbol* b) const {
    return compareSymbol(a, b);
  }
};


// Visit class types in depth-first preorder order.
// Assigns class IDs to classes in that order.
static void preorderVisitClassesComputeIds(TypeSymbol* ts, int* nextNumber) {
  typedef std::set<TypeSymbol*, compareSymbolFunctor> children_set;

  children_set children;

  if (ts != NULL) {
    AggregateType* at   = toAggregateType(ts->type);
    int            myN1 = *nextNumber;

    INT_ASSERT(at != NULL);

    *nextNumber = *nextNumber + 1;
    at->classId = myN1;

    // visit children in order
    forv_Vec(AggregateType, child, at->dispatchChildren) {
      if (child != NULL) {
        children.insert(child->symbol);
      }
    }

    for (children_set::iterator it = children.begin();
         it != children.end();
         ++it ) {
      TypeSymbol* child = *it;

      preorderVisitClassesComputeIds(child, nextNumber);
    }
  }
}

static int gMaxClassId = 1;

static void assignClassIds() {
  int next = 1;

  preorderVisitClassesComputeIds(dtObject->symbol, &next);

  gMaxClassId = next - 1;
}


// Computes a maximum ID of subclasses and stores that in n2.
// Returns the maximum ID of a subclass.
// This helps with Schubert numbering
static int computeMaxSubclass(TypeSymbol* ts, std::vector<int>& n2) {
  int retval = 0;

  if (ts != NULL) {
    AggregateType* at    = toAggregateType(ts->type);
    int            myId  = at->classId;
    int            maxN1 = myId;

    forv_Vec(AggregateType, child, at->dispatchChildren) {
      if (child != NULL) {
        int subMax = computeMaxSubclass(child->symbol, n2);

        if (subMax > maxN1) {
          maxN1 = subMax;
        }
      }
    }

    if ((size_t) myId >= n2.size()) {
      n2.resize(myId + 1);
    }

    // set n2 for node, which is max of this n1
    // and child n1s.
    n2[myId] = maxN1;
    retval   = maxN1;
  }

  return retval;
}


static void codegenGlobalConstArray(const char*          name,
                                    const char*          eltType,
                                    std::vector<GenRet>* vals,
                                    bool                 isHeader) {
  GenInfo* info = gGenInfo;

  if(isHeader) {
    if( info->cfile ) {
      FILE* hdrfile = info->cfile;
      fprintf(hdrfile, "extern const %s %s[];\n", eltType, name);
    }
    return;
  }

  // Now generate arrays
  if( info->cfile ) {
    FILE* f = info->cfile;
    fprintf(f, "const %s %s[] = {\n", eltType, name);
    bool first = true;
    std::vector<GenRet> & array = *vals;
    int n = array.size();
    for(int i = 0; i < n; i++ ) {
      if (!first)
        fprintf(f, ",\n");
      fprintf(f, "/* %d */ %s", i, array[i].c.c_str());
      first = false;
    }
    fprintf(f, "\n};\n");
  } else {
#ifdef HAVE_LLVM

  llvm::Type *llvmEltType = getTypeLLVM(eltType);

  INT_ASSERT(llvmEltType);

  std::vector<llvm::Constant *> table;

  std::vector<GenRet> & array = *vals;
  int n = array.size();
  table.resize(n);
  for(int i = 0; i < n; i++ ) {
    llvm::Value* val = array[i].val;
    INT_ASSERT(val);
    table[i] = llvm::cast<llvm::Constant>(val);
  }

  llvm::ArrayType *tableType =
    llvm::ArrayType::get(llvmEltType, table.size());

  if(llvm::GlobalVariable *globalTable = info->module->getNamedGlobal(name)) {
    globalTable->eraseFromParent();
  }

  llvm::GlobalVariable *globalTable =llvm::cast<llvm::GlobalVariable>(
      info->module->getOrInsertGlobal(name, tableType));
  globalTable->setInitializer(llvm::ConstantArray::get(tableType, table));
  globalTable->setConstant(true);

  info->lvt->addGlobalValue(name, globalTable, GEN_VAL, true);
#endif
  }
}

// This uses Schubert Numbering but we could use Cohen's Display,
// which can be computed more incrementally.
// See
// "Implementing statically typed object-oriented programming languages",
// by Roland Ducournau
static void
genSubclassArray(bool isHeader) {
  const char* eltType = "chpl__class_id";
  const char* name = "chpl_subclass_max_id";

  if(isHeader) {
    // Just pass NULL when generating header
    codegenGlobalConstArray(name, eltType, NULL, true);
    return;
  }

  // Otherwise, compute n2 array and then code-generate it
  std::vector<int> n2;

  computeMaxSubclass(dtObject->symbol, n2);

  // make sure n2 always contains at least 1 element
  if (n2.empty())
    n2.push_back(0);

  // Construct the GenRet array of integers
  std::vector<GenRet> tmp;
  for(size_t i = 0; i < n2.size(); i++) {
    tmp.push_back( new_IntSymbol(n2[i], INT_SIZE_32)->codegen() );
  }

  // Now emit the global array declaration
  codegenGlobalConstArray(name, eltType, &tmp, false);
}


// Returns the type, in .c or .type field, for the passed name.
// The type_name typically refers to something defined in the runtime.
static
GenRet codegenTypeByName(const char* type_name)
{
  GenInfo* info = gGenInfo;

  GenRet ret;
  if (info->cfile) {
    ret.c = type_name;
  } else {
#ifdef HAVE_LLVM
    ret.type = getTypeLLVM(type_name);
#endif
  }
  return ret;
}



// codegenTypedNull takes in the pointer type
// (.c string or .type for LLVM) and generates NULL of that type.
static
GenRet codegenTypedNull(GenRet funcPtrType)
{
  GenInfo* info = gGenInfo;

  GenRet nullFn;
  if (info->cfile) {
    // C doesn't really care about the type of NULL, so use existing routine.
    nullFn.c = "(" + funcPtrType.c + ")(NULL)";
  } else {
#ifdef HAVE_LLVM
    // With LLVM, generate a NULL of the right type.
    INT_ASSERT(funcPtrType.type);
    nullFn.val = llvm::Constant::getNullValue(funcPtrType.type);
#endif
  }
  return nullFn;
}

#ifdef HAVE_LLVM
static
llvm::Constant* codegenStringForTableLLVM(std::string s)
{
  llvm::Constant* ret;
  GenRet str = new_CStringSymbol(s.c_str())->codegen();
  ret = llvm::cast<llvm::GlobalVariable>(str.val)->getInitializer();
  return ret;
}
#endif

static
GenRet codegenStringForTable(std::string s)
{
  GenInfo *info = gGenInfo;

  GenRet ret;
  if (info->cfile) {
    ret.c = "\"" + s + "\"";
  } else {
#ifdef HAVE_LLVM
    ret.val = codegenStringForTableLLVM(s);
#endif
  }
  return ret;
}


static void
genFtable(std::vector<FnSymbol*> & fSymbols, bool isHeader) {
  GenInfo* info = gGenInfo;

  const char* eltType = "chpl_fn_p";
  const char* name = "chpl_ftable";

  if(isHeader) {
    // Just pass NULL when generating header
    codegenGlobalConstArray(name, eltType, NULL, true);
    return;
  }

  GenRet funcPtrType = codegenTypeByName(eltType);

  // Construct the table elements
  std::vector<GenRet> ftable;
  ftable.reserve(fSymbols.size());

  forv_Vec(FnSymbol, fn, fSymbols) {
    GenRet gen;
    if (info->cfile) {
      gen.c = "(" + funcPtrType.c + ")";
      gen.c += fn->cname;
    } else {
#ifdef HAVE_LLVM
      INT_ASSERT(funcPtrType.type);
      llvm::Function *func = getFunctionLLVM(fn->cname);
      gen.val = info->irBuilder->CreatePointerCast(func, funcPtrType.type);
#endif
    }
    ftable.push_back(gen);
  }

  // make sure ftable always contains at least 1 element
  if (ftable.empty()) {
    GenRet nullFn = codegenTypedNull(funcPtrType);
    ftable.push_back(nullFn);
  }

  // Now emit the global array declaration
  codegenGlobalConstArray(name, eltType, &ftable, false);
}

static void
genFinfo(std::vector<FnSymbol*> & fSymbols, bool isHeader) {
  GenInfo* info = gGenInfo;

  const char* eltType = "chpl_fn_info";
  const char* name = "chpl_finfo";

  if(isHeader) {
    // Just pass NULL when generating header
    codegenGlobalConstArray(name, eltType, NULL, true);
    return;
  }

  // Compute the element type
  GenRet structType = codegenTypeByName(eltType);

#ifdef HAVE_LLVM
  llvm::Type *int32Ty = NULL;
  if (!info->cfile) {
    int32Ty = llvm::IntegerType::getInt32Ty(info->module->getContext());
  }
#endif


  // Construct the table elements
  std::vector<GenRet> finfo;
  finfo.reserve(fSymbols.size());

  // buf for creating C structures
  char* buf = NULL;
  int buf_len = 0;

  if (info->cfile) {
    // compute the maximum file name length
    forv_Vec(FnSymbol, fn, fSymbols) {
      int len = strlen(fn->cname);
      if (len > buf_len)
        buf_len = len;
    }
    // and then add 100 for two integers and punctuation
    buf_len += 100;
    buf = (char*) malloc(buf_len);
  }

  forv_Vec(FnSymbol, fn, fSymbols) {
    const char* fn_name = fn->cname;
    int fileno = getFilenameLookupPosition(fn->astloc.filename);
    int lineno = fn->astloc.lineno;

    GenRet gen;

    if (info->cfile) {
      int rc = snprintf(buf, buf_len,
                        "{\"%s\", %d, %d}", fn_name, fileno, lineno);
      INT_ASSERT( rc < buf_len ); // assert output not truncated
      gen.c = buf;
    } else {
#ifdef HAVE_LLVM
      llvm::Constant* fields[3];
      fields[0] = codegenStringForTableLLVM(fn_name);
      fields[1] = llvm::ConstantInt::get(int32Ty, fileno);
      fields[2] = llvm::ConstantInt::get(int32Ty, lineno);
      INT_ASSERT(structType.type);
      llvm::StructType* st = llvm::cast<llvm::StructType>(structType.type);
      gen.val = llvm::ConstantStruct::get(st, fields);
#endif
    }

    finfo.push_back(gen);
  }

  // Free the buffer for C conversions.
  if (buf) free(buf);

  // make sure the table always contains at trailing NULL element
  {
    GenRet nullStruct;
    if (info->cfile) {
      nullStruct.c = "{(char *)0, 0, 0}";
    } else {
      nullStruct = codegenTypedNull(structType);
    }
    finfo.push_back(nullStruct);
  }

  // Now emit the global array declaration
  codegenGlobalConstArray(name, eltType, &finfo, false);
}

static void
genVirtualMethodTable(std::vector<TypeSymbol*>& types, bool isHeader) {
  GenInfo* info = gGenInfo;
  const char* vmt = "chpl_vmtable";
  const char* eltType = "chpl_fn_p";
  if(isHeader) {
    codegenGlobalConstArray(vmt, eltType, NULL, true);
    return;
  }

  // compute max # methods per type
  int maxVMT = 0;

  // note: the virtual method table can contain keys
  // that point to deallocated memory (e.g. for classes that
  // have been removed). So it is important to only 'get'
  // live AST elements from the VMT rather than traversing it
  // directly.
  forv_Vec(TypeSymbol, ts, types) {
    if (AggregateType* ct = toAggregateType(ts->type))
      if (isObjectOrSubclass(ct))
        if (Vec<FnSymbol*>* vfns = virtualMethodTable.get(ct))
          if (vfns->n > maxVMT)
            maxVMT = vfns->n;
  }
  gMaxVMT = maxVMT;

  GenRet funcPtrType = codegenTypeByName(eltType);

  std::vector<GenRet> vmt_elts;

  // Make sure VMT has at least one element
  vmt_elts.resize(1);

  // compute 1D virtual method table
  // (this is not fundamental, but is currently used to simplify codegen)
  //    indexExpr = maxVMT * classId + fnId
  forv_Vec(TypeSymbol, ts, types) {
    if (AggregateType* ct = toAggregateType(ts->type)) {
      if (isObjectOrSubclass(ct)) {
        if (Vec<FnSymbol*>* vfns = virtualMethodTable.get(ct)) {
          int i = 0;
          forv_Vec(FnSymbol, vfn, *vfns) {
            int classId = ct->classId;
            int fnId = i;
            int index = gMaxVMT * classId + fnId;

            INT_ASSERT(classId > 0);

            GenRet fnAddress;

            if( info->cfile ) {
              fnAddress.c = "(" + funcPtrType.c + ")";
              fnAddress.c += vfn->cname;
            } else {
#ifdef HAVE_LLVM
              INT_ASSERT(funcPtrType.type);
              llvm::Function *func = getFunctionLLVM(vfn->cname);
              fnAddress.val = info->irBuilder->CreatePointerCast(func, funcPtrType.type);
#endif
            }

            if (vmt_elts.size() <= (size_t) index)
              vmt_elts.resize(index+1);

            vmt_elts[index] = fnAddress;

            i++;
          }
        }
      }
    }
  }

  // Fill any elements not filled above with codegenNullPointer
  for (size_t i = 0; i < vmt_elts.size(); i++) {
    if (vmt_elts[i].isEmpty()) {
      vmt_elts[i] = codegenTypedNull(funcPtrType);
    }
  }


  codegenGlobalConstArray(vmt, eltType, &vmt_elts, false);
}

static void genFilenameTable() {
  const char *name = "chpl_filenameTable";
  const char *sizeName = "chpl_filenameTableSize";
  const char *eltType = "c_string";

  // Compute the element type
  GenRet cstringType = codegenTypeByName(eltType);

  // Construct the table elements
  std::vector<GenRet> table;
  table.reserve(gFilenameLookup.size());

  for (std::vector<std::string>::iterator it = gFilenameLookup.begin();
       it != gFilenameLookup.end(); it++) {
    GenRet gen;
    std::string & path = (*it);
    std::string genPath;

    if(!strncmp(CHPL_HOME, path.c_str(), strlen(CHPL_HOME))) {
      genPath = "$CHPL_HOME";
      genPath += (path.c_str()+strlen(CHPL_HOME));
    } else {
      genPath = path;
    }

    gen = codegenStringForTable(genPath);
    table.push_back(gen);
  }

  // Now emit the global array declaration
  codegenGlobalConstArray(name, eltType, &table, false);

  // Now emit the size
  genGlobalInt32(sizeName, gFilenameLookup.size());
}

//
// This adds the Chapel symbol table to the config file
// Our symbol table is formed by two 1-D arrays with 2 elements
// per entry:
//
// chpl_funSymTable     = cname, Chapel name
// chpl_filenumSymTable = Chapel file name index, Chapel line number
//
static void genUnwindSymbolTable(){
  std::vector<FnSymbol*> symbols;

  //If CHPL_UNWIND is none we don't want any symbols in our tables
  if(strcmp(CHPL_UNWIND, "none") != 0){
    // Gets only user symbols
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if(strncmp(fn->name, "chpl_", 5) || fn->hasFlag(FLAG_MODULE_INIT)) {
        symbols.push_back(fn);
      }
    }
  }

  // Generate the cname, Chapel name table
  {
    const char *name = "chpl_funSymTable";
    const char *eltType = "c_string";

    // Compute the element type
    GenRet cstringType = codegenTypeByName(eltType);

    // Construct the table elements
    std::vector<GenRet> table;
    table.reserve(symbols.size() * 2);

    forv_Vec(FnSymbol, fn, symbols) {
      table.push_back(codegenStringForTable(fn->cname));
      table.push_back(codegenStringForTable(fn->name));
    }
    table.push_back(codegenStringForTable(""));
    table.push_back(codegenStringForTable(""));

    // Now emit the global array declaration
    codegenGlobalConstArray(name, eltType, &table, false);
  }

  // Generate the filename index, linenum table
  {
    const char *name = "chpl_filenumSymTable";
    const char *eltType = "c_int";

    // Compute the element type
    GenRet cintType = codegenTypeByName(eltType);

    // Construct the table elements
    std::vector<GenRet> table;
    table.reserve(symbols.size() * 2);

    forv_Vec(FnSymbol, fn, symbols) {
      int fileno = getFilenameLookupPosition(fn->fname());
      int lineno = fn->linenum();

      table.push_back( new_IntSymbol(fileno, INT_SIZE_32)->codegen() );
      table.push_back( new_IntSymbol(lineno, INT_SIZE_32)->codegen() );
    }
    table.push_back( new_IntSymbol(0, INT_SIZE_32)->codegen() );
    table.push_back( new_IntSymbol(0, INT_SIZE_32)->codegen() );

    // Now emit the global array declaration
    codegenGlobalConstArray(name, eltType, &table, false);
  }

  // Now emit the size of the symbol table
  genGlobalInt32("chpl_sizeSymTable", symbols.size() * 2);
}

static void
genClassNames(std::vector<TypeSymbol*> & typeSymbol, bool isHeader) {
  const char* eltType = "c_string";
  const char* name = "chpl_classNames";

  if(isHeader) {
    // Just pass NULL when generating header
    codegenGlobalConstArray(name, eltType, NULL, true);
    return;
  }

  std::vector<const char*> names;

  forv_Vec(TypeSymbol, ts, typeSymbol) {
    if (AggregateType* ct = toAggregateType(ts->type)) {
      if (isObjectOrSubclass(ct)) {
        int id = ct->classId;
        INT_ASSERT(id > 0);
        if (id >= (int)names.size())
          names.resize(id+1, NULL);
        names[id] = ts->name;
      }
    }
  }

  std::vector<GenRet> tmp;
  for(size_t i = 0; i < names.size(); i++) {
    const char* name = names[i];
    if (name == NULL)
      name = "";
    tmp.push_back(codegenStringForTable(name));
  }

  // Now emit the global array declaration
  codegenGlobalConstArray(name, eltType, &tmp, false);
}


static bool
compareSymbol(const void* v1, const void* v2) {
  Symbol* s1 = (Symbol*)v1;
  Symbol* s2 = (Symbol*)v2;
  ModuleSymbol* m1 = s1->getModule();
  ModuleSymbol* m2 = s2->getModule();
  if (m1 != m2) {
    if (m1->modTag < m2->modTag)
      return 1;
    if (m1->modTag > m2->modTag)
      return 0;
    return strcmp(m1->cname, m2->cname) < 0;
  }

  if (s1->linenum() != s2->linenum())
    return s1->linenum() < s2->linenum();

  int result = strcmp(s1->type->symbol->cname, s2->type->symbol->cname);
  if (!result)
    result = strcmp(s1->cname, s2->cname);

  return result < 0;
}

static int
compareSymbol2(const void* v1, const void* v2) {
  Symbol* s1 = *(Symbol* const *)v1;
  Symbol* s2 = *(Symbol* const *)v2;
  ModuleSymbol* m1 = s1->getModule();
  ModuleSymbol* m2 = s2->getModule();
  if (m1 != m2) {
    if (m1->modTag < m2->modTag)
      return -1;
    if (m1->modTag > m2->modTag)
      return 1;
    return strcmp(m1->cname, m2->cname);
  }

  if (s1->linenum() != s2->linenum())
    return (s1->linenum() < s2->linenum()) ? -1 : 1;

  int result = strcmp(s1->type->symbol->cname, s2->type->symbol->cname);
  if (!result)
    result = strcmp(s1->cname, s2->cname);
  return result;
}

//
// given a name and up to two sets of names, return a name that is in
// neither set and add the name to the first set; the second set may
// be omitted; the returned name to be capped at fMaxCIdentLen if non-0
// less how much can be added to it - maxCNameAddedChars
//
// the unique numbering is based on the map uniquifyNameCounts which
// can be cleared to reset
//
int fMaxCIdentLen = 0;
static const int maxUniquifyAddedChars = 25;
// keep in sync with AggregateType::classStructName()
static const int maxCNameAddedChars = 20;
static char* longCNameReplacementBuffer = NULL;
static Map<const char*, int> uniquifyNameCounts;

// Return the next uniquifying number for 'name'.
// Cache the lookup result in 'elem' - speed up "call_tmp" etc.
static int uniquifyNameNextCount(MapElem<const char*, int>*& elem,
                                 const char* name) {
  if (!elem) {
    elem = uniquifyNameCounts.get_record(name);
    if (!elem) {  // The first time we see 'name'.
      uniquifyNameCounts.put(name, 2);
      return 2;
    }
  }
  return ++elem->value;
}

static void uniquifyName(Symbol* sym,
                         std::set<const char*>* set1,
                         std::set<const char*>* set2 = NULL)
{
  const char* name = sym->cname;
  const char* newName = name;

  if (sym->isRenameable())
  {
    if (fMaxCIdentLen > 0 &&
        (int)(strlen(newName) + maxCNameAddedChars) > fMaxCIdentLen)
    {
      // how much of the name to preserve
      int prefixLen = fMaxCIdentLen - maxUniquifyAddedChars - maxCNameAddedChars;
      if (!longCNameReplacementBuffer) {
        longCNameReplacementBuffer = (char*)malloc(prefixLen+1);
        longCNameReplacementBuffer[prefixLen] = '\0';
      }
      strncpy(longCNameReplacementBuffer, newName, prefixLen);
      INT_ASSERT(longCNameReplacementBuffer[prefixLen] == '\0');
      longCNameReplacementBuffer[prefixLen-1] = 'X'; //fyi truncation marker
      name = newName = astr(longCNameReplacementBuffer);
    }

    MapElem<const char*, int>* elem = NULL;
    while ((set1->find(newName)!=set1->end()) || (set2 && (set2->find(newName)!=set2->end()))) {
      char numberTmp[64];
      snprintf(numberTmp, 64, "%d", uniquifyNameNextCount(elem, name));
      newName = astr(name, numberTmp);
    }

    sym->cname = newName;
  }
  else
  {
    // If we have already seen this name before, we need to go back
    // to that earlier-seen symbol, either renaming it or checking
    // that its type is the same. See also #9299.
    //
    // This is currently not implemented. For now, at least detect it
    // to avoid generating erroneous C code silently.
    //
    // Do this only for things local to a function.
    // We use multiple Symbols for the same extern at the global scope,
    // be it a (possibly generic) function, type, variable.
    // Ex.: c_pointer_return, qbuffer_ptr_t, QBUFFER_PTR_NULL.
    //
    if (set2 && (set1->find(name) != set1->end()) )
      INT_FATAL(sym, "name conflict with a non-renameable symbol");
  }

  // Record the name even if the symbol is not renameable.
  // This enables detection of like-named symbols encountered later.
  set1->insert(newName);
}

static inline bool shouldCodegenAggregate(AggregateType* ct)
{
  // never codegen definitions of primitive or arithmetic types.
  if( toPrimitiveType(ct) ) return false;

  // Don't codegen types with FLAG_NO_CODEGEN.  This is used for
  // types that are defined in the runtime for example.
  if( ct->symbol->hasFlag(FLAG_NO_CODEGEN) ) return false;

  // Don't visit classes since they are prototyped individually all at once..
  // ..except for classes with FLAG_REF or FLAG_DATA_CLASS.. which
  //   we do visit.
  if( isClass(ct) ) { // is it actually a class?
    if( ct->symbol->hasFlag(FLAG_REF) ||
        ct->symbol->hasFlag(FLAG_WIDE_REF) ||
        ct->symbol->hasFlag(FLAG_DATA_CLASS)) return true;
    else return false;
  }

  // otherwise, visit record/union
  return true;
}


static void codegen_aggregate_def(AggregateType* ct) {
  //DFS, check visited
  if (!shouldCodegenAggregate(ct)) return;
  if (ct->symbol->hasFlag(FLAG_CODEGENNED)) return;
  ct->symbol->addFlag(FLAG_CODEGENNED);

  // For reference or data class types, first generate
  // the referenced type
  Type* vt = NULL;
  if(ct->symbol->hasFlag(FLAG_REF))
    vt = ct->symbol->getValType();
  else if(ct->symbol->hasFlag(FLAG_DATA_CLASS))
    vt = getDataClassType(ct->symbol)->typeInfo();
  else if(ct->symbol->hasFlag(FLAG_C_ARRAY))
    vt = ct->cArrayElementType();
  if (vt) {
    if (AggregateType* fct = toAggregateType(vt)) {
      codegen_aggregate_def(fct);
    }
  }
  // For other types, generate the field types
  for_fields(field, ct) {
    if (AggregateType* fct = toAggregateType(field->type)) {
      codegen_aggregate_def(fct);
    }
  }
  // Lastly, generate the type we're working on.
  // Codegen what we have here.
  ct->symbol->codegenDef();
}

static void genConfigGlobalsAndAbout() {
  GenInfo* info = gGenInfo;

  if (info->cfile) {
    genComment("Compilation Info");
    fprintf(info->cfile, "\n#include <stdio.h>");
    fprintf(info->cfile, "\n#include \"chpltypes.h\"\n\n");
  }

  genGlobalString("chpl_compileCommand", compileCommand);
  genGlobalString("chpl_compileVersion", compileVersion);
  genGlobalString("chpl_compileDirectory", getCwd());
  if (strcmp(saveCDir, "") != 0) {
    char *actualPath = realpath(saveCDir, NULL);
    genGlobalString("chpl_saveCDir", actualPath);
  } else {
    genGlobalString("chpl_saveCDir", "");
  }

  genGlobalString("CHPL_HOME", CHPL_HOME);

  genGlobalInt("CHPL_STACK_CHECKS", !fNoStackChecks, false);
  genGlobalInt("CHPL_CACHE_REMOTE", fCacheRemote, false);

  for (std::map<std::string, const char*>::iterator env=envMap.begin(); env!=envMap.end(); ++env) {
    if (env->first != "CHPL_HOME") {
      genGlobalString(env->first.c_str(), env->second);
    }
  }

  if (info->cfile) {
    fprintf(info->cfile, "\nvoid chpl_program_about(void);\n");
    fprintf(info->cfile, "\nvoid chpl_program_about() {\n");
  } else {
#ifdef HAVE_LLVM
    llvm::FunctionType* programAboutType;
    llvm::Function* programAboutFunc;
    if ((programAboutFunc = getFunctionLLVM("chpl_program_about"))) {
      programAboutType = programAboutFunc->getFunctionType();
    } else {
      programAboutType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(info->module->getContext()), false
      );
      programAboutFunc = llvm::Function::Create(
        programAboutType, llvm::Function::ExternalLinkage, "chpl_program_about", info->module
      );
    }

    llvm::BasicBlock* programAboutBlock = llvm::BasicBlock::Create(
      info->module->getContext(), "entry", programAboutFunc
    );
    info->irBuilder->SetInsertPoint(programAboutBlock);
#endif
  }

  codegenCallPrintf(astr("Compilation command: ", compileCommand, "\\n"));
  codegenCallPrintf(astr("Chapel compiler version: ", compileVersion, "\\n"));
  codegenCallPrintf("Chapel environment:\\n");
  codegenCallPrintf(astr("  CHPL_HOME: ", CHPL_HOME, "\\n"));
  for (std::map<std::string, const char*>::iterator env=envMap.begin(); env!=envMap.end(); ++env) {
    if (env->first != "CHPL_HOME") {
      codegenCallPrintf(astr("  ", env->first.c_str(), ": ", env->second, "\\n"));
    }
  }

  if (info->cfile) {
    fprintf(info->cfile, "}\n");
  } else {
#ifdef HAVE_LLVM
    info->irBuilder->CreateRetVoid();
#endif
  }
}

static void genFunctionTables() {
  genComment("Filename Lookup Table");
  genFilenameTable();

  genComment("Unwind symbol tables");
  genUnwindSymbolTable();
}

//
// Produce compilation-time configuration info into a .c file and
// #include that .c into the current codegen output file.
//
// Only put C data objects into this file, not Chapel ones, as it may
// also be #include'd into a launcher, and those are C/C++ code.
//
// New generated variables should be added to runtime/include/chplcgfns.h
//
static const char* sCfgFname = "chpl_compilation_config";

static void codegen_header_compilation_config() {
  const bool usingLauncher = 0 != strcmp(CHPL_LAUNCHER, "none");
  // Generate C code only when not in LLVM mode or when using a launcher
  const bool genCCode = usingLauncher || !llvmCodegen;

  GenInfo* info = gGenInfo;
  FILE* save_cfile = info->cfile;
  fileinfo cfgfile = { NULL, NULL, NULL };

  if (llvmCodegen) {
    info->cfile = NULL;
    genConfigGlobalsAndAbout();
    genFunctionTables();
  }

  // Generate the about info and function tables for the C backend and for the launcher
  if (genCCode) {
    openCFile(&cfgfile, sCfgFname, "c");
    // Follow convention of just not writing to the file if we can't open it
    if (cfgfile.fptr) {
      info->cfile = cfgfile.fptr;
      genConfigGlobalsAndAbout();
      genFunctionTables();
      closeCFile(&cfgfile);
    }
  }

  info->cfile = save_cfile;
}

static void protectNameFromC(Symbol* sym) {
  //
  // Symbols that start with 'chpl_' were presumably named by the
  // implementation (compiler, internal modules, runtime) and
  // sufficiently unique to not require further munging.
  //
  if (strncmp(sym->cname, "chpl_", 5) == 0) {
    return;
  }

  //
  // Don't rename the symbol if it's not able to be (typically because
  // it's exported, extern, or has otherwise been flagged as not being
  // renameable).
  //
  if (!sym->isRenameable()) {
    return;
  }

  // Don't rename fields
  if (isVarSymbol(sym)) {
    if (isAggregateType(sym->defPoint->parentSymbol->type)) {
      return;
    }
  }

  //
  // Walk from the symbol up to its enclosing module.  If the symbol
  // is declared within an extern declaration, we should preserve its
  // name for similar reasons.
  //
  ModuleSymbol* symMod = sym->getModule();
  if (sym != symMod) {
    Symbol* parentSym = sym->defPoint->parentSymbol;
    while (parentSym != symMod) {
      if (parentSym->hasFlag(FLAG_EXTERN)) {
        return;
      }
      parentSym = parentSym->defPoint->parentSymbol;
    }
  }

  //
  // For the sake of clarity, let's also avoid renaming arguments of
  // exported functions.
  //
  if (toArgSymbol(sym)) {
    Symbol* parentSym = sym->defPoint->parentSymbol;
    if (parentSym->hasFlag(FLAG_EXPORT)) {
      return;
    }
  }

  //
  // Rename the symbol
  //
  const char* oldName = sym->cname;
  const char* newName = astr(oldName, "_chpl");
  sym->cname = newName;
  //
  // Can we free this given how we create names?  free() doesn't like
  // const char*, I don't want to just cast it away, and I'm not
  // certain we can assume it isn't aliased to something else, like
  // sym->name...  In other cases, we seem to leak old names as
  // well... :P
  //
  //  free(oldName);
}

static void genGlobalSerializeTable(GenInfo* info) {
  FILE* hdrfile = info->cfile;
  std::vector<CallExpr*> serializeCalls;
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isResolved() && call->resolvedFunction()->hasFlag(FLAG_BROADCAST_FN)) {
      SymExpr* se = toSymExpr(call->get(2));
      INT_ASSERT(se != NULL);

      VarSymbol* imm = toVarSymbol(se->symbol());
      INT_ASSERT(imm && imm->isImmediate());
      uint64_t idx = imm->immediate->int_value();

      if (idx+1 > serializeCalls.size()) {
        serializeCalls.resize(idx+1);
      }

      serializeCalls[idx] = call;
    }
  }

  if( hdrfile ) {
    fprintf(hdrfile, "\nvoid* const chpl_global_serialize_table[] = {");
    if (serializeCalls.size() == 0) {
      // Quiet PGI warning about empty initializer
      fprintf(hdrfile, "\nNULL,");
    } else {
      for (unsigned int i = 0; i < serializeCalls.size(); i++) {
        CallExpr* call = serializeCalls[i];
        INT_ASSERT(call != NULL);
        SymExpr* global = toSymExpr(call->get(1));
        INT_ASSERT(isModuleSymbol(global->symbol()->defPoint->parentSymbol));

        const char* prefix = i == 0 ? "\n&%s" : ",\n&%s";
        fprintf(hdrfile, prefix, global->symbol()->cname);
      }
    }
    fprintf(hdrfile, "\n};\n");
  } else {
#ifdef HAVE_LLVM
    llvm::Type *global_serializeTableEntryType =
      llvm::IntegerType::getInt8PtrTy(info->module->getContext());

    std::vector<llvm::Constant *> global_serializeTable;

    for_vector(CallExpr, call, serializeCalls) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);

      global_serializeTable.push_back(llvm::cast<llvm::Constant>(
            info->irBuilder->CreatePointerCast(
              info->lvt->getValue(se->symbol()->cname).val,
              global_serializeTableEntryType)));
    }

    if(llvm::GlobalVariable *GVar = llvm::cast_or_null<llvm::GlobalVariable>(
          info->module->getNamedGlobal("chpl_global_serialize_table"))) {
      GVar->eraseFromParent();
    }

    llvm::ArrayType *global_serializeTableType =
      llvm::ArrayType::get(global_serializeTableEntryType,
                          global_serializeTable.size());
    llvm::GlobalVariable *global_serializeTableGVar =
      llvm::cast<llvm::GlobalVariable>(
          info->module->getOrInsertGlobal("chpl_global_serialize_table",
                                          global_serializeTableType));
    global_serializeTableGVar->setInitializer(
        llvm::ConstantArray::get(
          global_serializeTableType, global_serializeTable));
    info->lvt->addGlobalValue("chpl_global_serialize_table",
                              global_serializeTableGVar, GEN_PTR, true);
#endif
  }
}

// TODO: Split this into a number of smaller routines.<hilde>
static void codegen_defn(std::set<const char*> & cnames, std::vector<TypeSymbol*> & types,
  std::vector<FnSymbol*> & functions, std::vector<VarSymbol*> & globals) {
  GenInfo* info = gGenInfo;
  FILE* hdrfile = info->cfile;

  genClassIDs(types, false);
  genSubclassArray(false);
  genClassNames(types, false);

  genComment("Function Pointer Table");
  genFtable(ftableVec, false);
  genFinfo(ftableVec, false);

  genComment("Virtual Method Table");
  genVirtualMethodTable(types, false);

  if(fIncrementalCompilation) {
    genComment("Global Variables");
    forv_Vec(VarSymbol, varSymbol, globals) {
      varSymbol->codegenGlobalDef(false);
    }
  }

  flushStatements();
#ifndef HAVE_LLVM
  zlineToFileIfNeeded(rootModule, info->cfile);
#endif

  genComment("Global Serialize Table");
  genGlobalSerializeTable(info);

  genGlobalInt("chpl_numGlobalsOnHeap", numGlobalsOnHeap, false);
  int globals_registry_static_size = (numGlobalsOnHeap ? numGlobalsOnHeap : 1);
  if( hdrfile ) {
    fprintf(hdrfile, "\nptr_wide_ptr_t chpl_globals_registry[%d];\n",
                      globals_registry_static_size);
  } else {
    #ifdef HAVE_LLVM
          return; // Nothing in remainder of function should be done twice for LLVM
    #endif
  }
  if( hdrfile ) {
    fprintf(hdrfile, "\nconst char* chpl_mem_descs[] = {\n");
    bool first = true;
    if (memDescsVec.n == 0) {
      // Quiet PGI warning about empty initializer
      fprintf(hdrfile, "\nNULL,");
    } else {
      forv_Vec(const char*, memDesc, memDescsVec) {
        if (!first)
          fprintf(hdrfile, ",\n");
        fprintf(hdrfile, "\"%s\"", memDesc);
        first = false;
      }
    }
    fprintf(hdrfile, "\n};\n");
  }

  genGlobalInt("chpl_mem_numDescs", memDescsVec.n, false);

  //
  // add table of private-broadcast constants
  //
  if( hdrfile ) {
    fprintf(hdrfile, "\nvoid* const chpl_private_broadcast_table[] = {\n");
    int i = 0;
    forv_Vec(CallExpr, call, gCallExprs) {
      if (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) {
        SymExpr* se = toSymExpr(call->get(1));
        INT_ASSERT(se);
        SET_LINENO(call);
        fprintf(hdrfile, "%s&%s",
                ((i == 0) ? "" : ",\n"), se->symbol()->cname);
        // To preserve operand order, this should be insertAtTail.
        // The change must also be made below (for LLVM) and in the signature
        // of chpl_comm_broadcast_private().
        call->insertAtHead(new_IntSymbol(i));
        i++;
      }
    }
    if (i == 0) {
      // Quiet PGI warning about empty initializer
      fprintf(hdrfile, "NULL");
    }
    fprintf(hdrfile, "\n};\n");
    genGlobalInt("chpl_private_broadcast_table_len", i, false);
  }
}

static void codegen_header(std::set<const char*> & cnames, std::vector<TypeSymbol*> & types,
  std::vector<FnSymbol*> & functions, std::vector<VarSymbol*> & globals) {
  GenInfo* info = gGenInfo;

  // reserved symbol names that require renaming to compile
#include "reservedSymbolNames.h"

  //
  // collect types and apply canonical sort
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      legalizeName(ts);
      types.push_back(ts);
    }
  }
  std::sort(types.begin(), types.end(), compareSymbol);

  //
  // collect globals and apply canonical sort
  //
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->defPoint->parentExpr != rootModule->block &&
        toModuleSymbol(var->defPoint->parentSymbol)) {
      legalizeName(var);
      globals.push_back(var);
    }
  }
  std::sort(globals.begin(), globals.end(), compareSymbol);
  //
  // collect functions and apply canonical sort
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    legalizeName(fn);
    functions.push_back(fn);
  }
  std::sort(functions.begin(), functions.end(), compareSymbol);


  //
  // by default, mangle all Chapel symbols to avoid clashing with C
  // identifiers.  To disable, compile with --no-munge-user-idents
  //
  if (fMungeUserIdents) {
    forv_Vec(ModuleSymbol, sym, gModuleSymbols) {
      protectNameFromC(sym);
    }
    forv_Vec(VarSymbol, sym, gVarSymbols) {
      protectNameFromC(sym);
    }
    forv_Vec(ShadowVarSymbol, sym, gShadowVarSymbols) {
      protectNameFromC(sym);
    }
    forv_Vec(ArgSymbol, sym, gArgSymbols) {
      protectNameFromC(sym);
    }
    forv_Vec(TypeSymbol, sym, gTypeSymbols) {
      protectNameFromC(sym);
    }
    forv_Vec(FnSymbol, sym, gFnSymbols) {
      protectNameFromC(sym);
    }
    forv_Vec(EnumSymbol, sym, gEnumSymbols) {
      protectNameFromC(sym);
    }
    forv_Vec(LabelSymbol, sym, gLabelSymbols) {
      protectNameFromC(sym);
    }
  }


  //
  // mangle type names if they clash with other types
  //
  forv_Vec(TypeSymbol, ts, types) {
    uniquifyName(ts, &cnames);
  }
  uniquifyNameCounts.clear();

  //
  // change enum constant names into <type name>_<constant name> and
  // mangle if they clash with other types or enum constants
  //
  forv_Vec(TypeSymbol, ts, types) {
    if (EnumType* enumType = toEnumType(ts->type)) {
      for_enums(constant, enumType) {
        Symbol* sym = constant->sym;
        legalizeName(sym);
        sym->cname = astr(enumType->symbol->cname, "_", sym->cname);
        uniquifyName(sym, &cnames);
      }
    }
  }
  uniquifyNameCounts.clear();

  //
  // mangle field names if they clash with other fields in the same
  // class
  //
  forv_Vec(TypeSymbol, ts, types) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      if (AggregateType* ct = toAggregateType(ts->type)) {
        std::set<const char*> fieldNameSet;
        for_fields(field, ct) {
          legalizeName(field);
          uniquifyName(field, &fieldNameSet);
        }
        uniquifyNameCounts.clear();
      }
    }
  }

  //
  // mangle global variable names if they clash with types, enum
  // constants, or other global variables
  //
  forv_Vec(VarSymbol, var, globals) {
    uniquifyName(var, &cnames);
  }
  uniquifyNameCounts.clear();

  //
  // mangle function names if they clash with types, enum constants,
  // global variables, or other functions
  //
  for_vector(FnSymbol, fn, functions) {
    uniquifyName(fn, &cnames);
  }
  uniquifyNameCounts.clear();

  //
  // mangle formal argument names if they clash with types, enum
  // constants, global variables, functions, or earlier formal
  // arguments in the same function
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    std::set<const char*> formalNameSet;
    for_formals(formal, fn) {
      legalizeName(formal);
      uniquifyName(formal, &formalNameSet, &cnames);
    }
    uniquifyNameCounts.clear();
  }

  //
  // mangle local variable names if they clash with types, global
  // variables, functions, formal arguments of their function, or
  // other local variables in the same function
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    std::set<const char*> local;

    for_formals(formal, fn) {
      local.insert(formal->cname);
    }

    std::vector<DefExpr*> defs;
    collectDefExprs(fn->body, defs);
    for_vector(DefExpr, def, defs) {
      legalizeName(def->sym);
      // give temps cnames
      if (def->sym->hasFlag(FLAG_TEMP)) {
        if (localTempNames) {
          // temp name is _tNNN_
          if (!strncmp(def->sym->cname, "_t", 2))
            def->sym->cname = astr("T", def->sym->cname + 2);
        } else {
          // temp name is tmp
          if (!strcmp(def->sym->cname, "tmp"))
            def->sym->cname = astr("T");
        }
      }
      uniquifyName(def->sym, &local, &cnames);
    }
    uniquifyNameCounts.clear();
  }

  codegen_header_compilation_config();

  if (fLibraryCompile) {
    codegen_library_header(functions);
    codegen_library_python(functions);
    codegen_library_fortran(functions);
  }

  FILE* hdrfile = info->cfile;

  if( hdrfile) {
    // Insert include guard to help MLI builds avoid multiple inclusion.
    fprintf(hdrfile, "#ifndef CHPL_GEN_HEADER_INCLUDE_GUARD\n");
    fprintf(hdrfile, "#define CHPL_GEN_HEADER_INCLUDE_GUARD\n");

    // This is done in runClang for LLVM version.
    fprintf(hdrfile, "\n#define CHPL_GEN_CODE\n\n");

    // Include sys_basic.h to get C types always defined,
    // proper library .h inclusion
    fprintf(hdrfile, "#include \"sys_basic.h\"\n");
    genIncludeCommandLineHeaders(hdrfile);

    fprintf(hdrfile, "#include \"stdchpl.h\"\n");

#ifdef HAVE_LLVM
    //include generated extern C header file
    if (externC && gAllExternCode.filename != NULL) {
      fprintf(hdrfile, "%s", astr("#include \"", gAllExternCode.filename, "\"\n"));
      // If we wanted to, here is where we would re-enable
      // the memory warning macros.
    }
#endif
  }

  assignClassIds();
  genClassIDs(types, true);
  genSubclassArray(true);
  genClassNames(types, true);

  genComment("Class Prototypes");
  forv_Vec(TypeSymbol, typeSymbol, types) {
    if (!typeSymbol->hasFlag(FLAG_REF) && !typeSymbol->hasFlag(FLAG_DATA_CLASS))
    {
      typeSymbol->codegenPrototype();
    }
  }

  // codegen enumerated types
  genComment("Enumerated Types");
  forv_Vec(TypeSymbol, typeSymbol, types) {
    if (toEnumType(typeSymbol->type)) {
      typeSymbol->codegenDef();
    }
  }

  // codegen records/unions/references/data class in topological order
  genComment("Records, Unions, Data Class, References (Hierarchically)");
  forv_Vec(TypeSymbol, ts, types) {
    if (AggregateType* ct = toAggregateType(ts->type))
      codegen_aggregate_def(ct);
  }

  // codegen remaining types
  genComment("Classes");
  forv_Vec(TypeSymbol, typeSymbol, types) {
    if (isClass(typeSymbol->type) &&
        !typeSymbol->hasFlag(FLAG_REF) &&
        !typeSymbol->hasFlag(FLAG_DATA_CLASS) &&
        typeSymbol->hasFlag(FLAG_NO_OBJECT) &&
        !typeSymbol->hasFlag(FLAG_OBJECT_CLASS))
      typeSymbol->codegenDef();
  }

  //
  // codegen class definitions in breadth first order starting with
  // "object" and following its dispatch children
  //

  Vec<TypeSymbol*> next, current;

  current.add(dtObject->symbol);

  while (current.n) {
    forv_Vec(TypeSymbol, ts, current) {
      ts->codegenDef();

      if (AggregateType* at = toAggregateType(ts->type)) {
        forv_Vec(AggregateType, child, at->dispatchChildren) {
          if (child != NULL) {
            next.set_add(child->symbol);
          }
        }
      }
    }

    current.clear();
    current.move(next);
    current.set_to_vec();

    qsort(current.v, current.n, sizeof(current.v[0]), compareSymbol2);

    next.clear();
  }

  if(!info->cfile) {
    // Codegen any type annotations that are necessary.
    // Start with primitive types in case they are referenced by
    // records or classes.
    forv_Vec(TypeSymbol, typeSymbol, gTypeSymbols) {
      if (typeSymbol->defPoint->parentExpr == rootModule->block &&
          isPrimitiveType(typeSymbol->type) &&
          typeSymbol->llvmType) {
        typeSymbol->codegenMetadata();
      }
    }
    forv_Vec(TypeSymbol, typeSymbol, types) {
      if (!isDecoratedClassType(typeSymbol->type))
        typeSymbol->codegenMetadata();
    }
    // Aggregate annotations for class objects must wait until all other
    // type annotations are defined, because there might be cycles.
    forv_Vec(TypeSymbol, typeSymbol, types) {
      if (isClass(typeSymbol->type))
        typeSymbol->codegenAggMetadata();
    }
  }

  genComment("Function Prototypes");
  for_vector(FnSymbol, fnSymbol, functions) {
    fnSymbol->codegenPrototype();
  }

  genComment("Function Pointer Table");
  for_vector(FnSymbol, fn2, functions) {
    if (fn2->hasFlag(FLAG_BEGIN_BLOCK) ||
        fn2->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK) ||
        fn2->hasFlag(FLAG_ON_BLOCK)) {
    ftableVec.push_back(fn2);
    ftableMap[fn2] = ftableVec.size()-1;
    }
  }

  genFtable(ftableVec,true);
  genFinfo(ftableVec,true);

  genComment("Virtual Method Table");
  genVirtualMethodTable(types,true);

  genComment("Global Variables");
  forv_Vec(VarSymbol, varSymbol, globals) {
    varSymbol->codegenGlobalDef(true);
  }
  flushStatements();

  genGlobalInt("chpl_numGlobalsOnHeap", numGlobalsOnHeap, true);
  int globals_registry_static_size = (numGlobalsOnHeap ? numGlobalsOnHeap : 1);
  if( hdrfile ) {
    fprintf(hdrfile, "\nextern ptr_wide_ptr_t chpl_globals_registry[%d];\n",
                    globals_registry_static_size);
  } else {
#ifdef HAVE_LLVM
    llvm::Type* ptr_wide_ptr_t = info->lvt->getType("ptr_wide_ptr_t");
    INT_ASSERT(ptr_wide_ptr_t);

    if(llvm::GlobalVariable *GVar = llvm::cast_or_null<llvm::GlobalVariable>(
          info->module->getNamedGlobal("chpl_globals_registry"))) {
      GVar->eraseFromParent();
    }
    llvm::GlobalVariable *chpl_globals_registryGVar =
      llvm::cast<llvm::GlobalVariable>(
          info->module->getOrInsertGlobal("chpl_globals_registry",
            llvm::ArrayType::get(
              ptr_wide_ptr_t,
              globals_registry_static_size)));
    chpl_globals_registryGVar->setInitializer(
        llvm::Constant::getNullValue(
          chpl_globals_registryGVar->getType()->getContainedType(0)));
    info->lvt->addGlobalValue("chpl_globals_registry",
                              chpl_globals_registryGVar, GEN_PTR, true);
#endif
  }
  if( hdrfile ) {
      fprintf(hdrfile, "\nextern const char* chpl_mem_descs[];\n");
    } else {
#ifdef HAVE_LLVM
    std::vector<llvm::Constant *> memDescTable;
    forv_Vec(const char*, memDesc, memDescsVec) {
      memDescTable.push_back(llvm::cast<llvm::GlobalVariable>(
            new_CStringSymbol(memDesc)->codegen().val)->getInitializer());
    }
    llvm::ArrayType *memDescTableType = llvm::ArrayType::get(
        llvm::IntegerType::getInt8PtrTy(info->module->getContext()),
        memDescTable.size());

    if(llvm::GlobalVariable *GVar =llvm::cast_or_null<llvm::GlobalVariable>(
          info->module->getNamedGlobal("chpl_mem_descs"))) {
      GVar->eraseFromParent();
    }

    llvm::GlobalVariable *chpl_memDescsGVar = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal("chpl_mem_descs", memDescTableType));
    chpl_memDescsGVar->setInitializer(
        llvm::ConstantArray::get(memDescTableType, memDescTable));
    chpl_memDescsGVar->setConstant(true);
    info->lvt->addGlobalValue("chpl_mem_descs",chpl_memDescsGVar,GEN_PTR,true);
#endif
  }

  genGlobalInt("chpl_mem_numDescs", memDescsVec.size(), true);

  //
  // add table of private-broadcast constants
  //
  if( hdrfile ) {
    fprintf(hdrfile, "\nextern void* const chpl_private_broadcast_table[];\n");
  } else {
#ifdef HAVE_LLVM
    llvm::Type *private_broadcastTableEntryType =
      llvm::IntegerType::getInt8PtrTy(info->module->getContext());

    std::vector<llvm::Constant *> private_broadcastTable;

    int broadcastID = 0;
    forv_Vec(CallExpr, call, gCallExprs) {
      if (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) {
        SymExpr* se = toSymExpr(call->get(1));
        INT_ASSERT(se);

        private_broadcastTable.push_back(llvm::cast<llvm::Constant>(
              info->irBuilder->CreatePointerCast(
                info->lvt->getValue(se->symbol()->cname).val,
                private_broadcastTableEntryType)));
        // To preserve operand order, this should be insertAtTail.
        call->insertAtHead(new_IntSymbol(broadcastID++));
      }
    }

    if(llvm::GlobalVariable *GVar = llvm::cast_or_null<llvm::GlobalVariable>(
          info->module->getNamedGlobal("chpl_private_broadcast_table"))) {
      GVar->eraseFromParent();
    }

    llvm::ArrayType *private_broadcastTableType =
      llvm::ArrayType::get(private_broadcastTableEntryType,
                          private_broadcastTable.size());
    llvm::GlobalVariable *private_broadcastTableGVar =
      llvm::cast<llvm::GlobalVariable>(
          info->module->getOrInsertGlobal("chpl_private_broadcast_table",
                                          private_broadcastTableType));
    private_broadcastTableGVar->setInitializer(
        llvm::ConstantArray::get(
          private_broadcastTableType, private_broadcastTable));
    info->lvt->addGlobalValue("chpl_private_broadcast_table",
                              private_broadcastTableGVar, GEN_PTR, true);
    genGlobalInt("chpl_private_broadcast_table_len",
                 private_broadcastTable.size(), false);
#endif
  }

  if (hdrfile) {
    fprintf(hdrfile, "#include \"chpl-gen-includes.h\"\n");
  }
}

// Sometimes we have to define a type while code generating.
// When that happens, we need to add a little bit to the header...
// This is only needed for C (since in LLVM we must add
//  the types as we use them).
static void codegen_header_addons() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      if (AggregateType* ct = toAggregateType(ts->type))
        codegen_aggregate_def(ct);
    }
  }
}

static void
codegen_config() {
  GenInfo* info = gGenInfo;

  // LLVM backend need _config.c generated for the launcher,
  // so we produce the C for it either way.
  {
    FILE* mainfile = info->cfile;
    if( mainfile ) fprintf(mainfile, "#include \"_config.c\"\n");
    fileinfo configFile;
    openCFile(&configFile, "_config.c");
    FILE* outfile = configFile.fptr;
    info->cfile = outfile;

    fprintf(outfile, "#include \"error.h\"\n\n");

    genGlobalInt("mainHasArgs", mainHasArgs, false);

    fprintf(outfile, "void CreateConfigVarTable(void) {\n");
    fprintf(outfile, "initConfigVarTable();\n");

    forv_Vec(VarSymbol, var, gVarSymbols) {
      if (var->hasFlag(FLAG_CONFIG) && !var->isType()) {
        fprintf(outfile, "installConfigVar(\"%s\", \"", var->name);
        Type* type = var->type;
        if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
          type = type->getField("addr")->type;
        if (type->symbol->hasFlag(FLAG_HEAP))
          type = type->getField("value")->type;
        if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
          type = type->getField("addr")->type;
        fprintf(outfile, "%s", type->symbol->name);
        if (var->getModule()->modTag == MOD_INTERNAL) {
          fprintf(outfile, "\", \"Built-in\"");
        } else {
          fprintf(outfile, "\", \"%s\"", var->getModule()->name);
        }
        fprintf(outfile,", /* private = */ %d);\n", var->hasFlag(FLAG_PRIVATE));

      }
    }

    fprintf(outfile, "}\n\n\n");

    closeCFile(&configFile);
    info->cfile = mainfile;
  }


  if( llvmCodegen ) {
#ifdef HAVE_LLVM
    llvm::FunctionType *createConfigType;
    llvm::Function *createConfigFunc;
    genGlobalInt("mainHasArgs", mainHasArgs, false);
    if((createConfigFunc = getFunctionLLVM("CreateConfigVarTable"))) {
      createConfigType = createConfigFunc->getFunctionType();
    }
    else {
      createConfigType = llvm::FunctionType::get(
          llvm::Type::getVoidTy(info->module->getContext()), false);
      createConfigFunc =
        llvm::Function::Create(createConfigType,
                               llvm::Function::ExternalLinkage,
                               "CreateConfigVarTable", info->module);
    }

    llvm::BasicBlock *createConfigBlock =
      llvm::BasicBlock::Create(info->module->getContext(),
                               "entry", createConfigFunc);
    info->irBuilder->SetInsertPoint(createConfigBlock);

    llvm::Function *initConfigFunc = getFunctionLLVM("initConfigVarTable");
    info->irBuilder->CreateCall(initConfigFunc, {} );

    llvm::Function *installConfigFunc = getFunctionLLVM("installConfigVar");

    forv_Vec(VarSymbol, var, gVarSymbols) {
      if (var->hasFlag(FLAG_CONFIG) && !var->isType()) {
        std::vector<llvm::Value *> args (4);
        args[0] = info->irBuilder->CreateLoad(
            new_CStringSymbol(var->name)->codegen().val);

        Type* type = var->type;
        if (type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          type = type->getField("addr")->type;
        }
        if (type->symbol->hasFlag(FLAG_HEAP)) {
          type = type->getField("value")->type;
        }
        if (type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          type = type->getField("addr")->type;
        }
        args[1] = info->irBuilder->CreateLoad(
            new_CStringSymbol(type->symbol->name)->codegen().val);

        if (var->getModule()->modTag == MOD_INTERNAL) {
          args[2] = info->irBuilder->CreateLoad(
              new_CStringSymbol("Built-in")->codegen().val);
        }
        else {
          args[2] =info->irBuilder->CreateLoad(
              new_CStringSymbol(var->getModule()->name)->codegen().val);
        }

        args[3] = info->irBuilder->getInt32(var->hasFlag(FLAG_PRIVATE));

        info->irBuilder->CreateCall(installConfigFunc, args);
      }
    }
    info->irBuilder->CreateRetVoid();
    //llvm::verifyFunction(*createConfigFunc);
#endif
  }
}

static const char* generateFileName(ChainHashMap<char*, StringHashFns, int>& filenames, const char* name, const char* currentModuleName){
  // Macs are case-insensitive when it comes to files, so
  // the following bit of code creates a unique filename
  // with case-insensitivity taken into account

  // create the lowercase filename
  char lowerFilename[FILENAME_MAX];
  sprintf(lowerFilename, "%s", currentModuleName);
  for (unsigned int i=0; i<strlen(lowerFilename); i++) {
    lowerFilename[i] = tolower(lowerFilename[i]);
  }

  // create a filename by bumping a version number until we get a
  // filename we haven't seen before
  char filename[FILENAME_MAX];
  sprintf(filename, "%s", lowerFilename);
  int version = 1;
  while (filenames.get(filename)) {
    version++;
    int wanted_to_write = snprintf(filename, sizeof(filename), "%s%d",
                                   lowerFilename, version);
    if (wanted_to_write < 0) {
      USR_FATAL("character encoding error while generating file name");
    } else if ((size_t)wanted_to_write >= sizeof(filename)) {
      USR_FATAL("module name '%s' is too long to be the basis for a file name",
                currentModuleName);
    }
  }
  filenames.put(filename, 1);

  // build the real filename using that version number -- preserves
  // case by default by going back to currentModule->name rather
  // than using the lowercase filename
  if (version == 1) {
    sprintf(filename, "%s", currentModuleName);
  } else {
    sprintf(filename, "%s%d", currentModuleName, version);
  }

  name = astr(filename);
  return name;
}


static bool
shouldChangeArgumentTypeToRef(ArgSymbol* arg) {
  FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);

  bool shouldPassRef = (arg->intent & INTENT_FLAG_REF) ||
                       arg->requiresCPtr();

  bool alreadyRef = arg->typeInfo()->symbol->hasFlag(FLAG_REF) ||
                    arg->isRef() ||
                    arg->isWideRef();

  // Only change argument types for functions with a ref intent
  // that don't already have an argument being passed by ref
  // and that aren't extern functions.
  return (shouldPassRef &&
          !alreadyRef &&
          !fn->hasFlag(FLAG_EXTERN) &&
          // TODO: Consider flag for export wrappers instead of this.
          !(fLibraryCompile && fn->hasFlag(FLAG_EXPORT)) &&
          !arg->hasFlag(FLAG_NO_CODEGEN));
}

static void
adjustArgSymbolTypesForIntent(void)
{
  // Adjust ArgSymbols that have ref/const ref concrete
  // intent so that their type is ref. This allows the
  // rest of this code to work as expected.
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (shouldChangeArgumentTypeToRef(arg)) {
      arg->qual   = QUAL_REF;
      arg->intent = INTENT_REF;
    }
  }
}

static void convertToRefTypes() {
#define updateSymbols(SymType) \
  forv_Vec(SymType, sym, g##SymType##s) { \
    QualifiedType q = sym->qualType(); \
    Type* type      = q.type(); \
    if (q.isRef() && !q.isRefType()) { \
      type = getOrMakeRefTypeDuringCodegen(type); \
    } else if (q.isWideRef() && !q.isWideRefType()) { \
      type = getOrMakeRefTypeDuringCodegen(type); \
      type = getOrMakeWideTypeDuringCodegen(type); \
    } \
    sym->type = type; \
    if (type->symbol->hasFlag(FLAG_REF)) { \
      sym->qual = QUAL_REF; \
    } else if (type->symbol->hasFlag(FLAG_WIDE_REF)) { \
      sym->qual = QUAL_WIDE_REF; \
    } \
  }

  updateSymbols(VarSymbol);
  updateSymbols(ArgSymbol);
  updateSymbols(ShadowVarSymbol);

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->getReturnSymbol()) {
      fn->retType = fn->getReturnSymbol()->type;
    }
  }

#undef updateSymbols
}

extern bool printCppLineno;
debug_data *debug_info=NULL;




#ifdef HAVE_LLVM

// handle e.g. chpl_clang_builtin_wrapper_cabs

static bool hasWrapper(const char *name)
{
  auto it = chplClangBuiltinWrappedFunctions.find(name);
  if(it != end(chplClangBuiltinWrappedFunctions))
    return true;
  return false;
}

static const char* getClangBuiltinWrappedName(const char* name)
{
  auto it = chplClangBuiltinWrappedFunctions.find(name);
  if(it != end(chplClangBuiltinWrappedFunctions))
    return astr(WRAPPER_PREFIX, name);
  return name;
}
#endif

// Set the executable name to the name of the file containing the
// main module (minus its path and extension) if it isn't set
// already.  If in library mode, set the name of the header file as well.
static void setupDefaultFilenames() {
  if (executableFilename[0] == '\0') {
    ModuleSymbol* mainMod = ModuleSymbol::mainModule();
    const char* mainModFilename = mainMod->astloc.filename;
    const char* filename = stripdirectories(mainModFilename);

    // "Executable" name should be given a "lib" prefix in library compilation,
    // and just the main module name in normal compilation.
    if (fLibraryCompile) {
      // If the header name isn't set either, don't use the prefix version
      if (libmodeHeadername[0] == '\0') {
        // copy from that slash onwards into the libmodeHeadername,
        // saving space for a `\0` terminator
        if (strlen(filename) >= sizeof(libmodeHeadername)) {
          INT_FATAL("input filename exceeds header filename buffer size");
        }
        strncpy(libmodeHeadername, filename, sizeof(libmodeHeadername)-1);
        libmodeHeadername[sizeof(libmodeHeadername)-1] = '\0';
        // remove the filename extension from the library header name.
        char* lastDot = strrchr(libmodeHeadername, '.');
        if (lastDot == NULL) {
          INT_FATAL(mainMod,
                    "main module filename is missing its extension: %s\n",
                    libmodeHeadername);
        }
        *lastDot = '\0';
      }
      if (strlen(filename) >= sizeof(executableFilename) - 3) {
        INT_FATAL("input filename exceeds executable filename buffer size");
      }
      strncpy(executableFilename, filename,
              sizeof(executableFilename)-1);

      if (fLibraryPython && pythonModulename[0] == '\0') {
        strncpy(pythonModulename, filename, sizeof(pythonModulename)-1);
        pythonModulename[sizeof(pythonModulename)-1] = '\0';
        char* lastDot = strrchr(pythonModulename, '.');
        if (lastDot == NULL) {
          INT_FATAL(mainMod,
                    "main module filename is missing its extension: %s\n",
                    pythonModulename);
        }
        *lastDot = '\0';
      }

    } else {
      // copy from that slash onwards into the executableFilename,
      // saving space for a `\0` terminator
      if (strlen(filename) >= sizeof(executableFilename)) {
        INT_FATAL("input filename exceeds executable filename buffer size");
      }
      strncpy(executableFilename, filename, sizeof(executableFilename)-1);
      executableFilename[sizeof(executableFilename)-1] = '\0';
    }

    // remove the filename extension from the executable filename
    char* lastDot = strrchr(executableFilename, '.');
    if (lastDot == NULL) {
      INT_FATAL(mainMod, "main module filename is missing its extension: %s\n",
                executableFilename);
    }
    *lastDot = '\0';

  }

  // If we're in library mode and the executable name was set but the header
  // name wasn't, use the executable name for the header name as well
  if (fLibraryCompile && libmodeHeadername[0] == '\0') {
    strncpy(libmodeHeadername, executableFilename, sizeof(libmodeHeadername)-1);
    libmodeHeadername[sizeof(libmodeHeadername)-1] = '\0';
  }

  // If we're in library mode and the library name was explicitly set, use that
  // name for the python module.
  if (fLibraryCompile && fLibraryPython && pythonModulename[0] == '\0') {
    strncpy(pythonModulename, executableFilename, sizeof(pythonModulename)-1);
    pythonModulename[sizeof(pythonModulename)-1] = '\0';
  }
}


void codegen() {
  if (no_codegen)
    return;

  if( fLLVMWideOpt ) {
    // --llvm-wide-opt is picky about other settings.
    // Check them here.
    if (!llvmCodegen ) USR_FATAL("--llvm-wide-opt requires --llvm");
  }

  // Prepare primitives for codegen
  CallExpr::registerPrimitivesForCodegen();

  setupDefaultFilenames();

  if( llvmCodegen ) {
#ifndef HAVE_LLVM
    USR_FATAL("This compiler was built without LLVM support");
#else
    // Initialize the global gGenInfo for for LLVM code generation
    // by starting out with data from running clang on C dependencies.
    runClang(NULL);
#endif
  } else {
    // Initialize the global gGenInfo for C code generation
    gGenInfo = new GenInfo();
  }

  SET_LINENO(rootModule);

  fileinfo hdrfile    = { NULL, NULL, NULL };
  fileinfo mainfile   = { NULL, NULL, NULL };
  fileinfo defnfile   = { NULL, NULL, NULL };
  fileinfo strconfig  = { NULL, NULL, NULL };

  GenInfo* info     = gGenInfo;

  INT_ASSERT(info);

  adjustArgSymbolTypesForIntent();

  convertToRefTypes();

  // Wrap calls to chosen functions from c library
  if( llvmCodegen ) {
#ifdef HAVE_LLVM
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_EXTERN)) {
          if(hasWrapper(fn->cname))
            fn->cname = getClangBuiltinWrappedName(fn->cname);
      }
    }
#endif
  }

  if( llvmCodegen ) {
#ifdef HAVE_LLVM

    if(fIncrementalCompilation)
      USR_FATAL("Incremental compilation is not yet supported with LLVM");

    if(printCppLineno || debugCCode)
    {
      debug_info = new debug_data(*info->module);
    }
    if(debug_info) {
      // first find the main module, this will be the compile unit.
      forv_Vec(ModuleSymbol, currentModule, allModules) {
        if(currentModule->hasFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE)) {
          //So, this is pretty quick. I'm assuming that the main module is in the current dir, no optimization (need to figure out how to get this)
          // and no compile flags, since I can't figure out how to get that either.
          const char *current_dir = "./";
          const char *empty_string = "";
          debug_info->create_compile_unit(currentModule->astloc.filename, current_dir, false, empty_string);
          break;
        }
      }
    }

    prepareCodegenLLVM();
#endif
  } else {
    openCFile(&hdrfile,  "chpl__header", "h");
    openCFile(&mainfile, "_main",        "c");
    openCFile(&defnfile, "chpl__defn",    "c");
    openCFile(&strconfig,  "chpl_str_config", "c");

    zlineToFileIfNeeded(rootModule, mainfile.fptr);
    fprintf(mainfile.fptr, "#include \"chpl_str_config.c\"\n");
    fprintf(mainfile.fptr, "#include \"chpl__header.h\"\n");
    fprintf(mainfile.fptr, "#include \"%s.c\"\n", sCfgFname);
    fprintf(mainfile.fptr, "#include \"chpl__defn.c\"\n");

    std::vector<const char*> userFileName;
    if(fIncrementalCompilation) {
      ChainHashMap<char*, StringHashFns, int> fileNameHashMap;
      forv_Vec(ModuleSymbol, currentModule, allModules) {
        const char* filename = NULL;
        filename = generateFileName(fileNameHashMap, filename, currentModule->name);
        if(currentModule->modTag == MOD_USER) {
          fileinfo modulefile;
          openCFile(&modulefile, filename, "c");
          int modulePathLen = strlen(astr(modulefile.pathname));
          char path[FILENAME_MAX];
          strncpy(path, astr(modulefile.pathname), modulePathLen-2);
          path[modulePathLen-2]='\0';
          userFileName.push_back(astr(path));
          closeCFile(&modulefile);
        }
      }
    }
    
    codegen_makefile(&mainfile, NULL, false, userFileName);
  }

  if (fLibraryCompile && fLibraryMakefile) {
    codegen_library_makefile();
  }

  // Vectors to store different symbol names to be used while generating header
  std::set<const char*> cnames;
  std::vector<TypeSymbol*> types;
  std::vector<FnSymbol*> functions;
  std::vector<VarSymbol*> globals;

  // This dumps the generated sources into the build directory.
  info->cfile = hdrfile.fptr;
  codegen_header(cnames, types, functions, globals);

  // Prepare the LLVM IR dumper for code generation
  // This needs to happen after protectNameFromC which happens
  // currently in codegen_header.
  preparePrintLlvmIrForCodegen();

  info->cfile = defnfile.fptr;
  codegen_defn(cnames, types, functions, globals);

  info->cfile = mainfile.fptr;
  codegen_config();

  // Don't need to do most of the rest of the function for LLVM;
  // just codegen the modules.
  if( llvmCodegen ) {
#ifdef HAVE_LLVM
    checkAdjustedDataLayout();
    forv_Vec(ModuleSymbol, currentModule, allModules) {
      currentModule->codegenDef();
    }

    finishCodegenLLVM();
#endif
  } else {
    ChainHashMap<char*, StringHashFns, int> fileNameHashMap;
    forv_Vec(ModuleSymbol, currentModule, allModules) {
      const char* filename = NULL;
      filename = generateFileName(fileNameHashMap, filename,currentModule->name);

      fileinfo modulefile;
      openCFile(&modulefile, filename, "c");
      info->cfile = modulefile.fptr;
      if(fIncrementalCompilation && (currentModule->modTag == MOD_USER))
        fprintf(modulefile.fptr, "#include \"chpl__header.h\"\n");
      currentModule->codegenDef();

      closeCFile(&modulefile);

      if(!(fIncrementalCompilation && (currentModule->modTag == MOD_USER)))
        fprintf(mainfile.fptr, "#include \"%s%s\"\n", filename, ".c");
    }

    if (fMultiLocaleInterop) {
      codegenMultiLocaleInteropWrappers();
    }

    fprintf(strconfig.fptr, "#include \"chpl-string.h\"\n");
    fprintf(strconfig.fptr, "chpl_string defaultStringValue=\"\";\n");

    info->cfile = hdrfile.fptr;
    codegen_header_addons();

    fprintf(hdrfile.fptr, "\n#endif");
    fprintf(hdrfile.fptr, " /* END CHPL_GEN_HEADER_INCLUDE_GUARD */\n"); 

    closeCFile(&hdrfile);
    fprintf(mainfile.fptr, "/* last line not #include to avoid gcc bug */\n");
    closeCFile(&mainfile);
    closeCFile(&defnfile);
    closeCFile(&strconfig);
  }

  if (fPrintEmittedCodeSize)
  {
    fprintf(stderr, "Statements emitted: %d\n", gStmtCount);
  }
}

void makeBinary(void) {
  if (no_codegen)
    return;

  if(llvmCodegen) {
#ifdef HAVE_LLVM
    makeBinaryLLVM();
#endif
  } else {
    const char* makeflags = printSystemCommands ? "-f " : "-s -f ";
    const char* command = astr(astr(CHPL_MAKE, " "),
                               makeflags,
                               getIntermediateDirName(), "/Makefile");
    mysystem(command, "compiling generated source");
  }

  if (fLibraryCompile && fLibraryPython) {
    codegen_make_python_module();
  }
}

GenInfo::GenInfo()
         :   cfile(NULL), cLocalDecls(), cStatements(),
             lineno(-1), filename(NULL)
#ifdef HAVE_LLVM
             ,
             lvt(NULL), module(NULL), irBuilder(NULL), mdBuilder(NULL),
             loopStack(), currentStackVariables(),
             llvmContext(),
             tbaaRootNode(NULL),
             tbaaUnionsNode(NULL),
             noAliasDomain(NULL),
             noAliasScopes(),
             noAliasScopeLists(),
             noAliasLists(),
             globalToWideInfo(),
             FPM_postgen(NULL),
             clangInfo(NULL)
#endif
{
}

std::string numToString(int64_t num)
{
  return int64_to_string(num);
}
std::string int64_to_string(int64_t i)
{
  char buf[32];
  snprintf(buf, sizeof(buf), "%" PRId64, i);
  std::string ret(buf);
  return ret;
}
std::string uint64_to_string(uint64_t i)
{
  char buf[32];
  snprintf(buf, sizeof(buf), "%" PRIu64, i);
  std::string ret(buf);
  return ret;
}
std::string real_to_string(double num)
{
  char buf[32];

  if (chpl_isfinite(num)) {
    if (chpl_signbit(num)) snprintf(buf, sizeof(buf), "-%a" , -num);
    else                   snprintf(buf, sizeof(buf), "%a" , num);
  } else if (chpl_isinf(num)) {
    if (chpl_signbit(num)) strncpy(buf, "-INFINITY", sizeof(buf));
    else                   strncpy(buf, "INFINITY", sizeof(buf));
  } else {
    if (chpl_signbit(num)) strncpy(buf, "-NAN", sizeof(buf));
    else                   strncpy(buf, "NAN", sizeof(buf));
  }
  std::string ret(buf);
  return ret;
}
void genComment(const char* comment, bool push) {
  GenInfo* info = gGenInfo;
  if( info->cfile ) {
    if (push) {
      std::string str = comment;
      info->cStatements.push_back("/*** "+str+" ***/ ");
    } else {
      fprintf(info->cfile, "/*** %s ***/\n\n", comment);
    }
  }
}

void flushStatements(void)
{
  GenInfo* info = gGenInfo;
  size_t i;
  if( info->cfile ) {
    for(i = 0; i < info->cLocalDecls.size(); ++i) {
      fprintf(info->cfile, "%s;\n", info->cLocalDecls[i].c_str());
    }
    for(i = 0; i < info->cStatements.size(); ++i) {
      fprintf(info->cfile, "%s", info->cStatements[i].c_str());
    }
    info->cLocalDecls.clear();
    info->cStatements.clear();
  }
}

void nprint_view(GenRet& gen) {
#ifdef HAVE_LLVM
  GenInfo* info = gGenInfo;
  llvm::Module* M = info->module;
#endif

  printf("GenRet {\n");
  if (!gen.c.empty())
    printf("c=%s\n", gen.c.c_str());
#ifdef HAVE_LLVM
  if (gen.val) {
    printf("val= ");
    fflush(stdout);
    gen.val->print(llvm::outs(), true);
    llvm::outs().flush();
    printf("\n");
  }
  if (gen.type) {
    printf("type= ");
    fflush(stdout);
    gen.type->print(llvm::outs(), true);
    llvm::outs().flush();
    printf("\n");
  }
  if (gen.surroundingStruct) {
    TypeSymbol* ts = gen.surroundingStruct->symbol;
    printf("surroundingStruct=%s (%i)\n", ts->name, ts->id);
  }
  if (gen.fieldOffset) {
    printf("fieldOffset=%i\n", (int) gen.fieldOffset);
  }
  if (gen.fieldTbaaTypeDescriptor) {
    printf("fieldTbaaTypeDescriptor= ");
    fflush(stdout);
    gen.fieldTbaaTypeDescriptor->print(llvm::outs(), M, true);
    llvm::outs().flush();
    printf("\n");
  }
  if (gen.aliasScope) {
    printf("aliasScope= ");
    fflush(stdout);
    gen.aliasScope->print(llvm::outs(), M, true);
    llvm::outs().flush();
    printf("\n");
  }
  if (gen.noalias) {
    printf("noalias= ");
    fflush(stdout);
    gen.noalias->print(llvm::outs(), M, true);
    llvm::outs().flush();
    printf("\n");
  }
#endif
  printf("canBeMarkedAsConstAfterStore=%i\n",
         (int) gen.canBeMarkedAsConstAfterStore);
  printf("alreadyStored %i\n", (int) gen.alreadyStored);
  if (gen.chplType) {
    TypeSymbol* ts = gen.chplType->symbol;
    printf("chplType=%s (%i)\n", ts->name, ts->id);
  }
  if (gen.isLVPtr == GEN_VAL) {
    printf("isLVPtr=GEN_VAL\n");
  }
  if (gen.isLVPtr == GEN_PTR) {
    printf("isLVPtr=GEN_PTR\n");
  }
  if (gen.isLVPtr == GEN_WIDE_PTR) {
    printf("isLVPtr=GEN_WIDE_PTR\n");
  }
  printf("isUnsigned %i\n", (int) gen.isUnsigned);
  printf("}\n");
}
