#ifndef CHPL_RUNTIME_ETC_MLI_MLI_COMMON_C_
#define CHPL_RUNTIME_ETC_MLI_MLI_COMMON_C_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <zmq.h>

// Error codes must all be less than zero unless we change the protocol!
enum chpl_mli_errors {

  CHPL_MLI_ERROR_NONE       = +0,
  CHPL_MLI_ERROR_SHUTDOWN   = -1,
  CHPL_MLI_ERROR_UNKNOWN    = -2,
  CHPL_MLI_ERROR_NOFUNC     = -3,
  CHPL_MLI_ERROR_SOCKET     = -4,
  CHPL_MLI_ERROR_EXCEPT     = -5

};

//
// Both the client and server will be using this to perform RPC.
//
struct chpl_mli_context {

  void* context;
  void* main;
  void* arg;
  void* res;

};

static
char* chpl_mli_concat(char* buffer, size_t count, ...) {
  char* bufptr = buffer;
  va_list argp;
  int i = 0;
  int length = 0;

  length = strlen(buffer);
  va_start(argp, count);

  for (i = 0; i < count; i++) {
    const char* chunk = va_arg(argp, const char*);
    strcpy(bufptr, chunk);
    bufptr += strlen(chunk);
  }

  buffer[length] = 0;
  va_end(argp);

  return buffer;  
}

static
void chpl_mli_connect(void* socket, const char* ip, const char* port) {
  char* buffer = NULL;
  char* bufptr = NULL;
  size_t len = 0;

  len = strlen("tcp://") + strlen(ip) + strlen(":") + strlen(port);
  buffer = malloc(len);

  if (buffer == NULL) {
    // TODO: Respond to memory error how?
  }

  buffer = chpl_mli_concat(buffer, 4, "tcp://", ip, ":", port);
  
  // TODO: Can this error out at all?
  zmq_connect(socket, buffer);

  free(buffer);

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





#endif
