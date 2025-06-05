/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/libraries/LibraryFile.h"
#include "chpl/libraries/LibraryFileWriter.h"
#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/Module.h"
#include "chpl/util/filesystem.h"

using namespace chpl;
using namespace uast;
using namespace libraries;

static void usage(int argc, char** argv) {
  printf("%s libfile.dyno\n", argv[0]);
  exit(0);
}

int main(int argc, char** argv) {
  Context ctx;
  Context* context = &ctx;
  const char* input = "input.dyno";

  if (argc == 1) usage(argc, argv);
  else if (argc > 1) input = argv[1];

  UniqueString libPath = UniqueString::get(context, input);
  const LibraryFile* lib = LibraryFile::load(context, libPath);

  lib->summarize(context, std::cout);

  return 0;
}
