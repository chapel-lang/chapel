/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/util/string-escapes.h"

#include <cassert>

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;

static void checkQuoting(const char* input, const char* expect) {
  std::string input_s = input;
  std::string expect_s = expect;

  std::string got = quoteStringForC(input_s);
  assert(got == expect_s);
}

int main(int argc, char** argv) {
 
  checkQuoting("\a", "\\a");
  checkQuoting("\"", "\\\"");
  checkQuoting("?", "\\?");
  checkQuoting("\x01", "\\x01");

  {
    char input[4] = {1, 'a', 'a', 0};
    checkQuoting(input, "\\x01\\x61\\x61");
  }

  checkQuoting("\\ \? \a \b \f \n \r \t \v",
               "\\\\ \\\? \\a \\b \\f \\n \\r \\t \\v");

  return 0;
}
