#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "chplalloc.h"
#include "chpltypes.h"
#include "beautify.h"
#include "files.h"
#include "misc.h"
#include "mysystem.h"
#include "stringutil.h"

char executableFilename[FILENAME_MAX] = "a.out";
char saveCDir[FILENAME_MAX] = "";

FILE* codefile;
FILE* extheadfile;
FILE* intheadfile;

static char* tmpdirname = NULL;
static char* intDirName = NULL; // directory for intermediates; tmpdir or saveCDir

static const int MAX_CHARS_PER_PID = 32;

static FILE* makefile;
static char* intExeFilename;
static int numLibFlags = 0;
static char** libFlag = NULL;


void addLibInfo(char* libName) {
  static int libSpace = 0;

  numLibFlags++;
  if (numLibFlags > libSpace) {
    libSpace = 2*numLibFlags;
    libFlag = (char**)REALLOC(libFlag, libSpace*sizeof(char*));
  }
  libFlag[numLibFlags-1] = copystring(libName);
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

    tmpdirname = glomstrings(4, tmpdirprefix, userid, mypidstr, tmpdirsuffix);

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
      INT_FATAL("tmp directory name looks fishy");
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


static void genCFilenames(char* modulename, char** outfilename, 
                          char** extheadfilename, char** intheadfilename) {
  static char* outfilesuffix = ".c";
  static char* extheadsuffix = ".h";
  static char* intheadsuffix = "-internal.h";

  *outfilename = glomstrings(2, modulename, outfilesuffix);
  *extheadfilename = glomstrings(2, modulename, extheadsuffix);
  *intheadfilename = glomstrings(2, modulename, intheadsuffix);
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


static void genIfndef(FILE* outfile, char* filename) {
  char* macroname = copystring(filename);
  char* dot;
  do {
    dot = strchr(macroname, '.');
    if (dot == NULL) {
      dot = strchr(macroname, '-');
    }
    if (dot != NULL) {
      *dot = '_';
    }
  } while (dot != NULL);
  fprintf(outfile, "#ifndef _%s_\n", macroname);
  fprintf(outfile, "#define _%s_\n", macroname);
  fprintf(outfile, "\n");
}


static void genEndif(FILE* outfile) {
  fprintf(outfile, "\n");
  fprintf(outfile, "#endif\n");
}


void open_common(fileinfo* common_file) {
  codefile = common_file->fptr;
  extheadfile = common_file->fptr;
  intheadfile = common_file->fptr;
}


void openCFiles(char* modulename, fileinfo* outfile,
                fileinfo* extheader, fileinfo* intheader) {
  genCFilenames(modulename, &(outfile->filename),
                &(extheader->filename), &(intheader->filename));

  outfile->pathname = genIntFilename(outfile->filename);
  extheader->pathname = genIntFilename(extheader->filename);
  intheader->pathname = genIntFilename(intheader->filename);

  fprintf(makefile, "\t%s \\\n", outfile->pathname);
  
  outfile->fptr = openfile(outfile->pathname);
  extheader->fptr = openfile(extheader->pathname);
  intheader->fptr = openfile(intheader->pathname);

  codefile = outfile->fptr;
  extheadfile = extheader->fptr;
  intheadfile = intheader->fptr;

  genIfndef(extheader->fptr, extheader->filename);
  genIfndef(intheader->fptr, intheader->filename);
}


void closeCFiles(fileinfo* outfile, 
                 fileinfo* extheadfile, fileinfo* intheadfile) {
  genEndif(extheadfile->fptr);
  genEndif(intheadfile->fptr);

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
  fprintf(makefile, "LIBS =");
  int i;
  for (i=0; i<numLibFlags; i++) {
    fprintf(makefile, " %s", libFlag[i]);
  }
  fprintf(makefile, "\n");
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


static char** inputFilenames = {NULL};


void testInputFiles(int numFilenames, char* filename[]) {
  inputFilenames = (char**)MALLOC((numFilenames+1)*sizeof(char*));
  int i;
  for (i=0; i<numFilenames; i++) {
    FILE* testfile = openInputFile(filename[i]);
    closeInputFile(testfile);
    inputFilenames[i] = copystring(filename[i]);
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
  fprintf(gdbfile, "    call cleanup()\n");
  fprintf(gdbfile, "  end\n");
  fprintf(gdbfile, "  set $_exitcode = 'r'\n");
  fprintf(gdbfile, "end\n");
  fprintf(gdbfile, "define hook-quit\n");
  fprintf(gdbfile, "  if ($_exitcode == 'r')\n");
  fprintf(gdbfile, "    call cleanup(0)\n");
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


