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

#include "test-common.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"

#include <cstdlib>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;

static const char* outPath = "/tmp/chpl-queries-trace.txt";

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("Usage: %s file.chpl otherFile.chpl ...\n", argv[0]);
    return 0; // need this to return 0 for testing to be happy
  }

  Context context;
  Context* ctx = &context;

  ctx->setQueryTimingFlag(true);
  ctx->beginQueryTimingTrace(outPath);

  for (int i = 1; i < argc; i++) {
    auto filepath = UniqueString::get(ctx, argv[i]);
    const ModuleVec& mods = parseToplevel(ctx, filepath);
    for (const Module* mod : mods) {
      const ResolutionResultByPostorderID& rr = resolveModule(ctx, mod->id());
      (void)rr;
    }
  }

  ctx->endQueryTimingTrace();
  ctx->queryTimingReport(std::cout);

  printf("Wrote trace to %s\n", outPath);

  std::string command = "$CHPL_HOME/compiler/dyno/util/analyze-query-trace ";
  command += outPath;
  command += " --dot-png /tmp/chpl-queries.png";
  command += " --collapse";
  command += " --collapse-regex 'get.*Type'";

  printf("Running %s\n", command.c_str());
  int rc = system(command.c_str());
  if (rc != 0) {
    printf("Non-zero exit code from running %s\n", command.c_str());
  }

  return 0;
}
