#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"


int initConfigVarTable(void);
void printConfigVarTable(void);
void initSetValue(char* varName, char* value, char* moduleName);
char* lookupSetValue(char* varName, char* moduleName);
int installConfigVar(char* varName, char* value, char* moduleName);
int setInCommandLine_integer64(char* varName, _integer64* value, 
			       char* moduleName);
int setInCommandLine_float64(char* varName, _float64* value, char* moduleName);
int setInCommandLine_boolean(char* varName, _boolean* value, char* moduleName);
int setInCommandLine_string(char* varName, _string* value, char* moduleName);
int setInCommandLine_complex128(char* varName, _complex128* value, 
				char* moduleName);

#endif

