#include <typeinfo>
#include "expr.h"
#include "files.h"
#include "createConfigVarTable.h"
#include "stmt.h"


CreateConfigVarTable::CreateConfigVarTable(void) {
  char* rtconfigFile = "rtconfig";
  openCFiles(rtconfigFile, &outfileinfo, &extheadfileinfo, &intheadfileinfo);
  fprintf(codefile, "#include \"%s\"\n", extheadfileinfo.filename);
  fprintf(codefile, "#include \"%s\"\n", intheadfileinfo.filename);
  fprintf(codefile, "#include \"stdchpl.h\"\n");
  fprintf(codefile, "\n");
  fprintf(codefile, "void CreateConfigVarTable(void) {\n");
  fprintf(codefile, "int isDefaultValue = 1;\n");
  fprintf(codefile, "\n");
  fprintf(codefile, "initConfigVarTable();\n");
}


void CreateConfigVarTable::preProcessStmt(Stmt* &stmt) {
  codefile = outfileinfo.fptr;

  if (typeid(*stmt) == typeid(VarDefStmt)) {
    VarDefStmt* varDef = dynamic_cast<VarDefStmt*>(stmt);

    if (varDef->var->varClass == VAR_CONFIG) {
      fprintf(codefile, "installConfigVar(\"%s\", ", varDef->var->name);
      varDef->init->printCfgInitString(codefile);
      fprintf(codefile, ", isDefaultValue);\n");
    }
  }
}


void CreateConfigVarTable::closeCFile() {
  codefile = outfileinfo.fptr;
  
  fprintf(codefile, "}\n");
  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}
