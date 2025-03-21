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


static void testCleanLocalPath() {
  assert(cleanLocalPath("foo.chpl") == "foo.chpl");
  assert(cleanLocalPath("./foo.chpl") == "foo.chpl");
  assert(cleanLocalPath("/foo.chpl") == "/foo.chpl");
  assert(cleanLocalPath("../foo.chpl") == "../foo.chpl");
  assert(cleanLocalPath(".") == ".");
  assert(cleanLocalPath("/") == "/");
  assert(cleanLocalPath("") == "");
}

static void testPathInDirPath() {
  assert(filePathInDirPath("foo.chpl", ".") == true);
  assert(filePathInDirPath("foo.chpl", "") == false); // documented
  assert(filePathInDirPath("./foo.chpl", ".") == true);
  assert(filePathInDirPath("./foo.chpl", "./") == true);
  assert(filePathInDirPath("foo.chpl", "/") == false);
  assert(filePathInDirPath("/foo.chpl", "/") == true);
  assert(filePathInDirPath("../foo.chpl", "/") == false);
  assert(filePathInDirPath("../foo.chpl", ".") == false);
  assert(filePathInDirPath("../foo.chpl", "./") == false);
  assert(filePathInDirPath("../foo.chpl", "..") == true);
  assert(filePathInDirPath("../foo.chpl", "../") == true);
  assert(filePathInDirPath("/dir/foo.chpl", "/dir") == true);
  assert(filePathInDirPath("/dir/foo.chpl", "/dir/") == true);
  assert(filePathInDirPath("/a/b/c/d.chpl", "/a") == true);
  assert(filePathInDirPath("/a/b/c/d.chpl", "/a/") == true);
  assert(filePathInDirPath("/a/b/c/d.chpl", "/a/b/c/") == true);
  assert(filePathInDirPath("/aa/foo.chpl", "/a") == false);
}

static void testIsSameFile() {
  assert(isSameFile("testPathUtils", "testPathUtils") == true);
  assert(isSameFile("testPathUtils", "../util/testPathUtils") == true);
  // also test behavior with files that don't exist
  assert(isSameFile("foo", "testPathUtils") == false);
  assert(isSameFile("foo", "foo") == true);
  assert(isSameFile("foo", "../util/foo") == true);
  assert(isSameFile("foo", "./././foo") == true);
  assert(isSameFile("foo", "bar") == false);
  assert(isSameFile("../bar/../baz", "../baz") == true);
  assert(isSameFile("testPathUtils", "") == false);
  assert(isSameFile("foo", "") == false);
  assert(isSameFile("", ".") == false);
  assert(isSameFile(".", "") == false);
  assert(isSameFile("", "") == true);
  assert(isSameFile("/bin", "") == false);
  assert(isSameFile("/usr/bin", "/usr/bin") == true);
  assert(isSameFile("/usr/bin", "") == false);
  assert(isSameFile("/usr/bin", "") == false);
}

static void checkDeduplicate(std::vector<std::string> input,
                             std::vector<std::string> expect) {
  std::vector<std::string> got = deduplicateSamePaths(input);

  if (expect.size() != got.size()) {
    printf("size mismatch\n");
    assert(expect.size() == got.size());
    return;
  }

  for (size_t i = 0; i < expect.size(); i++) {
    if (expect[i] != got[i]) {
      printf("mismatch, expected %s but got %s\n",
             expect[i].c_str(), got[i].c_str());
      assert(expect[i] == got[i]);
    }
  }
}

static void testDeduplicateSamePaths() {
  checkDeduplicate({"foo", "bar", "baz", "foo"}, {"foo", "bar", "baz"});
  checkDeduplicate({"testPathUtils", "../util/testPathUtils", "foo"},
                   {"testPathUtils", "foo"});
  checkDeduplicate({"../baz", "../bar/../baz"},
                   {"../baz"});
}


int main(int argc, char** argv) {
  testCleanLocalPath();
  testPathInDirPath();
  testIsSameFile();
  testDeduplicateSamePaths();

  return 0;
}
