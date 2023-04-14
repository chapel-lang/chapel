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

#include "chpl/framework/TemporaryFileResult.h"

#include "chpl/framework/Context.h"
#include "chpl/util/filesystem.h"

#include "llvm/ADT/Twine.h"
#include "llvm/Support/FileSystem.h"

namespace chpl {


static int globalTemporaryResultCounter = 1;

TemporaryFileResult::TemporaryFileResult() {
  hash_.fill(0);
}

owned<TemporaryFileResult>
TemporaryFileResult::create(Context* context,
                            llvm::StringRef prefix,
                            llvm::StringRef suffix) {
  const std::string& tmpDir = context->tmpDir();

  int index = globalTemporaryResultCounter++;

  TemporaryFileResult* ret = new TemporaryFileResult();

  ret->path_ = (llvm::Twine(tmpDir) + "/" +
                prefix + "-" + llvm::Twine(index) + suffix).str();
  // delete the file on destruction unless we are saving the tmp dir files
  ret->deleteOnDestroy_ = !context->shouldSaveTmpDirFiles();

  return toOwned(ret);
}

TemporaryFileResult::~TemporaryFileResult() {
  if (deleteOnDestroy_) {
    if (!path_.empty()) {
      std::error_code err = llvm::sys::fs::remove(path_,
                                                  /* IgnoreNonExisting */ true);
      std::ignore = err; // it is a temporary space issue but
                         // failing to remove such a file need not be a fatal
                         // error. Could warn about it though.
    }
  }
}

std::error_code TemporaryFileResult::complete() {
  CHPL_ASSERT(length_ == 0); // should not call complete twice e.g.
  if (!path_.empty()) {
    uint64_t len = 0;
    std::error_code err = llvm::sys::fs::file_size(path_, len);
    if (!err) {
      // compute the file hash
      auto hashOrErr = hashFile(path_);
      if (hashOrErr) {
        length_ = len;
        hash_ = hashOrErr.get();
      } else {
        err = hashOrErr.getError();
      }
    }

    if (err) {
      return err;
    }
  }

  return std::error_code();
}

bool TemporaryFileResult::operator==(const TemporaryFileResult& other) const {
  // Handle empty paths
  if (path_.empty() && other.path_.empty()) {
    return true; // both are empty
  }
  if (path_.empty() || other.path_.empty()) {
    return false; // one of them is empty but not the other
  }
  // Note: the path itself is not compared here,
  // so that the comparison is about the file contents, instead.

  return (length_ == other.length_ && hash_ == other.hash_);
}

bool TemporaryFileResult::update(owned<TemporaryFileResult>& keep,
                                 owned<TemporaryFileResult>& addin) {
#ifndef NDEBUG
  if (const TemporaryFileResult* t = addin.get()) {
    if (!t->path_.empty()) {
      // 'complete' should be called on a TemporaryFileResult after the
      // data is stored within the file. If it is forgetten, the query
      // framework will not correctly handle incremental changes. So,
      // check here in the update function (which will be called when
      // the TemporaryFileResult is the result of a query) that the data
      // is completed.

      HashFileResult zero;
      zero.fill(0);
      CHPL_ASSERT(t->hash_ != zero && "did you forget to call .complete() ?");
    }
  }
#endif

  return defaultUpdateOwned(keep, addin);
}

void TemporaryFileResult::stringify(std::ostream& ss,
                                    StringifyKind stringKind) const {
  ss << "TemporaryFileResult("
     << "path=" << path_ << ", "
     << "length=" << length_ << ", "
     << "hash=" << fileHashToHex(hash_) << ")";
}

IMPLEMENT_DUMP(TemporaryFileResult);


} // end namespace chpl
