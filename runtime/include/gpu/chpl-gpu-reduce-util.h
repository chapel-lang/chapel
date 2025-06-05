/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#define GPU_DEV_CUB_WRAP(MACRO, impl_kind, chpl_kind) \
  MACRO(impl_kind, chpl_kind, chpl_bool)  \
  MACRO(impl_kind, chpl_kind, int8_t)  \
  MACRO(impl_kind, chpl_kind, int16_t)  \
  MACRO(impl_kind, chpl_kind, int32_t)  \
  MACRO(impl_kind, chpl_kind, int64_t)  \
  MACRO(impl_kind, chpl_kind, uint8_t)  \
  MACRO(impl_kind, chpl_kind, uint16_t)  \
  MACRO(impl_kind, chpl_kind, uint32_t)  \
  MACRO(impl_kind, chpl_kind, uint64_t)  \
  MACRO(impl_kind, chpl_kind, _real32)   \
  MACRO(impl_kind, chpl_kind, _real64);

#define GPU_CUB_WRAP(MACRO, chpl_kind) \
  MACRO(chpl_kind, chpl_bool)  \
  MACRO(chpl_kind, int8_t)  \
  MACRO(chpl_kind, int16_t)  \
  MACRO(chpl_kind, int32_t)  \
  MACRO(chpl_kind, int64_t)  \
  MACRO(chpl_kind, uint8_t)  \
  MACRO(chpl_kind, uint16_t)  \
  MACRO(chpl_kind, uint32_t)  \
  MACRO(chpl_kind, uint64_t)  \
  MACRO(chpl_kind, _real32)   \
  MACRO(chpl_kind, _real64);

#endif // HAS_GPU_LOCALE

