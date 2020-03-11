/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "chplrt.h"

#include "arg.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-env.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpl-tasks.h"
#include "chpl-linefile-support.h"
#include "config.h"
#include "error.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


static int gdbFlag = 0;
static int lldbFlag = 0;
int32_t blockreport = 0; // report locations of blocked threads on SIGINT
int32_t taskreport = 0;  // report thread hierarchy on SIGINT


typedef struct _flagType {
  const char* flagDash;
  const char* subArg;
  const char* flagDashDash;
  const char* description;
  const char headerType;
} flagType;

static const flagType flagList[] = {
  { "h", "", "help", "print this message", 'g' },
  { "a", "", "about", "print compilation information", 'g' },
  { "nl", " <n>", "",
    "run program using n locales\n"
    "(equivalent to setting the numLocales config const)", 'g' },
  { "q", "", "quiet", "run program in quiet mode", 'g' },
  { "v", "", "verbose", "run program in verbose mode", 'g' },
  { "b", "", "blockreport",
    "report location of blocked threads on SIGINT", 'g' },
  { "t", "", "taskreport",
    "report list of pending and executing tasks on SIGINT", 'g' },
  { "", "", "gdb", "run program in gdb", 'g' },
  { "", "", "lldb", "run program in lldb", 'g' },
  { "E", "<envVar>=<val>", "",
    "set the value of an environment variable", 'g' },

  { "s", "", "<cfgVar>=<val>", "set the value of a config var", 'c' },
  { "f", "<filename>", "",
    "read in a file of config var assignments", 'c' },
};

const int numFlags = sizeof(flagList) / sizeof(flagList[0]);


int _runInGDB(void) {
  return gdbFlag;
}

int _runInLLDB(void) {
  return lldbFlag;
}


static void defineEnvVar(const char* currentArg,
                         int32_t lineno, int32_t filename) {
  char* eqp;

  //
  // We want to put a duplicate of the given value in the environment,
  // to avoid conflicts between unknown/hidden modifications of the
  // original command line string by either command line processing or
  // the system environment handling.  We also don't want to set any
  // variable that is already set, because already-set vars are either
  // specific to the compute node and we shouldn't override them, or
  // they're the result of other software forwarding the environment
  // for us.  putenv(3) doesn't make a duplicate and will override a
  // setting that is already present.  setenv(3) makes a duplicate and
  // can avoid overriding, but it needs the var and value as separate
  // strings.  We can provide these if we change "<var>=<value>" to
  // "<var>\0<value>" in the original environment.  Here we assume
  // that we can do this, but if we have trouble with modifying the
  // environment like this we may have to revisit this decision.  (We
  // do put the '=' back after we're done.)
  //
  if ((eqp = strchr(currentArg, '=')) == NULL) {
    chpl_error("-E argument must be of the form name=value", lineno, filename);
  }

  *eqp = '\0';
  chpl_env_set(currentArg, eqp + 1, 0);
  *eqp = '=';
}


static void parseDashEArgs(int* argc, char* argv[]) {
  const int32_t filename = CHPL_FILE_IDX_COMMAND_LINE_ARG;

  for (int i = 1; i < *argc; i++) {
    int lineno = i;
    const char* currentArg = argv[i];

    if (currentArg[0] == '-' && currentArg[1] == 'E') {
      //
      // Current argument is '-E'.
      //
      if (currentArg[2] == '\0') {
        i++;
        if (i >= *argc) {
          chpl_error("-E flag is missing <name=value> argument",
                     lineno, filename);
        }
        currentArg = argv[i];
        defineEnvVar(currentArg, lineno, filename);
      } else {
        defineEnvVar(currentArg + 2, lineno, filename);
      }
    } else {
      //
      // Current argument is something other than '-E'.
      //
      // Skip over it.  The only tricky bit here is deciding whether to
      // skip one or two args for a short-form flag with a sub-argument.
      // There are no long-form flags that take sub-args.
      //
      if (currentArg[0] == '-' && currentArg[1] != '-') {
        currentArg++;
        for (int fi = 0; fi < numFlags; fi++) {
          int flagLen = strlen(flagList[fi].flagDash);
          if (strncmp(currentArg, flagList[fi].flagDash, flagLen) == 0) {
            if (flagList[fi].subArg[0] != '\0' &&
                currentArg[flagLen] == '\0') {
              i++;
            }
            break;
          }
        }
      }
    }
  }
}


static void printHeaders(char thisType, char* lastType) {
  if (thisType != *lastType) {
    fprintf(stdout, "\n");
    switch (thisType) {
    case 'c':
      fprintf(stdout, "CONFIG VAR FLAGS:\n");
      fprintf(stdout, "=================\n");
      break;

    case 'g':
      fprintf(stdout, "FLAGS:\n");
      fprintf(stdout, "======\n");
      break;
    }
    *lastType = thisType;
  }
}


void printHelpTable(void) {
  int i = 0;
  int longestFlag = 0;
  char flagBuf[100];
  char lastHeaderType = '\0';

  printAdditionalHelp();

  for (i = 0; i < numFlags; i++) {
    int flagLen;
    if (flagList[i].flagDash[0] == '\0') {
      flagLen = 2 + strlen(flagList[i].flagDashDash);
    } else {
      if (flagList[i].flagDashDash[0] == '\0') {
        flagLen = 1 + strlen(flagList[i].flagDash) +
                  strlen(flagList[i].subArg);
      } else {
        flagLen = 1 + strlen(flagList[i].flagDash) +
                  strlen(flagList[i].subArg) +
                  2 + 2 + strlen(flagList[i].flagDashDash);
      }
    }

    if (longestFlag < flagLen) {
      longestFlag = flagLen;
    }
  }

  for (i = 0; i < numFlags; i++) {
    printHeaders(flagList[i].headerType, &lastHeaderType);
    if (flagList[i].flagDash[0] == '\0') {
      snprintf(flagBuf, sizeof(flagBuf), "--%s", flagList[i].flagDashDash);
    } else {
      if (flagList[i].flagDashDash[0] == '\0') {
        snprintf(flagBuf, sizeof(flagBuf), "-%s%s",
                 flagList[i].flagDash, flagList[i].subArg);
      } else {
        snprintf(flagBuf, sizeof(flagBuf), "-%s%s, --%s",
                 flagList[i].flagDash, flagList[i].subArg,
                 flagList[i].flagDashDash);
      }
    }

    fprintf(stdout, "  %-*s  : ", longestFlag, flagBuf);
    {
      const char* p;
      const char* pNext;
      for (p = flagList[i].description;
           (pNext = strchr(p, '\n')) != NULL;
           p = pNext + 1) {
        fprintf(stdout, "%.*s\n%*s", (int) (pNext - p), p,
                longestFlag + 6, "");
      }
      fprintf(stdout, "%s\n", p);
    }
  }
  fprintf(stdout, "\n");
}


static int32_t _argNumLocales = 0;

void parseNumLocales(const char* numPtr, int32_t lineno, int32_t filename) {
  int invalid;
  char invalidChars[2] = "\0\0";
  _argNumLocales = c_string_to_int32_t_precise(numPtr, &invalid, invalidChars);
  if (invalid) {
    char* message = chpl_glom_strings(3, "\"", numPtr,
                                      "\" is not a valid number of locales");
    chpl_error(message, lineno, filename);
  }
  if (_argNumLocales < 1) {
    chpl_error("Number of locales must be greater than 0", lineno, filename);
  }
}

int32_t getArgNumLocales(void) {
  int32_t retval = 0;
  if (_argNumLocales) {
    retval = _argNumLocales;
  }
  return retval;
}


extern void chpl_program_about(void); // The generated code provides this
void parseArgs(chpl_bool isLauncher, chpl_parseArgsMode_t mode,
               int* argc, char* argv[]) {
  int i;
  int printHelp = 0;
  int printAbout = 0;
  int origargc = *argc;
  int stop_parsing = 0;
  int saw_socket_conn = 0;

  //
  // Handle the pre-parse for '-E' arguments separately.
  //
  if (mode == parse_dash_E) {
    assert(!isLauncher);
    parseDashEArgs(argc, argv);
    return;
  }

  for (i = 1; i < *argc; i++) {
    const int32_t filename = CHPL_FILE_IDX_COMMAND_LINE_ARG;
    int lineno = i + (origargc - *argc);
    int argLength = 0;
    const char* currentArg = argv[i];
    argLength = strlen(currentArg);

    if (mainHasArgs && (stop_parsing || argLength < 2)) {
      /* update the argv structure passed to a Chapel program, but don't parse
       * the arguments
       */
      chpl_gen_main_arg.argv[chpl_gen_main_arg.argc] = argv[i];
      chpl_gen_main_arg.argc++;
      continue;
    }

    /* if the Chapel main takes arguments, then "--" is a magic argument that
     * will prevent parsing of any additional arguments
     */
    if (mainHasArgs && strcmp(currentArg, "--") == 0) {
      stop_parsing = 1;
      continue;
    }

    if (argLength < 2) {
      const char* message = chpl_glom_strings(3, "\"", currentArg,
                                              "\" is not a valid argument");
      chpl_error(message, lineno, filename);
    }

    switch (currentArg[0]) {
    case '-':
      switch (currentArg[1]) {
      case '-':
        {
          const char* flag = currentArg + 2;

          if (strcmp(flag, "gdb") == 0) {
            gdbFlag = i;
            break;
          }

          if (strcmp(flag, "lldb") == 0) {
            lldbFlag = i;
            break;
          }

          if (strcmp(flag, "help") == 0) {
            printHelp = 1;
            chpl_gen_main_arg.argv[chpl_gen_main_arg.argc] = "--help";
            chpl_gen_main_arg.argc++;
            break;
          }
          if (strcmp(flag, "about") == 0) {
            printAbout = 1;
            break;
          }
          if (strcmp(flag, "verbose") == 0) {
            verbosity=2;
            break;
          }
          if (strcmp(flag, "blockreport") == 0) {
            blockreport = 1;
            break;
          }
          if (strcmp(flag, "taskreport") == 0) {
            taskreport = 1;
            break;
          }
          if (strcmp(flag, "quiet") == 0) {
            verbosity = 0;
            break;
          }
          if (strcmp(flag, "chpl-mli-socket-loc") == 0) {
            if (saw_socket_conn) {
              chpl_error("multiple uses of --chpl-mli-socket-loc, this flag is "
                         "intended for internal use only", lineno, filename);
            }
            i++;
            if (i >= *argc) {
              chpl_error("--chpl-mli-socket-loc developer flag is missing"
                         "<connection> argument",
                         lineno, filename);
            }
            currentArg = argv[i];
            if (strncmp(currentArg, "tcp://", 6) != 0) {
              chpl_error("unexpected start of <connection> for "
                         "--chpl-mli-socket-loc developer flag, this flag is "
                         "intended for internal use only", lineno, filename);
            }
            saw_socket_conn = 1;
            // We reached information about the socket in a multilocale library
            // run.  Save it.
            chpl_gen_main_arg.argv[chpl_gen_main_arg.argc++] =
              "--chpl-mli-socket-loc";
            chpl_gen_main_arg.argv[chpl_gen_main_arg.argc++] = currentArg;
            break;
          }
          if (argLength < 3) {
            char* message = chpl_glom_strings(3, "\"", currentArg,
                                              "\" is not a valid argument");
            chpl_error(message, lineno, filename);
          }
          i += handlePossibleConfigVar(argc, argv, i, lineno, filename);
          break;
        }

      case 'a':
        if (currentArg[2] == '\0') {
          printAbout = 1;
        } else {
          i += handleNonstandardArg(argc, argv, i, lineno, filename);
        }
        break;

      case 'b':
        if (currentArg[2] == '\0') {
          blockreport = 1;
        } else {
          i += handleNonstandardArg(argc, argv, i, lineno, filename);
        }
        break;

      case 'E':
        if (isLauncher) {
          i += handleNonstandardArg(argc, argv, i, lineno, filename);
        } else {
          //
          // We parse -E only in parse_dash_E mode, which is handled above.
          //
          if (currentArg[2] == '\0') {
            i++;
          }
        }
        break;

      case 'f':
        if (currentArg[2] == '\0') {
          i++;
          if (i >= *argc) {
            chpl_error("-f flag is missing <filename> argument",
                       lineno, filename);
          }
          currentArg = argv[i];
          parseConfigFile(currentArg, lineno, filename);
        } else {
          parseConfigFile(currentArg + 2, lineno, filename);
        }
        break;

      case 'h':
        if (currentArg[2] == '\0') {
          printHelp = 1;
          chpl_gen_main_arg.argv[chpl_gen_main_arg.argc] = "-h";
          chpl_gen_main_arg.argc++;
        } else {
          i += handleNonstandardArg(argc, argv, i, lineno, filename);
        }
        break;

      case 'n':
        if (currentArg[2] == 'l') {
          const char* numPtr;
          if (currentArg[3] == '\0') {
            i++;
            if (i >= *argc) {
              chpl_error("-nl flag is missing <numLocales> argument",
                         lineno, filename);
            }
            currentArg = argv[i];
            numPtr = currentArg;
          } else {
            numPtr = &(currentArg[3]);
          }
          initSetValue("numLocales", numPtr, "Built-in", lineno, filename);
          break;
        }
        i += handleNonstandardArg(argc, argv, i, lineno, filename);
        break;

      case 'q':
        if (currentArg[2] == '\0') {
          verbosity = 0;
        } else {
          i += handleNonstandardArg(argc, argv, i, lineno, filename);
        }
        break;

      case 's':
        {
          if (argLength < 3) {
            char* message = chpl_glom_strings(3, "\"", currentArg,
                                              "\" is not a valid argument");
            chpl_error(message, lineno, filename);
          }
          i += handlePossibleConfigVar(argc, argv, i, lineno, filename);
          break;
        }

      case 't':
        if (currentArg[2] == '\0') {
          taskreport = 1;
        } else {
          i += handleNonstandardArg(argc, argv, i, lineno, filename);
        }
        break;

      case 'v':
        if (currentArg[2] == '\0') {
          verbosity = 2;
        } else {
          i += handleNonstandardArg(argc, argv, i, lineno, filename);
        }
        break;

      default:
        i += handleNonstandardArg(argc, argv, i, lineno, filename);
        break;
      }
      break;

    default:
      i += handleNonstandardArg(argc, argv, i, lineno, filename);
      break;
    }
  }

  if (printAbout) {
    chpl_program_about();
    chpl_exit_any(0);
  }

  if (printHelp) {
    if (!mainHasArgs) {
      printHelpTable();
      printConfigVarTable();
      chpl_exit_any(0);
    }
  }
}


int chpl_specify_locales_error(void) {
  chpl_error("Specify number of locales via -nl <#> or --numLocales=<#>", 0, 0);
  return 0;
}
