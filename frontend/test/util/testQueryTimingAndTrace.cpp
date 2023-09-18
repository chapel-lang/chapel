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
#include "chpl/util/filesystem.h"

#include <cstdlib>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;

static const char* outPath = "/tmp/chpl-queries-trace.txt";
// relative to $CHPL_HOME
static const char* relativeCommandPath = "frontend/util/analyze-query-trace";

int main(int argc, char** argv) {
  // even if invoked by Dyno tests, ensure the command we would run for this
  // manual test exists
  std::string commandPath;
  commandPath += getenv("CHPL_HOME");
  commandPath += "/";
  commandPath += relativeCommandPath;
  if (!fileExists(commandPath.c_str())) {
    printf("Could not find analyze query trace command at path %s\n",
           commandPath.c_str());
    return 1;
  }

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

  std::string command;
  command += commandPath;
  command += " ";
  command += outPath;
  command += " --dot-png /tmp/chpl-queries.png";
  command += " --collapse";
  command += " --collapse-regex 'get.*Type'";

  printf("Running %s\n", command.c_str());
  int rc = system(command.c_str());
  if (rc != 0) {
    printf("Non-zero exit code %i from running %s\n", rc, command.c_str());
    // observed that exit code may be out of valid range, so use 1 as a
    // safe erroneous exit code value to avoid system turning it into 0
    return 1;
  }

  return 0;
}
