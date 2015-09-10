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

#ifndef _CHPL_ENV_H_
#define _CHPL_ENV_H_

#include "chpltypes.h"

//
// Returns the value of a CHPL_RT_* environment variable, with default.
//
const char* chpl_get_rt_env(const char*, const char*);

//
// Returns the value of a boolean CHPL_RT_* environment variable, with
// default.
//
chpl_bool chpl_get_rt_env_bool(const char*, chpl_bool);

#endif
