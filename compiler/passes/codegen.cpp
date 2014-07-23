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

// Global so that we don't have to pass around
// to all of the codegen() routines
GenInfo* gGenInfo;
int gMaxVMT = -1;
fileinfo gAllExternCode;
int stmtCount = 0;


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
    llvm::GlobalVariable * gv = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(name, id_type));
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
    llvm::GlobalVariable *globalString = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(
          cname, llvm::IntegerType::getInt8PtrTy(info->module->getContext())));
    globalString->setInitializer(llvm::cast<llvm::GlobalVariable>(
          new_StringSymbol(value)->codegen().val)->getInitializer());
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
    llvm::GlobalVariable *globalInt = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(
          cname, llvm::IntegerType::getInt32Ty(info->module->getContext())));
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
    if (AggregateType* ct = toAggregateType(ts->type)) {
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
      table[fID++] = llvm::cast<llvm::Constant>(
          info->builder->CreatePointerCast(func, funcPtrType));
    }
    if (fSymbols.n == 0) {
      table[0] = llvm::Constant::getNullValue(funcPtrType);
    }
    
    llvm::ArrayType *funcPtrTableType =
      llvm::ArrayType::get(funcPtrType, table.size());
    
    if(llvm::GlobalVariable *ftable =
        info->module->getNamedGlobal(ftable_name)) {
      ftable->eraseFromParent();
    }
    
    llvm::GlobalVariable *ftable = llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(ftable_name, funcPtrTableType));
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
    if(virtualMethodTable.v[i].key && virtualMethodTable.v[i].value->n > maxVMT)
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
      if (AggregateType* ct = toAggregateType(ts->type)) {
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
      if (AggregateType* ct = toAggregateType(ts->type)) {
        if (!isReferenceType(ct) && isClass(ct)) {
          int n = 0;
          if (Vec<FnSymbol*>* vfns = virtualMethodTable.get(ct)) {
            forv_Vec(FnSymbol, vfn, *vfns) {
              llvm::Function *func = getFunctionLLVM(vfn->cname);
              table.push_back(llvm::cast<llvm::Constant>(
                    info->builder->CreatePointerCast(func, funcPtrType)));
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
    
    llvm::ArrayType *vmTableType =
      llvm::ArrayType::get(vmTableEntryType, table.size());
    
    if(llvm::GlobalVariable *vmTable = info->module->getNamedGlobal(vmtData)) {
      vmTable->eraseFromParent();
    }
    
    llvm::GlobalVariable *vmTable =llvm::cast<llvm::GlobalVariable>(
        info->module->getOrInsertGlobal(vmtData, vmTableType));
    vmTable->setInitializer(llvm::ConstantArray::get(vmTableType, table));
    vmTable->setConstant(true);
    
    llvm::Value* vmtElmPtr =
      info->builder->CreateConstInBoundsGEP2_64(vmTable, 0, 0);
     
    info->lvt->addGlobalValue(vmt, vmtElmPtr, GEN_VAL, true);
#endif
  }
}

static int
compareSymbol(const void* v1, const void* v2) {
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

static inline bool shouldCodegenAggregate(AggregateType* ct)
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


static void codegen_aggregate_def(AggregateType* ct) {
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


//
// Produce compilation-time configuration info into a .h file and
// #include that .h into the current codegen output file.
//
// Only put C data objects into this file, not Chapel ones, as it may
// also be #include'd into a launcher, and those are C/C++ code.
//
static const char* cfg_fname = "chpl_compilation_config";
static void codegen_header_compilation_config() {
  int i;
  GenInfo* info = gGenInfo;
  FILE* save_cfile = info->cfile;

  fileinfo cfgfile = { NULL, NULL, NULL };

  openCFile(&cfgfile, cfg_fname, "h");

  if (!cfgfile.fptr) return; // follow convention of just not writing
                             // to the file if we can't open it

  info->cfile = cfgfile.fptr;

  genComment("Compilation Info");

  genGlobalString("chpl_compileCommand", compileCommand);
  genGlobalString("chpl_compileVersion", compileVersion);
  genGlobalString("CHPL_HOME", CHPL_HOME);

  for (i=0; i < num_chpl_env_vars; i++) {
    genGlobalString(chpl_env_var_names[i], chpl_env_vars[i]);
  }

  genGlobalInt("CHPL_STACK_CHECKS", !fNoStackChecks);
  genGlobalInt("CHPL_CACHE_REMOTE", fCacheRemote);

  // generate the "about" function
  fprintf(cfgfile.fptr, "\nvoid chpl_program_about(void);\n");
  fprintf(cfgfile.fptr, "\nvoid chpl_program_about() {\n");
  fprintf(cfgfile.fptr, "printf(\"Compilation command: %s\\n\");\n",
          compileCommand);
  fprintf(cfgfile.fptr, "printf(\"Chapel compiler version: %s\\n\");\n",
          compileVersion);
  fprintf(cfgfile.fptr, "printf(\"Chapel environment:\\n\");\n");
  fprintf(cfgfile.fptr, "printf(\"  CHPL_HOME: %s\\n\");\n",
          CHPL_HOME);
  for (i=0; i < num_chpl_env_vars; i++) {
    fprintf(cfgfile.fptr, "printf(\"  %s: %s\\n\");\n",
            chpl_env_var_names[i], chpl_env_vars[i]);
  }
  fprintf(cfgfile.fptr, "}\n");

  closeCFile(&cfgfile);

  info->cfile = save_cfile;
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
      if (AggregateType* ct = toAggregateType(ts->type)) {
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

  codegen_header_compilation_config();

  FILE* hdrfile = info->cfile;

  if( hdrfile ) {
    // This is done in runClang for LLVM version.
    fprintf(hdrfile, "\n#define CHPL_GEN_CODE\n\n");

    // Include sys_basic.h to get C types always defined,
    // proper library .h inclusion
    fprintf(hdrfile, "#include \"sys_basic.h\"\n");
    genIncludeCommandLineHeaders(hdrfile);

    fprintf(hdrfile, "#include \"stdchpl.h\"\n");

    // Include the compilation config file
    fprintf(hdrfile, "#include \"%s.h\"\n", cfg_fname);

    //include generated extern C header file
    if (externC && gAllExternCode.filename != NULL) {
      fprintf(hdrfile, "%s", astr("#include \"", gAllExternCode.filename, "\"\n"));
    }
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
      if (AggregateType* ct = toAggregateType(ts->type))
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

  if( ! info->cfile ) { 
    // Codegen any type annotations that are necessary.
    forv_Vec(TypeSymbol, typeSymbol, types) {
      typeSymbol->codegenMetadata();
    }
  }


  genComment("Function Prototypes");
  forv_Vec(FnSymbol, fnSymbol, functions) {
    fnSymbol->codegenPrototype();
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

  genComment("Virtual Method Table");
  genVirtualMethodTable(types);

  genComment("Global Variables");
  forv_Vec(VarSymbol, varSymbol, globals) {
    varSymbol->codegenGlobalDef();
  }
  flushStatements();

  genGlobalInt("chpl_numGlobalsOnHeap", numGlobalsOnHeap);
  int globals_registry_static_size = (numGlobalsOnHeap ? numGlobalsOnHeap : 1);
  if( hdrfile ) {
    fprintf(hdrfile, "\nptr_wide_ptr_t chpl_globals_registry[%d];\n",
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
      memDescTable.push_back(llvm::cast<llvm::GlobalVariable>(
            new_StringSymbol(memDesc)->codegen().val)->getInitializer());
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

  genGlobalInt("chpl_mem_numDescs", memDescsVec.n);

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
        // To preserve operand order, this should be insertAtTail.
        // The change must also be made below (for LLVM) and in the signature
        // of chpl_comm_broadcast_private().
        call->insertAtHead(new_IntSymbol(i));
        i++;
      }
    }
    fprintf(hdrfile, "\n};\n");
  } else {
#ifdef HAVE_LLVM
    llvm::Type *private_broadcastTableEntryType =
      llvm::IntegerType::getInt8PtrTy(info->module->getContext());

    std::vector<llvm::Constant *> private_broadcastTable;
    private_broadcastTable.push_back(llvm::cast<llvm::Constant>(
          info->builder->CreatePointerCast(
            info->lvt->getValue("chpl_verbose_comm").val,
            private_broadcastTableEntryType)));
    private_broadcastTable.push_back(llvm::cast<llvm::Constant>(
          info->builder->CreatePointerCast(
            info->lvt->getValue("chpl_comm_diagnostics").val,
            private_broadcastTableEntryType)));
    private_broadcastTable.push_back(llvm::cast<llvm::Constant>(
          info->builder->CreatePointerCast(
            info->lvt->getValue("chpl_verbose_mem").val,
            private_broadcastTableEntryType)));

    int broadcastID = 3;
    forv_Vec(CallExpr, call, gCallExprs) {
      if (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) {
        SymExpr* se = toSymExpr(call->get(1));
        INT_ASSERT(se);

        private_broadcastTable.push_back(llvm::cast<llvm::Constant>(
              info->builder->CreatePointerCast(
                info->lvt->getValue(se->var->cname).val,
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
  }
 
 
  if( llvmCodegen ) {
#ifdef HAVE_LLVM
    llvm::FunctionType *createConfigType;
    llvm::Function *createConfigFunc;
    genGlobalInt("mainHasArgs", mainHasArgs);
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
    info->builder->SetInsertPoint(createConfigBlock);

    llvm::Function *initConfigFunc = getFunctionLLVM("initConfigVarTable");
    info->builder->CreateCall(initConfigFunc);

    llvm::Function *installConfigFunc = getFunctionLLVM("installConfigVar");

    forv_Vec(VarSymbol, var, gVarSymbols) {
      if (var->hasFlag(FLAG_CONFIG) && !var->hasFlag(FLAG_TYPE_VARIABLE)) {
        std::vector<llvm::Value *> args (3);
        args[0] = info->builder->CreateLoad(
            new_StringSymbol(var->name)->codegen().val);

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
        args[1] = info->builder->CreateLoad(
            new_StringSymbol(type->symbol->name)->codegen().val);

        if (var->getModule()->modTag == MOD_INTERNAL) {
          args[2] = info->builder->CreateLoad(
              new_StringSymbol("Built-in")->codegen().val);
        }
        else {
          args[2] =info->builder->CreateLoad(
              new_StringSymbol(var->getModule()->name)->codegen().val);
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

  if( fLLVMWideOpt ) {
    // --llvm-wide-opt is picky about other settings.
    // Check them here.
    if (!llvmCodegen ) USR_FATAL("--llvm-wide-opt requires --llvm");
    if ( widePointersStruct ) {
      // generating global pointers of size > 64 bits is not
      // possible with LLVM 3.3; it might be possible in the future.

      // If we have -fLLVMWideOpt, we must use packed wide
      // pointers (because optimizations assume pointer size
      //  is the same - at most 64 bits - for all address spaces.
      //  'multiple address space' patch series, submitted to LLVM 3.2,
      //  was backed out mostly for lack of testing. Perhaps the situation
      //  will be resolved in LLVM 3.4).
      USR_FATAL("--llvm-wide-opt requires packed wide pointers; " \
                "try export CHPL_WIDE_POINTERS=node16");
    }
  }

  if( widePointersStruct ) {
    // OK
  } else {
    // While the C code generator can emit packed pointers,
    // it does so only to help make sure that packed pointer code
    // generation is correct. It is not a "supported configuration".
    if( ! llvmCodegen )
      USR_WARN("C code generation for packed pointers not supported");
  }

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

  fileinfo hdrfile  = { NULL, NULL, NULL };
  fileinfo mainfile = { NULL, NULL, NULL };

  GenInfo* info     = gGenInfo;

  INT_ASSERT(info);

  if( llvmCodegen ) {
#ifdef HAVE_LLVM
    if( fHeterogeneous )
      INT_FATAL("fHeretogeneous not yet supported with LLVM");

    prepareCodegenLLVM();
#endif
  } else {
    openCFile(&hdrfile,  "chpl__header", "h");
    openCFile(&mainfile, "_main",        "c");

    fprintf(mainfile.fptr, "#include \"chpl__header.h\"\n");

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

    finishCodegenLLVM();
#endif 
    return;
  }

  if (fHeterogeneous) {
    codegenTypeStructureInclude(mainfile.fptr);
    forv_Vec(TypeSymbol, ts, gTypeSymbols) {
      if ((ts->type != dtOpaque) &&
          (!toPrimitiveType(ts->type) ||
           !toPrimitiveType(ts->type)->isInternalType)) {
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

  if (fPrintEmittedCodeSize)
  {
    fprintf(stderr, "Statements emitted: %d\n", stmtCount);
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
}

#ifdef HAVE_LLVM
GenInfo::GenInfo(
    std::string clangInstallDirIn,
    std::string compilelineIn,
    std::vector<std::string> clangCCArgsIn,
    std::vector<std::string> clangLDArgsIn,
    std::vector<std::string> clangOtherArgsIn,
    bool parseOnlyIn )
       :   cfile(NULL), cLocalDecls(), cStatements(),
           lineno(-1), filename(NULL), parseOnly(parseOnlyIn),
           // the rest of these are only in GenInfo with HAVE_LLVM
           module(NULL), builder(NULL), lvt(NULL),
           clangInstallDir(clangInstallDirIn),
           compileline(compilelineIn),
           clangCCArgs(clangCCArgsIn), clangLDArgs(clangLDArgsIn),
           clangOtherArgs(clangOtherArgsIn),
           codegenOptions(), diagOptions(NULL),
           DiagClient(NULL),
           DiagID(NULL),
           Diags(NULL),
           Clang(NULL), clangTargetOptions(), clangLangOptions(),
           moduleName("root"), llvmContext(), Ctx(NULL),
           targetData(NULL), cgBuilder(NULL), cgAction(NULL),
           tbaaRootNode(NULL), tbaaFtableNode(NULL), tbaaVmtableNode(NULL),
           targetLayout(), globalToWideInfo(),
           FPM_postgen(NULL)
{
  std::string home(CHPL_HOME);
  std::string rtmain = home + "/runtime/include/rtmain.c";

  setupClang(this, rtmain);

  // Create a new LLVM module, IRBuilder, and LayeredValueTable. 
  if( ! parseOnly ) {
    module = new llvm::Module(moduleName, llvmContext);
    builder = new llvm::IRBuilder<>(module->getContext());
  }

  lvt = new LayeredValueTable();

  // These are initialized only after we have types
  // for everything and are deciding what calls to make.
  // these are set by setupClangContext from CCodeGenAction.
  Ctx = NULL;
  targetData = NULL;
  cgBuilder = NULL;
}
#endif
// No LLVM
GenInfo::GenInfo()
         :   cfile(NULL), cLocalDecls(), cStatements(),
             lineno(-1), filename(NULL), parseOnly(false)
#ifdef HAVE_LLVM
             // Could set more of these to NULL, but the real
             // point is to just core-dump if we end up trying
             // to use them....
             , module(NULL), builder(NULL), lvt(NULL)
#endif
{
}

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

