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

  // hilde sez:  This is very kludgy.  What do we really mean?
  if ((!strncmp("chpl_", sym->cname, 5) && (strcmp("chpl_gen_main", sym->cname) && strcmp("chpl_user_main", sym->cname))  && sym->cname[5] != '_') ||
      (sym->cname[0] == '_' && (sym->cname[1] == '_' || (sym->cname[1] >= 'A' && sym->cname[1] <= 'Z')))) {
    sym->cname = astr("chpl__", sym->cname);
  }
}


static void
genClassIDs(FILE* outfile, Vec<TypeSymbol*> typeSymbols) {
  fprintf(outfile, "/*** Class Type Identification Numbers ***/\n\n");
  int count=0;
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ClassType* ct = toClassType(ts->type)) {
      if (!isReferenceType(ct) && isClass(ct)) {
        fprintf(outfile, "const chpl__class_id chpl__cid_%s = %d;\n", 
                ts->cname, count++);
      }
    }
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
  // never codegen definitions of primitive types
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


static void codegen_aggregate_def(Vec<ClassType*>& visit,
                                  FILE* hdrfile,
                                  ClassType* ct) {
  if(!shouldCodegenAggregate(ct)) return;
  if(visit.set_in(ct)) return;
  visit.set_add(ct);

  // For reference or data class types, first generate
  // the referenced type
  Type* vt = NULL;
  if(ct->symbol->hasFlag(FLAG_REF))
    vt = ct->symbol->getValType();
  else if(ct->symbol->hasFlag(FLAG_DATA_CLASS))
    vt = getDataClassType(ct->symbol)->typeInfo();
  if (vt) {
    if (ClassType* fct = toClassType(vt)) {
      codegen_aggregate_def(visit, hdrfile, fct);
    }
  }
  // For other types, generate the field types
  for_fields(field, ct) {
    if (ClassType* fct = toClassType(field->type)) {
      codegen_aggregate_def(visit, hdrfile, fct);
    }
  }
  // Lastly, generate the type we're working on.
  // Codegen what we have here.
  ct->symbol->codegenDef(hdrfile);
  fprintf(hdrfile, "\n");
}

// TODO: Split this into a number of smaller routines.<hilde>
static void codegen_header(FILE* hdrfile, FILE* codefile=NULL) {
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

  fprintf(hdrfile, "/*** Compilation Info ***/\n\n");
  if (fGPU)
    fprintf(hdrfile, "#ifndef ENABLE_GPU\n");
  fprintf(hdrfile, "const char* chpl_compileCommand     = \"%s\";\n", compileCommand);
  fprintf(hdrfile, "const char* chpl_compileVersion     = \"%s\";\n", compileVersion);
  fprintf(hdrfile, "const char* CHPL_HOST_PLATFORM      = \"%s\";\n", CHPL_HOST_PLATFORM);
  fprintf(hdrfile, "const char* CHPL_TARGET_PLATFORM    = \"%s\";\n", CHPL_TARGET_PLATFORM);
  fprintf(hdrfile, "const char* CHPL_HOST_COMPILER      = \"%s\";\n", CHPL_HOST_COMPILER);
  fprintf(hdrfile, "const char* CHPL_TARGET_COMPILER    = \"%s\";\n", CHPL_TARGET_COMPILER);
  fprintf(hdrfile, "const char* CHPL_TASKS              = \"%s\";\n", CHPL_TASKS);
  fprintf(hdrfile, "const char* CHPL_THREADS            = \"%s\";\n", CHPL_THREADS);
  fprintf(hdrfile, "const char* CHPL_COMM               = \"%s\";\n", CHPL_COMM);
  if (fGPU) {
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

  fprintf(hdrfile, "\n#define CHPL_GEN_CODE\n\n");


  // Include sys_basic.h to get C types always defined, proper library .h inclusion
  fprintf(hdrfile, "#include \"sys_basic.h\"\n");

  genIncludeCommandLineHeaders(hdrfile);

  fprintf(hdrfile, "#include \"stdchpl.h\"\n");

  genClassIDs(hdrfile, types);

  fprintf(hdrfile, "\n/*** Class Prototypes ***/\n\n");
  forv_Vec(TypeSymbol, typeSymbol, types) {
    if (!typeSymbol->hasFlag(FLAG_REF) && !typeSymbol->hasFlag(FLAG_DATA_CLASS))
      typeSymbol->codegenPrototype(hdrfile);
  }

  // codegen enumerated types
  fprintf(hdrfile, "\n/*** Enumerated Types ***/\n\n");

  forv_Vec(TypeSymbol, typeSymbol, types) {
    if (toEnumType(typeSymbol->type))
      typeSymbol->codegenDef(hdrfile);
  }

  {
    Vec<ClassType*> visit;

    // codegen records/unions/references/data class in topological order
    fprintf(hdrfile, "\n/*** Records, Unions, Data Class, References (Hierarchically) ***/\n\n");
    forv_Vec(TypeSymbol, ts, types) {
      if (ClassType* ct = toClassType(ts->type)) {
        codegen_aggregate_def(visit, hdrfile, ct);
        fprintf(hdrfile, "\n");
      }
    }
  }

  // codegen remaining types
  fprintf(hdrfile, "\n/*** Classes ***/\n\n");
  forv_Vec(TypeSymbol, typeSymbol, types) {
    if (isClass(typeSymbol->type) &&
        !typeSymbol->hasFlag(FLAG_REF) &&
        !typeSymbol->hasFlag(FLAG_DATA_CLASS) &&
        typeSymbol->hasFlag(FLAG_NO_OBJECT) &&
        !typeSymbol->hasFlag(FLAG_OBJECT_CLASS))
      typeSymbol->codegenDef(hdrfile);
  }

  //
  // codegen class definitions in breadth first order starting with
  // "object" and following its dispatch children
  //
  Vec<TypeSymbol*> next, current;
  current.add(dtObject->symbol);
  while (current.n) {
    forv_Vec(TypeSymbol, ts, current) {
      ts->codegenDef(hdrfile);
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

  fprintf(hdrfile, "\n/*** Function Prototypes ***/\n\n");
  forv_Vec(FnSymbol, fnSymbol, functions) {
    if (fnSymbol->hasFlag(FLAG_GPU_ON)) {
      fprintf(hdrfile, "\n#ifdef ENABLE_GPU\n");
      fnSymbol->codegenPrototype(hdrfile);
      fprintf(hdrfile, "#endif\n");
      continue;
    }
    if (!fnSymbol->hasFlag(FLAG_EXTERN)) {
      fnSymbol->codegenPrototype(hdrfile);
    }
  }
    
  if (fGPU)
    fprintf(hdrfile, "\n#ifndef ENABLE_GPU\n");

  fprintf(hdrfile, "\n/*** Function Pointer Table ***/\n\n");
  fprintf(hdrfile, "chpl_fn_p chpl_ftable[] = {\n");
  forv_Vec(FnSymbol, fn, functions) {
    if (fn->hasFlag(FLAG_BEGIN_BLOCK) ||
        fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK) ||
        fn->hasFlag(FLAG_ON_BLOCK)) {
    ftableVec.add(fn);
    ftableMap.put(fn, ftableVec.n-1);
    }
  }

  bool first = true;
  forv_Vec(FnSymbol, fn, ftableVec) {
    if (!first)
      fprintf(hdrfile, ",\n");
    fprintf(hdrfile, "(chpl_fn_p)%s", fn->cname);
    first = false;
  }

  if (ftableVec.n == 0)
    fprintf(hdrfile, "(chpl_fn_p)0");
  fprintf(hdrfile, "\n};\n");

  if (fGPU) {
    fprintf(hdrfile, "#else\n");
    fprintf(hdrfile, "extern chpl_fn_p chpl_ftable[];\n");
    fprintf(hdrfile, "#endif\n");
  }

  fprintf(hdrfile, "\n/*** Virtual Method Table ***/\n\n");
  int maxVMT = 0;
  for (int i = 0; i < virtualMethodTable.n; i++)
    if (virtualMethodTable.v[i].key && virtualMethodTable.v[i].value->n > maxVMT)
      maxVMT = virtualMethodTable.v[i].value->n;
  const char* vmt = "chpl_vmtable";
  fprintf(hdrfile, "chpl_fn_p %s[][%d] = {\n", vmt, maxVMT);
  bool comma = false;
  forv_Vec(TypeSymbol, ts, types) {
    if (ClassType* ct = toClassType(ts->type)) {
      if (!isReferenceType(ct) && isClass(ct)) {
        if (comma)
          fprintf(hdrfile, ",\n");
        fprintf(hdrfile, "{ /* %s */\n", ct->symbol->cname);
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
        fprintf(hdrfile, "}");
        comma = true;
      }
    }
  }
  fprintf(hdrfile, "\n};\n");
  
  if (fGPU)
    fprintf(hdrfile, "\n#ifndef ENABLE_GPU\n");

  fprintf(hdrfile, "\n/*** Global Variables ***/\n\n");
  forv_Vec(VarSymbol, varSymbol, globals) {
    varSymbol->codegenDef(hdrfile);
  }

  fprintf(hdrfile, "\nconst int chpl_numGlobalsOnHeap = %d;\n", numGlobalsOnHeap);
  fprintf(hdrfile, "\nvoid** chpl_globals_registry;\n");
  fprintf(hdrfile, "\nvoid* chpl_globals_registry_static[%d];\n", 
          (numGlobalsOnHeap ? numGlobalsOnHeap : 1));
  fprintf(hdrfile, "\nconst int chpl_heterogeneous = ");
  if (fHeterogeneous)
    fprintf(hdrfile, " 1;\n");
  else
    fprintf(hdrfile, " 0;\n");
  fprintf(hdrfile, "\nconst char* chpl_mem_descs[] = {\n");
  first = true;
  forv_Vec(const char*, memDesc, memDescsVec) {
    if (!first)
      fprintf(hdrfile, ",\n");
    fprintf(hdrfile, "\"%s\"", memDesc);
    first = false;
  }
  fprintf(hdrfile, "\n};\n");
  fprintf(hdrfile, "\nconst int chpl_mem_numDescs = %d;\n", memDescsVec.n);

  //
  // add table of private-broadcast constants
  //
  fprintf(hdrfile, "\nvoid* const chpl_private_broadcast_table[] = {\n");
  fprintf(hdrfile, "&chpl_verbose_comm");
  fprintf(hdrfile, ",\n&chpl_comm_diagnostics");
  fprintf(hdrfile, ",\n&chpl_verbose_mem");
  int i = 3;
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      fprintf(hdrfile, ",\n&%s", se->var->cname);
      call->insertAtHead(new_IntSymbol(i));
      i++;
    }
  }
  fprintf(hdrfile, "\n};\n");

  if (fGPU) {
    fprintf(hdrfile, "#else\n");
    forv_Vec(VarSymbol, varSymbol, globals) {
      fprintf(hdrfile,"extern ");
      varSymbol->codegenDef(hdrfile);
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


static void
codegen_config(FILE* outfile) {
  fprintf(outfile, "#include \"_config.c\"\n");
  fileinfo configFile;
  openCFile(&configFile, "_config.c");
  outfile = configFile.fptr;

  fprintf(outfile, "#include \"error.h\"\n\n");

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
}


void codegen(void) {
  if (no_codegen)
    return;

  fileinfo hdrfile, mainfile, gpusrcfile;
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

  // This dumps the generated sources into the build directory.
  codegen_header(hdrfile.fptr);

  codegen_config(mainfile.fptr);

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
    currentModule->codegenDef(modulefile.fptr);
    closeCFile(&modulefile);
    fprintf(mainfile.fptr, "#include \"%s%s\"\n", filename, ".c");
  }

  if (fHeterogeneous) 
    codegenTypeStructures(hdrfile.fptr);

  closeCFile(&hdrfile);
  closeCFile(&mainfile);
}
