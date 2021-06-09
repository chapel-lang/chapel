/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RUNTIME_ETC_SRC_MLI_COMMON_CODE_C_
#define CHPL_RUNTIME_ETC_SRC_MLI_COMMON_CODE_C_

// Include these for definitions of interop types.
#include "chpl-export-wrappers.h"
#include "chpl-external-array.h"

// The server shims will make some runtime calls.
#ifdef CHPL_MLI_IS_SERVER
  #include "chplexit.h"
  #include "chpl-mem.h"
  #include "error.h"
#endif

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <zmq.h>

//
// Define a bunch of shims that are used by both the client and the server.
// For the server, these refer to functions defined in the Chapel runtime,
// but for the client, they refer to standard library functions.
//

#if !defined(CHPL_MLI_IS_CLIENT) && !defined(CHPL_MLI_IS_SERVER)
  #error Client/server identity macro is not defined!
#endif

#if defined(CHPL_MLI_IS_CLIENT) && defined(CHPL_MLI_IS_SERVER)
  #error Client/server identity macro are both defined!
#endif

// Shims for memory allocation functions.
#ifdef CHPL_MLI_IS_SERVER
#   define chpl_mli_malloc(bytes) \
           chpl_mem_alloc(bytes, CHPL_RT_MD_MLI_DATA, 0, 0)
#   define chpl_mli_free(ptr) chpl_mem_free(ptr, 0, 0)
#else
#   define chpl_mli_malloc(bytes) malloc(bytes)
#   define chpl_mli_free(ptr) free(ptr)
#endif

// Shim for exit.
#ifdef CHPL_MLI_IS_SERVER
#   define chpl_mli_exit(status) chpl_exit_any(status)
# else
#   define chpl_mli_exit(status) exit(status)
#endif

// Shim for terminate (exit with error).
// TODO: Need to make sure this exits both the client and the server.
#ifdef CHPL_MLI_IS_SERVER
#   define chpl_mli_terminate() chpl_error("", 0, 0)
# else
#   define chpl_mli_terminate() exit(1)
#endif

// Print prefix, use for debugging.
#ifdef CHPL_MLI_IS_SERVER
#   define chpl_mli_pfx "[Server]"
#else
#   define chpl_mli_pfx "[Client]"
#endif

// Print a debug message, or do nothing if debug messages are turned off.
#ifdef CHPL_MLI_DEBUG_PRINT
#   define chpl_mli_debugf(fmt, ...)                  \
  do {                                                \
      printf("%s %s: ", chpl_mli_pfx, __FUNCTION__);  \
      printf(fmt, __VA_ARGS__);                       \
  } while (0)
#else
#   define chpl_mli_debugf(fmt, ...)
#endif

// Error codes must all be less than zero unless we change the protocol.
enum chpl_mli_errors {

  CHPL_MLI_CODE_NONE      = +0,
  CHPL_MLI_CODE_SHUTDOWN  = -1,
  CHPL_MLI_CODE_UNKNOWN   = -2,
  CHPL_MLI_CODE_NOFUNC    = -3,
  CHPL_MLI_CODE_SOCKET    = -4,
  CHPL_MLI_CODE_MEMORY    = -6

};

const char* chpl_mli_errstr(enum chpl_mli_errors e);

const char* chpl_mli_errstr(enum chpl_mli_errors e) {
  static const char* mli_errors_[] = {
    "NONE",
    "SHUTDOWN",
    "UNKNOWN",
    "NOFUNC",
    "SOCKET",
    "MEMORY"
  };

  if (e > CHPL_MLI_CODE_NONE || e < CHPL_MLI_CODE_MEMORY) {
    return "<invalid>";
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

  result = chpl_mli_malloc(length + 1);
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
int chpl_mli_zmq_wire(void* socket, void* buffer, size_t bytes, int push) {
  return push ? zmq_send(socket, buffer, bytes, 0):
                zmq_recv(socket, buffer, bytes, 0);
}

static
int chpl_mli_zmq_move(void* socket, void* buffer, size_t bytes, int push) {
  int result = 0;
  int retry = 0;

  result = chpl_mli_zmq_wire(socket, buffer, bytes, push);

  while (-1 == result && errno == EAGAIN) {
    chpl_mli_debugf("Socket busy, retry: %d\n", retry);
    result = chpl_mli_zmq_wire(socket, buffer, bytes, push);
    if (retry++ > 5) {
      chpl_mli_debugf("%s\n", "Too many retries!");
      chpl_mli_terminate();
    }
  }

  // TODO: We need to handle socket errors more robustly.
  if (-1 == result) {
    chpl_mli_debugf("Failed to push to socket: %p\n", socket);
    chpl_mli_debugf("%s\n", zmq_strerror(errno));
    chpl_mli_terminate();
  }

  return result;
}

static
int chpl_mli_push(void* socket, void* buffer, size_t bytes) {
  chpl_mli_debugf("%zu bytes at %p to %p\n",
                  bytes,
                  buffer,
                  socket);

  return chpl_mli_zmq_move(socket, buffer, bytes, 1);
}

static
int chpl_mli_pull(void* socket, void* buffer, size_t bytes) {
  chpl_mli_debugf("%zu bytes at %p to %p\n",
                  bytes,
                  buffer,
                  socket);

  return chpl_mli_zmq_move(socket, buffer, bytes, 0);
}

static
int chpl_mli_push_ack(void* socket) {
  return chpl_mli_push(socket, NULL, 0);
}

static
int chpl_mli_pull_ack(void* socket) {
  return chpl_mli_pull(socket, NULL, 0);
}

static
int chpl_mli_push_byte_buffer(void* socket, chpl_byte_buffer* obj) {
  chpl_mli_push(socket, &obj->size, sizeof(obj->size));
  chpl_mli_pull_ack(socket);
  chpl_mli_debugf("Byte buffer length: %" PRIu64 "\n", obj->size);

  chpl_mli_push(socket, obj->data, obj->size);
  chpl_mli_pull_ack(socket);

  // TODO: User data may not be trustable? E.g. not null terminated...
  // TODO: Truncate beyond a certain length.
  chpl_mli_debugf("Byte buffer data: %s\n", obj->data);

  return 0;
}

static
int chpl_mli_pull_byte_buffer(void* socket, chpl_byte_buffer* obj) {
  chpl_mli_pull(socket, &obj->size, sizeof(obj->size));
  chpl_mli_push_ack(socket);
  chpl_mli_debugf("Byte buffer length: %" PRIu64 "\n", obj->size);

  obj->data = chpl_mli_malloc(obj->size + 1);
  obj->isOwned = true;

  // TODO: Need to handle synchronous termination.
  if (NULL == obj->data) {
    chpl_mli_debugf("%s\n", "Failed to allocate buffer!");
    chpl_mli_terminate();
  }

  chpl_mli_pull(socket, obj->data, obj->size);
  chpl_mli_push_ack(socket);

  obj->data[obj->size] = 0;

  chpl_mli_debugf("Byte buffer data: %s\n", obj->data);

  return 0;
}

// Determine connection information for that socket.
static
char * chpl_mli_connection_info(void* socket) {
  // Determine port used by ZMQ for socket.
  size_t lenPort = 256;
  char* portRes = (char *)chpl_mli_malloc(lenPort);
  int portErr = zmq_getsockopt(socket, ZMQ_LAST_ENDPOINT,
                               portRes, &lenPort);
  (void) portErr;

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
    fullConnection = (char*)chpl_mli_malloc(lenFull + 1);
    strcpy(fullConnection, "tcp://");
    strcat(fullConnection, chpl_rt_masterip);
    chpl_mli_debugf("full connection before port was %s\n", fullConnection);

  } else {
    // Determine hostname of where we are currently running
    size_t lenHostname = 256;
    char* hostRes = (char *)chpl_mli_malloc(lenHostname);
    int hostErr = gethostname(hostRes, lenHostname);
    (void) hostErr;
    chpl_mli_debugf("hostname was %s\n", hostRes);

    // Recreate the connection using the hostname instead of 0.0.0.0
    int lenFull = lenHostname + lenPort + lenConnBoilerplate;
    fullConnection = (char *)chpl_mli_malloc(lenFull + 1);
    strcpy(fullConnection, "tcp://");
    strcat(fullConnection, hostRes);
    chpl_mli_debugf("full connection before port was %s\n", fullConnection);
    chpl_mli_free(hostRes);
  }
  strcat(fullConnection, ":");
  strcat(fullConnection, traveler);

  chpl_mli_free(portRes);
  return fullConnection;
}

#endif
