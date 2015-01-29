#ifndef _PARSER_H_
#define _PARSER_H_

#include "symbol.h"

extern ModTag currentModuleType;
extern bool currentFileNamedOnCommandLine;

ModuleSymbol* ParseFile(const char* filename, ModTag modtype, 
                        bool namedOnCommandLine=false);
ModuleSymbol* ParseMod(const char* modname, ModTag modtype);

void addModuleToParseList(const char* name, CallExpr* newUse);
void parseDependentModules(ModTag modtype);

#endif
