#ifndef CHPL_RUNTIME_ETC_MLI_MLI_CLIENT_RUNTIME_C_
#define CHPL_RUNTIME_ETC_MLI_MLI_CLIENT_RUNTIME_C_

#include "mli_common_code.c"

//
// A little bit of global state will make our lives easier down the road.
//
struct chpl_mli_context chpl_client;

static
void chpl_mli_client_init(struct chpl_mli_context* client) {
  if (client->context) { return; }

  client->context = zmq_ctx_new();
  client->main    = zmq_socket(client->context, ZMQ_REQ);
  client->arg     = zmq_socket(client->context, ZMQ_PUB);
  client->res     = zmq_socket(client->context, ZMQ_SUB);
  
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
// TODO: This should spawn the server as a process on the current host.
//
void chpl_mli_server_spawn(const char* ip, const char* port) {

  return;
}

//
// TODO
//
// This function is responsible for AT LEAST the following things. The idea
// here is that we transparently override the "chpl_library_init" that is
// provided by the runtime.
//
//    1) Set up the clientside ZMQ sockets.
//    2) Spawn the server.
//    3) Ensure the server and client are connected.
//
// For right now, we spawn the generated launcher (because that's the most
// simple for me). Eventually, we should absorb the launcer into the library,
// so that no launcher executable is generated.
//
// Eventually, we would like to expose some way for the user to choose which
// port to spawn the server on, or even let them connect remotely.
//
// Ideally, this should all be handled via the single init function, but I am
// not sure how, yet.
// 
// --
//
void chpl_library_init(int argc, char** argv) {
  static int initialized = 0;

  if (initialized) { return; }
  initialized = 1;

  // Set up the clientside ZMQ sockets.
  chpl_mli_client_init(&chpl_client);

  // TODO: Spawn the server.
  chpl_mli_server_spawn(NULL, "5555");

  // Just hardcode these values for now.
  chpl_mli_connect(chpl_client.main, "localhost", "5555");
  chpl_mli_connect(chpl_client.arg, "localhost", "5555");
  chpl_mli_connect(chpl_client.res, "localhost", "5555");

  return;
}

//
// TODO
//
//
// --
//
void chpl_library_finalize(void) {
  static int finalized = 0;

  if (finalized) { return; }
  finalized = 1;

  //
  // TODO: Tell the server to close.
  //

  // TODO: It would be a good idea to set LINGER to 0 as well.
  chpl_mli_close(chpl_client.main);
  chpl_mli_close(chpl_client.arg);
  chpl_mli_close(chpl_client.res);

  chpl_mli_client_deinit(&chpl_client);

  return;
}

#endif

