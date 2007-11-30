#ifndef _PARSER_H_
#define _PARSER_H_

#include "symbol.h"

ModuleSymbol* ParseFile(const char* filename, ModTag moduletype);
AList* parse_string(char* string);

#endif
