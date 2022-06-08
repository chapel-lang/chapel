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

#ifndef CHPL_QUERIES_ID_H
#define CHPL_QUERIES_ID_H

#include "chpl/queries/UniqueString.h"
#include "chpl/util/hash.h"

namespace chpl {
class Context;


/**
  This class represents an ID for an AST node.
  AST element IDs can be helpful for creating maps with AST
  elements as keys.
  All AST nodes have IDs.
 */
class ID final {
 private:
  // A path to the symbol, e.g. MyModule#0.MyFunction#1
  UniqueString symbolPath_;

  // Within that symbol, what is the number of this node in
  // a postorder traversal?
  // The symbol itself has ID -1.
  int postOrderId_ = -1;

  // How many of the previous ids would be considered within
  // this node?
  int numChildIds_ = 0;

 public:
  /**
    Construct an empty ID
   */
  ID() = default;

  /**
    Construct an ID with a symbol path and postorder traversal number
   */
  ID(UniqueString symbolPath, int postOrderId, int numChildIds)
      : symbolPath_(symbolPath),
        postOrderId_(postOrderId),
        numChildIds_(numChildIds) {
  }

  /**
    Return a path to the ID symbol scope. For example, a function 'foo'
    declared in a module M would have symbolPath M.foo.

    Functions, class/record/union/enum declarations, and modules create
    new ID symbol scopes.
   */
  UniqueString symbolPath() const { return symbolPath_; }

  /**
    Returns the numbering of this node in a postorder traversal
    of a symbol's nodes. When the AST node defines a new ID symbol scope,
    (as with Function or Module) this will return -1.
   */
  int postOrderId() const { return postOrderId_; }

  /**
    Return the number of ids contained in this node, not including itself. In
    the postorder traversal numbering, the ids contained appear before the node.

    The node with postorder traversal ID
      postOrderId() - numChildIds()
    is the first node contained within this node.

    E.g. in this notional AST:
      Node(LeafA LeafB)

      LeafA has id 0 and numContainedIds 0
      LeafB has id 1 and numContainedIds 0
      Node  has id 2 and numContainedIds 2

    Note that the number of contained children does not include
    contained IDs with a different symbol scope. So, for example,
    a module consisting only of a function declaration would have
    numContainedChildren() == 0.
   */
  int numContainedChildren() const { return numChildIds_; }

  /**
    Returns a new ID for the parent symbol ID.

    * if postOrderId is >= 0, returns the id with postOrderId == -1
    * if postOrderId is -1, returns the id from removing the
      last '.bla' part from the symbolPath.

    If this ID has no parent, returns an empty ID.

    The returned ID always has numContainedChildren() of 0 and
    it cannot be used with contains(). However it is suitable
    for use in looking up an ID in a map.
   */
  ID parentSymbolId(Context* context) const;

  /**
    returns 'true' if the AST node with this ID contains the AST
    node with the other ID, including if they refer to the same AST node.
   */
  bool contains(const ID& other) const;

  /**
    compare this ID with another ID
      result  < 0 if this < other
      result == 0 if this == other
      result  > 0 if this > other
   */
  int compare(const ID& other) const;

  bool operator==(const ID& other) const {
    (void)numChildIds_; // quiet nextLinter
    return symbolPath_ == other.symbolPath_ &&
           postOrderId_ == other.postOrderId_;
  }

  bool operator!=(const ID& other) const {
    return !(*this == other);
  }

  bool operator<(const ID& other) const {
    return this->compare(other) < 0;
  }
  bool operator<=(const ID& other) const {
    return this->compare(other) <= 0;
  }
  bool operator>(const ID& other) const {
    return this->compare(other) > 0;
  }
  bool operator>=(const ID& other) const {
    return this->compare(other) >= 0;
  }

  bool isEmpty() const {
    return symbolPath_.isEmpty();
  }

  size_t hash() const {
    (void)numChildIds_; // quiet nextLinter
    std::hash<int> hasher;
    return hash_combine(symbolPath_.hash(), hasher(postOrderId_));
  }

  void swap(ID& other) {
    std::swap(symbolPath_, other.symbolPath_);
    std::swap(postOrderId_, other.postOrderId_);
    std::swap(numChildIds_, other.numChildIds_);
  }

  void mark(Context* context) const {
    this->symbolPath_.mark(context);
  }

  static bool update(chpl::ID& keep, chpl::ID& addin);

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond

  std::string str() const;
};

// docs are turned off for this as a workaround for breathe errors
/// \cond DO_NOT_DOCUMENT

/// \endcond


} // end namespace chpl

namespace std {
  template<> struct less<chpl::ID> {
    inline bool operator()(const chpl::ID& lhs, const chpl::ID& rhs) const {
      return lhs.compare(rhs) < 0;
    }
  };
  template<> struct hash<chpl::ID> {
    inline size_t operator()(const chpl::ID& key) const {
      return key.hash();
    }
  };
} // end namespace std

#endif
