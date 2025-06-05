/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
static inline void chpl_gpu_printf0(const char *fmt) {
  printf("%s", fmt);
}
static inline void chpl_gpu_printf1(const char *fmt,
 void *x1)
{
  printf(fmt, x1);
}
static inline void chpl_gpu_printf2(const char *fmt,
  void *x1, void *x2)
{
  printf(fmt, x1, x2);
}
static inline void chpl_gpu_printf3(const char *fmt,
  void *x1, void *x2, void *x3)
{
  printf(fmt, x1, x2, x3);
}
static inline void chpl_gpu_printf4(const char *fmt,
  void *x1, void *x2, void *x3, void *x4)
{
  printf(fmt, x1, x2, x3, x4);
}
static inline void chpl_gpu_printf5(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5)
{
  printf(fmt, x1, x2, x3, x4, x5);
}
static inline void chpl_gpu_printf6(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5, void *x6)
{
  printf(fmt, x1, x2, x3, x4, x5, x6);
}
static inline void chpl_gpu_printf7(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5, void *x6, void *x7)
{
  printf(fmt, x1, x2, x3, x4, x5, x6, x7);
}
static inline void chpl_gpu_printf8(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5, void *x6, void *x7,
  void *x8)
{
  printf(fmt, x1, x2, x3, x4, x5, x6, x7, x8);
}

static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) {
  if (!chpl_gpu_no_cpu_mode_warning) {
    chpl_warning("assertOnGpu() is ignored with CHPL_GPU=cpu", lineno, filenameIdx);
  }
}

static inline unsigned int chpl_gpu_clock(int32_t lineno, int32_t filenameIdx) {
  if (!chpl_gpu_no_cpu_mode_warning) {
    chpl_warning("chpl_gpu_clock was called.", lineno, filenameIdx);
  }
  return (unsigned int)clock();
}

static inline void chpl_gpu_printTimeDelta(
  const char *msg, unsigned int start, unsigned int end)
{
  printf("%s%u\n", msg, end - start);
}

static inline void chpl_gpu_force_sync(void) {
  if (!chpl_gpu_no_cpu_mode_warning) {
    chpl_warning("chpl_gpu_force_sync was called", 0, 0);
  }
}

static inline void chpl_gpu_force_warp_sync(unsigned mask) {
  if (!chpl_gpu_no_cpu_mode_warning) {
    chpl_warning("chpl_gpu_force_warp_sync was called", 0, 0);
  }
}


#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_INCLUDES_H
