#ifndef _log_H_
#define _log_H_

#include <vec.h>
#include <cstdio>


//#include "../ifa/defs.h"

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

#ifndef NUL
 #define NUL '\0'
#endif

// Used in passlist.h to match up pass designator letters (used on the command line
// with these slightly more mnemonic #defines.

// These entries should be kept in the same order as the entries in passlist.h.
#define LOG_parse               'p'
#define LOG_checkParsed         NUL
#define LOG_readExternC         'c'
#define LOG_cleanup             'u'
#define LOG_scopeResolve        'S'
#define LOG_flattenClasses      'b'
#define LOG_docs                NUL
#define LOG_normalize           'N'
#define LOG_checkNormalized     NUL
#define LOG_buildDefaultFunctions 'D'
#define LOG_createTaskFunctions NUL
#define LOG_resolve             'R'
#define LOG_resolveIntents      'i'
#define LOG_checkResolved       NUL
#define LOG_processIteratorYields NUL
#define LOG_flattenFunctions    'e'
#define LOG_cullOverReferences  'O'
#define LOG_callDestructors     'd'
#define LOG_lowerIterators      'L'
#define LOG_parallel            'P'
#define LOG_prune               'X'
#define LOG_complex2record      'C'
#define LOG_removeUnnecessaryAutoCopyCalls 'U'
#define LOG_inlineFunctions     'I'
#define LOG_scalarReplace       'r'
#define LOG_refPropagation      'g'
#define LOG_copyPropagation     'G'
#define LOG_deadCodeElimination 'x'
#define LOG_removeWrapRecords   'w'
#define LOG_removeEmptyRecords  'm'
#define LOG_localizeGlobals     'l'
#define LOG_loopInvariantCodeMotion 'q'
#define LOG_prune2              'Y'
#define LOG_returnStarTuplesByRefArgs 's'
#define LOG_insertWideReferences 'W'
#define LOG_optimizeOnClauses   'o'
#define LOG_addInitCalls        'M'
#define LOG_insertLineNumbers   'n'
#define LOG_codegen             'E'
#define LOG_makeBinary          NUL
#define LOG_ANY -1

void init_logs();
void log_flags_arg(ArgumentState *arg_state, char *arg);
FILE *log_fp(int log);
int log(int log, char *str, ...);
bool logging(int log = LOG_ANY);    // Default means "any pass".

#endif
