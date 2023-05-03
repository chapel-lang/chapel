/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef _CHPL_GPU_GEN_INCLUDES_H
#define _CHPL_GPU_GEN_INCLUDES_H

#ifdef HAS_GPU_LOCALE

#include "chpltypes.h"
#include "chpl-comm.h"

static inline void chpl_gpu_write(const char *str) { printf("%s", str); }

static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) {
  chpl_warning("assertOnGpu() is ignored with CHPL_GPU=cpu", lineno, filenameIdx);
}

static inline unsigned int chpl_gpu_clock(void) {
  chpl_warning("chpl_gpu_clock was called.", 0, 0);
  return (unsigned int)clock();
}

static inline void chpl_gpu_printTimeDelta(
  const char *msg, unsigned int start, unsigned int end)
{
  printf("%s%u\n", msg, end - start);
}

static inline void chpl_gpu_force_sync() {
  chpl_warning("chpl_gpu_force_sync was called", 0, 0);
}

#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_INCLUDES_H
