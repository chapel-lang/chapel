/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _PARSER_H_
#define _PARSER_H_

#include "symbol.h"

extern ModTag currentModuleType;

ModuleSymbol* ParseFile(const char* filename, ModTag modtype);
ModuleSymbol* ParseMod(const char* modname, ModTag modtype);

void addModuleToParseList(const char* name, CallExpr* newUse);
void parseDependentModules(ModTag modtype);

#endif
