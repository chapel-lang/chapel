/*
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

#ifndef CHPL_RUNTIME_ETC_MLI_MLI_CLIENT_RUNTIME_C_
#define CHPL_RUNTIME_ETC_MLI_MLI_CLIENT_RUNTIME_C_


#include "mli_common_code.c"
#include <sys/types.h>
#include <unistd.h>


struct chpl_mli_context chpl_client;

extern const char* mli_servername;
FILE* server_pipe;

static
void chpl_mli_client_init(struct chpl_mli_context* client) {
  if (client->context) { return; }

  client->context = zmq_ctx_new();
  client->setup_sock = zmq_socket(client->context, ZMQ_PULL);
  client->main    = zmq_socket(client->context, ZMQ_REQ);
  client->arg     = zmq_socket(client->context, ZMQ_REQ);
  client->res     = zmq_socket(client->context, ZMQ_REP);

  return;
}


void chpl_mli_client_deinit(struct chpl_mli_context* client) {
  if (!client->context) { return; }

  // TODO: Set linger on the sockets to zero to prevent blocking.
  zmq_ctx_destroy(client->context);
  client->context = NULL;

  return;
}

static void chpl_mli_spawn_server() {
  char command[FILENAME_MAX+9] = "./";
  strcat(command, mli_servername);
  strcat(command, " -nl 1");

  chpl_mli_debugf("executing %s as a subprocess\n", command);
  server_pipe = popen(command, "r");
}

void chpl_mli_terminate(enum chpl_mli_errors e) {
  const char* errstr = chpl_mli_errstr(e);
  chpl_mli_debugf("Terminated abruptly with error: %s\n", errstr);
  mli_terminate();
}


//
// TODO: In multi-locale libraries, argc/argv formals are currently ignored.
//
void chpl_library_init(int argc, char** argv) {
  static int initialized = 0;

  // Just hardcode these values for now.
  const char* ip = "localhost";
  const char* setupPort = "5554";
  const char* mainport = "5555";
  const char* argport = "5556";
  const char* resport = "5557";

  (void) argc;
  (void) argv;

  if (initialized) { return; }
  initialized = 1;

  // Set up the clientside ZMQ sockets.
  chpl_mli_client_init(&chpl_client);

  chpl_mli_bind(chpl_client.setup_sock, ip, setupPort);

  char* setup_sock_conn = chpl_mli_connection_info(chpl_client.setup_sock);
  chpl_mli_debugf("setup socket used %s\n", setup_sock_conn);

  // TODO: pass in argv/argc, connection information
  chpl_mli_spawn_server();

  mli_free(setup_sock_conn);


  // TODO: Maybe move the open connections to init?
  chpl_mli_connect(chpl_client.main, ip, mainport);
  chpl_mli_connect(chpl_client.arg, ip, argport);
  chpl_mli_connect(chpl_client.res, ip, resport);

  return;
}


void chpl_library_finalize(void) {
  static int finalized = 0;

  if (finalized) { return; }
  finalized = 1;

  {
    int64_t shutdown = CHPL_MLI_ERROR_SHUTDOWN;
    chpl_mli_push(chpl_client.main, &shutdown, sizeof(shutdown), 0);
    chpl_mli_pull(chpl_client.main, &shutdown, sizeof(shutdown), 0);

    // Can server ever respond with a different error?
    if (shutdown != CHPL_MLI_ERROR_SHUTDOWN) { ;;; }
  }

  char server_output[256];
  // TODO: intersperse server output with function calls instead of all at
  // the end
  while(!feof(server_pipe)) {
    if (fgets(server_output, 256, server_pipe) != NULL) {
      printf("%s", server_output);
    }
  }
  if (pclose(server_pipe)) {
    chpl_mli_debugf("Failed to shut down server\n");
  }

  // TODO: It would be a good idea to set LINGER to 0 as well.
  // TODO: Maybe move the close connections to deinit?
  chpl_mli_close(chpl_client.setup_sock);
  chpl_mli_close(chpl_client.main);
  chpl_mli_close(chpl_client.arg);
  chpl_mli_close(chpl_client.res);

  chpl_mli_client_deinit(&chpl_client);

  return;
}


#endif

