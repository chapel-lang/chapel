/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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


#ifdef HAS_GPU_LOCALE

#ifndef cuda_utils_h
#define cuda_utils_h

#include <cuda.h>
#include <cuda_runtime.h>

static void chpl_gpu_cuda_check(int err, const char* file, int line) {
  if(err != CUDA_SUCCESS) {
    const int msg_len = 256;
    char msg[msg_len];

    snprintf(msg, msg_len,
             "%s:%d: Error calling CUDA function: %s (Code: %d)",
             file, line, cudaGetErrorString(err), err);

    chpl_internal_error(msg);
  }
}

#define CUDA_CALL(call) do {\
  chpl_gpu_cuda_check((int)call, __FILE__, __LINE__);\
} while(0);

#endif

#endif
