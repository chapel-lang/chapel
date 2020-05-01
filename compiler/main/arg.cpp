/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#include "arg.h"

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "driver.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"

#include <cstdio>

#include <inttypes.h>

static const char* get_envvar_setting(const ArgumentDescription& desc);

/************************************* | **************************************
*                                                                             *
* Generate a usage page                                                       *
*                                                                             *
************************************** | *************************************/

static void  print_n_spaces(int n);
static void  word_wrap_print(const char* text, int startCol, int endCol);

void usage(const ArgumentState* state,
           int                  status,
           bool                 printEnvHelp,
           bool                 printCurrentSettings)
{
  const ArgumentDescription* desc           = state->desc;
  const int                  desc_start_col = 39;
  const int                  end_col        = 79;

  fprintf(stdout, "Usage: %s [flags] [source files]\n", state->program_name);

  // The last row is the only row where the name is NULL
  for (int i = 0; desc[i].name != 0; i++)
  {
    // If this is a header row (the name is the empty string)
    if (desc[i].name[0] == '\0')
    {
      if (strncmp(desc[i].description, "Developer Flags", 15) == 0)
      {
        if (developer == false)
        {
          // We assume that developer flags are listed at the end of the
          // argument list in driver.cpp.  If we encounter a section header
          // whose prefix matches "Developer Flags" and (developer == false),
          // then we truncate the rest of the options list.
          break;
        }
      }

      fprintf(stdout, "\n%s:\n", desc[i].description);
    }
    else
    {
      int nprinted = 0;

      if (desc[i].key != ' ')
        nprinted = fprintf(stdout, "  -%c, --", desc[i].key);
      else
        nprinted = fprintf(stdout, "      --");

      if (desc[i].type &&
          (strcmp(desc[i].type, "N") == 0 || strcmp(desc[i].type, "n") == 0))
        nprinted += fprintf(stdout, "[no-]");

      nprinted += fprintf(stdout, "%s", desc[i].name);

      if (desc[i].argumentOptions)
        nprinted += fprintf(stdout, " %s", desc[i].argumentOptions);

      if (nprinted > (desc_start_col - 2))
      {
        fprintf(stdout, "\n");
        print_n_spaces(desc_start_col - 1);
      }
      else
      {
        print_n_spaces(desc_start_col - nprinted - 1);
      }

      word_wrap_print(desc[i].description, desc_start_col, end_col);

      if (printEnvHelp || printCurrentSettings)
      {
        /* print environment variable stuff */
        if (printEnvHelp)
        {
          const char* envvar = desc[i].env;

          printf("          env var:   ");

          if (envvar)
          {
            const char* setting = get_envvar_setting(desc[i]);

            printf("%s", envvar);

            if (setting)
              printf(" (set to '%s')", setting);
            else
              printf(" (not set)");
          }
          else
          {
            printf("<none>");
          }

          printf("\n");
        }

        /* print default setting stuff */
        if (printCurrentSettings)
        {
          char type = desc[i].type[0];

          printf("          currently: ");

          switch (type)
          {
          case 'I':
          case '+':
            if (desc[i].location != 0)
              printf("%d", *(int*) desc[i].location);
            else
              printf("''");

            break;

          case 'P':
          case 'S':
            if (desc[i].location != 0)
              printf("'%s'", (char*) desc[i].location);
            else
              printf("''");

            break;

          case 'D':
            if (desc[i].location != 0)
              printf("%g", *(double*) desc[i].location);
            else
              printf("''");

            break;

          case 'f':
          case 'F':
          case 'T':
            if (desc[i].location != 0)
            {
              bool sel = *((bool*) desc[i].location);

              printf("%s", (sel == true) ? "selected" : "not selected");
            }
            else
              printf("''");

            break;

          case 'L':
            if (desc[i].location != 0)
              printf("%" PRId64, *(int64_t*) desc[i].location);
            else
              printf("''");

            break;

          case 'N':
          case 'n':
            if (desc[i].location != 0)
              printf("--%s%s",
                     (*(bool*) desc[i].location ^ (type == 'N')) ? "no-" : "",
                     desc[i].name);
            else
              printf("''");

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
  }

  clean_exit(status);
}

static void print_n_spaces(int n)
{
  for (int i = 0; i < n; i++)
    fprintf(stdout, " ");
}

static void word_wrap_print(const char* text, int startCol, int endCol)
{
  /*
   * Print the buffer "text" to stdout with all non-whitespace text at or
   * after startCol, and print at most endCol characters per line.
   * Do not break lines in the middle of words. When this function is called,
   * stdout must be in a state such that the next character fprintf will print
   * is in startCol.
   */
  char*       textDup   = strdup(text);
  const char* delims    = " ";
  char*       savePtr   = 0;

  char*       word      = strtok_r(textDup, delims, &savePtr);

  int         spaceLeft = 1 + endCol - startCol;
  bool        first     = true;

  while (word)
  {
    int wordlength = strlen(word);

    if (first)
    {
      spaceLeft = spaceLeft - fprintf(stdout, "%s", word);
      first      = false;
    }
    else
    {
      if (wordlength + 1 < spaceLeft)
      {
        spaceLeft -= fprintf(stdout, " %s", word);
      }
      else
      {
        fprintf(stdout, "\n");

        print_n_spaces(startCol - 1);

        spaceLeft = 1 + endCol - startCol - fprintf(stdout, "%s", word);
      }
    }

    word = strtok_r(NULL, delims, &savePtr);
  }

  free(textDup);

  fprintf(stdout, "\n");
}

/************************************* | **************************************
*                                                                             *
* Initialize program_name and program_loc                                     *
*                                                                             *
************************************** | *************************************/

void init_args(ArgumentState* state, const char* argv0) {
  char* name = strdup(argv0);

  if (char* firstSlash = strrchr(name, '/')) {
    name  = firstSlash + 1;
  }

  state->program_name = name;
  state->program_loc  = findProgramPath(argv0);
}


/*
 * Initialize arg_desc member.
 */

void init_arg_desc(ArgumentState* state, ArgumentDescription* arg_desc) {
  state->desc = arg_desc;
}

/************************************* | **************************************
*                                                                             *
* Process the argument descriptor                                             *
*                                                                             *
************************************** | *************************************/

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

static void ProcessEnvironment(const ArgumentState* state);
static void ProcessCommandLine(ArgumentState* state, int argc, char* argv[]);

void process_args(ArgumentState* state, int argc, char* argv[])
{
#ifdef _BASEAST_H_
  astlocMarker markAstLoc(0, "<command line>");
#endif

  ProcessEnvironment(state);
  ProcessCommandLine(state, argc, argv);
}

/************************************* | **************************************
*                                                                             *
* Walk the descriptor and apply values from the environment                   *
*                                                                             *
************************************** | *************************************/

static void ApplyValue(const ArgumentState*       state,
                       const ArgumentDescription* desc,
                       const char*                value);

static void ProcessEnvironment(const ArgumentState* state)
{
  ArgumentDescription* desc = state->desc;

  // The name field is defined by every row except the final guard
  for (int i = 0; desc[i].name != 0; i++)
  {
    if (desc[i].env)
    {
      const char* env = get_envvar_setting(desc[i]);

      if (env != 0)
      {
        char sel = desc[i].type[0];

        if (sel == 'N' || sel == 'n')
        {
          switch (env[0])
          {
          case 'Y':
          case 'y':
          case 'T':
          case 't':
          case '1':
            env = (sel == 'N') ? "true" : "false";
          break;

          case 'N':
          case 'n':
          case 'F':
          case 'f':
          case '0':
            env = (sel == 'N') ? "false" : "true";
          break;

          default:
            USR_FATAL_CONT("When the environment variable %s"
                           " is set and not empty, it must start with one of Y y T t 1"
                           " (indicates 'yes') or N n F f 0 (indicates '--no')."
                           " Currently it is set to \"%s\".",
                           desc[i].env,
                           env);
            break;
          }
        }

        ApplyValue(state, &desc[i], env);
      }
    }
  }
}

static void ApplyValue(const ArgumentState*       state,
                       const ArgumentDescription* desc,
                       const char*                value)
{
  void* location = desc->location;

  if (location != 0)
  {
    char type = desc->type[0];

    switch (type)
    {
      case '+':
        *((int*)     location) = *((int*) location) + 1;
        break;

      case 'T':
        *((int*)     location) = !(*((int*) location));
        break;

      case 'F':
      case 'f':
        *((bool*)    location) = (type == 'F') ? 1 : 0;
        break;

      case 'n':
      case 'N':
        *((bool*)    location) = (strcmp(value, "true") == 0) ? true : false;
        break;

      case 'I':
        *((int*)     location) = strtol(value, NULL, 0);
        break;

      case 'L':
        *((int64_t*) location) = str2int64(value);
        break;

      case 'D':
        *((double*)  location) = strtod(value, NULL);
        break;

      case 'P':
        strncpy((char*) location, value, FILENAME_MAX);
        break;

      case 'S':
      {
        long bufSize = strtol(desc->type + 1, NULL, 10);

        strncpy((char*) location, value, bufSize);

        break;
      }
    }
  }

  if (desc->pfn)
    desc->pfn(desc, value);
}

/************************************* | **************************************
*                                                                             *
* Walk the command line.                                                      *
*    options start with "-" or "--" and must be matched in the descriptor     *
*    non-options are assumed to be source filenames                           *
*                                                                             *
************************************** | *************************************/

static void process_arg(const ArgumentState*       state,
                        const ArgumentDescription* desc,
                        char***                    argv,
                        const char*                currentFlag);

static void print_suggestions(const char* flag, const ArgumentDescription* desc);
static void bad_flag(const char* flag, const char* arg, const ArgumentDescription* desc);
static void extraneous_arg(const char* flag, const char* extras, const ArgumentDescription* desc);

static void missing_arg(const char* currentFlag);

static void ProcessCommandLine(ArgumentState* state, int argc, char* aargv[])
{
  ArgumentDescription* desc     = state->desc;
  char**               argvSave = (char**) malloc((argc + 1) * sizeof(char*));
  char**               argvBase = (char**) malloc((argc + 1) * sizeof(char*));
  char**               argv     = argvBase;

  for (int i = 0; i < argc; i++) {
    argvSave[i] = strdup(aargv[i]);
  }

  argvSave[argc] = NULL;

  for (int i = 0; i < argc + 1; i++)
    argvBase[i] = argvSave[i];

  while (*++argv)
  {
    if ((*argv)[0] == '-')
    {
      if ((*argv)[1] == '-')
      {
        char* end   = strchr((*argv) + 2, '=');
        int   len   = (end != 0) ? end - ((*argv) + 2) : strlen((*argv) + 2);
        bool  found = false;

        for (int i = 0; desc[i].name != 0 && found == false; i++)
        {
          // Skip sections headers
          if (desc[i].type != 0)
          {
            int flagLen = strlen(desc[i].name);

            if (len == flagLen && strncmp(desc[i].name, (*argv) + 2, len) == 0)
            {
              const char* currentFlag = *argv;

              *argv = (end == 0) ? *argv + strlen(*argv) : end;

              process_arg(state, &(state->desc[i]), &argv, currentFlag);

              found = true;
              break;
            }
            else if ((desc[i].type[0] == 'N' || desc[i].type[0] == 'n')         &&
                     len                                         == flagLen + 3 &&
                     strncmp("no-",        (*argv) + 2,       3) == 0           &&
                     strncmp(desc[i].name, (*argv) + 5, len - 3) == 0)
            {
              const char* currentFlag = *argv;

              *argv        = (end == 0) ? *argv + strlen(*argv) - 1 : end;

              desc[i].type = (desc[i].type[0] == 'N') ? "f" : "F";

              process_arg(state, &(state->desc[i]), &argv, currentFlag);

              desc[i].type = (desc[i].type[0] == 'f') ? "N" : "n";

              found = true;
              break;
            }
          }
        }

        if (found == false)
        {
          // This does not return
          bad_flag(*argv, *argv, desc);
        }
      }
      else
      {
        char singleDashArg = *++(*argv);
        char errFlag[3]    = { '-', singleDashArg, '\0' };
        bool found         = false;

        for (int i = 0; desc[i].name != 0 && found == false; i++)
        {
          // Skip sections headers
          if (desc[i].type != 0)
          {
            if (desc[i].key == singleDashArg)
            {
              ++(*argv);

              process_arg(state, &(state->desc[i]), &argv, (*argv)-2);

              if (**argv != '\0')
                extraneous_arg(errFlag, *argv, desc);

              found = true;
            }
          }
        }

        if (found == false)
        {
          // This does not return
          bad_flag(errFlag, *argv, desc);
        }

      }
    }
    else
    {
      state->file_argument = (const char **)realloc(
        state->file_argument,
        sizeof(char*) * (state->nfile_arguments + 2));

      state->file_argument[state->nfile_arguments++] = strdup(*argv);
      state->file_argument[state->nfile_arguments]   = NULL;
    }
  }

  // Free the allocated strings
  for (int i = 0; i < argc; i++) {
    free(argvSave[i]);
  }

  // Free the vectors
  free(argvSave);
  free(argvBase);
}

static void process_arg(const ArgumentState*       state,
                        const ArgumentDescription* desc,
                        char***                    argv,
                        const char*                currentFlag)
{
  const char* arg = NULL;

  if (desc->type)
  {
    char type = desc->type[0];

    if (type == 'F' || type == 'f')
      *((bool*) desc->location) = (type == 'F') ? true : false;

    else if (type == 'N' || type == 'n')
      *((bool*) desc->location) = (type == 'N') ? true : false;

    else if (type=='T')
      *((int*)  desc->location) = !(*((int*) desc->location));

    else if (type == '+')
      *((int*)  desc->location) = *((int*)  desc->location) + 1;

    else
    {
      // If there's an equal sign or characters remain, take the current arg.
      // Otherwise, take the next one (which may be null).
      arg = ***argv ? **argv : *++(*argv);

      if (!arg)
        missing_arg(currentFlag);

      if (*arg == '=')
        ++arg;

      switch (type)
      {
        case 'I':
          *((int*)     desc->location) = atoi(arg);
          break;

        case 'D':
          *((double*)  desc->location) = atof(arg);
          break;

        case 'L':
          *((int64_t*) desc->location) = atoll(arg);
          break;

        case 'P':
          if (desc->location != NULL) {
            strncpy((char*) desc->location, arg, FILENAME_MAX);
          }
          break;

        case 'S':
          if( desc->location ) {
            int len = strlen(arg);
            int maxlen = atoi(desc->type + 1);
            if( len > maxlen ) {
              USR_FATAL("argument for --%s is too long", desc->name);
            }
            strncpy((char*) desc->location, arg, maxlen);
          }
          break;

        default:
          fprintf(stdout,
                  "%s: bad argument description\n",
                 state->program_name);

          clean_exit(1);
          break;
      }

      **argv += strlen(**argv); // Consume the argument value.
    }
  }

  if (desc->pfn)
    desc->pfn(desc, arg);
}

static void print_suggestions(const char* flag, const ArgumentDescription* desc)
{
  const char* nodashes = flag;
  // skip past any '-' characters at the start of flag
  while (*nodashes == '-') nodashes++;
  char* usearg = strdup(nodashes);
  // Chop off the string after '='
  for (int i = 0; usearg[i]; i++) {
    if (usearg[i] == '=')
      usearg[i] = '\0';
  }

  // Find the first developer only flag
  // (Code in this function assumes developer flags are after other flags)
  int firstDeveloperOnly = 0;
  for (int i = 0; desc[i].name != 0; i++) {
    const char* devFlags = "Developer Flags";
    if (desc[i].description &&
        // Does the description start with devFlags?
        startsWith(desc[i].description, devFlags)) {
      firstDeveloperOnly = i;
      break;
    }
  }

  bool helped = false;
  // Find some common confusions and print a suggestion
  for (int i = 0; desc[i].name != 0; i++) {
    // Skip developer-only options for non-developer compile
    if (!developer && i >= firstDeveloperOnly)
      break;

    // Ignore empty entries
    if (desc[i].name[0] == '\0')
      continue;

    if (usearg[0] == desc[i].key && usearg[1] == '\0') {
      // e.g. --s was used instead of -s
      fprintf(stderr, "       Did you mean -%c ?\n", desc[i].key);
      helped = true;
    } if (0 == strcmp(usearg, desc[i].name)) {
      fprintf(stderr, "       Did you mean --%s ?\n", desc[i].name);
      helped = true;
    } else if (desc[i].env &&
               (0 == strcmp(usearg, desc[i].env) ||
                (desc[i].env[0] == '_' &&
                 0 == strcmp(usearg, desc[i].env+1)))) {
      fprintf(stderr, "       Did you mean --%s ?\n", desc[i].name);
      helped = true;
    }
  }

  // Did the user elaborate on a flag that was abbreviated?
  // e.g. --helpme
  if (!helped) {
    for (int i = 0; desc[i].name != 0; i++) {
      // Skip developer-only options for non-developer compile
      if (!developer && i >= firstDeveloperOnly)
        break;

      // Ignore empty entries
      if (desc[i].name[0] == '\0')
        continue;

      if (startsWith(usearg, desc[i].name)) {
        fprintf(stderr, "       Did you mean --%s ?\n", desc[i].name);
        helped = true;
      }
    }
  }

  // Did the user type a portion of a flag?
  if (!helped) {
    for (int i = 0; desc[i].name != 0; i++) {
      // Skip developer-only options for non-developer compile
      if (!developer && i >= firstDeveloperOnly)
        break;

      // Ignore empty entries
      if (desc[i].name[0] == '\0')
        continue;

      if (startsWith(desc[i].name, usearg)) {
        fprintf(stderr, "       Did you mean --%s ?\n", desc[i].name);
      }
    }
  }

  free(usearg);
}

static void bad_flag(const char* flag, const char* arg, const ArgumentDescription* desc)
{
  fprintf(stderr, "Unrecognized flag: '%s' (use '-h' for help)\n", flag);
  print_suggestions(arg, desc);
  clean_exit(1);
}

static void extraneous_arg(const char* flag, const char* extras, const ArgumentDescription* desc)
{
  fprintf(stderr,
          "Extra characters after flag '%s': '%s' (use 'h' for help)\n",
          flag,
          extras);
  print_suggestions(flag, desc);
  clean_exit(1);
}

static void missing_arg(const char* currentFlag)
{
  fprintf(stderr,
          "Missing argument for flag: '%s' (use '-h' for help)\n",
          currentFlag);
  clean_exit(1);
}

/************************************* | **************************************
*                                                                             *
* The value in the environment could be one of                                *
*     NULL    (no value)                                                      *
*     ""      (the value is the empty string)                                 *
*     string  (the value is a general string)                                 *
*                                                                             *
* If the value is NULL     then the result is NULL                            *
*                                                                             *
* If the value is ""       then                                               *
*    If the type is 'P' or 'S' (Path or String) then return ""                *
*    otherwise return NULL                                                    *
*                                                                             *
* If the value is a string then the result is that string                     *
*                                                                             *
************************************** | *************************************/

static const char* get_envvar_setting(const ArgumentDescription& desc)
{
  const char* retval = 0;

  if (desc.env != 0)
  {
    // The result of getenv() must not be modified or free'd
    const char* env = getenv(desc.env);

    // The environment variable is not set
    if (env == 0)
      retval = 0;

    // The environment variable IS the empty string
    else if (env[0] == '\0')
      retval = (desc.type[0] == 'P' || desc.type[0] == 'S') ? "" : 0;

    // The environment variable IS NOT the empty string
    else
      retval = env;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Cleanup                                                                     *
*                                                                             *
************************************** | *************************************/

void free_args(ArgumentState* state) {
  if (state->file_argument)
    free(state->file_argument);
}
