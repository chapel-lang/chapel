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

#include "chpl/framework/serialize-functions.h"

namespace chpl {


void writeUnsignedVarint(std::ostream& os, uint64_t num) {
  for (int i = 0; i < 10; i++) {
    uint8_t byte = num & 0x7f;
    num >>= 7;
    if (num != 0) byte |= 0x80;
    os.put(byte);
    if (byte & 0x80) {
      // OK, continue
    } else {
      break;
    }
  }
}

void writeSignedVarint(std::ostream& os, int64_t num) {
  uint64_t uNum = (num << 1) ^ (num >> 63);
  writeUnsignedVarint(os, uNum);
}

uint64_t readUnsignedVarint(std::istream& is) {
  uint64_t num = 0;
  for (int i = 0; i < 10; i++) {
    auto byte = is.get();
    // TODO: what should this function do on EOF?
    // What if it reads a byte that should be followed by another
    // byte there is no other byte (due to EOF)?
    if (byte == is.eof()) break;
    uint64_t part = byte & 0x7f;
    num |= part << (7*i);
    if (byte & 0x80) {
      // continue
    } else {
      break; // all done
    }
  }
  return num;
}

int64_t readSignedVarint(std::istream& is) {
  uint64_t uNum = readUnsignedVarint(is);
  return (uNum >> 1) ^ -((int64_t)(uNum & 1));
}


} // end namespace chpl