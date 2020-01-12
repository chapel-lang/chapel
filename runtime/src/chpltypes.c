/*
 * Copyright 2004-2020 Cray Inc.
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

#include "chplrt.h"

#include "chplfp.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chpl-comm-compiler-macros.h"
#include "error.h"

#include <complex.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int64_t real2int( _real64 f) {
  // need to use a union here rather than a pointer cast to avoid gcc
  // warnings when compiling -O3
  union {
    _real64 r;
    uint64_t u;
  } converter;

  converter.r = f;
  return converter.u;
}


int64_t
object2int( _chpl_object o) {
  return (intptr_t) o;
}

/* This should be moved somewhere else, but where is the questions */
const char* chpl_get_argument_i(chpl_main_argument* args, int32_t i)
{
  if( i < 0 ) return NULL;
  if( i > args->argc ) return NULL;
  return args->argv[i];
}

