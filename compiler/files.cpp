#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "geysa.h"
#include "mysystem.h"
#include "stringutil.h"

static const int MAX_CHARS_PER_PID = 32;
static char* tmpdirname = NULL;
static FILE* makefile;

char* createtmpdir(void) {
  const char* tmpdirprefix = "/tmp/chpl-deleteme.";

  pid_t mypid = getpid();
#ifdef DEBUGTMPDIR
  mypid = 0;
#endif
  char mypidstr[MAX_CHARS_PER_PID];
  snprintf(mypidstr, MAX_CHARS_PER_PID, "%d", mypid);

  tmpdirname = glomstrings(2, tmpdirprefix, mypidstr);

  const char* mkdircommand = "mkdir ";
  const char* redirect = "> /dev/null 2>&1";
  const char* command = glomstrings(3, mkdircommand, tmpdirname, redirect);

  mysystem(command, "making temporary directory", 1);

  return tmpdirname;
}


void deletetmpdir(void) {
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
}


static char* gentmpfilename(char* filename) {
  char* slash = "/";
  char* newfilename = glomstrings(3, tmpdirname, slash, filename);

  return newfilename;
}


static char* stripdirectories(char* filename) {
  char* filenamebase = strrchr(filename, '/');

  if (filenamebase == NULL) {
    filenamebase = filename;
  } else {
    filenamebase++;
  }
  char* strippedname = (char*)MALLOC((strlen(filenamebase+1)*sizeof(char)));
  strcpy(strippedname, filenamebase);

  return strippedname;
}
				     


static char* genoutfilename(char* infilename) {
  char* infilenamebase = stripdirectories(infilename);
  
  char* outfilename = gentmpfilename(infilenamebase);

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


static void genMakefileHeader(char* srcfilename, char* compilerDir,
			      char* binName) {
  char* runtimedir = compilerdir2runtimedir(compilerDir);

  fprintf(makefile, "BINNAME = %s\n", binName);
  fprintf(makefile, "CHPLRTDIR = %s\n", runtimedir);
  fprintf(makefile, "CHPLSRC = \\\n");
}


FILE* openMakefile(char* srcfilename, char* compilerDir, char* binName) {
  char* makefilename = gentmpfilename("Makefile");
  makefile = openfile(makefilename);
  genMakefileHeader(srcfilename, compilerDir, binName);

  return makefile;
}


void closeMakefile(void) {
  fprintf(makefile, "\n");
  fprintf(makefile, "include $(CHPLRTDIR)/etc/Makefile.include");

  closefile(makefile);
}
