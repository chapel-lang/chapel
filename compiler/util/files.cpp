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

// Get realpath on linux
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include "files.h"

#include "beautify.h"
#include "driver.h"
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"
#include "tmpdirname.h"

#include <pwd.h>
#include <unistd.h>

#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>

char               executableFilename[FILENAME_MAX + 1] = "a.out";
char               saveCDir[FILENAME_MAX + 1]           = "";

char               ccflags[256]                         = "";
char               ldflags[256]                         = "";
bool               ccwarnings                           = false;

int                numLibFlags                          = 0;
const char**       libFlag                              = NULL;

Vec<const char*>   incDirs;

// directory for intermediates; tmpdir or saveCDir
static const char* intDirName        = NULL;

static const int   MAX_CHARS_PER_PID = 32;

void addLibInfo(const char* libName) {
  static int libSpace = 0;

  numLibFlags++;

  if (numLibFlags > libSpace) {
    libSpace = 2*numLibFlags;
    libFlag = (const char**)realloc(libFlag, libSpace*sizeof(char*));
  }

  libFlag[numLibFlags-1] = astr(libName);
}

void addIncInfo(const char* incDir) {
  incDirs.add(astr(incDir));
}

void ensureDirExists(const char* dirname, const char* explanation) {
  const char* mkdircommand = "mkdir -p ";
  const char* command = astr(mkdircommand, dirname);

  mysystem(command, explanation);
}


static void removeSpacesFromString(char* str)
{
  char* src = str;
  char* dst = str;
  while (*src != '\0')
  {
    *dst = *src++;
    if (*dst != ' ')
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
  removeSpacesFromString(myuserid);

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


void deleteDir(const char* dirname) {
  const char* cmd = astr("rm -rf ", dirname);
  mysystem(cmd, astr("removing directory: ", dirname));
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

static const char* stripdirectories(const char* filename) {
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

static FILE* openfile(const char* filename,
                      const char* mode  = "w",
                      bool        fatal = true) {
  FILE* newfile;

  newfile = fopen(filename, mode);

  if (newfile == NULL) {
    const char* errorstr = "opening ";
    const char* errormsg = astr(errorstr,
                                filename, ": ",
                                strerror(errno));

    if (fatal) {
      USR_FATAL(errormsg);
    }
  }

  return newfile;
}


static void closefile(FILE* thefile) {
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
  fi->fptr = fopen(fi->pathname, "w");
}

void appendCFile(fileinfo* fi, const char* name, const char* ext) {
  if (ext)
    fi->filename = astr(name, ".", ext);
  else
    fi->filename = astr(name);

  fi->pathname = genIntermediateFilename(fi->filename);
  fi->fptr     = fopen(fi->pathname, "a+");
}

void closeCFile(fileinfo* fi, bool beautifyIt) {
  fclose(fi->fptr);
  if (beautifyIt && saveCDir[0])
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

  for (int i = 0; i < numNewFilenames; i++, cursor++) {
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

    inputFilenames[cursor] = astr(filename[i]);
  }

  inputFilenames[cursor] = NULL;

  if (!foundChplSource && fUseIPE == false)
    USR_FATAL("Command line contains no .chpl source files");
}

void addSourceFile(const char* filename) {
  const char* filenamearr[1] = {filename};
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

const std::string runUtilScript(const char* script) {
  char buffer[256];
  std::string result = "";

  FILE* pipe = popen(astr(CHPL_HOME, "/util/", script), "r");
  if (!pipe) {
    USR_FATAL(astr("running $CHPL_HOME/util/", script));
  }

  while (!feof(pipe)) {
    if (fgets(buffer, 256, pipe) != NULL) {
      result += buffer;
    }
  }
  if (pclose(pipe)) {
    USR_FATAL(astr("'$CHPL_HOME/util/", script, "' did not run successfully"));
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
                   "\t$(CC) -c -o $@ $(GEN_CFLAGS) $(COMP_GEN_CFLAGS) $<\n");
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


void codegen_makefile(fileinfo* mainfile, const char** tmpbinname, bool skip_compile_link) {
  fileinfo makefile;
  openCFile(&makefile, "Makefile");
  const char* tmpDirName = intDirName;
  const char* strippedExeFilename = stripdirectories(executableFilename);
  const char* exeExt = "";
  const char* tmpbin = "";

  fprintf(makefile.fptr, "CHPL_MAKE_HOME = %s\n\n", CHPL_HOME);
  fprintf(makefile.fptr, "TMPDIRNAME = %s\n", tmpDirName);

  // LLVM builds just use the makefile for the launcher and
  // so want to skip the actual program generation.
  if( skip_compile_link ) {
    fprintf(makefile.fptr, "SKIP_COMPILE_LINK = skip\n");
  }

  if (fLibraryCompile) {
    if (fLinkStyle==LS_DYNAMIC) exeExt = ".so";
    else exeExt = ".a";
  }
  fprintf(makefile.fptr, "BINNAME = %s%s\n\n", executableFilename, exeExt);
  // BLC: This munging is done so that cp won't complain if the source
  // and destination are the same file (e.g., a.out and ./a.out)
  tmpbin = astr(tmpDirName, "/", strippedExeFilename, ".tmp", exeExt);
  if( tmpbinname ) *tmpbinname = tmpbin;
  fprintf(makefile.fptr, "TMPBINNAME = %s\n", tmpbin);
  // BLC: We generate a TMPBINNAME which is the name that will be used
  // by the C compiler in creating the executable, and is in the
  // --savec directory (a /tmp directory by default).  We then copy it
  // over to BINNAME -- the name given by the user, or a.out by
  // default -- after linking is done.  As it turns out, this saves a
  // factor of 5 or so in time in running the test system, as opposed
  // to specifying BINNAME on the C compiler command line.

  fprintf(makefile.fptr, "COMP_GEN_CFLAGS =");
  if (ccwarnings) {
    fprintf(makefile.fptr, " $(WARN_GEN_CFLAGS)");
  }
  if (debugCCode) {
    fprintf(makefile.fptr, " $(DEBUG_CFLAGS)");
  }
  if (optimizeCCode) {
    fprintf(makefile.fptr, " $(OPT_CFLAGS)");
  }
  if (specializeCCode) {
    fprintf(makefile.fptr, " $(SPECIALIZE_CFLAGS)");
  }
  if (fieeefloat) {
    fprintf(makefile.fptr, " $(IEEE_FLOAT_GEN_CFLAGS)");
  } else {
    fprintf(makefile.fptr, " $(NO_IEEE_FLOAT_GEN_CFLAGS)");
  }
  if (fLibraryCompile && (fLinkStyle==LS_DYNAMIC))
    fprintf(makefile.fptr, " $(SHARED_LIB_CFLAGS)");
  forv_Vec(const char*, dirName, incDirs) {
    fprintf(makefile.fptr, " -I%s", dirName);
  }
  fprintf(makefile.fptr, " %s\n", ccflags);

  fprintf(makefile.fptr, "COMP_GEN_LFLAGS =");
  if (!fLibraryCompile) {
    if (fLinkStyle==LS_DYNAMIC)
      fprintf(makefile.fptr, " $(GEN_DYNAMIC_FLAG)" );
    else if (fLinkStyle==LS_STATIC)
      fprintf(makefile.fptr, " $(GEN_STATIC_FLAG)" );
  } else {
    if (fLinkStyle==LS_DYNAMIC)
      fprintf(makefile.fptr, " $(LIB_DYNAMIC_FLAG)" );
    else
      fprintf(makefile.fptr, " $(LIB_STATIC_FLAG)" );
  }
  fprintf(makefile.fptr, " %s\n", ldflags);

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
  fprintf(makefile.fptr, "\n");

  fprintf(makefile.fptr, "CHPLSRC = \\\n");
  fprintf(makefile.fptr, "\t%s \\\n\n", mainfile->pathname);
  genCFiles(makefile.fptr);
  genObjFiles(makefile.fptr);
  fprintf(makefile.fptr, "\nLIBS =");
  for (int i=0; i<numLibFlags; i++)
    fprintf(makefile.fptr, " %s", libFlag[i]);
  fprintf(makefile.fptr, "\n");

  // MPF - we want to allow the runtime to make use of debug/optimize
  // information
  if (debugCCode) {
    fprintf(makefile.fptr, "DEBUG = 1\n");
  }
  if (optimizeCCode) {
    fprintf(makefile.fptr, "OPTIMIZE = 1\n");
  }
  fprintf(makefile.fptr, "\n");
  fprintf(makefile.fptr, "\n");

  if (!fLibraryCompile) {
    fprintf(makefile.fptr,
            "include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.exe\n");
  } else {
    if (fLinkStyle == LS_DYNAMIC)
      fprintf(makefile.fptr,
              "include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.shared\n");
    else
      fprintf(makefile.fptr,
              "include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.static\n");
  }
  fprintf(makefile.fptr, "\n");
  genCFileBuildRules(makefile.fptr);
  closeCFile(&makefile, false);
}


static const char* searchPath(Vec<const char*> path, const char* filename,
                              const char* foundfile = NULL,
                              bool noWarn = false) {
  forv_Vec(const char*, dirname, path) {
    //    printf("searching %s\n", dirname);
    const char* fullfilename = astr(dirname, "/", filename);
    FILE* file = openfile(fullfilename, "r", false);
    if (file != NULL) {
      closefile(file);
      if (foundfile == NULL) {
        foundfile = fullfilename;
      } else if (!noWarn) {
        USR_WARN("Ambiguous module source file -- using %s over %s",
                 cleanFilename(foundfile),
                 cleanFilename(fullfilename));
      }
    }
  }
  return foundfile;
}

//
// These are lists representing the internal, standard, user, and
// flag-based (and envvar-based) paths respectively.  The last is
// treated somewhat differently than the others in that -M flags are
// handled first by the compiler, but should come after the user
// paths, so are added into usrModPath as a post-processing pass.
//
static Vec<const char*> intModPath;
static Vec<const char*> stdModPath;
static Vec<const char*> usrModPath;
static Vec<const char*> flagModPath;


static void addUsrDirToModulePath(const char* dir) {
  //
  // a set representing the unique directories added to the path to
  // avoid duplicates (for efficiency and clarity of error messages)
  //
  static Vec<const char*> modPathSet;

  const char* uniquedir = astr(dir);
  if (!modPathSet.set_in(uniquedir)) {
    usrModPath.add(uniquedir);
    modPathSet.set_add(uniquedir);
  }
}


//
// track directories specified via -M and CHPL_MODULE_PATH.
//
void addFlagModulePath(const char* newpath) {
  const char* uniquedir = astr(newpath);
  flagModPath.add(uniquedir);
}


//
// Once we've added all filename-based directories to the user module
// search path, we'll add all unique directories specified via -M and
// CHPL_MODULE_PATH.
//
void addDashMsToUserPath() {
  forv_Vec(const char*, dirname, flagModPath) {
    addUsrDirToModulePath(dirname);
  }
}


void setupModulePaths() {
  const char* modulesRoot = 0;

  if (fMinimalModules == true)
    modulesRoot = "modules-minimal";

  else if (fUseIPE == true)
    modulesRoot = "modules-ipe";

  else
    modulesRoot = "modules";

  intModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/internal/localeModels/",
                      CHPL_LOCALE_MODEL));
  intModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/internal/tasktable/",
                      fEnableTaskTracking ? "on" : "off"));
  intModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/internal/threads/",
                      CHPL_THREADS));
  intModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/internal/tasks/",
                      CHPL_TASKS));
  intModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/internal/comm/",
                      CHPL_COMM));
  intModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/internal"));

  stdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/standard/gen/",
                      CHPL_TARGET_PLATFORM,
                      "-", CHPL_TARGET_COMPILER));

  stdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/standard"));
  stdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/layouts"));
  stdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/dists"));
  stdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/dists/dims"));

  const char* envvarpath = getenv("CHPL_MODULE_PATH");

  if (envvarpath) {
    char path[FILENAME_MAX+1];
    strncpy(path, envvarpath, FILENAME_MAX);
    char* colon = NULL;

    do {
      char* start = colon ? colon+1 : path;
      colon = strchr(start, ':');
      if (colon) {
        *colon = '\0';
      }
      addFlagModulePath(start);
    } while (colon);
  }
}


void addModulePathFromFilename(const char* origfilename) {
  char dirname[FILENAME_MAX+1];
  strncpy(dirname, origfilename, FILENAME_MAX);
  char* lastslash = strrchr(dirname, '/');
  if (lastslash != NULL) {
    *lastslash = '\0';
    addUsrDirToModulePath(dirname);
    *lastslash = '/';
  } else {
    addUsrDirToModulePath(".");
  }
}


const char* modNameToFilename(const char* modName,
                              bool        isInternal,
                              bool*       isStandard) {
  const char* filename = astr(modName, ".chpl");
  const char* fullfilename;
  if (isInternal) {
    fullfilename = searchPath(intModPath, filename, NULL, true);
  } else {
    fullfilename = searchPath(usrModPath, filename);
    *isStandard = (fullfilename == NULL);
    fullfilename = searchPath(stdModPath, filename, fullfilename);
  }
  return  fullfilename;
}

const char* stdModNameToFilename(const char* modName) {
  const char* fullfilename = searchPath(stdModPath,
                                        astr(modName, ".chpl"),
                                        NULL);

  if (fullfilename == NULL) {
    USR_FATAL("Can't find standard module '%s'\n", modName);
  }

  return fullfilename;
}


static void helpPrintPath(Vec<const char*> path) {
  forv_Vec(const char*, dirname, path) {
    fprintf(stderr, "  %s\n", cleanFilename(dirname));
  }
}

void printModuleSearchPath() {
  fprintf(stderr, "module search dirs:\n");
  if (developer) {
    helpPrintPath(intModPath);
  }
  helpPrintPath(usrModPath);
  helpPrintPath(stdModPath);
  fprintf(stderr, "end of module search dirs\n");
}

void readArgsFromCommand(const char* cmd, std::vector<std::string> & args)
{
  // Gather information from compileline into clangArgs.
  if(FILE* fd = popen(cmd,"r")) {
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
  }
}

// would just use realpath, but it is not supported on all platforms.
static
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

// This also exists in runtime/src/sys.c
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
