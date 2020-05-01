/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _config_H_
#define _config_H_

#include "chpltypes.h"

void printHelpMessage(void);
void initConfigVarTable(void);
void printConfigVarTable(void);
void initSetValue(const char* varName, const char* value, 
                  const char* moduleName, int32_t lineno, int32_t filename);
const char* lookupSetValue(const char* varName, const char* moduleName);
void installConfigVar(const char* varName, const char* value, 
                      const char* moduleName, int private);

int handlePossibleConfigVar(int* argc, char* argv[], int argnum, 
                            int32_t lineno, int32_t filename);
void parseConfigFile(const char* configFilename, 
                     int32_t lineno, int32_t filename);

chpl_bool chpl_config_has_value(c_string v, c_string m);
c_string chpl_config_get_value(c_string v, c_string m);

extern const int mainHasArgs;
extern chpl_main_argument chpl_gen_main_arg;



#endif

