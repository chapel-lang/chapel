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

#ifndef CHPL_LIBRARIES_LIBRARY_FILE_WRITER_H
#define CHPL_LIBRARIES_LIBRARY_FILE_WRITER_H

#include "chpl/framework/UniqueString.h"

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
struct LibraryFileSerializationHelper {
 friend class LibraryFileWriter;

 private:
  uint64_t moduleSectionStart = 0;
  uint64_t uAstCounter = 0;

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

  LibraryFileSerializationHelper(uint64_t moduleSectionStart)
    : moduleSectionStart(moduleSectionStart) {
  }

 public:
  /** Record that a symbol should be present in the symbol table. */
  void noteSymbolForTable(const uast::AstNode* ast);

  /* Should be called when starting to serialize a uAST node */
  void beginAst(const uast::AstNode* ast, std::ostream& os);
  /* Should be called when finishing serialization of a uAST node */
  void endAst(const uast::AstNode* ast, std::ostream& os);

  /* Should be called when starting to write a location group */
  void beginLocation(const uast::AstNode* ast, std::ostream& os);
  /* Should be called when finishing writing a location group */
  void endLocation(const uast::AstNode* ast, std::ostream& os);
};


/** For writing a .dyno library file */
class LibraryFileWriter {
 private:
  using PathToIndex = std::unordered_map<UniqueString, unsigned int>;

  Context* context = nullptr;
  std::vector<UniqueString> inputFiles;
  std::string outputFilePath;
  std::ofstream fileStream;
  bool ok = true;

  // top-level modules and paths where they came from
  std::vector<std::pair<const uast::Module*, UniqueString>> modulesAndPaths;

  /** Emit an error indicating failure to create the library and
      set 'ok' to 'false' */
  void fail(const char* msg);

  /** Gather the top-level modules */
  void gatherTopLevelModules();

  /** Open the file */
  void openFile();

  /** Write the file header */
  void writeHeader();

  /** pad with zeros to 16-byte offset */
  void padToAlign();

  /** Write the module section for the given module. Returns
     the file offset to this section. */
  uint64_t writeModuleSection(const uast::Module* mod,
                              UniqueString fromFilePath);

  /** Note the top-level symbols for the passed module for the symbol table */
  void noteToplevelSymbolsForTable(const uast::Module* mod,
                                   LibraryFileSerializationHelper& reg);

  /** Write the symbol table for a given module. Returns the
      relative offset to the symbol table. */
  uint64_t writeSymbolTable(const uast::Module* mod,
                            Serializer& ser,
                            LibraryFileSerializationHelper& reg);

  /** Write the uAST section. This will populate 'longStringsOffsets'
      with 0 offsets. Returns the relative offset of the uAST section. */
  uint64_t writeAst(const uast::Module* mod,
                    Serializer& ser,
                    LibraryFileSerializationHelper& reg);

  /** Write the long strings in 'longStringsOffsets' that were gathered
      in the process of writing the module & update their indices.
      Returns the relative offset to the long strings section. */
  uint64_t writeLongStrings(uint64_t moduleSectionStart, Serializer& ser);

  /** Write the locations. Returns the relative offset of the locations section.
  */
  uint64_t writeLocations(const uast::Module* mod,
                          Serializer& ser,
                          LibraryFileSerializationHelper& reg);

  /** Write a locations group starting with the location for 'ast'
      and containing contained locations until reaching a node that
      is in the symbol table (and so gets its own location group).
      Returns the relative offset of the locations group. */
  uint64_t writeLocationGroup(const uast::AstNode* ast,
                              Serializer& ser,
                              LibraryFileSerializationHelper& reg,
                              const PathToIndex& pathToIdx);

  /** Write the location entry for a given ast node and its children,
      stopping when reaching a child node in the symbol table */
  void writeLocationEntries(const uast::AstNode* ast,
                            Serializer& ser,
                            LibraryFileSerializationHelper& reg,
                            int& lastLine);

 public:
  /**
    Construct a LibraryFileWriter to output the uAST from the
    top-level modules in the source files provided in 'paths'. */
  LibraryFileWriter(Context* context,
                    std::vector<UniqueString> paths,
                    std::string outputFilePath) :
    context(context),
    inputFiles(paths),
    outputFilePath(outputFilePath)
  { }

  /**
    Write the header and sections to the library file.
    Returns 'true' if everything is OK, or 'false' if there was an
    error in the process.
    */
  bool writeAllSections();
};


} // end namespace libraries
} // end namespace chpl

#endif
