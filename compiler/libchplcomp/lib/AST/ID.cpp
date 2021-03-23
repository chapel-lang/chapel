#include "chpl/AST/BaseAST.h"

namespace chpl {

  // Returns 'true' if this symbol contains another AST node.
  bool ID::contains(const ID other) const {
    UniqueString thisPath = this->symbolPath();
    UniqueString otherPath = other.symbolPath();

    if (thisPath == otherPath) {
      // the nodes have the same parent symbol, so consider the AST ids
      int thisId  = this->postOrderId();
      int thisNContained = this->numContainedIds();
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
}
