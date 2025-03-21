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

#include "chpl/util/filtering.h"
#include <regex>
#include <iostream>

namespace chpl {

namespace {
  std::string buildSphinxMarkupRegexp() {
    // See documentation on inline Sphinx markup here:
    // https://chapel-lang.org/docs/tools/chpldoc/chpldoc.html#inline-markup-2
    // There are some details involved in this, but the main idea is to match
    // strings of the form:
    // 
    //    :role:`content`
    // 
    // We write the regexp so the relevant part of "content" will be captured by
    // std::smatch if we wish to use this to filter out the markup.

    // There are four "kinds" of content we consider, so we OR these together
    // Capture a normal Chapel identifier
    std::string reCntType1 = R"#(([\w$.]+))#";
    // If it starts with ~, capture last identifier to the right of a '.'
    // Note in a repeated capture group (e.g. `(\w\.?)+` only the last iteration is captured
    std::string reCntType2 = R"#(~([\w$]+\.?)+)#";
    // Starts with !, capture identifier to right of ! without capturing ! itself
    std::string reCntType3 = R"#(!([\w$.]+))#";
    // support explicit title and reference target, captures the target
    // note that Sphinx allows any character for the nice name of the title
    // but does not allow a space directly before the '<'
    std::string reCntType4 = R"#((?:[^<]+[^\s])<([\w$.]+)>)#";

    // OR all the content types together, wrapping each in a non capturing group
    std::string reContent = "(?:(?:" + reCntType1 + ")|(?:" + reCntType2 + ")|(?:" + reCntType3 + ")|(?:" + reCntType4 +  + "))";

    // Various roles; put into a (?...) group so we don't "capture" it
    std::string reRole = R"#((?:mod|proc|iter|data|const|var|param|type|class|record|attr|enum|enumconstant))#";

    // Regexp to match :role:`content`; note \B is used on either end to indicate markup is separated by word boundaries
    std::string reSphinxMrkp = R"#(\B\:)#" + reRole + R"#(\:`)#" + reContent + R"#(`\B)#";

    return reSphinxMrkp;
  }
}

std::string removeSphinxMarkup(const std::string& msg) {
  static const auto reStr = buildSphinxMarkupRegexp();

  // for a given match there are multiple groups. Since they are all ORed
  // together only one is actually defined. The others exist they just aren't
  // valid matches. This is what requires the inner for loop, as opposed to a 
  // regex_replace(). The outer loop is required because 'msg' may have more
  // than one string it needs to match

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
