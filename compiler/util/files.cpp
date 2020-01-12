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

// Get realpath on linux
#ifdef __linux__
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif
#endif

#include "files.h"

#include "beautify.h"
#include "driver.h"
#include "llvmVer.h"
#include "library.h"
#include "misc.h"
#include "mysystem.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "tmpdirname.h"

#ifdef HAVE_LLVM
#include "llvm/Support/FileSystem.h"
#endif

#include <pwd.h>
#include <unistd.h>

#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <map>

#include <sys/types.h>
#include <sys/stat.h>

char executableFilename[FILENAME_MAX + 1] = "";
char libmodeHeadername[FILENAME_MAX + 1]  = "";
char fortranModulename[FILENAME_MAX + 1]  = "";
char pythonModulename[FILENAME_MAX + 1]   = "";
char saveCDir[FILENAME_MAX + 1]           = "";

std::string ccflags;
std::string ldflags;

std::vector<const char*>   incDirs;
std::vector<const char*>   libDirs;
std::vector<const char*>   libFiles;

// directory for intermediates; tmpdir or saveCDir
static const char* intDirName        = NULL;

static const int   MAX_CHARS_PER_PID = 32;

static void addPath(const char* pathVar, std::vector<const char*>* pathvec) {
  char* dirString = strdup(pathVar);

  char* colon;              // used to refer to ':'s in dirString

  do {
    colon = strchr(dirString, ':'); // are there colon separators?
    if (colon != NULL) {
      *colon = '\0';                      // if so, cut the string there
      colon++;                            // and advance to the next
    }

    pathvec->push_back(astr(dirString));

    dirString = colon;                     // advance dirString
  } while (colon != NULL);
}

//
// Convert a libString of the form "foo:bar:baz" to entries in libDirs
//
void addLibPath(const char* libString) {
  addPath(libString, &libDirs);
}

void addLibFile(const char* libFile) {
  // use astr() to get a copy of the string that this vector can own
  libFiles.push_back(astr(libFile));
}

void addIncInfo(const char* incDir) {
  addPath(incDir, &incDirs);
}

void ensureDirExists(const char* dirname, const char* explanation) {
#ifdef HAVE_LLVM
  std::error_code err = llvm::sys::fs::create_directories(dirname);
  if (err) {
    USR_FATAL("creating directory %s failed: %s\n",
              dirname,
              err.message().c_str());
  }
#else
  const char* mkdircommand = "mkdir -p ";
  const char* command = astr(mkdircommand, dirname);

  mysystem(command, explanation);
#endif
}


static void removeSpacesBackslashesFromString(char* str)
{
  char* src = str;
  char* dst = str;
  while (*src != '\0')
  {
    *dst = *src++;
    if (*dst != ' ' && *dst != '\\')
        dst++;
  }
  *dst = '\0';
}


/*
 * Find the default tmp directory. Try getting the tmp dir from the ISO/IEC
 * 9945 env var options first, then P_tmpdir, then "/tmp".
 */
static const char* getTempDir() {
  const char* possibleDirsInEnv[] = {"TMPDIR", "TMP", "TEMP", "TEMPDIR"};
  for (unsigned int i = 0; i < (sizeof(possibleDirsInEnv) / sizeof(char*)); i++) {
    const char* curDir = getenv(possibleDirsInEnv[i]);
    if (curDir != NULL) {
      return curDir;
    }
  }
#ifdef P_tmpdir
  return P_tmpdir;
#else
  return "/tmp";
#endif
}


const char* makeTempDir(const char* dirPrefix) {
  const char* tmpdirprefix = astr(getTempDir(), "/", dirPrefix);
  const char* tmpdirsuffix = ".deleteme";

  pid_t mypid = getpid();
#ifdef DEBUGTMPDIR
  mypid = 0;
#endif

  char mypidstr[MAX_CHARS_PER_PID];
  snprintf(mypidstr, MAX_CHARS_PER_PID, "-%d", (int)mypid);

  struct passwd* passwdinfo = getpwuid(geteuid());
  const char* userid;
  if (passwdinfo == NULL) {
    userid = "anon";
  } else {
    userid = passwdinfo->pw_name;
  }
  char* myuserid = strdup(userid);
  removeSpacesBackslashesFromString(myuserid);

  const char* tmpDir = astr(tmpdirprefix, myuserid, mypidstr, tmpdirsuffix);
  ensureDirExists(tmpDir, "making temporary directory");

  free(myuserid); myuserid = NULL;

  return tmpDir;
}

static void ensureTmpDirExists() {
  if (saveCDir[0] == '\0') {
    if (tmpdirname == NULL) {
      tmpdirname = makeTempDir("chpl-");
      intDirName = tmpdirname;
    }
  } else {
    if (intDirName != saveCDir) {
      intDirName = saveCDir;
      ensureDirExists(saveCDir, "ensuring --savec directory exists");
    }
  }
}


#if !defined(HAVE_LLVM) || HAVE_LLVM_VER < 50
static
void deleteDirSystem(const char* dirname) {
  const char* cmd = astr("rm -rf ", dirname);
  mysystem(cmd, astr("removing directory: ", dirname));
}
#endif

#ifdef HAVE_LLVM
static
void deleteDirLLVM(const char* dirname) {
#if HAVE_LLVM_VER >= 50
  // LLVM 5 added remove_directories
  std::error_code err = llvm::sys::fs::remove_directories(dirname, false);
  if (err) {
    USR_FATAL("removing directory %s failed: %s\n",
              dirname,
              err.message().c_str());
  }
#else
  deleteDirSystem(dirname);
#endif
}
#endif



void deleteDir(const char* dirname) {
#ifdef HAVE_LLVM
  deleteDirLLVM(dirname);
#else
  deleteDirSystem(dirname);
#endif
}


void deleteTmpDir() {
  static int inDeleteTmpDir = 0; // break infinite recursion

  if (inDeleteTmpDir) {
    return;
  }
  inDeleteTmpDir = 1;

#ifndef DEBUGTMPDIR
  if (tmpdirname != NULL) {
    if (strlen(tmpdirname) < 1 ||
        strchr(tmpdirname, '*') != NULL ||
        strcmp(tmpdirname, "//") == 0) {
      INT_FATAL("tmp directory name looks fishy");
    }
    deleteDir(tmpdirname);
    tmpdirname = NULL;
  }
#endif

  inDeleteTmpDir = 0;
}


const char* genIntermediateFilename(const char* filename) {
  const char* slash = "/";

  ensureTmpDirExists();

  return astr(intDirName, slash, filename);
}

const char* getDirectory(const char* filename) {
  const char* filenamebase = strrchr(filename, '/');
  if (filenamebase == NULL) {
    return astr(".");
  } else {
    char dir[FILENAME_MAX];
    const int len = filenamebase - filename;
    strncpy(dir, filename, len);
    dir[len] = '\0';
    return astr(dir);
  }
}

const char* stripdirectories(const char* filename) {
  const char* filenamebase = strrchr(filename, '/');

  if (filenamebase == NULL) {
    filenamebase = filename;
  } else {
    filenamebase++;
  }
  const char* strippedname = astr(filenamebase);

  return strippedname;
}

const char* objectFileForCFile(const char* inputFilename) {
  const char* pathlessFilename = stripdirectories(inputFilename);
  const char* objFilename = genIntermediateFilename(astr(pathlessFilename, ".o"));
  return objFilename;
}

FILE* openfile(const char* filename,
               const char* mode,
               bool        fatal) {
  FILE* newfile = fopen(filename, mode);

  if (newfile == NULL) {
    const char* errorstr = "opening ";
    const char* errormsg = astr(errorstr,
                                filename, ": ",
                                strerror(errno));

    if (fatal == true) {
      USR_FATAL(errormsg);
    }
  }

  return newfile;
}


void closefile(FILE* thefile) {
  if (fclose(thefile) != 0) {
    const char* errorstr = "closing file: ";
    const char* errormsg = astr(errorstr, strerror(errno));

    USR_FATAL(errormsg);
  }
}


void openfile(fileinfo* thefile, const char* mode) {
  thefile->fptr = openfile(thefile->pathname, mode);
}


void closefile(fileinfo* thefile) {
  closefile(thefile->fptr);
}


void openCFile(fileinfo* fi, const char* name, const char* ext) {
  if (ext)
    fi->filename = astr(name, ".", ext);
  else
    fi->filename = astr(name);

  fi->pathname = genIntermediateFilename(fi->filename);
  openfile(fi, "w");
}

void closeCFile(fileinfo* fi, bool beautifyIt) {
  closefile(fi->fptr);
  //
  // We should beautify if (1) we were asked to and (2) either (a) we
  // were asked to save the C code or (b) we were asked to codegen cpp
  // #line information (note that this can affect the output in the
  // event of a failed C compilation whether or not the --savec option
  // is used because a C codegen error will report the Chapel line #,
  // which can be helpful.
  //
  // TODO: With some refactoring, we could simply do the #line part of
  // beautify without also improving indentation and such which could
  // save some time.
  //
  if (beautifyIt && (saveCDir[0] || printCppLineno))
    beautify(fi);
}

fileinfo* openTmpFile(const char* tmpfilename, const char* mode) {
  fileinfo* newfile = (fileinfo*)malloc(sizeof(fileinfo));

  newfile->filename = astr(tmpfilename);
  newfile->pathname = genIntermediateFilename(tmpfilename);
  openfile(newfile, mode);

  return newfile;
}


FILE* openInputFile(const char* filename) {
  return openfile(filename, "r");
}


void closeInputFile(FILE* infile) {
  closefile(infile);
}


static const char** inputFilenames = NULL;


static bool checkSuffix(const char* filename, const char* suffix) {
  const char* dot = strrchr(filename, '.');

  return (dot && strcmp(dot+1, suffix) == 0);
}


bool isCSource(const char* filename) {
  return checkSuffix(filename, "c");
}

bool isCHeader(const char* filename) {
  return checkSuffix(filename, "h");
}

bool isObjFile(const char* filename) {
  return checkSuffix(filename, "o");
}

static bool foundChplSource = false;

bool isChplSource(const char* filename) {
  bool retval = checkSuffix(filename, "chpl");
  if (retval) foundChplSource = true;
  return retval;
}

static bool isRecognizedSource(const char* filename) {
  return (isCSource(filename) ||
          isCHeader(filename) ||
          isObjFile(filename) ||
          isChplSource(filename));
}


void addSourceFiles(int numNewFilenames, const char* filename[]) {
  static int numInputFiles = 0;
  int cursor = numInputFiles;
  char achar;
  numInputFiles += numNewFilenames;
  inputFilenames = (const char**)realloc(inputFilenames,
                                         (numInputFiles+1)*sizeof(char*));

  for (int i = 0; i < numNewFilenames; i++) {
    if (!isRecognizedSource(filename[i])) {
      USR_FATAL(astr("file '",
                     filename[i],
                     "' does not have a recognized suffix"));
    }
    // WE SHOULDN"T TRY TO OPEN .h files, just .c and .chpl and .o
    if (!isCHeader(filename[i])) {
      FILE* testfile = openInputFile(filename[i]);
      if (fscanf(testfile, "%c", &achar) != 1) {
        USR_FATAL(astr("source file '",
                       filename[i],
                       "' is either empty or a directory"));
      }

      closeInputFile(testfile);
    }

    //
    // Don't add the same file twice -- it's unnecessary and can mess
    // up things like unprotected headers
    //
    bool duplicate = false;
    const char* newFilename = astr(filename[i]);
    for (int j = 0; j < cursor; j++) {
      if (inputFilenames[j] == newFilename) {  // legal due to astr()
        duplicate = true;
        break;
      }
    }
    if (duplicate) {
      numInputFiles--;
    } else {
      inputFilenames[cursor++] = newFilename;
    }
  }
  inputFilenames[cursor] = NULL;

  if (!foundChplSource)
    USR_FATAL("Command line contains no .chpl source files");
}

static const char* addCurrentDirToSourceFile(const char* filename,
                                             const char* modFilename) {
  // Do nothing if modFilename is NULL
  if (modFilename == NULL) {
    return filename;
  }

  // Do nothing if the filename is already absolute
  if (filename[0] == '/') {
    return filename;
  }

  // Do nothing if the current module's directory is "./"
  const char* modDir = getDirectory(modFilename);
  if (strcmp(modDir, ".") == 0) {
    return filename;
  }

  // If the file is a .c or .o...
  if (isCSource(filename) || isObjFile(filename)) {
    // ...and it isn't already an absolute path, add the module directory
    return astr(modDir, "/", filename);
  }

  // If the file is a .h, add the module's directory to the -I path
  if (isCHeader(filename)) {
    for_vector(const char, dir, incDirs) {
      if (dir == modDir) {
        // we've already added this -I directory, so don't do it again
        return filename;
      }
    }
    addIncInfo(modDir);
    return filename;
  }

  // otherwise, leave it as-is
  return filename;
}

void addSourceFile(const char* filename, const char* modFilename) {
  const char* filenamearr[1] = { addCurrentDirToSourceFile(filename,
                                                           modFilename)};
  addSourceFiles(1, filenamearr);
}


const char* nthFilename(int i) {
  return inputFilenames[i];
}


const char* createDebuggerFile(const char* debugger, int argc, char* argv[]) {
  const char* dbgfilename = genIntermediateFilename(astr(debugger, ".commands"));
  FILE* dbgfile = openfile(dbgfilename);
  int i;

  if (strcmp(debugger, "gdb") == 0) {
    fprintf(dbgfile, "set args");
  } else if (strcmp(debugger, "lldb") == 0) {
    fprintf(dbgfile, "settings set -- target.run-args");
  } else {
      INT_FATAL(astr("createDebuggerFile doesn't know how to handle the given "
                     "debugger: '", debugger, "'"));
  }
  for (i=1; i<argc; i++) {
    if (strcmp(argv[i], astr("--", debugger)) != 0) {
      fprintf(dbgfile, " %s", argv[i]);
    }
  }

  fprintf(dbgfile, "\n");
  closefile(dbgfile);
  mysystem(astr("cat ", CHPL_HOME, "/compiler/etc/", debugger, ".commands >> ",
                dbgfilename),
           astr("appending ", debugger, " commands"),
           false);

  return dbgfilename;
}

std::string runPrintChplEnv(std::map<std::string, const char*> varMap) {
  // Run printchplenv script, passing currently known CHPL_vars as well
  std::string command = "";

  // Pass known variables in varMap into printchplenv by appending to command
  for (std::map<std::string, const char*>::iterator ii=varMap.begin(); ii!=varMap.end(); ++ii)
  {
    command += ii->first + "=" + std::string(ii->second) + " ";
  }

  // Toss stderr away until printchplenv supports a '--suppresswarnings' flag
  command += std::string(CHPL_HOME) + "/util/printchplenv --all --internal --no-tidy --simple 2> /dev/null";

  return runCommand(command);
}

std::string getVenvDir() {
  // Runs `util/chplenv/chpl_home_utils.py --venv` and removes the newline

  std::string command = "CHPL_HOME=" + std::string(CHPL_HOME) + " python ";
  command += std::string(CHPL_HOME) + "/util/chplenv/chpl_home_utils.py --venv 2> /dev/null";

  std::string venvDir = runCommand(command);
  venvDir.erase(venvDir.find_last_not_of("\n\r")+1);

  return venvDir;
}

bool compilingWithPrgEnv() {
  return (strstr(CHPL_ORIG_TARGET_COMPILER, "cray-prgenv") != NULL);
}

std::string runCommand(std::string& command) {
  // Run arbitrary command and return result
  char buffer[256];
  std::string result = "";
  std::string error = "";

  // Call command
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) {
    error = "running " + command;
    USR_FATAL(error.c_str());
  }

  // Read output of command into result via buffer
  while (!feof(pipe)) {
    if (fgets(buffer, 256, pipe) != NULL) {
      result += buffer;
    }
  }

  if (pclose(pipe)) {
    error = command + " did not run successfully";
    USR_FATAL(error.c_str());
  }

  return result;
}

const char* getIntermediateDirName() {
  ensureTmpDirExists();

  return intDirName;
}

static void genCFiles(FILE* makefile) {
  int filenum = 0;
  int first = 1;
  while (const char* inputFilename = nthFilename(filenum++)) {
    if (isCSource(inputFilename)) {
      if (first) {
        fprintf(makefile, "CHPL_CL_C_SRCS = \\\n");
        first = 0;
      }
      fprintf(makefile, "\t%s \\\n", inputFilename);
    }
  }
  if (!first)
    fprintf(makefile, "\n");
}

static void genCFileBuildRules(FILE* makefile) {
  int filenum = 0;
  while (const char* inputFilename = nthFilename(filenum++)) {
    if (isCSource(inputFilename)) {
      const char* objFilename = objectFileForCFile(inputFilename);
      fprintf(makefile, "%s: %s FORCE\n", objFilename, inputFilename);
      fprintf(makefile,
              "\t$(CC) -c -o $@ $(GEN_CFLAGS) $(COMP_GEN_CFLAGS) $(CHPL_RT_INC_DIR) $<\n");
      fprintf(makefile, "\n");
    }
  }
  fprintf(makefile, "\n");
}


static void genObjFiles(FILE* makefile) {
  int filenum = 0;
  int first = 1;
  while (const char* inputFilename = nthFilename(filenum++)) {
    bool objfile = isObjFile(inputFilename);
    bool cfile = isCSource(inputFilename);
    if (objfile || cfile) {
      if (first) {
        fprintf(makefile, "CHPL_CL_OBJS = \\\n");
        first = 0;
      }
      if (objfile) {
        fprintf(makefile, "\t%s \\\n", inputFilename);
      } else {
        const char* pathlessFilename = stripdirectories(inputFilename);
        const char* objFilename = genIntermediateFilename(astr(pathlessFilename, ".o"));
        fprintf(makefile, "\t%s \\\n", objFilename);
      }
    }
  }
  if (!first)
    fprintf(makefile, "\n");
}


void genIncludeCommandLineHeaders(FILE* outfile) {
  int filenum = 0;
  while (const char* inputFilename = nthFilename(filenum++)) {
    if (isCHeader(inputFilename)) {
      fprintf(outfile, "#include \"%s\"\n", inputFilename);
    }
  }
}

std::string genMakefileEnvCache(void);
std::string genMakefileEnvCache(void) {
  std::string result;
  std::map<std::string, const char*>::iterator env;

  for (env = envMap.begin(); env != envMap.end(); ++env) {
    const std::string& key = env->first;
    const char* oldPrefix = "CHPL_";
    const char* newPrefix = "CHPL_MAKE_";
    INT_ASSERT(key.substr(0, strlen(oldPrefix)) == oldPrefix);
    std::string keySuffix = key.substr(strlen(oldPrefix), std::string::npos);
    std::string chpl_make_key = newPrefix + keySuffix;
    result += chpl_make_key + "=" + std::string(env->second) + "|";
  }

  return result;
}

void codegen_makefile(fileinfo* mainfile, const char** tmpbinname,
                      bool skip_compile_link,
                      const std::vector<const char*>& splitFiles) {
  const char* tmpDirName = intDirName;
  const char* strippedExeFilename = stripdirectories(executableFilename);
  const char* exeExt = getLibraryExtension();
  const char* server = "";
  const char* tmpserver = "";
  const char* tmpbin = "";
  bool startsWithLib = !strncmp(executableFilename, "lib", 3);
  bool dyn = (fLinkStyle == LS_DYNAMIC);
  std::string makeallvars;
  fileinfo makefile;

  openCFile(&makefile, "Makefile");

  // Capture different compiler directories.
  fprintf(makefile.fptr, "CHPL_MAKE_HOME = %s\n\n", CHPL_HOME);
  fprintf(makefile.fptr, "CHPL_MAKE_RUNTIME_LIB = %s\n\n", CHPL_RUNTIME_LIB);
  fprintf(makefile.fptr, "CHPL_MAKE_RUNTIME_INCL = %s\n\n", CHPL_RUNTIME_INCL);
  fprintf(makefile.fptr, "CHPL_MAKE_THIRD_PARTY = %s\n\n", CHPL_THIRD_PARTY);
  fprintf(makefile.fptr, "TMPDIRNAME = %s\n\n", tmpDirName);

  // Store chapel environment variables in a cache.
  makeallvars = genMakefileEnvCache();
  fprintf(makefile.fptr, "export CHPL_MAKE_CHPLENV_CACHE := %s\n\n",
          makeallvars.c_str());

  //
  // LLVM builds just use the makefile for the launcher and so want to skip
  // the actual program generation.
  //
  if (skip_compile_link) {
    fprintf(makefile.fptr, "SKIP_COMPILE_LINK = skip\n");
  }

  //
  // In --library compilation, put the generated library in the library
  // directory.
  //
  if (fLibraryCompile) {

    ensureLibDirExists();
    fprintf(makefile.fptr, "BINNAME = %s/", libDir);
    if (!startsWithLib) { fprintf(makefile.fptr, "lib"); }
    fprintf(makefile.fptr, "%s%s\n\n", executableFilename, exeExt);

    //
    // Now that the client and launcher are merged, the server name becomes
    // the name of the library without any extension. We munge the server
    // name with a trailing underscore just to guarantee that it's different
    // from the file name.
    //
    if (fMultiLocaleInterop) {
      server = astr(executableFilename, "_server");
      fprintf(makefile.fptr, "SERVERNAME = %s\n\n", server);
    }

  } else {
    fprintf(makefile.fptr, "BINNAME = %s%s\n\n", executableFilename, exeExt);
  }

  //
  // BLC: This munging is done so that cp won't complain if the source
  // and destination are the same file (e.g., myprogram and ./myprogram).
  //
  if (fLibraryCompile) {
    const char* pfx = startsWithLib ? "/" : "/lib";
    tmpbin = astr(tmpDirName, pfx, strippedExeFilename, ".tmp", exeExt);

    if (fMultiLocaleInterop) {
      tmpserver = astr(tmpDirName, "/", strippedExeFilename, "_server");
    }
  } else {
    tmpbin = astr(tmpDirName, "/", strippedExeFilename, ".tmp", exeExt);
  }

  // Write out the temporary filename to the caller if necessary.
  if (tmpbinname) { *tmpbinname = tmpbin; }

  //
  // BLC: We generate a TMPBINNAME which is the name that will be used
  // by the C compiler in creating the executable, and is in the
  // --savec directory (a /tmp directory by default).  We then copy it
  // over to BINNAME -- the name given by the user/default module's filename
  // -- after linking is done.  As it turns out, this saves a
  // factor of 5 or so in time in running the test system, as opposed
  // to specifying BINNAME on the C compiler command line.
  //
  fprintf(makefile.fptr, "TMPBINNAME = %s\n", tmpbin);

  if (fMultiLocaleInterop) {
    fprintf(makefile.fptr, "TMPSERVERNAME = %s\n\n", tmpserver);
  }

  // Bunch of C compiler flags.
  fprintf(makefile.fptr, "COMP_GEN_WARN = %i\n", ccwarnings);
  fprintf(makefile.fptr, "COMP_GEN_DEBUG = %i\n", debugCCode);
  fprintf(makefile.fptr, "COMP_GEN_OPT = %i\n", optimizeCCode);
  fprintf(makefile.fptr, "COMP_GEN_SPECIALIZE = %i\n", specializeCCode);
  fprintf(makefile.fptr, "COMP_GEN_FLOAT_OPT = %i\n", ffloatOpt);

  if (fMultiLocaleInterop) {
    const char* loc = "$(CHPL_MAKE_HOME)/runtime/etc/src";
    fprintf(makefile.fptr, "COMP_GEN_MLI_EXTRA_INCLUDES = -I%s\n", loc);
  }
  
  // Build a string out of include directories, for convenience.
  std::string includedirs;
  for_vector(const char, dirName, incDirs) {
    includedirs += " -I";
    includedirs += dirName;
  }

  // Compiler flags for each deliverable.
  if (fLibraryCompile && !fMultiLocaleInterop && dyn) {
    fprintf(makefile.fptr, "COMP_GEN_USER_CFLAGS = %s %s %s\n",
            "$(SHARED_LIB_CFLAGS)",
            includedirs.c_str(),
            ccflags.c_str());
  } else {
    fprintf(makefile.fptr, "COMP_GEN_USER_CFLAGS = %s %s\n",
            includedirs.c_str(),
            ccflags.c_str());
  }

  // Linker flags for each deliverable.
  const char* lmode = "";
  if (!fLibraryCompile) {
    //
    // Important that _no_ RHS is produced when the link style is default!
    // Tests will _fail_ that rely on this assumption if we do otherwise.
    //
    switch (fLinkStyle) {
    case LS_DYNAMIC:
      lmode = "$(GEN_DYNAMIC_FLAG)"; break;
    case LS_STATIC:
      lmode = "$(GEN_STATIC_FLAG)"; break;
    }
  } else if (fLibraryCompile && !fMultiLocaleInterop) {
    lmode = dyn ? "$(LIB_DYNAMIC_FLAG)" : "$(LIB_STATIC_FLAG)";
  }

  fprintf(makefile.fptr, "COMP_GEN_LFLAGS = %s %s\n",
          lmode, ldflags.c_str());

  // Block of code for generating TAGS command, developer convenience.
  fprintf(makefile.fptr, "TAGS_COMMAND = ");
  if (developer && saveCDir[0] && !printCppLineno) {
    fprintf(makefile.fptr,
            "-@which $(CHPL_TAGS_UTIL) > /dev/null 2>&1 && "
            "test -f $(CHPL_MAKE_HOME)/runtime/$(CHPL_TAGS_FILE) && "
            "cd %s && "
            "cp $(CHPL_MAKE_HOME)/runtime/$(CHPL_TAGS_FILE) . && "
            "$(CHPL_TAGS_UTIL) $(CHPL_TAGS_FLAGS) "
              "$(CHPL_TAGS_APPEND_FLAG) *.c *.h",
            saveCDir);
  }

  fprintf(makefile.fptr, "\n\n");

  // List source files needed to compile this deliverable.
  if (fMultiLocaleInterop) {

    const char* mli_client = astr(intDirName, "/", "chpl_mli_client.c");
    const char* mli_server = astr(intDirName, "/", "chpl_mli_server.c");

    // Only one source file for client (for now).
    fprintf(makefile.fptr, "CHPLSRC = \\\n");
    fprintf(makefile.fptr, "\t%s \n", mli_client);

    // The server bundle includes "_main.c", bypassing the need to include it.
    fprintf(makefile.fptr, "CHPLSERVERSRC = \\\n");
    fprintf(makefile.fptr, "\t%s \n", mli_server);

  } else {
    fprintf(makefile.fptr, "CHPLSRC = \\\n");
    fprintf(makefile.fptr, "\t%s \\\n\n", mainfile->pathname);
  }

  // List object files needed to compile this deliverable.
  fprintf(makefile.fptr, "CHPLUSEROBJ = \\\n");
  for (size_t i = 0; i < splitFiles.size(); i++) {
    fprintf(makefile.fptr, "\t%s \\\n", splitFiles[i]);
  }
  fprintf(makefile.fptr, "\n");
  
  genCFiles(makefile.fptr);
  genObjFiles(makefile.fptr);
  
  // List libraries/locations needed to compile this deliverable.
  fprintf(makefile.fptr, "\nLIBS =");
  for_vector(const char, dirName, libDirs) {
    fprintf(makefile.fptr, " -L%s", dirName);
  }

  for_vector(const char, libName, libFiles) {
    fprintf(makefile.fptr, " -l%s", libName);
  }

  if (fLinkStyle==LS_STATIC) { fprintf(makefile.fptr, " $(LIBMVEC)"); }
  fprintf(makefile.fptr, "\n\n\n");

  // Figure out the appropriate base Makefile to include.
  std::string incpath = "include $(CHPL_MAKE_HOME)/runtime/etc/";
  if (fMultiLocaleInterop) {
    incpath += dyn ? "Makefile.mli-shared" : "Makefile.mli-static";
  } else if (fLibraryCompile) {
    incpath += dyn ? "Makefile.shared" : "Makefile.static";
  } else {
    incpath += "Makefile.exe";
  }

  fprintf(makefile.fptr, "%s\n\n", incpath.c_str());

  genCFileBuildRules(makefile.fptr);
  closeCFile(&makefile, false);
}

const char* filenameToModulename(const char* filename) {
  const char* moduleName = astr(filename);
  const char* firstSlash = strrchr(moduleName, '/');

  if (firstSlash) {
    moduleName = firstSlash + 1;
  }

  return asubstr(moduleName, strrchr(moduleName, '.'));
}

void readArgsFromCommand(std::string cmd, std::vector<std::string>& args) {
  // Gather information from compileline into clangArgs.
  if(FILE* fd = popen(cmd.c_str(),"r")) {
    int ch;
    // Read arguments.
    while( (ch = getc(fd)) != EOF ) {
      // Read the next argument.
      // skip leading spaces
      while( ch != EOF && isspace(ch) ) ch = getc(fd);
      std::string arg;
      arg.push_back(ch);
      // read until space. TODO - handle quoting/spaces
      ch = getc(fd);
      while( ch != EOF && !isspace(ch) ) {
        arg += ch;
        ch = getc(fd);
      }
      // First argument is the clang install directory...
      args.push_back(arg);
    }
    fclose(fd);
  }
}

void readArgsFromFile(std::string path, std::vector<std::string>& args) {

  FILE* fd = fopen(path.c_str(), "r");
  if (!fd)
    USR_FATAL("Could not open file %s", path.c_str());

  int ch;
  // Read arguments.
  while( (ch = getc(fd)) != EOF ) {
    // Read the next argument.
    // skip leading spaces
    while( ch != EOF && isspace(ch) ) ch = getc(fd);
    std::string arg;
    arg.push_back(ch);
    // read until space. TODO - handle quoting/spaces
    ch = getc(fd);
    while( ch != EOF && !isspace(ch) ) {
      arg += ch;
      ch = getc(fd);
    }
    args.push_back(arg);
  }

  fclose(fd);
}

// Expands variables like $CHPL_HOME in the string
void expandInstallationPaths(std::string& s) {
  const char* tofix[] = {"$CHPL_RUNTIME_LIB", CHPL_RUNTIME_LIB,
                         "$CHPL_RUNTIME_INCL", CHPL_RUNTIME_INCL,
                         "$CHPL_THIRD_PARTY", CHPL_THIRD_PARTY,
                         "$CHPL_HOME", CHPL_HOME,
                         NULL};

  // For each of the patterns in tofix, find/replace all occurrences.
  for (int j = 0; tofix[j] != NULL; j += 2) {

    const char* key = tofix[j];
    const char* val = tofix[j+1];
    size_t key_len = strlen(key);
    size_t val_len = strlen(val);

    size_t off = 0;
    while (true) {
      off = s.find(key, off);
      if (off == std::string::npos)
        break; // no more occurrences to replace
      s.replace(off, key_len, val);
      off += val_len;
    }
  }
}

void expandInstallationPaths(std::vector<std::string>& args) {

  for (size_t i = 0; i < args.size(); i++) {
    std::string s = args[i];
    expandInstallationPaths(s);
    args[i] = s;
  }
}

// would just use realpath, but it is not supported on all platforms.
char* chplRealPath(const char* path)
{
  // We would really rather use
  // char* got = realpath(path, NULL);
  // but that doesn't work on some Mac OS X versions.
  char* buf = (char*) malloc(PATH_MAX);
  char* got = realpath(path, buf);
  char* ret = NULL;
  if( got ) ret = strdup(got);
  free(buf);
  return ret;
}


// Returns a "real path" to the file in the directory,
// or NULL if the file did not exist.
// The return value must be freed by the caller.
// We try to use realpath but might give up.
char* dirHasFile(const char *dir, const char *file)
{
  struct stat stats;
  int len = strlen(dir) + strlen(file) + 2;
  char* tmp = (char*) malloc(len);
  char* real;

  if( ! tmp ) INT_FATAL("no memory");

  snprintf(tmp, len, "%s/%s", dir, file);
  real = chplRealPath(tmp);
  if( real == NULL ) {
    // realpath not working on this system,
    // just use tmp.
    real = tmp;
  } else {
    free(tmp);
  }

  if( stat(real, &stats) != 0) {
    free(real);
    real = NULL;
  }

  return real;
}

// This also exists in runtime/src/qio/sys.c
// returns 0 on success.
static int sys_getcwd(char** path_out)
{
  int sz = 128;
  char* buf;

  buf = (char*) malloc(sz);
  if( !buf ) return ENOMEM;

  while( 1 ) {
    if ( getcwd(buf, sz) != NULL ) {
      break;

    } else if ( errno == ERANGE ) {
      // keep looping but with bigger buffer.
      sz *= 2;

      /*
       * Realloc may return NULL, in which case we will need to free the memory
       * initially pointed to by buf.  This is why we store the result of the
       * call in newP instead of directly into buf.  If a non-NULL value is
       * returned we update the buf pointer.
       */
      void* newP = realloc(buf, sz);

      if (newP != NULL) {
        buf = static_cast<char*>(newP);

      } else {
        free(buf);
        return ENOMEM;
      }

    } else {
      // Other error, stop.
      free(buf);
      return errno;
    }
  }

  *path_out = buf;
  return 0;
}


/*
 * Returns the current working directory. Does not report failures. Use
 * sys_getcwd() if you need error reports.
 */
const char* getCwd() {
  const char* result = getcwd(NULL, PATH_MAX);
  if (result) {
    return result;
  } else {
    return "";
  }
}


// Find the path to the running program
// (or return NULL if we couldn't figure it out).
// The return value must be freed by the caller.
char* findProgramPath(const char *argv0)
{
  char* real = NULL;
  char* path;

  /* Note - there are lots of friendly
   * but platform-specific ways to do this:
    #ifdef __linux__
      int ret;
      ret = readlink("/proc/self/exe", dst, max_dst - 1);
      // return an error if there was an error.
      if( ret < 0 ) return -1;
      // append the NULL byte
      if( ret < max_dst ) dst[ret] = '\0';
      return 0;
    #else
    #ifdef __APPLE__
      uint32_t sz = max_dst - 1;
      return _NSGetExecutablePath(dst, &sz);
    #else
      // getexe path not available.
      return -1;
    #endif
  */


  // Is argv0 an absolute path?
  if( argv0[0] == '/' ) {
    real = dirHasFile("/", argv0);
    return real;
  }

  // Is argv0 a relative path?
  if( strchr(argv0, '/') != NULL ) {
    char* cwd = NULL;
    if( 0 == sys_getcwd(&cwd) ) {
      real = dirHasFile(cwd, argv0);
    } else {
      real = NULL;
    }
    free(cwd);
    return real;
  }

  // Is argv0 just in $PATH?
  path = getenv("PATH");
  if( path == NULL ) return NULL;

  path = strdup(path);
  if( path == NULL ) return NULL;

  // Go through PATH changing ':' into '\0'
  char* start;
  char* end;
  char* path_end = path + strlen(path);

  start = path;
  while( start != NULL && start < path_end ) {
    end = strchr(start, ':');
    if( end == NULL ) end = path_end;
    else end[0] = '\0'; // replace ':' with '\0'

    real = dirHasFile(start, argv0);
    if( real ) break;

    start = end + 1;
  }

  free(path);
  return real;
}

// Return true if both files exist and
// they point to the same inode.
// (if a filesystem does not return reasonable
//  inodes, this function might return true when
//  the paths are different - so it should be interpreted
//  as "NO" or "MAYBE").
bool isSameFile(const char* pathA, const char* pathB)
{
  struct stat statsA;
  struct stat statsB;
  int rc;

  rc = stat(pathA, &statsA);
  if( rc != 0 ) return false;
  rc = stat(pathB, &statsB);
  if( rc != 0 ) return false;

  // is the inode the same?
  if( statsA.st_dev == statsB.st_dev &&
      statsA.st_ino == statsB.st_ino ) {
    return true;
  }

  return false;
}
