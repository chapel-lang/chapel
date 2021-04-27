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

#ifndef CHPL_AST_LOCATION_H
#define CHPL_AST_LOCATION_H

#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace uast {


/**
  This class represents a source location.
 */
class Location final {
 private:
  UniqueString path_;
  int firstLine_ = -1;
  int firstColumn_ = -1;
  int lastLine_ = -1;
  int lastColumn_ = -1;
 public:
  Location()
    : path_() {
  }

  Location(UniqueString path,
           int firstLine=-1, int firstColumn=-1,
           int lastLine=-1, int lastColumn=-1)
    : path_(path),
      firstLine_(firstLine), firstColumn_(firstColumn),
      lastLine_(lastLine), lastColumn_(lastColumn) {
  }

  UniqueString path() { return path_; }

  inline bool operator==(const Location other) const {
    return this->path_ == other.path_ &&
           this->firstLine_ == other.firstLine_ &&
           this->firstColumn_ == other.firstColumn_ &&
           this->lastLine_ == other.lastLine_ &&
           this->lastColumn_ == other.lastColumn_;
  }
  inline bool operator!=(const Location other) const {
    return !(*this == other);
  }

  size_t hash() const {
    size_t h = this->path_.hash();
    h = hash_combine(h, this->firstLine_);
    h = hash_combine(h, this->firstColumn_);
    h = hash_combine(h, this->lastLine_);
    h = hash_combine(h, this->lastColumn_);
    return h;
  }

  void swap(Location& other) {
    Location oldThis = *this;
    *this = other;
    other = oldThis;
  }
};

} // end namespace uast

template<> struct update<chpl::uast::Location> {
  bool operator()(chpl::uast::Location& keep,
                  chpl::uast::Location& addin) const {
    return defaultUpdate(keep, addin);
  }
};


// Allow chpl::uast::Location to be just called chpl::Location
// TODO: Should it be moved out of the ast namespace? What directory
// should it go in?

using chpl::uast::Location;

} // end namespace chpl

#endif
