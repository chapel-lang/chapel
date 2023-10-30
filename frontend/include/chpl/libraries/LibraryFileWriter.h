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
#include <vector>

namespace chpl {
// forward declarations
class Context;
namespace uast {
  class Module;
}
namespace libraries {


/** For writing a .dyno library file */
class LibraryFileWriter {
 private:
  Context* context = nullptr;
  std::vector<UniqueString> inputFiles;
  std::string outputFilePath;
  std::ofstream fileStream;
  // top-level modules and paths where they came from
  std::vector<std::pair<const uast::Module*, UniqueString>> modulesAndPaths;

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

  /** Write the symbol table for a given module. Returns the
      relative offset to the symbol table. */
  uint64_t writeSymbolTable(const uast::Module* mod,
                            uint64_t moduleSectionStart);

  /** Write the uAST section. This will populate 'longStringsOffsets'
      with 0 offsets. Returns the relative offset of the uAST section. */
  uint64_t writeAst(const uast::Module* mod,
                    uint64_t moduleSectionStart,
                    Serializer& ser);

  /** Write the long strings in 'longStringsOffsets' that were gathered
      in the process of writing the module & update their indices.
      Returns the relative offset to the long strings section. */
  uint64_t writeLongStrings(uint64_t moduleSectionStart, Serializer& ser);

  /** Write the locations. Returns the relative offset of the locations section.
  */
  uint64_t writeLocations(const uast::Module* mod, uint64_t moduleSectionStart);

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
