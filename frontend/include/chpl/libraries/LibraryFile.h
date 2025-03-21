/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_LIBRARIES_LIBRARY_FILE_H
#define CHPL_LIBRARIES_LIBRARY_FILE_H

#include "chpl/framework/UniqueString.h"
#include "chpl/libraries/LibraryFileFormat.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/util/memory.h"

#include <sstream>
#include <system_error>
#include <unordered_map>

#ifdef HAVE_LLVM
#include "llvm/IR/Module.h"
#endif

// forward declare LLVM Support Library dependencies
namespace llvm {
  class Module;

  namespace sys {
    namespace fs {
      class mapped_file_region;
    }
  }
}

namespace chpl {
// forward declarations
class Context;
class Deserializer;
namespace uast {
  class Module;
}
namespace libraries {
class LibraryFile;

/** Helper object to interact with the Deserializer to:
      * read long strings from a LibraryFile strings table
      * register deserialized AstNodes for symbol table objects
 */
class LibraryFileDeserializationHelper {
 friend class LibraryFile;
 private:
  // key: module-section-relative offset for the serialized uast for
  //      a symbol table symbol
  // value: symbol table index of that symbol
  // This will is computed from reading the symbol table.
  // A copy is stored here as a performance optimization.
  std::unordered_map<uint32_t, int> offsetToSymIdx;

  // To support deserializing UniqueStrings
  // This will be computed from reading the long strings section header.
  // A copy is stored here as a performance optimization.
  int nStrings = 0;
  const unsigned char* stringSectionData = nullptr;
  size_t stringSectionLen = 0;
  const uint32_t* stringOffsetsTable = nullptr;

  // for AstNodes in the symbol table that were deserialized
  // maps to the symbol table index of that symbol.
  // This will be computed during the deserialization process.
  std::unordered_map<const uast::AstNode*, int> astToSymIdx;

  LibraryFileDeserializationHelper() { }

 public:
  /**
    Given a long string index, returns the string size and the string data. The
    string won't necessarily be null-terminated. Returns a 0-length string if
    the ID is out of bounds.
   */
  std::pair<size_t, const char*> getString(int id) const;

  /**
    When deserializing an AstNode, track some of the uast nodes to
    be able to map them back to symbol table id. */
  void registerAst(const uast::AstNode* ast, uint64_t startOffset);
};

/** For reading a .dyno LibraryFile.
    Some data is read from the file on-demand.
    Uses `mmap` to keep the code simple and to support random-access well. */
class LibraryFile {
 public:
  /**
     Helper object to represent a loaded Symbol Table from a module section.
   */
  class ModuleSection {
   friend class LibraryFile;
   private:
    struct SymbolInfo {
      // relative to the respective sections
      uint32_t symbolEntryOffset = 0;
      uint32_t astOffset = 0;
      uint32_t locationOffset = 0;
      UniqueString symbolPath;
      std::vector<UniqueString> cnames;
      bool operator==(const SymbolInfo& other) const {
        return symbolEntryOffset == other.symbolEntryOffset &&
               astOffset == other.astOffset &&
               locationOffset == other.locationOffset &&
               symbolPath == other.symbolPath &&
               cnames == other.cnames;
      }
      bool operator!=(const SymbolInfo& other) const {
        return !(*this == other);
      }
    };

    // maps from symbol table index to SymbolInfo which contains some
    // useful offsets.
    // This will be computed from reading the symbol table.
    std::vector<SymbolInfo> symbols;

    // key: uast-section-relative offset for the serialized uast for
    //      a symbol table symbol
    // value: symbol table index of that symbol
    // This is computed by reading the symbol table.
    std::unordered_map<uint32_t, int> offsetToSymIdx;

    const unsigned char* symbolTableData = nullptr;
    size_t symbolTableLen = 0;

    const unsigned char* astSectionData = nullptr;
    size_t astSectionLen = 0;

    const unsigned char* stringSectionData = nullptr;
    size_t stringSectionLen = 0;

    const unsigned char* locationSectionData = nullptr;
    size_t locationSectionLen = 0;

    const unsigned char* genCodeSectionData = nullptr;
    size_t genCodeSectionLen = 0;

    // To support deserializing UniqueStrings
    int nStrings = 0;
    const uint32_t* stringOffsetsTable = nullptr;

    // To support reading LLVM IR
    const unsigned char* llvmIrData = nullptr;
    size_t llvmIrDataLen = 0;

    ModuleSection() { }
   public:
    bool operator==(const ModuleSection& other) const {
      return symbols == other.symbols &&
             offsetToSymIdx == other.offsetToSymIdx &&
             symbolTableData == other.symbolTableData &&
             symbolTableLen == other.symbolTableLen &&
             astSectionData == other.astSectionData &&
             astSectionLen == other.astSectionLen &&
             stringSectionData == other.stringSectionData &&
             stringSectionLen == other.stringSectionLen &&
             locationSectionData == other.locationSectionData &&
             locationSectionLen == other.locationSectionLen &&
             genCodeSectionData == other.genCodeSectionData &&
             genCodeSectionLen == other.genCodeSectionLen &&
             nStrings == other.nStrings &&
             stringOffsetsTable == other.stringOffsetsTable &&
             llvmIrData == other.llvmIrData &&
             llvmIrDataLen == other.llvmIrDataLen;
    }
    bool operator!=(const ModuleSection& other) const {
      return !(*this == other);
    }
    static bool update(owned<ModuleSection>& keep,
                       owned<ModuleSection>& addin) {
      return defaultUpdateOwned(keep, addin);
    }
    void mark(Context* context) const {
      // nothing to mark
    }
  };

  /** Helper type to be returned by loadLocations */
  struct LocationMaps {
    using MapType = std::unordered_map<const uast::AstNode*, Location>;

    MapType astToLocation;
    #define LOCATION_MAP(ast__, location__) \
      MapType CHPL_ID_LOC_MAP(ast__, location__);
    #include "chpl/uast/all-location-maps.h"
    #undef LOCATION_MAP

    void clear();
    const MapType* getLocationMap(int tag) const;
    MapType* getLocationMap(int tag);

    void swap(LocationMaps& other);
    static bool update(LocationMaps& keep, LocationMaps& addin);
    void mark(Context* context) const;
  };


 private:
  struct ModuleInfo {
    UniqueString moduleSymPath;
    UniqueString sourceFilePath;
    Region moduleRegion;
  };

  UniqueString libPath;

  int fd = -1;
  llvm::sys::fs::mapped_file_region* mappedFile;

  // these are computed from mapped_file
  unsigned char fileHash[HASH_SIZE];
  size_t fileLen = 0;
  const unsigned char* fileData = nullptr;

  // stores module symbol IDs and the file paths they came from
  std::vector<ModuleInfo> modules;

  // maps from the moduleSymPath to the index in 'modules' above
  std::map<UniqueString, size_t> moduleSymPathToIdx;

  LibraryFile() { }

  void invalidFileError(Context* context) const;

  std::error_code openAndMap();

  // reads the library file and raises errors with Context if there
  // are any errors
  // returns 'true' if everything is OK, 'false' if there were errors.
  bool readHeaders(Context* context);

  static const owned<LibraryFile>& loadLibraryFileQuery(Context* context,
                                                        UniqueString libPath);

  // Reads the module section metadata into 'mod' and raises errors with Context
  // if there are any errors.
  // Validates the header for each section contained in the module.
  // returns 'true' if everything is OK, 'false' if there were errors.
  bool readModuleSection(Context* context,
                         Region moduleRegion,
                         UniqueString moduleSymPath,
                         ModuleSection& mod) const;

  // reads the module metadata (including the symbol table)
  // returns nullptr if something went wrong
  static const owned<ModuleSection>&
  loadModuleSectionQuery(Context* context,
                         const LibraryFile* f,
                         int moduleIndex);

  // returns nullptr if something went wrong
  const ModuleSection* loadModuleSection(Context* context,
                                         int moduleIndex) const;
  // reads the string table metadata & sets up 'helper' to be ready
  // to read long strings
  LibraryFileDeserializationHelper setupHelper(Context* context,
                                               const ModuleSection* mod) const;

  // deserializes the uAST for the module starting at the passed offset
  // and stores that uAST in the passed builder.
  //
  // returns 'true' if everything is OK, 'false' if there were errors
  bool readModuleAst(Context* context,
                     Region moduleOffset,
                     LibraryFileDeserializationHelper& helper,
                     uast::Builder& builder) const;

  // deserializes the uAST for all of the modules from sourceFilePath
  // that are stored in this library
  //
  // returns an empty BuilderResult if something went wrong.
  static const uast::BuilderResult& loadAstQuery(Context* context,
                                                 const LibraryFile* f,
                                                 UniqueString sourceFilePath);

  // read the file paths stored in the locations section
  // returns 'true' if everything is OK, 'false' if there were errors.
  bool readLocationPaths(Context* context,
                         std::vector<UniqueString>& paths,
                         const ModuleSection* m) const;

  // populate 'map' with Locations by reading a serialized LocationGroup
  // 'symbolTableSymbolAst' needs to be the corresponding uAST for the
  // symbol represented by the location group.
  // returns 'true' if everything is OK, 'false' if there were errors.

  bool
  readLocationGroup(Context* context,
                    LocationMaps& maps,
                    Deserializer& des,
                    const uast::AstNode* symbolTableSymbolAst,
                    const std::vector<UniqueString>& paths,
                    const uast::BuilderResult& br) const;

  // populate 'map' with Locations by reading serialized Location entries.
  // 'cur' needs to be the corresponding uAST for the node whose location
  // is currently being determined.
  // returns 'true' if everything is OK, 'false' if there were errors.
  bool
  readLocationEntries(Context* context,
                      LocationMaps& maps,
                      Deserializer& des,
                      const uast::AstNode* cur,
                      UniqueString path,
                      const uast::BuilderResult& br,
                      int& lastEntryLastLine) const;

  // loads the locations, returns 'true' if it is OK
  bool doLoadLocations(Context* context,
                       int moduleIndex,
                       int symbolTableEntryIndex,
                       const uast::AstNode* symbolTableEntryAst,
                       LocationMaps& result) const;

  // Compute a locations map that stores locations for uAST nodes
  // within the symbol table symbol passed.
  static const LocationMaps&
  loadLocationsQuery(Context* context,
                     const LibraryFile* f,
                     int moduleIndex,
                     int symbolTableEntryIndex,
                     const uast::AstNode* symbolTableEntryAst);

  static owned<llvm::Module>
  loadLlvmModuleImpl(Context* context, const LibraryFile* f, int moduleIndex);

 public:
  ~LibraryFile();

  bool operator==(const LibraryFile& other) const;
  bool operator!=(const LibraryFile& other) const {
    return !(*this == other);
  }
  size_t hash();
  void mark(Context* context) const;
  static bool update(owned<LibraryFile>& keep, owned<LibraryFile>& addin);
  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /**
    This query reads the file from the given path and produces a LibraryFile,
    which represents the contents of the library.
   */
  static const LibraryFile* load(Context* context, UniqueString libPath);

  /**
    Register all of the paths supported by this library so that
    attempts to load a module contained in the library will use the
    library version.
   */
  void registerLibrary(Context* context) const;

  /**
    Returns a vector containing the paths stored in this module.
    */
  std::vector<UniqueString> containedFilePaths() const;

  /**
    Print a summary of a LibraryFile.
    */
  void summarize(Context* context, std::ostream& s) const;

  /**
    Load uAST from this LibraryFile for a particular source path.
   */
  const uast::BuilderResult& loadSourceAst(Context* context,
                                           UniqueString fromSourcePath) const;

  /**
    Load uAST from a this LibraryFile for a particular module path.
    For a toplevel module, the module path is just the module name.
    For a submodule M of a parent module P, it would be P.M.

    Returns nullptr if no such module is found in this LibraryFile
    or if an error occurred.
   */
  const uast::Module* loadModuleAst(Context* context,
                                    UniqueString moduleSymPath) const;

  /**
    Lookup the location for an Ast node stored in this library file.
    Assumes that 'ast' is stored within the Locations section
    found from module 'moduleIndex' and symbol entry 'symbolTableEntryIndex'.
   */
  const LocationMaps&
  loadLocations(Context* context,
                int moduleIndex,
                int symbolTableEntryIndex,
                const uast::AstNode* symbolTableEntryAst) const;

#ifdef HAVE_LLVM
  /**
    Load LLVM IR from a this LibraryFile for a particular module path.
    For a toplevel module, the module path is just the module name.
    For a submodule M of a parent module P, it would be P.M.

    Returns nullptr if no such module is found in this LibraryFile
    or if an error occurred.
   */
  owned<llvm::Module> loadGenCodeModule(Context* context,
                                        UniqueString moduleSymPath) const;
#endif
};


} // end namespace libraries
} // end namespace chpl

#endif
