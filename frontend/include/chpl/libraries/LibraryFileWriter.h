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

#ifndef CHPL_LIBRARIES_LIBRARY_FILE_WRITER_H
#define CHPL_LIBRARIES_LIBRARY_FILE_WRITER_H

#include "chpl/framework/ID.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/libraries/LibraryFileFormat.h"

#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace chpl {
// forward declarations
class Context;
namespace uast {
  class Module;
}
namespace libraries {


class LibraryFileWriter;

/**
  Helper object for registering serialized uAST positions during serialization
 */
class LibraryFileSerializationHelper {
 friend class LibraryFileWriter;

 private:
  uint64_t moduleSectionStart = 0;
  uint64_t astSectionStartFileOffset = 0;
  uint64_t locationSectionStartFileOffset = 0;
  uint64_t astCounter = 0;

  // stores the uAST nodes that are represented in the symbol table
  // in uAST declaration / traversal order
  std::vector<const uast::AstNode*> symbolTableVec;
  // a set storing the same elements as symbolTableVec
  std::unordered_set<const uast::AstNode*> symbolTableSet;

  // stores the relative offset (to the start of the module header)
  // for serialized uAST for the given node
  // Only nodes in symbolTableSet are represented here.
  std::unordered_map<const uast::AstNode*, uint32_t> astOffsets;

  // stores the relative offset (to the start of the module header)
  // for serialized Location information for the given node.
  // Only nodes in symbolTableSet are represented here.
  std::unordered_map<const uast::AstNode*, uint32_t> locOffsets;

  LibraryFileSerializationHelper() { }

 public:
  /** Record that a symbol should be present in the symbol table. */
  void noteSymbolForTable(const uast::AstNode* ast);

  /* Should be called when starting to serialize a uAST node */
  void beginAst(const uast::AstNode* ast, std::ostream& os);
  /* Should be called when finishing serialization of a uAST node */
  void endAst(const uast::AstNode* ast, std::ostream& os);
};


/** For writing a .dyno library file */
class LibraryFileWriter {
 public:
  struct GenInfo {
    UniqueString cname;
    bool isInstantiation = false;
    // TODO: other information about instantiations
  };

 private:
  using PathToIndex = std::unordered_map<UniqueString, unsigned int>;
  struct ModInfo {
    UniqueString moduleName;
    const uast::Module* moduleAst = nullptr;
    UniqueString fromSourcePath;
    std::string genCode; // LLVM IR bc data
    std::unordered_map<ID, std::vector<GenInfo>> genMap;
  };

  Context* context = nullptr;
  std::vector<UniqueString> inputFiles;
  std::string outputFilePath;
  std::ofstream fileStream;
  bool ok = true;

  // per-module information
  std::vector<ModInfo> modules;

  /** Emit an error indicating failure to create the library and
      set 'ok' to 'false' */
  void fail(const char* msg);

  /** Gather the top-level modules and the paths they came from */
  static std::vector<ModInfo>
  gatherTopLevelModules(Context* context, std::vector<UniqueString> paths);

  /** Open the file */
  void openFile();

  /** Write the file header */
  void writeHeader();

  /** pad with zeros to 16-byte offset */
  void padToAlign();

  /** Write the module section for the given module. Returns
     the file offset to this section. */
  Region writeModuleSection(const ModInfo& info);

  /** Note the top-level symbols for the passed module for the symbol table */
  void noteToplevelSymbolsForTable(const uast::Module* mod,
                                   LibraryFileSerializationHelper& reg);

  /** Write the symbol table for a given module. Returns the
      module-relative offset to the symbol table. */
  Region writeSymbolTable(const ModInfo& info,
                          Serializer& ser,
                          LibraryFileSerializationHelper& reg);

  /** Write the uAST section. This will populate 'longStringsOffsets'
      with 0 offsets. Returns the module-relative offset of the uAST section. */
  Region writeAst(const uast::Module* mod,
                  Serializer& ser,
                  LibraryFileSerializationHelper& reg);

  /** Write the long strings in 'longStringsOffsets' that were gathered
      in the process of writing the module & update their indices.
      Returns the module-relative offset to the long strings section. */
  Region writeLongStrings(uint64_t moduleSectionStart, Serializer& ser);

  /** Write the locations.
      Returns the module-relative offset of the locations section.
  */
  Region writeLocations(const uast::Module* mod,
                        Serializer& ser,
                        LibraryFileSerializationHelper& reg);

  /** Write a locations group starting with the location for 'ast'
      and contained locations until reaching another node that
      is in the symbol table (and so gets its own location group).
      Returns the relative offset of the locations group. */
  Region writeLocationGroup(const uast::AstNode* ast,
                            Serializer& ser,
                            LibraryFileSerializationHelper& reg,
                            const PathToIndex& pathToIdx);

  /** Write the location entry for a given ast node and its children,
      stopping when reaching a child node in the symbol table */
  void writeLocationEntries(const uast::AstNode* ast,
                            Serializer& ser,
                            LibraryFileSerializationHelper& reg,
                            int& lastLine);

  /** Write the generated code section and returns the
      module-relative offset of the section. */
  Region writeGenCode(uint64_t moduleSectionStart,
                      Serializer& ser,
                      const std::string& gen);

 public:
  /**
    Construct a LibraryFileWriter to output to 'outputFilePath' */
  LibraryFileWriter(Context* context, std::string outputFilePath)
    : context(context), outputFilePath(outputFilePath)
  { }

  /**
    Parse the uAST for the provided source paths and save
    the result.
    */
  void setSourcePaths(std::vector<UniqueString> paths);

  /**
    Set the buffer storing the generated LLVM IR byte code
    for the top-level module with the passed name.

    Must be done after 'setSourcePaths'.
   */
  void setGeneratedCode(UniqueString modName,
                        std::string buffer,
                        std::unordered_map<ID, std::vector<GenInfo>> genMap);

  /**
    Write the header and sections to the library file.
    Returns 'true' if everything is OK, or 'false' if there was an
    error in the process.
    */
  bool writeAllSections();

  /** Gather the names of the top-level modules in the source
      code files from 'paths' */
  static std::vector<UniqueString>
  gatherTopLevelModuleNames(Context* context, std::vector<UniqueString> paths);
};


} // end namespace libraries
} // end namespace chpl

#endif
