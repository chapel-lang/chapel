/*
 * Copyright 2004-2015 Cray Inc.
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

void printTabs(std::ofstream *file);

void createDocsFileFolders(std::string filename);

void printModule(std::ofstream *file, ModuleSymbol *mod, std::string name);

void printClass(std::ofstream *file, AggregateType *cl);

void printFields(std::ofstream *file, AggregateType *cl);

void inheritance(Vec<AggregateType*> *list, AggregateType *cl);

bool devOnlyFunction(FnSymbol *fn);

bool devOnlyModule(ModuleSymbol *mod);

bool isNotSubmodule(ModuleSymbol *mod);

std::string generateSphinxProject(std::string dirpath);

void generateSphinxOutput(std::string dirpath);

static void ltrimAndPrintLines(std::string s, std::ofstream *file);
