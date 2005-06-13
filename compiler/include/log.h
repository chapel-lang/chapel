/* -*-Mode: c++;-*-
*/

#ifndef _log_H_
#define _log_H_

#include "extern.h"

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

class ArgumentState;

#define LOG_AST         'a'
#define LOG_IF1         'i'
#define LOG_TEST_FA     'F'
#define LOG_DISPATCH    'd'

EXTERN char log_dir[FILENAME_MAX] EXTERN_INIT("./log");
EXTERN char log_tag[256];

#define logging_level(_log, _level) (log_tag[(unsigned char)(_log)] >= _level)
#define logging(_log) logging_level(_log, 1)

void init_logs();
void log_flags_arg(ArgumentState *arg_state, char *arg);
FILE *log_fp(int log);
int log(int log, char *str, ...);
int log_level(int log, int level, char *str, ...);


#endif
