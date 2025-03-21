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

#ifndef CHPL_PARSING_FILE_CONTENTS_H
#define CHPL_PARSING_FILE_CONTENTS_H

#include "chpl/framework/update-functions.h"
#include "chpl/framework/stringify-functions.h"

#include <string>

namespace chpl {

// Forward declare the error class that can't be referenced until later.
class ErrorBase;

namespace parsing {

/**
 This class represents the result of reading a file.
 */
class FileContents {
 private:
  std::string text_;
  // TODO: it would be better to use the LLVM error handling strategy here,
  //       instead of storing errors created via Context.
  const ErrorBase* error_ = nullptr;

 public:
  /** Construct a FileContents containing empty text and no error */
  FileContents()
    : text_(), error_() { }
  /** Construct a FileContents containing the passed text and no error */
  FileContents(std::string text)
    : text_(std::move(text)), error_() { }
  /** Construct a FileContents containing the passed text and error */
  FileContents(std::string text, const ErrorBase* error)
    : text_(std::move(text)), error_(error) { }

  /** Return a reference to the contents of this file */
  const std::string& text() const { return text_; }

  /** Return a reference to an error encountered when reading this file */
  const ErrorBase* error() const { return error_; }

  bool operator==(const FileContents& other) const {
    return text_ == other.text_ &&
           error_ == other.error_;
  }
  bool operator!=(const FileContents& other) const {
    return !(*this == other);
  }
  void swap(FileContents& other) {
    text_.swap(other.text_);
    std::swap(error_, other.error_);
  }
  static bool update(FileContents& keep, FileContents& addin) {
    return chpl::defaultUpdate(keep, addin);
  }
  void mark(Context* context) const;
};


} // end namespace parsing

/// \cond DO_NOT_DOCUMENT
/// \endcond


} // end namespace chpl
#endif
