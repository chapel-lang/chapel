#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

int setInCommandLine(char* varName, _integer64* value);
int initConfigVarTable(void);
int installConfigVar(char* varName, char* value);

#endif

