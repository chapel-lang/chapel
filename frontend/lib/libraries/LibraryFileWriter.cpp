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

#include "chpl/libraries/LibraryFileWriter.h"

#include "chpl/libraries/LibraryFileFormat.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/util/filesystem.h"
#include "chpl/util/version-info.h"

namespace chpl {
namespace libraries {

static UniqueString cleanLocalPath(Context* context, UniqueString path) {
  if (path.startsWith("/") ||
      path.startsWith("./") == false) {
    return path;
  }

  auto str = path.str();
  while (str.find("./") == 0) {
    str = str.substr(2);
  }

  return chpl::UniqueString::get(context, str);
}

void LibraryFileWriter::gatherTopLevelModules() {
  // Parse the paths and gather a vector of top-level modules.
  for (auto path : inputFiles) {
    path = cleanLocalPath(context, path);
    UniqueString empty;
    std::vector<const uast::Module*> modsInFile =
      parsing::parse(context, path, empty);
    for (auto mod : modsInFile) {
      topLevelModules.push_back(mod);
    }
  }
}

void LibraryFileWriter::openFile() {
  fileStream.open(outputFilePath,
                  std::ios::out | std::ios::trunc | std::ios::binary);
}

void LibraryFileWriter::writeHeader() {
  // Construct a file header and write it
  FileHeader header;
  memset(&header, 0, sizeof(header));

  header.magic = FILE_HEADER_MAGIC;
  header.fileFormatVersionMajor = FORMAT_VERSION_MAJOR;
  header.fileFormatVersionMinor = FORMAT_VERSION_MINOR;
  header.chplVersionMajor = getMajorVersion();
  header.chplVersionMinor = getMinorVersion();
  header.chplVersionUpdate = getUpdateVersion();
  header.nModules = topLevelModules.size();
  // hash remains 0s at this point
  fileStream.write((const char*) &header, sizeof(header));

  // write the placeholder module section table
  size_t n = topLevelModules.size();
  for (size_t i = 0; i < n; i++) {
    uint64_t zero = 0;
    fileStream.write((const char*) &zero, sizeof(zero));
  }
}

uint64_t LibraryFileWriter::writeModuleSection(const uast::Module* mod) {
  auto moduleSectionStart = fileStream.tellp();

  // Construct a module section header and write it
  ModuleHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = MODULE_SECTION_MAGIC;
  fileStream.write((const char*) &header, sizeof(header));

  // create a serializer to write the uAST
  auto ser = Serializer(fileStream);

  // write the symbol table section
  header.symbolTable = writeSymbolTable(mod, moduleSectionStart);
  header.uAstSection = writeAst(mod, moduleSectionStart, ser);
  header.longStringsTable = writeLongStrings(moduleSectionStart, ser);
  header.locationSection = writeLocations(mod, moduleSectionStart);

  // update the module header with the saved locations by writing the
  // header again.
  auto savePos = fileStream.tellp();

  fileStream.seekp(moduleSectionStart);
  fileStream.write((const char*) &header, sizeof(header));

  // seek back where we were
  fileStream.seekp(savePos);

  return moduleSectionStart;
}

uint64_t LibraryFileWriter::writeSymbolTable(const uast::Module* mod,
                                             uint64_t moduleSectionStart) {
  uint64_t symTableStart = fileStream.tellp();
  SymbolTableHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = SYMBOL_TABLE_MAGIC;
  fileStream.write((const char*) &header, sizeof(header));

  // TODO: actually output something meaningful

  return symTableStart - moduleSectionStart;
}

uint64_t LibraryFileWriter::writeAst(const uast::Module* mod,
                                     uint64_t moduleSectionStart,
                                     Serializer& ser) {
  uint64_t astStart = fileStream.tellp();
  AstSectionHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = MODULE_SECTION_MAGIC;
  fileStream.write((const char*) &header, sizeof(header));

  // serialize the data
  uint64_t serializedStart = fileStream.tellp();
  mod->serialize(ser);
  header.nBytesAstEntries = serializedStart - fileStream.tellp();
  header.nEntries = ser.numAstsSerialized();

  // update the number serialized by rewriting the header
  auto savePos = fileStream.tellp();

  fileStream.seekp(astStart);
  fileStream.write((const char*) &header, sizeof(header));

  // seek back where we were
  fileStream.seekp(savePos);

  return astStart - moduleSectionStart;
}

uint64_t
LibraryFileWriter::writeLongStrings(uint64_t moduleSectionStart,
                                    Serializer& ser) {
  uint64_t longStringsStart = fileStream.tellp();

  LongStringsTableHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = LONG_STRINGS_TABLE_MAGIC;
  header.nLongStrings = ser.nextStringIdx();
  fileStream.write((const char*) &header, sizeof(header));

  size_t n = header.nLongStrings;

  std::vector<uint64_t> offsets;
  offsets.resize(n);
  // write 0s for the offsets. We will rewrite it in a moment.
  fileStream.write((const char*) &offsets[0], n*sizeof(uint64_t));

  // gather the strings by idx so we can output them
  std::vector<std::pair<const char*, size_t>> byId;
  byId.resize(n);

  for (const auto& kv : ser.stringCache()) {
    const char* ptr = kv.first;
    const auto& pair = kv.second;
    uint32_t idx = pair.first;
    size_t len = pair.second;
    if (0 < idx && idx < n) {
      byId[idx] = std::make_pair(ptr, len);
    }
  }

  // output the strings in idx order and update offsets
  for (size_t i = 0; i < n; i++) {
    const auto& pair = byId[i];
    const char* ptr = pair.first;
    size_t len = pair.second;
    if (byId[i].first != nullptr) {
      uint64_t pos = fileStream.tellp();
      offsets[i] = pos - moduleSectionStart;
      fileStream.write(ptr, len);
    }
  }

  // update the offsets in the file
  fileStream.seekp(longStringsStart+sizeof(header));

  // write the actual offsets
  fileStream.write((const char*) &offsets[0], n*sizeof(uint64_t));

  return longStringsStart - moduleSectionStart;
}

uint64_t LibraryFileWriter::writeLocations(const uast::Module* mod,
                                           uint64_t moduleSectionStart) {
  uint64_t locationsSectionStart = fileStream.tellp();

  // TODO: actually write the locations

  return locationsSectionStart - moduleSectionStart;
}

bool LibraryFileWriter::writeAllSections() {
  gatherTopLevelModules();
  openFile();
  writeHeader();

  std::vector<uint64_t> moduleSectionOffsets;

  for (auto mod : topLevelModules) {
    // write the module section & update the header's table
    uint64_t offset = writeModuleSection(mod);
    moduleSectionOffsets.push_back(offset);
  }

  // update the module section table
  // seek just after the fixed portion of the file header
  fileStream.seekp(sizeof(FileHeader));
  // write the offsets
  fileStream.write((const char*) &moduleSectionOffsets[0],
                   sizeof(uint64_t)*moduleSectionOffsets.size());

  // compute and store the file hash
  CHPL_ASSERT(sizeof(HashFileResult) == HASH_SIZE);

  // flush the file data so the 'hashFile' call will read everything
  // we just wrote
  fileStream.flush();

  llvm::ErrorOr<HashFileResult> hashOrErr = hashFile(outputFilePath);

  if (hashOrErr) {
    const HashFileResult& h = hashOrErr.get();
    // use the minimum of HASH_SIZE and sizeof(HashFileResult)
    // (should get assert above in case these stop matching, but
    //  that will be off in some builds)
    size_t n = sizeof(HashFileResult);
    if (HASH_SIZE < n) n = HASH_SIZE;
    // seek to the hash position within the file
    fileStream.seekp(offsetof(FileHeader, hash));
    // write the hash bytes
    fileStream.write((const char*) &h, n);
  }

  // TODO: if further error information is desired, probably best
  // to switch to using the C FILE* and fwrite.
  bool ok = fileStream.good();
  fileStream.close();

  return ok;
}


} // end namespace libraries
} // end namespace chpl
