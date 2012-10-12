#ifndef _BSWAP_H_
#define _BSWAP_H_

// get htobe16, etc.
//#define _BSD_SOURCE
#include "sys_basic.h"


#include <sys/param.h>
#ifdef __GLIBC__
#include <endian.h>

#endif

// __bswap_64 does not compile with the PGI or Cray compilers,
// and so we switch to our own versions in those environments.
#ifdef __PGI 
#define REDO_BSWAP
#endif

#ifdef _CRAYC
#define REDO_BSWAP
#endif

#ifdef REDO_BSWAP
// Undo any bswap definitions we might have gotten ...
#undef __bswap_16
#undef __bswap_32
#undef __bswap_64
// ... and make sure we don't get them back.
#define _BITS_BYTESWAP_H 1

#undef htobe16
#undef htobe32
#undef htobe64
#undef htole16
#undef htole32
#undef htole64
#undef be16toh
#undef be32toh
#undef be64toh
#undef le16toh
#undef le32toh
#undef le64toh
#endif

#ifdef htobe64
// we should have htobe etc.

#else

#ifdef __bswap_64
// OK, we have __bswap_64
#else
// slow compatability method.
// stolen from glibc bits/byteswap.h
#define __bswap_16(x) \
  ((uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
#define __bswap_32(x) \
  ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |               \
  (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
# define __bswap_64(x) \
     (  (((x) & 0xff00000000000000ull) >> 56)                                 \
      | (((x) & 0x00ff000000000000ull) >> 40)                                 \
      | (((x) & 0x0000ff0000000000ull) >> 24)                                 \
      | (((x) & 0x000000ff00000000ull) >> 8)                                  \
      | (((x) & 0x00000000ff000000ull) << 8)                                  \
      | (((x) & 0x0000000000ff0000ull) << 24)                                 \
      | (((x) & 0x000000000000ff00ull) << 40)                                 \
      | (((x) & 0x00000000000000ffull) << 56))
#endif


// stolen from glibc endian.h
# if __BYTE_ORDER == __LITTLE_ENDIAN
#  define htobe16(x) __bswap_16 (x)
#  define htole16(x) (x)
#  define be16toh(x) __bswap_16 (x)
#  define le16toh(x) (x)

#  define htobe32(x) __bswap_32 (x)
#  define htole32(x) (x)
#  define be32toh(x) __bswap_32 (x)
#  define le32toh(x) (x)

#  define htobe64(x) __bswap_64 (x)
#  define htole64(x) (x)
#  define be64toh(x) __bswap_64 (x)
#  define le64toh(x) (x)
# else
#  define htobe16(x) (x)
#  define htole16(x) __bswap_16 (x)
#  define be16toh(x) (x)
#  define le16toh(x) __bswap_16 (x)

#  define htobe32(x) (x)
#  define htole32(x) __bswap_32 (x)
#  define be32toh(x) (x)
#  define le32toh(x) __bswap_32 (x)

#  define htobe64(x) (x)
#  define htole64(x) __bswap_64 (x)
#  define be64toh(x) (x)
#  define le64toh(x) __bswap_64 (x)
# endif
#endif


#endif
