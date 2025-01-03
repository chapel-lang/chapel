/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _files_H_
#define _files_H_

#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <functional>
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

void codegen_makefile(fileinfo* mainfile, const char** tmpbinname=NULL,
                      const char** tmpservername=NULL,
                      bool skip_compile_link=false,
                      const std::vector<const char *>& splitFiles
                        = std::vector<const char*>());

void ensureDirExists(const char* dirname, const char* explanation,
                     bool checkWriteable = true);
const char* getCwd();
void deleteDir(const char* dirname);
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
std::vector<std::string> getChplFilenames();
bool isChplSource(const char* filename);
bool isCHeader(const char* filename);
bool isCSource(const char* filename);
bool isObjFile(const char* filename);
bool isDynoLib(const char* filename);
void addSourceFiles(int numFilenames, const char* filename[]);
void addSourceFile(const char* filename, const char* modFilename);
void assertSourceFilesFound();
const char* nthFilename(int i);
// Functions to add C library or include dir information.
// If running in driver compilation phase and the information is not from
// parsing the command line, these will also save to a tmp dir for later use.
void addLibPath(const char* filename, bool fromCmdLine = false);
void addLibFile(const char* filename, bool fromCmdLine = false);
void addIncInfo(const char* incDir, bool fromCmdLine = false);

// Save (append) provided string into the given tmp file.
// For storing information that needs to be saved between driver phases.
void saveDriverTmp(const char* tmpFilePath, const char* stringToSave,
                   bool appendNewline = true);
// Like saveDriverTmp, but accepts a vector of strings to save in one go without
// repeatedly opening/closing file. Newline separated by default unless
// noNewlines is true.
void saveDriverTmpMultiple(const char* tmpFilePath,
                           std::vector<const char*> stringsToSave,
                           bool noNewlines = false);
// Feed strings from the specified tmp file (one per line) into the given
// restoring function, which should copy any it needs to keep.
// For accessing information saved between driver phases with saveDriverTmp.
void restoreDriverTmp(const char* tmpFilePath,
                      std::function<void(const char*)> restoreSavedString);
// Like restoreDriverTmp, but just saves the entire contents of the file into
// the given string including newlines.
void restoreDriverTmpMultiline(
    const char* tmpFilePath,
    std::function<void(const char*)> restoreSavedString);

// Restore lib dir, lib name, and inc dir info that was saved to disk, for
// compiler-driver use.
void restoreLibraryAndIncludeInfo();
// Restore source file names that were saved to disk, for compiler-driver use.
void restoreAdditionalSourceFiles();

void genIncludeCommandLineHeaders(FILE* outfile);

const char* createDebuggerFile(const char* debugger, int argc, char* argv[]);

std::string getChplDepsApp();
bool compilingWithPrgEnv();
std::string runCommand(const std::string& command,
                       const std::string& description);

const char* filenameToModulename(const char* filename);

const char* getIntermediateDirName();

void readArgsFromCommand(std::string path, std::vector<std::string>& args);
bool readArgsFromFile(std::string path, std::vector<std::string>& cmds,
                      bool errFatal=true);
void expandInstallationPaths(std::string& arg);
void expandInstallationPaths(std::vector<std::string>& args);

bool isDirectory(const char* path);
bool pathExists(const char* path);

char*       chplRealPath(const char* path);
char*       dirHasFile(const char* dir, const char* file);
char*       findProgramPath(const char* argv0);
bool        isSameFile(const char* pathA, const char* pathB);

#endif
