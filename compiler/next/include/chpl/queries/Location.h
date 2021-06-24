/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

  size_t hash() const {
    return chpl::hash(path_, firstLine_, firstColumn_, lastLine_, lastColumn_);
  }

  void swap(Location& other) {
    Location oldThis = *this;
    *this = other;
    other = oldThis;
  }

  void markUniqueStrings(Context* context) const {
    this->path_.mark(context);
  }
};

// docs are turned off for this as a workaround for breathe errors
/// \cond DO_NOT_DOCUMENT
template<> struct update<chpl::Location> {
  bool operator()(chpl::Location& keep,
                  chpl::Location& addin) const {
    return defaultUpdate(keep, addin);
  }
};
template<> struct mark<chpl::Location> {
  void operator()(Context* context, const chpl::Location& keep) const {
    keep.markUniqueStrings(context);
  }
};
/// \endcond

} // end namespace chpl

#endif
