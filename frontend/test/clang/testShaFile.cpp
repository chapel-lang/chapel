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

// TODO: move this file to the util tests
#include "chpl/framework/Context.h"
#include "chpl/util/filesystem.h"

#include <cstring>

using namespace chpl;

int main(int argc, char** argv) {

  for (int i = 1; i < argc; i++) {
    printf("hashing %s\n", argv[i]);
    char* fname = argv[i];

    llvm::ErrorOr<HashFileResult> hashOrErr = hashFile(fname);
    if (hashOrErr) {
      printf("%s\n", fileHashToHex(hashOrErr.get()).c_str());
    } else {
      printf("error\n");
    }
  }

  return 0;
}
