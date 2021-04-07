#ifndef CHPL_AST_ID_H
#define CHPL_AST_ID_H

#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

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
  int numContainedIds_;
 public:
  ID() { }
  // Return a path to the symbol (or, for an expression, the parent symbol)
  UniqueString symbolPath() const { return symbolPath_; }
  // Returns the numbering of this node in a postorder traversal
  // of a symbol's nodes. This number is not normally relevant
  // when the AST in question is a symbol.
  int postOrderId() const { return postOrderId_; }
  // Return the number of ids contained in this node. In the postorder
  // traversal numbering, the ids contained appear before the node.
  //
  // The node with postorder traversal ID 
  //   postOrderId(Node) - numContainedIds()
  // is the first node contained within this node.

  // E.g. in this notional AST:
  //   Node(LeafA LeafB)  
  //
  //   LeafA has id 0 and numContainedIds 0
  //   LeafB has id 1 and numContainedIds 0
  //   Node  has id 2 and numContainedIds 2
  int numContainedIds() const { return numContainedIds_; }

  // returns 'true' if the AST node with this ID contains the AST
  // node with the other ID.
  bool contains(const ID other) const;

  int compare(const ID other) const;
};

} // end namespace ast
} // end namespace chpl

namespace std {
  template<> struct less<chpl::ast::ID> {
    bool operator()(const chpl::ast::ID lhs, const chpl::ast::ID rhs) const {
      return lhs.compare(rhs) < 0;
    }
  };
} // end namespace std

#endif
