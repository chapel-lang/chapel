#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chplcomm.h"
#include "chplexit.h"
#include "chplrt.h"
#include "error.h"

// Helper functions

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

static int usingGDBFile = 0;
static const char* gdbfilename = "./.chpl.gdb.commands";

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


static void createGDBFile(int argc, char* argv[], int gdbArgNo) {
  FILE* gdbfile = openfile(gdbfilename, "w");
  int i;

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


// Chapel interface

int _chpl_comm_user_invocation(int argc, char* argv[]) {
  return 1;
}

int _chpl_comm_default_num_locales(void) {
  return 1;
}

void _chpl_comm_verify_num_locales(_int32 proposedNumLocales) {
  if (proposedNumLocales != 1) {
    _printError("Only 1 locale may be used for CHPL_COMM layer 'none'", 0, 0);
  }
}

char** _chpl_comm_create_argcv(_int32 numLocales, int argc, char* argv[],
                               int* commArgc) {
  *commArgc = argc;
  return argv;
}

void _chpl_comm_init(int *argc_p, char ***argv_p, int runInGDB) {
  if (runInGDB == 0) {
    _numLocales = 1;
    _localeID = 0;
  } else {
    char* command;
    int status;

    createGDBFile(*argc_p, *argv_p, runInGDB);
    command = _glom_strings(4, "gdb -q ", (*argv_p)[0]," -x ", gdbfilename);
    status = mysystem(command, "running gdb", 0);

    _chpl_exit_all(status);
  }
}

void _chpl_comm_rollcall(void) {
  _chpl_msg(2, "executing on a single locale\n");
}

void _chpl_comm_barrier(const char *msg) { }

static void _chpl_comm_exit_common(void) {
  if (usingGDBFile) {
    char* command = _glom_strings(2, "rm ", gdbfilename);
    usingGDBFile = 0;  // to avoid infinite recursion if this fails
    mysystem(command, "removing gdb commands file", 0);
  }
}

void _chpl_comm_exit_any(int status) {
  _chpl_comm_exit_common();
}

void _chpl_comm_exit_all(int status) {
  _chpl_comm_exit_common();
}

void  _chpl_comm_put(void* addr, _int32 locale, void* raddr, _int32 size) {
  memcpy(raddr, addr, size);
}

void  _chpl_comm_get(void* addr, _int32 locale, void* raddr, _int32 size) {
  memcpy(addr, raddr, size);
}

void _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  (*f)(arg);
}

void _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  (*f)(arg);
}
