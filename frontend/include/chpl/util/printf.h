/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_PRINTF_H
#define CHPL_UTIL_PRINTF_H

#include <string>
#include <cstdarg>

// docs generator has trouble with the attribute applied to 'build'
// so the above ifndef works around the issue.
#ifndef DOXYGEN
#define DYNO_ATTR_PRINTF_FORMAT(fmt__, rest__)\
    __attribute__ ((format (printf, fmt__, rest__)))
#else
#define DYNO_ATTR_PRINTF_FORMAT(fmt__, rest__)
#endif

namespace chpl {

std::string vprintToString(const char* format, ...);
std::string vprintToString(const char* format, va_list vl);

} // end namespace chpl

#endif
