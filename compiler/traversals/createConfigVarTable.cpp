#include <typeinfo>
#include "createConfigVarTable.h"
#include "expr.h"
#include "stmt.h"
#include "symscope.h"


CreateConfigVarTable::CreateConfigVarTable(void) {
  char* rtconfigFile = "rtconfig";
  openCFiles(rtconfigFile, &outfileinfo, &extheadfileinfo, &intheadfileinfo);
  fprintf(codefile, "#include \"%s\"\n", extheadfileinfo.filename);
  fprintf(codefile, "#include \"%s\"\n", intheadfileinfo.filename);
  fprintf(codefile, "#include \"stdchpl.h\"\n");
  fprintf(codefile, "\n");
  fprintf(codefile, "void CreateConfigVarTable(void) {\n");
  fprintf(codefile, "initConfigVarTable();\n");
}


void CreateConfigVarTable::preProcessStmt(Stmt* stmt) {
  codefile = outfileinfo.fptr;

  if (typeid(*stmt) == typeid(VarDefStmt)) {
    VarDefStmt* varDef = dynamic_cast<VarDefStmt*>(stmt);
    char* moduleName = varDef->var->parentScope->symContext->name;

    if (varDef->var->varClass == VAR_CONFIG) {
      fprintf(codefile, "installConfigVar(\"%s\", ", varDef->var->name);
      varDef->var->init->printCfgInitString(codefile);
      fprintf(codefile, ", \"%s\");\n", moduleName);
    }
  }
}


void CreateConfigVarTable::closeCFile() {
  codefile = outfileinfo.fptr;
  
  fprintf(codefile, "}\n");
  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}
