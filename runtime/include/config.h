#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

int setInCommandLine_integer64(char* varName, _integer64* value);
int setInCommandLine_float64(char* varName, _float64* value);
int setInCommandLine_boolean(char* varName, _boolean* value);
int setInCommandLine_string(char* varName, _string* value);
int setInCommandLine_complex128(char* varName, _complex128* value);
int initConfigVarTable(void);
int installConfigVar(char* varName, char* value);
char* lookupConfigVarValue(char* varName);

#endif

