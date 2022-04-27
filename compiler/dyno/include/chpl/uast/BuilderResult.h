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

#ifndef CHPL_UAST_BUILDERRESULT_H
#define CHPL_UAST_BUILDERRESULT_H

#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/mark-functions.h"
#include "chpl/queries/update-functions.h"
#include "chpl/uast/AstNode.h"
#include "chpl/util/iteration.h"
#include "chpl/queries/stringify-functions.h"

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
  AstList topLevelExpressions_;
  std::vector<ErrorMessage> errors_;

  // Given an ID, what is the AstNode?
  std::unordered_map<ID, const AstNode*> idToAst_;

  // Given an ID, what is the parent ID?
  std::unordered_map<ID, ID> idToParentId_;

  // Goes from ID to Location, applies to all AST nodes except Comment
  std::unordered_map<ID, Location> idToLocation_;

  // Goes from Comment ID to Location, applies to all AST nodes except Comment
  std::vector<Location> commentIdToLocation_;

 private:
  static void updateFilePaths(Context* context, const BuilderResult& keep);

 public:
  /** Construct an empty BuilderResult */
  BuilderResult();
  /** Construct a BuilderResult that records a particular file path. */
  BuilderResult(UniqueString filePath);

  /** Return the file path this result refers to */
  UniqueString filePath() const {
    return filePath_;
  }

  /** return the number of top-level expressions */
  int numTopLevelExpressions() const {
    return topLevelExpressions_.size();
  }

  /** return the i'th top-level expression */
  const AstNode* topLevelExpression(int i) const {
    assert(0 <= i && (size_t) i < topLevelExpressions_.size());
    return topLevelExpressions_[i].get();
  }

  /** iterate over the parsed top-level expressions */
  AstListIteratorPair<AstNode> topLevelExpressions() const {
    return AstListIteratorPair<AstNode>(topLevelExpressions_.begin(),
                                        topLevelExpressions_.end());
  }

  /** If the top-level expressions contain only a single Module,
     return it. Otherwise, return nullptr. */
  const Module* singleModule() {
    if (topLevelExpressions_.size() == 1) {
      return topLevelExpressions_[0]->toModule();
    }

    return nullptr;
  }


  /** return the number of errors */
  int numErrors() const {
    return errors_.size();
  }

  /** return the i'th error */
  const ErrorMessage& error(int i) const {
    return errors_[i];
  }

  // An iterable over the errors of this.
  using ErrorIterable = Iterable<std::vector<ErrorMessage>>;

  /**
    Iterate over the errors.
  */
  ErrorIterable errors() const {
    return ErrorIterable(errors_);
  }

  /** Find the AstNode* corresponding to a particular ID, or return
      nullptr if there is not one in this result. */
  const AstNode* idToAst(ID id) const;
  /** Find the Location for a particular ID.
      Returns a location just to path if none is found. */
  Location idToLocation(ID id, UniqueString path) const;
  /** Find the Location for a particular comment.
      The Comment must have been from this BuilderResult, but this is not
      checked.
      An empty Location will be returned if the Comment couldn't be found
  */
  Location commentToLocation(const Comment *comment) const;
  /** Find the ID for a parent given an ID.
      Returns the empty ID if none is found */
  ID idToParentId(ID id) const;

  BuilderResult(BuilderResult&&) = default; // move-constructable
  BuilderResult(const BuilderResult&) = delete; // not copy-constructable
  BuilderResult& operator=(const BuilderResult&) = delete; // not assignable

  void swap(BuilderResult& other);

  static bool update(BuilderResult& keep, BuilderResult& addin);
  void mark(Context* context) const;
};


} // end namespace uast



} // end namespace chpl


#endif
