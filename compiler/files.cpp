#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "beautify.h"
#include "files.h"
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"

char executableFilename[FILENAME_MAX] = "a.out";
char saveCDir[FILENAME_MAX] = "";

static char* tmpdirname = NULL;
static char* intDirName = NULL; // directory for intermediates; tmpdir or saveCDir

static const int MAX_CHARS_PER_PID = 32;

static FILE* makefile;
static char* intExeFilename;


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
    snprintf(mypidstr, MAX_CHARS_PER_PID, "%d", mypid);

    tmpdirname = glomstrings(3, tmpdirprefix, mypidstr, tmpdirsuffix);

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

  if (intDirName == NULL) {
    createTmpDir();    
  }

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
  char* strippedname = copystring(filenamebase);

  return strippedname;
}


static char* stripextension(char* filename) {
  char* newfilename = copystring(filename);
  char* suffix = strrchr(newfilename, '.');
  if (suffix != NULL) {
    *suffix = '\0';
  }
  return newfilename;
}


static void genCFilenames(char* infilename, char** outfilename, 
			  char** extheadfilename, char** intheadfilename) {
  static char* outfilesuffix = ".c";
  static char* extheadsuffix = ".h";
  static char* intheadsuffix = "-internal.h";

  char* infilenamebase = stripextension(stripdirectories(infilename));

  *outfilename = glomstrings(2, infilenamebase, outfilesuffix);
  *extheadfilename = glomstrings(2, infilenamebase, extheadsuffix);
  *intheadfilename = glomstrings(2, infilenamebase, intheadsuffix);
}


static FILE* openfile(char* outfilename, char* mode = "w") {
  FILE* outfile;

  outfile = fopen(outfilename, mode);
  if (outfile == NULL) {
    char* errorstr = "opening ";
    char* errormsg = glomstrings(4, errorstr, outfilename, ": ", 
				 strerror(errno));

    fail(errormsg);
  }

  return outfile;
}


static void closefile(FILE* thefile) {
  if (fclose(thefile) != 0) {
    char* errorstr = "closing file: ";
    char* errormsg = glomstrings(2, errorstr, strerror(errno));

    fail(errormsg);
  }
}


void openfile(fileinfo* thefile, char* mode) {
  thefile->fptr = openfile(thefile->pathname, mode);
}


void closefile(fileinfo* thefile) {
  closefile(thefile->fptr);
}


void openCFiles(char* infilename, fileinfo* outfile,
		fileinfo* extheadfile, fileinfo* intheadfile) {

  genCFilenames(infilename, &(outfile->filename),
		&(extheadfile->filename), &(intheadfile->filename));

  outfile->pathname = genIntFilename(outfile->filename);
  extheadfile->pathname = genIntFilename(extheadfile->filename);
  intheadfile->pathname = genIntFilename(intheadfile->filename);

  fprintf(makefile, "\t%s \\\n", outfile->pathname);
  
  outfile->fptr = openfile(outfile->pathname);
  extheadfile->fptr = openfile(extheadfile->pathname);
  intheadfile->fptr = openfile(intheadfile->pathname);
}


void closeCFiles(fileinfo* outfile, 
		 fileinfo* extheadfile, fileinfo* intheadfile) {
  closefile(outfile->fptr);
  closefile(extheadfile->fptr);
  closefile(intheadfile->fptr);

  beautify(outfile);
  beautify(extheadfile);
  beautify(intheadfile);
}


fileinfo* openTmpFile(char* tmpfilename) {
  fileinfo* newfile = (fileinfo*)MALLOC(sizeof(fileinfo));

  newfile->filename = copystring(tmpfilename);
  newfile->pathname = genIntFilename(tmpfilename);
  openfile(newfile, "w");

  return newfile;
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


void openMakefile(char* srcfilename, char* compilerDir) {
  char* makefilename = genIntFilename("Makefile");
  makefile = openfile(makefilename);
  genMakefileHeader(srcfilename, compilerDir);
}


void closeMakefile(void) {
  fprintf(makefile, "\n");
  fprintf(makefile, "include $(CHPLRTDIR)/etc/Makefile.include\n");

  closefile(makefile);
}


FILE* openInputFile(char* filename) {
  return openfile(filename, "r");
}


void closeInputFile(FILE* infile) {
  closefile(infile);
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
  fprintf(gdbfile, "    call cleanup()\n");
  fprintf(gdbfile, "  end\n");
  fprintf(gdbfile, "  set $_exitcode = 'r'\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define hook-quit\n");
  fprintf(gdbfile, "  if ($_exitcode == 'r')\n");
  fprintf(gdbfile, "    call cleanup(0)\n");
  fprintf(gdbfile, "  end\n");
  fprintf(gdbfile, "end\n");

  closefile(gdbfile);

  return gdbfilename;
}


void makeAndCopyBinary(void) {
  const char* gmakeflags = printSystemCommands ? "-f " : "-s -f ";
  char* command = glomstrings(4, "gmake ", gmakeflags, intDirName, 
			      "/Makefile");
  mysystem(command, "compiling generated source");

  command = glomstrings(4, "cp ", intExeFilename, " ", executableFilename);
  mysystem(command, "copying binary to final directory");
}


