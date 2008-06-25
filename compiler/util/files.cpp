#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include "beautify.h"
#include "chpltypes.h"
#include "files.h"
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"

char executableFilename[FILENAME_MAX] = "a.out";
char saveCDir[FILENAME_MAX] = "";
char ccflags[256] = "";
char ldflags[256] = "";
bool ccwarnings = false;


//
// IF tmpdirname's name CHANGES, IT NEEDS TO CHANGE IN createGDBFile AS WELL
//
static const char* tmpdirname = NULL;
//
//                 ^^^^^^^^^^
//

static const char* intDirName = NULL; // directory for intermediates; tmpdir or saveCDir

static const int MAX_CHARS_PER_PID = 32;

static const char* intExeFilename;
static int numLibFlags = 0;
static const char** libFlag = NULL;


void addLibInfo(const char* libName) {
  static int libSpace = 0;

  numLibFlags++;
  if (numLibFlags > libSpace) {
    libSpace = 2*numLibFlags;
    libFlag = (const char**)realloc(libFlag, libSpace*sizeof(char*));
  }
  libFlag[numLibFlags-1] = astr(libName);
}


static void ensureDirExists(const char* dirname,
                            const char* explanation) {
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


static FILE* openfile(const char* filename, const char* mode = "w") {
  FILE* newfile;

  newfile = fopen(filename, mode);
  if (newfile == NULL) {
    const char* errorstr = "opening ";
    const char* errormsg = astr(errorstr, filename, ": ", 
                                     strerror(errno));

    USR_FATAL(errormsg);
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


void closeCFile(fileinfo* fi) {
  fclose(fi->fptr);
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
  char* dot;
  return ((dot = strrchr(filename, '.')) &&
          strcmp(dot+1, suffix) == 0);
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

bool isChplSource(const char* filename) {
  return checkSuffix(filename, "chpl");
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
    FILE* testfile = openInputFile(filename[i]);
    if (fscanf(testfile, "%c", &achar) != 1) {
      USR_FATAL(astr("source file '", filename[i], 
                          "' is either empty or a directory"));
    }
    
    closeInputFile(testfile);
    inputFilenames[i] = astr(filename[i]);
  }
  inputFilenames[i] = NULL;
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
  fprintf(gdbfile, "define hook-run\n");
  fprintf(gdbfile, "  if (tmpdirname != 0)\n");
  fprintf(gdbfile, "    call cleanup_for_exit()\n");
  fprintf(gdbfile, "  end\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define hook-quit\n");
  fprintf(gdbfile, "  if (tmpdirname != 0)\n");
  fprintf(gdbfile, "    call cleanup_for_exit()\n");
  fprintf(gdbfile, "  end\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define halt\n");
  fprintf(gdbfile, "  set tmpdirname = 0\n");
  fprintf(gdbfile, "  quit\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define rerun\n");
  fprintf(gdbfile, "  set tmpdirname = 0\n");
  fprintf(gdbfile, "  run\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define view\n");
  fprintf(gdbfile, "  call print_view($arg0)\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define iview\n");
  fprintf(gdbfile, "  call iprint_view($arg0)\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define nview\n");
  fprintf(gdbfile, "  call nprint_view($arg0)\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define lview\n");
  fprintf(gdbfile, "  call list_view($arg0)\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "break gdbShouldBreakHere\n");

  closefile(gdbfile);

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
  if (chplmake[0] == '\0') {
    strncpy(chplmake, runUtilScript("chplmake"), 256);
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


void codegen_makefile(fileinfo* mainfile) {
  fileinfo makefile;
  openCFile(&makefile, "Makefile");
  const char* strippedExeFilename = stripdirectories(executableFilename);
  intExeFilename = genIntFilename(strippedExeFilename);
  // BLC: This munging is done so that cp won't complain if the source
  // and destination are the same file (e.g., a.out and ./a.out)
  intExeFilename = astr(intExeFilename, ".tmp");
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
  fprintf(makefile.fptr, " %s\n", ccflags);
  fprintf(makefile.fptr, "COMP_GEN_LFLAGS =");
  fprintf(makefile.fptr, " %s\n", ldflags);
  fprintf(makefile.fptr, "BINNAME = %s\n", executableFilename);
  fprintf(makefile.fptr, "TMPBINNAME = %s\n", intExeFilename);
  fprintf(makefile.fptr, "CHAPEL_ROOT = %s\n", CHPL_HOME);
  fprintf(makefile.fptr, "TAGS_COMMAND = ");
  if (developer && saveCDir[0] && !printCppLineno) {
    fprintf(makefile.fptr, "-@which $(CHPL_TAGS_UTIL) > /dev/null 2>&1 && cd %s && cp $(CHAPEL_ROOT)/runtime/$(CHPL_TAGS_FILE) . && $(CHPL_TAGS_UTIL) $(CHPL_TAGS_FLAGS) $(CHPL_TAGS_APPEND_FLAG) *.c *.h", saveCDir);
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
  fprintf(makefile.fptr, "\n");
  fprintf(makefile.fptr, "include $(CHAPEL_ROOT)/runtime/etc/Makefile.include\n");
  fprintf(makefile.fptr, "\n");
  genCFileBuildRules(makefile.fptr);
  closeCFile(&makefile);
}

