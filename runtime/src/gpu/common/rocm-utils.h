/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.  *
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

#ifndef __HIP_PLATFORM_AMD__
#define __HIP_PLATFORM_AMD__
#endif
#include <hip/hip_common.h>
#include <hip/hip_runtime.h>

#if __has_include(<rocm-core/rocm_version.h>)  // 5.x wants this
#include <rocm-core/rocm_version.h>
#elif __has_include(<rocm/rocm_version.h>)  // 4.x wants this
#include <rocm/rocm_version.h>
#elif __has_include(<rocm_version.h>)  // Deprecated. 5.x used to want this
#include <rocm_version.h>
#elif !defined(ROCM_VERSION_MAJOR)
#define ROCM_VERSION_MAJOR 4 // this is the safe bet
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void chpl_internal_error(const char*);


static void chpl_gpu_rocm_check(int err, const char* file, int line) {
  if(err == hipErrorContextAlreadyInUse) { return; }
  if(err != hipSuccess) {
    const int msg_len = 256;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "%s:%d: Error calling HIP function: %s (Code: %d)",
             file, line, hipGetErrorString((hipError_t)err), err);

    chpl_internal_error(msg);
  }
}

#ifdef __cplusplus
}
#endif

#define ROCM_CALL(call) do {\
  chpl_gpu_rocm_check((int)call, __FILE__, __LINE__);\
} while(0);
