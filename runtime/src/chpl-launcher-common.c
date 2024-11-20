/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include <stdio.h>
#include <stdlib.h>
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
#include "chplexit.h"
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"
#include "whereami.c"

// used in get_enviro_keys
extern char** environ;

// This global variable stores the arguments to main
// that should be handled by the program.
chpl_main_argument chpl_gen_main_arg;
// This variable is normally declared in config.h
// and comes from the generated code.
extern const int mainHasArgs;
extern const int mainPreserveDelimiter;
extern const int launcher_is_mli;
extern const char* launcher_mli_real_name;


//
// Are we doing a dry run, printing the system launcher command but
// not running it?
//
#define CHPL_DRY_RUN_ARG "--dry-run"

static int dryRunFlag = 0;

int chpl_doDryRun(void) {
  return dryRunFlag;
}


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
  char buf[buflen];
  char *cur;
  int fdo[2], outfd;
  int fde[2], errfd;
  fd_set set;
  pid_t pid;
  int status;
  int rv, numRead;

  if (pipe(fdo) < 0) {
    snprintf(buf, sizeof(buf), "Unable to run '%s' (pipe failed): %s\n",
             command, strerror(errno));
    chpl_internal_error(buf);
  }
  if (pipe(fde) < 0) {
    snprintf(buf, sizeof(buf), "Unable to run '%s' (pipe failed): %s\n",
             command, strerror(errno));
    chpl_internal_error(buf);
  }

  pid = fork();
  switch (pid) {
  case 0: // child should exit on errors
    close(fdo[0]);
    if (fdo[1] != STDOUT_FILENO) {
      if (dup2(fdo[1], STDOUT_FILENO) != STDOUT_FILENO) {
        snprintf(buf, sizeof(buf), "Unable to run '%s' (dup2 failed): %s",
                command, strerror(errno));
        chpl_internal_error(buf);
      }
    }
    close(fde[0]);
    if (fde[1] != STDERR_FILENO) {
      if (dup2(fde[1], STDERR_FILENO) != STDERR_FILENO) {
        snprintf(buf, sizeof(buf), "Unable to run '%s' (dup2 failed): %s",
                command, strerror(errno));
        chpl_internal_error(buf);
      }
    }
    execvp(command, argv);
    // should only return on error
    snprintf(buf, sizeof(buf), "Unable to run '%s': %s",
                command, strerror(errno));
    chpl_internal_error(buf);
  case -1:
    snprintf(buf, sizeof(buf), "Unable to run '%s' (fork failed): %s",
            command, strerror(errno));
    chpl_warning(buf, 0, 0);
    return -1;
  default:
    outfd = fdo[0];
    errfd = fde[0];
    close(fdo[1]);
    close(fde[1]);
    numRead = 0;
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
        } else {
          snprintf(buf, sizeof(buf), "Unable to run '%s' (read failed): %s",
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
        } else {
          snprintf(buf, sizeof(buf), "Unable to run '%s' (read failed): %s",
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
      snprintf(buf, sizeof(buf), "Unable to run '%s' (no bytes read)",
               command);
      chpl_warning(buf, 0, 0);
      return 0;
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
// Find the named executable in the PATH, if it's there.
//
char *chpl_find_executable(const char *prog_name) {
  const char *cmd_fmt = "which %s";
  const int cmd_len
            = strlen(cmd_fmt)     // 'which' command, as printf() format
              - 2                 //   length of "%s" specifier
              + strlen(prog_name) //   length of prog_name
              + 1;                //   length of trailing '\0'
  char cmd[cmd_len];
  (void) snprintf(cmd, sizeof(cmd), cmd_fmt, prog_name);

  // hopefully big enough; PATH_MAX is problematic, but what's better?
  const size_t path_len = PATH_MAX;
  char *path = chpl_mem_alloc(path_len,
                              CHPL_RT_MD_COMMAND_BUFFER, -1, 0);

  return (chpl_run_cmdstr(cmd, path, path_len) > 0) ? path : NULL;
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
    snprintf(evList, (size_t) evListSize, "%s=%s", evName, evVal);
  } else {
    snprintf(evList + evListSizeWas - 1,
             (size_t) (evListSize - (evListSizeWas - 1)),
             " %s=%s", evName, evVal);
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

  char *term_path;
  if ((term_path = chpl_find_executable(dbg_term)) == NULL) {
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
// Print out the system launcher command (used by --verbose and
// --dry-run).
//
static
void print_sys_launch_cmd(FILE *f, const char* command, char * const argv[]) {
  if (evListSize > 0) {
    fprintf(f, "%s ", evList);
  }
  // TODO: remove this sanity check
  if (command != NULL && strcmp(command, argv[0]) != 0) {
    chpl_internal_error("command, argv[0]) != 0");
  }
  for (char * const *arg = argv; *arg != NULL; arg++) {
    fprintf(f, "%s%s", (arg == argv) ? "" : " ", *arg);
  }
  fprintf(f, "\n");
  fflush(f);
}


//
// This function calls execvp(3)
//
int chpl_launch_using_exec(const char* command, char * const argv1[], const char* argv0) {
  if (verbosity > 1 || chpl_doDryRun()) {
    print_sys_launch_cmd(stdout, command, argv1);
  }
  chpl_launch_sanity_checks(argv0);

  if (chpl_doDryRun()) {
    chpl_exit_any(0);
  }

  execvp(command, argv1);
  {
    char msg[256];
    snprintf(msg, sizeof(msg), "execvp() failed for command %s: %s", command,
             strerror(errno));
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
    snprintf(msg, sizeof(msg), "fork() failed: %s", strerror(errno));
    chpl_internal_error(msg);
  }
  default:
    {
      if (waitpid(pid, &status, 0) != pid) {
        char msg[256];
        snprintf(msg, sizeof(msg), "waitpid() failed: %s", strerror(errno));
        chpl_internal_error(msg);
      }
    }
  }
  return WEXITSTATUS(status);
}

int chpl_launch_using_system(char* command, char* argv0) {
  if (verbosity > 1 || chpl_doDryRun()) {
    char * const argv[] = { command, NULL };
    print_sys_launch_cmd(stdout, NULL, argv);
  }
  chpl_launch_sanity_checks(argv0);
  return chpl_doDryRun() ? 0 : system(command);
}

// This function returns a string containing a character-
// separated list of environment variables that should be
// forwarded.

char* chpl_get_enviro_keys(char sep)
{
  // count the variables in environ, and how many characters in each name
  int numVars = 0;
  int numChars = 0;
  for(int i = 0; environ && environ[i]; i++) {
    numVars++;
    int keyLen = strstr(environ[i], "=") - environ[i];

    // if the key ends with _modshare, skip it
    if (keyLen > 8 && strncmp(environ[i] + keyLen - 8, "_modshare", 9) == 0) {
      continue;
    }
    numVars++;
    numChars += keyLen;
  }
  // allocate space for the keys, the separators, and the null terminator
  int bufferLength = numChars + numVars + 1;
  char* buffer = chpl_mem_allocMany(bufferLength, sizeof(char),
                                 CHPL_RT_MD_COMMAND_BUFFER, -1, 0);

  // copy the keys into the buffer
  int bufferOffset = 0;
  for(int i = 0; environ && environ[i]; i++) {

    int keyLen = strstr(environ[i], "=") - environ[i];
    // skip keys that end with _modshare
    if (keyLen > 8 && strncmp(environ[i] + keyLen - 8, "_modshare", 9) == 0) {
      continue;
    }
    strncpy(buffer + bufferOffset, environ[i], keyLen);
    bufferOffset += keyLen;

    buffer[bufferOffset] = sep;
    bufferOffset++;
  }
  buffer[bufferOffset] = '\0';
  buffer[bufferLength-1] = '\0';

  return buffer;
}


static const
argDescTuple_t universalArgs[]
               = { { CHPL_DRY_RUN_ARG,
                     "just print system launcher command, don't run it" },
                   { NULL, NULL },
                 };


int handleNonstandardArg(int* argc, char* argv[], int argNum,
                         int32_t lineno, int32_t filename) {
  int numHandled = chpl_launch_handle_arg(*argc, argv, argNum,
                                          lineno, filename);
  if (numHandled == 0) {
    //
    // The specific launcher didn't handle this arg.  Check to see if
    // it's one that is universal to all launchers.
    //
    if (strcmp(argv[argNum], CHPL_DRY_RUN_ARG) == 0) {
      dryRunFlag = 1;
      numHandled = 1;
    }
  }

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
  }

  //
  // Remove the handled arg from caller's argv and tell them to
  // continue parsing where what used to be the next arg now is.
  //
  int i;
  for (i=argNum+numHandled; i<*argc; i++) {
    argv[i-numHandled] = argv[i];
  }
  *argc -= numHandled;
  return -1;
}


static void printAdditionalHelpEntry(const argDescTuple_t* argTuple,
                                     size_t argFieldWidth);

void printAdditionalHelp(void) {
  const argDescTuple_t* argSources[] = { chpl_launch_get_help(),
                                         universalArgs };

  //
  // So we can format nicely, first figure out the longest arg we'll
  // need to print, then do the actual printing.
  //
  size_t argLenMax = 0;
  for (int i = 0; i < sizeof(argSources) / sizeof(argSources[0]); i++) {
    if (argSources[i] != NULL) {
      for (const argDescTuple_t* p = argSources[i]; p->arg != NULL; p++) {
        size_t argLen = strlen(p->arg);
        if (argLen > argLenMax) {
          argLenMax = argLen;
        }
      }
    }
  }

  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  for (int i = 0; i < sizeof(argSources) / sizeof(argSources[0]); i++) {
    if (argSources[i] != NULL) {
      for (const argDescTuple_t* p = argSources[i]; p->arg != NULL; p++) {
        printAdditionalHelpEntry(p, argLenMax);
      }
    }
  }
}

static
void printAdditionalHelpEntry(const argDescTuple_t* argTuple,
                              size_t argFieldWidth) {
  fprintf(stdout,
          "  %-*s  %s %s\n",
          (int) argFieldWidth, argTuple->arg,
          (argTuple->arg[0] == '\0') ? " " : ":",
          argTuple->desc);
}


// These are defined in the config.c file, which is built
// on-the-fly in runtime/etc/Makefile.launcher.
extern const char launcher_real_suffix[];
extern const char launcher_exe_suffix[];    // May be the empty string.

static char* chpl_real_binary_name;

void chpl_compute_real_binary_name(const char* argv0) {

  char* cursor;// = chpl_real_binary_name;
  int exe_length = strlen(launcher_exe_suffix);
  int length;
  const char* real_suffix = getenv("CHPL_LAUNCHER_SUFFIX");

  //
  // If we're launching a multi-locale library, skip the steps below, and use
  // the baked in executable name instead.
  //
  if (launcher_is_mli) {

    length = strlen(launcher_mli_real_name);
    chpl_real_binary_name = chpl_mem_alloc(length+1,
                                           CHPL_RT_MD_COMMAND_BUFFER, -1, 0);

    strcpy(chpl_real_binary_name, launcher_mli_real_name);
    chpl_real_binary_name[length] = '\0';

  } else {

    if (NULL == real_suffix) {
      real_suffix = launcher_real_suffix;
    }

    length = wai_getExecutablePath(NULL, 0, NULL);

    // See if the launcher name contains the exe_suffix
    if (exe_length > 0 &&
        !strncmp(argv0 + length - exe_length, launcher_exe_suffix, exe_length))
      // We matched the exe suffix, so remove it before adding the real suffix.
      length -= exe_length;

    chpl_real_binary_name = chpl_mem_alloc(length +
                                           strlen(launcher_real_suffix)+1,
                                           CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    cursor = chpl_real_binary_name;

    // Copy the filename sans exe suffix.
    wai_getExecutablePath(cursor, length, NULL);
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

void chpl_launcher_get_job_name(char *baseName, char *jobName, int jobLen) {
  const char* prefix = getenv("CHPL_LAUNCHER_JOB_PREFIX");
  const char* name = getenv("CHPL_LAUNCHER_JOB_NAME");

  if (prefix == NULL) {
    prefix = "CHPL-";
  }
  if (name == NULL) {
    snprintf(jobName, jobLen, "%s%.10s", prefix, baseName);
  } else {
    strncpy(jobName, name, jobLen);
    jobName[jobLen-1] = '\0';
  }
}


int chpl_launch_prep(int* c_argc, char* argv[], int32_t* c_execNumLocales,
                     int32_t* c_execNumLocalesPerNode) {
  //
  // This is a user invocation, so parse the arguments to determine
  // the number of locales.
  //
  int32_t execNumLocales;
  int32_t execNumLocalesPerNode;
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

  execNumLocalesPerNode = getArgNumLocalesPerNode();
  if (execNumLocalesPerNode > 1) {
    chpl_comm_verify_supports_colocales(execNumLocalesPerNode);
  }


  //
  // Let the comm layer do any last-minute pre-launch activities it
  // needs to.
  //
  CHPL_COMM_PRELAUNCH(execNumLocales);

  *c_execNumLocales = execNumLocales;
  *c_execNumLocalesPerNode = execNumLocalesPerNode;
  *c_argc = argc;

  return 0;
}


int chpl_launcher_main(int argc, char* argv[]) {
  int32_t execNumLocales;
  int32_t execNumLocalesPerNode;

  //
  // The chpl_launch_prep function calls parseArgs, which modifies argc, so
  // so we need to make sure those changes are visible before calling
  // chpl_launch.
  //
  if (chpl_launch_prep(&argc, argv, &execNumLocales,
                       &execNumLocalesPerNode)) {
    return -1;
  }

  //
  // Launch the program.
  // This may not return (e.g., if calling chpl_launch_using_exec()).
  //
  int retval = chpl_launch(argc, argv, execNumLocales, execNumLocalesPerNode);
  chpl_mem_free(chpl_real_binary_name, 0, 0);
  return retval;
}

void chpl_launcher_no_colocales_error(const char *name) {
  char msg[100];
  if (name == NULL) {
    name = CHPL_LAUNCHER;
  }
  snprintf(msg, sizeof(msg), "'%s' launcher does not support co-locales.", name);
  chpl_error(msg, 0, 0);
}
