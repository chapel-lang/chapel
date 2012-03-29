#include "beautify.h"
#include "files.h"
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"
#include "tmpdirname.h"
#include <cstring>
#include <cstdlib>
#include <pwd.h>
#include <cerrno>
#include <unistd.h>

char executableFilename[FILENAME_MAX+1] = "a.out";
char saveCDir[FILENAME_MAX+1] = "";
char ccflags[256] = "";
char ldflags[256] = "";
bool ccwarnings = false;


static const char* intDirName = NULL; // directory for intermediates; tmpdir or saveCDir

static const int MAX_CHARS_PER_PID = 32;

static int numLibFlags = 0;
static const char** libFlag = NULL;

Vec<const char*> incDirs;


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


static void ensureTmpDirExists(void) {
  if (saveCDir[0] == '\0') {
    if (tmpdirname == NULL) {
      const char* tmpdirprefix = "/tmp/chpl-";
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
      
      tmpdirname = astr(tmpdirprefix, userid, mypidstr, tmpdirsuffix);
      intDirName = tmpdirname;
      ensureDirExists(intDirName, "making temporary directory");
    }
  } else {
    if (intDirName != saveCDir) {
      intDirName = saveCDir;
      ensureDirExists(saveCDir, "ensuring --savec directory exists");
    }
  }
}


void deleteTmpDir(void) {
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
    const char* rmdircommand = "rm -r ";
    const char* command = astr(rmdircommand, tmpdirname);

    mysystem(command, "removing temporary directory");
    tmpdirname = NULL;
  }
#endif

  inDeleteTmpDir = 0;
}


static const char* genIntFilename(const char* filename) {
  const char* slash = "/";

  ensureTmpDirExists();    

  const char* newfilename = astr(intDirName, slash, filename);

  return newfilename;
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


static FILE* openfile(const char* filename, const char* mode = "w", 
                      bool fatal = true) {
  FILE* newfile;

  newfile = fopen(filename, mode);
  if (newfile == NULL) {
    const char* errorstr = "opening ";
    const char* errormsg = astr(errorstr, filename, ": ", 
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

  fi->pathname = genIntFilename(fi->filename);
  fi->fptr = fopen(fi->pathname, "w");
}

void appendCFile(fileinfo* fi, const char* name, const char* ext) {
  if (ext)
    fi->filename = astr(name, ".", ext);
  else
    fi->filename = astr(name);
  
  fi->pathname = genIntFilename(fi->filename);
  fi->fptr = fopen(fi->pathname, "a+");
}
void closeCFile(fileinfo* fi, bool beautifyIt) {
  fclose(fi->fptr);
  if (beautifyIt)
    beautify(fi);
}


fileinfo* openTmpFile(const char* tmpfilename, const char* mode) {
  fileinfo* newfile = (fileinfo*)malloc(sizeof(fileinfo));

  newfile->filename = astr(tmpfilename);
  newfile->pathname = genIntFilename(tmpfilename);
  openfile(newfile, mode);

  return newfile;
}


FILE* openInputFile(const char* filename) {
  return openfile(filename, "r");
}


void closeInputFile(FILE* infile) {
  closefile(infile);
}


static const char** inputFilenames = {NULL};


static bool checkSuffix(const char* filename, const char* suffix) {
  const char* dot = strrchr(filename, '.');
  return (dot && strcmp(dot+1, suffix) == 0);
}


static bool isCSource(const char* filename) {
  return checkSuffix(filename, "c");
}

static bool isCHeader(const char* filename) {
  return checkSuffix(filename, "h");
}

static bool isObjFile(const char* filename) {
  return checkSuffix(filename, "o");
}

static bool foundChplSource = false;

bool isChplSource(const char* filename) {
  bool retval = checkSuffix(filename, "chpl");
  if (retval) foundChplSource = true;
  return retval;
}

static bool isRecognizedSource(char* filename) {
  return (isCSource(filename) ||
          isCHeader(filename) ||
          isObjFile(filename) ||
          isChplSource(filename));
}


void testInputFiles(int numFilenames, char* filename[]) {
  inputFilenames = (const char**)malloc((numFilenames+1)*sizeof(char*));
  int i;
  char achar;
  for (i=0; i<numFilenames; i++) {
    if (!isRecognizedSource(filename[i])) {
      USR_FATAL(astr("file '", filename[i], 
                          "' does not have a recognized suffix"));
    }
    // WE SHOULDN"T TRY TO OPEN .h files, just .c and .chpl and .o
    if (!isCHeader(filename[i])) {
      FILE* testfile = openInputFile(filename[i]);
      if (fscanf(testfile, "%c", &achar) != 1) {
        USR_FATAL(astr("source file '", filename[i], 
                       "' is either empty or a directory"));
      }
      
      closeInputFile(testfile);
    }
    inputFilenames[i] = astr(filename[i]);
  }
  inputFilenames[i] = NULL;

  if (!foundChplSource)
    USR_FATAL("Command line contains no .chpl source files");
}


const char* nthFilename(int i) {
  return inputFilenames[i];
}


const char* createGDBFile(int argc, char* argv[]) {
  const char* gdbfilename = genIntFilename("gdb.commands");
  FILE* gdbfile = openfile(gdbfilename);
  int i;

  fprintf(gdbfile, "set args");
  for (i=1; i<argc; i++) {
    if (strcmp(argv[i], "--gdb") != 0) {
      fprintf(gdbfile, " %s", argv[i]);
    }
  }
  fprintf(gdbfile, "\n");
  closefile(gdbfile);
  mysystem(astr("cat ", CHPL_HOME, "/compiler/etc/gdb.commands >> ", 
                gdbfilename), 
           "appending gdb commands", 0);


  return gdbfilename;
}


static const char* mysystem_getresult(const char* command, 
                                      const char* description,
                                      int ignorestatus) {
  const char* systemFilename = "system.out.tmp";
  const char* fullSystemFilename = genIntFilename(systemFilename);
  char* result = (char*)malloc(256*sizeof(char));
  mysystem(astr(command, " > ", fullSystemFilename), description, ignorestatus);
  fileinfo* systemFile = openTmpFile(systemFilename, "r");
  fscanf(systemFile->fptr, "%s", result);
  closefile(systemFile);
  return astr(result);  // canonicalize
}


const char* runUtilScript(const char* script) {
  return mysystem_getresult(astr(CHPL_HOME, "/util/", script), 
                            astr("running $CHPL_HOME/util/", script), 0);
}


void makeBinary(void) {
  if (no_codegen)
    return;

  if (chplmake[0] == '\0') {
    strncpy(chplmake, runUtilScript("chplenv/chplmake"), 256);
  }
  const char* makeflags = printSystemCommands ? "-f " : "-s -f ";
  const char* command = astr(astr(chplmake, " "), makeflags, intDirName, 
                            "/Makefile");
  mysystem(command, "compiling generated source");
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
      const char* pathlessFilename = stripdirectories(inputFilename);
      const char* objFilename = genIntFilename(astr(pathlessFilename, ".o"));
      fprintf(makefile, "%s: %s FORCE\n", objFilename, inputFilename);
      fprintf(makefile, "\t$(CC) -c -o $@ $(GEN_CFLAGS) $(COMP_GEN_CFLAGS) $<\n");
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
        const char* objFilename = genIntFilename(astr(pathlessFilename, ".o"));
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


void codegen_makefile(fileinfo* mainfile, fileinfo *gpusrcfile) {
  fileinfo makefile;
  openCFile(&makefile, "Makefile");
  const char* tmpDirName = intDirName;
  const char* strippedExeFilename = stripdirectories(executableFilename);
  const char* exeExt = "";

  fprintf(makefile.fptr, "CHPL_MAKE_HOME = %s\n\n", CHPL_HOME);
  fprintf(makefile.fptr, "TMPDIRNAME = %s\n", tmpDirName);

  if (fLibraryCompile) {
    if (fLinkStyle==LS_DYNAMIC) exeExt = ".so";
    else exeExt = ".a";
  }
  fprintf(makefile.fptr, "BINNAME = %s%s\n\n", executableFilename, exeExt);
  // BLC: This munging is done so that cp won't complain if the source
  // and destination are the same file (e.g., a.out and ./a.out)
  fprintf(makefile.fptr, "TMPBINNAME = $(TMPDIRNAME)/%s.tmp%s\n", 
          strippedExeFilename, exeExt);  
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
            "$(CHPL_TAGS_UTIL) $(CHPL_TAGS_FLAGS) $(CHPL_TAGS_APPEND_FLAG) *.c *.h",
            saveCDir);
  }
  fprintf(makefile.fptr, "\n");

  fprintf(makefile.fptr, "CHPLSRC = \\\n");
  fprintf(makefile.fptr, "\t%s \\\n\n", mainfile->pathname);
  if (fGPU) {
    fprintf(makefile.fptr, "CHPL_GPU_SRC = \\\n");
    fprintf(makefile.fptr, "\t%s \\\n\n", gpusrcfile->pathname);
  }
  genCFiles(makefile.fptr);
  genObjFiles(makefile.fptr);
  fprintf(makefile.fptr, "\nLIBS =");
  for (int i=0; i<numLibFlags; i++)
    fprintf(makefile.fptr, " %s", libFlag[i]);
  fprintf(makefile.fptr, "\n");

  // MPF - we want to allow the runtime to make use of debug/optimize information
  if (debugCCode) {
    fprintf(makefile.fptr, "DEBUG = 1\n");
  }
  if (optimizeCCode) {
    fprintf(makefile.fptr, "OPTIMIZE = 1\n");
  }
  fprintf(makefile.fptr, "\n");
  fprintf(makefile.fptr, "\n");

  if (!fLibraryCompile) {
    fprintf(makefile.fptr, "include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.exe\n");
  } else {
    if (fLinkStyle == LS_DYNAMIC)
      fprintf(makefile.fptr, "include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.shared\n");
    else
      fprintf(makefile.fptr, "include $(CHPL_MAKE_HOME)/runtime/etc/Makefile.static\n");
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
                 cleanFilename(foundfile), cleanFilename(fullfilename));
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
void addDashMsToUserPath(void) {
  forv_Vec(const char*, dirname, flagModPath) {
    addUsrDirToModulePath(dirname);
  }
}


void setupModulePaths(void) {
  intModPath.add(astr(CHPL_HOME, "/modules/internal/", CHPL_THREADS));
  intModPath.add(astr(CHPL_HOME, "/modules/internal/", CHPL_TASKS));
  intModPath.add(astr(CHPL_HOME, "/modules/internal"));
  stdModPath.add(astr(CHPL_HOME, "/modules/standard/gen/", CHPL_TARGET_PLATFORM,
                      "-", CHPL_TARGET_COMPILER));
  stdModPath.add(astr(CHPL_HOME, "/modules/standard"));
  stdModPath.add(astr(CHPL_HOME, "/modules/layouts"));
  stdModPath.add(astr(CHPL_HOME, "/modules/dists"));
  stdModPath.add(astr(CHPL_HOME, "/modules/dists/dims"));
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


const char* modNameToFilename(const char* modName, bool isInternal, 
                              bool* isStandard) {
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
  const char* fullfilename = searchPath(stdModPath, astr(modName, ".chpl"), 
                                        NULL);
  if (fullfilename == NULL) {
    INT_FATAL("Can't find standard module %s\n", modName);
  }
  return fullfilename;
}


static void helpPrintPath(Vec<const char*> path) {
  forv_Vec(const char*, dirname, path) {
    fprintf(stderr, "  %s\n", cleanFilename(dirname));
  }
}

void printModuleSearchPath(void) {
  fprintf(stderr, "module search dirs:\n");
  helpPrintPath(usrModPath);
  helpPrintPath(stdModPath);
  fprintf(stderr, "end of module search dirs\n");
}
