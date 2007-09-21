#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

#define _config_has_value(v,m) (lookupSetValue(v,m) != NULL)
#define _config_get_value(v, m) (lookupSetValue(v,m))

typedef enum { fdash, sdash, ddash } configSource;

void addToConfigList(const char* currentArg, configSource argSource);
int askedToParseArgs(void);
void parseConfigArgs(void);
void printHelpMessage(void);
void initConfigVarTable(void);
void printConfigVarTable(void);
void initSetValue(char* varName, char* value, const char* moduleName);
char* lookupSetValue(const char* varName, const char* moduleName);
void installConfigVar(const char* varName, const char* value, 
                      const char* moduleName);

// Defined in the compiler-generated code:
extern void CreateConfigVarTable(void);

#endif

