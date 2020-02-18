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

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include "chplcgfns.h"
#include "chpl-comm-launch.h"
#include "chpl-comm-locales.h"
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"

// used in get_enviro_keys
extern char** environ;

// This global variable stores the arguments to main
// that should be handled by the program.
chpl_main_argument chpl_gen_main_arg;
// This variable is normally declared in config.h
// and comes from the generated code.
extern const int mainHasArgs;

extern const int launcher_is_mli;
extern const char* launcher_mli_real_name;

static void chpl_launch_sanity_checks(const char* argv0) {
  // Do sanity checks just before launching.
  struct stat statBuf;

  // Make sure the _real binary exists
  // (this should be called after someone has called
  // chpl_compute_real_binary_name() )
  if (stat(chpl_get_real_binary_name(), &statBuf) != 0) {
    char errorMsg[256];
    int wanted_to_write = snprintf(errorMsg, sizeof(errorMsg),
                                   "unable to locate file: %s",
                                   chpl_get_real_binary_name());
    if (wanted_to_write < 0) {
      const char fallbackMsg[] =
        "character encoding error in name of executable to be launched";
      strcpy(errorMsg, fallbackMsg);
    } else if ((size_t)wanted_to_write >= sizeof(errorMsg)) {
      strcpy(&errorMsg[sizeof(errorMsg) - 4], "...");
    }
    chpl_error(errorMsg, 0, 0);
  }
}

//
// Use this utility routine to build up an argv[] list.  Start with
// *largc==0, *largv==NULL, *largv_len==0, and it will keep those
// updated as you append args one after another.
//
void chpl_append_to_largv(int* largc, const char*** largv, int* largv_len,
                          const char* arg) {
  if (*largc >= *largv_len) {
    *largv_len += 10;
    *largv = chpl_mem_realloc(*largv, *largv_len * sizeof(**largv),
                              CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  }
  (*largv)[(*largc)++] = (arg);
}

//
// Use this function to run short utility programs that will return less
//  than 1024 characters of output.  The program must not expect any input.
//  On success, returns the number of bytes read and the output of the
//  command in outbuf.  Returns -1 on failure.
//
int
chpl_run_utility1K(const char *command, char *const argv[], char *outbuf, int outbuflen) {
  const int buflen = 1024;
  int curlen;
  char buf[buflen];
  char *cur;
  int fdo[2], outfd;
  int fde[2], errfd;
  fd_set set;
  pid_t pid;
  int status;
  int rv, numRead;

  if (pipe(fdo) < 0) {
    sprintf(buf, "Unable to run '%s' (pipe failed): %s\n", command, strerror(errno));
    chpl_internal_error(buf);
  }
  if (pipe(fde) < 0) {
    sprintf(buf, "Unable to run '%s' (pipe failed): %s\n", command, strerror(errno));
    chpl_internal_error(buf);
  }

  pid = fork();
  switch (pid) {
  case 0: // child should exit on errors
    close(fdo[0]);
    if (fdo[1] != STDOUT_FILENO) {
      if (dup2(fdo[1], STDOUT_FILENO) != STDOUT_FILENO) {
        sprintf(buf, "Unable to run '%s' (dup2 failed): %s",
                command, strerror(errno));
        chpl_internal_error(buf);
      }
    }
    close(fde[0]);
    if (fde[1] != STDERR_FILENO) {
      if (dup2(fde[1], STDERR_FILENO) != STDERR_FILENO) {
        sprintf(buf, "Unable to run '%s' (dup2 failed): %s",
                command, strerror(errno));
        chpl_internal_error(buf);
      }
    }
    execvp(command, argv);
    // should only return on error
    sprintf(buf, "Unable to run '%s': %s",
                command, strerror(errno));
    chpl_internal_error(buf);
  case -1:
    sprintf(buf, "Unable to run '%s' (fork failed): %s",
            command, strerror(errno));
    chpl_warning(buf, 0, 0);
    return -1;
  default:
    outfd = fdo[0];
    errfd = fde[0];
    close(fdo[1]);
    close(fde[1]);
    numRead = 0;
    curlen = buflen > outbuflen ? outbuflen : buflen;
    cur = buf;
    while (numRead < buflen) {
      struct timeval tv = { 1, 0 };
      FD_ZERO(&set);
      FD_SET(outfd, &set);
      FD_SET(errfd, &set);
      select(errfd+1, &set, NULL, NULL, &tv);
      if (FD_ISSET(outfd, &set)) {
        rv = read(outfd, cur, buflen);
        if (rv == 0) {
          if (waitpid(pid, &status, WNOHANG) == pid)
            break;
        } else if (rv > 0) {
          cur += rv;
          numRead += rv;
          curlen -= rv;
        } else {
          sprintf(buf, "Unable to run '%s' (read failed): %s",
                  command, strerror(errno));
          chpl_warning(buf, 0, 0);
          return -1;
        }
      }
      if (FD_ISSET(errfd, &set)) {
        rv = read(errfd, cur, buflen);
        if (rv == 0) {
          if (waitpid(pid, &status, WNOHANG) == pid)
            break;
        } else if (rv > 0) {
          cur += rv;
          numRead += rv;
          curlen -= rv;
        } else {
          sprintf(buf, "Unable to run '%s' (read failed): %s",
                  command, strerror(errno));
          chpl_warning(buf, 0, 0);
          return -1;
        }
      }
    }

    if (numRead != 0) {
      if (strstr(buf, "internal error: ") == NULL) {
        memcpy(outbuf, buf, numRead);
      } else {
        // The utility program ran, but failed with an internal error
        // from child's branch above (dup2 or exevp)
        buf[numRead] = 0;
        chpl_warning(buf, 0, 0);
        return -1;
      }
    } else {
      sprintf(buf, "Unable to run '%s' (no bytes read)", command);
      chpl_warning(buf, 0, 0);
      return -1;
    }

    // NOTE: We don't do a waitpid() here, so the program may keep running.
    //  That is a bad program, and I'm not going to deal with it here.
  }
  return numRead;
}

//
// This is an even simpler run-a-command utility.  The command is just
// a string to be run, by something like `/bin/sh -c "commandStr"`.  It
// must not expect any input.  Its output to stdout is placed in outbuf,
// truncated to fit if necessary.  Its output to stderr is discarded.
// On success, the return value is the number of bytes in outbuf.  On
// failure, it is -1; output may have been placed in outbuf in this
// case, but if so it should be ignored.
//
int
chpl_run_cmdstr(const char *commandStr, char *outbuf, int outbuflen) {
  const char* commandStr_more = "2>/dev/null";
  char my_cmd[strlen(commandStr) + 1 + strlen(commandStr_more) + 1];
  FILE* f;
  int retVal;

  (void) snprintf(my_cmd, sizeof(my_cmd),
                  "%s %s", commandStr, commandStr_more);

  retVal = -1;
  if ((f = popen(my_cmd, "r")) != NULL) {
    if (fgets(outbuf, outbuflen, f) != NULL) {
      // success; strip any final trailing newline
      retVal = strlen(outbuf);
      if (retVal > 0 && outbuf[retVal - 1] == '\n')
        outbuf[--retVal] = '\0';
    }

    (void) pclose(f);
  }

  return retVal;
}


//
// Record environment variables set by the launcher itself.  This is
// called back from the runtime chpl_env_set() function.
//
static char* evList = NULL;
static int evListSize = 0;

void chpl_launcher_record_env_var(const char* evName, const char *evVal) {
  size_t evNameLen = strlen(evName);
  size_t evValLen = strlen(evVal);
  size_t evListSizeWas = evListSize;
  evListSize = (evListSizeWas // existing list
                              // spacing ' ' overwrites previous '\0'
                + evNameLen   // evName
                + 1           // '='
                + evValLen    // evVal
                + 1           // '\0'
               );
  evList = chpl_mem_realloc(evList, evListSize,
                            CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  if (evListSizeWas == 0) {
    sprintf(evList, "%s=%s", evName, evVal);
  } else {
    sprintf(evList + evListSizeWas - 1, " %s=%s", evName, evVal);
  }
}


//
// If the user wants us to run each program instance in its own
// debugger window, this returns the argc/argv fragment that
// supports that.
//
// Note: the user may have to go to some effort in support of making
//       this work, particularly to allow a system-launched xterm
//       running in the target program environment to open its window
//       in the launch environment.
//
static
void get_debugger_wrapper(int* argc, char*** argv) {
  *argc = 0;
  *argv = NULL;

  const int use_gdb = (getenv("CHPL_COMM_USE_GDB") != NULL);
  const int use_lldb = (getenv("CHPL_COMM_USE_LLDB") != NULL);
  if (!use_gdb && !use_lldb) {
    return;
  }

  //
  // Determine the terminal emulator to use.
  //
  const char* dbg_term = getenv("CHPL_COMM_DBG_TERM");
  if (dbg_term == NULL) {
    dbg_term = "xterm";
  } else if (strcmp(dbg_term, "xterm") != 0 &&
             strcmp(dbg_term, "urxvt") != 0) {
    //
    // We're currently limiting to xterm and urxvt because of lack of
    // testing/interest. Most other terminal emulators seem to support the
    // -e flag, but there are some (like tilda) that uses different flags
    // (-c for tilda). Ideally, there should be a lookup table for those
    // flags if there is interest in adding support for more terminal
    // emulators.
    //
    chpl_warning("CHPL_COMM_DBG_TERM can only be set to xterm or urxvt; "
                 "using xterm",
                 0, 0);
    dbg_term = "xterm";
  }

  // hopefully big enough; PATH_MAX is problematic, but what's better?  
  const size_t term_path_size = PATH_MAX;
  char *term_path = chpl_mem_alloc(term_path_size,
                                   CHPL_RT_MD_COMMAND_BUFFER, -1, 0);

  static char cmd[16] = "";
  snprintf(cmd, sizeof(cmd), "which %s", dbg_term);
  if (chpl_run_cmdstr(cmd, term_path, term_path_size) <= 0) {
    static char err_msg[128] = "";
    snprintf(err_msg, sizeof(err_msg), 
             "CHPL_COMM_USE_(G|LL)DB ignored because no %s", dbg_term);
    chpl_warning(err_msg, 0, 0);
    return;
  }

  char** largv = NULL;
  int largv_size = 0;
  int largc = 0;
#define ADD_LARGV(s)                                                    \
  do {                                                                  \
    if (largc >= largv_size) {                                          \
      largv_size += 10;                                                 \
      largv = chpl_mem_realloc(largv, largv_size * sizeof(*largv),      \
                               CHPL_RT_MD_COMMAND_BUFFER, -1, 0);       \
    }                                                                   \
    largv[largc++] = (char*) (s);                                       \
  } while (0)

  ADD_LARGV(term_path);
  ADD_LARGV("-e");
  if (use_gdb) {
    ADD_LARGV("gdb");
    ADD_LARGV("--args");
  } else {
    ADD_LARGV("lldb");
    ADD_LARGV("--");
  }

#undef ADD_LARGV

  *argc = largc;
  *argv = largv;
}


//
// This function returns a NULL terminated argv list as required by
// chpl_launch_using_exec(), i.e., execve(3).
//
//     argc, argv: as passed to main()
//     largc, largv: launcher command and args
//
char** chpl_bundle_exec_args(int argc, char *const argv[],
                              int largc, char *const largv[]) {
  int dbg_argc = 0;
  char** dbg_argv = NULL;
  if (argc > 0) {
    get_debugger_wrapper(&dbg_argc, &dbg_argv);
  }

  int len = argc + dbg_argc + largc + 2;
  int newargc = 0;
  char **newargv = chpl_mem_allocMany(len, sizeof(char*),
                                      CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  if (!newargv) {
    chpl_internal_error("Could not allocate memory");
  }

  newargv[len-1] = NULL;
  newargv[len-2] = NULL;

  // add any launcher args
  if (largc > 0) {
    memcpy(newargv, largv, largc*sizeof(char *));
    newargc = largc;
  }

  if (argc > 0) {
    // if there is a wrapper, add it after the launcher args
    if (strcmp(chpl_get_real_binary_wrapper(), "") != 0) {
      newargv[newargc++] = (char *) chpl_get_real_binary_wrapper();
    }

    // add any debugger wrapper
    if (dbg_argc > 0) {
      memcpy(&newargv[newargc], dbg_argv, dbg_argc * sizeof(newargv[0]));
      newargc += dbg_argc;
    }

    // add the _real binary (after launchers args and/or wrappers)
    chpl_compute_real_binary_name(argv[0]);
    newargv[newargc++] = (char *) chpl_get_real_binary_name();
    if (argc > 1) {
      // add args passed to main (skip original binary) after _real binary
      memcpy(newargv+newargc, argv+1, (argc-1)*sizeof(char *));
    }
  }

  return newargv;
}

//
// This function calls execvp(3)
//
int chpl_launch_using_exec(const char* command, char * const argv1[], const char* argv0) {
  if (verbosity > 1) {
    char * const *arg;
    if (evListSize > 0) {
      printf("%s ", evList);
    }
    printf("%s ", command);
    fflush(stdout);
    for (arg = argv1+1; *arg; arg++) {
      printf(" %s", *arg);
      fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
  }
  chpl_launch_sanity_checks(argv0);

  execvp(command, argv1);
  {
    char msg[256];
    sprintf(msg, "execvp() failed: %s", strerror(errno));
    chpl_internal_error(msg);
  }
  return -1;
}

int chpl_launch_using_fork_exec(const char* command, char * const argv1[], const char* argv0) {
  int status;
  pid_t pid = fork();
  switch (pid) {
  case 0:
    chpl_launch_using_exec(command, argv1, argv0);
    // should not return
  case -1:
  {
    char msg[256];
    sprintf(msg, "fork() failed: %s", strerror(errno));
    chpl_internal_error(msg);
  }
  default:
    {
      if (waitpid(pid, &status, 0) != pid) {
        char msg[256];
        sprintf(msg, "waitpid() failed: %s", strerror(errno));
        chpl_internal_error(msg);
      }
    }
  }
  return WEXITSTATUS(status);
}

int chpl_launch_using_system(char* command, char* argv0) {
  if (verbosity > 1) {
    if (evListSize > 0) {
      printf("%s ", evList);
    }
    printf("%s\n", command);
  }
  chpl_launch_sanity_checks(argv0);
  return system(command);
}

// This function returns a string containing a character-
// separated list of environment variables that should be
// forwarded.

char* chpl_get_enviro_keys(char sep)
{
  int pass;
  int i;
  int j;
  int k = 0;
  char* ret = NULL;

  for( pass = 0; pass < 2; pass++ ) {
    k = 0;
    for( i = 0; environ && environ[i]; i++ ) {
      // We could do this for only some environment
      // variables if we wanted to; that would amount
      // to an if statement checking environ[i];
      // but we find it to be more similar to MPI/SLURM
      // to forward all environment variables.
      // Count/store the separator
      if( k > 0 ) {
        if( pass == 0 ) k++;
        else ret[k++] = sep;
      }

      for( j = 0; environ[i][j] && environ[i][j] != '='; j++ ) {
        if( pass == 0 ) {
          // on first pass, just count.
          k++;
        } else {
          // on second pass, add to buffer.
          ret[k++] = environ[i][j];
        }
      }
    }
    if( pass == 0 ) ret = chpl_mem_allocMany(k+1, sizeof(char),
                                             CHPL_RT_MD_COMMAND_BUFFER,-1,0);
  }

  return ret;
}

static const int charset_env_nargs = 4;

int chpl_get_charset_env_nargs()
{
  return charset_env_nargs;
}

//
// Populate the argv array and return the number of arguments added.
// Return the number of arguments populated.
//
int chpl_get_charset_env_args(char *argv[])
{
  // If any of the relevant character set environment variables
  // are set, replicate the state of all of them.  This needs to
  // be done separately from the -E mechanism because Perl
  // launchers modify the character set environment, losing our
  // settings.
  //
  // Note that if we are setting these variables, and one or more
  // of them is empty, we must set it with explicitly empty
  // contents (e.g. LC_ALL= instead of -u LC_ALL) so that the
  // Chapel launch mechanism will not overwrite it.
  char *lang = getenv("LANG");
  char *lc_all = getenv("LC_ALL");
  char *lc_collate = getenv("LC_COLLATE");
  if (!lang && !lc_all && !lc_collate)
    return 0;

  argv[0] = (char *)"env";
  if (lang == NULL)
    lang = (char *)"";
  char *lang_buf = chpl_mem_allocMany(sizeof("LANG=") + strlen(lang),
                        sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  strcpy(lang_buf, "LANG=");
  strcat(lang_buf, lang);
  argv[1] = lang_buf;
  if (lc_all == NULL)
    lc_all = (char *)"";
  char *lc_all_buf = chpl_mem_allocMany(sizeof("LC_ALL=") + strlen(lc_all),
                        sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  strcpy(lc_all_buf, "LC_ALL=");
  strcat(lc_all_buf, lc_all);
  argv[2] = lc_all_buf;
  if (lc_collate == NULL)
    lc_collate = (char *)"";
  char *lc_collate_buf = chpl_mem_allocMany(
                        sizeof("LC_COLLATE=") + strlen(lc_collate),
                        sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  strcpy(lc_collate_buf, "LC_COLLATE=");
  strcat(lc_collate_buf, lc_collate);
  argv[3] = lc_collate_buf;

  return charset_env_nargs;
}

int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, int32_t filename) {
  int numHandled = chpl_launch_handle_arg(*argc, argv, argNum, 
                                          lineno, filename);
  if (numHandled == 0) {
    if (mainHasArgs) {
      chpl_gen_main_arg.argv[chpl_gen_main_arg.argc] = argv[argNum];
      chpl_gen_main_arg.argc++;
    } else {
      char* message;
      message = chpl_glom_strings(3,"Unexpected flag:  \"",argv[argNum],"\"");
      chpl_error(message, lineno, filename);
    }
    return 0;
  } else {
    int i;
    for (i=argNum+numHandled; i<*argc; i++) {
      argv[i-numHandled] = argv[i];
    }
    *argc -= numHandled;
    return -1;  // back the cursor up in order to re-parse this arg
  }
}


void printAdditionalHelp(void) {
  chpl_launch_print_help();
}

// These are defined in the config.c file, which is built
// on-the-fly in runtime/etc/Makefile.launcher.
extern const char launcher_real_suffix[];
extern const char launcher_exe_suffix[];    // May be the empty string.

#define BIN_NAME_SIZE 256
static char chpl_real_binary_name[BIN_NAME_SIZE];

void chpl_compute_real_binary_name(const char* argv0) {

  char* cursor = chpl_real_binary_name;
  int exe_length = strlen(launcher_exe_suffix);
  int length;
  const char* real_suffix = getenv("CHPL_LAUNCHER_SUFFIX");

  //
  // If we're launching a multi-locale library, skip the steps below, and use
  // the baked in executable name instead.
  //
  if (launcher_is_mli) {

    length = strlen(launcher_mli_real_name);

    if (length >= BIN_NAME_SIZE) {
      chpl_internal_error("Real executable name is too long.");
    }

    strncpy(chpl_real_binary_name, launcher_mli_real_name, BIN_NAME_SIZE);
    chpl_real_binary_name[length] = '\0';

  } else {

    if (NULL == real_suffix) {
      real_suffix = launcher_real_suffix;
    }

    length = strlen(argv0);
    if (length + strlen(launcher_real_suffix) >= BIN_NAME_SIZE)
      chpl_internal_error("Real executable name is too long.");

    // See if the launcher name contains the exe_suffix
    if (exe_length > 0 &&
        !strncmp(argv0 + length - exe_length, launcher_exe_suffix, exe_length))
      // We matched the exe suffix, so remove it before adding the real suffix.
      length -= exe_length;

    // Copy the filename sans exe suffix.
    strncpy(cursor, argv0, length);
    cursor += length;
    strcpy(cursor, launcher_real_suffix);
  }
}

// Undocumented means to wrap the _real binary. Useful for things like timing
// the _real executable or running it through some other program. Note that
// this will not work with launchers that add arguments after the first
// positional argument expecting it to be the binary. e.g. amudprun adds
// __AMUDP_SLAVE_PROCESS__* and the hostname:port. Returns the value of
// CHPL_LAUNCHER_REAL_WRAPPER if set, "" otherwise
const char* chpl_get_real_binary_wrapper(void) {
  const char* real_wrapper = getenv("CHPL_LAUNCHER_REAL_WRAPPER");
  if (real_wrapper != NULL) {
    return real_wrapper;
  } else {
    return "";
  }
}

const char* chpl_get_real_binary_name(void) {
  return &chpl_real_binary_name[0];
}

int chpl_launch_prep(int* c_argc, char* argv[], int32_t* c_execNumLocales) {
  //
  // This is a user invocation, so parse the arguments to determine
  // the number of locales.
  //
  int32_t execNumLocales;
  int argc = *c_argc;

  // Set up main argument parsing.
  chpl_gen_main_arg.argv = chpl_mem_allocMany(argc, sizeof(char*),
                                      CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  chpl_gen_main_arg.argv[0] = argv[0];
  chpl_gen_main_arg.argc = 1;
  chpl_gen_main_arg.return_value = 0;

  CreateConfigVarTable();
  parseArgs(true, parse_normally, &argc, argv);

  execNumLocales = getArgNumLocales();

  //
  // If the user did not specify a number of locales let the
  // comm layer decide how many to use (or flag an error)
  //
  if (execNumLocales == 0) {
    execNumLocales = chpl_comm_default_num_locales();
  }
  //
  // Before proceeding, allow the comm layer to verify that the
  // number of locales is reasonable
  //
  chpl_comm_verify_num_locales(execNumLocales);

  //
  // Let the comm layer do any last-minute pre-launch activities it
  // needs to.
  //
  CHPL_COMM_PRELAUNCH();

  *c_execNumLocales = execNumLocales;
  *c_argc = argc;

  return 0;
}


int chpl_launcher_main(int argc, char* argv[]) {
  int32_t execNumLocales;

  //
  // The chpl_launch_prep function calls parseArgs, which modifies argc, so
  // so we need to make sure those changes are visible before calling
  // chpl_launch.
  //
  if (chpl_launch_prep(&argc, argv, &execNumLocales)) {
    return -1;
  }

  //
  // Launch the program.
  // This may not return (e.g., if calling chpl_launch_using_exec()).
  //
  return chpl_launch(argc, argv, execNumLocales);
}
