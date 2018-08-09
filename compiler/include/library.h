/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef LIBRARY_H
#define LIBRARY_H

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "files.h"
#include "symbol.h"

extern char libDir[FILENAME_MAX + 1];
extern std::map<TypeSymbol*, std::pair<std::string, std::string> > pythonNames;

void codegen_library_header(std::vector<FnSymbol*> functions);
void codegen_library_makefile();
void codegen_library_python(std::vector<FnSymbol*> functions);

void ensureLibDirExists();
void openLibraryHelperFile(fileinfo* fi,
                           const char* name,
                           const char* ext = NULL);
void closeLibraryHelperFile(fileinfo* fi, bool beautifyIt = true);
const char* getLibraryExtension();

#endif //LIBRARY_H
