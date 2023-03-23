/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/util/filtering.h"
#include <regex>

namespace chpl {

std::string removeSphinxMarkup(const std::string& msg) {
  // TODO: Support explicit title and reference targets like in reST direct hyperlinks (and having only target
  //       show up in sanitized message).
  static const auto reStr = R"#(\B\:(?:mod|proc|iter|data|const|var|param|type|class|record|attr|enum)\:`(?:([$\w\$\.]+)|(?:~([$\w\$]+\.?)+)|(?:!([$\w\$\.]+)))`\B)#";
  std::string filteredMsg = msg;
  std::smatch match;
  while(std::regex_search(filteredMsg, match, std::regex(reStr))) {
    for(size_t i = 1; i < match.size(); i++) {
      if(match[i].matched) {
        filteredMsg = match.prefix().str() + match[i].str() + match.suffix().str();
        break;
      }
    }
  }
  return filteredMsg;
}

} // namespace chpl
