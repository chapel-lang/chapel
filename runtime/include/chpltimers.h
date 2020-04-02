/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chpltimers_h_
#define _chpltimers_h_

#include "chpltypes.h"  // For _real64.

#include <sys/time.h>   // For struct timeval.

typedef struct timeval _timevalue;

_timevalue chpl_null_timevalue(void);
_timevalue chpl_now_timevalue(void);
int64_t chpl_timevalue_seconds(_timevalue t);
int64_t chpl_timevalue_microseconds(_timevalue t);
void chpl_timevalue_parts(_timevalue t, int32_t* seconds, int32_t* minutes, int32_t* hours, int32_t* mday, int32_t* month, int32_t* year, int32_t* wday, int32_t* yday, int32_t* isdst);

#ifndef LAUNCHER

_real64 chpl_now_time(void);

#endif // LAUNCHER

#endif
