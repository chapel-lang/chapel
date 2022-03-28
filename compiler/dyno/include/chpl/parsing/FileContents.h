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

#ifndef CHPL_PARSING_FILE_CONTENTS_H
#define CHPL_PARSING_FILE_CONTENTS_H

#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/update-functions.h"
#include "chpl/queries/stringify-functions.h"

#include <string>

namespace chpl {
namespace parsing {


/**
 This class represents the result of reading a file.
 */
class FileContents {
 private:
  std::string text_;
  ErrorMessage error_;

 public:
  /** Construct a FileContents containing empty text and no error */
  FileContents()
    : text_(), error_() { }
  /** Construct a FileContents containing the passed text and no error */
  FileContents(std::string text)
    : text_(std::move(text)), error_() { }
  /** Construct a FileContents containing the passed text and error */
  FileContents(std::string text, ErrorMessage error)
    : text_(std::move(text)), error_(std::move(error)) { }

  /** Return a reference to the contents of this file */
  const std::string& text() const { return text_; }

  /** Return a reference to an error encountered when reading this file */
  const ErrorMessage& error() const { return error_; }

  bool operator==(const FileContents& other) const {
    return text_ == other.text_ &&
           error_ == other.error_;
  }
  bool operator!=(const FileContents& other) const {
    return !(*this == other);
  }
  void swap(FileContents& other) {
    text_.swap(other.text_);
    error_.swap(other.error_);
  }
  static bool update(FileContents& keep, FileContents& addin) {
    return defaultUpdate(keep, addin);
  }
  void mark(Context* context) const {
    error_.mark(context);
  }
};


} // end namespace parsing

/// \cond DO_NOT_DOCUMENT
/// \endcond


} // end namespace chpl
#endif
