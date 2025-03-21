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

#ifndef CHPL_UTIL_STRING_ESCAPES_H
#define CHPL_UTIL_STRING_ESCAPES_H

#include <string>

namespace chpl {


/** checks for interior null bytes in the string */
bool stringContainsZeroBytes(const char* s, size_t len);

/** add \ escapes according to C quoting rules */
std::string escapeStringC(const std::string& unescaped);

/** add \ escapes according to C quoting rules */
std::string escapeStringC(const char* unescaped);

/** Unescape characters according to the C quoting rules.  */
std::string unescapeStringC(const std::string& str);

/** Unescape characters according to the C quoting rules.  */
std::string unescapeStringC(const char* str);

/** add \ escapes for storing something as a symbolPath in an ID */
std::string escapeStringId(const std::string& unescaped);

/** add \ escapes for storing something as a symbolPath in an ID */
std::string escapeStringId(const char* unescaped);

/** remove \ escapes from something stored as a symbolPath in an ID */
std::string unescapeStringId(const std::string& str);

/** remove \ escapes from something stored as a symbolPath in an ID */
std::string unescapeStringId(const char* str);


} // end namespace chpl

#endif
