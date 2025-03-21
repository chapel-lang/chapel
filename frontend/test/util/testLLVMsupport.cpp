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

#include "llvm/Support/FileSystem.h"

// Filesystem create/delete works from LLVM support library.
static void test1() {
  std::string dirname = "test1dir";
  std::error_code err = llvm::sys::fs::create_directories(dirname);
  assert(!err);
  assert(llvm::sys::fs::is_directory(dirname));
  err = llvm::sys::fs::remove_directories(dirname);
  assert(!err);
}


int main(int argc, char** argv) {
  test1();
  return 0;
}
