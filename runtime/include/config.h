#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

#define _config_has_value(v,m) (lookupSetValue(v,m) != NULL)
#define _config_get_value(v, m) (lookupSetValue(v,m))

void printHelpMessage(void);
void initConfigVarTable(void);
void printConfigVarTable(void);
void initSetValue(const char* varName, const char* value, 
                  const char* moduleName, int32_t lineno, chpl_string filename);
char* lookupSetValue(const char* varName, const char* moduleName);
void installConfigVar(const char* varName, const char* value, 
                      const char* moduleName);

int handlePossibleConfigVar(int* argc, char* argv[], int argnum, 
                            int32_t lineno, chpl_string filename);
void parseConfigFile(const char* configFilename, 
                     int32_t lineno, chpl_string filename);

#endif

