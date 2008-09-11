#include <cctype>
#include <cstring>
#include <cstdio>
#include "astutil.h"
#include "beautify.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "mysystem.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static fileinfo runtimeFile;

static int max(int a, int b) {
  return (a >= b) ? a : b;
}

static void setOrder(Map<ClassType*,int>& order, int& maxOrder, ClassType* ct);
static void codegen_cid2offsets_help(FILE* outfile, ClassType* ct);

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
        if (fct->classTag != CLASS_CLASS || fct->symbol->hasFlag(FLAG_REF)) {
          setOrder(order, maxOrder, fct);
          i = max(i, getOrder(order, maxOrder, fct, visit));
        }
      }
    }
  }
  return i + (ct->classTag != CLASS_CLASS);
}


static void
setOrder(Map<ClassType*,int>& order, int& maxOrder,
         ClassType* ct) {
  if (order.get(ct) || (ct->classTag == CLASS_CLASS &&
                        !ct->symbol->hasFlag(FLAG_REF)))
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
}

static inline bool symbol_clashes(const char *symbol_name,
                                  ChainHashMap<const char*, StringHashFns, int> symbol_map) {
  if (!strncmp("chpl_", symbol_name, 5) && strcmp("chpl_main", symbol_name) && symbol_name[5] != '_' ||
      symbol_name[0] == '_' &&
      symbol_name[1] == '_' || symbol_name[1] >= 'A' && symbol_name[1] <= 'Z')
    return true;
  else
    return symbol_map.get(symbol_name);
}

static void codegen_header(void) {
  ChainHashMap<const char*, StringHashFns, int> cnames;
  Vec<TypeSymbol*> typeSymbols;
  Vec<FnSymbol*> fnSymbols;
  Vec<VarSymbol*> varSymbols;

  // reserved symbol names that require renaming to compile
#include "reservedSymbolNames.h"

  //
  // change enum constant names into EnumTypeName__constantName
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (EnumType* enumType = toEnumType(ast)) {
      const char* enumName = enumType->symbol->cname;
      for_enums(constant, enumType) {
        Symbol* sym = constant->sym;
        sym->cname = astr(enumName, "__", sym->cname);
        if (symbol_clashes(sym->cname, cnames))
          sym->cname = astr("chpl__", sym->cname, "_", istr(sym->id));
        cnames.put(sym->cname, 1);
      }
    }
  }


  //
  // mangle type names if they clash with other types
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (ts->defPoint->parentExpr != rootModule->block) {
      legalizeCName(ts);
      if (!ts->hasFlag(FLAG_EXTERN) && symbol_clashes(ts->cname, cnames))
        ts->cname = astr("chpl__", ts->cname, "_", istr(ts->id));
      cnames.put(ts->cname, 1);
      typeSymbols.add(ts);
    }
  }

  //
  // mangle field names if they clash with types or other fields in
  // the same class
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
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
  forv_Vec(BaseAST, ast, gAsts) {
    if (VarSymbol* var = toVarSymbol(ast)) {
      if (var->defPoint->parentExpr != rootModule->block &&
          toModuleSymbol(var->defPoint->parentSymbol)) {
        legalizeCName(var);
        if (!var->hasFlag(FLAG_EXTERN) && symbol_clashes(var->cname, cnames))
          var->cname = astr("chpl__", var->cname, "_", istr(var->id));
        cnames.put(var->cname, 1);
        varSymbols.add(var);
      }
    }
  }

  //
  // mangle function names if they clash with types, global variables,
  // or other functions
  //
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn == chpl_main)
      fn->cname = astr("chpl_main");
    legalizeCName(fn);
    if (!fn->hasFlag(FLAG_EXPORT) && !fn->hasFlag(FLAG_EXTERN) && symbol_clashes(fn->cname, cnames))
      fn->cname = astr("chpl__", fn->cname, "_", istr(fn->id));
    cnames.put(fn->cname, 1);
    fnSymbols.add(fn);
  }

  //
  // mangle formal argument names if they clash with types, global
  // variables, functions, or earlier formal arguments in the same
  // function
  //
  forv_Vec(FnSymbol, fn, gFns) {
    Vec<const char*> formalSet;
    for_formals(formal, fn) {
      legalizeCName(formal);
      if (symbol_clashes(formal->cname, cnames) || formalSet.set_in(formal->cname))
        formal->cname = astr("chpl__", formal->cname, "_", istr(formal->id));
      formalSet.set_add(formal->cname);
    }
  }

  //
  // mangle local variable names if they clash with types, global
  // variables, functions, formal arguments of their function, or
  // other local variables in the same function; shorten temp names
  //
  forv_Vec(FnSymbol, fn, gFns) {
    Vec<const char*> local;

    for_formals(formal, fn) {
      local.set_add(formal->cname);
    }

    Vec<BaseAST*> asts;
    collect_asts(fn->body, asts);
    int tmpID = 1;
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast)) {
        if (def->sym->hasFlag(FLAG_TEMP)) {
          do {
            def->sym->cname = astr("T", istr(tmpID++));
          } while (local.set_in(def->sym->cname) || symbol_clashes(def->sym->cname, cnames));
          local.set_add(def->sym->cname);
        }
      }
    }
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast)) {
        if (!def->sym->hasFlag(FLAG_TEMP)) {
          legalizeCName(def->sym);
          const char* cname;
          do {
            cname = astr("T", istr(tmpID++), "_", def->sym->cname);
          } while (local.set_in(cname) || symbol_clashes(cname, cnames));
          def->sym->cname = cname;
          local.set_add(cname);
        }
      }
    }
  }

  qsort(varSymbols.v, varSymbols.n, sizeof(varSymbols.v[0]), compareSymbol);
  qsort(typeSymbols.v, typeSymbols.n, sizeof(typeSymbols.v[0]), compareSymbol);
  qsort(fnSymbols.v, fnSymbols.n, sizeof(fnSymbols.v[0]), compareSymbol);

  fileinfo header;
  FILE* outfile;
  if (fRuntime) {
    chpl_main->cname = astr("chpl_init_", userModules.v[0]->name);
    baseModule->initFn->body->replace(new BlockStmt(new CallExpr(PRIMITIVE_RETURN, gVoid)));
    fileinfo runtimeHeaderFile;
    openRuntimeFile(&runtimeHeaderFile, userModules.v[0]->name, "h");
    outfile = runtimeHeaderFile.fptr;
    fprintf(outfile, "#ifndef _%s_H_\n", userModules.v[0]->name);
    fprintf(outfile, "#define _%s_H_\n", userModules.v[0]->name);
    fprintf(outfile, "#include \"stdchplrt.h\"\n");
    forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
      if (fnSymbol->hasFlag(FLAG_EXPORT) || fnSymbol == chpl_main)
        fnSymbol->codegenPrototype(outfile);
    }
    fprintf(outfile, "#endif\n");
    closeCFile(&runtimeHeaderFile);
    beautify(&runtimeHeaderFile);
    openRuntimeFile(&runtimeFile, userModules.v[0]->name, "c");
    outfile = runtimeFile.fptr;
    fprintf(outfile, "#include \"chapel_code.h\"\n");
  } else {
    openCFile(&header, "_chpl_header", "h");
    outfile = header.fptr;
  }
  genIncludeCommandLineHeaders(outfile);

  fprintf(outfile, "\n/*** Class ID -- referred to in runtime ***/\n\n");
  fprintf(outfile, "typedef enum {\n");
  bool comma = false;
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ClassType* ct = toClassType(ts->type)) {
      if (!isReference(ct) && ct->classTag == CLASS_CLASS) {
        fprintf(outfile, "%s_e_%s", (comma) ? ",\n  " : "  ", ts->cname);
        comma = true;
      }
    }
  }
  if (!comma)
    fprintf(outfile, "  _e_placeholder");
  fprintf(outfile, "\n} _class_id;\n\n");

  if (!fRuntime) {
    fprintf(outfile, "#define CHPL_GEN_CODE\n");
    fprintf(outfile, "#include \"stdchpl.h\"\n");
  }

  fprintf(outfile, "\n/*** Class Prototypes ***/\n\n");
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (!typeSymbol->hasFlag(FLAG_REF))
      typeSymbol->codegenPrototype(outfile);
  }

  // codegen enumerated types
  fprintf(outfile, "\n/*** Enumerated Types ***/\n\n");

  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (toEnumType(typeSymbol->type))
      typeSymbol->codegenDef(outfile);
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

  // codegen reference types
  fprintf(outfile, "/*** Primitive References ***/\n\n");
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ts->hasFlag(FLAG_REF)) {
      ClassType* ct = toClassType(getValueType(ts->type));
      if (ct && ct->classTag != CLASS_CLASS)
        continue; // references to records and unions codegened below
      ts->codegenPrototype(outfile);
    }
  }

  // codegen records/unions in topological order
  fprintf(outfile, "\n/*** Records and Unions (Hierarchically) ***/\n\n");
  for (int i = 1; i <= maxOrder; i++) {
    forv_Vec(TypeSymbol, ts, typeSymbols) {
      if (ClassType* ct = toClassType(ts->type))
        if (order.get(ct) == i && !ct->symbol->hasFlag(FLAG_REF))
          ts->codegenDef(outfile);
    }
    forv_Vec(TypeSymbol, ts, typeSymbols) {
      if (ts->hasFlag(FLAG_REF))
        if (ClassType* ct = toClassType(getValueType(ts->type)))
          if (order.get(ct) == i)
            ts->codegenPrototype(outfile);
    }
    fprintf(outfile, "\n");
  }

  // codegen remaining types
  fprintf(outfile, "\n/*** Classes ***/\n\n");
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    ClassType* ct = toClassType(typeSymbol->type);
    if (!ct || ct->classTag != CLASS_CLASS)
      continue;
    if (toEnumType(typeSymbol->type))
      continue;
    if (typeSymbol->hasFlag(FLAG_REF))
      continue;

    if (ClassType* ct = toClassType(typeSymbol->type))
      if (!ct->symbol->hasFlag(FLAG_NO_OBJECT) || ct->symbol->hasFlag(FLAG_OBJECT_CLASS))
        continue;

    typeSymbol->codegenDef(outfile);
  }
  /* Generate class definitions in breadth first order starting with
     "object" and following its dispatch children. */

  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;
  current_p->set_add(dtObject);
  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (toClassType(t)) {
        t->symbol->codegenDef(outfile);
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

  fprintf(outfile, "\n/*** Function Prototypes ***/\n\n");
  forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
    if (!fnSymbol->hasFlag(FLAG_EXTERN)) {
      if (fRuntime && fnSymbol != chpl_main && !fnSymbol->hasFlag(FLAG_EXPORT))
        fprintf(outfile, "static ");
      if (!fRuntime || 
          (!fnSymbol->hasFlag(FLAG_EXPORT) && fnSymbol != chpl_main))
        fnSymbol->codegenPrototype(outfile);
    }
  }

  fprintf(outfile, "\n/*** Global Variables ***/\n\n");
  forv_Vec(VarSymbol, varSymbol, varSymbols) {
    if (fRuntime) {
      if (varSymbol->defPoint->parentSymbol == baseModule)
        continue;
      fprintf(outfile, "static ");
    }
    varSymbol->codegenDef(outfile);
  }

  if (!fRuntime) {
    fprintf(outfile, "\nchar** _global_vars_registry;\n");
    fprintf(outfile, "\nchar* _global_vars_registry_static[%d];\n", 
            (numGlobalsOnHeap ? numGlobalsOnHeap : 1));
  }

  if (!fRuntime) {
    closeCFile(&header);
    beautify(&header);
  }
}

static void codegen_cid2offsets_help(FILE* outfile, ClassType* ct) {
  for_fields(field, ct) {
    if (ClassType* innerct = toClassType(field->type)) {
      if (!innerct->symbol->hasFlag(FLAG_REF) &&
          !innerct->symbol->hasFlag(FLAG_NO_OBJECT)) {
        if (innerct->classTag == CLASS_CLASS) {
          if (field->hasFlag(FLAG_SUPER_CLASS)) {
            codegen_cid2offsets_help(outfile, innerct);
          } else {
            fprintf(outfile, "(size_t)&((_");
            ct->symbol->codegen(outfile);
            fprintf(outfile, "*)NULL)->");
            field->codegen(outfile);
            fprintf(outfile, ", ");
          }
        }
      }
    }
  }
}


static void
codegen_cid2offsets(FILE* outfile) {
  // Generate one array for each Class.  Initialize it to contain the offsets
  // of each class field that is a pointer to a class.  The first location
  // in the array is the size of the class. Then, generate a function
  // cid2offsets which takes a cid and returns a pointer to the array
  // corresponding to the class with that cid.
  forv_Vec(TypeSymbol, typeSym, gTypes) {
    if (ClassType* ct = toClassType(typeSym->type)) {
      if (ct->classTag == CLASS_CLASS && !ct->symbol->hasFlag(FLAG_REF) && !ct->symbol->hasFlag(FLAG_NO_OBJECT)) {
        fprintf(outfile, "size_t _");
        ct->symbol->codegen(outfile);
        fprintf(outfile, "_offsets[] = { sizeof(_");
        ct->symbol->codegen(outfile);
        fprintf(outfile, "), ");
        if (typeSym->hasFlag(FLAG_DATA_CLASS)) {
          if (ClassType* elementType = toClassType(ct->substitutions.v[0].value)) {
            if (elementType->classTag == CLASS_CLASS) {
              fprintf(outfile, "(size_t)-1,"); // Special token for arrays of classes
              fprintf(outfile, "(size_t)&((_");
              ct->symbol->codegen(outfile);
              fprintf(outfile, "*)0)->size,");
              fprintf(outfile, "(size_t)&((_");
              ct->symbol->codegen(outfile);
              fprintf(outfile, "*)0)->_data,");
            }
          }
        }
        codegen_cid2offsets_help(outfile, ct);
        fprintf(outfile, "0 };\n");
      }
    }
  }

  fprintf(outfile, "size_t* cid2offsets(_class_id cid) {\n");
  fprintf(outfile, "size_t* offsets = NULL;\n");
  fprintf(outfile, "switch(cid) {\n");
  forv_Vec(TypeSymbol, typeSym, gTypes) {
    if (ClassType* ct = toClassType(typeSym->type)) {
      if (ct->classTag == CLASS_CLASS && !isReference(ct) && !ct->symbol->hasFlag(FLAG_NO_OBJECT)) {
        fprintf(outfile, "case %s%s:\n", "_e_", ct->symbol->cname);
        fprintf(outfile, "offsets = _");
        ct->symbol->codegen(outfile);
        fprintf(outfile, "_offsets;\n");
        fprintf(outfile, "break;\n");
      }
    }
  }
  fprintf(outfile, "default:\n");
  fprintf(outfile, "chpl_error(\"Bad cid in cid2offsets\", 0, 0);\n");
  fprintf(outfile, "break;\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "return offsets;\n");
  fprintf(outfile, "}\n");
}


static void
codegen_cid2size(FILE* outfile) {
  fprintf(outfile, "size_t cid2size(_class_id cid) {\n");
  fprintf(outfile, "size_t size = 0;\n");
  fprintf(outfile, "switch(cid) {\n");
  forv_Vec(TypeSymbol, typeSym, gTypes) {
    if (ClassType* ct = toClassType(typeSym->type)) {
      if (ct->classTag == CLASS_CLASS && !isReference(ct) && !ct->symbol->hasFlag(FLAG_NO_OBJECT)) {
        fprintf(outfile, "case %s%s:\n", "_e_", ct->symbol->cname);
        fprintf(outfile, "size = sizeof(_");
        ct->symbol->codegen(outfile);
        fprintf(outfile, ");\n");
        fprintf(outfile, "break;\n");
      }
    }
  }
  fprintf(outfile, "default:\n");
  fprintf(outfile, "chpl_error(\"Bad cid in cid2size\", 0, 0);\n");
  fprintf(outfile, "break;\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "return size;\n");
  fprintf(outfile, "}\n");
}


static void
codegen_config(FILE* outfile) {
  fprintf(outfile, "#include \"_config.c\"\n");
  fileinfo configFile;
  openCFile(&configFile, "_config.c");
  outfile = configFile.fptr;

  fprintf(outfile, "void CreateConfigVarTable(void) {\n");
  fprintf(outfile, "initConfigVarTable();\n");

  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {
      VarSymbol* var = toVarSymbol(def->sym);
      if (var && var->hasFlag(FLAG_CONFIG)) {
        fprintf(outfile, "installConfigVar(\"%s\", \"", var->name);
        Type* type = var->type;
        if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
          type = type->getField("addr")->type;
        if (type->symbol->hasFlag(FLAG_HEAP))
          type = type->getField("_val")->type;
        if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
          type = type->getField("addr")->type;
        fprintf(outfile, type->symbol->name);
        fprintf(outfile, "\", \"%s\");\n", var->getModule()->name);
      }
    }
  }

  fprintf(outfile, "parseConfigArgs();\n");

  fprintf(outfile, "}\n");

  closeCFile(&configFile);
  beautify(&configFile);
}


void codegen(void) {
  if (no_codegen)
    return;

  fileinfo mainfile;
  openCFile(&mainfile, "_main", "c");
  fprintf(mainfile.fptr, "#include \"_chpl_header.h\"\n");

  codegen_makefile(&mainfile);

  codegen_header();

  codegen_config(mainfile.fptr);

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
    sprintf(filename, lowerFilename);
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
      currentModule->codegenDef(runtimeFile.fptr);
    } else {
      currentModule->codegenDef(modulefile.fptr);
    }
    closeCFile(&modulefile);
    fprintf(mainfile.fptr, "#include \"%s%s\"\n", filename, ".c");
    beautify(&modulefile);
  }

  if (fRuntime) {
    closeCFile(&runtimeFile);
    beautify(&runtimeFile);
  }

  fileinfo memoryfile;
  openCFile(&memoryfile, "_memory_management", "c");
  fprintf(mainfile.fptr, "#include \"%s\"\n", memoryfile.filename);

  codegen_cid2offsets(memoryfile.fptr);
  codegen_cid2size(memoryfile.fptr);
  closeCFile(&memoryfile);
  beautify(&memoryfile);
  closeCFile(&mainfile);
  beautify(&mainfile);
}
