#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

#define _config_has_value(v,m) (lookupSetValue(v,m) != NULL)
#define _config_get_value(v, m) (lookupSetValue(v,m))

void addToConfigList(char* currentArg, int isSingleArg);
int askedToParseArgs(void);
void parseConfigArgs(void);
void printHelpMessage(void);
int askedToPrintHelpMessage(void);
void initConfigVarTable(void);
void printConfigVarTable(void);
void initSetValue(char* varName, char* value, char* moduleName);
char* lookupSetValue(char* varName, char* moduleName);
void installConfigVar(char* varName, char* value, char* moduleName);
int setInCommandLine_int32(char* varName, _int32* value, char* moduleName);
int setInCommandLine_uint32(char* varName, _uint32* value, char* moduleName);
int setInCommandLine_int64(char* varName, _int64* value, char* moduleName);
int setInCommandLine_uint64(char* varName, _uint64* value, char* moduleName);
int setInCommandLine_real64(char* varName, _real64* value, char* moduleName);
int setInCommandLine_bool(char* varName, _bool* value, char* moduleName);
int setInCommandLine_string(char* varName, _string* value, char* moduleName);
int setInCommandLine_complex128( char* varName, _complex128* value, char* moduleName);

#endif

