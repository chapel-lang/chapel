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

#include "chpl-env.h"
#include "chpltypes.h"
#include "error.h"

#ifdef LAUNCHER
#include "chpllaunch.h"
#endif

#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define GRIPE(name, kind, val, kindFmt, dflt)                           \
  do {                                                                  \
    if (name == NULL) {                                                 \
      chpl_msg(1,                                                       \
               "warning: env var improper %s value \"%s\", "            \
               "assuming %" kindFmt "\n",                               \
               val, kind, dflt);                                        \
    } else {                                                            \
      chpl_msg(1,                                                       \
               "warning: CHPL_RT_%s improper %s value \"%s\", "         \
               "assuming %" kindFmt "\n",                               \
               name, kind, val, dflt);                                  \
    }                                                                   \
  } while (0)


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

  GRIPE(evName, "bool", evVal, "c", (dflt ? 'T' : 'F'));

  return dflt;
}


int64_t chpl_env_str_to_int(const char* evName, const char* evVal,
                            int64_t dflt) {
  int64_t val;

  if (evVal == NULL)
    return dflt;

  if (sscanf(evVal, "%" SCNi64, &val) == 1)
    return val;

  GRIPE(evName, "int", evVal, PRId64, dflt);

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
    GRIPE(evName, "int percentage", evVal, "d", dflt);
  }

  return dflt;
}


uint64_t chpl_env_str_to_uint(const char* evName, const char* evVal,
                              uint64_t dflt) {
  int64_t val;

  if (evVal == NULL)
    return dflt;

  if (isdigit(evVal[0])
      && sscanf(evVal, "%" SCNu64, &val) == 1)
    return val;

  GRIPE(evName, "unsigned int", evVal, PRIu64, dflt);

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
    GRIPE(evName, "size", evVal, "zd", dflt);
    return dflt;
  }

  return val;
}


void chpl_env_set(const char* evName, const char* evVal, int overwrite) {
#ifdef LAUNCHER
  if (overwrite || getenv(evName) == NULL) {
    chpl_launcher_record_env_var(evName, evVal);
  }
#endif
  if (setenv(evName, evVal, overwrite) != 0) {
    char buf[200];
    snprintf(buf, sizeof(buf), "cannot setenv %s=\"%s\"", evName, evVal);
    chpl_error(buf, 0, 0);
  }
}


void chpl_env_set_uint(const char* evName, uint64_t evVal, int overwrite) {
  char buf[21]; // big enough for 64-bit unsigned, plus trailing '\0'
  snprintf(buf, sizeof(buf), "%" PRIu64, evVal);
  chpl_env_set(evName, buf, overwrite);
}
