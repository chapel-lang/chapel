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

#include "chpl/framework/Context.h"
#include "chpl/util/filesystem.h"

#include <cstring>
#include <cstdio>

using namespace chpl;

static void checkHash(const char* name,
                      std::string data,
                      std::string expectHex) {


  printf("hash test %s\n", name);

  FILE* f = fopen(name, "w");
  assert(f);

  if (!f) return;

  if (f) {
    size_t wrote = fwrite(&data[0], 1, data.size(), f);
    assert(wrote == data.size());
    fclose(f);
  }

  llvm::ErrorOr<HashFileResult> hashOrErr = hashFile(name);
  
  // delete the file since it is no longer needed
  remove(name);

  std::string hashHex;
  if (hashOrErr) {
    hashHex = fileHashToHex(hashOrErr.get());
    printf("file hash   %s\n", hashHex.c_str());
    assert(hashHex == expectHex);
  } else {
    printf("error computing file hash\n");
    assert(false);
    return;
  }

  // check also hashing it with hashString
  HashFileResult hash2 = hashString(data);
  std::string hashHex2 = fileHashToHex(hash2);
  printf("string hash %s\n", hashHex2.c_str());
  assert(hashHex2 == expectHex);
  assert(hashHex2 == hashHex);
}

static void testHashes() {
#if LLVM_VERSION_MAJOR >= 13
  // sha-256
  checkHash("hello.data", "hello",
            "2CF24DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824");
  checkHash("ones.data", std::string(1024*1024*32, '1'),
            "34511EDC22A4D5749A71FF3D4A47AAC494214D65EE88EC69FAC46C9ABDF5CE0D");
#else
  // sha-1 since sha-256 not available in LLVM < 13
  checkHash("hello.data", "hello",
            "AAF4C61DDCC5E8A2DABEDE0F3B482CD9AEA9434D");
  checkHash("ones.data", std::string(1024*1024*32, '1'),
            "CBBA74D49460868EA01AC8825FF1657205C22476");
#endif
}

int main(int argc, char** argv) {

  if (argc == 1) {
    testHashes();
  } else {
    for (int i = 1; i < argc; i++) {
      printf("hashing %s\n", argv[i]);
      char* fname = argv[i];

      llvm::ErrorOr<HashFileResult> hashOrErr = hashFile(fname);
      if (hashOrErr) {
        printf("file hash   %s\n", fileHashToHex(hashOrErr.get()).c_str());
      } else {
        printf("error\n");
      }

      // check also hashing it with hashString
      std::string fileData, error;
      bool ok = readFile(fname, fileData, error);
      if (ok) {
        HashFileResult hash2 = hashString(fileData);
        printf("string hash %s\n", fileHashToHex(hash2).c_str());
        if (hashOrErr) {
          assert(hash2 == hashOrErr.get());
        }
      }
    }
  }

  return 0;
}
