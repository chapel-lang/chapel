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

#include "chpl/framework/ID.h"
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

  // populate modulePathToSection and moduleIdsAndFilePaths
  // module offsets are stored just after the file header
  const uint64_t* moduleOffsets = (const uint64_t*) (data + sizeof(FileHeader));
  for (uint32_t i = 0; i < nModules; i++) {
    uint64_t offset = moduleOffsets[i];
    const ModuleHeader* mod = (const ModuleHeader*) (data + offset);

    if (mod->magic != MODULE_SECTION_MAGIC ||
        offset+mod->len > len) {
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
    std::string fromFilePathStr = des.read<std::string>();
    UniqueString moduleId = UniqueString::get(context, moduleIdStr);
    UniqueString fromFilePath = UniqueString::get(context, fromFilePathStr);

    ModuleInfo info;
    info.moduleSymPath = moduleId;
    info.sourceFilePath = fromFilePath;
    info.moduleSectionOffset = offset;
    modules.push_back(info);
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
  ret.moduleSectionLen = modHdr->len;
  // string offsets start just after the header
  ret.offsetsTable = (const uint64_t*) (tableHdr+1);
  return ret;
}

bool LibraryFile::readModuleAst(Context* context,
                                uint64_t moduleOffset,
                                uast::Builder& builder) const {
  const ModuleHeader* modHdr = (const ModuleHeader*) (data + moduleOffset);
  // read the uast
  uint64_t uAstOffset = moduleOffset + modHdr->uAstSection;
  if (modHdr->magic != MODULE_SECTION_MAGIC ||
      uAstOffset+sizeof(AstSectionHeader) > len) {
    context->error(Location(), "Invalid module section header in %s",
                   libPath.c_str());
    return false;
  }

  const AstSectionHeader* astHdr =
    (const AstSectionHeader*) (data + uAstOffset);

  if (astHdr->magic != UAST_SECTION_MAGIC ||
      astHdr->nEntries == 0 ||
      astHdr->nBytesAstEntries == 0 ||
      uAstOffset + astHdr->nBytesAstEntries > len) {
    context->error(Location(), "Invalid AST section header in %s",
                   libPath.c_str());
    return false;
  }

  // create a LibraryFileStringsTable helper
  LibraryFileStringsTable table = readStringsTable(context, moduleOffset);

  auto des = Deserializer(context,
                          astHdr+1, // just after the AstSectionHeader
                          astHdr->nBytesAstEntries,
                          &table);

  builder.addToplevelExpression(uast::AstNode::deserializeWithoutIds(des));

  return true;
}

const uast::BuilderResult&
LibraryFile::loadAstQuery(Context* context,
                          const LibraryFile* f,
                          UniqueString fromSourceFilePath) {
  QUERY_BEGIN(loadAstQuery, context, f, fromSourceFilePath);

  // The parentSymbolPath should be the same for all modules from
  // a given source file. (Otherwise, we cannot return assign IDs
  // within the same Builder or return them in the same BuilderResult).
  UniqueString parentSymbolPath;
  bool parentSymbolPathSet = false;
  for (const auto& modInfo: f->modules) {
    if (modInfo.sourceFilePath == fromSourceFilePath) {
      auto locParentSymbolPath =
        ID::parentSymbolPath(context, modInfo.moduleSymPath);
      if (parentSymbolPathSet) {
        CHPL_ASSERT(locParentSymbolPath == parentSymbolPath);
      }
      parentSymbolPath = locParentSymbolPath;
    }
  }

  auto builder = uast::Builder::createForLibraryFileModule(context,
                                                           f->libPath,
                                                           parentSymbolPath);

  bool ok = true;
  for (const auto& modInfo: f->modules) {
    if (modInfo.sourceFilePath == fromSourceFilePath) {
      ok = f->readModuleAst(context, modInfo.moduleSectionOffset, *builder);
      if (!ok) break;
    }
  }

  uast::BuilderResult result;

  if (ok) {
    auto r = builder->result();
    result.swap(r);
  }

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

void LibraryFile::registerLibrary(Context* context) const {
  for (const auto& modInfo : modules) {
    int postOrderId = -1; // the symbol itself
    ID id = ID(modInfo.moduleSymPath, postOrderId, /* numChildIds */ 0);
    // note: numChildIds is not needed in this ID
    context->registerLibraryForModule(id, modInfo.sourceFilePath, libPath);
  }
}

std::vector<UniqueString> LibraryFile::containedFilePaths() const {
  std::set<UniqueString> paths;
  for (const auto& modInfo : modules) {
    paths.insert(modInfo.sourceFilePath);
  }

  std::vector<UniqueString> ret;
  for (auto path: paths) {
    ret.push_back(path);
  }

  return ret;
}

const uast::BuilderResult&
LibraryFile::loadSourceAst(Context* context,
                           UniqueString fromSourcePath) const {
  return loadAstQuery(context, this, fromSourcePath);
}

const uast::Module*
LibraryFile::loadModuleAst(Context* context,
                           UniqueString moduleSymPath) const {
  // figure out which source file has the requested module
  UniqueString sourceFilePath;
  for (const auto& modInfo : modules) {
    if (modInfo.moduleSymPath == moduleSymPath) {
      sourceFilePath = modInfo.sourceFilePath;
    }
  }

  if (sourceFilePath.isEmpty()) {
    return nullptr;
  }

  const uast::BuilderResult& r = loadSourceAst(context, sourceFilePath);

  // now find the module with the appropriate symbol path
  for (auto ast : r.topLevelExpressions()) {
    if (ast->id().symbolPath() == moduleSymPath) {
      return ast->toModule();
    }
  }

  // return nullptr if not found
  return nullptr;
}


} // end namespace libraries
} // end namespace chpl
