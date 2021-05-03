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

#ifndef CHPL_AST_ID_H
#define CHPL_AST_ID_H

#include "chpl/ast/UniqueString.h"

namespace chpl {
namespace uast {


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
  // The symbol itself would be the last node traversed.
  int postOrderId_;
  // How many of the previous ids would be considered within
  // this node?
  int numChildIds_;
 public:
  /**
    Construct an empty ID
   */
  ID();
  /**
    Construct an ID with a symbol path and postorder traversal number
   */
  ID(UniqueString symbolPath, int postOrderId, int numChildIds);
  /**
    Return a path to the symbol (or, for an expression, the parent symbol)
   */
  UniqueString symbolPath() const { return symbolPath_; }
  /**
    Returns the numbering of this node in a postorder traversal
    of a symbol's nodes. This number is not normally relevant
    when the AST in question is a symbol.
   */
  int postOrderId() const { return postOrderId_; }
  /**
    Return the number of ids contained in this node, not including itself. In
    the postorder traversal numbering, the ids contained appear before the node.

    The node with postorder traversal ID
      postOrderId(Node) - numChildIds()
    is the first node contained within this node.

    E.g. in this notional AST:
      Node(LeafA LeafB)

      LeafA has id 0 and numContainedIds 0
      LeafB has id 1 and numContainedIds 0
      Node  has id 2 and numContainedIds 2
   */
  int numContainedChildren() const { return numChildIds_; }

  /**
    returns 'true' if the AST node with this ID contains the AST
    node with the other ID, including if they refer to the same AST node. 
   */
  bool contains(const ID other) const;

  /**
    compare this ID with another ID
      result  < 0 if this < other
      result == 0 if this == other
      result  > 0 if this > other
   */
  int compare(const ID other) const;

  inline bool operator==(const ID other) const {
    return this->symbolPath_ == other.symbolPath_ &&
           this->postOrderId_ == other.postOrderId_;
  }
  inline bool operator!=(const ID other) const {
    return this->symbolPath_ != other.symbolPath_ ||
           this->postOrderId_ != other.postOrderId_;
  }

  size_t hash() const {
    std::hash<int> hasher;
    return hash_combine(symbolPath_.hash(), hasher(postOrderId_));
  }

  void swap(ID& other) {
    ID oldThis = *this;
    *this = other;
    other = oldThis;
  }
};

} // end namespace uast

template<> struct update<chpl::uast::ID> {
  bool operator()(chpl::uast::ID& keep,
                  chpl::uast::ID& addin) const {
    return defaultUpdate(keep, addin);
  }
};

// Allow chpl::uast::ID to be just called chpl::ID
// TODO: Should it be moved out of the uast namespace? What directory
// should it go in?
using chpl::uast::ID;


} // end namespace chpl

namespace std {
  template<> struct less<chpl::uast::ID> {
    bool operator()(const chpl::uast::ID lhs, const chpl::uast::ID rhs) const {
      return lhs.compare(rhs) < 0;
    }
  };
  template<> struct hash<chpl::uast::ID> {
    size_t operator()(const chpl::uast::ID key) const {
      return (size_t) key.hash();
    }
  };
  template<> struct equal_to<chpl::uast::ID> {
    bool operator()(const chpl::uast::ID lhs,
                    const chpl::uast::ID rhs) const {
      return lhs == rhs;
    }
  };
} // end namespace std

#endif
