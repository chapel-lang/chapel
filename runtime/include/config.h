#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

/*** WANT:
#define _INIT_CONFIG(v, v_type, chapel_name, module_name, default_init) \
  if (!setInCommandLine##v_type(chapel_name, &v, module_name)) { \
    v = default_init; \
  }
***/

#define _INIT_CONFIG(v, v_type, chapel_name, module_name) \
  (!setInCommandLine##v_type(chapel_name, &v, module_name))

void initConfigVarTable(void);
void printConfigVarTable(void);
void initSetValue(char* varName, char* value, char* moduleName);
char* lookupSetValue(char* varName, char* moduleName);
void installConfigVar(char* varName, char* value, char* moduleName);
int setInCommandLine_integer64(char* varName, _integer64* value, 
			       char* moduleName);
int setInCommandLine_float64(char* varName, _float64* value, char* moduleName);
int setInCommandLine_boolean(char* varName, _boolean* value, char* moduleName);
int setInCommandLine_string(char* varName, _string* value, char* moduleName);
int setInCommandLine_complex128(char* varName, _complex128* value, 
				char* moduleName);

#endif

