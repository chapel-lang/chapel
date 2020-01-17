/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include <limits>

#ifndef INT8_MIN
#define INT8_MIN (std::numeric_limits<int8_t>::min())
#endif
#ifndef INT16_MIN
#define INT16_MIN (std::numeric_limits<int16_t>::min())
#endif
#ifndef INT32_MIN
#define INT32_MIN (std::numeric_limits<int32_t>::min())
#endif
#ifndef INT64_MIN
#define INT64_MIN (std::numeric_limits<int64_t>::min())
#endif

#ifndef INT8_MAX
#define INT8_MAX (std::numeric_limits<int8_t>::max())
#endif
#ifndef INT16_MAX
#define INT16_MAX (std::numeric_limits<int16_t>::max())
#endif
#ifndef INT32_MAX
#define INT32_MAX (std::numeric_limits<int32_t>::max())
#endif
#ifndef INT64_MAX
#define INT64_MAX (std::numeric_limits<int64_t>::max())
#endif

#ifndef UINT8_MAX
#define UINT8_MAX (std::numeric_limits<uint8_t>::max())
#endif
#ifndef UINT16_MAX
#define UINT16_MAX (std::numeric_limits<uint16_t>::max())
#endif
#ifndef UINT32_MAX
#define UINT32_MAX (std::numeric_limits<uint32_t>::max())
#endif
#ifndef UINT64_MAX
#define UINT64_MAX (std::numeric_limits<uint64_t>::max())
#endif


