#ifndef _PARSER_H_
#define _PARSER_H_

#include "stmt.h"

Stmt* ParseFile(char* filename, bool prelude = false);

#endif
