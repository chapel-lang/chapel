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
void initSetValue(char* varName, char* value, const char* moduleName);
char* lookupSetValue(const char* varName, const char* moduleName);
void installConfigVar(const char* varName, const char* value, 
                      const char* moduleName);

#endif

