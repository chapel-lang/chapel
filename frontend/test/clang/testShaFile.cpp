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

#include "llvm/Support/SHA1.h"
#include "llvm/Support/SHA256.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/VirtualFileSystem.h"

#include <cstring>

using namespace chpl;

/*static std::string toHex(llvm::ArrayRef<uint8_t> Input) {
  static const char *const LUT = "0123456789abcdef";
  size_t Length = Input.size();

  std::string Output;
  Output.reserve(2 * Length);
  for (size_t i = 0; i < Length; ++i) {
    const unsigned char c = Input[i];
    Output.push_back(LUT[c >> 4]);
    Output.push_back(LUT[c & 15]);
  }
  return Output;
}*/


static void TestSHA256Sum(llvm::StringRef Input) {
  llvm::SHA256 Hash;
  //llvm::SHA1 Hash;
  Hash.update(Input);
  auto hash = Hash.final();
  auto hashStr = toHex(hash);
  printf("hash was %s\n", hashStr.c_str());
}

int main(int argc, char** argv) {
  std::string chpl_home;

  auto fs = llvm::vfs::createPhysicalFileSystem();

  for (int i = 1; i < argc; i++) {
    printf("hashing %s\n", argv[i]);
    char* fname = argv[i];

    auto maybeBuf = fs->getBufferForFile(fname);
    if (maybeBuf) {
      llvm::MemoryBuffer* buf = maybeBuf->get();
      TestSHA256Sum(buf->getBuffer());
      //TestSHA256Sum(buf->getBuffer());
    }
  }

  return 0;
}
