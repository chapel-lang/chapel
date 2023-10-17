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

#include "chpl/framework/LibraryFile.h"

namespace chpl {

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
    ModuleVec modsInFile = parse(context, path, empty);
    for (auto mod : modsInFile) {
      topLevelModules.push_back(mod);
    }
  }
}

void LibraryFileWriter::openFile() {
  fileStream.open(outFileName,
                  std::ios::out | std::ios::trunc | std::ios::binary);
}

void LibraryFileWriter::writeHeader() {
  // Construct a file header and write it
  detail::FileHeader header;
  memset(&header, 0, sizeof(header));

  header.magic = detail::FILE_HEADER_MAGIC;
  header.fileFormatVersionMajor = detail::LIBRARY_VERSION_MAJOR;
  header.fileFormatVersionMinor = detail::LIBRARY_VERSION_MINOR;
  header.chplVersionMajor = getMajorVersion();
  header.chplVersionMinor = getMinorVersion();
  header.chplVersionUpdate = getUpdateVersion();
  header.nModules = topLevelModules.size();
  fileStream.write(&header, sizeof(header));

  // write the placeholder module section table
  for (auto mod : topLevelModules) {
    fileStream.write((uint64_t) 0);
  }
}

uint64_t LibraryFileWriter::writeModuleSection(const uast::Module* mod) {
  auto moduleSectionStart = fileStream.tellp();

  // Construct a module section header and write it
  detail::ModuleHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = detail::MODULE_SECTION_MAGIC;
  fileStream.write(&header, sizeof(header));

  // create a serializer to write the uAST
  auto ser = Serializer(fileStream);

  // write the symbol table section
  header.symbolTable = writeSymbolTable(mod, moduleSectionStart);
  header.uAstSection = writeAst(mod, moduleSectionStart, ser);
  header.longStringsTable = writeLongStrings(moduleSectionStart);
  header.locationSection = writeLocations(mod, moduleSectionStart);

  // update the module header with the saved locations by writing the
  // header again.
  auto savePos = fileStream.tellp();
  
  fileStream.seekp(moduleSectionStart);
  fileStream.write(&header, sizeof(header));

  // seek back where we were
  fileStream.seekp(saveP);

  return moduleSectionStart; 
}

uint64_t LibraryFileWriter::writeSymbolTable(const uast::Module* mod,
                                             uint64_t moduleSectionStart) {
  auto symTableStart = fileStream.tellp();
  detail::SymbolTableHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = SYMBOL_TABLE_MAGIC;
  fileStream.write(&header, sizeof(header));
  
  // TODO TODO TODO actually output something meaningful

  return symTableStart - moduleSectionStart;
}

uint64_t LibraryFileWriter::writeAst(const uast::Module* mod,
                                     uint64_t moduleSectionStart,
                                     Serializer& ser) {
  auto astStart = fileStream.tellp();
  detail::uAstSectionHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = detail::MODULE_SECTION_MAGIC;
  fileStream.write(&header, sizeof(header));

  // serialize the data
  mod.serialize(ser);
  header.nEntries = ser.numAstsSerialized();

  // update the number serialized by rewriting the header
  auto savePos = fileStream.tellp();
  
  fileStream.seekp(astStart);
  fileStream.write(&header, sizeof(header));

  // seek back where we were
  fileStream.seekp(saveP);

  return astStart - moduleSectionStart;
}

uint64_t
LibraryFileWriter::writeLongStrings(uint64_t moduleSectionStart,
                                    Serializer& ser) {
  auto longStringsStart = fileStream.tellp();

  detail::LongStringsTableHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = detail::LONG_STRINGS_TABLE_MAGIC;
  header.nLongStrings = ser.nextStringIdx();
  fileStream.write(&header, sizeof(header));
  
  size_t n = header.nLongStrings;

  std::vector<uint64_t> offsets;
  offsets.resize(n);
  // write 0s for the offsets. We will rewrite it in a moment.
  fileStream.write(&offsets[0], n*sizeof(uint64_t));

  // gather the strings by idx so we can output them
  std::vector<std::pair<const char*, size_t>> byId;
  byId.resize(n);

  for (const auto& kv : ser.stringCache()) {
    const char* ptr = kv.first; 
    const auto& pair = kv.second;
    uint32_t idx = pair.first;
    size_t len = pair.second;
    if (0 < idx && idx < nLongStrings) {
      byId[idx] = std::make_pair(ptr, len);
    }
  }

  // output the strings in idx order and update offsets
  for (size_t i = 0; i < n; i++) {
    const auto& pair = byId[i];
    const char* ptr = pair.first;
    size_t len = pair.second;
    if (byId[i].first != nullptr) {
      offsets[i] = fileStream.tellp() - moduleSectionStart;
      fileStream.write(ptr, len);
    }
  }

  // update the offsets in the file
  fileStream.seekp(longStringsStart+sizeof(header));

  // write the actual offsets
  fileStream.write(&offsets[0], n*sizeof(uint64_t));
}

uint64_t LibraryFileWriter::writeLocations(const uast::Module* mod) {
}

void LibraryFileWriter::writeAllSections() {
  gatherTopLevelModules();
  openFile();
  writeHeader();

  std::vector<uint64_t> moduleSectionOffsets;

  for (mod : topLevelModules) {
    // write the module section & update the header's table
    uint64_t offset = writeModuleSection(mod);
    moduleSectionOffsets.push_back(offset);
  }

  auto savePos = fileStream.tellp();

  // update the module section table
  // seek just after the fixed portion of the file header
  fileStream.seekp(sizeof(detail::FileHeader));
  // write the offsets
  fileStream.write(&offset[0], sizeof(uint64_t)*moduleSectionOffsets.size());

  // seek back where we were
  fileStream.seekp(saveP);

  fileStream.close();
}


} // end namespace chpl
