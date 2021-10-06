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

#ifndef CHPL_UAST_BUILDERRESULT_H
#define CHPL_UAST_BUILDERRESULT_H

#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/mark-functions.h"
#include "chpl/queries/update-functions.h"
#include "chpl/uast/ASTNode.h"

#include <vector>
#include <unordered_map>
#include <utility>

namespace chpl {

class Context;
class Location;

namespace uast {
  class BuilderResult;
}

namespace parsing {
  // forward declare for use in friend declaration
  const uast::BuilderResult& parseFile(Context* context, UniqueString path);
}

namespace uast {

/**
  This type records the result of building some AST.
 */
class BuilderResult final {
  friend class Builder;
  friend const BuilderResult& chpl::parsing::parseFile(Context* context,
                                                       UniqueString path);

 private:
  UniqueString filePath_;
  ASTList topLevelExpressions_;
  std::vector<ErrorMessage> errors_;

  // Given an ID, what is the ASTNode?
  std::unordered_map<ID, const ASTNode*> idToAst_;

  // Given an ID, what is the parent ID?
  std::unordered_map<ID, ID> idToParentId_;

  // Goes from ASTNode* to Location because Comments don't have AST IDs
  std::unordered_map<const ASTNode*, Location> astToLocation_;

 private: 
  static void updateFilePaths(Context* context, const BuilderResult& keep);

 public:
  /* Construct an empty BuilderResult */
  BuilderResult();
  /* Construct a BuilderResult that records a particular file path. */
  BuilderResult(UniqueString filePath);

  /* Return the file path this result refers to */
  UniqueString filePath() const {
    return filePath_;
  }

  /* return the number of top-level expressions */
  int numTopLevelExpressions() const {
    return topLevelExpressions_.size();
  }

  /* return the i'th top-level expression */
  const ASTNode* topLevelExpression(int i) const {
    assert(0 <= i && (size_t) i < topLevelExpressions_.size());
    return topLevelExpressions_[i].get();
  }

  /* iterate over the parsed top-level expressions */
  ASTListIteratorPair<ASTNode> topLevelExpressions() const {
    return ASTListIteratorPair<ASTNode>(topLevelExpressions_.begin(),
                                        topLevelExpressions_.end());
  }

  /* If the top-level expressions contain only a single Module,
     return it. Otherwise, return nullptr. */
  const Module* singleModule() {
    if (topLevelExpressions_.size() == 1) {
      return topLevelExpressions_[0]->toModule();
    }

    return nullptr;
  }


  /* return the number of errors */
  int numErrors() const {
    return errors_.size();
  }

  /* return the i'th error */
  ErrorMessage error(int i) const {
    return errors_[i];
  }

  // TODO: add something to iterate over the errors e.g.
  // ErrorListIteratorPair errors() const

  /* Find the ASTNode* corresponding to a particular ID, or return
     nullptr if there is not one in this result. */
  const ASTNode* idToAst(ID id) const;
  /* Find the Location for a particular ID.
     Returns a location just to path if none is found. */
  Location idToLocation(ID id, UniqueString path) const;
  /* Find the ID for a parent given an ID.
     Returns the empty ID if none is found */
  ID idToParentId(ID id) const;

  BuilderResult(BuilderResult&&) = default; // move-constructable
  BuilderResult(const BuilderResult&) = delete; // not copy-constructable
  BuilderResult& operator=(const BuilderResult&) = delete; // not assignable

  void swap(BuilderResult& other);

  static bool update(BuilderResult& keep, BuilderResult& addin);
  static void mark(Context* context, const BuilderResult& keep);
};

} // end namespace uast

// docs are turned off for this as a workaround for breathe errors
/// \cond DO_NOT_DOCUMENT
template<> struct update<chpl::uast::BuilderResult> {
  bool operator()(chpl::uast::BuilderResult& keep,
                  chpl::uast::BuilderResult& addin) const {
    return chpl::uast::BuilderResult::update(keep, addin);
  }
};
template<> struct mark<chpl::uast::BuilderResult> {
  void operator()(Context* context,
                  const chpl::uast::BuilderResult& keep) const {
    chpl::uast::BuilderResult::mark(context, keep);
  }
};
/// \endcond

} // end namespace chpl


#endif
