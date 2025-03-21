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

#include "test-common.h"

#include "chpl/util/string-escapes.h"

using namespace chpl;

static void checkQuotingC(const char* input, const char* expect) {
  std::string input_s = input;
  std::string expect_s = expect;

  std::string quoted = escapeStringC(input_s);
  assert(quoted == expect_s);

  std::string quoted2 = escapeStringC(input);
  assert(quoted2 == expect_s);

  std::string unquoted = unescapeStringC(quoted.c_str());
  assert(unquoted == input_s);
}

static void checkQuotingId(const char* input, const char* expect) {
  std::string input_s = input;
  std::string expect_s = expect;

  std::string quoted = escapeStringId(input_s);
  assert(quoted == expect_s);

  std::string quoted2 = escapeStringId(input);
  assert(quoted2 == expect_s);

  std::string unquoted = unescapeStringId(quoted.c_str());
  assert(unquoted == input_s);
}

int main(int argc, char** argv) {

  checkQuotingC("a.b#3", "a.b#3");
  checkQuotingC("\a", "\\a");
  checkQuotingC("\"", "\\\"");
  checkQuotingC("?", "\\?");
  checkQuotingC("\x01", "\\x01");

  {
    char input[4] = {1, 'a', 'a', 0};
    checkQuotingC(input, "\\x01\\x61\\x61");
  }

  checkQuotingC("\\ \? \a \b \f \n \r \t \v",
                "\\\\ \\\? \\a \\b \\f \\n \\r \\t \\v");

  checkQuotingId("\a", "\\a");
  checkQuotingId("###", "\\#\\#\\#");
  checkQuotingId("...", "\\.\\.\\.");
  checkQuotingId("File.name", "File\\.name");

  return 0;
}
