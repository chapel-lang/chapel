#ifndef _PARSER_H_
#define _PARSER_H_

#include "symbol.h"

ModuleSymbol* ParseFile(char* filename, bool prelude = false);

#endif
