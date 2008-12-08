/*****************************************************************************

This code is based on arg.h developed by John Plevyak and released as
part of his iterative flow analysis package available at SourceForge
(http://sourceforge.net/projects/ifa/).  The original code is:

Copyright (c) 1992-2006 John Plevyak.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*****************************************************************************/

#ifndef _arg_H
#define _arg_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef __alpha
#define atoll atol
#endif

/* Argument Handling
*/
struct ArgumentState;

typedef void ArgumentFunction(struct ArgumentState *arg_state, char *arg);

typedef struct {
  const char *name;
  char key;
  const char *argumentOptions;
  const char *description;
  const char *type;
  void *location;
  const char *env;
  ArgumentFunction *pfn;
} ArgumentDescription;

typedef struct ArgumentState {
  char **file_argument;
  int nfile_arguments;
  const char *program_name;
  const char *program_loc;
  ArgumentDescription *desc;
} ArgumentState;

void usage(ArgumentState *arg_state, int status, bool printEnvHelp, 
           bool printCurrentSettings);
void process_args(ArgumentState *arg_state, int argc, char **argv);
void free_args(ArgumentState *arg_state);

#endif
