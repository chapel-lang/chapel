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

#ifndef CHPL_LIBRARIES_LIBRARY_FILE_H
#define CHPL_LIBRARIES_LIBRARY_FILE_H

#include "chpl/framework/UniqueString.h"
#include "chpl/libraries/LibraryFileFormat.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/util/memory.h"

#include <sstream>
#include <system_error>
#include <unordered_map>

// forward declare LLVM Support Library dependencies
namespace llvm {
  namespace sys {
    namespace fs {
      class mapped_file_region;
    }
  }
}

namespace chpl {
// forward declarations
class Context;
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
  struct SymbolInfo {
    // relative to the module section
    uint32_t symbolEntryOffset;
    uint32_t uastOffset;
    uint32_t locationsOffset;
  };

  int nStrings = 0;
  const unsigned char* moduleSectionData = nullptr;
  size_t moduleSectionLen = 0;
  const uint32_t* stringOffsetsTable = nullptr;

  // key: module-section-relative offset for the serialized uast for
  //      a symbol table symbol
  // value: symbol table index of that symbol
  std::unordered_map<uint32_t, int> offsetToSymIdx;

  // for AstNodes in the symbol table that were deserialized
  // maps to the symbol table index of that symbol.
  std::unordered_map<const uast::AstNode*, int> astToSymIdx;

  // maps from symbol table index to SymbolInfo which contains some
  // useful offsets
  std::vector<SymbolInfo> symbols;

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
 private:
  struct ModuleInfo {
    UniqueString moduleSymPath;
    UniqueString sourceFilePath;
    uint64_t moduleSectionOffset = 0;
  };

  UniqueString libPath;

  int fd = -1;
  llvm::sys::fs::mapped_file_region* mappedFile;

  // these are computed from mapped_file
  unsigned char fileHash[HASH_SIZE];
  size_t len = 0;
  const unsigned char* data = nullptr;

  // stores module symbol IDs and the file paths they came from
  std::vector<ModuleInfo> modules;

  LibraryFile() { }

  std::error_code openAndMap();

  // reads the library file and raises errors with Context if there
  // are any errors
  // returns 'true' if everything is OK, 'false' if there were errors.
  bool readHeaders(Context* context);

  static const owned<LibraryFile>& loadLibraryFileQuery(Context* context,
                                                        UniqueString libPath);

  // reads the string table metadata & sets up 'helper' to be ready
  // to read long strings
  LibraryFileDeserializationHelper setupHelper(Context* context,
                                               uint64_t moduleOffset) const;

  // deserializes the uAST for the module starting at the passed offset
  // and stores that uAST in the passed builder.
  //
  // returns 'true' if everything is OK, 'false' if there were errors
  bool readModuleAst(Context* context,
                     uint64_t moduleOffset,
                     LibraryFileDeserializationHelper& helper,
                     uast::Builder& builder) const;

  // deserializes the uAST for all of the modules from sourceFilePath
  // that are stored in this library
  //
  // returns an empty BuilderResult if something went wrong.
  static const uast::BuilderResult& loadAstQuery(Context* context,
                                                 const LibraryFile* f,
                                                 UniqueString sourceFilePath);

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
  Location lookupLocation(Context* context,
                          int moduleIndex,
                          int symbolTableEntryIndex,
                          const uast::AstNode* ast) const;
};


} // end namespace libraries
} // end namespace chpl

#endif
