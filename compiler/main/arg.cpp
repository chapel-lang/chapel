/*****************************************************************************

This code is based on arg.cc developed by John Plevyak and released as
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


#include <stdio.h>
#include "arg.h"
#include "chpl.h"
#include "stringutil.h"

/*
Flag types:

  I = int
  P = path
  S = string
  D = double
  f = set to false
  F = set to true
  + = increment
  T = toggle
  L = int64 (long)
  N = --no-... flag, --no version sets to false
  n = --no-... flag, --no version sets to true
*/

static void 
bad_flag(char* flag) {
  fprintf(stderr, "Unrecognized flag: '%s' (use '-h' for help)\n", flag);
  exit(1);
}


static void 
missing_arg(char* currentFlag) {
  fprintf(stderr, "Missing argument for flag: '%s' (use '-h' for help)\n", 
          currentFlag);
  exit(1);
}


static void 
process_arg(ArgumentState *arg_state, int i, char ***argv, char* currentFlag) {
  char * arg = NULL;
  ArgumentDescription *desc = arg_state->desc;
  if (desc[i].type) {
    char type = desc[i].type[0];
    if (type=='F'||type=='f'||type=='N'||type=='n')
      *(bool *)desc[i].location = (type=='F'||type=='N') ? true : false;
    else if (type=='T')
      *(int *)desc[i].location = !*(int *)desc[i].location;
    else if (type == '+') 
      (*(int *)desc[i].location)++;
    else {
      arg = *++(**argv) ? **argv : *++(*argv);
      if (!arg) missing_arg(currentFlag);
      switch (type) {
        case 'I':
          *(int *)desc[i].location = atoi(arg);
          break;
        case 'D':
          *(double *)desc[i].location = atof(arg);
          break;
        case 'L':
          *(int64 *)desc[i].location = atoll(arg);
          break;
        case 'P': strncpy((char *)desc[i].location,arg, FILENAME_MAX);
          break;
        case 'S': strncpy((char *)desc[i].location,arg, atoi(desc[i].type+1));
          break;
        default:
          fprintf(stdout, "%s:bad argument description\n", 
                 arg_state->program_name);
          exit(1);
          break;
      }
      **argv += strlen(**argv)-1;
    }
  }
  if (desc[i].pfn)
    desc[i].pfn(arg_state, arg);
}


void
process_args(ArgumentState *arg_state, int argc, char **aargv) {
  int i, len;
  char *end = 0;
  char **argv = (char**)malloc((argc+1)*sizeof(char*));
  for (i = 0; i < argc; i++)
    argv[i] = _dupstr(aargv[i]);
  argv[i] = NULL;
  ArgumentDescription *desc = arg_state->desc;
  /* Grab Environment Variables */
  for (i = 0;; i++) {
    if (!desc[i].name)
      break; 
    if (desc[i].env) {
      char type = desc[i].type[0];
      char * env = getenv(desc[i].env);
      if (!env) continue;
      switch (type) {
        case '+': (*(int *)desc[i].location)++; break;
        case 'f': 
        case 'F': 
        case 'N':
        case 'n':
          *(bool *)desc[i].location = type!='f'?1:0; break;
        case 'T': *(int *)desc[i].location = !*(int *)desc[i].location; break;
        case 'I': *(int *)desc[i].location = strtol(env, NULL, 0); break;
        case 'D': *(double *)desc[i].location = strtod(env, NULL); break;
        case 'L': *(int64 *)desc[i].location = str2int64(env); break;
        case 'P': strncpy((char *)desc[i].location, env, FILENAME_MAX); break;
        case 'S': strncpy((char *)desc[i].location, env, strtol(desc[i].type+1, NULL, 0)); break;
      }
      if (desc[i].pfn)
        desc[i].pfn(arg_state, env);
    }
  }

  /*
    Grab Command Line Arguments
  */
  while (*++argv) {
    if (**argv == '-') {
      if ((*argv)[1] == '-') {
        for (i = 0;; i++) {
          if (!desc[i].name)
            bad_flag(*argv);
          if ((end = strchr((*argv)+2, '=')))
            len = end - ((*argv) + 2);
          else
            len = strlen((*argv) + 2);
          int flaglen = (int)strlen(desc[i].name);
          if (len == flaglen &&
              !strncmp(desc[i].name,(*argv)+2, len))
          {
            char* currentFlag = _dupstr(*argv);
            if (!end)
              *argv += strlen(*argv) - 1;
            else
              *argv = end;
            process_arg(arg_state, i, &argv, currentFlag);
            break;
          } else if (desc[i].type && 
                     (desc[i].type[0] == 'N' || desc[i].type[0] == 'n') &&
                     len == flaglen+3 &&
                     !strncmp("no-", (*argv)+2, 3) &&
                     !strncmp(desc[i].name,(*argv)+5,len-3)) {
            char* currentFlag = _dupstr(*argv);
            if (!end)
              *argv += strlen(*argv) - 1;
            else
              *argv = end;
            if (desc[i].type[0] == 'N') {
              desc[i].type = "f";
            } else {
              desc[i].type = "F";
            }
            process_arg(arg_state, i, &argv, currentFlag);
            if (desc[i].type[0] == 'f') {
              desc[i].type = "N";
            } else {
              desc[i].type = "n";
            }
            break;
          }
        }
      } else {
        while (*++(*argv))
          for (i = 0;; i++) {
            if (!desc[i].name)
              bad_flag((*argv)-1);
            if (desc[i].key == **argv) {
              process_arg(arg_state, i, &argv, (*argv)-1);
              break;
            }
          }
      }
    } else {
      arg_state->file_argument = (char **)realloc(
        arg_state->file_argument, 
        sizeof(char**) * (arg_state->nfile_arguments + 2));
      arg_state->file_argument[arg_state->nfile_arguments++] = *argv;
      arg_state->file_argument[arg_state->nfile_arguments] = NULL;
    }
  }
}


static void
print_n_spaces(int n) {
  int i;
  for (i = 0; i < n; i++)
    fprintf(stdout, " ");
}


static void
word_wrap_print(const char* text, int start_column, int end_column) {
  /*
   * Print the buffer "text" to stdout with all non-whitespace text at or
   * after start_column, and print at most end_column characters per line.
   * Do not break lines in the middle of words. When this function is called,
   * stdout must be in a state such that the next character fprintf will print
   * is in start_column.
   */
  int space_left = 1 + end_column - start_column;
  bool first = true;
  const char* delims = " ";
  char* save_ptr, *text_dup, *word;
  text_dup = strdup(text);
  word = strtok_r(text_dup, delims, &save_ptr);

  while (word) {
    int wordlength = strlen(word);
    if (first) {
      space_left -= fprintf(stdout, "%s", word);
      first = false;
    } else {
      if (wordlength + 1 < space_left) {
        space_left -= fprintf(stdout, " %s", word);
      } else {
        fprintf(stdout, "\n");
        print_n_spaces(start_column - 1);
        space_left = 1 + end_column - start_column;
        space_left -= fprintf(stdout, "%s", word);
      }
    }
    word = strtok_r(NULL, delims, &save_ptr);
  }
  free(text_dup);
  fprintf(stdout, "\n");
}


void usage(ArgumentState* arg_state, int status, bool printEnvHelp,
           bool printCurrentSettings) {
  ArgumentDescription *desc = arg_state->desc;
  const int desc_start_col = 42, end_col = 79;
  int i, nprinted;

  fprintf(stdout,"Usage: %s [flags] [source files]\n",arg_state->program_name);

  for (i = 0;; i++) {
    if (!desc[i].name)
      break;
    if (desc[i].name[0] == '\0') {
      if (!strcmp(desc[i].description, "Developer Flags")) {
        if (developer) {
          fprintf(stdout, "\n\n%s\n", desc[i].description);
          fprintf(stdout, "===============\n");
          continue;
        } else {
          break;
        }
      } else {
        fprintf(stdout, "\n%s:\n", desc[i].description);
        continue;
      }
    }
    if (desc[i].key != ' ') {
      nprinted = fprintf(stdout, "  -%c, --", desc[i].key);
    } else {
      nprinted = fprintf(stdout, "      --");
    }
    if (desc[i].type && 
        (!strcmp(desc[i].type, "N") || !strcmp(desc[i].type, "n")))
      nprinted += fprintf(stdout, "[no-]");
    nprinted += fprintf(stdout, "%s", desc[i].name);

    if (desc[i].argumentOptions)
      nprinted += fprintf(stdout, " %s", desc[i].argumentOptions);
    if (nprinted > (desc_start_col - 2)) {
      fprintf(stdout, "\n");
      print_n_spaces(desc_start_col - 1);
    } else {
      print_n_spaces(desc_start_col - nprinted - 1);
    }
    word_wrap_print(desc[i].description, desc_start_col, end_col);

    if (printEnvHelp || printCurrentSettings) {
      /* print environment variable stuff */
      if (printEnvHelp) {
        printf("          env var: ");
        const char* envvar = desc[i].env;
        if (envvar) {
          const char* setting = getenv(envvar);
          printf("%s", envvar);
          if (setting) {
            printf(" (set to '%s')", setting);
          } else {
            printf(" (not set)");
          }
        } else {
          printf("<none>");
        }
        printf("\n");
      }
      /* print default setting stuff */
      if (printCurrentSettings) {
        printf("          currently: ");
        char type = desc[i].type[0];
        switch (type) {
        case 'I':
        case '+':
          printf("%d", *(int*)desc[i].location);
          break;
        case 'P':
        case 'S':
          if (desc[i].location) {
            printf("'%s'", (char*)desc[i].location);
          } else {
            printf("''");
          }
          break;
        case 'D':
          printf("%g", *(double*)desc[i].location);
          break;
        case 'f':
        case 'F':
        case 'T':
          printf("%s", *(bool*)desc[i].location ? "selected" : "not selected");
          break;
        case 'L':
          printf("%Ld", *(int64*)desc[i].location);
          break;
        case 'N':
        case 'n':
          printf("--%s%s", 
                 (*(bool*)desc[i].location ^ type == 'N') ? "no-" : "",
                 desc[i].name);
          break;
        default:
          INT_FATAL("Unexpected case in usage()");
          break;
        }
        printf("\n");
      }
      printf("\n");
    }
  }
  exit(status);
}

void
free_args(ArgumentState *arg_state) {
  if (arg_state->file_argument)
    free(arg_state->file_argument);
}
