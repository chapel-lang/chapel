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

#ifndef CHPL_QUERIES_LOCATION_H
#define CHPL_QUERIES_LOCATION_H

#include "chpl/queries/UniqueString.h"
#include "chpl/util/hash.h"

namespace chpl {


/**
  This class represents a source location.
 */
class Location final {
  UniqueString path_;
  int firstLine_   = -1;
  int firstColumn_ = -1;
  int lastLine_    = -1;
  int lastColumn_  = -1;

public:
  Location() = default;

  explicit
  Location(UniqueString path,
           int firstLine=-1, int firstColumn=-1,
           int lastLine=-1, int lastColumn=-1)
    : path_(std::move(path)),
      firstLine_(firstLine), firstColumn_(firstColumn),
      lastLine_(lastLine), lastColumn_(lastColumn) {
  }

  bool isEmpty() const {
    return path_.isEmpty();
  }

  UniqueString path() const { return path_; }
  int firstLine() const { return firstLine_; }
  int firstColumn() const { return firstColumn_; }
  int lastLine() const { return lastLine_; }
  int lastColumn() const { return lastColumn_; }
  int line() const { return firstLine(); }

  inline bool operator==(const Location& other) const {
    return this->path_ == other.path_ &&
           this->firstLine_ == other.firstLine_ &&
           this->firstColumn_ == other.firstColumn_ &&
           this->lastLine_ == other.lastLine_ &&
           this->lastColumn_ == other.lastColumn_;
  }

  inline bool operator!=(const Location& other) const {
    return !(*this == other);
  }
  void swap(Location& other) {
    path_.swap(other.path_);
    std::swap(firstLine_, other.firstLine_);
    std::swap(firstColumn_, other.firstColumn_);
    std::swap(lastLine_, other.lastLine_);
    std::swap(lastColumn_, other.lastColumn_);
  }
  static bool update(Location& keep, Location& addin);
  void mark(Context* context) const {
    this->path_.mark(context);
  }

  size_t hash() const {
    return chpl::hash(path_, firstLine_, firstColumn_, lastLine_, lastColumn_);
  }
};

/// \cond DO_NOT_DOCUMENT

/// \endcond


} // end namespace chpl

#endif
