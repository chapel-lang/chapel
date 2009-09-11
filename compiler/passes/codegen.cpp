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


static int max(int a, int b) {
  return (a >= b) ? a : b;
}


static void setOrder(Map<ClassType*,int>& order, int& maxOrder, ClassType* ct);

static int
getOrder(Map<ClassType*,int>& order, int& maxOrder,
         ClassType* ct, Vec<ClassType*>& visit) {
  if (visit.set_in(ct))
    return 0;
  visit.set_add(ct);
  if (order.get(ct))
    return order.get(ct);
  int i = 0;
  for_fields(field, ct) {
    if (ClassType* fct = toClassType(field->type)) {
      if (!visit.set_in(fct)) {
        if (!isClass(fct) || fct->symbol->hasFlag(FLAG_REF)) {
          setOrder(order, maxOrder, fct);
          i = max(i, getOrder(order, maxOrder, fct, visit));
        }
      }
    }
  }
  return i + !isClass(ct);
}


static void
setOrder(Map<ClassType*,int>& order, int& maxOrder,
         ClassType* ct) {
  if (order.get(ct) || (isClass(ct) && !ct->symbol->hasFlag(FLAG_REF)))
    return;
  int i;
  Vec<ClassType*> visit;
  i = getOrder(order, maxOrder, ct, visit);
  order.put(ct, i);
  if (i > maxOrder)
    maxOrder = i;
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

static void legalizeCName(Symbol* sym) {
  if (sym->hasFlag(FLAG_EXTERN))
    return;
  for (const char* ch = sym->cname; *ch != '\0'; ch++) {
    switch (*ch) {
    case '>': ch = subChar(sym, ch, "_GREATER_"); break;
    case '<': ch = subChar(sym, ch, "_LESS_"); break;
    case '=': ch = subChar(sym, ch, "_EQUAL_"); break;
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
    default: break;
    }
  }
  if ((!strncmp("chpl_", sym->cname, 5) && strcmp("chpl_main", sym->cname) && sym->cname[5] != '_') ||
      (sym->cname[0] == '_' && (sym->cname[1] == '_' || (sym->cname[1] >= 'A' && sym->cname[1] <= 'Z')))) {
    sym->cname = astr("chpl__", sym->cname);
  }
}


static void
genClassTagEnum(FILE* outfile, Vec<TypeSymbol*> typeSymbols) {
  fprintf(outfile, "/*** Class Type Identification Numbers ***/\n\n");
  fprintf(outfile, "typedef enum {\n");
  bool comma = false;
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ClassType* ct = toClassType(ts->type)) {
      if (!isReferenceType(ct) && isClass(ct)) {
        fprintf(outfile, "%schpl__cid_%s", (comma) ? ",\n  " : "  ", ts->cname);
        comma = true;
      }
    }
  }
  if (!comma)
    fprintf(outfile, "  chpl__cid_placeholder");
  fprintf(outfile, "\n} chpl__class_id;\n\n");
}


static void codegen_header(FILE* hdrfile, FILE* codefile=NULL) {
  ChainHashMap<const char*, StringHashFns, int> cnames;
  Vec<TypeSymbol*> typeSymbols;
  Vec<FnSymbol*> fnSymbols;
  Vec<VarSymbol*> varSymbols;

  // reserved symbol names that require renaming to compile
#include "reservedSymbolNames.h"

  //
  // change enum constant names into EnumTypeName__constantName
  //
  forv_Vec(EnumType, enumType, gEnumTypes) {
    const char* enumName = enumType->symbol->cname;
    for_enums(constant, enumType) {
      Symbol* sym = constant->sym;
      legalizeCName(sym);
      sym->cname = astr(enumName, "__", sym->cname);
      if (cnames.get(sym->cname))
        sym->cname = astr("chpl__", sym->cname, "_", istr(sym->id));
      cnames.put(sym->cname, 1);
    }
  }

  //
  // mangle type names if they clash with other types
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      legalizeCName(ts);
      if (!ts->hasFlag(FLAG_EXTERN) && cnames.get(ts->cname))
        ts->cname = astr("chpl__", ts->cname, "_", istr(ts->id));
      cnames.put(ts->cname, 1);
      typeSymbols.add(ts);
    }
  }

  //
  // mangle field names if they clash with types or other fields in
  // the same class
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      if (ClassType* ct = toClassType(ts->type)) {
        Vec<const char*> fieldSet;
        for_fields(field, ct) {
          legalizeCName(field);
          if (fieldSet.set_in(field->cname))
            field->cname = astr(field->cname, "_", istr(field->id));
          fieldSet.set_add(field->cname);
        }
      }
    }
  }

  //
  // mangle global variable names if they clash with types or other
  // global variables
  //
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->defPoint->parentExpr != rootModule->block &&
        toModuleSymbol(var->defPoint->parentSymbol)) {
      legalizeCName(var);
      if (!var->hasFlag(FLAG_EXTERN) && cnames.get(var->cname))
        var->cname = astr("chpl__", var->cname, "_", istr(var->id));
      cnames.put(var->cname, 1);
      varSymbols.add(var);
    }
  }

  //
  // mangle function names if they clash with types, global variables,
  // or other functions
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn == chpl_main)
      fn->cname = astr("chpl_main");
    legalizeCName(fn);
    if (!fn->hasFlag(FLAG_EXPORT) && !fn->hasFlag(FLAG_EXTERN) && cnames.get(fn->cname))
      fn->cname = astr("chpl__", fn->cname, "_", istr(fn->id));
    cnames.put(fn->cname, 1);
    fnSymbols.add(fn);
  }

  //
  // mangle formal argument names if they clash with types, global
  // variables, functions, or earlier formal arguments in the same
  // function
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Vec<const char*> formalSet;
    for_formals(formal, fn) {
      legalizeCName(formal);
      if (cnames.get(formal->cname) || formalSet.set_in(formal->cname))
        formal->cname = astr("chpl__", formal->cname, "_", istr(formal->id));
      formalSet.set_add(formal->cname);
    }
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

    Map<const char*, int> duplicateCounts;
    Vec<DefExpr*> defs;
    collectDefExprs(fn->body, defs);
    forv_Vec(DefExpr, def, defs) {
      legalizeCName(def->sym);
      if (!strcmp(def->sym->cname, "_tmp")) {
        def->sym->cname = astr("T");
      }
      const char* cname = def->sym->cname;
      while (local.set_in(cname) || cnames.get(cname)) {
        char numberTmp[64];
        int count = duplicateCounts.get(def->sym->cname);
        duplicateCounts.put(def->sym->cname, count+1);
        snprintf(numberTmp, 64, "%d", count+1);
        cname = astr(def->sym->cname, numberTmp);
      }
      local.set_add(cname);
      def->sym->cname = cname;
    }
  }

  qsort(varSymbols.v, varSymbols.n, sizeof(varSymbols.v[0]), compareSymbol);
  qsort(typeSymbols.v, typeSymbols.n, sizeof(typeSymbols.v[0]), compareSymbol);
  qsort(fnSymbols.v, fnSymbols.n, sizeof(fnSymbols.v[0]), compareSymbol);

  if (fRuntime) {
    chpl_main->cname = astr("chpl_init_", mainModules.v[0]->name);
    baseModule->initFn->body->replace(new BlockStmt(new CallExpr(PRIM_RETURN, gVoid)));
    fprintf(hdrfile, "#ifndef _%s_H_\n", mainModules.v[0]->name);
    fprintf(hdrfile, "#define _%s_H_\n", mainModules.v[0]->name);
    fprintf(hdrfile, "#include \"stdchplrt.h\"\n");
    forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
      if (fnSymbol->hasFlag(FLAG_EXPORT) || fnSymbol == chpl_main)
        fnSymbol->codegenPrototype(hdrfile);
    }
    fprintf(hdrfile, "#endif\n");
    // For the runtime, generate the rest of the things that normally
    // go into the hdrfile into the codefile
    hdrfile = codefile;
  } else {
    fprintf(hdrfile, "/*** Compilation Info ***/\n\n");
    if (fGPU)
      fprintf(hdrfile, "#ifndef ENABLE_GPU\n");
    fprintf(hdrfile, "const char* chpl_compileCommand     = \"%s\";\n", compileCommand);
    fprintf(hdrfile, "const char* chpl_compileVersion     = \"%s\";\n", compileVersion);
    fprintf(hdrfile, "const char* CHPL_HOST_PLATFORM      = \"%s\";\n", CHPL_HOST_PLATFORM);
    fprintf(hdrfile, "const char* CHPL_TARGET_PLATFORM    = \"%s\";\n", CHPL_TARGET_PLATFORM);
    fprintf(hdrfile, "const char* CHPL_HOST_COMPILER      = \"%s\";\n", CHPL_HOST_COMPILER);
    fprintf(hdrfile, "const char* CHPL_TARGET_COMPILER    = \"%s\";\n", CHPL_TARGET_COMPILER);
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
      fprintf(hdrfile, "extern const char* CHPL_THREADS;\n");
      fprintf(hdrfile, "extern const char* CHPL_COMM;\n");
      fprintf(hdrfile, "#endif\n");
    }

    fprintf(hdrfile, "\n#define CHPL_GEN_CODE\n\n");
  }
  genIncludeCommandLineHeaders(hdrfile);

  genClassTagEnum(hdrfile, typeSymbols);

  if (!fRuntime) {
    fprintf(hdrfile, "#include \"stdchpl.h\"\n");
  }

  fprintf(hdrfile, "\n/*** Class Prototypes ***/\n\n");
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (!typeSymbol->hasFlag(FLAG_REF))
      typeSymbol->codegenPrototype(hdrfile);
  }

  // codegen enumerated types
  fprintf(hdrfile, "\n/*** Enumerated Types ***/\n\n");

  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (toEnumType(typeSymbol->type))
      typeSymbol->codegenDef(hdrfile);
  }

  // codegen reference types
  fprintf(hdrfile, "\n/*** Primitive References ***/\n\n");
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ts->hasFlag(FLAG_REF)) {
      Type* vt = ts->type->getValueType();
      if (isRecord(vt) || isUnion(vt))
        continue; // references to records and unions codegened below
      ts->codegenPrototype(hdrfile);
    }
  }

  // order records/unions topologically
  //   (int, int) before (int, (int, int))
  Map<ClassType*,int> order;
  int maxOrder = 0;
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ClassType* ct = toClassType(ts->type))
      setOrder(order, maxOrder, ct);
  }

  // debug
  //   for (int i = 0; i < order.n; i++) {
  //     if (order.v[i].key && order.v[i].value) {
  //       printf("%d: %s\n", order.v[i].value, order.v[i].key->symbol->name);
  //     }
  //   }
  //   printf("%d\n", maxOrder);

  // codegen records/unions in topological order
  fprintf(hdrfile, "\n/*** Records and Unions (Hierarchically) ***/\n\n");
  for (int i = 1; i <= maxOrder; i++) {
    forv_Vec(TypeSymbol, ts, typeSymbols) {
      if (ClassType* ct = toClassType(ts->type))
        if (order.get(ct) == i && !ct->symbol->hasFlag(FLAG_REF))
          ts->codegenDef(hdrfile);
    }
    forv_Vec(TypeSymbol, ts, typeSymbols) {
      if (ts->hasFlag(FLAG_REF))
        if (ClassType* ct = toClassType(ts->type->getValueType()))
          if (order.get(ct) == i)
            ts->codegenPrototype(hdrfile);
    }
    fprintf(hdrfile, "\n");
  }

  // codegen remaining types
  fprintf(hdrfile, "\n/*** Classes ***/\n\n");
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (isClass(typeSymbol->type) &&
        !typeSymbol->hasFlag(FLAG_REF) &&
        typeSymbol->hasFlag(FLAG_NO_OBJECT) &&
        !typeSymbol->hasFlag(FLAG_OBJECT_CLASS))
      typeSymbol->codegenDef(hdrfile);
  }
  /* Generate class definitions in breadth first order starting with
     "object" and following its dispatch children. */

  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;
  current_p->set_add(dtObject);
  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (toClassType(t)) {
        t->symbol->codegenDef(hdrfile);
        forv_Vec(Type, child, t->dispatchChildren) {
          if (child)
            next_p->set_add(child);
        }
      } else {
        INT_ASSERT(t == NULL);
      }
    }
    Vec<Type*>* temp = next_p;
    next_p = current_p;
    current_p = temp;
    next_p->clear();
  }

  fprintf(hdrfile, "\n/*** Function Prototypes ***/\n\n");
  forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
    if (fnSymbol->hasFlag(FLAG_GPU_ON)) {
      fprintf(hdrfile, "\n#ifdef ENABLE_GPU\n");
      fnSymbol->codegenPrototype(hdrfile);
      fprintf(hdrfile, "#endif\n");
      continue;
    }
    if (!fnSymbol->hasFlag(FLAG_EXTERN)) {
      if (fRuntime && fnSymbol != chpl_main && !fnSymbol->hasFlag(FLAG_EXPORT))
        fprintf(hdrfile, "static ");
      if (!fRuntime || 
          (!fnSymbol->hasFlag(FLAG_EXPORT) && fnSymbol != chpl_main)) {
          fnSymbol->codegenPrototype(hdrfile);
      }
    }
  }
    
  if (!fRuntime) {
    if (fGPU)
      fprintf(hdrfile, "\n#ifndef ENABLE_GPU\n");

    fprintf(hdrfile, "\n/*** Function Pointer Table ***/\n\n");
    fprintf(hdrfile, "chpl_fn_p chpl_ftable[] = {\n");
    int i = 0;
    forv_Vec(FnSymbol, fn, fnSymbols) {
      if (fn->hasFlag(FLAG_BEGIN_BLOCK) ||
          fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK) ||
          fn->hasFlag(FLAG_ON_BLOCK)) {
        if (i > 0)
          fprintf(hdrfile, ",\n");
        ftable.put(fn, i++);
        fprintf(hdrfile, "(chpl_fn_p)%s", fn->cname);
      }
    }
    if (i == 0)
      fprintf(hdrfile, "(chpl_fn_p)0");
    fprintf(hdrfile, "\n};\n");
  
    if (fGPU) {
      fprintf(hdrfile, "#else\n");
      fprintf(hdrfile, "extern chpl_fn_p chpl_ftable[];\n");
      fprintf(hdrfile, "#endif\n");
    }
  }
  
  if (fGPU)
    fprintf(hdrfile, "\n#ifndef ENABLE_GPU\n");

  fprintf(hdrfile, "\n/*** Global Variables ***/\n\n");
  forv_Vec(VarSymbol, varSymbol, varSymbols) {
    if (fRuntime) {
      if (varSymbol->defPoint->parentSymbol == baseModule)
        continue;
      fprintf(hdrfile, "static ");
    }
    varSymbol->codegenDef(hdrfile);
  }

  if (!fRuntime) {
    fprintf(hdrfile, "\nconst int chpl_numGlobalsOnHeap = %d;\n", numGlobalsOnHeap);
    fprintf(hdrfile, "\nvoid** chpl_globals_registry;\n");
    fprintf(hdrfile, "\nvoid* chpl_globals_registry_static[%d];\n", 
            (numGlobalsOnHeap ? numGlobalsOnHeap : 1));
    fprintf(hdrfile, "\nconst char* chpl_memDescs[] = {\n");
    bool first = true;
    forv_Vec(const char*, memDesc, memDescsVec) {
      if (!first)
        fprintf(hdrfile, ",\n");
      fprintf(hdrfile, "\"%s\"", memDesc);
      first = false;
    }
    fprintf(hdrfile, "\n};\n");
    fprintf(hdrfile, "\nconst int chpl_num_memDescs = %d;\n", memDescsVec.n);

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
  }

  if (fGPU) {
    fprintf(hdrfile, "#else\n");
    forv_Vec(VarSymbol, varSymbol, varSymbols) {
      if (fRuntime) {
        if (varSymbol->defPoint->parentSymbol == baseModule)
          continue;
        fprintf(hdrfile, "static ");
      }
      fprintf(hdrfile,"extern ");
      varSymbol->codegenDef(hdrfile);
    }

    if (!fRuntime) {
      fprintf(hdrfile, "\nextern const int chpl_numGlobalsOnHeap;\n");
      fprintf(hdrfile, "\nextern void** chpl_globals_registry;\n");
      fprintf(hdrfile, "\nextern void* chpl_globals_registry_static[];\n");
      fprintf(hdrfile, "\nextern const char* chpl_memDescs[];\n");
      fprintf(hdrfile, "\nextern const int chpl_num_memDescs;\n");
    }
    fprintf(hdrfile, "#endif\n");
  }
}


static void
codegen_config(FILE* outfile) {
  fprintf(outfile, "#include \"_config.c\"\n");
  fileinfo configFile;
  openCFile(&configFile, "_config.c");
  outfile = configFile.fptr;

  fprintf(outfile, "void CreateConfigVarTable(void) {\n");
  fprintf(outfile, "initConfigVarTable();\n");

  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->hasFlag(FLAG_CONFIG)) {
      fprintf(outfile, "installConfigVar(\"%s\", \"", var->name);
      Type* type = var->type;
      if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
        type = type->getField("addr")->type;
      if (type->symbol->hasFlag(FLAG_HEAP))
        type = type->getField("value")->type;
      if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
        type = type->getField("addr")->type;
      fprintf(outfile, "%s", type->symbol->name);
      fprintf(outfile, "\", \"%s\");\n", var->getModule()->name);
    }
  }

  fprintf(outfile, "}\n\n\n");

  int numRealms = getNumRealms();
  fprintf(outfile, "int32_t chpl_numRealms = %d;\n\n\n", numRealms);

  fprintf(outfile, "const char* chpl_realmType(int32_t r) {\n");
  fprintf(outfile, "  switch (r) {\n");
  for (int i=0; i<numRealms; i++) {
    fprintf(outfile, "    case %d: return \"%s\"; break;\n", i, getRealmType(i));
  }
  fprintf(outfile, "    default:\n");
  fprintf(outfile, "      chpl_internal_error(\"attempt to query realm other than 0-%d\\n\");\n", numRealms-1);
  fprintf(outfile, "      return \"unknown\";\n");
  fprintf(outfile, "  }\n");
  fprintf(outfile, "}\n");

  closeCFile(&configFile);
}


void codegen(void) {
  if (no_codegen)
    return;

  fileinfo hdrfile, mainfile, runtimeheader, runtimecode, gpusrcfile;
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

  if (fRuntime) {
    openCFile(&runtimeheader, mainModules.v[0]->name, "h", true);
    openCFile(&runtimecode, mainModules.v[0]->name, "c", true);
    fprintf(runtimecode.fptr, "#include \"chapel_code.h\"\n\n");
  }
  if (fRuntime)
    codegen_header(runtimeheader.fptr, runtimecode.fptr);
  else
    codegen_header(hdrfile.fptr);

  if (!fRuntime)
    codegen_config(mainfile.fptr);

  if (genCommunicatedStructures)
    codegenTypeStructureInclude(mainfile.fptr);

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
    if (fRuntime) {
      currentModule->codegenDef(runtimecode.fptr);
    } else {
      currentModule->codegenDef(modulefile.fptr);
    }
    closeCFile(&modulefile);
    fprintf(mainfile.fptr, "#include \"%s%s\"\n", filename, ".c");
  }

  if (genCommunicatedStructures) 
    codegenTypeStructures(hdrfile.fptr);

  if (fRuntime) {
    closeCFile(&runtimeheader);
    closeCFile(&runtimecode);
  }

  closeCFile(&hdrfile);
  closeCFile(&mainfile);
}
