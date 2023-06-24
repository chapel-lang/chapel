#include "arg-helpers.h"
#include "driver.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"

void arg_fatalError(const char *msg) {
  INT_FATAL("%s", msg);
}

void arg_fatalUserError(const char *msg) {
  USR_FATAL("%s", msg);
}

const char *arg_programLoc(const char *argv0, void *mainAddr) {
  return findProgramPath(argv0);
}

void arg_fatalErrorCont(const char *msg) {
  USR_FATAL_CONT("%s", msg);
}

void arg_warn(const char *msg, const char* path) {
  USR_WARN(msg, path, nullptr);
}
