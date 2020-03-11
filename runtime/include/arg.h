/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _arg_H_
#define _arg_H_

#include "chpltypes.h" // for c_string

#include <stdint.h> // for int32_t

//
// defined in arg.c
//
extern int32_t blockreport;
extern int32_t taskreport;

typedef enum {
  parse_normally,
  parse_dash_E
} chpl_parseArgsMode_t;

void parseNumLocales(const char* numPtr, int32_t lineno, int32_t filename);
void parseArgs(chpl_bool isLauncher, chpl_parseArgsMode_t mode,
               int* argc, char* argv[]);
int32_t getArgNumLocales(void);
int32_t chpl_baseUniqueLocaleID(int32_t r);
int _runInGDB(void);
int _runInLLDB(void);
int chpl_specify_locales_error(void);

//
// defined with main()
//
int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, int32_t filename);
void printAdditionalHelp(void);

void printHelpTable(void);

#endif
