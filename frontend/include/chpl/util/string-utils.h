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

#ifndef CHPL_UTIL_STRING_UTILS_H
#define CHPL_UTIL_STRING_UTILS_H

#include <string>

namespace chpl
{


// once we are on C++20, we can just use string::starts_with
/** Returns true if the string 's' starts with 'prefix'. */
bool startsWith(const std::string& s, const std::string& prefix);

/** Returns true if the string 's' starts with 'prefix'. */
bool startsWith(const char* s, const char* prefix);

/**
  If the string 's' starts with 'prefix', return a string where
  'prefix' is replaced by 'replacement'. Otherwise, return 's' unmodified.
 */
std::string replacePrefix(const std::string& s,
                          const std::string& prefix,
                          const std::string& replacement);


} // namespace chpl


#endif // CHPL_UTIL_STRING_UTILS_H
