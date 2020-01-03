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

#include <string>

#include "symbol.h"

void createDocsFileFolders(std::string filename);

void printModule(std::ofstream *file, ModuleSymbol *mod, unsigned int tabs, std::string parentName);

void printClass(std::ofstream *file, AggregateType *cl, unsigned int tabs);

void printFields(std::ofstream *file, AggregateType *cl, unsigned int tabs);

bool devOnlyFunction(FnSymbol *fn);

bool devOnlyModule(ModuleSymbol *mod);

bool isNotSubmodule(ModuleSymbol *mod);

static void makeDir(const char* dirpath);

static bool existsAndDir(const char* dirpath);

static std::string generateSphinxProject(std::string dirpath);

static void generateSphinxOutput(std::string sphinxDir, std::string outputDir);

static std::string filenameFromMod(ModuleSymbol *mod, std::string docsWorkDir);
