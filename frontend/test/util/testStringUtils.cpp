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

#include "test-common.h"

#include "chpl/util/string-utils.h"

using namespace chpl;

int main(int argc, char** argv) {
  assert(startsWith(std::string("abcde"),  std::string("")));
  assert(startsWith(std::string("abcde"),  std::string("a")));
  assert(startsWith(std::string("abcde"),  std::string("ab")));
  assert(startsWith(std::string("abcde"),  std::string("abc")));
  assert(startsWith(std::string("abcde"),  std::string("abcd")));
  assert(startsWith(std::string("abcde"),  std::string("abcde")));
  assert(!startsWith(std::string("abcde"), std::string("abcdef")));
  assert(!startsWith(std::string("abcde"), std::string("bcde")));
  assert(!startsWith(std::string("abcde"), std::string("cde")));
  assert(!startsWith(std::string("abcde"), std::string("de")));
  assert(!startsWith(std::string("abcde"), std::string("e")));

  assert(startsWith("abcde",  ""));
  assert(startsWith("abcde",  "a"));
  assert(startsWith("abcde",  "ab"));
  assert(startsWith("abcde",  "abc"));
  assert(startsWith("abcde",  "abcd"));
  assert(startsWith("abcde",  "abcde"));
  assert(!startsWith("abcde", "abcdef"));
  assert(!startsWith("abcde", "bcde"));
  assert(!startsWith("abcde", "cde"));
  assert(!startsWith("abcde", "de"));
  assert(!startsWith("abcde", "e"));

  assert(replacePrefix("abcde", "", "x") == "xabcde");
  assert(replacePrefix("abcde", "abcdef", "x") == "abcde");
  assert(replacePrefix("abcde", "abcde", "x") == "x");
  assert(replacePrefix("abcdef", "abcde", "x") == "xf");

  return 0;
}
