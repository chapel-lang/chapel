/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _misc_H_
#define _misc_H_

int show_error(char *str, d_loc_t &loc, ...);
int show_error(char *str, AST *a, ...);
int show_error(char *str, Var *v, ...);
int buf_read(char *pathname, char **buf, int *len);
void fail(char *str, ...);
char *loc_string(d_loc_t &l);
// char *dupstr(char *s, char *e = 0); in map.h because of template's in header hell

#endif
