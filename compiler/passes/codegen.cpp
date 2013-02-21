#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include <cctype>
#include <cstring>
#include <cstdio>


#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "mysystem.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "config.h"
#include "codegen.h"


#ifdef HAVE_LLVM
// This one is not normally included by clang clients
// and not normally installed in the include directory.
#include "CodeGenModule.h"
#include "CGRecordLayout.h"
#endif


// Global so that we don't have to pass around
// to all of the codegen() routines
GenInfo* gGenInfo;
int gMaxVMT = -1;

void processRuntimeHeaders();
#ifdef HAVE_LLVM
void finishClangGeneration();
#endif

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
  if (sym->hasFlag(FLAG_EXTERN))
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
    case '~': ch = subChar(sym, ch, "_TILDA_"); break;
    case '.': ch = subChar(sym, ch, "_DOT_"); break;
    default: break;
    }
  }

  // Add chpl_ to operator names.
  if ((sym->cname[0] == '_' && (sym->cname[1] == '_' || (sym->cname[1] >= 'A' && sym->cname[1] <= 'Z')))) {
    sym->cname = astr("chpl__", sym->cname);
  }
}

static void
genGlobalDefClassId(const char* cname, int id) {
  GenInfo* info = gGenInfo;
  const char* id_type_name = "chpl__class_id";
  std::string name("chpl__cid_");
  name += cname;
  
  if( info->cfile ) {
    fprintf(info->cfile, "const %s %s = %d;\n", 
                      id_type_name, name.c_str(), id);
  } else {
#ifdef HAVE_LLVM
    GenRet id_type_g = CLASS_ID_TYPE->codegen();
    llvm::Type *id_type = id_type_g.type;
    llvm::GlobalVariable * gv = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal(name, id_type));
    gv->setInitializer(info->builder->getInt32(id));
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
    llvm::GlobalVariable *globalString = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal(cname, llvm::IntegerType::getInt8PtrTy(info->module->getContext())));
    globalString->setInitializer(llvm::cast<llvm::GlobalVariable>(new_StringSymbol(value)->codegen().val)->getInitializer());
    globalString->setConstant(true);
    info->lvt->addGlobalValue(cname, globalString, GEN_PTR, true);
#endif
  }
}
static void
genGlobalInt(const char* cname, int value) {
  GenInfo* info = gGenInfo;
  if( info->cfile ) {
    fprintf(info->cfile, "const int %s = %d;\n", cname, value);
  } else {
#ifdef HAVE_LLVM
    llvm::GlobalVariable *globalInt = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal(cname, llvm::IntegerType::getInt32Ty(info->module->getContext())));
    globalInt->setInitializer(info->builder->getInt32(value));
    globalInt->setConstant(true);
    info->lvt->addGlobalValue(cname, globalInt, GEN_PTR, false);
#endif
  }
}
static void
genClassIDs(Vec<TypeSymbol*> & typeSymbols) {
  genComment("Class Type Identification Numbers");

  int count=0;
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ClassType* ct = toClassType(ts->type)) {
      if (!isReferenceType(ct) && isClass(ct)) {
        genGlobalDefClassId(ts->cname, count);
        count++;
      }
    }
  }
}
static void
genFtable(Vec<FnSymbol*> & fSymbols) {
  GenInfo* info = gGenInfo;
  const char* ftable_name = "chpl_ftable";
  if( info->cfile ) {
    FILE* hdrfile = info->cfile;
    fprintf(hdrfile, "chpl_fn_p %s[] = {\n", ftable_name);
    bool first = true;
    forv_Vec(FnSymbol, fn, fSymbols) {
      if (!first)
        fprintf(hdrfile, ",\n");
      fprintf(hdrfile, "(chpl_fn_p)%s", fn->cname);
      first = false;
    }

    if (fSymbols.n == 0)
      fprintf(hdrfile, "(chpl_fn_p)0");
    fprintf(hdrfile, "\n};\n");
  } else {
#ifdef HAVE_LLVM
    std::vector<llvm::Constant *> table ((fSymbols.n == 0) ? 1 : fSymbols.n);
    
    llvm::Type *funcPtrType = info->lvt->getType("chpl_fn_p");

    int fID = 0;
    forv_Vec(FnSymbol, fn, fSymbols) {
      llvm::Function *func = getFunctionLLVM(fn->cname);
      table[fID++] = llvm::cast<llvm::Constant>(info->builder->CreatePointerCast(func, funcPtrType));
    }
    if (fSymbols.n == 0) {
      table[0] = llvm::Constant::getNullValue(funcPtrType);
    }
    
    llvm::ArrayType *funcPtrTableType = llvm::ArrayType::get(funcPtrType, table.size());
    
    if(llvm::GlobalVariable *ftable = info->module->getNamedGlobal(ftable_name)) {
      ftable->eraseFromParent();
    }
    
    llvm::GlobalVariable *ftable = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal(ftable_name, funcPtrTableType));
    ftable->setInitializer(llvm::ConstantArray::get(funcPtrTableType, table));
    ftable->setConstant(true);
    info->lvt->addGlobalValue(ftable_name, ftable, GEN_PTR, true);
#endif
  }
}

static void
genVirtualMethodTable(Vec<TypeSymbol*>& types) {
  GenInfo* info = gGenInfo;
  int maxVMT = 0;
  for (int i = 0; i < virtualMethodTable.n; i++)
    if (virtualMethodTable.v[i].key && virtualMethodTable.v[i].value->n > maxVMT)
      maxVMT = virtualMethodTable.v[i].value->n;

  gMaxVMT = maxVMT;

  const char* vmt = "chpl_vmtable";
  if( info->cfile ) {
    FILE* hdrfile = info->cfile;
    // MPF - in order to simplify code generation, making
    // chpl_vmtable a 1D array.
    fprintf(hdrfile, "chpl_fn_p %s[] = {\n", vmt);
    bool comma = false;
    forv_Vec(TypeSymbol, ts, types) {
      if (ClassType* ct = toClassType(ts->type)) {
        if (!isReferenceType(ct) && isClass(ct)) {
          if (comma)
            fprintf(hdrfile, ",\n");
          fprintf(hdrfile, " /* %s */\n", ct->symbol->cname);
          int n = 0;
          if (Vec<FnSymbol*>* vfns = virtualMethodTable.get(ct)) {
            forv_Vec(FnSymbol, vfn, *vfns) {
              if (n > 0)
                fprintf(hdrfile, ",\n");
              fprintf(hdrfile, "(chpl_fn_p)%s", vfn->cname);
              n++;
            }
          }
          for (int i = n; i < maxVMT; i++) {
            if (n > 0)
              fprintf(hdrfile, ",\n");
            fprintf(hdrfile, "(chpl_fn_p)NULL");
            n++;
          }
          comma = true;
        }
      }
    }
    fprintf(hdrfile, "\n};\n");
  } else {
#ifdef HAVE_LLVM
    const char* vmtData = "chpl_vmtable_data";
    std::vector<llvm::Constant *> table;
    llvm::Type *funcPtrType = getTypeLLVM("chpl_fn_p");
    llvm::Type *vmTableEntryType = funcPtrType;
    
    forv_Vec(TypeSymbol, ts, types) {
      if (ClassType* ct = toClassType(ts->type)) {
        if (!isReferenceType(ct) && isClass(ct)) {
          int n = 0;
          if (Vec<FnSymbol*>* vfns = virtualMethodTable.get(ct)) {
            forv_Vec(FnSymbol, vfn, *vfns) {
              llvm::Function *func = getFunctionLLVM(vfn->cname);
              table.push_back(llvm::cast<llvm::Constant>(info->builder->CreatePointerCast(func, funcPtrType)));
              n++;
            }
          }
          for (int i = n; i < maxVMT; i++) {
            table.push_back(llvm::Constant::getNullValue(funcPtrType));
            n++; 
          }
        }
      }
    }
    
    llvm::ArrayType *vmTableType = llvm::ArrayType::get(vmTableEntryType, table.size());
    
    if(llvm::GlobalVariable *vmTable = info->module->getNamedGlobal(vmtData)) {
      vmTable->eraseFromParent();
    }
    
    llvm::GlobalVariable *vmTable = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal(vmtData, vmTableType));
    vmTable->setInitializer(llvm::ConstantArray::get(vmTableType, table));
    vmTable->setConstant(true);
    
    llvm::Value* vmtElmPtr = info->builder->CreateConstInBoundsGEP2_64(vmTable, 0, 0);
     
    /*llvm::GlobalVariable *vmtElmGlobal = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal(vmt, vmtElmPtr->getType()));
    vmTable->setInitializer(vmtElmPtr);
    vmTable->setConstant(true);
    */
    info->lvt->addGlobalValue(vmt, vmtElmPtr, GEN_VAL, true);
#endif
  }
}

static int
compareSymbol(const void* v1, const void* v2) {
  Symbol* s1 = *(Symbol**)v1;
  Symbol* s2 = *(Symbol**)v2;
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
//
// the unique numbering is based on the map uniquifyNameCounts which
// can be cleared to reset
//
int fMaxCIdentLen = 0;
static const int maxUniquifyAddedChars = 25;
static char* longCNameReplacementBuffer = NULL;
static Map<const char*, int> uniquifyNameCounts;
static const char* uniquifyName(const char* name,
                                Vec<const char*>* set1,
                                Vec<const char*>* set2 = NULL) {
  const char* newName = name;
  if (fMaxCIdentLen > 0 && (int)(strlen(newName)) > fMaxCIdentLen) {
    // how much of the name to preserve
    int prefixLen = fMaxCIdentLen - maxUniquifyAddedChars;
    if (!longCNameReplacementBuffer) {
      longCNameReplacementBuffer = (char*)malloc(prefixLen+1);
      longCNameReplacementBuffer[prefixLen] = '\0';
    }
    strncpy(longCNameReplacementBuffer, newName, prefixLen);
    INT_ASSERT(longCNameReplacementBuffer[prefixLen] == '\0');
    name = newName = astr(longCNameReplacementBuffer);
  }
  while (set1->set_in(newName) || (set2 && set2->set_in(newName))) {
    char numberTmp[64];
    int count = uniquifyNameCounts.get(name);
    uniquifyNameCounts.put(name, count+1);
    snprintf(numberTmp, 64, "%d", count+2);
    newName = astr(name, numberTmp);
  }
  set1->set_add(newName);
  return newName;
}

static inline bool shouldCodegenAggregate(ClassType* ct)
{
  // never codegen definitions of primitive or arithmetic types.
  if( toPrimitiveType(ct) ) return false;

  // Needed special handling for complex types, since after complex2record
  // they appear like normal records but we must not define them
  // since they are defined in the runtime headers
  // Added a flag, FLAG_NO_CODEGEN, to handle this case.
  // This flag could be used for other similar cases if necessary.
  if( ct->symbol->hasFlag(FLAG_NO_CODEGEN) ) return false;

  // Don't visit classes since they are prototyped individually all at once..
  // ..except for classes with FLAG_REF or FLAG_DATA_CLASS.. which
  //   we do visit.
  if( isClass(ct) ) { // is it actually a class?
    if( ct->symbol->hasFlag(FLAG_REF) ||
        ct->symbol->hasFlag(FLAG_WIDE) ||
        ct->symbol->hasFlag(FLAG_DATA_CLASS)) return true;
    else return false;
  }

  // otherwise, visit record/union
  return true;
}


static void codegen_aggregate_def(ClassType* ct) {
  if (!shouldCodegenAggregate(ct)) return;
  if (ct->symbol->codegenned) return;
  ct->symbol->codegenned = true;

  // For reference or data class types, first generate
  // the referenced type
  Type* vt = NULL;
  if(ct->symbol->hasFlag(FLAG_REF))
    vt = ct->symbol->getValType();
  else if(ct->symbol->hasFlag(FLAG_DATA_CLASS))
    vt = getDataClassType(ct->symbol)->typeInfo();
  if (vt) {
    if (ClassType* fct = toClassType(vt)) {
      codegen_aggregate_def(fct);
    }
  }
  // For other types, generate the field types
  for_fields(field, ct) {
    if (ClassType* fct = toClassType(field->type)) {
      codegen_aggregate_def(fct);
    }
  }
  // Lastly, generate the type we're working on.
  // Codegen what we have here.
  ct->symbol->codegenDef();
}


// TODO: Split this into a number of smaller routines.<hilde>
static void codegen_header() {
  GenInfo* info = gGenInfo;
  Vec<const char*> cnames;
  Vec<TypeSymbol*> types;
  Vec<FnSymbol*> functions;
  Vec<VarSymbol*> globals;

  // reserved symbol names that require renaming to compile
#include "reservedSymbolNames.h"

  //
  // collect types and apply canonical sort
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      legalizeName(ts);
      types.add(ts);
    }
  }
  qsort(types.v, types.n, sizeof(types.v[0]), compareSymbol);

  //
  // collect globals and apply canonical sort
  //
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->defPoint->parentExpr != rootModule->block &&
        toModuleSymbol(var->defPoint->parentSymbol)) {
      legalizeName(var);
      globals.add(var);
    }
  }
  qsort(globals.v, globals.n, sizeof(globals.v[0]), compareSymbol);

  //
  // collect functions and apply canonical sort
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    legalizeName(fn);
    functions.add(fn);
  }
  qsort(functions.v, functions.n, sizeof(functions.v[0]), compareSymbol);


  //
  // mangle type names if they clash with other types
  //
  forv_Vec(TypeSymbol, ts, types) {
    if (!ts->hasFlag(FLAG_EXTERN))
      ts->cname = uniquifyName(ts->cname, &cnames);
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
        sym->cname = uniquifyName(sym->cname, &cnames);
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
      if (ClassType* ct = toClassType(ts->type)) {
        Vec<const char*> fieldNameSet;
        for_fields(field, ct) {
          legalizeName(field);
          field->cname = uniquifyName(field->cname, &fieldNameSet);
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
    if (!var->hasFlag(FLAG_EXTERN))
      var->cname = uniquifyName(var->cname, &cnames);
  }
  uniquifyNameCounts.clear();

  //
  // mangle function names if they clash with types, enum constants,
  // global variables, or other functions
  //
  forv_Vec(FnSymbol, fn, functions) {
    if (!fn->hasFlag(FLAG_USER_NAMED))
      fn->cname = uniquifyName(fn->cname, &cnames);
  }
  uniquifyNameCounts.clear();

  //
  // mangle formal argument names if they clash with types, enum
  // constants, global variables, functions, or earlier formal
  // arguments in the same function
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Vec<const char*> formalNameSet;
    for_formals(formal, fn) {
      legalizeName(formal);
      formal->cname = uniquifyName(formal->cname, &formalNameSet, &cnames);
    }
    uniquifyNameCounts.clear();
  }

  //
  // mangle local variable names if they clash with types, global
  // variables, functions, formal arguments of their function, or
  // other local variables in the same function
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Vec<const char*> local;

    for_formals(formal, fn) {
      local.set_add(formal->cname);
    }

    Vec<DefExpr*> defs;
    collectDefExprs(fn->body, defs);
    forv_Vec(DefExpr, def, defs) {
      legalizeName(def->sym);
      // give temps cnames
      if (def->sym->hasFlag(FLAG_TEMP)) {
        if (localTempNames) {
          // temp name is _tNNN_
          if (!strncmp(def->sym->cname, "_t", 2))
            def->sym->cname = astr("T", def->sym->cname + 2);
        } else {
          // temp name is _tmp
          if (!strcmp(def->sym->cname, "_tmp"))
            def->sym->cname = astr("T");
        }
      }
      def->sym->cname = uniquifyName(def->sym->cname, &local, &cnames);
    }
    uniquifyNameCounts.clear();
  }

  FILE* hdrfile = info->cfile;
  genComment("Compilation Info");

  if (fGPU) {
    INT_ASSERT(hdrfile);
    fprintf(hdrfile, "#ifndef ENABLE_GPU\n");
  }

  genGlobalString("chpl_compileCommand", compileCommand);
  genGlobalString("chpl_compileVersion", compileVersion);
  genGlobalString("CHPL_HOST_PLATFORM", CHPL_HOST_PLATFORM);
  genGlobalString("CHPL_TARGET_PLATFORM", CHPL_TARGET_PLATFORM);
  genGlobalString("CHPL_HOST_COMPILER", CHPL_HOST_COMPILER);
  genGlobalString("CHPL_TARGET_COMPILER", CHPL_TARGET_COMPILER);
  genGlobalString("CHPL_TASKS", CHPL_TASKS);
  genGlobalString("CHPL_THREADS", CHPL_THREADS);
  genGlobalString("CHPL_COMM", CHPL_COMM);

  if (fGPU) {
    INT_ASSERT(hdrfile);
    fprintf(hdrfile, "#else\n");
    fprintf(hdrfile, "extern const char* chpl_compileCommand;\n");
    fprintf(hdrfile, "extern const char* chpl_compileVersion;\n");
    fprintf(hdrfile, "extern const char* CHPL_HOST_PLATFORM;\n");
    fprintf(hdrfile, "extern const char* CHPL_TARGET_PLATFORM;\n");
    fprintf(hdrfile, "extern const char* CHPL_HOST_COMPILER;\n");
    fprintf(hdrfile, "extern const char* CHPL_TARGET_COMPILER;\n");
    fprintf(hdrfile, "extern const char* CHPL_TASKS;\n");
    fprintf(hdrfile, "extern const char* CHPL_COMM;\n");
    fprintf(hdrfile, "#endif\n");
  }

  if( hdrfile ) {
    // This is done in readCTypes for LLVM version.
    fprintf(hdrfile, "\n#define CHPL_GEN_CODE\n\n");

    // Include sys_basic.h to get C types always defined, proper library .h inclusion
    fprintf(hdrfile, "#include \"sys_basic.h\"\n");

    genIncludeCommandLineHeaders(hdrfile);

    fprintf(hdrfile, "#include \"stdchpl.h\"\n");
  }

  genClassIDs(types);

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

  {
    // codegen records/unions/references/data class in topological order
    genComment("Records, Unions, Data Class, References (Hierarchically)");
    forv_Vec(TypeSymbol, ts, types) {
      if (ClassType* ct = toClassType(ts->type))
        codegen_aggregate_def(ct);
    }
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
      forv_Vec(Type, child, ts->type->dispatchChildren) {
        if (child)
          next.set_add(child->symbol);
      }
    }
    current.clear();
    current.move(next);
    current.set_to_vec();
    qsort(current.v, current.n, sizeof(current.v[0]), compareSymbol);
    next.clear();
  }

  genComment("Function Prototypes");
  forv_Vec(FnSymbol, fnSymbol, functions) {
    if (fnSymbol->hasFlag(FLAG_GPU_ON)) {
      INT_ASSERT(hdrfile);
      fprintf(hdrfile, "\n#ifdef ENABLE_GPU\n");
      fnSymbol->codegenPrototype();
      fprintf(hdrfile, "#endif\n");
      continue;
    }
    fnSymbol->codegenPrototype();
  }
    
  if (fGPU) {
    INT_ASSERT(hdrfile);
    fprintf(hdrfile, "\n#ifndef ENABLE_GPU\n");
  }

  genComment("Function Pointer Table");
  forv_Vec(FnSymbol, fn, functions) {
    if (fn->hasFlag(FLAG_BEGIN_BLOCK) ||
        fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK) ||
        fn->hasFlag(FLAG_ON_BLOCK)) {
    ftableVec.add(fn);
    ftableMap.put(fn, ftableVec.n-1);
    }
  }

  genFtable(ftableVec);

  if (fGPU) {
    INT_ASSERT(hdrfile);
    fprintf(hdrfile, "#else\n");
    fprintf(hdrfile, "extern chpl_fn_p chpl_ftable[];\n");
    fprintf(hdrfile, "#endif\n");
  }

  genComment("Virtual Method Table");
  genVirtualMethodTable(types);

  if (fGPU) {
    INT_ASSERT(hdrfile);
    fprintf(hdrfile, "\n#ifndef ENABLE_GPU\n");
  }

  genComment("Global Variables");
  forv_Vec(VarSymbol, varSymbol, globals) {
    varSymbol->codegenGlobalDef();
  }
  flushStatements();

  genGlobalInt("chpl_numGlobalsOnHeap", numGlobalsOnHeap);
  int globals_registry_static_size = (numGlobalsOnHeap ? numGlobalsOnHeap : 1);
  if( hdrfile ) {
    fprintf(hdrfile, "\nvoid** chpl_globals_registry;\n");
    fprintf(hdrfile, "\nvoid* chpl_globals_registry_static[%d];\n", globals_registry_static_size);
  } else {
#ifdef HAVE_LLVM
    llvm::Type* voidstarstar =
      llvm::PointerType::get(llvm::IntegerType::getInt8PtrTy(info->module->getContext()), 0);

    llvm::GlobalVariable *chpl_globals_registryGVar = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal("chpl_globals_registry", voidstarstar));
    chpl_globals_registryGVar->setInitializer(llvm::Constant::getNullValue(chpl_globals_registryGVar->getType()->getContainedType(0)));
    info->lvt->addGlobalValue("chpl_globals_registry", chpl_globals_registryGVar, GEN_PTR, true);

    if(llvm::GlobalVariable *GVar = llvm::cast<llvm::GlobalVariable>(info->module->getNamedGlobal("chpl_globals_registry_static"))) {
      GVar->eraseFromParent();
    }
    llvm::GlobalVariable *chpl_globals_registry_staticGVar = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal("chpl_globals_registry_static", llvm::ArrayType::get(llvm::IntegerType::getInt8PtrTy(info->module->getContext()), globals_registry_static_size)));
    chpl_globals_registry_staticGVar->setInitializer(llvm::Constant::getNullValue(chpl_globals_registry_staticGVar->getType()->getContainedType(0)));
    info->lvt->addGlobalValue("chpl_globals_registry_static", chpl_globals_registry_staticGVar, GEN_PTR, true);
#endif
  }
  genGlobalInt("chpl_heterogeneous", fHeterogeneous?1:0);
  if( hdrfile ) {
    fprintf(hdrfile, "\nconst char* chpl_mem_descs[] = {\n");
    bool first = true;
    forv_Vec(const char*, memDesc, memDescsVec) {
      if (!first)
        fprintf(hdrfile, ",\n");
      fprintf(hdrfile, "\"%s\"", memDesc);
      first = false;
    }
    fprintf(hdrfile, "\n};\n");
  } else {
#ifdef HAVE_LLVM
    std::vector<llvm::Constant *> memDescTable;
    forv_Vec(const char*, memDesc, memDescsVec) {
      memDescTable.push_back(llvm::cast<llvm::GlobalVariable>(new_StringSymbol(memDesc)->codegen().val)->getInitializer());
    }
    llvm::ArrayType *memDescTableType = llvm::ArrayType::get(llvm::IntegerType::getInt8PtrTy(info->module->getContext()), memDescTable.size());

    if(llvm::GlobalVariable *GVar = llvm::cast<llvm::GlobalVariable>(info->module->getNamedGlobal("chpl_mem_descs"))) {
      GVar->eraseFromParent();
    }

    llvm::GlobalVariable *chpl_memDescsGVar = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal("chpl_mem_descs", memDescTableType));
    chpl_memDescsGVar->setInitializer(llvm::ConstantArray::get(memDescTableType, memDescTable));
    chpl_memDescsGVar->setConstant(true);
    info->lvt->addGlobalValue("chpl_mem_descs", chpl_memDescsGVar, GEN_PTR, true);
#endif
  }

  genGlobalInt("chpl_mem_numDescs", memDescsVec.n);

  genGlobalInt("chpl_no_stdmodules", fNoInternalModules);

  //
  // add table of private-broadcast constants
  //
  if( hdrfile ) {
    fprintf(hdrfile, "\nvoid* const chpl_private_broadcast_table[] = {\n");
    fprintf(hdrfile, "&chpl_verbose_comm");
    fprintf(hdrfile, ",\n&chpl_comm_diagnostics");
    fprintf(hdrfile, ",\n&chpl_verbose_mem");
    int i = 3;
    forv_Vec(CallExpr, call, gCallExprs) {
      if (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) {
        SymExpr* se = toSymExpr(call->get(1));
        INT_ASSERT(se);
        SET_LINENO(call);
        fprintf(hdrfile, ",\n&%s", se->var->cname);
        call->insertAtHead(new_IntSymbol(i));
        i++;
      }
    }
    fprintf(hdrfile, "\n};\n");
  } else {
#ifdef HAVE_LLVM
    llvm::Type *private_broadcastTableEntryType = llvm::IntegerType::getInt8PtrTy(info->module->getContext());
    std::vector<llvm::Constant *> private_broadcastTable;
    private_broadcastTable.push_back(llvm::cast<llvm::Constant>(info->builder->CreatePointerCast(info->lvt->getValue("chpl_verbose_comm").val, private_broadcastTableEntryType)));
    private_broadcastTable.push_back(llvm::cast<llvm::Constant>(info->builder->CreatePointerCast(info->lvt->getValue("chpl_comm_diagnostics").val, private_broadcastTableEntryType)));
    private_broadcastTable.push_back(llvm::cast<llvm::Constant>(info->builder->CreatePointerCast(info->lvt->getValue("chpl_verbose_mem").val, private_broadcastTableEntryType)));

    int broadcastID = 3;
    forv_Vec(CallExpr, call, gCallExprs) {
      if (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) {
        SymExpr* se = toSymExpr(call->get(1));
        INT_ASSERT(se);

        private_broadcastTable.push_back(llvm::cast<llvm::Constant>(info->builder->CreatePointerCast(info->lvt->getValue(se->var->cname).val, private_broadcastTableEntryType)));

        call->insertAtHead(new_IntSymbol(broadcastID++));
      }
    }

    if(llvm::GlobalVariable *GVar = llvm::cast<llvm::GlobalVariable>(info->module->getNamedGlobal("chpl_private_broadcast_table"))) {
      GVar->eraseFromParent();
    }

    llvm::ArrayType *private_broadcastTableType = llvm::ArrayType::get(private_broadcastTableEntryType, private_broadcastTable.size());
    llvm::GlobalVariable *private_broadcastTableGVar = llvm::cast<llvm::GlobalVariable>(info->module->getOrInsertGlobal("chpl_private_broadcast_table", private_broadcastTableType));
    private_broadcastTableGVar->setInitializer(llvm::ConstantArray::get(private_broadcastTableType, private_broadcastTable));
    info->lvt->addGlobalValue("chpl_private_broadcast_table", private_broadcastTableGVar, GEN_PTR, true);
#endif
  }

  if (fGPU) {
    INT_ASSERT(hdrfile);
    fprintf(hdrfile, "#else\n");
    forv_Vec(VarSymbol, varSymbol, globals) {
      fprintf(hdrfile,"extern ");
      varSymbol->codegenDef();
    }

    fprintf(hdrfile, "\nextern const int chpl_numGlobalsOnHeap;\n");
    fprintf(hdrfile, "\nextern void** chpl_globals_registry;\n");
    fprintf(hdrfile, "\nextern void* chpl_globals_registry_static[];\n");
    fprintf(hdrfile, "\nconst int chpl_heterogeneous = ");
    if (fHeterogeneous)
      fprintf(hdrfile, " 1;\n");
    else
      fprintf(hdrfile, " 0;\n");
    fprintf(hdrfile, "\nextern const char* chpl_mem_descs[];\n");
    fprintf(hdrfile, "\nextern const int chpl_mem_numDescs;\n");
    fprintf(hdrfile, "#endif\n");
  }
}

// Sometimes we have to define a type while code generating.
// When that happens, we need to add a little bit to the header...
// This is only needed for C (since in LLVM we must add
//  the types as we use them).
static void codegen_header_addons() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      if (ClassType* ct = toClassType(ts->type))
        codegen_aggregate_def(ct);
    }
  }
}

static void
codegen_config() {
  GenInfo* info = gGenInfo;
  if( info->cfile ) {
    FILE* mainfile = info->cfile;
    FILE* outfile = info->cfile;
    fprintf(outfile, "#include \"_config.c\"\n");
    fileinfo configFile;
    openCFile(&configFile, "_config.c");
    outfile = configFile.fptr;
    info->cfile = outfile;

    fprintf(outfile, "#include \"error.h\"\n\n");

    genGlobalInt("mainHasArgs", mainHasArgs);

    fprintf(outfile, "void CreateConfigVarTable(void) {\n");
    fprintf(outfile, "initConfigVarTable();\n");

    forv_Vec(VarSymbol, var, gVarSymbols) {
      if (var->hasFlag(FLAG_CONFIG) && !var->hasFlag(FLAG_TYPE_VARIABLE)) {
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
          fprintf(outfile, "\", \"Built-in\");\n");
        } else {
          fprintf(outfile, "\", \"%s\");\n", var->getModule()->name);
        }
      }
    }

    fprintf(outfile, "}\n\n\n");

    closeCFile(&configFile);
    info->cfile = mainfile;
  } else {
#ifdef HAVE_LLVM
    llvm::FunctionType *createConfigType;
    llvm::Function *createConfigFunc;
    genGlobalInt("mainHasArgs", mainHasArgs);
    if((createConfigFunc = getFunctionLLVM("CreateConfigVarTable"))) {
      createConfigType = createConfigFunc->getFunctionType();
    }
    else {
      createConfigType = llvm::FunctionType::get(llvm::Type::getVoidTy(info->module->getContext()), false);
      createConfigFunc = llvm::Function::Create(createConfigType, llvm::Function::ExternalLinkage, "CreateConfigVarTable", info->module);
    }

    llvm::BasicBlock *createConfigBlock = llvm::BasicBlock::Create(info->module->getContext(), "entry", createConfigFunc);
    info->builder->SetInsertPoint(createConfigBlock);

    llvm::Function *initConfigFunc = getFunctionLLVM("initConfigVarTable");
    info->builder->CreateCall(initConfigFunc);

    llvm::Function *installConfigFunc = getFunctionLLVM("installConfigVar");

    forv_Vec(VarSymbol, var, gVarSymbols) {
      if (var->hasFlag(FLAG_CONFIG) && !var->hasFlag(FLAG_TYPE_VARIABLE)) {
        std::vector<llvm::Value *> args (3);
        args[0] = info->builder->CreateLoad(new_StringSymbol(var->name)->codegen().val);

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
        args[1] = info->builder->CreateLoad(new_StringSymbol(type->symbol->name)->codegen().val);

        if (var->getModule()->modTag == MOD_INTERNAL) {
          args[2] = info->builder->CreateLoad(new_StringSymbol("Built-in")->codegen().val);
        }
        else {
          args[2] = info->builder->CreateLoad(new_StringSymbol(var->getModule()->name)->codegen().val);
        }

        info->builder->CreateCall(installConfigFunc, args);
      }
    }
    info->builder->CreateRetVoid();
    llvm::verifyFunction(*createConfigFunc);
#endif
  }
}


void codegen(void) {
  if (no_codegen)
    return;

  fileinfo hdrfile, mainfile, gpusrcfile;
  GenInfo *info = gGenInfo;
  INT_ASSERT(info);
  if( llvmCodegen ) {
#ifndef HAVE_LLVM
    INT_FATAL("This compiler was built without LLVM support");
#endif
    fileinfo nullfile = {NULL, NULL, NULL};
    hdrfile = mainfile = gpusrcfile = nullfile; 
    if( fHeterogeneous ) INT_FATAL("fHeretogeneous not yet supported with LLVM");
    if (fGPU) INT_FATAL("fGPU not yet supported with LLVM"); 
  } else {
    openCFile(&hdrfile, "chpl__header", "h");
    openCFile(&mainfile, "_main", "c");
    fprintf(mainfile.fptr, "#include \"chpl__header.h\"\n");

    if (fGPU) {
      openCFile(&gpusrcfile, "chplGPU", "cu");
      forv_Vec(FnSymbol, fn, gFnSymbols) {
        if (fn->hasFlag(FLAG_GPU_ON)) {
          fprintf(gpusrcfile.fptr, "extern \"C\" \x7b\n");
          fprintf(gpusrcfile.fptr, "#include \"chpl__header.h\"\n");
          fprintf(gpusrcfile.fptr,"\x7d\n"); // acsii for the "{" character
          break;
        }
      }
      codegen_makefile(&mainfile, &gpusrcfile);
      closeCFile(&gpusrcfile);
    }
    else
      codegen_makefile(&mainfile);
  }

  // This dumps the generated sources into the build directory.
  info->cfile = hdrfile.fptr;
  codegen_header();

  info->cfile = mainfile.fptr;
  codegen_config();

  // Don't need to do most of the rest of the function for LLVM;
  // just codegen the modules.
  if( llvmCodegen ) {
#ifdef HAVE_LLVM
    forv_Vec(ModuleSymbol, currentModule, allModules) {
      mysystem(astr("# codegen-ing module", currentModule->name),
               "generating comment for --print-commands option");
      currentModule->codegenDef();
    }

    finishClangGeneration();

    // Now Optimize
    /*
    llvm::FunctionPassManager FPM (info->module);
    FPM.add(llvm::createBasicAliasAnalysisPass());
    FPM.add(llvm::createInstructionCombiningPass());
    FPM.add(llvm::createReassociatePass());
    FPM.add(llvm::createGVNPass());
    FPM.add(llvm::createCFGSimplificationPass());
    FPM.doInitialization();
    FIM.run(Function);
    */
#endif 
    return;
  }

  if (fHeterogeneous) {
    codegenTypeStructureInclude(mainfile.fptr);
    forv_Vec(TypeSymbol, ts, gTypeSymbols) {
      if ((ts->type != dtOpaque) &&
          (!toPrimitiveType(ts->type) ||
           (toPrimitiveType(ts->type) &&
            !toPrimitiveType(ts->type)->isInternalType))) {
        registerTypeToStructurallyCodegen(ts);
      }
    }
  }

  ChainHashMap<char*, StringHashFns, int> filenames;
  forv_Vec(ModuleSymbol, currentModule, allModules) {
    mysystem(astr("# codegen-ing module", currentModule->name),
             "generating comment for --print-commands option");

    // Macs are case-insensitive when it comes to files, so
    // the following bit of code creates a unique filename
    // with case-insensitivity taken into account

    // create the lowercase filename
    char lowerFilename[FILENAME_MAX];
    sprintf(lowerFilename, "%s", currentModule->name);
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
      sprintf(filename, "%s%d", lowerFilename, version);
    }
    filenames.put(filename, 1);

    // build the real filename using that version number -- preserves
    // case by default by going back to currentModule->name rather
    // than using the lowercase filename
    if (version == 1) {
      sprintf(filename, "%s", currentModule->name);
    } else {
      sprintf(filename, "%s%d", currentModule->name, version);
    }
    
    fileinfo modulefile;
    openCFile(&modulefile, filename, "c");
    info->cfile = modulefile.fptr;
    
    currentModule->codegenDef();
    closeCFile(&modulefile);
    fprintf(mainfile.fptr, "#include \"%s%s\"\n", filename, ".c");
  }

  if (fHeterogeneous) 
    codegenTypeStructures(hdrfile.fptr);

  info->cfile = hdrfile.fptr;
  codegen_header_addons();

  closeCFile(&hdrfile);
  closeCFile(&mainfile);
}

void makeBinary(void) {
  if (no_codegen)
    return;

  if(llvmCodegen) {
#ifdef HAVE_LLVM
    GenInfo* info = gGenInfo;

    std::string moduleFilename = genIntermediateFilename("chpl__module.bc");

    llvm::Module* llvmModule = info->module;
    // already done in Clang integration.
    //llvmModule->setTargetTriple(llvm::sys::getHostTriple());

    std::string errorInfo;
    llvm::OwningPtr<llvm::tool_output_file> output (new llvm::tool_output_file(moduleFilename.c_str(), errorInfo, llvm::raw_fd_ostream::F_Binary));
    llvm::WriteBitcodeToFile(llvmModule, output->os());
    output->keep();
    output->os().flush();

    std::string options = "";

    std::string home(CHPL_HOME);
    std::string compileline = info->compileline;
    compileline += " --llvm-install-dir --main.o --link";
    std::vector<std::string> args;
    readArgsFromCommand(compileline.c_str(), args);

    std::string clangInstall = args[0];
    std::string maino = args[1];
    std::vector<std::string> dotOFiles;

    // Gather C flags for compiling C files.
    std::string cargs;
    for( size_t i = 0; i < info->clangCCArgs.size(); ++i ) {
      cargs += " ";
      cargs += info->clangCCArgs[i];
    }

    // Compile any C files.
    int filenum = 0;
    while (const char* inputFilename = nthFilename(filenum++)) {
      if (isCSource(inputFilename)) {
        const char* objFilename = objectFileForCFile(inputFilename);
        mysystem(astr(clangInstall.c_str(),
                      "/bin/clang -c -o ", objFilename,
                      " ", inputFilename, cargs.c_str()),
                 "Compile C File");
        dotOFiles.push_back(objFilename);
      } else if( isObjFile(inputFilename) ) {
        dotOFiles.push_back(inputFilename);
      }
    }


    if(fFastFlag) {
      std::string command = clangInstall + "/bin/opt" +
                            " -O3 -o " + moduleFilename + ".opt.bc "
                            + moduleFilename;
      mysystem(command.c_str(), "Make Binary - Optimizations");
      moduleFilename += ".opt.bc";

      options += "-O3 "; // would use -O4 -Xlinker -O4 for Gold LTO
    }

    if(debugCCode) {
      options += " -g";
    } else {
    }


    // Run linker...
    std::string command = clangInstall + "/bin/clang " + options + " " +
                          moduleFilename + " " + maino + " -o " + executableFilename;
    for( size_t i = 0; i < dotOFiles.size(); i++ ) {
      command += " ";
      command += dotOFiles[i];
    }

    // start at 3 since args[2] = 'gcc';
    for(size_t i = 3; i < args.size(); ++i) {
      command += " ";
      command += args[i];
    }

    mysystem(command.c_str(), "Make Binary - Linking");
#endif
  } else {
    const char* makeflags = printSystemCommands ? "-f " : "-s -f ";
    const char* command = astr(astr(CHPL_MAKE, " "), makeflags, getIntermediateDirName(),
                              "/Makefile");
    mysystem(command, "compiling generated source");
  }
}

#ifdef HAVE_LLVM

static
GenRet handleMacro(clang::IdentifierInfo* id, clang::MacroInfo* macro)
{
  GenInfo* info = gGenInfo;
  clang::Preprocessor &preproc = info->Clang->getPreprocessor();
  GenRet ret;

  const bool debugPrint = false;

  if( debugPrint) printf("Adding macro %s\n", id->getName().str().c_str());

  //Handling only simple string or integer defines
  if(macro->getNumArgs() > 0) {
    return ret; // TODO -- handle macro functions.
  }

  // Check that we have a single token surrounded by any
  // number of parens. ie 1, (1), ((1))
  clang::Token tok; // the main token.
  size_t left_parens = 0;
  size_t right_parens = 0;
  ssize_t ntokens = macro->getNumTokens();
  ssize_t t_idx;
  if( ntokens > 0 ) {
      clang::MacroInfo::tokens_iterator ti = macro->tokens_end() - 1;
      for( t_idx = ntokens - 1; t_idx >= 0; t_idx-- ) {
        tok = *ti;
        if(tok.getKind() == clang::tok::r_paren) right_parens++;
        else break;
        --ti;
      }
  }

  {
    clang::MacroInfo::tokens_iterator ti = macro->tokens_begin();
    for( t_idx = 0; t_idx < ntokens; t_idx++ ) {
      tok = *ti;
      if(tok.getKind() == clang::tok::l_paren) left_parens++;
      else break;
      ++ti;
    }
  }
  if( left_parens == right_parens &&
      ntokens - left_parens - right_parens == 1 ) {
    // OK!
  } else {
    return ret; // we don't handle complicated expressions like A+B
  }


  switch(tok.getKind()) {
    case clang::tok::numeric_constant: {
      std::string numString (tok.getLiteralData(), tok.getLength());
      if( debugPrint) printf("num = %s\n", numString.c_str());

      if(numString.find('.') == std::string::npos) {
        IF1_int_type size = INT_SIZE_32;

        if(tolower(numString[numString.length() - 1]) == 'l') {
          numString[numString.length() - 1] = '\0';
          size = INT_SIZE_64;
        }

        if(tolower(numString[numString.length() - 1]) == 'u') {
          numString[numString.length() - 1] = '\0';
          ret = new_UIntSymbol(strtoul(numString.c_str(), NULL, 0), size);
        }
        else {
          ret = new_IntSymbol(strtol(numString.c_str(), NULL, 0), size);
        }
      }
      else {
        IF1_float_type size = FLOAT_SIZE_64;

        if(tolower(numString[numString.length() - 1]) == 'l') {
          numString[numString.length() - 1] = '\0';
        }

        ret = new_RealSymbol("real", strtod(numString.c_str(), NULL), size);
      }
      break;
    }
    case clang::tok::string_literal: {
      std::string body = std::string(tok.getLiteralData(), tok.getLength());
      if( debugPrint) printf("str = %s\n", body.c_str());
      ret = new_StringSymbol(body.c_str());
      break;
    }
    case clang::tok::identifier: {
      clang::IdentifierInfo* tokId = tok.getIdentifierInfo();
      std::string idName = tokId->getName();
      ret = info->lvt->getValue(idName);
      if( ! ret.val ) {
        clang::MacroInfo* otherMacro = preproc.getMacroInfo(tokId);
        if( otherMacro ) {
          ret = handleMacro(tokId, otherMacro);
        }
      }
      if( debugPrint) {
        printf("id = %s\n", idName.c_str());
        if( ret.val ) {
          fflush(stdout);
          ret.val->print(llvm::outs());
          printf("\n");
        }
      }
      break; 
    }
    default:
      break;
  }

  if( ret.val ) {
    info->lvt->addGlobalValue(id->getName(), ret);
  }
  return ret;
}
static
void readMacrosClang(void) {
  GenInfo* info = gGenInfo;

  // Pre-populate with important INTxx_MIN/MAX from stdint.h
  // because we have trouble reading these because they have
  // special stuff to get the right constant width, but they
  // are all known integer values.
  info->lvt->addGlobalValue("NULL", new_IntSymbol(0, INT_SIZE_64));
  info->lvt->addGlobalValue("INT8_MIN", new_IntSymbol(INT8_MIN, INT_SIZE_8));
  info->lvt->addGlobalValue("INT8_MAX", new_IntSymbol(INT8_MAX, INT_SIZE_8));
  info->lvt->addGlobalValue("UINT8_MAX", new_IntSymbol(UINT8_MAX, INT_SIZE_8));
  info->lvt->addGlobalValue("INT16_MIN", new_IntSymbol(INT16_MIN, INT_SIZE_16));
  info->lvt->addGlobalValue("INT16_MAX", new_IntSymbol(INT16_MAX, INT_SIZE_16));
  info->lvt->addGlobalValue("UINT16_MAX", new_IntSymbol(UINT16_MAX, INT_SIZE_16));
  info->lvt->addGlobalValue("INT32_MIN", new_IntSymbol(INT32_MIN, INT_SIZE_32));
  info->lvt->addGlobalValue("INT32_MAX", new_IntSymbol(INT32_MAX, INT_SIZE_32));
  info->lvt->addGlobalValue("UINT32_MAX", new_IntSymbol(UINT32_MAX, INT_SIZE_32));
  info->lvt->addGlobalValue("INT64_MIN", new_IntSymbol(INT64_MIN, INT_SIZE_64));
  info->lvt->addGlobalValue("INT64_MAX", new_IntSymbol(INT64_MAX, INT_SIZE_64));
  info->lvt->addGlobalValue("UINT64_MAX", new_IntSymbol(UINT64_MAX, INT_SIZE_64));

  //printf("Running ReadMacrosAction\n");
  clang::Preprocessor &preproc = info->Clang->getPreprocessor();
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
  for(clang::Preprocessor::macro_iterator i = preproc.macro_begin(); i != preproc.macro_end(); i++) {

    handleMacro(i->first, i->second);
  }
};

class CCodeGenConsumer : public clang::ASTConsumer {
  private:
    GenInfo* info;
  public:
    CCodeGenConsumer() : clang::ASTConsumer(), info(gGenInfo) {
      //info->module = new llvm::Module(info->moduleName, info->llvmContext);
    }
    
    virtual ~CCodeGenConsumer() { }
 
    // mostly taken from ModuleBuilder.cpp
     virtual void Initialize(clang::ASTContext &Context) {
       info->Ctx = &Context;
 
       info->module->setTargetTriple(info->Ctx->getTargetInfo().getTriple().getTriple());
       info->module->setDataLayout(info->Ctx->getTargetInfo().getTargetDescription());
       info->targetData = new llvm::TargetData(info->Ctx->getTargetInfo().getTargetDescription());
       info->cgBuilder = new clang::CodeGen::CodeGenModule(Context,
                                  info->codegenOptions,
                                  *info->module,
                                  *info->targetData, *info->Diags);
     }
 
     virtual void HandleCXXStaticMemberVarInstantiation(clang::VarDecl *VD) {
       info->cgBuilder->HandleCXXStaticMemberVarInstantiation(VD);
     }
 
     virtual bool HandleTopLevelDecl(clang::DeclGroupRef DG) {
       // Make sure to emit all elements of a Decl.
       for (clang::DeclGroupRef::iterator I = DG.begin(), E = DG.end(); I != E; ++I) {
         info->cgBuilder->EmitTopLevelDecl(*I);
  
  
         // Custom to Chapel
         if(clang::TypedefDecl *td = llvm::dyn_cast<clang::TypedefDecl>(*I)) {
           const clang::Type *ctype = td->getUnderlyingType().getTypePtrOrNull();
           //printf("Adding typedef %s\n", td->getNameAsString().c_str());
           if(ctype != NULL) {
             info->lvt->addGlobalCDecl(td);
           }
         } else if(clang::FunctionDecl *fd = llvm::dyn_cast<clang::FunctionDecl>(*I)) {
           info->lvt->addGlobalCDecl(fd);
         } else if(clang::VarDecl *vd = llvm::dyn_cast<clang::VarDecl>(*I)) {
           info->lvt->addGlobalCDecl(vd);
         }
         // End custom to Chapel
       }

       return true;
     }
 
     /// HandleTagDeclDefinition - This callback is invoked each time a TagDecl
     /// to (e.g. struct, union, enum, class) is completed. This allows the
     /// client hack on the type, which can occur at any point in the file
     /// (because these can be defined in declspecs).
     virtual void HandleTagDeclDefinition(clang::TagDecl *D) {
       info->cgBuilder->UpdateCompletedType(D);
       
       // In C++, we may have member functions that need to be emitted at this 
       // point.
       if (info->Ctx->getLangOpts().CPlusPlus && !D->isDependentContext()) {
         for (clang::DeclContext::decl_iterator M = D->decls_begin(), 
                                      MEnd = D->decls_end();
              M != MEnd; ++M)
           if (clang::CXXMethodDecl *Method = llvm::dyn_cast<clang::CXXMethodDecl>(*M))
             if (Method->doesThisDeclarationHaveABody() &&
                 (Method->hasAttr<clang::UsedAttr>() || 
                  Method->hasAttr<clang::ConstructorAttr>()))
               info->cgBuilder->EmitTopLevelDecl(Method);
       }


       // Custom to Chapel - make a note of C globals
       if(clang::EnumDecl *ed = llvm::dyn_cast<clang::EnumDecl>(D)) {
          // Add the enum type
          info->lvt->addGlobalCDecl(ed);
          // Add the enum values
          for(clang::EnumDecl::enumerator_iterator e = ed->enumerator_begin(); e != ed->enumerator_end(); e++) {
            info->lvt->addGlobalCDecl(*e); // & goes away with newer clang
          }
       } else if(clang::RecordDecl *rd = llvm::dyn_cast<clang::RecordDecl>(D)) {
          const clang::Type *ctype = rd->getTypeForDecl();
          
          if(ctype != NULL && rd->getDefinition() != NULL) {
            info->lvt->addGlobalCDecl(rd);
          }
       }
       // End Custom to Chapel
     }
 
     virtual void HandleTranslationUnit(clang::ASTContext &Ctx) {
       if (info->Diags->hasErrorOccurred()) {
         return;
       }
 
       /* we don't release the builder now, because
          we want to add a bunch of uses of functions
          that may not have been codegened yet.
       if (info->cgBuilder)
         cgBuilder->Release();
       */
     }
 
     virtual void CompleteTentativeDefinition(clang::VarDecl *D) {
       if (info->Diags->hasErrorOccurred())
         return;
 
       info->cgBuilder->EmitTentativeDefinition(D);
     }
 
     virtual void HandleVTable(clang::CXXRecordDecl *RD, bool DefinitionRequired) {
       if (info->Diags->hasErrorOccurred())
         return;
 
       info->cgBuilder->EmitVTable(RD, DefinitionRequired);
     }
};

class CCodeGenAction : public clang::ASTFrontendAction {
 public:
  CCodeGenAction() { }
 protected:
  virtual clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &CI,
                                                 llvm::StringRef InFile);
};

clang::ASTConsumer *
CCodeGenAction::CreateASTConsumer(clang::CompilerInstance &CI,
                                  llvm::StringRef InFile) {
  return new CCodeGenConsumer();
};

static void cleanupClang(GenInfo* info)
{
  if( info->cgBuilder ) info->cgBuilder->Release();
  delete info->cgBuilder;
  info->cgBuilder = NULL;
  delete info->targetData;
  info->targetData = NULL;
  delete info->Clang;
  info->Clang = NULL;
  delete info->cgAction;
  info->cgAction = NULL;
  info->Diags.reset();
  info->DiagID.reset();
  //delete info->DiagClient;
  //info->DiagClient = NULL;
}

static void setupClang(GenInfo* info, std::string mainFile)
{
  using namespace clang;

  std::string clangexe = info->clangInstallDir + "/bin/clang";
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
 
  clangArgs.push_back("-emit-llvm");
  //clangArgs.push_back("-c");
  clangArgs.push_back(mainFile.c_str()); // chpl - always compile rt file

  info->DiagClient = new TextDiagnosticPrinter(llvm::errs(), info->diagOptions);
  info->DiagID = new clang::DiagnosticIDs();
  info->Diags = new clang::DiagnosticsEngine(info->DiagID, info->DiagClient);

  clang::CompilerInvocation* CI =
    createInvocationFromCommandLine(clangArgs, info->Diags);

  {
    // Make sure we include clang's internal header dir
    llvm::sys::Path P(clangexe);
    if (!P.isEmpty()) {
      P.eraseComponent();  // Remove /clang from foo/bin/clang
      P.eraseComponent();  // Remove /bin   from foo/bin

      // Get foo/lib/clang/<version>/
      P.appendComponent("lib");
      P.appendComponent("clang");
      P.appendComponent(CLANG_VERSION_STRING);
    }
    CI->getHeaderSearchOpts().ResourceDir = P.str();
    llvm::sys::Path P2(P);
    P.appendComponent("include");
    CI->getHeaderSearchOpts().AddPath(P.str(), clang::frontend::System, false, false, false, true, false);
  }

  // Create a compiler instance to handle the actual work.
  info->Clang = new CompilerInstance();
  info->Clang->setInvocation(CI);
  //Clang->setInvocation(CI.take());

  // Create the compilers actual diagnostics engine.
  info->Clang->createDiagnostics(int(clangArgs.size()),&clangArgs[0]);
  if (!info->Clang->hasDiagnostics())
    INT_FATAL("Bad diagnostics from clang");

 
  /*
  // This part only needed to do lower-level clang calls later.
  clang::FileManager fileManaer(Clang->getInvocation->getFileSystemOpts())
  clang::SourceManager *sourceManager(Clang->getDiagnostics(), fileManager);
  clang::SelectorTable selector_table;
  clang::Builtin::Context builtin_context;
  clang::ASTContext* ast_context = 
    new clang::ASTContext(Clang->getLangOpts(),
                          Clang->getSourceManager(),
                          & Clang->getTarget(),
                          Clang->getPreprocessor().getIdentifierTable(),
                          selector_table,
                          builtin_context,
                          0);
  Clang->setASTContext(ast_context);
  */


}
void finishClangGeneration() {
  GenInfo* info = gGenInfo;

#if 0
  std::string m_expr = "int testfun(void) { return 2; }";
//  const char* fname = "chpl-computed-macro-functions.h";

  info->Clang->getASTContext();

  llvm::MemoryBuffer *memory_buffer = llvm::MemoryBuffer::getMemBufferCopy(m_expr, __FUNCTION__);
  //clang::FileID memory_buffer_file_id = info->Clang->getSourceManager().createMainFileIDForMemBuffer (memory_buffer);
  clang::FileID memory_buffer_file_id = info->Clang->getSourceManager().createFileIDForMemBuffer (memory_buffer);
  
  /*
  CCodeGenConsumer* ccgc = new CCodeGenConsumer();
  //clang::ParseAST(info->Clang->getPreprocessor(), ccgc, info->Clang->getASTContext()); doesn't work because it gets the preprocessor to enter the main file.

  clang::Sema* sema = new clang::Sema(info->Clang->getPreprocessor(),
                                      info->Clang->getASTContext(),
                                      *ccgc);
  clang::Parser *P = new clang::Parser(info->Clang->getPreprocessor(),
                                       *sema,
                                       false);
  clang::ASTConsumer *Consumer = &sema->getASTConsumer();
  info->Clang->getPreprocessor().EnterSourceFile(memory_buffer_file_id, NULL, clang::SourceLocation());
  P->Initialize();
  sema->Initialize();
  clang::Parser::DeclGroupPtrTy ADecl;
  P->ParseTopLevelDecl(ADecl);
  // calling handle
  //Consumer->HandleTranslationUnit(sema->getASTContext());

  delete sema;
  delete ccgc;
  */

  /*
    
  // Code generate any additional code.
  if( info->Clang->hasSourceManager() ) {
    info->Clang->getSourceManager().clearIDTables();
  }
  llvm::MemoryBuffer * buf = llvm::MemoryBuffer::getMemBufferCopy(tmp, fname);
  info->Clang->getSourceManager().createFileIDForMemBuffer(buf, 0);
  clang::FrontendInputFile f(fname, clang::IK_C, true);

  if( info->cgAction->BeginSourceFile(* info->Clang, f) ) {
    info->cgAction->Execute();
    info->cgAction->EndSourceFile();
  }
  */

#endif
  // Now finish any Clang code generation.
  cleanupClang(info);
/*
  setupClang(info, "/tmp/bogo.c");
  // Now run the clang compiler again, this time with in-memory
  // code.
  info->cgAction = new CCodeGenAction();
  if (!info->Clang->ExecuteAction(*info->cgAction))
    INT_FATAL("error running clang C code gen action part 2");

  cleanupClang(info);
*/

  // Verify the LLVM module.
  if(llvm::verifyModule(*info->module,llvm::PrintMessageAction)){
    INT_FATAL("LLVM module verification failed");
  }

  //
  //llvm::llvm_shutdown();
}


static void handleErrorLLVM(void* user_data, const std::string& reason)
{
  INT_FATAL("llvm fatal error: %s", reason.c_str());
}

#endif

GenInfo::GenInfo(std::string clangInstallDirIn, std::string compilelineIn, std::vector<std::string> clangCCArgsIn, std::vector<std::string> clangLDArgsIn, std::vector<std::string> clangOtherArgsIn )
       :   cfile(NULL), cLocalDecls(), cStatements(),
           lineno(-1), filename(NULL)
#ifdef HAVE_LLVM
           , module(NULL), builder(NULL), lvt(NULL),
           clangInstallDir(clangInstallDirIn),
           compileline(compilelineIn),
           clangCCArgs(clangCCArgsIn), clangLDArgs(clangLDArgsIn),
           clangOtherArgs(clangOtherArgsIn),
           codegenOptions(), /*targetData(), */ diagOptions(),
           DiagClient(NULL),
           DiagID(NULL),
           Diags(NULL),
          // TheDriver(NULL), C(NULL), CI(NULL),
           Clang(NULL),
           moduleName("root"), llvmContext(), Ctx(NULL),
           targetData(NULL), cgBuilder(NULL) //, clangGlobals()
#endif
{

#ifdef HAVE_LLVM
  std::string home(CHPL_HOME);
  std::string rtmain = home + "/runtime/include/rtmain.c";

  setupClang(this, rtmain);

  // Create a new LLVM module, IRBuilder, and LayeredValueTable. 
  module = new llvm::Module(moduleName, llvmContext);
  builder = new llvm::IRBuilder<>(module->getContext());
  lvt = new LayeredValueTable();

  // These are initialized only after we have types
  // for everything and are deciding what calls to make.
  // these are set by CCodeGenAction.
  Ctx = NULL;
  targetData = NULL;
  cgBuilder = NULL;
#endif
}

void readCTypes(void) {
  /* TODO -- note that clang/examples/clang-interpreter/main.cpp 
             includes an example for getting the executable path,
             so that we could automatically set CHPL_HOME. */
  std::string home(CHPL_HOME);
  std::string compileline = home + "/util/config/compileline";
  if( debugCCode ) compileline += " DEBUG=1";
  if( optimizeCCode ) compileline += " OPTIMIZE=1";
  std::string readargsfrom = compileline + " --llvm-install-dir --includes-and-defines";
  std::vector<std::string> args;
  std::vector<std::string> clangCCArgs;
  std::vector<std::string> clangLDArgs;
  std::vector<std::string> clangOtherArgs;
  std::string clangInstallDir;

  // Add cflags,etc that used to be put into the Makefile
  // (see codegen_makefile in files.cpp)
  if (ccwarnings) {
    // Could add warning arguments here. to clangCCArgs
  }

  if (debugCCode) {
    clangCCArgs.push_back("-g");
  }

  if (optimizeCCode) {
    clangCCArgs.push_back("-O3");
  }

  if (fieeefloat) {
    // believe clang is always ieee float
  } else {
    clangCCArgs.push_back("-ffast-math");
  }

  forv_Vec(const char*, dirName, incDirs) {
    clangCCArgs.push_back(std::string("-I") + dirName);
  }

  clangCCArgs.push_back(ccflags);

  clangLDArgs.push_back(ldflags);

  for (int i=0; i<numLibFlags; i++)
    clangLDArgs.push_back(libFlag[i]);

  clangCCArgs.push_back("-DCHPL_GEN_CODE");
  
  // Gather information from readargsfrom into clangArgs.
  readArgsFromCommand(readargsfrom.c_str(), args);
  clangInstallDir = args[0];
  for( size_t i = 1; i < args.size(); ++i ) {
    clangOtherArgs.push_back(args[i]);
  }

  clangOtherArgs.push_back("-include");
  clangOtherArgs.push_back("sys_basic.h");

  // Include header files from the command line.
  {
    int filenum = 0;
    while (const char* inputFilename = nthFilename(filenum++)) {
      if (isCHeader(inputFilename)) {
        std::string s("-include");
        s += inputFilename;
        clangOtherArgs.push_back(s);
      }
    }
  }

  // Initialize gGenInfo
  gGenInfo = new GenInfo(clangInstallDir, compileline, clangCCArgs, clangLDArgs, clangOtherArgs);

  // NOTE -- in the future, turn these on for to C code generation
  // to support "include a C file"
  if( llvmCodegen )
  {
#ifdef HAVE_LLVM
    GenInfo *info = gGenInfo;

    // Install an LLVM Fatal Error Handler.
    llvm::install_fatal_error_handler(handleErrorLLVM);

    // Run the Start Generation action
    // Now initialize a code generator...
    // this will enable us to ask for addresses of static (inline) functions
    // and cause them to be emitted eventually.
    info->cgAction = new CCodeGenAction();
    if (!info->Clang->ExecuteAction(*info->cgAction))
      INT_FATAL("error running clang C code gen action");


    // Some IRBuilder methods, codegenning a string,
    // need a basic block in order to get to the module
    // so we create a dummy function to code generate into
    llvm::Type * voidTy =  llvm::Type::getVoidTy(info->module->getContext());
    std::vector<llvm::Type*> args;
    llvm::FunctionType * FT = llvm::FunctionType::get(voidTy, args, false);
    llvm::Function * F = llvm::Function::Create(FT, llvm::Function::InternalLinkage, "chplDummyFunction", info->module);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(info->module->getContext(), "entry", F);
    info->builder->SetInsertPoint(block);

    // read macros. May call IRBuilder methods to codegen a string,
    // so needs to happen after we set the insert point.
    readMacrosClang();

    info->builder->CreateRetVoid();
#endif
  }
}

#ifdef HAVE_LLVM
llvm::Function* getFunctionLLVM(const char* name)
{
  GenInfo* info = gGenInfo;
  llvm::Function* fn = info->module->getFunction(name);
  if( fn ) return fn;

  GenRet got = info->lvt->getValue(name);
  if( got.val ) {
    fn = llvm::cast<llvm::Function>(got.val);
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
#endif

std::string numToString(int64_t num)
{
  char name[32];
  sprintf(name, "%" PRId64, num);
  return std::string(name);
}
std::string int64_to_string(int64_t i)
{
  char buf[32];
  sprintf(buf, "%"PRId64, i);
  std::string ret(buf);
  return ret;
}
std::string uint64_to_string(uint64_t i)
{
  char buf[32];
  sprintf(buf, "%"PRIu64, i);
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
void genIdComment(int id) {
  GenInfo* info = gGenInfo;
  if( info->cfile ) fprintf(info->cfile, "/* %7d */ ", id);
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

#ifdef HAVE_LLVM
// should support TypedefDecl,EnumDecl,RecordDecl
llvm::Type* codegenCType(const clang::TypeDecl* td)
{
  GenInfo* info = gGenInfo;
  clang::CodeGen::CodeGenTypes & cdt = info->cgBuilder->getTypes();
  clang::QualType qType;

  // handle TypedefDecl
  if( const clang::TypedefNameDecl* tnd = llvm::dyn_cast<clang::TypedefNameDecl>(td) ) {
    qType = tnd->getCanonicalDecl()->getUnderlyingType();
    // had const clang::Type *ctype = td->getUnderlyingType().getTypePtrOrNull();
    //could also do:
    //  qType = tnd->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
  } else if( const clang::EnumDecl* ed = llvm::dyn_cast<clang::EnumDecl>(td) ) {
    qType = ed->getCanonicalDecl()->getIntegerType(); // could be getPromotionType()
    //could also do:
    //  qType = tnd->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
  } else if( const clang::RecordDecl* rd = llvm::dyn_cast<clang::RecordDecl>(td) ) {
    clang::RecordDecl *def = rd->getDefinition();
    INT_ASSERT(def);
    qType = def->getCanonicalDecl()->getTypeForDecl()->getCanonicalTypeInternal();
  } else {
    INT_FATAL("Unknown clang type declaration");
  }
  return cdt.ConvertTypeForMem(qType);
}

// should support FunctionDecl,VarDecl,EnumConstantDecl
GenRet codegenCValue(const clang::ValueDecl *vd)
{
  GenInfo* info = gGenInfo;
  GenRet ret;

  if( info->cfile ) {
    ret.c = vd->getName();
    return ret;
  }

  if(const clang::FunctionDecl *fd = llvm::dyn_cast<clang::FunctionDecl>(vd)) {
    // It's a function decl.
    ret.val = info->cgBuilder->GetAddrOfFunction(fd);
    ret.isLVPtr = GEN_VAL;
  } else if(const clang::VarDecl *vard = llvm::dyn_cast<clang::VarDecl>(vd)) {
    // It's a (global) variable decl
    ret.val = info->cgBuilder->GetAddrOfGlobalVar(vard);
    ret.isLVPtr = GEN_PTR;
  } else if(const clang::EnumConstantDecl *ed = llvm::dyn_cast<clang::EnumConstantDecl>(vd)) {
    // It's a constant enum value
    llvm::APInt v = ed->getInitVal();

    ret.isUnsigned = ! ed->getType()->hasSignedIntegerRepresentation();
    clang::CodeGen::CodeGenTypes & cdt = info->cgBuilder->getTypes();
    llvm::Type* type = cdt.ConvertTypeForMem(ed->getType());
    ret.val = llvm::ConstantInt::get(type, v);
    ret.isLVPtr = GEN_VAL;
  } else {
    INT_FATAL("Unknown clang value declaration");
  }
  return ret;
}

LayeredValueTable::LayeredValueTable(){
  layers.push_front(std::map<std::string, Storage>());
}

void LayeredValueTable::addLayer(){
  layers.push_front(std::map<std::string, Storage>());
}

void LayeredValueTable::removeLayer(){
  if(layers.size() != 1) {
    layers.pop_front();
  }
}


void LayeredValueTable::addValue(std::string name, llvm::Value *value, uint8_t isLVPtr, bool isUnsigned) {
  Storage store;
  store.u.value = value;
  store.isLVPtr = isLVPtr;
  store.isUnsigned = isUnsigned;
  //layers.front().insert(std::pair<std::string, Storage>(name, store));
  (layers.front())[name] = store;
}

void LayeredValueTable::addGlobalValue(std::string name, llvm::Value *value, uint8_t isLVPtr, bool isUnsigned) {
  Storage store;
  store.u.value = value;
  store.isLVPtr = isLVPtr;
  store.isUnsigned = isUnsigned;
  //layers.back().insert(std::pair<std::string, Storage>(name, store));
  (layers.back())[name] = store;
}

void LayeredValueTable::addGlobalValue(std::string name, GenRet gend) {
  addGlobalValue(name, gend.val, gend.isLVPtr, gend.isUnsigned);
}
 
void LayeredValueTable::addGlobalType(std::string name, llvm::Type *type) {
  Storage store;
  store.u.type = type;
  layers.back().insert(std::pair<std::string, Storage>(name, store));
}

void LayeredValueTable::addGlobalCDecl(clang::NamedDecl* cdecl) {
  std::string name = cdecl->getNameAsString(); // could be getName().str()
  Storage store;
  store.u.cdecl = cdecl;

  layers.back().insert(std::pair<std::string, Storage>(name, store));
}

void LayeredValueTable::addBlock(std::string name, llvm::BasicBlock *block) {
  Storage store;
  store.u.block = block;

  layer_iterator blockLayer = --layers.end();
  if(layers.size() > 1) {
    blockLayer = --blockLayer;
  }
  blockLayer->insert(std::pair<std::string, Storage>(name, store));
}

GenRet LayeredValueTable::getValue(std::string name) {
  if(Storage *store = get(name)) {
    if( store->u.value && llvm::isa<llvm::Value>(store->u.value) ) {
      GenRet ret;
      ret.val = store->u.value;
      ret.isLVPtr = store->isLVPtr;
      ret.isUnsigned = store->isUnsigned;
      return ret;
    }
    if( store->u.cdecl && llvm::isa<clang::NamedDecl>(store->u.cdecl) ) {
      // we have a clang named decl.
      // maybe TypedefDecl,EnumDecl,RecordDecl,FunctionDecl,VarDecl,EnumConstantDecl
      if( llvm::isa<clang::ValueDecl>(store->u.cdecl) ) {
        clang::ValueDecl* vd = llvm::cast<clang::ValueDecl>(store->u.cdecl);

        // Convert it to an LLVM value
        // should support FunctionDecl,VarDecl,EnumConstantDecl
        return codegenCValue(vd);
      }
    }
  }
  GenRet ret;
  return ret;
}

llvm::BasicBlock *LayeredValueTable::getBlock(std::string name) {
  if(Storage *store = get(name)) {
    if( store->u.block && llvm::isa<llvm::BasicBlock>(store->u.block) )
      return store->u.block;
  }
  return NULL;
}

llvm::Type *LayeredValueTable::getType(std::string name) {
  if(Storage *store = get(name)) {
    if( store->u.type && llvm::isa<llvm::Type>(store->u.type) )
      return store->u.type;
    if( store->u.cdecl && llvm::isa<clang::NamedDecl>(store->u.cdecl) ) {
      // we have a clang named decl.
      // maybe TypedefDecl,EnumDecl,RecordDecl,FunctionDecl,VarDecl,EnumConstantDecl
      if( llvm::isa<clang::TypeDecl>(store->u.cdecl) ) {
        clang::TypeDecl* td = llvm::cast<clang::TypeDecl>(store->u.cdecl);
        // Convert it to an LLVM type.
        // should support TypedefDecl,EnumDecl,RecordDecl
        return codegenCType(td);
      }
    }
  }
  return NULL;
}

clang::NamedDecl* LayeredValueTable::getCDecl(std::string name) {
  if(Storage *store = get(name)) {
    if( store->u.cdecl && llvm::isa<clang::NamedDecl>(store->u.cdecl) ) {
      // we have a clang named decl.
      // maybe TypedefDecl,EnumDecl,RecordDecl,FunctionDecl,VarDecl,EnumConstantDecl
      return store->u.cdecl;
    }
  }
  return NULL;
}


LayeredValueTable::Storage* LayeredValueTable::get(std::string name) {
  for(layer_iterator i = layers.begin(); i != layers.end(); ++i) {
    value_iterator j = i->find(name);

    if(j != i->end())
    {
      return &j->second;
    }
  }

  return NULL;
}

int getCRecordMemberGEP(const char* typeName, const char* fieldName)
{
  GenInfo* info = gGenInfo;
  clang::NamedDecl* d = info->lvt->getCDecl(typeName);
  int ret;
  INT_ASSERT(d);
  if( llvm::isa<clang::TypedefDecl>(d) ) {
    clang::TypedefDecl* td = llvm::cast<clang::TypedefDecl>(d);
    const clang::Type* t = td->getUnderlyingType().getTypePtr();
    while( t->isPointerType() ) {
      t = t->getPointeeType().getTypePtr();
    }
    const clang::RecordType* rt = t->getAsStructureType();
    INT_ASSERT(rt);
    d = rt->getDecl();
    // getAsUnionType also available, but we don't support extern unions
  }
  INT_ASSERT(llvm::isa<clang::RecordDecl>(d));
  clang::RecordDecl* rec = llvm::cast<clang::RecordDecl>(d);
  // Find the field decl.
  clang::RecordDecl::field_iterator it;
  clang::FieldDecl* field = NULL;
  for( it = rec->field_begin(); it != rec->field_end(); ++it ) {
    if( fieldName == it->getName() ) {
      field = *it;
      break;
    }
  }
  INT_ASSERT(field);
  ret = info->cgBuilder->getTypes().getCGRecordLayout(rec).getLLVMFieldNo(field);
  return ret;
}

bool isBuiltinExternCFunction(const char* cname)
{
  if( 0 == strcmp(cname, "sizeof") ) return true;
  else return false;
}

#endif

