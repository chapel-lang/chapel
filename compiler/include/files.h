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

#ifndef _files_H_
#define _files_H_

#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include "vec.h"

extern char executableFilename[FILENAME_MAX+1];
extern char libmodeHeadername[FILENAME_MAX+1];
extern char fortranModulename[FILENAME_MAX+1];
extern char pythonModulename[FILENAME_MAX+1];
extern char saveCDir[FILENAME_MAX+1];
extern std::string ccflags;
extern std::string ldflags;
extern bool ccwarnings;
extern std::vector<const char*> incDirs;
extern std::vector<const char*> libDirs;
extern std::vector<const char*> libFiles;

struct fileinfo {
  FILE* fptr;
  const char* filename;
  const char* pathname;
};

void codegen_makefile(fileinfo* mainfile, const char** tmpbinname=NULL, bool skip_compile_link=false, const std::vector<const char *>& splitFiles = std::vector<const char*>());

void ensureDirExists(const char* /* dirname */, const char* /* explanation */);
const char* getCwd();
const char* makeTempDir(const char* dirPrefix);
void deleteDir(const char* dirname);
void deleteTmpDir();
const char* objectFileForCFile(const char* cfile);

const char* genIntermediateFilename(const char* filename);
const char* getDirectory(const char* filename);
const char* stripdirectories(const char* filename);

void openCFile(fileinfo* fi, const char* name, const char* ext = NULL);
void closeCFile(fileinfo* fi, bool beautifyIt=true);

fileinfo* openTmpFile(const char* tmpfilename, const char* mode = "w");

void      openfile(fileinfo*   thefile,
                   const char* mode);

FILE*     openfile(const char* filename,
                   const char* mode  = "w",
                   bool        fatal = true);

void      closefile(fileinfo* thefile);
void      closefile(FILE*     thefile);

FILE* openInputFile(const char* filename);
void closeInputFile(FILE* infile);
bool isChplSource(const char* filename);
bool isCHeader(const char* filename);
bool isCSource(const char* filename);
bool isObjFile(const char* filename);
void addSourceFiles(int numFilenames, const char* filename[]);
void addSourceFile(const char* filename, const char* modFilename);
const char* nthFilename(int i);
void addLibPath(const char* filename);
void addLibFile(const char* filename);
void addIncInfo(const char* incDir);

void genIncludeCommandLineHeaders(FILE* outfile);

const char* createDebuggerFile(const char* debugger, int argc, char* argv[]);

std::string runPrintChplEnv(std::map<std::string, const char*> varMap);
std::string getVenvDir();
bool compilingWithPrgEnv();
std::string runCommand(std::string& command);

const char* filenameToModulename(const char* filename);

const char* getIntermediateDirName();

void readArgsFromCommand(std::string path, std::vector<std::string>& args);
void readArgsFromFile(std::string path, std::vector<std::string>& cmds);
void expandInstallationPaths(std::string& arg);
void expandInstallationPaths(std::vector<std::string>& args);

char*       chplRealPath(const char* path);
char*       dirHasFile(const char* dir, const char* file);
char*       findProgramPath(const char* argv0);
bool        isSameFile(const char* pathA, const char* pathB);

#endif
