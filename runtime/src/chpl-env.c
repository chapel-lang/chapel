/*
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

#include "chplrt.h"

#include "chpl-env.h"
#include "chpltypes.h"
#include "error.h"

#include <ctype.h>
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


chpl_bool chpl_env_str_to_bool(const char* evName, const char* evVal,
                               chpl_bool dflt) {
  if (evVal == NULL)
    return dflt;
  if (strchr("0fFnN", evVal[0]) != NULL)
    return false;
  if (strchr("1tTyY", evVal[0]) != NULL)
    return true;

  if (evName == NULL) {
    chpl_msg(1,
             "warning: env var improper bool value \"%s\", assuming %c\n",
             evVal, (dflt ? 'T' : 'F'));
  } else {
    chpl_msg(1,
             "warning: CHPL_RT_%s improper bool value \"%s\", assuming %c\n",
             evName, evVal, (dflt ? 'T' : 'F'));
  }

  return dflt;
}


int64_t chpl_env_str_to_int(const char* evName, const char* evVal,
                            int64_t dflt) {
  int64_t val;

  if (evVal == NULL)
    return dflt;

  if (sscanf(evVal, "%" SCNi64, &val) == 1)
    return val;

  if (evName == NULL) {
    chpl_msg(1,
             "warning: env var improper int value \"%s\", assuming "
             "%" PRId64 "\n",
             evVal, dflt);
  } else {
    chpl_msg(1,
             "warning: CHPL_RT_%s improper int value \"%s\", assuming "
             "%" PRId64 "\n",
             evName, evVal, dflt);
  }

  return dflt;
}


int chpl_env_str_to_int_pct(const char* evName, const char* evVal,
                            int dflt, chpl_bool doWarn) {
  int val;
  int evIdx;

  if (evVal == NULL)
    return dflt;

  if (sscanf(evVal, "%d%n", &val, &evIdx) == 1
      && val > 0
      && evVal[evIdx] == '%') {
    return val;
  }

  if (doWarn) {
    if (evName == NULL) {
      chpl_msg(1,
               "warning: env var improper int percentage \"%s\", assuming "
               "%d\n",
               evVal, dflt);
    } else {
      chpl_msg(1,
               "warning: CHPL_RT_%s improper int percentage \"%s\", assuming "
               "%d\n",
               evName, evVal, dflt);
    }
  }

  return dflt;
}


size_t chpl_env_str_to_size(const char* evName, const char* evVal,
                            size_t dflt) {
  chpl_bool okay;
  int scnCnt;
  size_t val;
  char units;

  if (evVal == NULL)
    return dflt;

  //
  // Try to collect a strictly unsigned decimal, octal, or hexadecimal
  // number, with an optional following units character.
  //
  okay = false;
  if (isdigit(evVal[0])
      && (scnCnt = sscanf(evVal, "%zi%c", &val, &units)) > 0) {
    okay = true;
    if (scnCnt == 2 && strchr("kKmMgG", units) != NULL) {
      switch (units) {
      case 'k' : case 'K': val <<= 10; break;
      case 'm' : case 'M': val <<= 20; break;
      case 'g' : case 'G': val <<= 30; break;
      }
    }
  }

  if (!okay) {
    if (evName == NULL) {
      chpl_msg(1,
               "warning: env var improper size value \"%s\", assuming %zd\n",
               evVal, dflt);
    } else {
      chpl_msg(1,
               "warning: CHPL_RT_%s improper size value \"%s\", assuming "
               "%zd\n",
               evName, evVal, dflt);
    }

    return dflt;
  }

  return val;
}
