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

#ifndef CHPL_QUERIES_COMMENT_ID_H
#define CHPL_QUERIES_COMMENT_ID_H

#include "chpl/queries/UniqueString.h"
#include "chpl/util/hash.h"


namespace chpl {
/**
   This class represents a unique ID for a comment
   It is separate from ID because it does not participate in the query system
 */
class CommentID {
 private:
  // Index of comment in a file
  int index_ = -1;

 public:
  CommentID() = default;
  CommentID(int index) : index_(index) {
    assert(index >= 0);
  }

  /** Return the index of the comment id */
  int index() const { return index_; }
};

} // end namespace chpl

#endif
