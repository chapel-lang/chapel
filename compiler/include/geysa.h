/* -*-Mode: c++;-*-
*/
#ifndef _geysa_H_
#define _geysa_H_

#if !defined(__FreeBSD__) || (__FreeBSD_version >= 500000)
#endif
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "chplalloc.h"
#include "chpltypes.h"

#define round2(_x,_n) ((_x + ((_n)-1)) & ~((_n)-1))
#define tohex1(_x) \
((((_x)&15) > 9) ? (((_x)&15) - 10 + 'A') : (((_x)&15) + '0'))
#define tohex2(_x) \
((((_x)>>4) > 9) ? (((_x)>>4) - 10 + 'A') : (((_x)>>4) + '0'))
#define numberof(_x) ((sizeof(_x))/(sizeof((_x)[0])))

extern int verbose_level;

#include "extern.h"
#include "list.h"
#include "vec.h"
#include "map.h"
#include "misc.h"
#include "log.h"

#if defined(__CYGWIN__) || 1
#undef assert
#define assert(_x) do { if (!(_x)) myassert(__FILE__, __LINE__, #_x); } while(0)
#endif

#endif
