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

#include "chpl/framework/Context.h"
#include "chpl/util/clang-integration.h"

#include <cstring>

using namespace chpl;

int main(int argc, char** argv) {
  std::string chpl_home;
  std::vector<std::string> clangArgs;

  clangArgs.push_back("clang");

  for (int i = 1; i < argc; i++) {
    clangArgs.push_back(argv[i]);
  }

  if (clangArgs.empty()) {
    clangArgs.push_back("test.c");
  }

  if (const char* chpl_home_env  = getenv("CHPL_HOME")) {
    chpl_home = chpl_home_env;
    printf("CHPL_HOME was set\n");
  } else {
    printf("CHPL_HOME was not set\n");
  }

  Context::Configuration config;
  config.chplHome = chpl_home;
  Context context(config);
  Context* ctx = &context;

  printf("Computing CC1 args\n");
  const std::vector<std::string>& cc1args = chpl::util::getCC1Arguments(ctx,
                                                            clangArgs,
                                                            false);
  for (auto& arg : cc1args) {
    printf(" %s\n", arg.c_str());
  }

  return 0;
}
