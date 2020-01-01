/*
 * Copyright 2004-2020 Cray Inc.
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
#include <set>
#include <string>
#include <vector>
#include <utility>

#include "files.h"

class ArgSymbol;
class FnSymbol;
class Symbol;
class Type;
class TypeSymbol;

enum PythonFileType {
  C_PXD, // used for C extern declarations in Cython
  PYTHON_PYX,  // used for Python translations in Cython
  C_PYX // use for C code in the .pyx file
};

// Stores arg/function symbol to element type for the arg's array or function's
// array return type
extern std::map<Symbol*, TypeSymbol*> exportedArrayElementType;

extern char libDir[FILENAME_MAX + 1];
extern std::map<TypeSymbol*, std::pair<std::string, std::string> > pythonNames;
extern std::map<TypeSymbol*, std::string> fortranKindNames;
extern std::map<TypeSymbol*, std::string> fortranTypeNames;

extern std::map<ArgSymbol*, std::string> exportedDefaultValues;
extern std::set<FnSymbol*> exportedStrRets;

void codegen_library_header(std::vector<FnSymbol*> functions);
void codegen_library_fortran(std::vector<FnSymbol*> functions);
void codegen_library_makefile();
void codegen_library_python(std::vector<FnSymbol*> functions);
void codegen_make_python_module();

void ensureLibDirExists();
void openLibraryHelperFile(fileinfo* fi,
                           const char* name,
                           const char* ext = NULL);
void closeLibraryHelperFile(fileinfo* fi, bool beautifyIt = true);
const char* getLibraryExtension();

bool isUserRoutine(FnSymbol* fn);

std::string getPythonTypeName(Type* type, PythonFileType pxd);

#endif //LIBRARY_H
