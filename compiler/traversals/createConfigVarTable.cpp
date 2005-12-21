#include <typeinfo>
#include "createConfigVarTable.h"
#include "expr.h"
#include "stmt.h"
#include "symscope.h"
#include "symtab.h"


bool CreateConfigVarTable::running = false;

CreateConfigVarTable::CreateConfigVarTable(void) {
  char* rtconfigFile = "rtconfig";
  openCFiles(rtconfigFile, &outfileinfo);
  fprintf(codefile, "#include \"stdchpl.h\"\n");
  fprintf(codefile, "\n");
  fprintf(codefile, "void CreateConfigVarTable(void) {\n");
  fprintf(codefile, "initConfigVarTable();\n");
}


void CreateConfigVarTable::postProcessExpr(Expr* expr) {
  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    codefile = outfileinfo.fptr;

    VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);

    if (var && var->varClass == VAR_CONFIG) {
      char* moduleName = dynamic_cast<Symbol*>(var->parentScope->astParent)->name;
      fprintf(codefile, "installConfigVar(\"%s\", \"", var->name);
      if (use_alloc) {
        fprintf(codefile, "__init__"); // should contain init
      } else {
        var->defPoint->init->print(codefile);
      }
      fprintf(codefile, "\", \"%s\");\n", moduleName);
    }
  }
}


void CreateConfigVarTable::closeCFile() {
  codefile = outfileinfo.fptr;

  fprintf(codefile, "if (askedToParseArgs()) {\n");
  fprintf(codefile, "parseConfigArgs();\n");
  fprintf(codefile, "}\n");

  fprintf(codefile, "if (askedToPrintHelpMessage()) {\n");
  fprintf(codefile, "printHelpTable();\n");
  fprintf(codefile, "printConfigVarTable();\n");
  fprintf(codefile, "}\n");
  
  fprintf(codefile, "}\n");
  closeCFiles(&outfileinfo);
}


void CreateConfigVarTable::run(Vec<ModuleSymbol*>* modules) {
  CreateConfigVarTable::running = true;
  Traversal::run(modules);
  CreateConfigVarTable::running = false;
}


void createConfigVarTable(void) {
  Pass* pass = new CreateConfigVarTable();
  pass->run(Symboltable::getModules(pass->whichModules));
}
