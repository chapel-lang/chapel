/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/util/memory.h"

#include <sstream>
#include <system_error>

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


/** For reading a .dyno LibraryFile.
    Some data is read from the file on-demand.
    Uses `mmap` to keep the code simple and to support random-access well. */
class LibraryFile {
 private:
  UniqueString libPath;

  int fd = -1;
  llvm::sys::fs::mapped_file_region* mappedFile;

  // these are computed from mapped_file
  unsigned char fileHash[HASH_SIZE];
  size_t len = 0;
  const unsigned char* data = nullptr;

  // maps from module path to the offset of its module section header
  std::map<UniqueString, uint64_t> modulePathToSection;

  LibraryFile() { }

  std::error_code openAndMap();

  // reads the library file and raises errors with Context if there
  // are any errors
  // returns 'true' if everything is OK, 'false' if there were errors.
  bool readHeaders(Context* context);

  static const owned<LibraryFile>& loadLibraryFileQuery(Context* context,
                                                        UniqueString libPath);

 public:
  ~LibraryFile();

  bool operator==(const LibraryFile& other) const;
  bool operator!=(const LibraryFile& other) const {
    return !(*this == other);
  }
  size_t hash();
  void mark(Context* context) const;
  static bool update(owned<LibraryFile>& keep, owned<LibraryFile>& addin);

  /**
    This query reads the file from the given path and produces a LibraryFile,
    which contains useful information about the library's contents.
   */
  const LibraryFile* load(Context* context, UniqueString libPath);

  /**
    Load uAST from a this LibraryFile for a particular module path.
    For a toplevel module, the module path is just the module name.
    For a submodule M of a parent module P, it would be P.M.

    Returns nullptr if no such module is found in this LibraryFile.
   */
  const uast::Module* loadModule(Context* context, UniqueString modulePath);
};


} // end namespace libraries
} // end namespace chpl

#endif
