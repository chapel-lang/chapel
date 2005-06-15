/*****************************************************************************

This code is based on beautify.cpp from the alpha version of the ZPL
compiler, 06/08/2004.  Changes were made to utilize the Chapel
compiler's file and string utilities and to make use of its vector
class rather than ZPL's generic_stack class.

The original code is:

Copyright (c) 2004 University of Washington 

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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "chplalloc.h"
#include "chpltypes.h"
#include "beautify.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"
#include "mysystem.h"
#include "vec.h"

#define ZLINEFORMAT "#line %d \"%s\"\n"
#define ZLINEINPUT "/* ZLINE: "
#define ZLINEINPUTFORMAT "/* ZLINE: %d %s"
#define ZLINEINPUTLEN strlen(ZLINEINPUT)
#define ZCMAXMAPSIZE 4096

#define FALSE 0
#define TRUE 1
#define max(x,y) ((x)>(y) ? x : y)

static Vec<int> justification;
static int depth = 0;
static int parens = 0;
static int justify = 0;
static int inquote = FALSE;
static int intick = FALSE;
static int escaped = FALSE;

static void update_state(char *line) {
  int oldstuff; /* characters up to the last open paren */
  int stuff;            /* characters since last open paren */
  char *cp;
  int oldoldstuff;

  oldstuff = justify;
  stuff = 0;
  cp = line;
  escaped = FALSE;

  while (cp[0] != '\0') {
    switch (*cp) {
    case '\\': 
      escaped = !escaped;
      stuff++;
      break;
    case '\'':
      stuff++;
      if (!escaped && !inquote) {
        intick = !intick;
      }
      escaped = FALSE;
      break;
    case '\"':
      stuff++;
      if (!escaped) {
        inquote = !inquote;
      }
      escaped = FALSE;
      break;
    case '{':
      if (!inquote && !intick) {
        if (oldstuff == -1) {
          INT_FATAL("Unbalanced curly braces:\n\t%s",line);
        }
        oldstuff = 0;   /* assume all parens have been closed */
        stuff = 0;
        depth++;
      } else {
        stuff++;
      }
      escaped = FALSE;
      break;
    case '}':
      if (!inquote && !intick) {
        if (oldstuff == -1) {
          INT_FATAL("Unbalanced curly braces:\n\t%s",line);
        }
        oldstuff = 0;   /* assume all parens have been closed */
        stuff = 0;
        depth--;
      } else {
        stuff++;
      }
      escaped = FALSE;
      break;
    case '(':
      if (!inquote && !intick) {
        justification.add(oldstuff);
        if (oldstuff == -1) {
          INT_FATAL("Unbalanced parentheses:\n\t%s",line);
        }
        oldoldstuff = oldstuff;
        oldstuff = oldoldstuff + stuff + 1;
        stuff = 0;
        parens++;
      } else {
        stuff++;
      }
      escaped = FALSE;
      break;
    case ')':
      if (!inquote && !intick) {
        oldstuff = justification.pop();
        stuff = 0;
        parens--;
      } else {
        stuff++;
      }
      escaped = FALSE;
      break;
    default:
      stuff++;
      escaped = FALSE;
      break;
    }
    cp++;
  }

  if ((parens == 0) || (oldstuff == -1)) {
    justify = 0;
  } else {
    justify = oldstuff;
  }

}

void beautify(fileinfo* origfile) {
  char line[1024];
  char *cp;
  FILE *inputfile;
  FILE *outputfile;
  char* command;
  int i;
  int new_line, indent;
  int zline;
  char zname[1024];
  int current_line;
  int total_lines;
  int old_depth;
  char* znptr;
  fileinfo* tmpfile;

  zline = -1;
  
  openfile(origfile, "r");
  inputfile = origfile->fptr;

  tmpfile = openTmpFile("beautify.tmp");
  outputfile = tmpfile->fptr;

  new_line = TRUE;
  indent = TRUE;
  total_lines = 0;
  current_line = 0;
  while (fgets(line, sizeof(line), inputfile)) {
    cp = line;
    if (new_line == TRUE) {
      indent = TRUE;

      while (isspace((int)(*cp)))               /* remove leading spaces */
        cp++;

      /* record zpl/c source line map info */
      if (!strncmp(cp, ZLINEINPUT, ZLINEINPUTLEN)) {
        sscanf(cp, ZLINEINPUTFORMAT, &zline, zname);
        znptr = strrchr(zname,'/');
        if (znptr != NULL) {
          strcpy(zname,znptr+1);
        }
        
        total_lines = max(total_lines, zline);
      }
    }
    else {
      indent = FALSE;
    }

    if (cp[strlen(cp)-1] == '\n')
      new_line = TRUE;
    else
      new_line = FALSE;

    switch (cp[0]) {
    case '\0':
      fprintf(outputfile, "\n");        /* output blank line */
        break;
    case '}':
      /*** assumes there is no open curly braces follow on the line ***/
      old_depth = depth;
      update_state(cp);         /* update state first */
      if (zline >= 0) {
        fprintf(outputfile, ZLINEFORMAT, zline, zname);
        current_line++;
      }
      for (i = 0; i < 2*(old_depth-1)+justify; i++) {
        fprintf(outputfile, " ");
      }
      fprintf(outputfile, "%s", cp);    /* output line */
      break;
    default:
      if (strncmp(cp, "case", 4) == 0)
        i = 1;
      else
        i = 0;

      if (strncmp(cp, ZLINEINPUT, ZLINEINPUTLEN)) { /* don't output ZLINE comments */
        if (zline >= 0) {
          fprintf(outputfile, ZLINEFORMAT, zline, zname);
          current_line++;
        }
        if ((indent == TRUE) && (cp[0] != '#'))
          for (i = 0; i < 2*depth+justify; i++)
            fprintf(outputfile, " ");
        else {}                         /* don't indent pre-processor stmts*/
        fprintf(outputfile, "%s", cp);  /* output line */
      } else {                          /* ignore ZLINE lines in line count */
        current_line--;
      }

      update_state(cp);         /* update state */
    }
    current_line++;
  }

  closefile(tmpfile);
  closefile(origfile);

  command = glomstrings(4, "mv ", tmpfile->pathname, " ", origfile->pathname);
  mysystem(command, "moving beautified file");
  
  if (justification.n != 0) {
    INT_FATAL( "Parentheses or curly braces are not balanced "
               "in codegen for %s.", origfile->pathname);
  }

}
