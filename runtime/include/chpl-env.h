/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _CHPL_ENV_H_
#define _CHPL_ENV_H_

#include "chpltypes.h"

#include <inttypes.h>

//
// Returns the string value of a CHPL_RT_* environment variable, with
// a default.
//
const char* chpl_env_rt_get(const char*, const char*);

//
// These convert string values, presumably from environment variables,
// to typed values (boolean, int, or size), with a default.
//
chpl_bool chpl_env_str_to_bool(const char*, chpl_bool);
int64_t chpl_env_str_to_int(const char*, int64_t);
size_t chpl_env_str_to_size(const char*, size_t);

//
// These combine getting the environment variable and converting to a
// typed value.
//
static inline
chpl_bool chpl_env_rt_get_bool(const char* ev, chpl_bool dflt) {
  return chpl_env_str_to_bool(chpl_env_rt_get(ev, NULL), dflt);
}

static inline
int64_t chpl_env_rt_get_int(const char* ev, int64_t dflt) {
  return chpl_env_str_to_int(chpl_env_rt_get(ev, NULL), dflt);
}

static inline
size_t chpl_env_rt_get_size(const char* ev, size_t dflt) {
  return chpl_env_str_to_size(chpl_env_rt_get(ev, NULL), dflt);
}

#endif
