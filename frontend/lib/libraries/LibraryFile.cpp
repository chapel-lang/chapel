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
LibraryFileDeserializationHelper::getString(int id) const {
  if (0 < id && id < nStrings) {
    uint64_t offset = stringOffsetsTable[id];
    uint64_t nextOffset = stringOffsetsTable[id+1];
    return std::make_pair(nextOffset-offset,
                          (const char*) (moduleSectionData + offset));
  } else {
    return std::make_pair(0, nullptr);
  }
}

void LibraryFileDeserializationHelper::registerAst(const uast::AstNode* ast,
                                                   uint64_t startOffset) {
  auto search = offsetToSymIdx.find(startOffset);
  if (search != offsetToSymIdx.end()) {
    astToSymIdx[ast] = search->second;
  }
}


void LibraryFile::LocationMaps::clear() {
  astToLocation.clear();

  #define LOCATION_MAP(ast__, location__) { \
      auto& m = CHPL_ID_LOC_MAP(ast__, location__); \
      m.clear(); \
    }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP
}

const LibraryFile::LocationMaps::MapType*
LibraryFile::LocationMaps::getLocationMap(int tag) const {
  switch (tag) {

    #define LOCATION_MAP(ast__, location__) \
      case (int) uast::BuilderResult::LocationMapTag::location__: \
        return &CHPL_ID_LOC_MAP(ast__, location__); \

    #include "chpl/uast/all-location-maps.h"
    #undef LOCATION_MAP

    default:
      return nullptr;
  }

  return nullptr;
}
LibraryFile::LocationMaps::MapType*
LibraryFile::LocationMaps::getLocationMap(int tag) {
   return const_cast<MapType*>(
       const_cast<const LocationMaps*>(this)->getLocationMap(tag));
}

void LibraryFile::LocationMaps::swap(LocationMaps& other) {
  astToLocation.swap(other.astToLocation);

  #define LOCATION_MAP(ast__, location__) { \
    auto& m1 = CHPL_ID_LOC_MAP(ast__, location__); \
    auto& m2 = other.CHPL_ID_LOC_MAP(ast__, location__); \
    m1.swap(m2); \
  }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP
}
bool LibraryFile::LocationMaps::update(LocationMaps& keep,
                                       LocationMaps& addin) {
  bool changed = false;
  changed |= defaultUpdate(keep.astToLocation, addin.astToLocation);

  #define LOCATION_MAP(ast__, location__) { \
    auto& m1 = keep.CHPL_ID_LOC_MAP(ast__, location__); \
    auto& m2 = addin.CHPL_ID_LOC_MAP(ast__, location__); \
    changed |= defaultUpdate(m1, m2); \
  }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP

  return changed;
}
void LibraryFile::LocationMaps::mark(Context* context) const {
  #define LOCATION_MAP(ast__, location__) { \
    auto& m = CHPL_ID_LOC_MAP(ast__, location__); \
    for (const auto& p : m) { \
      context->markPointer(p.first); \
      p.second.mark(context); \
    } \
  }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP
}


LibraryFile::~LibraryFile() {
  if (mappedFile) delete mappedFile;
  if (fd >= 0) llvm::sys::fs::closeFile(fd);
}

void LibraryFile::invalidFileError(Context* context) const {
  context->error(Location(), "Invalid library file %s", libPath.c_str());
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
    invalidFileError(context);
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
      invalidFileError(context);
      return false;
    }

    // Read starting just after the module header
    LibraryFileDeserializationHelper helper;

    Deserializer des(context,
                     data + offset,
                     data + offset + sizeof(ModuleHeader),
                     data + len - offset,
                     helper);

    std::string moduleIdStr = des.read<std::string>();
    std::string fromFilePathStr = des.read<std::string>();
    UniqueString moduleId = UniqueString::get(context, moduleIdStr);
    UniqueString fromFilePath = UniqueString::get(context, fromFilePathStr);

    if (!des.ok()) {
      invalidFileError(context);
      return false;
    }

    ModuleInfo info;
    info.moduleSymPath = moduleId;
    info.sourceFilePath = fromFilePath;
    info.moduleSectionOffset = offset;

    modules.push_back(std::move(info));
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

bool LibraryFile::readModuleSection(Context* context,
                                    uint64_t moduleOffset,
                                    ModuleSection& mod) const {

  const ModuleHeader* modHdr = (const ModuleHeader*) (data + moduleOffset);
  // from the module header, find the longStringsTable offset
  // and the symbol table offset
  uint64_t symbolTableRelOffset = modHdr->symbolTable;
  uint64_t symbolTableOffset = moduleOffset + modHdr->symbolTable;
  uint64_t longStringsOffset = moduleOffset + modHdr->longStringsTable;
  if (modHdr->magic != MODULE_SECTION_MAGIC ||
      symbolTableRelOffset+sizeof(SymbolTableHeader) > modHdr->len ||
      symbolTableOffset+sizeof(SymbolTableHeader) > len ||
      longStringsOffset+sizeof(LongStringsTableHeader) > len) {
    invalidFileError(context);
    return false;
  }

  const LongStringsTableHeader* strTableHeader =
    (const LongStringsTableHeader*) (data + longStringsOffset);

  if (strTableHeader->magic != LONG_STRINGS_TABLE_MAGIC ||
      longStringsOffset + strTableHeader->nLongStrings*sizeof(uint64_t) > len) {
    invalidFileError(context);
    return false;
  }

  const SymbolTableHeader* symTableHeader =
    (const SymbolTableHeader*) (data + symbolTableOffset);

  if (symTableHeader->magic != SYMBOL_TABLE_MAGIC ||
      symbolTableOffset +
        symTableHeader->nEntries*sizeof(SymbolTableEntry) > len) {
    invalidFileError(context);
    return false;
  }

  mod.nStrings = strTableHeader->nLongStrings;
  mod.moduleSectionData = data + moduleOffset;
  mod.moduleSectionLen = modHdr->len;
  // string offsets start just after the header
  mod.stringOffsetsTable = (const uint32_t*) (strTableHeader+1);

  LibraryFileDeserializationHelper helper = setupHelper(context, &mod);

  // also read the symbol table uast locations
  size_t firstEntryRelOffset = symbolTableRelOffset + sizeof(SymbolTableHeader);
  Deserializer des(context,
                   data + moduleOffset,
                   data + moduleOffset + firstEntryRelOffset,
                   data + moduleOffset + modHdr->len,
                   helper);
  uint32_t n = symTableHeader->nEntries;
  std::string lastSymId;
  for (uint32_t i = 0; i < n; i++) {
    uint64_t pos = des.position();

    // read the entry offsets
    SymbolTableEntry entry;
    des.readData(&entry, sizeof(entry));
    // read the tag
    des.readByte();
    // read the variable-byte encoded common prefix length
    unsigned int nCommonPrefix = des.readVUint();
    // shorten lastSymId to the first nCommonPrefix bytes
    if (lastSymId.size() > nCommonPrefix) {
      lastSymId.erase(nCommonPrefix, lastSymId.size()-nCommonPrefix);
    }
    // read the variable-byte encoded suffix length
    unsigned int nSuffix = des.readVUint();
    // expand lastSymId to have room to store the suffix
    lastSymId.resize(nCommonPrefix+nSuffix);
    // read the string data
    des.readData(&lastSymId[nCommonPrefix], nSuffix);

    printf("Read symbol id %s (prefix %i suffix %i) offset %i\n",
           lastSymId.c_str(),
           (int) nCommonPrefix,
           (int) nSuffix,
           entry.uAstEntry);

    // record the information
    ModuleSection::SymbolInfo info;
    info.symbolEntryOffset = pos;
    info.uastOffset = entry.uAstEntry;
    info.locationsOffset = entry.locationEntry;
    mod.symbols.push_back(info);
    mod.offsetToSymIdx[info.uastOffset] = i;

    // check that the deserializer did not encounter an error
    // check that the SymbolTableEntry offsets are realistic
    if (!des.ok() ||
        moduleOffset + entry.uAstEntry > len ||
        moduleOffset + entry.locationEntry > len ||
        moduleOffset + entry.typeOrFnEntry > len) {
      invalidFileError(context);
      return false;
    }
  }

  return true;
}

const owned<LibraryFile::ModuleSection>&
LibraryFile::loadModuleSectionQuery(Context* context,
                                    const LibraryFile* f,
                                    int moduleIndex) {
  QUERY_BEGIN(loadModuleSectionQuery, context, f, moduleIndex);

  owned<ModuleSection> result;

  if (0 <= moduleIndex && (size_t) moduleIndex < f->modules.size()) {
    // figure out which module
    const ModuleInfo& info = f->modules[moduleIndex];
    uint64_t moduleOffset = info.moduleSectionOffset;
    result = toOwned(new ModuleSection());
    bool ok = f->readModuleSection(context, moduleOffset, *result.get());
    if (!ok) {
      // should have already raised an error.
      // clear 'result' so we don't return a partial result.
      result = nullptr;
    }
  }

  return QUERY_END(result);
}

const LibraryFile::ModuleSection*
LibraryFile::loadModuleSection(Context* context, int moduleIndex) const {
  return (loadModuleSectionQuery(context, this, moduleIndex)).get();
}

LibraryFileDeserializationHelper
LibraryFile::setupHelper(Context* context, const ModuleSection* mod) const {

  LibraryFileDeserializationHelper ret;

  // copy over offsetToSymIdx
  ret.offsetToSymIdx = mod->offsetToSymIdx;

  // copy over the string info to help UniqueString deserialization
  ret.nStrings = mod->nStrings;
  ret.moduleSectionData = mod->moduleSectionData;
  ret.moduleSectionLen = mod->moduleSectionLen;
  ret.stringOffsetsTable = mod->stringOffsetsTable;

  return ret;
}

bool LibraryFile::readModuleAst(Context* context,
                                uint64_t moduleOffset,
                                LibraryFileDeserializationHelper& helper,
                                uast::Builder& builder) const {
  const ModuleHeader* modHdr = (const ModuleHeader*) (data + moduleOffset);
  // read the uast
  uint64_t uAstOffset = moduleOffset + modHdr->uAstSection;
  if (modHdr->magic != MODULE_SECTION_MAGIC ||
      uAstOffset+sizeof(AstSectionHeader) > len) {
    invalidFileError(context);
    return false;
  }

  const AstSectionHeader* astHdr =
    (const AstSectionHeader*) (data + uAstOffset);

  if (astHdr->magic != UAST_SECTION_MAGIC ||
      astHdr->nEntries == 0 ||
      astHdr->nBytesAstEntries == 0 ||
      uAstOffset + astHdr->nBytesAstEntries > len) {
    invalidFileError(context);
    return false;
  }

  size_t pos = uAstOffset + sizeof(AstSectionHeader);
  auto des = Deserializer(context,
                          data + moduleOffset,
                          data + pos,
                          data + pos + astHdr->nBytesAstEntries,
                          helper);

  builder.addToplevelExpression(uast::AstNode::deserializeWithoutIds(des));

  if (!des.ok()) {
    invalidFileError(context);
    return false;
  }

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

  uast::Builder::SymbolTableVec combined;

  auto builder = uast::Builder::createForLibraryFileModule(context,
                                                           f->libPath,
                                                           parentSymbolPath,
                                                           f,
                                                           &combined);

  bool ok = true;
  int moduleIdx = 0;
  for (const auto& modInfo: f->modules) {
    if (modInfo.sourceFilePath == fromSourceFilePath) {
      const ModuleSection* m = f->loadModuleSection(context, moduleIdx);
      if (m == nullptr) {
        ok = false;
        break;
      }
      auto helper = f->setupHelper(context, m);
      ok = f->readModuleAst(context, modInfo.moduleSectionOffset,
                            helper, *builder);
      if (!ok) break;
      // save the symbol table from the helper into the combined map
      for (const auto& pair: helper.astToSymIdx) {
        uast::Builder::SymbolTableInfo info;
        info.ast = pair.first;
        info.moduleIndex = moduleIdx;
        info.symbolIndex = pair.second;
        combined.push_back(std::move(info));
      }
    }
    moduleIdx++;
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

bool LibraryFile::readLocationPaths(Context* context,
                                    std::vector<UniqueString>& paths,
                                    const ModuleSection* m) const {
  const ModuleHeader* modHdr = (const ModuleHeader*) m->moduleSectionData;
  uint64_t locationsOffset = modHdr->locationSection;
  if (modHdr->magic != MODULE_SECTION_MAGIC ||
      locationsOffset+sizeof(LocationSectionHeader) > m->moduleSectionLen) {
    invalidFileError(context);
    return false;
  }

  const LocationSectionHeader* locHdr =
    (const LocationSectionHeader*) (m->moduleSectionData + locationsOffset);
  if (locHdr->magic != LOCATION_SECTION_MAGIC ||
      locHdr->nFilePaths == 0 ||
      locHdr->nGroups == 0) {
    invalidFileError(context);
    return false;
  }

  size_t firstPathOffset = locationsOffset + sizeof(LocationSectionHeader);
  // read the location group paths
  auto helper = setupHelper(context, m);
  Deserializer des(context,
                   m->moduleSectionData,
                   m->moduleSectionData + firstPathOffset,
                   m->moduleSectionData + m->moduleSectionLen,
                   helper);

  uint32_t n = locHdr->nFilePaths;
  for (uint32_t i = 0; i < n; i++) {
    std::string filePath;
    unsigned char srcHash[HASH_SIZE];

    filePath = des.read<std::string>();
    des.readData(&srcHash[0], sizeof(srcHash));

    paths.push_back(UniqueString::get(context, filePath));
  }

  if (!des.ok()) {
    invalidFileError(context);
    return false;
  }

  return true;
}

bool LibraryFile::readLocationGroup(
                        Context* context,
                        LocationMaps& maps,
                        Deserializer& des,
                        const uast::AstNode* symbolTableSymbolAst,
                        const std::vector<UniqueString>& paths,
                        const uast::BuilderResult& br) const {
  unsigned int filePathIdx = des.readVUint();
  unsigned int startLine = des.readVInt();

  UniqueString path;
  if (filePathIdx < paths.size()) {
    path = paths[filePathIdx];
  } else {
    invalidFileError(context);
    return false;
  }

  int lastEntryLastLine = startLine;
  return readLocationEntries(context, maps, des, symbolTableSymbolAst, path, br,
                             lastEntryLastLine);
}


bool LibraryFile::readLocationEntries(
                        Context* context,
                        LocationMaps& maps,
                        Deserializer& des,
                        const uast::AstNode* cur,
                        UniqueString path,
                        const uast::BuilderResult& br,
                        int& lastEntryLastLine) const {

  int firstLineDiff = des.readVInt();
  int firstLine = lastEntryLastLine + firstLineDiff;

  int lastLineDiff = des.readVInt();
  int lastLine = firstLine + lastLineDiff;

  unsigned int firstCol = des.readVUint();
  unsigned int lastCol = des.readVUint();


  maps.astToLocation[cur] = Location(path, firstLine, firstCol,
                                     lastLine, lastCol);

  // read the additional locations
  unsigned int nAddnlLocs = des.readVUint();
  for (unsigned int i = 0; i < nAddnlLocs; i++) {
    int tag = des.readVInt(); // tag
    std::unordered_map<const uast::AstNode*, Location>* aMap =
      maps.getLocationMap(tag);
    int otherFirstLineDiff = des.readVInt();
    int otherFirstLine = firstLine + otherFirstLineDiff;
    int otherLastLineDiff = des.readVInt();
    int otherLastLine = otherFirstLine + otherLastLineDiff;
    unsigned int otherFirstCol = des.readVUint();
    unsigned int otherLastCol = des.readVUint();
    if (aMap) {
      (*aMap)[cur] = Location(path, otherFirstLine, otherFirstCol,
                              otherLastLine, otherLastCol);
    }
  }

  // update lastEntryLastLine
  lastEntryLastLine = lastLine;

  // proceed to read the locations for the child uast nodes,
  // but skip any child node that is represented in the symbol table
  for (auto child : cur->children()) {
    if (br.isSymbolTableSymbol(child->id())) {
      // it's in the symbol table, so will be read separately
      // when reading a location group
    } else {
      bool ok = readLocationEntries(context, maps, des, child,
                                    path, br, lastEntryLastLine);
      if (!ok) {
        return false;
      }
      if (!des.ok()) {
        // also exit early if the deserializer recorded an error
        return false;
      }
    }
  }

  return true;
}

const LibraryFile::LocationMaps&
LibraryFile::loadLocationsQuery(Context* context,
                                const LibraryFile* f,
                                int moduleIndex,
                                int symbolTableEntryIndex,
                                const uast::AstNode* symbolTableEntryAst) {
  QUERY_BEGIN(loadLocationsQuery, context, f,
              moduleIndex, symbolTableEntryIndex, symbolTableEntryAst);

  LocationMaps result;


  bool ok = false;
  if (0 <= moduleIndex && (size_t) moduleIndex < f->modules.size()) {
    UniqueString fromSourcePath = f->modules[moduleIndex].sourceFilePath;
    const uast::BuilderResult& br = f->loadSourceAst(context, fromSourcePath);

    const ModuleSection* m = f->loadModuleSection(context, moduleIndex);
    std::vector<UniqueString> paths;
    if (m != nullptr) {
      ok = f->readLocationPaths(context, paths, m);
    }

    if (ok &&
        0 <= symbolTableEntryIndex &&
        (size_t) symbolTableEntryIndex < m->symbols.size()) {
      auto helper = f->setupHelper(context, m);

      ok = false;
      size_t locationGroupOffset =
         m->symbols[symbolTableEntryIndex].locationsOffset;
      if (locationGroupOffset < m->moduleSectionLen) {
        Deserializer des(context,
                         m->moduleSectionData,
                         m->moduleSectionData + locationGroupOffset,
                         m->moduleSectionData + m->moduleSectionLen,
                         helper);
        ok = f->readLocationGroup(context, result, des,
                                  symbolTableEntryAst, paths, br);
        if (!des.ok()) {
          f->invalidFileError(context);
          ok = false;
        }
      }
    }
  }

  if (!ok) {
    // do not return a partial result on failure
    result.clear();
  }

  return QUERY_END(result);
}




const LibraryFile::LocationMaps&
LibraryFile::loadLocations(Context* context,
                           int moduleIndex,
                           int symbolTableEntryIndex,
                           const uast::AstNode* symbolTableEntryAst) const {
  return loadLocationsQuery(context, this, moduleIndex,
                            symbolTableEntryIndex, symbolTableEntryAst);
}


} // end namespace libraries
} // end namespace chpl
