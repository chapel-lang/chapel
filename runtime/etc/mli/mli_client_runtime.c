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


//
// TODO: In multi-locale libraries, argc/argv formals are currently ignored.
//
void chpl_library_init(int argc, char** argv) {
  static int initialized = 0;

  // Just hardcode these values for now.
  const char* ip = "localhost";
  const char* setupPort = "5554"; // ephemeral, ZMQ will determine free port
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

  // Determine port used by ZMQ for setup_sock.
  size_t lenPort = 256;
  char* portRes = (char *)calloc(lenPort, sizeof(char));
  int portErr = zmq_getsockopt(chpl_client.setup_sock, ZMQ_LAST_ENDPOINT,
                               portRes, &lenPort);
  printf("connected at %s\n", portRes);
  char *traveler = strchr(portRes, ':');
  traveler = strchr(traveler + 1, ':');
  traveler++;
  printf("everything after second : %s\n", traveler);

  size_t lenHostname = 256;
  char* hostRes = (char *)calloc(lenHostname, sizeof(char));
  err_t hostErr = gethostname(hostRes, lenHostname);
  printf("connected on host %s\n", hostRes);
  char* fullConnection = (char *)calloc(lenHostname + lenPort, sizeof(char));
  strcpy(fullConnection, "tcp://");
  strcat(fullConnection, hostRes);
  strcat(fullConnection, ":");
  strcat(fullConnection, traveler);
  printf("modified string is %s\n", fullConnection);
  free(fullConnection);
  free(hostRes);
  free(portRes); // Keep this until after we send it to the server

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

  // TODO: Shut down the server and handle errors.
  if (false) {
    int64_t shutdown = -1;
    chpl_mli_push(chpl_client.main, &shutdown, sizeof(shutdown), 0);     
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

