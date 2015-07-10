/*
 * Copyright 2004-2015 Cray Inc.
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

//
// Visual Debug support (Shared Interface)
//

#ifndef _chpl_visual_debug_h_
#define _chpl_visual_debug_h_

#include <stdint.h>
#include <stdarg.h>
#include "chpl-tasks.h"

extern int chpl_vdebug_fd;    // fd of output file, 0 => not gathering data
extern int chpl_vdebug;       // Should we generate debug data

// Linux and MacOS don't do a single write.  We require a single write.
extern int chpl_dprintf(int fd, const char * format, ...)
#ifdef __GNUC__
      __attribute__ ((format (printf, 2, 3)))
#endif
   ;

//  don't log messages/forks associated with a tag.
extern void chpl_vdebug_nolog(void);
//  start and open file if not NULL
extern void chpl_vdebug_start(const char *, double now); 
//  stop collecting data
extern void chpl_vdebug_stop(void);
//  Tag the data with a character tag, and possibly resume
extern void chpl_vdebug_tag(const char *);
//  resume from a tag point
extern void chpl_vdebug_pause(void);

#endif


