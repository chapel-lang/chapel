#include <string>

void printArg(std::ofstream *file, ArgSymbol *arg);

void printIntent(std::ofstream *file, IntentTag intent);

void printTabs(std::ofstream *file);

void createDocsFileFolders(std::string filename);

void printModule(std::ofstream *file, ModuleSymbol *mod, std::string name);

void printFunction(std::ofstream *file, FnSymbol *fn);

void printVarStart(std::ofstream *file, VarSymbol *var);

void printVarType(std::ofstream *file, VarSymbol *var);

void printVarDocs(std::ofstream *file, VarSymbol *var);

void printClass(std::ofstream *file, ClassType *cl);

void printFields(std::ofstream *file, ClassType *cl);

void inheritance(Vec<ClassType*> *list, ClassType *cl);

bool devOnlyFunction(FnSymbol *fn);

bool devOnlyModule(ModuleSymbol *mod);

bool isNotSubmodule(ModuleSymbol *mod);
