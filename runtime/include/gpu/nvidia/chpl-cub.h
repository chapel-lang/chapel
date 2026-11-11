/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#ifndef _CHPL_CUB_H
#define _CHPL_CUB_H

// CCCL 3.0-3.3 has __host__-only deduction guides clang rejects (NVIDIA/cccl#7896)
#if defined(__clang__) && __has_include(<cuda/std/__cccl/version.h>)
#include <cuda/std/__cccl/version.h>
#if CCCL_VERSION >= 3000000 && CCCL_VERSION < 3004000
#include <cuda/std/__cccl/execution_space.h>
#pragma push_macro("_CCCL_HOST")
#undef _CCCL_HOST
#define _CCCL_HOST
#include <cuda/std/string_view>
#pragma pop_macro("_CCCL_HOST")
#endif
#endif

#include <cub/cub.cuh>

#endif
