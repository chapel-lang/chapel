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

#include "chpl/libraries/LibraryFile.h"

#include "chpl/framework/query-impl.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Builder.h"

#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/FileSystem.h"

#include <cstring>

namespace chpl {
namespace libraries {


std::pair<size_t, const char*>
LibraryFileStringsTable::getString(int id) const {
  if (0 < id && id < nStrings) {
    uint64_t offset = offsetsTable[id];
    uint64_t nextOffset = offsetsTable[id+1];
    return std::make_pair(nextOffset-offset,
                          (const char*) (moduleSectionData + offset));
  } else {
    return std::make_pair(0, nullptr);
  }
}

LibraryFile::~LibraryFile() {
  if (mappedFile) delete mappedFile;
  if (fd >= 0) llvm::sys::fs::closeFile(fd);
}

std::error_code LibraryFile::openAndMap() {
  std::error_code err;

  // open the file
  err = llvm::sys::fs::openFileForRead(libPath.str(), fd);
  if (err) return err;

  // get the file size
  uint64_t fileSize = 0;
  err = llvm::sys::fs::file_size(libPath.str(), fileSize);
  if (err) return err;

  // memory map the file
  auto mode = llvm::sys::fs::mapped_file_region::readonly;
  mappedFile =
    new llvm::sys::fs::mapped_file_region(fd, mode, fileSize, /* offset */ 0,
                                          err);
  if (err) return err;

  len = fileSize;
  data = (const unsigned char*) mappedFile->const_data();

  // if we get here, return the no-error error code
  return std::error_code();
}


bool LibraryFile::readHeaders(Context* context) {
  std::error_code err = openAndMap();
  if (err) {
    context->error(Location(), "Could not open file %s: %s",
                   libPath.c_str(), err.message().c_str());
    return false;
  }

  if (fd < 0 || len <= 0 ||
      data == nullptr || data == (unsigned char*) -1) {
    // note: mmap can return -1 as a pointer upon failure
    context->error(Location(), "Could not read file %s", libPath.c_str());
  }

  // inspect the file header
  const FileHeader* header = (const FileHeader*) data;

  if (header->magic != FILE_HEADER_MAGIC) {
    context->error(Location(), "Invalid file header in %s", libPath.c_str());
    return false;
  }

  // save the file hash
  memcpy(&fileHash[0], &header->hash[0], HASH_SIZE);

  uint32_t nModules = header->nModules;

  // populate modulePathToSection
  // module offsets are stored just after the file header
  const uint64_t* moduleOffsets = (const uint64_t*) (data + sizeof(FileHeader));
  for (uint32_t i = 0; i < nModules; i++) {
    uint64_t offset = moduleOffsets[i];
    const ModuleHeader* mod = (const ModuleHeader*) (data + offset);

    if (mod->magic != MODULE_SECTION_MAGIC) {
      context->error(Location(), "Invalid module section header in %s",
                     libPath.c_str());
      return false;
    }

    // Read starting just after the module header
    LibraryFileStringsTable table = readStringsTable(context, offset);

    uint64_t pos = offset + sizeof(ModuleHeader);
    uint64_t remaining = len - pos;
    Deserializer des(context, data + pos, remaining, &table);

    std::string moduleIdStr = des.read<std::string>();
    UniqueString moduleId = UniqueString::get(context, moduleIdStr);
    modulePathToSection[moduleId] = offset;
  }

  return true;
}

const owned<LibraryFile>&
LibraryFile::loadLibraryFileQuery(Context* context, UniqueString libPath) {
  QUERY_BEGIN(loadLibraryFileQuery, context, libPath);

  owned<LibraryFile> result = toOwned(new LibraryFile());
  result->libPath = libPath;

  result->openAndMap();
  bool ok = result->readHeaders(context);

  if (!ok) {
    result = nullptr;
  }

  return QUERY_END(result);
}

LibraryFileStringsTable
LibraryFile::readStringsTable(Context* context, uint64_t moduleOffset) const {

  const ModuleHeader* modHdr = (const ModuleHeader*) (data + moduleOffset);
  // from the module header, find the longStringsTable offset
  uint64_t longStringsOffset = moduleOffset + modHdr->longStringsTable;
  if (modHdr->magic != MODULE_SECTION_MAGIC ||
      longStringsOffset+sizeof(LongStringsTableHeader) > len) {
    context->error(Location(), "Invalid module section header in %s",
                   libPath.c_str());
    return LibraryFileStringsTable();
  }

  const LongStringsTableHeader* tableHdr =
    (const LongStringsTableHeader*) (data + longStringsOffset);

  if (tableHdr->magic != LONG_STRINGS_TABLE_MAGIC ||
      longStringsOffset + tableHdr->nLongStrings*sizeof(uint64_t) > len) {
    context->error(Location(), "Invalid long strings section header in %s",
                   libPath.c_str());
    return LibraryFileStringsTable();
  }

  LibraryFileStringsTable ret;
  ret.nStrings = tableHdr->nLongStrings;
  ret.moduleSectionData = data + moduleOffset;
  ret.moduleSectionLen = len - moduleOffset;
  // string offsets start just after the header
  ret.offsetsTable = (const uint64_t*) (tableHdr+1);
  return ret;
}

uast::BuilderResult LibraryFile::readModuleAst(Context* context,
                                               UniqueString modulePath) const {
  auto it = modulePathToSection.find(modulePath);

  if (it == modulePathToSection.end()) {
    // fail if the module path requested is not in this file
    return uast::BuilderResult();
  }

  uint64_t moduleOffset = it->second;
  const ModuleHeader* modHdr = (const ModuleHeader*) (data + moduleOffset);
  // read the uast
  uint64_t uAstOffset = moduleOffset + modHdr->uAstSection;
  if (modHdr->magic != MODULE_SECTION_MAGIC ||
      uAstOffset+sizeof(AstSectionHeader) > len) {
    context->error(Location(), "Invalid module section header in %s",
                   libPath.c_str());
    return uast::BuilderResult();
  }

  const AstSectionHeader* astHdr =
    (const AstSectionHeader*) (data + uAstOffset);

  if (astHdr->magic != 0x0003bb1e5ec110e0 ||
      astHdr->nEntries == 0 ||
      astHdr->nBytesAstEntries == 0 ||
      uAstOffset + astHdr->nBytesAstEntries > len) {
    context->error(Location(), "Invalid AST section header in %s",
                   libPath.c_str());
    return uast::BuilderResult();
  }

  // create a LibraryFileStringsTable helper
  LibraryFileStringsTable table = readStringsTable(context, moduleOffset);

  UniqueString parentSymbolPath;
  if (!modulePath.isEmpty()) {
    parentSymbolPath = ID::parentSymbolPath(context, modulePath);
  }

  auto builder =
    uast::Builder::createForLibraryFileModule(context,
                                              libPath,
                                              parentSymbolPath);

  auto des = Deserializer(context,
                          astHdr+1, // just after the AstSectionHeader
                          astHdr->nBytesAstEntries,
                          &table);

  builder->addToplevelExpression(uast::AstNode::deserializeWithoutIds(des));

  uast::BuilderResult r = builder->result();

  if (r.numTopLevelExpressions() != 1 ||
      !r.topLevelExpression(0) ||
      !r.topLevelExpression(0)->toModule()) {
    context->error(Location(), "Invalid AST section in %s", libPath.c_str());
    return uast::BuilderResult();
  }

  return r;
}

const uast::BuilderResult&
LibraryFile::loadModuleAstQuery(Context* context,
                                const LibraryFile* f,
                                UniqueString modulePath) {
  QUERY_BEGIN(loadModuleAstQuery, context, f, modulePath);

  uast::BuilderResult result = f->readModuleAst(context, modulePath);

  return QUERY_END(result);
}


bool LibraryFile::operator==(const LibraryFile& other) const {
  // Handle empty paths
  if (libPath.isEmpty() && other.libPath.isEmpty()) {
    return true; // both are empty
  }
  if (libPath.isEmpty() || other.libPath.isEmpty()) {
    return false; // one of them is empty but not the other
  }
  // Note: the path itself is not compared here,
  // so that the comparison is about the file contents, instead.

  if (0 != memcmp(&fileHash[0], &other.fileHash[0], HASH_SIZE)) {
    return false;
  }

  if (len != other.len) {
    return false;
  }

  return true;
}

size_t LibraryFile::hash() {
  size_t total = 0;
  size_t arr[HASH_SIZE/sizeof(size_t)];
  memcpy(&arr[0], &fileHash[0], sizeof(arr));
  int n = HASH_SIZE/sizeof(size_t);
  for (int i = 0; i < n; i++) {
    total = hash_combine(total, arr[i]);
  }

  return total;
}

void LibraryFile::mark(Context* context) const {
  libPath.mark(context);
}

bool LibraryFile::update(owned<LibraryFile>& keep, owned<LibraryFile>& addin) {
  return defaultUpdateOwned(keep, addin);
}

void LibraryFile::stringify(std::ostream& ss,
                            chpl::StringifyKind stringKind) const {
  ss << llvm::toHex(fileHash);
}

const LibraryFile* LibraryFile::load(Context* context, UniqueString libPath) {
  return LibraryFile::loadLibraryFileQuery(context, libPath).get();
}

const uast::Module* LibraryFile::loadModuleAst(Context* context,
                                               UniqueString modulePath) const {
  auto it = modulePathToSection.find(modulePath);
  if (it != modulePathToSection.end()) {
    const uast::BuilderResult& r =
      loadModuleAstQuery(context, this, modulePath);

    return r.topLevelExpression(0)->toModule();
  }

  // otherwise, the module was not found.
  return nullptr;
}


} // end namespace libraries
} // end namespace chpl
