/* -*-Mode: c++;-*-
*/
#ifndef _driver_H_
#define _driver_H_

#include <stdio.h>

#define DEFAULT_SYSTEM_DIR      "."

// Begin temporary development flags
extern bool checkAnalysisTypeinfo;
// End temporary development flags

extern int fdce_if1;
extern int finline;
extern int fgraph;
extern int fgraph_constants;
extern int fgraph_frequencies;
extern char prelude_filename[FILENAME_MAX];
extern char system_dir[FILENAME_MAX];
extern int print_call_depth;
extern int parser_verbose_non_prelude;

extern bool analyzeAST;
extern int debugParserLevel;
extern bool suppressCodegen;
extern bool developer;
extern int scoping_test;

extern bool ignore_errors;

void do_analysis(char *fn);

#endif
