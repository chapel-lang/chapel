/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/util/string-utils.h"

#include <cstring>

namespace chpl {


bool startsWith(const std::string& s, const std::string& prefix) {
  return (0 == s.compare(0, prefix.length(), prefix));
}

bool startsWith(const char* s, const char* prefix) {
  return (0 == strncmp(s, prefix, strlen(prefix)));
}

std::string replacePrefix(const std::string& s,
                          const std::string& prefix,
                          const std::string& replacement) {
  if (startsWith(s, prefix)) {
    std::string ret = replacement;
    ret.append(s, prefix.length(), s.length() - prefix.length());
    return ret;
  } else {
    return s;
  }
}


} // end namespace chpl
