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

#include "chpl/libraries/LibraryFileWriter.h"

#include "chpl/libraries/LibraryFileFormat.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/all-uast.h"
#include "chpl/util/filesystem.h"
#include "chpl/util/version-info.h"

namespace chpl {
namespace libraries {

void LibraryFileSerializationHelper::beginAst(const uast::AstNode* ast,
                                              std::ostream& os) {
  if (symbolTableSet.count(ast) != 0) {
    uint64_t pos = os.tellp();
    astOffsets[ast] = (uint32_t) (pos - astSectionStartFileOffset);
  }
}

void LibraryFileSerializationHelper::endAst(const uast::AstNode* ast,
                                            std::ostream& os) {
  astCounter++;
}

void
LibraryFileSerializationHelper::noteSymbolForTable(const uast::AstNode* ast) {
  auto pair = symbolTableSet.insert(ast);
  if (pair.second) {
    // it was inserted
    symbolTableVec.push_back(ast);
  }
}

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

void LibraryFileWriter::fail(const char* msg) {
  context->error(Location(), "%s %s", msg, outputFilePath.c_str());
  ok = false;
}

std::vector<LibraryFileWriter::ModInfo>
LibraryFileWriter::gatherTopLevelModules(Context* context,
                                         std::vector<UniqueString> paths) {
  std::vector<ModInfo> modules;
  // Parse the paths and gather a vector of top-level modules.
  for (auto path : paths) {
    path = cleanLocalPath(context, path);
    UniqueString empty;
    std::vector<const uast::Module*> modsInFile =
      parsing::parse(context, path, empty);
    for (auto mod : modsInFile) {
      ModInfo info;
      info.moduleName = mod->name();
      info.moduleAst = mod;
      info.fromSourcePath = path;
      modules.push_back(std::move(info));
    }
  }
  return modules;
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
  header.nModules = modules.size();
  // hash remains 0s at this point
  fileStream.write((const char*) &header, sizeof(header));

  // emit an error if there are too many modules for the file format
  if (modules.size() >= MAX_NUM_MODULES) {
    fail("Too many modules to create library file");
  }

  // write the placeholder module section table,
  // including the offset just after the last module
  size_t n = modules.size();
  for (size_t i = 0; i <= n; i++) {
    uint64_t zero = 0;
    fileStream.write((const char*) &zero, sizeof(zero));
  }
}

void LibraryFileWriter::padToAlign() {
  for (int i = 0; i < 16; i++) {
    auto pos = fileStream.tellp();
    if (pos % 16 == 0) break;
    fileStream.put(0);
  }
}

Region LibraryFileWriter::writeModuleSection(const ModInfo& info) {
  const uast::Module* mod = info.moduleAst;
  UniqueString fromFilePath = info.fromSourcePath;
  const std::string& genCode = info.genCode;

  auto moduleSectionStart = fileStream.tellp();

  // Construct a module section header and write it
  ModuleHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = MODULE_SECTION_MAGIC;
  fileStream.write((const char*) &header, sizeof(header));

  LibraryFileSerializationHelper reg;
  reg.moduleSectionStart = moduleSectionStart;

  // create a serializer to write the uAST
  auto ser = Serializer(fileStream, &reg);

  // write the module symbol path
  ser.write<std::string>(mod->id().symbolPath().str());
  // write the module's file path
  ser.write<std::string>(fromFilePath.str());

  // compute the symbol table entries for this module & clear old ones
  noteToplevelSymbolsForTable(mod, reg);

  // write the various sections
  padToAlign();
  reg.astSectionStartFileOffset = fileStream.tellp();
  header.astSection = writeAst(mod, ser, reg);

  padToAlign();
  header.longStringsTable = writeLongStrings(moduleSectionStart, ser);

  padToAlign();
  reg.locationSectionStartFileOffset = fileStream.tellp();
  header.locationSection = writeLocations(mod, ser, reg);
  // note: implementation of writeLocations assumes it runs after writeAst

  padToAlign();
  header.symbolTable = writeSymbolTable(info, ser, reg);
  // note: implementation of writeSymbolTable assumes it runs
  // after writeAst and writeLocations

  padToAlign();
  header.genCodeSection = writeGenCode(moduleSectionStart, ser, genCode);

  // update the module header with the saved locations by writing the
  // header again.
  auto savePos = fileStream.tellp();

  // TODO: make this into a user-facing error if this situation
  // can come up in practice.
  CHPL_ASSERT(savePos - moduleSectionStart < INT32_MAX);

  fileStream.seekp(moduleSectionStart);
  fileStream.write((const char*) &header, sizeof(header));

  // seek back where we were
  fileStream.seekp(savePos);

  if (!ser.ok()) {
    fail("Error serializing for library file");
  }

  auto moduleSectionEnd = fileStream.tellp();

  return makeRegion(moduleSectionStart, moduleSectionEnd);
}

// Helper to gather top-level symbols, but also find primary methods.
// See also GatherDecls in the scope resolver.
struct GatherToplevelSymbols {
  std::vector<const uast::AstNode*> result;

  GatherToplevelSymbols() { }

  // Add NamedDecls to the map
  bool enter(const uast::NamedDecl* d) {
    if (d->isRecord() && d->name() == USTR("_tuple")) {
      // skip gathering _tuple from the standard library
      // since dyno handles tuple types directly rather
      // than through a record.
      return false;
    }

    result.push_back(d);

    // Traverse into records, classes, unions, interfaces
    // to find primary methods/fields.
    bool traverse = false;
    if (d->isAggregateDecl() || d->isInterface()) {
      traverse = true;
    }

    return traverse;
  }

  void exit(const uast::NamedDecl* d) { }

  // Traverse into TupleDecl and MultiDecl looking for NamedDecls
  bool enter(const uast::TupleDecl* d) {
    return true;
  }
  void exit(const uast::TupleDecl* d) { }

  bool enter(const uast::MultiDecl* d) {
    return true;
  }
  void exit(const uast::MultiDecl* d) { }

  // consider 'include module' something that defines a name
  bool enter(const uast::Include* d) {
    result.push_back(d);
    return false;
  }
  void exit(const uast::Include* d) { }

  // ignore other AST nodes
  bool enter(const uast::AstNode* ast) {
    return false;
  }
  void exit(const uast::AstNode* ast) { }
};

void LibraryFileWriter::noteToplevelSymbolsForTable(
                                        const uast::Module* mod,
                                        LibraryFileSerializationHelper& reg) {
  reg.noteSymbolForTable(mod);

  GatherToplevelSymbols g;

  for (auto child : mod->children()) {
    child->traverse(g);
  }

  for (auto ast : g.result) {
    reg.noteSymbolForTable(ast);
  }
}

static bool
symbolIdLess(const std::pair<const uast::AstNode*, std::string>& a,
             const std::pair<const uast::AstNode*, std::string>& b) {
  // Should be sorting by ID minus the top-level symbol name,
  // but that will be the same for all, so we can sort using the
  // ID's symbol path.
  return a.second < b.second;
}

static std::vector<std::pair<const uast::AstNode*, std::string>>
computeSymbolNames(const uast::Module* mod,
                   const std::vector<const uast::AstNode*>& symbolTableVec) {

  std::string modPrefix = mod->id().symbolPath().str();

  std::vector<std::pair<const uast::AstNode*, std::string>> result;
  for (auto sym: symbolTableVec) {
    // compute the symbol table ID
    UniqueString symPath = sym->id().symbolPath();
    CHPL_ASSERT(symPath.startsWith(modPrefix));
    std::string symId = sym->id().symbolPath().str();
    // remove the modPrefix from it
    symId.erase(0, modPrefix.size());
    // remove a leading '.'
    if (symId[0] == '.') {
      symId.erase(0, 1);
    }
    // for Variables, they do not get the name in the regular ID,
    // so adjust the symbol ID path to include the variable name
    if (!uast::Builder::astTagIndicatesNewIdScope(sym->tag())) {
      UniqueString name;
      if (auto nd = sym->toNamedDecl()) {
        name = nd->name();
      } else if (auto inc = sym->toInclude()) {
        name = inc->name();
      } else {
        CHPL_ASSERT(false);
      }
      if (!symId.empty()) {
        symId.append(".");
      }
      symId.append(name.str());
    }
    // store the symbol and name in the result vector
    result.emplace_back(sym, std::move(symId));
  }

  std::sort(result.begin(), result.end(), symbolIdLess);

  return result;
}

static unsigned int computeCommonPrefix(const std::string& last,
                                        const std::string& str) {
  unsigned int nCommonPrefix = 0;

  // compute the minimum of the lengths
  size_t n = last.size();
  if (str.size() < n) n = str.size();

  for (nCommonPrefix = 0; nCommonPrefix < n; nCommonPrefix++) {
    if (last[nCommonPrefix] != str[nCommonPrefix]) {
      break;
    }
  }

  return nCommonPrefix;
}

Region
LibraryFileWriter::writeSymbolTable(const ModInfo& info,
                                    Serializer& ser,
                                    LibraryFileSerializationHelper& reg) {

  const uast::Module* mod = info.moduleAst;

  uint64_t symTableStart = fileStream.tellp();
  SymbolTableHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = SYMBOL_TABLE_MAGIC;
  header.nEntries = reg.symbolTableVec.size();
  ser.writeData(&header, sizeof(header));

  // create an error if there are too many symbols
  ser.checkStringLength(reg.symbolTableVec.size(), MAX_NUM_SYMBOLS);

  // output each symbol table entry
  // assumption: symbolTableUpdates has already been updated with the
  // uast and location section offsets

  // Copy the vector of symbol table symbols and sort it by ID/name
  auto symsAndNames = computeSymbolNames(mod, reg.symbolTableVec);

  std::string lastSymId;
  std::string lastCname;

  for (auto pair : symsAndNames) {
    const uast::AstNode* sym = pair.first;
    const std::string& symId = pair.second;

    // create an error if the string length is too long
    ser.checkStringLength(symId.size());

    SymbolTableEntry entry;
    memset(&entry, 0, sizeof(entry));
    entry.astEntry = reg.astOffsets[sym];
    entry.locationEntry = reg.locOffsets[sym];

    // write the 3 relative offsets
    ser.writeData(&entry, sizeof(entry));

    // write the tag / flags / kind information
    auto tag = sym->tag();
    CHPL_ASSERT((int) tag < 256);
    ser.writeByte(tag);

    {
      //  * write the number of bytes in common with the previous one
      unsigned int nCommonPrefix = computeCommonPrefix(lastSymId, symId);
      ser.writeVUint(nCommonPrefix);
      //  * write the number of bytes of suffix
      unsigned int nSuffix = symId.size() - nCommonPrefix;
      ser.writeVUint(nSuffix);
      //  * write the suffix data
      ser.writeData(&symId[nCommonPrefix], nSuffix);
    }

    // write the number of generated versions
    auto search = info.genMap.find(sym->id());
    if (search != info.genMap.end()) {
      const std::vector<GenInfo>& gens = search->second;
      ser.writeVUint(gens.size()); // number of generated versions
      for (const auto& g : gens) {
        // output the byte indicating if it is an instantiation
        ser.writeByte(g.isInstantiation);
        // output the cname
        std::string cname = g.cname.str();
        // write the number of bytes in common with the previous one
        unsigned int nCommonPrefix = computeCommonPrefix(lastCname, cname);
        ser.writeVUint(nCommonPrefix);
        // write the number of bytes of suffix
        unsigned int nSuffix = cname.size() - nCommonPrefix;
        ser.writeVUint(nSuffix);
        //  * write the suffix data
        ser.writeData(&cname[nCommonPrefix], nSuffix);
      }
    } else {
      ser.writeVUint(0); // zero generated versions
    }

    lastSymId = symId;
  }

  uint64_t symTableEnd = fileStream.tellp();

  return makeRegion(symTableStart - reg.moduleSectionStart,
                    symTableEnd - reg.moduleSectionStart);
}

Region LibraryFileWriter::writeAst(const uast::Module* mod,
                                   Serializer& ser,
                                   LibraryFileSerializationHelper& reg) {
  uint64_t astStart = fileStream.tellp();
  AstSectionHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = UAST_SECTION_MAGIC;
  fileStream.write((const char*) &header, sizeof(header));

  // serialize the data
  mod->serialize(ser);
  header.nEntries = reg.astCounter;
  ser.checkStringLength(reg.astCounter, MAX_NUM_ASTS);

  // update the number serialized by rewriting the header
  auto savePos = fileStream.tellp();

  fileStream.seekp(astStart);
  fileStream.write((const char*) &header, sizeof(header));

  // seek back where we were
  fileStream.seekp(savePos);

  uint64_t astEnd = fileStream.tellp();
  return makeRegion(astStart - reg.moduleSectionStart,
                    astEnd - reg.moduleSectionStart);
}

Region LibraryFileWriter::writeLongStrings(uint64_t moduleSectionStart,
                                           Serializer& ser) {
  uint64_t longStringsStart = fileStream.tellp();

  LongStringsTableHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = LONG_STRINGS_TABLE_MAGIC;
  // nextStringIdx counts strings from 1, but we also want
  // to have an extra string (so we can compute the length of the last string)
  header.nLongStrings = ser.nextStringIdx()+1;

  fileStream.write((const char*) &header, sizeof(header));

  size_t n = header.nLongStrings;

  std::vector<uint32_t> offsets;
  offsets.resize(n);
  // write 0s for the offsets. We will rewrite it in a moment.
  fileStream.write((const char*) &offsets[0], n*sizeof(uint32_t));

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

    // write the offset where this string data starts
    uint64_t pos = fileStream.tellp();
    offsets[i] = pos - longStringsStart;
    // if there is any string data, write it
    if (ptr != nullptr && len > 0) {
      fileStream.write(ptr, len);
    }
  }

  // update the offsets in the file
  auto savePos = fileStream.tellp();

  // update the offsets in the file
  fileStream.seekp(longStringsStart+sizeof(header));

  // write the actual offsets to replace the dummy offsets written earlier
  fileStream.write((const char*) &offsets[0], n*sizeof(uint32_t));

  // seek back where we were
  fileStream.seekp(savePos);

  uint64_t longStringsEnd = fileStream.tellp();

  return makeRegion(longStringsStart - moduleSectionStart,
                    longStringsEnd - moduleSectionStart);
}

Region LibraryFileWriter::writeLocations(const uast::Module* mod,
                                         Serializer& ser,
                                         LibraryFileSerializationHelper& reg) {
  uint64_t locationsSectionStart = fileStream.tellp();

  LocationSectionHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = LOCATION_SECTION_MAGIC;
  header.nFilePaths = inputFiles.size();
  header.nGroups = reg.symbolTableVec.size();
  // the initial entry in symbolTableVec should store the module
  CHPL_ASSERT(reg.symbolTableVec.size() > 0 && reg.symbolTableVec[0] == mod);

  // create an error if there are too many symbols / location groups
  ser.checkStringLength(inputFiles.size(), MAX_NUM_FILES);
  ser.checkStringLength(reg.symbolTableVec.size(), MAX_NUM_SYMBOLS);

  fileStream.write((const char*) &header, sizeof(header));

  // compute the file hashes
  std::vector<HashFileResult> hashes;
  for (auto path : inputFiles) {
    HashFileResult hash;
    parsing::FileContents fileContents = parsing::fileText(context, path);
    if (fileContents.error() == nullptr) {
      hash = hashString(fileContents.text());
    } else {
      hash.fill(0);
    }

    hashes.push_back(hash);
  }

  // compute pathToIdx
  PathToIndex pathToIdx;
  size_t n = inputFiles.size();
  for (size_t i = 0; i < n; i++) {
    pathToIdx[inputFiles[i]] = i;
  }

  // for each file path, write
  //   * string storing file path
  //   * a file hash

  n = inputFiles.size();
  for (size_t i = 0; i < n; i++) {
    std::string path = inputFiles[i].str();
    HashFileResult hash = hashes[i];
    ser.write(path);
    ser.writeData(&hash, sizeof(hash));
  }

  // write the locations group for each top-level symbol,
  // including the module itself
  n = header.nGroups;
  for (size_t i = 0; i < n; i++) {
    writeLocationGroup(reg.symbolTableVec[i], ser, reg, pathToIdx);
  }

  uint64_t locationsSectionEnd = fileStream.tellp();
  return makeRegion(locationsSectionStart - reg.moduleSectionStart,
                    locationsSectionEnd - reg.moduleSectionStart);
}

Region
LibraryFileWriter::writeLocationGroup(const uast::AstNode* ast,
                                      Serializer& ser,
                                      LibraryFileSerializationHelper& reg,
                                      const PathToIndex& pathToIdx) {
  uint64_t groupStart = fileStream.tellp();

  // update the location entry since we are creating it
  reg.locOffsets[ast] = groupStart - reg.locationSectionStartFileOffset;

  // compute the starting Location
  Location startingLoc = parsing::locateAst(context, ast);
  CHPL_ASSERT(pathToIdx.count(startingLoc.path()) > 0);

  // compute the file path index
  unsigned int filePathIdx = 0;
  {
    auto search = pathToIdx.find(startingLoc.path());
    if (search != pathToIdx.end()) {
      filePathIdx = search->second;
    }
  }

  // write the group header
  int startingLine = startingLoc.firstLine();
  ser.writeVUint(filePathIdx);
  ser.writeVInt(startingLine);

  // write the location entries
  int lastLine = startingLine;
  writeLocationEntries(ast, ser, reg, lastLine);

  uint64_t groupEnd = fileStream.tellp();
  return makeRegion(groupStart - reg.moduleSectionStart,
                    groupEnd - reg.moduleSectionStart);
}

static unsigned int negToZero(int arg) {
  if (arg < 0) {
    return 0;
  }

  return arg;
}

void
LibraryFileWriter::writeLocationEntries(const uast::AstNode* ast,
                                        Serializer& ser,
                                        LibraryFileSerializationHelper& reg,
                                        int& lastLine) {
  Location entryLoc = parsing::locateAst(context, ast);

  // first line, stored as a signed difference
  int firstLineDiff = entryLoc.firstLine() - lastLine;
  ser.writeVInt(firstLineDiff);

  // last line, stored as a signed difference from the first line
  int lastLineDiff = entryLoc.lastLine() - entryLoc.firstLine();
  ser.writeVInt(lastLineDiff);

  // first column
  ser.writeVUint(negToZero(entryLoc.firstColumn()));

  // last column
  ser.writeVUint(negToZero(entryLoc.lastColumn()));

  // compute additional locations
  using LocationMapTag = uast::BuilderResult::LocationMapTag;
  std::vector<std::pair<LocationMapTag, Location>> additionalLocations;
  #define LOCATION_MAP(ast__, location__) \
    if (auto x = ast->to##ast__()) { \
      additionalLocations.emplace_back( \
          uast::BuilderResult::LocationMapTag::location__, \
          parsing::locate##location__##WithAst(context, x)); \
    }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP

  // write additional locations
  ser.writeVUint(additionalLocations.size());
  for (const auto& pair : additionalLocations) {
    LocationMapTag tag = pair.first;
    const Location& otherLoc = pair.second;
    // store the location tag
    // (just the 1-based index into LOCATION_MAP entries)
    ser.writeVInt((int)tag);
    // store first line
    int otherFirstLineDiff = otherLoc.firstLine() - entryLoc.firstLine();
    ser.writeVInt(otherFirstLineDiff);
    // store the last line
    int otherLastLineDiff = otherLoc.lastLine() - otherLoc.firstLine();
    ser.writeVInt(otherLastLineDiff);
    // store the first column
    ser.writeVUint(negToZero(otherLoc.firstColumn()));
    // store the last column
    ser.writeVUint(negToZero(otherLoc.lastColumn()));
  }

  // update lastLine
  lastLine = entryLoc.lastLine();

  // consider child nodes, but stop on ones that get their own location group
  for (auto child: ast->children()) {
    if (reg.symbolTableSet.count(child) > 0) {
      // it's in the symbol table, so don't write locations for it now;
      // it will be handled in a different locations group
    } else {
      writeLocationEntries(child, ser, reg, lastLine);
    }
  }
}


Region LibraryFileWriter::writeGenCode(uint64_t moduleSectionStart,
                                       Serializer& ser,
                                       const std::string& gen) {
  uint64_t genSectionStart = fileStream.tellp();

  GenCodeSectionHeader header;
  memset(&header, 0, sizeof(header));
  header.magic = GEN_CODE_SECTION_MAGIC;
  header.len = gen.size();

  // write the header
  ser.writeData(&header, sizeof(header));

  // write the gen code data
  ser.writeData(&gen[0], gen.size());

  uint64_t genSectionEnd = fileStream.tellp();
  return makeRegion(genSectionStart - moduleSectionStart,
                    genSectionEnd - moduleSectionStart);
}

void LibraryFileWriter::setSourcePaths(std::vector<UniqueString> paths) {
  inputFiles = paths;
  modules = gatherTopLevelModules(context, inputFiles);
}

void LibraryFileWriter::setGeneratedCode(
                         UniqueString modName,
                         std::string buffer,
                         std::unordered_map<ID, std::vector<GenInfo>> genMap) {
  bool handled = false;
  for (auto& info : modules) {
    if (info.moduleName == modName) {
      info.genCode.swap(buffer);
      info.genMap.swap(genMap);
      CHPL_ASSERT(!handled && "two modules with same name?");
      handled = true;
    }
  }
  CHPL_ASSERT(handled && "did you forget to call setSourcePaths?");
}

bool LibraryFileWriter::writeAllSections() {
  openFile();
  writeHeader();

  std::vector<uint64_t> moduleSectionOffsets;
  Region moduleRegion;
  memset(&moduleRegion, 0, sizeof(moduleRegion));

  for (auto& info : modules) {
    // write the module section & update the header's table
    padToAlign();
    moduleRegion = writeModuleSection(info);
    moduleSectionOffsets.push_back(moduleRegion.start);
  }
  // and the offset just after the last module
  moduleSectionOffsets.push_back(moduleRegion.end);
  CHPL_ASSERT(moduleSectionOffsets.size() == modules.size()+1);

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
  bool streamOk = fileStream.good();
  fileStream.close();

  if (!streamOk) {
    fail("error writing to file");
  }

  return ok;
}

std::vector<UniqueString>
LibraryFileWriter::gatherTopLevelModuleNames(Context* context,
                                             std::vector<UniqueString> paths) {
  // TODO: Should this be a query / be supported by a query?
  std::vector<UniqueString> ret;
  auto v = gatherTopLevelModules(context, paths);
  for (auto& info : v) {
    ret.push_back(info.moduleName);
  }
  return ret;
}


} // end namespace libraries
} // end namespace chpl
