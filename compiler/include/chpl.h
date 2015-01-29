#ifndef _chpl_H_
#define _chpl_H_

#include "extern.h"
#include "list.h"
#include "map.h"
#include "misc.h"
#include "vec.h"

#include <climits>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#define round2(_x,_n) ((_x + ((_n)-1)) & ~((_n)-1))

#define tohex1(_x) \
((((_x)&15) > 9) ? (((_x)&15) - 10 + 'A') : (((_x)&15) + '0'))

#define tohex2(_x) \
((((_x)>>4) > 9) ? (((_x)>>4) - 10 + 'A') : (((_x)>>4) + '0'))

#define numberof(_x) ((sizeof(_x))/(sizeof((_x)[0])))

extern int verbose_level;

#endif
