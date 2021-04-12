// make sure to get the XSI/POSIX.1-2001 strerror_r
#define _POSIX_C_SOURCE 200112L
#undef _GNU_SOURCE
#include "./sys_basic.h"

#include "mystrerror.h"

#include <string>

#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace chpl {

std::string my_strerror(int errno_) {
  char errbuf[256];
  int rc;
  errbuf[0] = '\0';
  rc = strerror_r(errno_, errbuf, sizeof(errbuf));
  if (rc != 0)
    strncpy(errbuf, "<unknown error>", sizeof(errbuf));
  return std::string(errbuf);
}

} // end namespace chpl
