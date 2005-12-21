#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "beautify.h"
#include "chplalloc.h"
#include "chpltypes.h"
#include "files.h"
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"

char executableFilename[FILENAME_MAX] = "a.out";
char saveCDir[FILENAME_MAX] = "";

FILE* codefile;

static char* tmpdirname = NULL;
static char* intDirName = NULL; // directory for intermediates; tmpdir or saveCDir

static const int MAX_CHARS_PER_PID = 32;

static FILE* makefile;
static char* intExeFilename;
static int numLibFlags = 0;
static char** libFlag = NULL;


char* sysdirToChplRoot(char* systemDir) {
  char* chplroot = stringcpy(systemDir);
  char* compilerSubdir = strstr(chplroot, "compiler");
  if (!compilerSubdir) {
    INT_FATAL("Can't convert systemDir to chapel root");
  }
  char* anotherCompilerSubdir;
  do {
    anotherCompilerSubdir = strstr(compilerSubdir+1, "compiler");
    if (anotherCompilerSubdir) {
      compilerSubdir = anotherCompilerSubdir;
    }
  } while (anotherCompilerSubdir);
  if (compilerSubdir == chplroot) {
    chplroot = stringcpy(".");
  } else {
    *(compilerSubdir-1) = '\0';
  }
  return chplroot;
}


void addLibInfo(char* libName) {
  static int libSpace = 0;

  numLibFlags++;
  if (numLibFlags > libSpace) {
    libSpace = 2*numLibFlags;
    libFlag = (char**)REALLOC(libFlag, libSpace*sizeof(char*));
  }
  libFlag[numLibFlags-1] = stringcpy(libName);
}


static void createTmpDir(void) {
  char* commandExplanation;

  if (strcmp(saveCDir, "") == 0) {
    const char* tmpdirprefix = "/tmp/chpl-";
    const char* tmpdirsuffix = ".deleteme";

    pid_t mypid = getpid();
#ifdef DEBUGTMPDIR
    mypid = 0;
#endif

    char mypidstr[MAX_CHARS_PER_PID];
    snprintf(mypidstr, MAX_CHARS_PER_PID, "-%d", (int)mypid);

    struct passwd* passwdinfo = getpwuid(geteuid());
    char* userid;
    if (passwdinfo == NULL) {
      userid = "anon";
    } else {
      userid = passwdinfo->pw_name;
    }

    tmpdirname = stringcat(tmpdirprefix, userid, mypidstr, tmpdirsuffix);

    intDirName = tmpdirname;
    commandExplanation = "making temporary directory";
  } else {
    intDirName = saveCDir;
    commandExplanation = "ensuring --savec directory exists";
  }

  const char* mkdircommand = "mkdir ";
  const char* redirect = " > /dev/null 2>&1";
  const char* command = stringcat(mkdircommand, intDirName, redirect);

  mysystem(command, commandExplanation, 1);
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
    char* command = stringcat(rmdircommand, tmpdirname);

    mysystem(command, "removing temporary directory");
    tmpdirname = NULL;
  }
#endif

  inDeleteTmpDir = 0;
}


static char* genIntFilename(char* filename) {
  char* slash = "/";

  if (intDirName == NULL) {
    createTmpDir();    
  }

  char* newfilename = stringcat(intDirName, slash, filename);

  return newfilename;
}


static char* stripdirectories(char* filename) {
  char* filenamebase = strrchr(filename, '/');

  if (filenamebase == NULL) {
    filenamebase = filename;
  } else {
    filenamebase++;
  }
  char* strippedname = stringcpy(filenamebase);

  return strippedname;
}


void genCFilenames(char* modulename, char** outfilename) {
  static char* outfilesuffix = ".c";
  *outfilename = stringcat(modulename, outfilesuffix);
}


static FILE* openfile(char* outfilename, char* mode = "w") {
  FILE* outfile;

  outfile = fopen(outfilename, mode);
  if (outfile == NULL) {
    char* errorstr = "opening ";
    char* errormsg = stringcat(errorstr, outfilename, ": ", 
                                 strerror(errno));

    fail(errormsg);
  }

  return outfile;
}


static void closefile(FILE* thefile) {
  if (fclose(thefile) != 0) {
    char* errorstr = "closing file: ";
    char* errormsg = stringcat(errorstr, strerror(errno));

    fail(errormsg);
  }
}


void openfile(fileinfo* thefile, char* mode) {
  thefile->fptr = openfile(thefile->pathname, mode);
}


void closefile(fileinfo* thefile) {
  closefile(thefile->fptr);
}


FILE* openCFile(char* name) {
  name = genIntFilename(name);
  return fopen(name, "w");
}


void closeCFile(FILE* f) {
  fclose(f);
}


void openCFiles(char* modulename, fileinfo* outfile) {
  genCFilenames(modulename, &(outfile->filename));
  outfile->pathname = genIntFilename(outfile->filename);

  fprintf(makefile, "\t%s \\\n", outfile->pathname);
  
  outfile->fptr = openfile(outfile->pathname);

  codefile = outfile->fptr;
}


void closeCFiles(fileinfo* outfile) {
  closefile(outfile->fptr);
  beautify(outfile);
}


fileinfo* openTmpFile(char* tmpfilename) {
  fileinfo* newfile = (fileinfo*)MALLOC(sizeof(fileinfo));

  newfile->filename = stringcpy(tmpfilename);
  newfile->pathname = genIntFilename(tmpfilename);
  openfile(newfile, "w");

  return newfile;
}


static void genMakefileHeader(char* srcfilename, char* systemDir) {
  char* strippedExeFilename = stripdirectories(executableFilename);
  intExeFilename = genIntFilename(strippedExeFilename);
  // BLC: This condtitional is done so that cp won't complain if
  // the source and destination are the same file
  if (strcmp(executableFilename, intExeFilename) == 0) {
    intExeFilename = stringcat(intExeFilename, ".tmp");
  }
  // BLC: We generate a TMPBINNAME which is the name that will be used
  // by the C compiler in creating the executable, and is in the
  // --savec directory (a /tmp directory by default).  We then copy it
  // over to BINNAME -- the name given by the user, or a.out by
  // default -- after linking is done.  As it turns out, this saves a
  // factor of 5 or so in time in running the test system, as opposed
  // to specifying BINNAME on the C compiler command line.
  fprintf(makefile, "BINNAME = %s\n", executableFilename);
  fprintf(makefile, "TMPBINNAME = %s\n", intExeFilename);
  fprintf(makefile, "CHAPEL_ROOT = %s\n", sysdirToChplRoot(systemDir));
  fprintf(makefile, "LIBS =");
  int i;
  for (i=0; i<numLibFlags; i++) {
    fprintf(makefile, " %s", libFlag[i]);
  }
  fprintf(makefile, "\n");
  fprintf(makefile, "CHPLSRC = \\\n");
}


void openMakefile(char* srcfilename, char* systemDir) {
  char* makefilename = genIntFilename("Makefile");
  makefile = openfile(makefilename);
  genMakefileHeader(srcfilename, systemDir);
}


void closeMakefile(void) {
  fprintf(makefile, "\n");
  fprintf(makefile, "include $(CHAPEL_ROOT)/runtime/etc/Makefile.include\n");

  closefile(makefile);
}


FILE* openInputFile(char* filename) {
  return openfile(filename, "r");
}


void closeInputFile(FILE* infile) {
  closefile(infile);
}


static char** inputFilenames = {NULL};


void testInputFiles(int numFilenames, char* filename[]) {
  inputFilenames = (char**)MALLOC((numFilenames+1)*sizeof(char*));
  int i;
  for (i=0; i<numFilenames; i++) {
    FILE* testfile = openInputFile(filename[i]);
    closeInputFile(testfile);
    inputFilenames[i] = stringcpy(filename[i]);
  }
  inputFilenames[i] = NULL;
}


char* nthFilename(int i) {
  return inputFilenames[i];
}


char* createGDBFile(int argc, char* argv[]) {
  char* gdbfilename = genIntFilename("gdb.commands");
  FILE* gdbfile = openfile(gdbfilename);
  int i;

  fprintf(gdbfile, "set args");
  for (i=1; i<argc; i++) {
    if (strcmp(argv[i], "--gdb") != 0) {
      fprintf(gdbfile, " %s", argv[i]);
    }
  }
  fprintf(gdbfile, "\n");
  fprintf(gdbfile, "set $_exitcode = 's'\n");
  fprintf(gdbfile, "define hook-run\n");
  fprintf(gdbfile, "  if ($_exitcode == 'r')\n");
  fprintf(gdbfile, "    call cleanup_for_exit()\n");
  fprintf(gdbfile, "  end\n");
  fprintf(gdbfile, "  set $_exitcode = 'r'\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define hook-quit\n");
  fprintf(gdbfile, "  if ($_exitcode == 'r')\n");
  fprintf(gdbfile, "    call cleanup_for_exit(0)\n");
  fprintf(gdbfile, "  end\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define halt\n");
  fprintf(gdbfile, "  set $_exitcode = 'h'\n");
  fprintf(gdbfile, "  quit\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define rerun\n");
  fprintf(gdbfile, "  set $_exitcode = 'r'\n");
  fprintf(gdbfile, "  run\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "break gdbShouldBreakHere\n");

  closefile(gdbfile);

  return gdbfilename;
}


void makeBinary(void) {
  const char* gmakeflags = printSystemCommands ? "-f " : "-s -f ";
  char* command = stringcat("make ", gmakeflags, intDirName, 
                              "/Makefile");
  mysystem(command, "compiling generated source");
}


