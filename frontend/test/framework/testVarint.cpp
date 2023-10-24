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

#include "chpl/framework/Context.h"
#include "chpl/framework/serialize-functions.h"

#include <iostream>
#include <iostream>
#include <sstream>
#include <string>

using namespace chpl;

static void testMatchUnsigned(uint64_t i, std::string expect) {
  std::ostringstream os;
  writeUnsignedVarint(os, i);
  std::string got = os.str();
  assert(got.size() == expect.size());
  for (size_t i = 0; i < expect.size(); i++) {
    assert(got[i] == expect[i]);
  }

  // check also that we can read it
  auto is = std::istringstream(got);
  uint64_t val = readUnsignedVarint(is);
  assert(val == i);
}

static void test1() {
  testMatchUnsigned(0, std::string(1, '\x00'));
  testMatchUnsigned(1, "\x01");
  testMatchUnsigned(0x7e, "\x7e");
  testMatchUnsigned(513, "\x81\x04");
  testMatchUnsigned(514, "\x82\x04");
  testMatchUnsigned(0x2bb, "\xbb\x05");
  testMatchUnsigned(0xfedcba9876543210ull,
                    "\x90\xe4\xd0\xb2\x87\xd3\xae\xee\xfe\x01");
}

static void testMatchSigned(int64_t i, std::string expect) {
  std::ostringstream os;
  writeSignedVarint(os, i);
  std::string got = os.str();
  assert(got.size() == expect.size());
  for (size_t i = 0; i < expect.size(); i++) {
    assert(got[i] == expect[i]);
  }

  // check also that we can read it
  auto is = std::istringstream(got);
  int64_t val = readSignedVarint(is);
  assert(val == i);
}

static void test2() {
  testMatchSigned(0, std::string(1, '\x00'));
  testMatchSigned(1, "\x02");
  testMatchSigned(2, "\x04");
  testMatchSigned(257, "\x82\x04");
  testMatchSigned(-257, "\x81\x04");
  testMatchSigned(-1, "\x01");
  testMatchSigned(-2, "\x03");
}

// testing unsigned writes and reads with a bunch of values
static void test3() {
  std::ostringstream os;
  for (uint64_t i = 0; i < 1000000; i++) {
    writeUnsignedVarint(os, i);
  }
  std::istringstream is(os.str());
  for (uint64_t i = 0; i < 1000000; i++) {
    uint64_t got = readUnsignedVarint(is);
    assert(got == i);
  }
}

// testing signed writes and reads with a bunch of values
static void test4() {
  std::ostringstream os;
  for (int64_t i = -500000; i < 500000; i++) {
    writeSignedVarint(os, i);
  }
  std::istringstream is(os.str());
  for (int64_t i = -500000; i < 500000; i++) {
    int64_t got = readSignedVarint(is);
    assert(got == i);
  }
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  test4();

  return 0;
}
