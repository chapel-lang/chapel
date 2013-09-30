#include <cstdarg>
#include <cstring>
#include <sys/stat.h>
#include "log.h"
#include "misc.h"
#include "runpasses.h"

static char save_dir[FILENAME_MAX];
static FILE *log_FILE[128];

static Vec<char> valid_log_flags;
static Vec<char> log_flags;

void init_logs() {
  if (log_dir[strlen(log_dir)-1] != '/') 
    strcat(log_dir, "/");
  mkdir(log_dir, 0xFFF);
  strcpy(save_dir, log_dir);
  strcat(save_dir, "save/");
  mkdir(save_dir, 0xFFF);
}

void 
log_flags_arg(ArgumentState *arg_state, char *arg) {
  if (valid_log_flags.count() == 0)
    initLogFlags(valid_log_flags);

  // --log or --log= means "log every pass"
  // Actually, passes whose log letter is NUL are skipped.
  if (*arg == '\0') {   // empty
    log_flags.set_union(valid_log_flags);   // Set all.
    return;
  }

  // Parse the argument of --log=<arg> or -d<arg> for specific log flags.
  // Each flag is a single letter; see log.h for the correspondence.
  while (*arg) {
    if (valid_log_flags.set_in(*arg))
      log_flags.set_add(*arg);
    else {
      fprintf(stderr, "Unrecognized log flag: '%c'\n", *arg);
      clean_exit(1);
    }
    arg++;
  }
}

FILE *
log_fp(int log) {
  if (!log_FILE[log]) {
    char save_p[FILENAME_MAX], orig_p[FILENAME_MAX];
    strcpy(save_p, save_dir);
    char *fn = &save_p[strlen(save_p)];
    sprintf(fn, "log.%c", (char)log);
    strcpy(orig_p, log_dir);
    strcat(orig_p, fn);
    rename(orig_p, save_p);
    log_FILE[log] = fopen(orig_p, "w");
  }
  return log_FILE[log];
}

static int
logit(int log, char *str, va_list ap) {
  if (!log_FILE[log])
    log_fp(log);
  return vfprintf(log_FILE[log], str, ap);
}

bool logging(int log) {
  if (log == LOG_ANY)
    if (log_flags.count() > 0)
      return true;

  return log_flags.set_in((char)log);
}

int 
log(int log, char *str, ...) {
  va_list ap;
  va_start(ap, str);
  if (!logging(log))
    return 0;
  int res = logit(log, str, ap);
  va_end(ap);
  return res;
}

