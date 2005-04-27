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


void CreateConfigVarTable::processSymbol(Symbol* symbol) {
  codefile = outfileinfo.fptr;

  VarSymbol* var = dynamic_cast<VarSymbol*>(symbol);

  if (var && var->varClass == VAR_CONFIG) {
    char* moduleName = var->parentScope->symContext->name;
    fprintf(codefile, "installConfigVar(\"%s\", ", var->name);
    var->defPoint->init->expr->printCfgInitString(codefile);
    fprintf(codefile, ", \"%s\");\n", moduleName);
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
  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}
