/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _misc_H_
#define _misc_H_

#include "ast.h"
#include "dparse.h"
#include "var.h"

void clean_exit(int status);

int show_error(char *str, d_loc_t &loc, ...);
int show_error(char *str, AST *a, ...);
int show_error(char *str, Var *v, ...);
int buf_read(char *pathname, char **buf, int *len);
void fail(char *str, ...);
char *loc_string(d_loc_t &l);
// char *dupstr(char *s, char *e = 0); in map.h because of template's in header hell


// INT_FATAL(ast, format, ...)
//   where ast         == AST* or NULL
//         format, ... == normal printf stuff
// results in something like:
// INTERNAL ERROR in compilerSrc.c (lineno): your text here (usrSrc:usrLineno)

#define INT_FATAL setupIntError(__FILE__, __LINE__); intFatal

void setupIntError(char* filename, int lineno);
void intFatal(AST* ast, char* fmt, ...);

void startCatchingSignals(void);
void stopCatchingSignals(void);

#endif
