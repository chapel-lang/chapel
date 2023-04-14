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

#ifndef CHPL_FRAMEWORK_TEMPORARYFILERESULT_H
#define CHPL_FRAMEWORK_TEMPORARYFILERESULT_H

#include "chpl/framework/stringify-functions.h"
#include "chpl/framework/update-functions.h"
#include "chpl/util/filesystem.h"
#include "chpl/util/memory.h"

#include "llvm/ADT/StringRef.h"

#include <ostream>

namespace chpl {
class Context;

/**

  This class represents a result stored in a temporary file within
  the Context's tmpDir() and enables that file to be used within
  the query system as an argument or result type. This class
  represents the data in the file, but not the file path itself.
  In some ways, it is similar to the way that the query system
  interacts with allocated objects; in this case it's just allocating
  a path in the temporary directory rather than memory.

  Note that users of this type need to call 'complete()' once the file
  has been created and not change it after that point.
*/
class TemporaryFileResult final {
  std::string path_;
  size_t length_ = 0;
  HashFileResult hash_;
  bool deleteOnDestroy_ = false;

 public:
  /** Create an empty TemporaryFileResult. It will have an empty
      path and nothing will be deleted when it goes out of scope. */
  TemporaryFileResult();

  /** Create a TemporaryFileResult that represents a file
      with a name like
         <context->tmpDir()>/<prefix><stuff><suffix>
      Clients should not be sensitive to what 'stuff' consists of,
      but in needs to be present in order to unique-ify such files.
   */ 
  static owned<TemporaryFileResult> create(Context* context,
                                           llvm::StringRef prefix,
                                           llvm::StringRef suffix);

  /** When a TemporaryFileResult is destroyed, the file is deleted,
      if it exists. */
  ~TemporaryFileResult();

  /** Returns the path to the file. */
  std::string path() const { return path_; }

  /** Updates information about the file stored here, which can include
      the file length and hash, after the file has been created.
      Returns an error code if there was an OS error when computing the
      length and hash. */
  std::error_code complete();

  bool operator==(const TemporaryFileResult& other) const;
  bool operator!=(const TemporaryFileResult& other) const {
    return !(*this == other);
  }
  static bool update(owned<TemporaryFileResult>& keep,
                     owned<TemporaryFileResult>& addin);
  void mark(Context* context) const { }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};


} // end namespace chpl
#endif
