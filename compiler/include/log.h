#ifndef _log_H_
#define _log_H_

#include <cstdio>

/*
  To add a new log type, add a new LOG_XXX with a unique letter:
     #define LOG_AST 'a'

  To write to the new log:
     log(LOG_AST, "starting AST #%d\n", 1);

  To test if a log should be written:
     if (logging(LOG_AST)) printf("writing AST log");

  To write based on a "log level":
     log_level(LOG_AST, 3, "starting level 3 AST log\n");

  To test based on a "log level":
     if (logging_level(LOG_AST, 3)) printf("starting level 3 AST log\n");

  To get the FILE* for a log:
     FILE *fp = log_fp(LOG_AST);

  log levels start at 1
  logs appear in the log_dir directory.
  one level of old logs is stored in log_dir/save.

  To specify logs on the command line:
    chpl -laai
  specifies AST log level 2 and IF1 log level 1
 */

struct ArgumentState;

#ifndef NUL
 #define NUL '\0'
#endif

// Driver uses this to configure the logger
void  log_flags_arg(const ArgumentState* state, const char* arg);

void  setupLogfiles();
void  teardownLogfiles();

void  log_writeLog(const char* passName, int passNum, char logTag);

bool  deletedIdON();

extern char  log_dir   [FILENAME_MAX + 1];
extern char  log_module[FILENAME_MAX + 1];

extern bool  fLogIds;

extern int   fdump_html;
extern char  fdump_html_chpl_home[FILENAME_MAX + 1];
extern bool  fdump_html_include_system_modules;
extern bool  fdump_html_wrap_lines;
extern bool  fdump_html_print_block_IDs;

extern FILE* deletedIdHandle;
extern char  deletedIdFilename[FILENAME_MAX + 1];

#endif
