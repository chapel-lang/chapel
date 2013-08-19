#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

void printHelpMessage(void);
void initConfigVarTable(void);
void printConfigVarTable(void);
void initSetValue(const char* varName, const char* value, 
                  const char* moduleName, int32_t lineno, chpl_string filename);
const char* lookupSetValue(const char* varName, const char* moduleName);
void installConfigVar(const char* varName, const char* value, 
                      const char* moduleName);

int handlePossibleConfigVar(int* argc, char* argv[], int argnum, 
                            int32_t lineno, chpl_string filename);
void parseConfigFile(const char* configFilename, 
                     int32_t lineno, chpl_string filename);

chpl_bool chpl_config_has_value(chpl_string v, chpl_string m);
chpl_string chpl_config_get_value(chpl_string v, chpl_string m);

extern const int mainHasArgs;
extern chpl_main_argument chpl_gen_main_arg;



#endif

