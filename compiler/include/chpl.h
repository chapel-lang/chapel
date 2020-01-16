/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
