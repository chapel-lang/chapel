#include "chpl/AST/ID.h"

namespace chpl {
namespace ast {

ID::ID()
  : symbolPath_(), postOrderId_(-1), numChildIds_(0) {
}

ID::ID(UniqueString symbolPath, int postOrderId, int numChildIds)
  : symbolPath_(symbolPath),
    postOrderId_(postOrderId),
    numChildIds_(numChildIds) {
}

// Returns 'true' if this symbol contains another AST node.
bool ID::contains(const ID other) const {
  UniqueString thisPath = this->symbolPath();
  UniqueString otherPath = other.symbolPath();

  if (thisPath == otherPath) {
    // the nodes have the same parent symbol, so consider the AST ids
    int thisId  = this->postOrderId();
    int thisNContained = this->numContainedChildren();
    int otherId = other.postOrderId();
    int thisFirstContained = thisId - thisNContained;

    return thisFirstContained <= otherId && otherId <= thisId;
  } else {
    // No need to consider the IDs in the event that thisPath
    // is a prefix of otherPath. In that event, they are different
    // symbols, but one is nested inside the other.
    return otherPath.startsWith(thisPath);
  }
}

int ID::compare(const ID other) const {
  // first, compare with the path portion
  UniqueString lhsPath = this->symbolPath();
  UniqueString rhsPath = other.symbolPath();
  int pathCmp = lhsPath.compare(rhsPath);
  if (pathCmp != 0)
    return pathCmp;

  // if that wasn't different, compare the id
  return this->postOrderId() - other.postOrderId();
}

} // end namespace ast
} // end namespace chpl
