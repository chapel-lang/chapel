/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/
#ifndef _mkdparse_H_
#define _mkdparse_H_

#include <stdlib.h>
#if defined(__cplusplus)
extern "C" {
#endif

struct Grammar;

void mkdparse(struct Grammar *g, char *grammar_pathname);
void mkdparse_from_string(struct Grammar *g, char *str);

#if defined(__cplusplus)
}
#endif

#endif
