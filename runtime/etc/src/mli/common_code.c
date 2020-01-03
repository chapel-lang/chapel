/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef CHPL_RUNTIME_ETC_SRC_MLI_COMMON_CODE_C_
#define CHPL_RUNTIME_ETC_SRC_MLI_COMMON_CODE_C_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <zmq.h>

//
// Grab headers for shims used by the client and server.
//
// The Chapel runtime is sanitized during Travis testing, and any calls to
// LIBC functions are reported as errors. Sadly, our code gets sanitized too,
// even though it _technically_ isn't part of the runtime itself. That's the
// reason why any of these shims exist in the first place.
//
// We really don't want to call "chpl_exit_any()" in the client - we'd prefer
// the LIBC "exit()" function. Fortunately, the LAUNCHER already accounts for
// this. So we "gain access" to "exit()" by pretending to be the launcher.
// This defines "chpl_exit_any()" as a macro representing "exit()".
//
#ifdef CHPL_MLI_IS_SERVER
#   include "chpl-mem.h"
#   include "error.h"
#elif defined(CHPL_MLI_IS_CLIENT)
#   ifndef LAUNCHER
#     define CHPL_MLI_LAUNCHER_IS_DEFINED
#     define LAUNCHER
# endif
#   include "chpl-mem-sys.h"
#   include "chplexit.h"
#   ifdef CHPL_MLI_LAUNCHER_IS_DEFINED
#     undef CHPL_MLI_LAUNCHER_IS_DEFINED
#     undef LAUNCHER
#   endif
#endif

//
// If this code is being run on the server, mli_malloc() is a wrapper for
// chpl_malloc(). If this code is being run on the client, then it is a
// wrapper for the system allocator.
//
#ifdef CHPL_MLI_IS_SERVER
#   define mli_malloc(bytes) chpl_malloc(bytes)
#   define mli_free(ptr) chpl_free(ptr)
#elif defined(CHPL_MLI_IS_CLIENT)
#   define mli_malloc(bytes) sys_malloc(bytes)
#   define mli_free(ptr) sys_free(ptr)
#else
#   error The mli_malloc/free macros were defined outside of client/server.
#endif

//
// Terminate all the locales properly if the server, else just "exit()".
//
#ifdef CHPL_MLI_IS_SERVER
#   define mli_terminate() chpl_error("", 0, 0)
#elif defined(CHPL_MLI_IS_CLIENT)
#   define mli_terminate() chpl_exit_any(1)
#else
#   error The mli_terminate macro was defined outside of client/server.
#endif

//
// Print prefix, used to indicate whether client or server is printing.
//
#ifdef CHPL_MLI_IS_SERVER
#   define chpl_mli_pfx "[Server]"
#elif defined(CHPL_MLI_IS_CLIENT)
#   define chpl_mli_pfx "[Client]"
#else
#   error The chpl_mli_pfx macro was defined outside of client/server.
#endif

//
// Print a debug message, or do nothing if debug messages are turned off.
//
#ifdef CHPL_MLI_DEBUG_PRINT
#   ifndef chpl_mli_pfx
#     error The chpl_mli_pfx macro must be defined.
#   endif
#   define chpl_mli_debugf(fmt, ...)              \
  do {                                            \
      printf("%s ", chpl_mli_pfx);                \
      printf(fmt, __VA_ARGS__);                   \
  } while (0)
#else
#   define chpl_mli_debugf(fmt, ...)
#endif

//
// Error codes must all be less than zero unless we change the protocol!
//
enum chpl_mli_errors {

  CHPL_MLI_CODE_NONE      = +0,
  CHPL_MLI_CODE_SHUTDOWN  = -1,
  CHPL_MLI_CODE_EUNKNOWN  = -2,
  CHPL_MLI_CODE_ENOFUNC   = -3,
  CHPL_MLI_CODE_ESOCKET   = -4,
  CHPL_MLI_CODE_EEXCEPT   = -5,
  CHPL_MLI_CODE_EMEMORY   = -6

};

const char* chpl_mli_errstr(enum chpl_mli_errors e);

const char* chpl_mli_errstr(enum chpl_mli_errors e) {
  static const char* mli_errors_[] = {
    "NONE",
    "SHUTDOWN",
    "EUNKNOWN",
    "ENOFUNC",
    "ESOCKET",
    "EEXCEPT",
    "EMEMORY"
  };

  if (e > CHPL_MLI_CODE_NONE || e < CHPL_MLI_CODE_EMEMORY) {
    return "INVALID_ERROR_CODE";
  }

  // Remember to negate the error code!
  return mli_errors_[-e];
}

//
// Both the client and server will be using this to communicate.
//
struct chpl_mli_context {

  void* context;
  void* setup_sock;
  void* main;
  void* arg;
  void* res;

};

static
char* chpl_mli_concat(size_t count, ...) {
  char* result = NULL;
  char* bufptr = NULL;
  va_list argp;
  size_t i = 0;
  size_t length = 0;

  va_start(argp, count);

  for (i = 0; i < count; i++) {
    const char* chunk = va_arg(argp, const char*);
    length += strlen(chunk);
  }

  result = mli_malloc(length + 1);
  if (result == NULL) { return NULL; }

  va_start(argp, count);
  bufptr = result;

  for (i = 0; i < count; i++) {
    const char* chunk = va_arg(argp, const char*);
    strcpy(bufptr, chunk);
    bufptr += strlen(chunk);
  }

  result[length] = 0;
  va_end(argp);

  return result;  
}

//
// Bind the socket, always to an ephemeral port and ip address (connect
// is what needs an explicit port/ip address)
//
static
void chpl_mli_bind(void* socket) {
  const char* real_ip = "tcp://*:*";
  int err = 0;

  // TODO: Remove this assert / make this more robust.
  err = zmq_bind(socket, real_ip);
  assert(err == 0);

  return;
}

static
void chpl_mli_connect(void* socket, const char* conn) {
  int err = 0;

  // TODO: Remove this assert / make this more robust. 
  err = zmq_connect(socket, conn);
  assert(err == 0);

  return;  
}

static
void chpl_mli_close(void* socket) {
  // TODO: Should set LINGER to zero if we don't want to block.
  zmq_close(socket);
}

static
int chpl_mli_push(void* socket, void* buffer, size_t bytes, int flags) {
  return zmq_send(socket, buffer, bytes, flags);  
}

static
int chpl_mli_pull(void* socket, void* buffer, size_t bytes, int flags) {
  return zmq_recv(socket, buffer, bytes, flags);
}

// Determine connection information for that socket.
static
char * chpl_mli_connection_info(void* socket) {
  // Determine port used by ZMQ for socket.
  size_t lenPort = 256;
  char* portRes = (char *)mli_malloc(lenPort);
  int portErr = zmq_getsockopt(socket, ZMQ_LAST_ENDPOINT,
                               portRes, &lenPort);

  // Get to port portion of the last endpoint.
  char *traveler = strchr(portRes, ':');
  traveler = strchr(traveler + 1, ':');
  traveler++;

  int lenConnBoilerplate = strlen("tcp://:");
  char* chpl_rt_masterip = getenv("CHPL_RT_MASTERIP");
  chpl_mli_debugf("got env var value for CHPL_RT_MASTERIP: %s\n",
                  chpl_rt_masterip);
  char* fullConnection = NULL;
  if (chpl_rt_masterip != NULL) {
    int lenMasterip = strlen(chpl_rt_masterip);
    int lenFull = lenConnBoilerplate + lenMasterip + lenPort;
    fullConnection = (char*)mli_malloc(lenFull + 1);
    strcpy(fullConnection, "tcp://");
    strcat(fullConnection, chpl_rt_masterip);
    chpl_mli_debugf("full connection before port was %s\n", fullConnection);

  } else {
    // Determine hostname of where we are currently running
    size_t lenHostname = 256;
    char* hostRes = (char *)mli_malloc(lenHostname);
    int hostErr = gethostname(hostRes, lenHostname);
    chpl_mli_debugf("hostname was %s\n", hostRes);

    // Recreate the connection using the hostname instead of 0.0.0.0
    int lenFull = lenHostname + lenPort + lenConnBoilerplate;
    fullConnection = (char *)mli_malloc(lenFull + 1);
    strcpy(fullConnection, "tcp://");
    strcat(fullConnection, hostRes);
    chpl_mli_debugf("full connection before port was %s\n", fullConnection);
    mli_free(hostRes);
  }
  strcat(fullConnection, ":");
  strcat(fullConnection, traveler);

  mli_free(portRes);
  return fullConnection;
}

#endif
