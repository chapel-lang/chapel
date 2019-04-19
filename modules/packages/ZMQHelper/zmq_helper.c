#include "ZMQHelper/zmq_helper.h"

// Used when the option specified for zmq_getsockopt would modify a char*,
// due to c_strings in Chapel being const char*.
int zmq_getsockopt_string_helper(void* s, int option, const char** res) {
  size_t len = 256;
  char* resbuf = (char *)chpl_calloc(len, sizeof(char));
  int err = zmq_getsockopt(s, option, resbuf, &len);
  *res = resbuf;
  return err;
}

// Used when the option specified for zmq_getsockopt would modify the int
// argument.  Mostly done for symmetry, this version could likely have been
// called directly from Chapel.
int zmq_getsockopt_int_helper(void* s, int option, int* res) {
  size_t intsize = sizeof(*res);
  int err = zmq_getsockopt(s, option, res, &intsize);
  return err;
}
