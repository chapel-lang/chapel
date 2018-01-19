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

#include "chplrt.h"

#include "chpl-env.h"
#include "chpltypes.h"
#include "error.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


const char* chpl_env_rt_get(const char* evs, const char* dflt) {
  char evName[100];
  const char* evVal;

  if (snprintf(evName, sizeof(evName), "CHPL_RT_%s", evs) >= sizeof(evName))
    chpl_internal_error("environment variable name buffer too small");

  evVal = getenv(evName);
  if (evVal == NULL)
    return dflt;
  return evVal;
}


chpl_bool chpl_env_str_to_bool(const char* evVal, chpl_bool dflt) {
  if (evVal == NULL)
    return dflt;
  if (strchr("0fFnN", evVal[0]) != NULL)
    return false;
  if (strchr("1tTyY", evVal[0]) != NULL)
    return true;

  chpl_msg(1,
           "warning: env var unknown bool value \"%s\", assuming %c\n",
           evVal, (dflt ? 'T' : 'F'));
  return dflt;
}


int64_t chpl_env_str_to_int(const char* evVal, int64_t dflt) {
  int64_t val;

  if (evVal == NULL)
    return dflt;

  if (sscanf(evVal, "%" SCNi64, &val) == 1)
    return val;

  chpl_msg(1,
           "warning: unknown env var int value \"%s\", assuming %" PRId64 "\n",
           evVal, dflt);
  return dflt;
}


size_t chpl_env_str_to_size(const char* evVal, size_t dflt) {
  size_t val;
  int scnCnt;
  char units;

  if (evVal == NULL)
    return dflt;

  if ((scnCnt = sscanf(evVal, "%zi%c", &val, &units)) != 1) {
    if (scnCnt == 2 && strchr("kKmMgG", units) != NULL) {
      switch (units) {
      case 'k' : case 'K': val <<= 10; break;
      case 'm' : case 'M': val <<= 20; break;
      case 'g' : case 'G': val <<= 30; break;
      }
    } else {
      chpl_msg(1,
               "warning: unknown env var size value \"%s\", assuming %zd\n",
               evVal, dflt);
      return dflt;
    }
  }

  return val;
}
