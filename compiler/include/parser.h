#ifndef _PARSER_H_
#define _PARSER_H_

#include "symbol.h"

extern ModTag currentModuleType;

ModuleSymbol* ParseFile(const char* filename, ModTag modtype);
ModuleSymbol* ParseMod(const char* modname, ModTag modtype);

void addModuleToParseList(const char* name, CallExpr* newUse);
void parseDependentModules(ModTag modtype);

#endif
