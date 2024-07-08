/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/libraries/LibraryFile.h"
#include "chpl/libraries/LibraryFileWriter.h"

namespace chpl {


void Serializer::writeUnsignedVarint(uint64_t num) {
  for (int i = 0; i < 10; i++) {
    uint8_t byte = num & 0x7f;
    num >>= 7;
    if (num != 0) byte |= 0x80;
    writeByte(byte);
    if (byte & 0x80) {
      // OK, continue
    } else {
      break;
    }
  }
}

void Serializer::writeSignedVarint(int64_t num) {
  uint64_t uNum = (num << 1) ^ (num >> 63);
  writeUnsignedVarint(uNum);
}

void Serializer::beginAst(const uast::AstNode* ast) {
  if (libraryFileHelper_ != nullptr) {
    libraryFileHelper_->beginAst(ast, os_);
  }
}

void Serializer::endAst(const uast::AstNode* ast) {
  if (libraryFileHelper_ != nullptr) {
    libraryFileHelper_->endAst(ast, os_);
  }
}


void Deserializer::registerAst(const uast::AstNode* ast, uint64_t startOffset) {
  if (libraryFileHelper_ != nullptr) {
    libraryFileHelper_->registerAst(ast, startOffset);
  }
}

uint64_t Deserializer::readUnsignedVarint() {
  uint64_t num = 0;
  for (int i = 0; i < 10; i++) {
    auto byte = readByte();
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

int64_t Deserializer::readSignedVarint() {
  uint64_t uNum = readUnsignedVarint();
  return (uNum >> 1) ^ -((int64_t)(uNum & 1));
}

std::pair<size_t, const char*> Deserializer::getString(int id) {
  if (libraryFileHelper_ != nullptr) {
    auto ret = libraryFileHelper_->getString(id);
    if (checkStringLength(ret.first)) {
      return ret;
    }
  } else if (localStringsTable_) {
    stringCacheType& table = *localStringsTable_.get();
    return table[id];
  }

  return std::make_pair((size_t) 0, (const char*) nullptr);
}


} // end namespace chpl
