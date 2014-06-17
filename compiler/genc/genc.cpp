//
//    GENC.CPP -- Code generator (C back-end)
//
//        Top-level
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

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

#include "genc.h"

//
//    GenC
//

// construction/destruction

  GenC::GenC() {
      cfile = NULL;
      lineno = -1;
      filename = NULL;
      parseOnly = false;

      gMaxVMT = -1;

      fMaxCIdentLen = 0;
      longCNameReplacementBuffer = NULL;
      
      argLen = 0;
      }
      
  GenC::~GenC() { }
      
// interface: codegen      
      
  void GenC::codegen() {
      SET_LINENO(rootModule);

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
      cfile = hdrfile.fptr;
      codegen_header();

      cfile = mainfile.fptr;
      codegen_config();

      if (fHeterogeneous) {
          codegenTypeStructureInclude(mainfile.fptr);
          forv_Vec(TypeSymbol, ts, gTypeSymbols) {
              if ((ts->type != dtOpaque) &&
                      (!toPrimitiveType(ts->type) ||
                          (toPrimitiveType(ts->type) &&
                              !toPrimitiveType(ts->type)->isInternalType)))
                  registerTypeToStructurallyCodegen(ts);
              }
          }

      ChainHashMap<char *, StringHashFns, int> filenames;
      forv_Vec(ModuleSymbol, currentModule, allModules) {
          mysystem(
              astr("# codegen-ing module", currentModule->name),
              "generating comment for --print-commands option");

        // Macs are case-insensitive when it comes to files, so
        // the following bit of code creates a unique filename
        // with case-insensitivity taken into account

        // create the lowercase filename
          char lowerFilename[FILENAME_MAX];
          sprintf(lowerFilename, "%s", currentModule->name);
          for (unsigned int i = 0; i < strlen(lowerFilename); i++)
              lowerFilename[i] = tolower(lowerFilename[i]);

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
          if (version == 1)
              sprintf(filename, "%s", currentModule->name);
          else
              sprintf(filename, "%s%d", currentModule->name, version);

          fileinfo modulefile;
          openCFile(&modulefile, filename, "c");
          cfile = modulefile.fptr;

          Symbol_codegenDef(currentModule);
          closeCFile(&modulefile);
          fprintf(mainfile.fptr, "#include \"%s%s\"\n", filename, ".c");
          }

      if (fHeterogeneous) 
          codegenTypeStructures(hdrfile.fptr);

      cfile = hdrfile.fptr;
      codegen_header_addons();

      closeCFile(&hdrfile);
      closeCFile(&mainfile);
      }
      
  void GenC::genComment(const char *comment, bool push/*=false*/) {
      if (push) {
          std::string str = comment;
          writeStatement("/*** "+str+" ***/ ");
          }
      else
          writeC("/*** %s ***/\n\n", comment);
      }
      
  void GenC::genIdComment(int id) {
      writeC("/* %7d */ ", id);
      }
      
  void GenC::flushStatements() {
      size_t i;
      for (i = 0; i < cLocalDecls.size(); ++i)
          fprintf(cfile, "%s;\n", cLocalDecls[i].c_str());
      for (i = 0; i < cStatements.size(); ++i)
          fprintf(cfile, "%s", cStatements[i].c_str());
      cLocalDecls.clear();
      cStatements.clear();
      }
            
  std::string GenC::numToString(int64_t num) {
      char name[32];
      sprintf(name, "%"PRId64, num);
      return std::string(name);
      }

  std::string GenC::int64_to_string(int64_t i) {
      char buf[32];
      sprintf(buf, "%"PRId64, i);
      std::string ret(buf);
      return ret;
      }

  std::string GenC::uint64_to_string(uint64_t i) {
      char buf[32];
      sprintf(buf, "%"PRIu64, i);
      std::string ret(buf);
      return ret;
      }
      
  void GenC::writeC(const char *fmt, ...) {
      va_list arg;
      va_start(arg, fmt);
      vfprintf(cfile, fmt, arg);
      va_end(arg);
      }

// codegen list. Separator only used for C codegenning.
  GenRet GenC::AList_codegen(AList *self, const char *separator/*=", "*/) {
    // C version
      GenRet ret;
      for_alist(node, *self) {
          ret.c += Expr_codegen(node).c;
          if (node->next != self->tail)
              ret.c += separator;
          }
      return ret;
      }

  GenRet GenC::Expr_codegen(Expr *self) {
      switch (self->astTag) {
      case E_SymExpr:
          return SymExpr_codegen((SymExpr *)self);
      case E_UnresolvedSymExpr:
          return UnresolvedSymExpr_codegen((UnresolvedSymExpr *)self);
      case E_DefExpr:
          return DefExpr_codegen((DefExpr *)self);
      case E_CallExpr:
          return CallExpr_codegen((CallExpr *)self);
      case E_NamedExpr:
          return NamedExpr_codegen((NamedExpr *)self);
      case E_BlockStmt:
          return BlockStmt_codegen((BlockStmt *)self);
      case E_CondStmt:
          return CondStmt_codegen((CondStmt *)self);
      case E_GotoStmt:
          return GotoStmt_codegen((GotoStmt *)self);
      case E_ExternBlockStmt:
          return ExternBlockStmt_codegen((ExternBlockStmt *)self);
      default:
          {
              INT_FATAL(self, "Unexpected call to Expr::codegen");
              GenRet ret;
              return ret;
              }      
          }
      }

// implementation: codegen

  void GenC::codegen_header() {
      Vec<const char *> cnames;
      Vec<TypeSymbol *> types;
      Vec<FnSymbol *> functions;
      Vec<VarSymbol *> globals;

    // reserved symbol names that require renaming to compile
      reservedSymbolNames(cnames);

      collectTypes(types);
      collectGlobals(globals);
      collectFunctions(functions);
      
      uniquifyTypeNames(types, cnames);
      uniquifyEnumNames(types, cnames);
      uniquifyFieldNames(types, cnames);
      uniquifyGlobalNames(globals, cnames);
      uniquifyFunctionNames(functions, cnames);
      uniquifyFormalNames(cnames); 
      uniquifyLocalNames(cnames); 

      genCompilationInfo();
      genIncludes();      

      genClassIDs(types);

      genClassPrototypes(types);
      genEnumeratedTypes(types);
      genAggregates(types);
      genOtherClasses(types);  
      genClassDefs();    

      genFunctionPrototypes(functions);
      genFunctionPointerTable(functions);
      genVirtualMethodTable(types);
      genGlobalVariables(globals);
      }

// Sometimes we have to define a type while code generating.
// When that happens, we need to add a little bit to the header...
// This is only needed for C (since in LLVM we must add
//  the types as we use them).
  void GenC::codegen_header_addons() {
      forv_Vec(TypeSymbol, ts, gTypeSymbols) {
          if (ts->defPoint->parentExpr != rootModule->block) {
              if (ClassType *ct = toClassType(ts->type))
                  codegen_aggregate_def(ct);
              }
          }
      }
      
  void GenC::codegen_config() {
      writeC("#include \"_config.c\"\n");
      
      FILE *mainfile = cfile;
      fileinfo configFile;
      openCFile(&configFile, "_config.c");
      cfile = configFile.fptr;

      writeC("#include \"error.h\"\n\n");

      genGlobalInt("mainHasArgs", mainHasArgs);

      writeC("void CreateConfigVarTable(void) {\n");
      writeC("initConfigVarTable();\n");

      forv_Vec(VarSymbol, var, gVarSymbols) {
          if (var->hasFlag(FLAG_CONFIG) && !var->hasFlag(FLAG_TYPE_VARIABLE)) {
              writeC("installConfigVar(\"%s\", \"", var->name);
              Type *type = var->type;
              if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
                  type = type->getField("addr")->type;
              if (type->symbol->hasFlag(FLAG_HEAP))
                  type = type->getField("value")->type;
              if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
                  type = type->getField("addr")->type;
              writeC("%s", type->symbol->name);
              if (var->getModule()->modTag == MOD_INTERNAL)
                  writeC("\", \"Built-in\");\n");
              else
                  writeC("\", \"%s\");\n", var->getModule()->name);
              }
          }

      writeC("}\n\n\n");

      closeCFile(&configFile);
      cfile = mainfile;
      }
      
// implementation: header (target-independent)

//
// collect types and apply canonical sort
//
  void GenC::collectTypes(Vec<TypeSymbol *> &types) {
      forv_Vec(TypeSymbol, ts, gTypeSymbols) {
          if (ts->defPoint->parentExpr != rootModule->block) {
              legalizeName(ts);
              types.add(ts);
              }
          }
      qsort(types.v, types.n, sizeof(types.v[0]), compareSymbol);
      }

//
// collect globals and apply canonical sort
//
  void GenC::collectGlobals(Vec<VarSymbol *> &globals) {
      forv_Vec(VarSymbol, var, gVarSymbols) {
          if (var->defPoint->parentExpr != rootModule->block &&
                  toModuleSymbol(var->defPoint->parentSymbol)) {
              legalizeName(var);
              globals.add(var);
              }
          }
      qsort(globals.v, globals.n, sizeof(globals.v[0]), compareSymbol);
      }

//
// collect functions and apply canonical sort
//
  void GenC::collectFunctions(Vec<FnSymbol *> &functions) {
      forv_Vec(FnSymbol, fn, gFnSymbols) {
          legalizeName(fn);
          functions.add(fn);
          }
      qsort(functions.v, functions.n, sizeof(functions.v[0]), compareSymbol);
      }

//
// mangle type names if they clash with other types
//
  void GenC::uniquifyTypeNames(
          Vec<TypeSymbol *> &types, Vec<const char *> &cnames) {
      forv_Vec(TypeSymbol, ts, types) {
          if (!ts->hasFlag(FLAG_EXTERN))
              ts->cname = uniquifyName(ts->cname, &cnames);
          }
      uniquifyNameCounts.clear();
      }

//
// change enum constant names into <type name>_<constant name> and
// mangle if they clash with other types or enum constants
//
  void GenC::uniquifyEnumNames(
          Vec<TypeSymbol *> &types, Vec<const char *> &cnames) {
      forv_Vec(TypeSymbol, ts, types) {
          if (EnumType *enumType = toEnumType(ts->type)) {
              for_enums(constant, enumType) {
                  Symbol *sym = constant->sym;
                  legalizeName(sym);
                  sym->cname = astr(enumType->symbol->cname, "_", sym->cname);
                  sym->cname = uniquifyName(sym->cname, &cnames);
                  }
              }
          }
      uniquifyNameCounts.clear();
      }

//
// mangle field names if they clash with other fields in the same class
//
  void GenC::uniquifyFieldNames(
          Vec<TypeSymbol *> &types, Vec<const char *> &cnames) {
      forv_Vec(TypeSymbol, ts, types) {
          if (ts->defPoint->parentExpr != rootModule->block) {
              if (ClassType *ct = toClassType(ts->type)) {
                  Vec<const char *> fieldNameSet;
                  for_fields(field, ct) {
                      legalizeName(field);
                      field->cname = uniquifyName(field->cname, &fieldNameSet);
                      }
                  uniquifyNameCounts.clear();
                  }
              }
          }
      }

//
// mangle global variable names if they clash with types, enum
// constants, or other global variables
//
  void GenC::uniquifyGlobalNames(
          Vec<VarSymbol *> &globals, Vec<const char *> &cnames) {
      forv_Vec(VarSymbol, var, globals) {
          if (!var->hasFlag(FLAG_EXTERN))
              var->cname = uniquifyName(var->cname, &cnames);
          }
      uniquifyNameCounts.clear();
      }

//
// mangle function names if they clash with types, enum constants,
// global variables, or other functions
//
  void GenC::uniquifyFunctionNames(
          Vec<FnSymbol *> &functions, Vec<const char *> &cnames) {
      forv_Vec(FnSymbol, fn, functions) {
          if (!fn->hasFlag(FLAG_USER_NAMED))
              fn->cname = uniquifyName(fn->cname, &cnames);
          }
      uniquifyNameCounts.clear();
      }

//
// mangle formal argument names if they clash with types, enum
// constants, global variables, functions, or earlier formal
// arguments in the same function
//
  void GenC::uniquifyFormalNames(Vec<const char *> &cnames) {
      forv_Vec(FnSymbol, fn, gFnSymbols) {
          Vec<const char *> formalNameSet;
          for_formals(formal, fn) {
              legalizeName(formal);
              formal->cname = uniquifyName(formal->cname, &formalNameSet, &cnames);
              }
          uniquifyNameCounts.clear();
          }
      }

//
// mangle local variable names if they clash with types, global
// variables, functions, formal arguments of their function, or
// other local variables in the same function
//
  void GenC::uniquifyLocalNames(Vec<const char *> &cnames) {
      forv_Vec(FnSymbol, fn, gFnSymbols) {
          Vec<const char *> local;

          for_formals(formal, fn) {
              local.set_add(formal->cname);
              }

          Vec<DefExpr *> defs;
          collectDefExprs(fn->body, defs);
          forv_Vec(DefExpr, def, defs) {
              legalizeName(def->sym);
            // give temps cnames
              if (def->sym->hasFlag(FLAG_TEMP)) {
                  if (localTempNames) {
                    // temp name is _tNNN_
                      if (!strncmp(def->sym->cname, "_t", 2))
                          def->sym->cname = astr("T", def->sym->cname + 2);
                      }
                  else {
                    // temp name is _tmp
                      if (!strcmp(def->sym->cname, "_tmp"))
                          def->sym->cname = astr("T");
                      }
                  }
              def->sym->cname = uniquifyName(def->sym->cname, &local, &cnames);
              }
          uniquifyNameCounts.clear();
          }
      }

  void GenC::genClassIDs(Vec<TypeSymbol *> &typeSymbols) {
      genComment("Class Type Identification Numbers");

      int count = 0;
      forv_Vec(TypeSymbol, ts, typeSymbols) {
          if (ClassType *ct = toClassType(ts->type)) {
              if (!isReferenceType(ct) && isClass(ct)) {
                  genGlobalDefClassId(ts->cname, count);
                  count++;
                  }
              }
          }
      }      

  void GenC::genClassPrototypes(Vec<TypeSymbol *> &types) {
      genComment("Class Prototypes");
      forv_Vec(TypeSymbol, typeSymbol, types) {
          if (!typeSymbol->hasFlag(FLAG_REF) && 
                  !typeSymbol->hasFlag(FLAG_DATA_CLASS))
              Symbol_codegenPrototype(typeSymbol);
          }
      }

// codegen enumerated types
  void GenC::genEnumeratedTypes(Vec<TypeSymbol *> &types) {
      genComment("Enumerated Types");
      forv_Vec(TypeSymbol, typeSymbol, types) {
          if (toEnumType(typeSymbol->type))
              Symbol_codegenDef(typeSymbol);
          }
      }

// codegen records/unions/references/data class in topological order
  void GenC::genAggregates(Vec<TypeSymbol *> &types) {
      genComment("Records, Unions, Data Class, References (Hierarchically)");
      forv_Vec(TypeSymbol, ts, types) {
          if (ClassType *ct = toClassType(ts->type))
              codegen_aggregate_def(ct);
          }
      }

// codegen remaining types
  void GenC::genOtherClasses(Vec<TypeSymbol *> &types) {
      genComment("Classes");
      forv_Vec(TypeSymbol, typeSymbol, types) {
          if (isClass(typeSymbol->type) &&
                  !typeSymbol->hasFlag(FLAG_REF) &&
                  !typeSymbol->hasFlag(FLAG_DATA_CLASS) &&
                  typeSymbol->hasFlag(FLAG_NO_OBJECT) &&
                  !typeSymbol->hasFlag(FLAG_OBJECT_CLASS))
              Symbol_codegenDef(typeSymbol);
          }
      }

//
// codegen class definitions in breadth first order starting with
// "object" and following its dispatch children
//
  void GenC::genClassDefs() {
      Vec<TypeSymbol *> next, current;
      current.add(dtObject->symbol);
      while (current.n) {
          forv_Vec(TypeSymbol, ts, current) {
              Symbol_codegenDef(ts);
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
      }

  void GenC::codegen_aggregate_def(ClassType *ct) {
      if (!shouldCodegenAggregate(ct)) 
          return;
      if (ct->symbol->codegenned) 
          return;
      ct->symbol->codegenned = true;

    // For reference or data class types, first generate
    // the referenced type
      Type *vt = NULL;
      if (ct->symbol->hasFlag(FLAG_REF))
          vt = ct->symbol->getValType();
      else if (ct->symbol->hasFlag(FLAG_DATA_CLASS))
          vt = getDataClassType(ct->symbol)->typeInfo();
      if (vt) {
          if (ClassType *fct = toClassType(vt))
              codegen_aggregate_def(fct);
          }
    // For other types, generate the field types
      for_fields(field, ct) {
          if (ClassType *fct = toClassType(field->type))
              codegen_aggregate_def(fct);
          }
    // Lastly, generate the type we're working on.
    // Codegen what we have here.
      Symbol_codegenDef(ct->symbol);
      }
      
  bool GenC::shouldCodegenAggregate(ClassType *ct) {
    // never codegen definitions of primitive or arithmetic types.
      if (toPrimitiveType(ct)) 
          return false;

    // Needed special handling for complex types, since after complex2record
    // they appear like normal records but we must not define them
    // since they are defined in the runtime headers
    // Added a flag, FLAG_NO_CODEGEN, to handle this case.
    // This flag could be used for other similar cases if necessary.
      if (ct->symbol->hasFlag(FLAG_NO_CODEGEN)) 
          return false;

    // Don't visit classes since they are prototyped individually all at once..
    // ..except for classes with FLAG_REF or FLAG_DATA_CLASS.. which
    //   we do visit.
      if (isClass(ct)) { // is it actually a class?
          if (ct->symbol->hasFlag(FLAG_REF) ||
                  ct->symbol->hasFlag(FLAG_WIDE) ||
                  ct->symbol->hasFlag(FLAG_DATA_CLASS)) 
              return true;
          else 
              return false;
          }

    // otherwise, visit record/union
      return true;
      }
      
// implementation: header (target-specific)

  void GenC::genCompilationInfo() {
      genComment("Compilation Info");

      if (fGPU)
          writeC("#ifndef ENABLE_GPU\n");

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
          writeC("#else\n");
          writeC("extern const char* chpl_compileCommand;\n");
          writeC("extern const char* chpl_compileVersion;\n");
          writeC("extern const char* CHPL_HOST_PLATFORM;\n");
          writeC("extern const char* CHPL_TARGET_PLATFORM;\n");
          writeC("extern const char* CHPL_HOST_COMPILER;\n");
          writeC("extern const char* CHPL_TARGET_COMPILER;\n");
          writeC("extern const char* CHPL_TASKS;\n");
          writeC("extern const char* CHPL_COMM;\n");
          writeC("#endif\n");
          }
      }
      
// C target only      
  void GenC::genIncludes() {
    // This is done in runClang for LLVM version.
      writeC("\n#define CHPL_GEN_CODE\n\n");

    // Include sys_basic.h to get C types always defined,
    // proper library .h inclusion
      writeC("#include \"sys_basic.h\"\n");
      genIncludeCommandLineHeaders(cfile);

      writeC("#include \"stdchpl.h\"\n");

    // include generated extern C header file
      if (externC && gAllExternCode.filename != NULL)
          writeC("%s", astr("#include \"", gAllExternCode.filename, "\"\n"));
      }
      
  void GenC::genGlobalDefClassId(const char *cname, int id) {
      const char *id_type_name = "chpl__class_id";
      std::string name("chpl__cid_");
      name += cname;

      writeC("const %s %s = %d;\n", id_type_name, name.c_str(), id);
      }

  void GenC::genFunctionPrototypes(Vec<FnSymbol *> &functions) {
      genComment("Function Prototypes");
      forv_Vec(FnSymbol, fnSymbol, functions) {
          if (fnSymbol->hasFlag(FLAG_GPU_ON)) {
              writeC("\n#ifdef ENABLE_GPU\n");
              Symbol_codegenPrototype(fnSymbol);
              writeC("#endif\n");
              continue;
              }
          Symbol_codegenPrototype(fnSymbol);
          }
      }

  void GenC::genFunctionPointerTable(Vec<FnSymbol *> &functions) {
      if (fGPU)
          writeC("\n#ifndef ENABLE_GPU\n");

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
          writeC("#else\n");
          writeC("extern chpl_fn_p chpl_ftable[];\n");
          writeC("#endif\n");
          }
      }
      
  void GenC::genFtable(Vec<FnSymbol *> &fSymbols) {
      const char *ftable_name = "chpl_ftable";
      writeC("chpl_fn_p %s[] = {\n", ftable_name);
      bool first = true;
      forv_Vec(FnSymbol, fn, fSymbols) {
          if (!first)
              writeC(",\n");
          writeC("(chpl_fn_p)%s", fn->cname);
          first = false;
          }

      if (fSymbols.n == 0)
          writeC("(chpl_fn_p)0");
      writeC("\n};\n");
      }
      
  void GenC::genVirtualMethodTable(Vec<TypeSymbol *> &types) {
      genComment("Virtual Method Table");

      int maxVMT = 0;
      for (int i = 0; i < virtualMethodTable.n; i++) {
          if (virtualMethodTable.v[i].key && virtualMethodTable.v[i].value->n > maxVMT)
              maxVMT = virtualMethodTable.v[i].value->n;
          }

      gMaxVMT = maxVMT;

      const char *vmt = "chpl_vmtable";
    // MPF - in order to simplify code generation, making
    // chpl_vmtable a 1D array.
      writeC("chpl_fn_p %s[] = {\n", vmt);
      bool comma = false;
      forv_Vec(TypeSymbol, ts, types) {
          if (ClassType *ct = toClassType(ts->type)) {
              if (!isReferenceType(ct) && isClass(ct)) {
                  if (comma)
                      writeC(",\n");
                  writeC(" /* %s */\n", ct->symbol->cname);
                  int n = 0;
                  if (Vec<FnSymbol *> *vfns = virtualMethodTable.get(ct)) {
                      forv_Vec(FnSymbol, vfn, *vfns) {
                          if (n > 0)
                              writeC(",\n");
                          writeC("(chpl_fn_p)%s", vfn->cname);
                          n++;
                          }
                      }
                  for (int i = n; i < maxVMT; i++) {
                      if (n > 0)
                          writeC(",\n");
                      writeC("(chpl_fn_p)NULL");
                      n++;
                      }
                  comma = true;
                  }
              }
          }
      writeC("\n};\n");
      }
      
  void GenC::genGlobalVariables(Vec<VarSymbol *> &globals) {
      if (fGPU)
          writeC("\n#ifndef ENABLE_GPU\n");

      genComment("Global Variables");
      forv_Vec(VarSymbol, varSymbol, globals) {
          VarSymbol_codegenGlobalDef(varSymbol);
          }
      flushStatements();

      genGlobalInt("chpl_numGlobalsOnHeap", numGlobalsOnHeap);
      int globals_registry_static_size = (numGlobalsOnHeap ? numGlobalsOnHeap : 1);
      writeC("\nvoid** chpl_globals_registry;\n");
      writeC("\nvoid* chpl_globals_registry_static[%d];\n", 
          globals_registry_static_size);
      genGlobalInt("chpl_heterogeneous", fHeterogeneous?1:0);
      writeC("\nconst char* chpl_mem_descs[] = {\n");
      bool first = true;
      forv_Vec(const char*, memDesc, memDescsVec) {
          if (!first)
              writeC(",\n");
          writeC("\"%s\"", memDesc);
          first = false;
          }
      writeC("\n};\n");

      genGlobalInt("chpl_mem_numDescs", memDescsVec.n);
      genGlobalInt("chpl_no_stdmodules", fNoInternalModules);

    //
    // add table of private-broadcast constants
    //
      writeC("\nvoid* const chpl_private_broadcast_table[] = {\n");
      writeC("&chpl_verbose_comm");
      writeC(",\n&chpl_comm_diagnostics");
      writeC(",\n&chpl_verbose_mem");
      int i = 3;
      forv_Vec(CallExpr, call, gCallExprs) {
          if (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) {
              SymExpr *se = toSymExpr(call->get(1));
              INT_ASSERT(se);
              SET_LINENO(call);
              writeC(",\n&%s", se->var->cname);
              call->insertAtHead(new_IntSymbol(i));
              i++;
              }
          }
      writeC("\n};\n");

      if (fGPU) {
          writeC("#else\n");
          forv_Vec(VarSymbol, varSymbol, globals) {
              writeC("extern ");
              Symbol_codegenDef(varSymbol);
              }

          writeC("\nextern const int chpl_numGlobalsOnHeap;\n");
          writeC("\nextern void** chpl_globals_registry;\n");
          writeC("\nextern void* chpl_globals_registry_static[];\n");
          writeC("\nconst int chpl_heterogeneous = ");
          if (fHeterogeneous)
              writeC(" 1;\n");
          else
              writeC(" 0;\n");
          writeC("\nextern const char* chpl_mem_descs[];\n");
          writeC("\nextern const int chpl_mem_numDescs;\n");
          writeC("#endif\n");
          }
      }
      
  void GenC::genGlobalString(const char *cname, const char *value) {
      writeC("const char* %s = \"%s\";\n", cname, value);
      }
      
  void GenC::genGlobalInt(const char *cname, int value) {
      writeC("const int %s = %d;\n", cname, value);
      }
      
// private: name mangling (target-independent)

//
// given a name and up to two sets of names, return a name that is in
// neither set and add the name to the first set; the second set may
// be omitted; the returned name to be capped at fMaxCIdentLen if non-0
//
// the unique numbering is based on the map uniquifyNameCounts which
// can be cleared to reset
//
  const char *GenC::uniquifyName(
          const char *name, 
          Vec<const char *> *set1, 
          Vec<const char *> *set2/*=NULL*/) {
      const char *newName = name;
      if (fMaxCIdentLen > 0 && (int)(strlen(newName)) > fMaxCIdentLen) {
        // how much of the name to preserve
          int prefixLen = fMaxCIdentLen - maxUniquifyAddedChars;
          if (!longCNameReplacementBuffer) {
              longCNameReplacementBuffer = (char *)malloc(prefixLen+1);
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
      
  int GenC::compareSymbol(const void *v1, const void *v2) {
      Symbol *s1 = *(Symbol **)v1;
      Symbol *s2 = *(Symbol **)v2;
      ModuleSymbol *m1 = s1->getModule();
      ModuleSymbol *m2 = s2->getModule();
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
      
  void GenC::legalizeName(Symbol *sym) {
      if (sym->hasFlag(FLAG_EXTERN))
          return;
      for (const char *ch = sym->cname; *ch != '\0'; ch++) {
          switch (*ch) {
          case '>': 
              ch = subChar(sym, ch, "_GREATER_"); 
              break;
          case '<': 
              ch = subChar(sym, ch, "_LESS_"); 
              break;
          case '=':
              {
                /* 
                  To help generated code readability, we'd like to convert =
                  into "ASSIGN" and == into "EQUALS".  Unfortunately, because
                  of the character-at-a-time approach taken here combined
                  with the fact that subChar() returns a completely new
                  string on every call, the way I implemented this is a bit
                  ugly (in part because I didn't want to spend the time to
                  reimplement this whole function -BLC 
                */
                  static const char *equalsStr = "_EQUALS_";
                  static int equalsLen = strlen(equalsStr);

                  if (*(ch + 1) == '=')
                    // If we're in the == case, replace the first = with EQUALS
                      ch = subChar(sym, ch, equalsStr);
                  else if ((ch - equalsLen >= sym->cname) && 
                          !strncmp(ch-equalsLen, equalsStr, equalsLen))
                    // Otherwise, if the thing preceding this '=' is the
                    // string _EQUALS_, we must have been the second '=' and
                    // we should just replace ourselves with an underscore to
                    // make things legal.
                      ch = subChar(sym, ch, "_");
                  else
                    // Otherwise, this must have simply been a standalone '='
                      ch = subChar(sym, ch, "_ASSIGN_");
                  }
              break;
          case '*': 
              ch = subChar(sym, ch, "_ASTERISK_"); 
              break;
          case '/': 
              ch = subChar(sym, ch, "_SLASH_"); 
              break;
          case '%': 
              ch = subChar(sym, ch, "_PERCENT_"); 
              break;
          case '+': 
              ch = subChar(sym, ch, "_PLUS_"); 
              break;
          case '-': 
              ch = subChar(sym, ch, "_HYPHEN_"); 
              break;
          case '^': 
              ch = subChar(sym, ch, "_CARET_"); 
              break;
          case '&': 
              ch = subChar(sym, ch, "_AMPERSAND_"); 
              break;
          case '|': 
              ch = subChar(sym, ch, "_BAR_"); 
              break;
          case '!': 
              ch = subChar(sym, ch, "_EXCLAMATION_"); 
              break;
          case '#': 
              ch = subChar(sym, ch, "_POUND_"); 
              break;
          case '?': 
              ch = subChar(sym, ch, "_QUESTION_"); 
              break;
          case '$': 
              ch = subChar(sym, ch, "_DOLLAR_"); 
              break;
          case '~': 
              ch = subChar(sym, ch, "_TILDA_"); 
              break;
          case '.': 
              ch = subChar(sym, ch, "_DOT_"); 
              break;
          case ' ': 
              ch = subChar(sym, ch, "_SPACE_"); 
              break;
          default: 
              break;
              }
          }

    // Add chpl_ to operator names.
      if ((sym->cname[0] == '_' &&
              (sym->cname[1] == '_' || (sym->cname[1] >= 'A' && sym->cname[1] <= 'Z'))))
          sym->cname = astr("chpl__", sym->cname);
      }
      
  const char *GenC::subChar(
          Symbol *sym, const char *ch, const char *x) {          
      char *tmp = (char *)malloc(ch-sym->cname+1);
      strncpy(tmp, sym->cname, ch-sym->cname);
      tmp[ch-sym->cname] = '\0';
      sym->cname = astr(tmp, x, ch+1); 
      free(tmp);
      return sym->cname;
      }
      
  void GenC::reservedSymbolNames(Vec<const char *> &cnames) {
    // reserved words in C
      cnames.set_add(astr("auto"));
      cnames.set_add(astr("break"));
      cnames.set_add(astr("case"));
      cnames.set_add(astr("char"));
      cnames.set_add(astr("const"));
      cnames.set_add(astr("continue"));
      cnames.set_add(astr("default"));
      cnames.set_add(astr("do"));
      cnames.set_add(astr("double"));
      cnames.set_add(astr("else"));
      cnames.set_add(astr("enum"));
      cnames.set_add(astr("extern"));
      cnames.set_add(astr("float"));
      cnames.set_add(astr("for"));
      cnames.set_add(astr("goto"));
      cnames.set_add(astr("if"));
      cnames.set_add(astr("inline"));
      cnames.set_add(astr("int"));
      cnames.set_add(astr("long"));
      cnames.set_add(astr("register"));
      cnames.set_add(astr("restrict"));
      cnames.set_add(astr("return"));
      cnames.set_add(astr("short"));
      cnames.set_add(astr("signed"));
      cnames.set_add(astr("sizeof"));
      cnames.set_add(astr("static"));
      cnames.set_add(astr("struct"));
      cnames.set_add(astr("switch"));
      cnames.set_add(astr("typedef"));
      cnames.set_add(astr("union"));
      cnames.set_add(astr("unsigned"));
      cnames.set_add(astr("void"));
      cnames.set_add(astr("volatile"));
      cnames.set_add(astr("while"));
    // all symbol names beginning with _ followed by an uppercase letter
    // need to be mangled; this should be done with custom code

      cnames.set_add(astr("abs"));
      cnames.set_add(astr("cos"));
      cnames.set_add(astr("sin"));
      cnames.set_add(astr("tan"));
      cnames.set_add(astr("acos"));
      cnames.set_add(astr("asin"));
      cnames.set_add(astr("atan"));
      cnames.set_add(astr("floor"));
      cnames.set_add(astr("sqrt"));
      cnames.set_add(astr("conjg"));
      cnames.set_add(astr("exit"));
      cnames.set_add(astr("_init"));
      cnames.set_add(astr("stdin"));
      cnames.set_add(astr("close"));
      cnames.set_add(astr("fwrite"));
      cnames.set_add(astr("fread"));
      cnames.set_add(astr("main"));
      cnames.set_add(astr("open"));
      cnames.set_add(astr("pow"));
      cnames.set_add(astr("printf"));
      cnames.set_add(astr("scanf"));
      cnames.set_add(astr("quad"));
      cnames.set_add(astr("read"));
      cnames.set_add(astr("sleep"));
      cnames.set_add(astr("stderr"));
      cnames.set_add(astr("stdout"));
      cnames.set_add(astr("wait"));
      cnames.set_add(astr("write"));
    // the following 6 are in /usr/include/bits/mathcalls.h - ridiculous
      cnames.set_add(astr("j0")); 
      cnames.set_add(astr("j1"));
      cnames.set_add(astr("jn"));
      cnames.set_add(astr("y0"));
      cnames.set_add(astr("y1")); // this is ridiculous...
      cnames.set_add(astr("yn"));
      cnames.set_add(astr("log2"));
      cnames.set_add(astr("remove"));
      cnames.set_add(astr("fprintf"));
      cnames.set_add(astr("fscanf"));
      cnames.set_add(astr("clone"));
      cnames.set_add(astr("new"));
      cnames.set_add(astr("delete"));
      cnames.set_add(astr("register"));
      cnames.set_add(astr("signal"));
      cnames.set_add(astr("ceil"));
      cnames.set_add(astr("acosh"));
      cnames.set_add(astr("asinh"));
      cnames.set_add(astr("atan2"));
      cnames.set_add(astr("atanh"));
      cnames.set_add(astr("cbrt"));
      cnames.set_add(astr("cosh"));
      cnames.set_add(astr("erf"));
      cnames.set_add(astr("erfc"));
      cnames.set_add(astr("exp"));
      cnames.set_add(astr("exp2"));
      cnames.set_add(astr("expm1"));
      cnames.set_add(astr("lgamma"));
      cnames.set_add(astr("log10"));
      cnames.set_add(astr("log1p"));
      cnames.set_add(astr("log"));
      cnames.set_add(astr("rint"));
      cnames.set_add(astr("sinh"));
      cnames.set_add(astr("tanh"));
      cnames.set_add(astr("ascii"));
      cnames.set_add(astr("isnan"));
      cnames.set_add(astr("random"));
      cnames.set_add(astr("truncate"));
      cnames.set_add(astr("time"));
      cnames.set_add(astr("nearbyint"));
      cnames.set_add(astr("round"));
      cnames.set_add(astr("tgamma"));
      cnames.set_add(astr("trunc"));
      cnames.set_add(astr("div"));
      cnames.set_add(astr("max"));
      cnames.set_add(astr("min"));
      cnames.set_add(astr("malloc"));
      cnames.set_add(astr("free"));

      cnames.set_add(astr("unlink"));

    // symbols in stdint.h
      cnames.set_add(astr("int8_t"));
      cnames.set_add(astr("int16_t"));
      cnames.set_add(astr("int32_t"));
      cnames.set_add(astr("int64_t"));
      cnames.set_add(astr("uint8_t"));
      cnames.set_add(astr("uint16_t"));
      cnames.set_add(astr("uint32_t"));
      cnames.set_add(astr("uint64_t"));
      cnames.set_add(astr("int_least8_t"));
      cnames.set_add(astr("int_least16_t"));
      cnames.set_add(astr("int_least32_t"));
      cnames.set_add(astr("int_least64_t"));
      cnames.set_add(astr("uint_least8_t"));
      cnames.set_add(astr("uint_least16_t"));
      cnames.set_add(astr("uint_least32_t"));
      cnames.set_add(astr("uint_least64_t"));
      cnames.set_add(astr("int_fast8_t"));
      cnames.set_add(astr("int_fast16_t"));
      cnames.set_add(astr("int_fast32_t"));
      cnames.set_add(astr("int_fast64_t"));
      cnames.set_add(astr("uint_fast8_t"));
      cnames.set_add(astr("uint_fast16_t"));
      cnames.set_add(astr("uint_fast32_t"));
      cnames.set_add(astr("uint_fast64_t"));
      cnames.set_add(astr("intptr_t"));
      cnames.set_add(astr("uintptr_t"));
      cnames.set_add(astr("intmax_t"));
      cnames.set_add(astr("uintmax_t"));
      cnames.set_add(astr("INT8_MAX"));
      cnames.set_add(astr("INT16_MAX"));
      cnames.set_add(astr("INT32_MAX"));
      cnames.set_add(astr("INT64_MAX"));
      cnames.set_add(astr("INT8_MIN"));
      cnames.set_add(astr("INT16_MIN"));
      cnames.set_add(astr("INT32_MIN"));
      cnames.set_add(astr("INT64_MIN"));
      cnames.set_add(astr("UINT8_MAX"));
      cnames.set_add(astr("UINT16_MAX"));
      cnames.set_add(astr("UINT32_MAX"));
      cnames.set_add(astr("UINT64_MAX"));
      cnames.set_add(astr("INT_LEAST8_MIN"));
      cnames.set_add(astr("INT_LEAST16_MIN"));
      cnames.set_add(astr("INT_LEAST32_MIN"));
      cnames.set_add(astr("INT_LEAST64_MIN"));
      cnames.set_add(astr("INT_LEAST8_MAX"));
      cnames.set_add(astr("INT_LEAST16_MAX"));
      cnames.set_add(astr("INT_LEAST32_MAX"));
      cnames.set_add(astr("INT_LEAST64_MAX"));
      cnames.set_add(astr("UINT_LEAST8_MAX"));
      cnames.set_add(astr("UINT_LEAST16_MAX"));
      cnames.set_add(astr("UINT_LEAST32_MAX"));
      cnames.set_add(astr("UINT_LEAST64_MAX"));
      cnames.set_add(astr("INT_FAST8_MIN"));
      cnames.set_add(astr("INT_FAST16_MIN"));
      cnames.set_add(astr("INT_FAST32_MIN"));
      cnames.set_add(astr("INT_FAST64_MIN"));
      cnames.set_add(astr("INT_FAST8_MAX"));
      cnames.set_add(astr("INT_FAST16_MAX"));
      cnames.set_add(astr("INT_FAST32_MAX"));
      cnames.set_add(astr("INT_FAST64_MAX"));
      cnames.set_add(astr("UINT_FAST8_MAX"));
      cnames.set_add(astr("UINT_FAST16_MAX"));
      cnames.set_add(astr("UINT_FAST32_MAX"));
      cnames.set_add(astr("UINT_FAST64_MAX"));
      cnames.set_add(astr("INTPTR_MIN"));
      cnames.set_add(astr("INTPTR_MAX"));
      cnames.set_add(astr("UINTPTR_MAX"));
      cnames.set_add(astr("INTMAX_MIN"));
      cnames.set_add(astr("INTMAX_MAX"));
      cnames.set_add(astr("UINTMAX_MAX"));
      cnames.set_add(astr("PTRDIFF_MIN"));
      cnames.set_add(astr("PTRDIFF_MAX"));
      cnames.set_add(astr("SIZE_MAX"));
      cnames.set_add(astr("WCHAR_MAX"));
      cnames.set_add(astr("WCHAR_MIN"));
      cnames.set_add(astr("WINT_MIN"));
      cnames.set_add(astr("WINT_MAX"));
      cnames.set_add(astr("SIG_ATOMIC_MIN"));
      cnames.set_add(astr("SIG_ATOMIC_MAX"));
      cnames.set_add(astr("INT8_C"));
      cnames.set_add(astr("INT16_C"));
      cnames.set_add(astr("INT32_C"));
      cnames.set_add(astr("INT64_C"));
      cnames.set_add(astr("UINT8_C"));
      cnames.set_add(astr("UINT16_C"));
      cnames.set_add(astr("UINT32_C"));
      cnames.set_add(astr("UINT64_C"));
      cnames.set_add(astr("INTMAX_C"));
      cnames.set_add(astr("UINTMAX_C"));

    // symbols from stdio.h
      cnames.set_add(astr("EOF"));

    // symbols from QIO
      cnames.set_add(astr("EEOF"));
      cnames.set_add(astr("ESHORT"));
      cnames.set_add(astr("EFORMAT"));

    // additional reserved words in the MTA/XMT compiler
      if (!strcmp(CHPL_TASKS, "mta")) {
          cnames.set_add(astr("purge"));
          cnames.set_add(astr("task")); 
          }

    // reserved words in c++
      cnames.set_add(astr("this"));
      cnames.set_add(astr("template"));
      cnames.set_add(astr("assert"));

      cnames.set_add(astr("flag"));

    // New additions from the PGI 13.3 C runtime libpgc.a (relaxedmath.o):
      cnames.set_add(astr("alloca"));
      cnames.set_add(astr("add128"));
      cnames.set_add(astr("add192"));
      cnames.set_add(astr("eq128"));
      cnames.set_add(astr("le128"));
      cnames.set_add(astr("lt128"));
      cnames.set_add(astr("ne128"));
      cnames.set_add(astr("sub128"));
      cnames.set_add(astr("sub192"));
      cnames.set_add(astr("a1"));
      cnames.set_add(astr("a2"));
      cnames.set_add(astr("a3"));
      cnames.set_add(astr("inc"));
      cnames.set_add(astr("inv32"));
      cnames.set_add(astr("invln2"));
      cnames.set_add(astr("isign"));
      cnames.set_add(astr("ln2"));
      cnames.set_add(astr("pi"));
      cnames.set_add(astr("table"));
      cnames.set_add(astr("ten23"));
      cnames.set_add(astr("ts"));
      cnames.set_add(astr("two"));
      }
