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

#include "chplrt.h"

#include "chpl-env.h"
#include "chpltypes.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>


const char* chpl_get_rt_env(const char* evs, const char* dflt) {
  char evName[100];
  const char* evVal;

  if (snprintf(evName, sizeof(evName), "CHPL_RT_%s", evs) >= sizeof(evName))
    chpl_internal_error("environment variable name buffer too small");

  evVal = getenv(evName);
  if (evVal == NULL)
    return dflt;
  return evVal;
}


chpl_bool chpl_get_rt_env_bool(const char* evs, chpl_bool dflt) {
  const char* evVal = chpl_get_rt_env(evs, NULL);

  if (evVal == NULL)
    return dflt;
  if (strchr("0fFnN", evVal[0]) != NULL)
    return false;
  if (strchr("1tTyY", evVal[0]) != NULL)
    return true;

  chpl_msg(1,
           "warning: unknown CHPL_RT_%s value; should be T or F, assuming %c\n",
           evs, (dflt ? 'T' : 'F'));
  return dflt;
}
