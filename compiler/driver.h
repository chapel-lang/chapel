/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _driver_H_
#define _driver_H_

#include <stdio.h>

#define DEFAULT_SYSTEM_DIR	"."

extern int fdce_if1;
extern int finline;
extern int fgraph;
extern int fgraph_constants;
extern int fgraph_frequencies;
extern char system_dir[FILENAME_MAX];
extern int print_call_depth;

#endif
