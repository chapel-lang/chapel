#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"

char executableFilename[FILENAME_MAX] = "a.out";
char saveCDir[FILENAME_MAX] = "";

static char* tmpdirname = NULL;
static char* intDirName; // directory for intermediates; tmpdir or saveCDir

static const int MAX_CHARS_PER_PID = 32;

static FILE* makefile;
static char* intExeFilename;


void createTmpDir(void) {
  char* commandExplanation;

  if (strcmp(saveCDir, "") == 0) {
    const char* tmpdirprefix = "/tmp/chpl-deleteme.";

    pid_t mypid = getpid();
#ifdef DEBUGTMPDIR
    mypid = 0;
#endif

    char mypidstr[MAX_CHARS_PER_PID];
    snprintf(mypidstr, MAX_CHARS_PER_PID, "%d", mypid);

    tmpdirname = glomstrings(2, tmpdirprefix, mypidstr);

    intDirName = tmpdirname;
    commandExplanation = "making temporary directory";
  } else {
    intDirName = saveCDir;
    commandExplanation = "ensuring --savec directory exists";
  }

  const char* mkdircommand = "mkdir ";
  const char* redirect = " > /dev/null 2>&1";
  const char* command = glomstrings(3, mkdircommand, intDirName, redirect);

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
      INT_FATAL(NULL, "tmp directory name looks fishy");
    }
    const char* rmdircommand = "rm -r ";
    char* command = glomstrings(2, rmdircommand, tmpdirname);

    mysystem(command, "removing temporary directory");
    tmpdirname = NULL;
  }
#endif

  inDeleteTmpDir = 0;
}


static char* genIntFilename(char* filename) {
  char* slash = "/";
  char* newfilename = glomstrings(3, intDirName, slash, filename);

  return newfilename;
}


static char* stripdirectories(char* filename) {
  char* filenamebase = strrchr(filename, '/');

  if (filenamebase == NULL) {
    filenamebase = filename;
  } else {
    filenamebase++;
  }
  char* strippedname = glomstrings(1, filenamebase);

  return strippedname;
}


static char* genoutfilename(char* infilename) {
  char* infilenamebase = stripdirectories(infilename);
  
  char* outfilename = genIntFilename(infilenamebase);

  // assumes file suffix is .chpl
  char* finalsuffix = strrchr(outfilename, '.');
  finalsuffix+=2;
  *finalsuffix = '\0';

  return outfilename;
}


static FILE* openfile(char* outfilename) {
  FILE* outfile;

  outfile = fopen(outfilename, "w");
  if (outfile == NULL) {
    char* errorstr = "opening ";
    char* errormsg = glomstrings(4, errorstr, outfilename, ": ", 
				 strerror(errno));

    fail(errormsg);
  }

  return outfile;
}


void closefile(FILE* thefile) {
  if (fclose(thefile) != 0) {
    char* errorstr = "closing file: ";
    char* errormsg = glomstrings(2, errorstr, strerror(errno));

    fail(errormsg);
  }
}


FILE* openoutfile(char* infilename) {
  char* outfilename = genoutfilename(infilename);
  fprintf(makefile, "\t%s \\\n", outfilename);

  return openfile(outfilename);
}


static char* compilerdir2runtimedir(char* compilerDir) {
  char* runtimedirname = "../runtime";
  char* runtimeDir = glomstrings(2, compilerDir, runtimedirname);
    
  return runtimeDir;
}


static void genMakefileHeader(char* srcfilename, char* compilerDir) {
  char* runtimedir = compilerdir2runtimedir(compilerDir);

  char* strippedExeFilename = stripdirectories(executableFilename);
  intExeFilename = genIntFilename(strippedExeFilename);
  fprintf(makefile, "BINNAME = %s\n", intExeFilename);
  fprintf(makefile, "CHPLRTDIR = %s\n", runtimedir);
  fprintf(makefile, "CHPLSRC = \\\n");
}


FILE* openMakefile(char* srcfilename, char* compilerDir) {
  char* makefilename = genIntFilename("Makefile");
  makefile = openfile(makefilename);
  genMakefileHeader(srcfilename, compilerDir);

  return makefile;
}


void closeMakefile(void) {
  fprintf(makefile, "\n");
  fprintf(makefile, "include $(CHPLRTDIR)/etc/Makefile.include\n");

  closefile(makefile);
}


void makeAndCopyBinary(void) {
  char* command = glomstrings(3, "make -f ", intDirName, "/Makefile");
  mysystem(command, "compiling generated source");

  command = glomstrings(4, "cp ", intExeFilename, " ", executableFilename);
  mysystem(command, "copying binary to final directory");
}


