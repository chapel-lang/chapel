/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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


#include "chpl/util/terminal.h"
#include <cstring>
#include <map>


namespace chpl {

std::string getFgColorCode(TermColorName colorName);

std::map<TermColorName, std::string> termColorMap = {
    {clear, "0"},
    {black,"30"},
    {red,"31"},
    {green, "32"},
    {yellow, "33"},
    {blue,"34"},
    {magenta,"35"},
    {cyan, "36"},
    {white, "37"},
    {bright_black, "90"},
    {bright_red, "91"},
    {bright_green, "92"},
    {bright_yellow, "93"},
    {bright_blue, "94"},
    {bright_magenta, "95"},
    {bright_cyan, "96"},
    {bright_white, "97"}
};

std::string getTerminalColor(TermColorName colorName) {
  std::string ret = "\033[";
  ret += getFgColorCode(colorName);
  ret += "m";
  return ret.c_str();
}

std::string clearTerminalColor() {
  std::string ret = getTerminalColor(clear);
  return ret;
}

std::string getFgColorCode(TermColorName colorName) {
  return termColorMap[colorName];
}

// This approach and these terminals are taken from googletest; see
// https://github.com/google/googletest/blob/master/googletest/src/gtest.cc#L3216
bool terminalSupportsColor(const char* term) {
  bool isColorTerm = false;
  const char* colorTerms[] = {"xterm",
                              "xterm-color",
                              "xterm-256color",
                              "screen",
                              "screen-256color",
                              "tmux",
                              "tmux-256color",
                              "rxvt-unicode",
                              "rxvt-unicode-256color",
                              "linux",
                              "cygwin",
                              NULL};
  if (term == NULL) term = "";
  for (int i = 0; colorTerms[i] != NULL; i++) {
    if (0 == strcmp(term, colorTerms[i])) {
      isColorTerm = true;
      break;
    }
  }
  return isColorTerm;
}


} //end namespace chpl
