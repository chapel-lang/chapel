/*
  Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

static char save_dir[FILENAME_MAX];
static FILE *log_FILE[256];

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
  while (*arg) {
    log_tag[((uint8)*arg)]++;
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

int 
log_level(int log, int log_level, char *str, ...) {
  va_list ap;
  va_start(ap, str);
  if (!logging_level(log, log_level))
    return 0;
  int res = logit(log, str, ap);
  va_end(ap);
  return res;
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

