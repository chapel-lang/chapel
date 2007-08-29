#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"
#include "chplexit.h"
#include "chplrt.h"
#include "error.h"
#include "gdb.h"

static int mysystem(const char* command, const char* description, 
                    int ignorestatus) {
  int status = system(command);

  if (status == -1) {
    _printError("system() fork failed", 0, "(command-line)");
  } else if (status != 0 && !ignorestatus) {
    _printError(description, 0, "(command-line)");
  }

  return status;
}


static FILE* openfile(const char* outfilename, const char* mode) {
  FILE* outfile;

  outfile = fopen(outfilename, mode);
  if (outfile == NULL) {
    const char* errorstr = "opening ";
    char* errormsg = _glom_strings(4, errorstr, outfilename, ": ", 
                                   strerror(errno));

    _printError(errormsg, 0, "(command-line)");
  }

  return outfile;
}


static void closefile(FILE* thefile) {
  if (fclose(thefile) != 0) {
    const char* errorstr = "closing file: ";
    char* errormsg = _glom_strings(2, errorstr, strerror(errno));

    _printError(errormsg, 0, "(command-line)");
  }
}


static int usingGDBFile = 0;
static const char* gdbfilename = "./.chpl.gdb.commands";


static void createGDBFile(int argc, char* argv[]) {
  FILE* gdbfile = openfile(gdbfilename, "w");
  int i;

  int gdbArgNo = _runInGDB();
  fprintf(gdbfile, "set args");
  for (i=1; i<argc; i++) {
    if (i != gdbArgNo) {
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
  fprintf(gdbfile, "    call cleanup_for_exit()\n");
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
  usingGDBFile = 1;
}


void removeGDBFile() {
  if (usingGDBFile) {
    char* command = _glom_strings(2, "rm ", gdbfilename);
    usingGDBFile = 0;  // to avoid infinite recursion if this fails
    mysystem(command, "removing gdb commands file", 0);
  }
}


void runInGDB(int argc, char* argv[]) {
  char* command;
  int status;

  createGDBFile(argc, argv);
  command = _glom_strings(4, "gdb -q ", argv[0]," -x ", gdbfilename);
  status = mysystem(command, "running gdb", 0);

  _chpl_exit_all(status);
}
