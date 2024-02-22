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

#include "chpl/libraries/LibraryFile.h"

#include "chpl/framework/ID.h"
#include "chpl/framework/query-impl.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Builder.h"

#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_os_ostream.h"

#ifdef HAVE_LLVM
#include "llvm/ADT/StringRef.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#endif

#include <cstring>

namespace chpl {
namespace libraries {


std::pair<size_t, const char*>
LibraryFileDeserializationHelper::getString(int id) const {
  if (0 < id && id < nStrings) {
    uint64_t offset = stringOffsetsTable[id];
    uint64_t nextOffset = stringOffsetsTable[id+1];
    return std::make_pair(nextOffset-offset,
                          (const char*) (stringSectionData + offset));
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

    case (int) uast::BuilderResult::LocationMapTag::BaseMap:
      return &astToLocation;

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

  fileLen = fileSize;
  fileData = (const unsigned char*) mappedFile->const_data();

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

  if (fd < 0 || fileLen < sizeof(FileHeader) ||
      fileData == nullptr || fileData == (unsigned char*) -1) {
    // note: mmap can return -1 as a pointer upon failure
    context->error(Location(), "Could not read file %s", libPath.c_str());
  }

  // inspect the file header
  const FileHeader* header = (const FileHeader*) fileData;

  if (header->magic != FILE_HEADER_MAGIC) {
    invalidFileError(context);
    return false;
  }

  // save the file hash
  memcpy(&fileHash[0], &header->hash[0], HASH_SIZE);

  uint32_t nModules = header->nModules;

  if (nModules >= MAX_NUM_MODULES ||
      sizeof(FileHeader) + (nModules+1)*sizeof(uint64_t) > fileLen) {
    invalidFileError(context);
    return false;
  }

  // populate modulePathToSection and moduleIdsAndFilePaths
  // module offsets are stored just after the file header
  const uint64_t* moduleOffsets =
    (const uint64_t*) (fileData + sizeof(FileHeader));
  for (uint32_t i = 0; i < nModules; i++) {
    uint64_t offset = moduleOffsets[i];
    uint64_t nextOffset = moduleOffsets[i+1];
    uint64_t modLen = nextOffset - offset;

    if (nextOffset > fileLen || nextOffset < offset) {
      invalidFileError(context);
      return false;
    }

    const ModuleHeader* mod = (const ModuleHeader*) (fileData + offset);

    if (mod->magic != MODULE_SECTION_MAGIC ||
        mod->symbolTable.start + sizeof(SymbolTableHeader) > modLen ||
        mod->symbolTable.end > modLen ||
        mod->astSection.start + sizeof(AstSectionHeader) > modLen ||
        mod->astSection.end > modLen ||
        mod->longStringsTable.start + sizeof(LongStringsTableHeader) > modLen ||
        mod->longStringsTable.end > modLen ||
        mod->locationSection.start + sizeof(LocationSectionHeader) > modLen ||
        mod->locationSection.end > modLen) {
      invalidFileError(context);
      return false;
    }

    // Read starting just after the module header
    LibraryFileDeserializationHelper helper;

    Deserializer des(context,
                     fileData + offset,
                     fileData + offset + sizeof(ModuleHeader),
                     fileData + nextOffset,
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
    info.moduleRegion = makeRegion(offset, nextOffset);

    modules.push_back(std::move(info));

    // and update the map to store the appropriate index
    moduleSymPathToIdx[info.moduleSymPath] = modules.size()-1;
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
                                    Region r, // file offsets
                                    UniqueString moduleSymPath,
                                    ModuleSection& mod) const {

  const ModuleHeader* modHdr = (const ModuleHeader*) (fileData + r.start);
  // from the module header, find the longStringsTable offset
  // and the symbol table offset
  Region symTable = modHdr->symbolTable;
  uint64_t symTableSectionLen = symTable.end - symTable.start;
  Region astSection = modHdr->astSection;
  uint64_t astSectionLen = astSection.end - astSection.start;
  Region longStrings = modHdr->longStringsTable;
  uint64_t longStringsSectionLen = longStrings.end - longStrings.start;
  Region locations = modHdr->locationSection;
  uint64_t locSectionLen = locations.end - locations.start;
  Region genCodeSection = modHdr->genCodeSection;
  uint64_t genCodeSectionLen = genCodeSection.end - genCodeSection.start;
  if (modHdr->magic != MODULE_SECTION_MAGIC ||
      r.start > fileLen || r.end > fileLen || r.start >= r.end ||
      r.start + symTable.start + sizeof(SymbolTableHeader) > r.end ||
      r.start + symTable.end > r.end ||
      r.start + astSection.start + sizeof(AstSectionHeader) > r.end ||
      r.start + astSection.end > r.end ||
      r.start + longStrings.start + sizeof(LongStringsTableHeader) > r.end ||
      r.start + longStrings.end > r.end ||
      r.start + locations.start + sizeof(LocationSectionHeader) > r.end ||
      r.start + locations.end > r.end ||
      r.start + genCodeSection.start + sizeof(LocationSectionHeader) > r.end ||
      r.start + genCodeSection.end > r.end) {
    invalidFileError(context);
    return false;
  }

  const SymbolTableHeader* symTableHeader =
    (const SymbolTableHeader*) (fileData + r.start + symTable.start);

  size_t symMinSize = symTableHeader->nEntries*sizeof(SymbolTableEntry);
  if (symTableHeader->magic != SYMBOL_TABLE_MAGIC ||
      symTable.start + symMinSize > symTable.end ||
      symTableHeader->nEntries >= MAX_NUM_SYMBOLS ) {
    invalidFileError(context);
    return false;
  }

  const AstSectionHeader* astSectionHeader =
    (const AstSectionHeader*) (fileData + r.start + astSection.start);
  if (astSectionHeader->magic != UAST_SECTION_MAGIC ||
      astSectionHeader->nEntries > MAX_NUM_ASTS) {
    invalidFileError(context);
    return false;
  }

  const LongStringsTableHeader* strTableHeader =
    (const LongStringsTableHeader*) (fileData + r.start + longStrings.start);

  size_t stringOffsetsSize = strTableHeader->nLongStrings*sizeof(uint64_t);
  if (strTableHeader->magic != LONG_STRINGS_TABLE_MAGIC ||
      longStrings.start + stringOffsetsSize > longStrings.end) {
    invalidFileError(context);
    return false;
  }

  const LocationSectionHeader* locHeader =
    (const LocationSectionHeader*) (fileData + r.start + locations.start);
  if (locHeader->magic != LOCATION_SECTION_MAGIC ||
      locHeader->nFilePaths > MAX_NUM_FILES ||
      locHeader->nGroups > MAX_NUM_SYMBOLS) {
    invalidFileError(context);
    return false;
  }

  const GenCodeSectionHeader* genHeader =
    (const GenCodeSectionHeader*) (fileData + r.start + genCodeSection.start);
  if (genHeader->magic != GEN_CODE_SECTION_MAGIC) {
    return false;
  }

  mod.symbolTableData = (const unsigned char*) symTableHeader;
  mod.symbolTableLen = symTableSectionLen;

  mod.astSectionData = (const unsigned char*) astSectionHeader;
  mod.astSectionLen = astSectionLen;

  mod.stringSectionData = (const unsigned char*) (strTableHeader);
  mod.stringSectionLen = longStringsSectionLen;

  mod.locationSectionData = (const unsigned char*) locHeader;
  mod.locationSectionLen = locSectionLen;

  mod.genCodeSectionData = (const unsigned char*) genHeader;
  mod.genCodeSectionLen = genCodeSectionLen;

  mod.nStrings = strTableHeader->nLongStrings;
  // string offsets start just after the header
  mod.stringOffsetsTable = (const uint32_t*) (strTableHeader+1);

  // LLVM IR data just after the header
  mod.llvmIrData = (const unsigned char*)(genHeader+1);
  mod.llvmIrDataLen = genCodeSectionLen - sizeof(GenCodeSectionHeader);

  LibraryFileDeserializationHelper helper = setupHelper(context, &mod);

  // also read the symbol table uast locations
  size_t hdrSize = sizeof(SymbolTableHeader);
  Deserializer des(context,
                   fileData + r.start + symTable.start,
                   fileData + r.start + symTable.start + hdrSize,
                   fileData + r.start + symTable.end,
                   helper);
  uint32_t n = symTableHeader->nEntries;
  std::string lastSymId;
  std::string lastCname;
  std::vector<UniqueString> cnames;

  for (uint32_t i = 0; i < n; i++) {
    uint64_t pos = des.position();

    // read the entry offsets
    SymbolTableEntry entry;
    des.readData(&entry, sizeof(entry));
    // read the tag
    des.readByte();

    {
      // read the variable-byte encoded common prefix length
      unsigned int nCommonPrefix = des.readVUint();
      // shorten lastSymId to the first nCommonPrefix bytes
      if (lastSymId.size() > nCommonPrefix) {
        lastSymId.erase(nCommonPrefix, lastSymId.size()-nCommonPrefix);
      }
      // read the variable-byte encoded suffix length
      unsigned int nSuffix = des.readVUint();

      if (!des.checkStringLength(nCommonPrefix+nSuffix) ||
          !des.checkStringLengthAvailable(nSuffix)) {
        invalidFileError(context);
        return false;
      }

      // expand lastSymId to have room to store the suffix
      lastSymId.resize(nCommonPrefix+nSuffix);
      // read the string data
      des.readData(&lastSymId[nCommonPrefix], nSuffix);
    }

    // consider the code-generated versions
    unsigned nGenerated = des.readVUint();
    cnames.clear();
    // read the data from each code-generated version
    for (unsigned int j = 0; j < nGenerated; j++) {
      des.readByte(); // isInstantiation

      // read the variable-byte encoded common prefix length
      unsigned int nCommonPrefix = des.readVUint();
      // shorten lastCname to the first nCommonPrefix bytes
      if (lastCname.size() > nCommonPrefix) {
        lastCname.erase(nCommonPrefix, lastCname.size()-nCommonPrefix);
      }
      // read the variable-byte encoded suffix length
      unsigned int nSuffix = des.readVUint();

      if (!des.checkStringLength(nCommonPrefix+nSuffix) ||
          !des.checkStringLengthAvailable(nSuffix)) {
        invalidFileError(context);
        return false;
      }

      // expand lastCname to have room to store the suffix
      lastCname.resize(nCommonPrefix+nSuffix);
      // read the string data
      des.readData(&lastCname[nCommonPrefix], nSuffix);
      // note the cname
      cnames.push_back(UniqueString::get(context, lastCname));
    }

    // record the information
    ModuleSection::SymbolInfo info;
    info.symbolEntryOffset = pos;
    info.astOffset = entry.astEntry;
    info.locationOffset = entry.locationEntry;
    {
      std::string fullSymPath = moduleSymPath.str();
      if (!lastSymId.empty()) {
        fullSymPath += "." + lastSymId;
      }
      info.symbolPath = UniqueString::get(context, fullSymPath);
    }
    info.cnames = cnames;
    mod.symbols.push_back(info);
    mod.offsetToSymIdx[info.astOffset] = i;


    // check that the deserializer did not encounter an error
    // check that the SymbolTableEntry offsets are realistic
    if (!des.ok() ||
        info.symbolEntryOffset > symTableSectionLen ||
        info.astOffset > astSectionLen ||
        info.locationOffset > locSectionLen) {
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
    Region moduleRegion = info.moduleRegion;
    result = toOwned(new ModuleSection());
    bool ok = f->readModuleSection(context, moduleRegion, info.moduleSymPath,
                                   *result.get());
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
  ret.stringSectionData = mod->stringSectionData;
  ret.stringSectionLen = mod->stringSectionLen;
  ret.stringOffsetsTable = mod->stringOffsetsTable;

  return ret;
}

bool LibraryFile::readModuleAst(Context* context,
                                Region mod,
                                LibraryFileDeserializationHelper& helper,
                                uast::Builder& builder) const {
  const ModuleHeader* modHdr = (const ModuleHeader*) (fileData + mod.start);

  if (modHdr->magic != MODULE_SECTION_MAGIC) {
    invalidFileError(context);
    return false;
  }

  // read the uast
  Region ast = modHdr->astSection;
  if (mod.start + ast.start + sizeof(AstSectionHeader) > mod.end ||
      mod.start + ast.end > mod.end) {
    invalidFileError(context);
    return false;
  }

  const AstSectionHeader* astHdr =
    (const AstSectionHeader*) (fileData + mod.start + ast.start);

  if (astHdr->magic != UAST_SECTION_MAGIC ||
      astHdr->nEntries > MAX_NUM_ASTS) {
    invalidFileError(context);
    return false;
  }

  size_t pos = sizeof(AstSectionHeader);
  auto des = Deserializer(context,
                          fileData + mod.start + ast.start,
                          fileData + mod.start + ast.start + pos,
                          fileData + mod.start + ast.end,
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

  auto builder = uast::Builder::createForLibraryFileModule(context,
                                                           f->libPath,
                                                           parentSymbolPath,
                                                           f);

  uast::Builder::SymbolTableVec combined;

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
      ok = f->readModuleAst(context, modInfo.moduleRegion, helper, *builder);
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

  builder->noteSymbolTableSymbols(std::move(combined));

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

  if (fileLen != other.fileLen) {
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

void LibraryFile::summarize(Context* context, std::ostream& s) const {
  s << "# Summary of library file " << libPath.str() << "\n";

  const FileHeader* fileHeader = (const FileHeader*) fileData;
  if (fileHeader->magic != FILE_HEADER_MAGIC)
    return;

  s << "  file format version = "
       << fileHeader->fileFormatVersionMajor
       << "." << fileHeader->fileFormatVersionMinor << "\n";
  s << "  chpl version = "
       << fileHeader->chplVersionMajor
       << "." << fileHeader->chplVersionMinor
       << "." << fileHeader->chplVersionUpdate << "\n";
  s << "  nModules = " << fileHeader->nModules << "\n";
  s << "  file hash = " << llvm::toHex(fileHash) << "\n";
  s << "  file size = " << fileLen << "\n";

  s << "\n";

  int i = 0;
  for (auto modInfo : modules) {
    s << "  ## module " << modInfo.moduleSymPath.str() << "\n";
    s << "     path = " << modInfo.sourceFilePath.str() << "\n";
    Region modRegion = modInfo.moduleRegion;
    s << "     size = " << modRegion.end - modRegion.start << "\n";

    const ModuleSection* mod = loadModuleSection(context, i);
    if (mod) {

#ifdef HAVE_LLVM
      owned<llvm::Module> llvmMod =
        loadGenCodeModule(context, modInfo.moduleSymPath);
#endif

      s << "     ### symbols\n";

      for (const auto& symInfo : mod->symbols) {
        UniqueString symPath = symInfo.symbolPath;
        s << "         " << symPath.str();
        for (auto cname : symInfo.cnames) {
          s << " " << cname;;
#ifdef HAVE_LLVM
          if (!cname.isEmpty() && llvmMod->getFunction(cname.str())) {
            s << "[llvm]";
          }
#endif
        }
        s << "\n";
      }


#ifdef HAVE_LLVM
      s << "     ### LLVM IR\n";
      if (llvmMod.get()) {
        auto err = llvmMod->materializeAll();
        if (err) {
          CHPL_ASSERT(false && "Failure to materialize a module");
        }
        auto llvmStream = llvm::raw_os_ostream(s);
        llvmMod->print(llvmStream, /*AssemblyAnnotationWriter*/nullptr);
      }
#endif
    }

    s << "\n";
    i++;
  }
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
  auto search = moduleSymPathToIdx.find(moduleSymPath);
  if (search != moduleSymPathToIdx.end()) {
    size_t moduleIdx = search->second;
    if (0 <= moduleIdx && moduleIdx < modules.size()) {
      sourceFilePath = modules[moduleIdx].sourceFilePath;
    } else {
      CHPL_ASSERT(false && "moduleSymPathToIdx has out of bounds index");
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
  const LocationSectionHeader* locHdr =
    (const LocationSectionHeader*) m->locationSectionData;

  // read the location group paths
  auto helper = setupHelper(context, m);
  Deserializer des(context,
                   m->locationSectionData,
                   m->locationSectionData + sizeof(LocationSectionHeader),
                   m->locationSectionData + m->locationSectionLen,
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

static int zeroNegToNegOne(unsigned int arg) {
  if (arg <= 0) {
    return -1;
  }

  return arg;
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
  int firstLine = zeroNegToNegOne(lastEntryLastLine + firstLineDiff);

  int lastLineDiff = des.readVInt();
  int lastLine = zeroNegToNegOne(firstLine + lastLineDiff);

  int firstCol = zeroNegToNegOne(des.readVUint());
  int lastCol = zeroNegToNegOne(des.readVUint());

  maps.astToLocation[cur] = Location(path, firstLine, firstCol,
                                     lastLine, lastCol);

  // read the additional locations
  unsigned int nAddnlLocs = des.readVUint();
  for (unsigned int i = 0; i < nAddnlLocs; i++) {
    int tag = des.readVInt(); // tag
    std::unordered_map<const uast::AstNode*, Location>* aMap =
      maps.getLocationMap(tag);
    int otherFirstLineDiff = des.readVInt();
    int otherFirstLine = zeroNegToNegOne(firstLine + otherFirstLineDiff);
    int otherLastLineDiff = des.readVInt();
    int otherLastLine = zeroNegToNegOne(otherFirstLine + otherLastLineDiff);
    int otherFirstCol = zeroNegToNegOne(des.readVUint());
    int otherLastCol = zeroNegToNegOne(des.readVUint());
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

bool LibraryFile::doLoadLocations(Context* context,
                                  int moduleIndex,
                                  int symbolTableEntryIndex,
                                  const uast::AstNode* symbolTableEntryAst,
                                  LocationMaps& result) const {
  if (moduleIndex < 0 || (size_t) moduleIndex >= modules.size()) {
    invalidFileError(context);
    return false;
  }

  UniqueString fromSourcePath = modules[moduleIndex].sourceFilePath;
  const uast::BuilderResult& br = loadSourceAst(context, fromSourcePath);
  const ModuleSection* m = loadModuleSection(context, moduleIndex);
  if (m == nullptr) {
    return false;
  }

  if (symbolTableEntryIndex < 0 ||
      (size_t) symbolTableEntryIndex >= m->symbols.size()) {
    invalidFileError(context);
    return false;
  }

  std::vector<UniqueString> paths;
  bool ok = readLocationPaths(context, paths, m);
  if (!ok) {
    return false;
  }

  size_t locationGroupOffset = m->symbols[symbolTableEntryIndex].locationOffset;
  if (locationGroupOffset >= m->locationSectionLen) {
    invalidFileError(context);
    return false;
  }

  auto helper = setupHelper(context, m);

  Deserializer des(context,
                   m->locationSectionData,
                   m->locationSectionData + locationGroupOffset,
                   m->locationSectionData + m->locationSectionLen,
                   helper);
  ok = readLocationGroup(context, result, des, symbolTableEntryAst, paths, br);
  if (!ok) {
    return false;
  }

  // also check for any error during deserialization
  if (!des.ok()) {
    invalidFileError(context);
    return false;
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

  bool ok = f->doLoadLocations(context, moduleIndex,
                               symbolTableEntryIndex, symbolTableEntryAst,
                               result);
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

#ifdef HAVE_LLVM
owned<llvm::Module>
LibraryFile::loadLlvmModuleImpl(Context* context,
                                const LibraryFile* f,
                                int moduleIndex) {
  owned<llvm::Module> result;

  if (0 <= moduleIndex && (size_t) moduleIndex < f->modules.size()) {
    const ModuleSection* ms = f->loadModuleSection(context, moduleIndex);
    if (ms != nullptr && ms->llvmIrData != nullptr && ms->llvmIrDataLen != 0) {
      auto inputData = llvm::StringRef((const char*)ms->llvmIrData,
                                       ms->llvmIrDataLen);
      std::string bufferName =
        f->libPath.str() + "-" +
        f->modules[moduleIndex].moduleSymPath.str() + "-llvm";
      bool reqNullTerm = false; /* RequiresNullTerminator */;
      auto ownedMemBuf =
        llvm::MemoryBuffer::getMemBuffer(inputData, bufferName, reqNullTerm);
      llvm::SMDiagnostic err;

      result =
        llvm::getLazyIRModule(std::move(ownedMemBuf),
                              err,
                              context->llvmContext(),
                              /*ShouldLazyLoadMetadata*/ false);
    }
  }

  return result;
}

owned<llvm::Module>
LibraryFile::loadGenCodeModule(Context* context,
                               UniqueString moduleSymPath) const {
  auto search = moduleSymPathToIdx.find(moduleSymPath);
  if (search != moduleSymPathToIdx.end()) {
    size_t moduleIdx = search->second;
    if (0 <= moduleIdx && moduleIdx < modules.size()) {
      // Load a copy of the LLVM IR
      return loadLlvmModuleImpl(context, this, moduleIdx);
    } else {
      CHPL_ASSERT(false && "moduleSymPathToIdx has out of bounds index");
    }
  }

  return nullptr;
}
#endif


} // end namespace libraries
} // end namespace chpl
