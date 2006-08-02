#include <stdio.h>
#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "mysystem.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"


#define STRSUB(x)                               \
  *ch = '\0';                                   \
  sym->cname = stringcat(sym->cname, x, ch+1);  \
  ch = sym->cname

static void legalizeCName(Symbol* sym) {
  for (char* ch = sym->cname; *ch != '\0'; ch++) {
    switch (*ch) {
    case '>': STRSUB("_GREATER_"); break;
    case '<': STRSUB("_LESS_"); break;
    case '=': STRSUB("_EQUAL_"); break;
    case '*': STRSUB("_ASTERISK_"); break;
    case '/': STRSUB("_SLASH_"); break;
    case '%': STRSUB("_PERCENT_"); break;
    case '+': STRSUB("_PLUS_"); break;
    case '-': STRSUB("_HYPHEN_"); break;
    case '^': STRSUB("_CARET_"); break;
    case '&': STRSUB("_AMPERSAND_"); break;
    case '|': STRSUB("_BAR_"); break;
    case '!': STRSUB("_EXCLAMATION_"); break;
    case '#': STRSUB("_POUND_"); break;
    case '?': STRSUB("_QUESTION_"); break;
    case '~': STRSUB("_TILDA_"); break;
    default: break;
    }
  }
}

static void codegen_header(void) {
  ChainHashMap<char*, StringHashFns, int> cnames;
  Vec<TypeSymbol*> typeSymbols;
  Vec<FnSymbol*> fnSymbols;
  Vec<VarSymbol*> varSymbols;

  chpl_main->addPragma("rename _chpl_main");

  cnames.put("main", 1);
  cnames.put("stdin", 1);
  cnames.put("stdout", 1);
  cnames.put("stderr", 1);
  cnames.put("fwrite", 1);
  cnames.put("fread", 1);
  cnames.put("_init", 1);
  cnames.put("y1", 1); // this is ridiculous...

  // Remove functions not used (misses mutual recursion)
  compute_call_sites();
  Vec<FnSymbol*> all_functions;
  collect_functions(&all_functions);
  forv_Vec(FnSymbol, fn, all_functions) {
    if (fn->calledBy->n == 0 && chpl_main != fn)
      fn->defPoint->parentStmt->remove();
  }

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    Symbol* sym = dynamic_cast<Symbol*>(ast);
    if (!sym)
      continue;

    if (sym->name == sym->cname)
      sym->cname = stringcpy(sym->name);

    if (char* pragma = sym->hasPragmaPrefix("rename"))
      sym->cname = stringcpy(pragma+7);

    if (VarSymbol* vs = dynamic_cast<VarSymbol*>(ast))
      if (vs->immediate)
        continue;

    if (sym->parentScope == rootScope)
      continue;

    if (sym->hasPragma("no codegen"))
      continue;

    legalizeCName(sym);

    // mangle symbol that is neither field nor formal if the symbol's
    // name has already been encountered
    if (!dynamic_cast<ArgSymbol*>(sym) &&
        !dynamic_cast<UnresolvedSymbol*>(sym) &&
        !dynamic_cast<ClassType*>(sym->parentScope->astParent) &&
        cnames.get(sym->cname))
      sym->cname = stringcat("_", intstring(sym->id), "_", sym->cname);

    cnames.put(sym->cname, 1);

    if (TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(sym)) {
      //      if (!typeSymbol->hasPragma("data class"))
      typeSymbols.add(typeSymbol);
    } else if (FnSymbol* fnSymbol = dynamic_cast<FnSymbol*>(sym)) {
      fnSymbols.add(fnSymbol);
    } else if (VarSymbol* varSymbol = dynamic_cast<VarSymbol*>(sym)) {
      if (dynamic_cast<ModuleSymbol*>(varSymbol->parentScope->astParent))
        varSymbols.add(varSymbol);
    }
  }

  FILE* outfile = openCFile("_chpl_header.h");
  fprintf(outfile, "#include \"stdchpl.h\"\n");
  fprintf(outfile, "\n");

  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    typeSymbol->codegenPrototype(outfile);
  }
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (dynamic_cast<EnumType*>(typeSymbol->type))
      typeSymbol->codegenDef(outfile);
  }
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (!dynamic_cast<EnumType*>(typeSymbol->type)) {
      typeSymbol->codegenDef(outfile);
    }
  }
  forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
    fnSymbol->codegenPrototype(outfile);
  }
  forv_Vec(VarSymbol, varSymbol, varSymbols) {
    varSymbol->codegenDef(outfile);
  }
  closeCFile(outfile);
}


static void
codegen_config() {
  fileinfo outfileinfo;
  char* rtconfigFile = "rtconfig";
  openCFiles(rtconfigFile, &outfileinfo);
  fprintf(codefile, "#include \"stdchpl.h\"\n\n");
  fprintf(codefile, "void CreateConfigVarTable(void) {\n");
  fprintf(codefile, "initConfigVarTable();\n");

  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      codefile = outfileinfo.fptr;
      VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);
      if (var && var->varClass == VAR_CONFIG) {
        fprintf(codefile, "installConfigVar(\"%s\", \"", var->name);
        fprintf(codefile, var->type->symbol->name);
        fprintf(codefile, "\", \"%s\");\n", var->getModule()->name);
      }
    }
  }

  codefile = outfileinfo.fptr;
  fprintf(codefile, "if (askedToParseArgs()) {\n");
  fprintf(codefile, "  parseConfigArgs();\n");
  fprintf(codefile, "}\n");

  fprintf(codefile, "if (askedToPrintHelpMessage()) {\n");
  fprintf(codefile, "  printHelpTable();\n");
  fprintf(codefile, "  printConfigVarTable();\n");
  fprintf(codefile, "}\n");
  
  fprintf(codefile, "}\n");
  closeCFiles(&outfileinfo);
}


void codegen(void) {
  if (no_codegen)
    return;

  codegen_header();

  openMakefile(allModules.v[0]->filename, system_dir);

  codegen_config();

  forv_Vec(ModuleSymbol, currentModule, allModules) {
    if (!currentModule->hasPragma("no codegen")) {
      mysystem(stringcat("# codegen-ing module", currentModule->name),
               "generating comment for --print-commands option");
      currentModule->codegenDef();
    }
  }

  closeMakefile();
  makeBinary();
}
