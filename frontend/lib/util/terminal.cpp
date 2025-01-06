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


#include "chpl/util/terminal.h"
#include <cstring>
#include <map>
#include <unistd.h>

namespace chpl {

std::string getFgColorCode(TermColorName colorName);

std::map<TermColorName, std::string> termColorMap = {
    {CLEAR,          "0"},
    {BLACK,          "30"},
    {RED,            "31"},
    {GREEN,          "32"},
    {YELLOW,         "33"},
    {BLUE,           "34"},
    {MAGENTA,        "35"},
    {CYAN,           "36"},
    {WHITE,          "37"},
    {BRIGHT_BLACK,   "90"},
    {BRIGHT_RED,     "91"},
    {BRIGHT_GREEN,   "92"},
    {BRIGHT_YELLOW,  "93"},
    {BRIGHT_BLUE,    "94"},
    {BRIGHT_MAGENTA, "95"},
    {BRIGHT_CYAN,    "96"},
    {BRIGHT_WHITE, "97"}
};

std::string getColorFormat(TermColorName colorName) {
  std::string ret = "\033[";
  ret += getFgColorCode(colorName);
  ret += "m";
  return ret.c_str();
}

std::string getClearColorFormat() {
  std::string ret = getColorFormat(CLEAR);
  return ret;
}

std::string getFgColorCode(TermColorName colorName) {
  return termColorMap[colorName];
}

// This approach and these terminals are taken from googletest; see
// https://github.com/google/googletest/blob/dfa67352364e00452360e1fa5d59677459712ee7/googletest/src/gtest.cc#L3237
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

  // Check if either output is to a tty.
  if (isatty(fileno(stderr)) == 0 ||
      isatty(fileno(stdout)) == 0) {
    isColorTerm = false;
  }

  return isColorTerm;
}


} //end namespace chpl
