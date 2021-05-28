/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include <string>
#include <vector>

static bool locationLessEq(YYLTYPE lhs, YYLTYPE rhs) {
  return (lhs.first_line < rhs.first_line) ||
         (lhs.first_line == rhs.first_line &&
          lhs.first_column <= rhs.first_column);
}

std::vector<ParserComment>* ParserContext::gatherComments(YYLTYPE location) {

  // If there were no comments, there is nothing to do.
  if (this->comments == nullptr) {
    return nullptr;
  }

  if (this->comments->size() == 0) {
    delete this->comments;
    this->comments = nullptr;
    return nullptr;
  }

  // Otherwise, gather only those comments that appear before the location.
  // This might be all of the comments or only some of them.

  ssize_t lastCommentToGather = -1;
  {
    size_t i = 0;
    for (ParserComment comment : *this->comments) {
      if (locationLessEq(comment.location, location)) {
        // OK, we can gather this comment (and any earlier ones)
        lastCommentToGather = i;
      }
      i++;
    }
  }

  // now, return the comments up to and including lastCommentToGather

  if (lastCommentToGather < 0) {
    // don't need to return any comments
    return nullptr;
  }

  if (lastCommentToGather == (int)this->comments->size()-1) {
    // need to return all comments
    std::vector<ParserComment>* ret = this->comments;
    this->comments = nullptr;
    return ret;
  }

  // general case: return only the comments up to lastCommentToGather
  std::vector<ParserComment>* ret = new std::vector<ParserComment>();
  for (int i = 0; i <= lastCommentToGather; i++) {
    ret->push_back((*this->comments)[i]);
  }
  this->comments->erase(this->comments->begin(),
                        this->comments->begin()+lastCommentToGather+1);
  return ret;
}

void ParserContext::noteDeclStartLoc(YYLTYPE loc) {
  if (this->declStartLocation.first_line == 0) {
    this->declStartLocation = loc;
  }
}
Decl::Visibility ParserContext::noteVisibility(Decl::Visibility visibility) {
  this->visibility = visibility;
  return this->visibility;
}
Variable::Kind ParserContext::noteVarDeclKind(Variable::Kind varDeclKind) {
  this->varDeclKind = varDeclKind;
  return this->varDeclKind;
}
YYLTYPE ParserContext::declStartLoc(YYLTYPE curLoc) {
  if (this->declStartLocation.first_line == 0)
    return curLoc;
  else
    return this->declStartLocation;
}
void ParserContext::resetDeclState() {
  this->varDeclKind = Variable::VAR;
  this->visibility = Decl::DEFAULT_VISIBILITY;
  YYLTYPE emptyLoc = {0};
  this->declStartLocation = emptyLoc;
}

void ParserContext::noteComment(YYLTYPE loc, const char* data, long size) {
  if (this->comments == nullptr) {
    this->comments = new std::vector<ParserComment>();
  }
  ParserComment c;
  c.location = loc;
  Location ll = this->convertLocation(loc);
  auto comment = Comment::build(this->builder, ll, std::string(data, size));
  free((void*)data);
  c.comment = comment.release();
  this->comments->push_back(c);
}

void ParserContext::clearCommentsBefore(YYLTYPE loc) {
  auto comments = this->gatherComments(loc);
  if (comments != nullptr) {
    for (ParserComment parserComment : *comments) {
      delete parserComment.comment;
    }
    delete comments;
  }
}
void ParserContext::clearComments() {
  if (this->comments != nullptr) {
    for (ParserComment parserComment : *this->comments) {
      delete parserComment.comment;
    }
    this->comments->clear();
  }
}

ParserExprList* ParserContext::makeList() {
  return new ParserExprList();
}
ParserExprList* ParserContext::makeList(ParserExprList* lst) {
  return lst;
}
ParserExprList* ParserContext::makeList(Expression* e) {
  ParserExprList* ret = new ParserExprList();
  ret->push_back(e);
  return ret;
}
ParserExprList* ParserContext::makeList(CommentsAndStmt cs) {
  ParserExprList* ret = new ParserExprList();
  this->appendList(ret, cs);
  return ret;
}

ParserExprList* ParserContext::appendList(ParserExprList* dst,
                                          ParserExprList* lst) {
  for (Expression* elt : *lst) {
    dst->push_back(elt);
  }
  delete lst;
  return dst;
}

ParserExprList* ParserContext::appendList(ParserExprList* dst, Expression* e) {
  dst->push_back(e);
  return dst;
}

ParserExprList* ParserContext::appendList(ParserExprList* dst,
                                          std::vector<ParserComment>* comments) {
  if (comments != nullptr) {
    for (ParserComment parserComment : *comments) {
      Comment* c = parserComment.comment;
      dst->push_back(c);
      this->commentLocations.insert({(void*)c, parserComment.location});
    }
    delete comments;
  }
  return dst;
}

ParserExprList* ParserContext::appendList(ParserExprList* dst,
                                          CommentsAndStmt cs) {
  // append the comments
  this->appendList(dst, cs.comments);
  // then append the statement
  if (cs.stmt != nullptr) {
    dst->push_back(cs.stmt);
  }
  return dst;
}

ASTList ParserContext::consumeList(ParserExprList* lst) {
  ASTList ret;
  if (lst != nullptr) {
    for (Expression* e : *lst) {
      ret.push_back(toOwned(e));
    }
    delete lst;
  }
  return ret;
}

void ParserContext::consumeNamedActuals(MaybeNamedActualList* lst,
                                        ASTList& actualsOut,
                                        std::vector<UniqueString>& namesOut) {
  bool anyActualNames = false;
  if (lst != nullptr) {
    for (auto& elt : *lst) {
      if (!elt.name.isEmpty())
        anyActualNames = true;
    }
    for (auto& elt : *lst) {
      actualsOut.push_back(toOwned(elt.expr));
      if (anyActualNames)
        namesOut.push_back(elt.name);
    }
    delete lst;
  }
}

std::vector<ParserComment>*
ParserContext::gatherCommentsFromList(ParserExprList* lst,
                                      YYLTYPE location) {
  if (lst == nullptr || lst->size() == 0) {
    // no list, so nothing to do
    return nullptr;
  }

  size_t nToMove = 0;
  while (lst->size() > nToMove) {
    Expression* e = (*lst)[nToMove];
    if (Comment* c = e->toComment()) {
      auto search = this->commentLocations.find(c);
      assert(search != this->commentLocations.end());
      YYLTYPE commentLocation = search->second;
      if (locationLessEq(commentLocation, location)) {
        nToMove++;
        continue;
      }
    }
    break;
  }

  if (nToMove == 0) {
    return nullptr;
  }

  std::vector<ParserComment>* ret = new std::vector<ParserComment>();
  for (size_t i = 0; i < nToMove; i++) {
    Comment* c = (*lst)[i]->toComment();
    assert(c);
    auto search = this->commentLocations.find(c);
    assert(search != this->commentLocations.end());
    YYLTYPE commentLocation = search->second;
    ParserComment pc;
    pc.location = commentLocation;
    pc.comment = c;
    ret->push_back(pc);
  }

  lst->erase(lst->begin(), lst->begin()+nToMove);

  return ret;
}

void ParserContext::discardCommentsFromList(ParserExprList* lst,
                                            YYLTYPE loc) {
  if (lst == nullptr) return;

  auto comments = gatherCommentsFromList(lst, loc);
  if (comments != nullptr) {
    for (ParserComment parserComment : *comments) {
      delete parserComment.comment;
    }
    delete comments;
  }
}

void ParserContext::appendComments(CommentsAndStmt*cs,
                                   std::vector<ParserComment>* comments) {
  if (comments == nullptr) return;

  if (cs->comments == nullptr) {
    cs->comments = comments;
    return;
  }

  // otherwise, append them and then delete comments
  for (ParserComment parserComment : *comments) {
    cs->comments->push_back(parserComment);
  }

  delete comments;
}

CommentsAndStmt ParserContext::finishStmt(CommentsAndStmt cs) {
  this->clearComments();
  return cs;
}
CommentsAndStmt ParserContext::finishStmt(Expression* e) {
  this->clearComments();
  return makeCommentsAndStmt(NULL, e);
}

ParserExprList*
ParserContext::blockToParserExprList(YYLTYPE lbrLoc, YYLTYPE rbrLoc,
                                     ParserExprList* body) {
  this->clearCommentsBefore(lbrLoc);
  ParserExprList* ret = body != nullptr ? body : new ParserExprList();
  this->appendList(ret, this->gatherComments(rbrLoc));
  return ret;
}

Location ParserContext::convertLocation(YYLTYPE location) {
  return Location(this->filename,
                  location.first_line,
                  location.first_column,
                  location.last_line,
                  location.last_column);
}

Identifier* ParserContext::buildEmptyIdent(YYLTYPE location) {
  UniqueString empty;
  return Identifier::build(builder, convertLocation(location), empty).release();
}
Identifier* ParserContext::buildIdent(YYLTYPE location, PODUniqueString name) {
  return Identifier::build(builder, convertLocation(location), name).release();
}

OpCall* ParserContext::buildBinOp(YYLTYPE location,
                                  Expression* lhs,
                                  PODUniqueString op,
                                  Expression* rhs) {
  return OpCall::build(builder, convertLocation(location),
                       op, toOwned(lhs), toOwned(rhs)).release();
}
OpCall* ParserContext::buildUnaryOp(YYLTYPE location,
                                    PODUniqueString op,
                                    Expression* expr) {
  return OpCall::build(builder, convertLocation(location),
                       op, toOwned(expr)).release();
}

FunctionParts ParserContext::makeFunctionParts(bool isInline,
                                               bool isOverride) {
  FunctionParts fp = {nullptr,
                      nullptr,
                      this->visibility,
                      Function::DEFAULT_LINKAGE,
                      nullptr,
                      isInline,
                      isOverride,
                      Function::PROC,
                      nullptr,
                      PODUniqueString::build(),
                      Function::DEFAULT_RETURN_INTENT,
                      false,
                      nullptr, nullptr, nullptr, nullptr,
                      nullptr};
  return fp;
}

CommentsAndStmt ParserContext::buildFunctionDecl(YYLTYPE location,
                                                 FunctionParts& fp) {
  CommentsAndStmt cs = {fp.comments, nullptr};
  if (fp.errorExpr == nullptr) {
    auto f = Function::build(builder, this->convertLocation(location),
                             fp.name, this->visibility,
                             fp.linkage, toOwned(fp.linkageNameExpr),
                             fp.isInline,
                             fp.isOverride,
                             fp.kind,
                             toOwned(fp.receiver),
                             fp.returnIntent,
                             fp.throws,
                             this->consumeList(fp.formals),
                             toOwned(fp.returnType),
                             toOwned(fp.where),
                             this->consumeList(fp.lifetime),
                             this->consumeList(fp.body));
    cs.stmt = f.release();
  } else {
    cs.stmt = fp.errorExpr;
  }
  this->clearComments();
  return cs;
}

// TODO: Need way to clear location of 'e' in the builder.
owned<Decl> ParserContext::buildLoopIndexDecl(YYLTYPE location,
                                              owned<Expression> e) {
  if (const Identifier* ident = e->toIdentifier()) {
    return Variable::build(builder, convertLocation(location),
                           ident->name(),
                           Decl::DEFAULT_VISIBILITY,
                           Variable::INDEX,
                           /*typeExpression*/ nullptr,
                           /*initExpression*/ nullptr);
  } else {
    noteError(location, "Cannot handle this kind of index var");
  }

  return nullptr;
}

FnCall* ParserContext::wrapCalledExpressionInNew(YYLTYPE location,
                                                 New::Management management,
                                                 FnCall* fnCall) {
  assert(fnCall->calledExpression());
  bool wrappedBaseExpression = false;

  // Find the child slot containing the called expression. Then remove it,
  // wrap it in a new expression, and swap in the new expression.
  for (auto& child : builder->mutableRefToChildren(fnCall)) {
    if (child.get() == fnCall->calledExpression()) {
      auto calledExpr = std::move(child).release()->toExpression();
      assert(calledExpr);
      auto newExpr = New::build(builder, convertLocation(location),
                                toOwned(calledExpr),
                                management);
      child = std::move(newExpr);
      wrappedBaseExpression = true;
      break;
    }
  }

  assert(wrappedBaseExpression);

  return fnCall;
}

BlockStyle ParserContext::determineBlockStyle(BlockOrDo blockOrDo) {
  BlockStyle ret = blockOrDo.usesDo ? BlockStyle::IMPLICIT
                                    : BlockStyle::EXPLICIT;
  auto exprLst = blockOrDo.exprList;

  int numNonCommentExprs = 0;
  bool foundBlock = false;

  // Look for one non-comment expression that is a block.
  for (auto expr : *exprLst) {
    numNonCommentExprs += !expr->isComment() ? 1 : 0;
    foundBlock = expr->isBlock();
    if (numNonCommentExprs > 1) break;
  }

  if (numNonCommentExprs == 1 && foundBlock &&
      ret == BlockStyle::IMPLICIT) {
    ret = BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK;
  }

  return ret;
}

ASTList
ParserContext::consumeAndFlattenTopLevelBlocks(BlockOrDo blockOrDo) {
  return builder->flattenTopLevelBlocks(consumeList(blockOrDo.exprList));
}

CommentsAndStmt
ParserContext::buildBracketLoopStmt(YYLTYPE locLeftBracket,
                                    YYLTYPE locIndex,
                                    ParserExprList* indexExprs,
                                    Expression* iterandExpr,
                                    WithClause* withClause,
                                    CommentsAndStmt stmt) {
  // Should not be nullptr, use other overload instead.
  assert(indexExprs && indexExprs->size() >= 1);

  auto blockStyle = stmt.stmt->isBlock() ? BlockStyle::EXPLICIT
                                         : BlockStyle::IMPLICIT;

  auto exprLst = makeList(stmt);
  auto comments = gatherCommentsFromList(exprLst, locLeftBracket);
  Expression* indexExpr = nullptr;

  if (indexExprs->size() > 1) {
    const char* msg = "Invalid index expression";
    return { .comments=comments, .stmt=raiseError(locIndex, msg) };
  } else {
    auto uncastedIndexExpr = consumeList(indexExprs)[0].release();
    indexExpr = uncastedIndexExpr->toExpression();
  }

  assert(indexExpr);
  auto index = buildLoopIndexDecl(locIndex, toOwned(indexExpr));

  auto astLst = consumeList(exprLst);
  auto statements = builder->flattenTopLevelBlocks(std::move(astLst));
  auto node = BracketLoop::build(builder, convertLocation(locLeftBracket),
                                 std::move(index),
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(statements),
                                 /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildBracketLoopStmt(YYLTYPE locLeftBracket,
                                                    YYLTYPE locIterExprs,
                                                    ParserExprList* iterExprs,
                                                    WithClause* withClause,
                                                    CommentsAndStmt stmt) {
  assert(iterExprs && iterExprs->size() >= 1);

  auto blockStyle = stmt.stmt->isBlock() ? BlockStyle::EXPLICIT
                                         : BlockStyle::IMPLICIT;

  auto exprLst = makeList(stmt);
  auto comments = gatherCommentsFromList(exprLst, locLeftBracket);
  Expression* iterandExpr = nullptr;

  if (iterExprs->size() > 1) {
    const char* msg = "Invalid iterand expression";
    return { .comments=comments, .stmt=raiseError(locIterExprs, msg) };
  } else {
    auto uncastedIterandExpr = consumeList(iterExprs)[0].release();
    iterandExpr = uncastedIterandExpr->toExpression();
  }

  assert(iterandExpr);

  auto astLst = consumeList(exprLst);
  auto statements = builder->flattenTopLevelBlocks(std::move(astLst));
  auto node = BracketLoop::build(builder, convertLocation(locLeftBracket),
                                 /*index*/ nullptr,
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(statements),
                                 /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildForallLoopStmt(YYLTYPE locForall,
                                                   YYLTYPE locIndex,
                                                   Expression* indexExpr,
                                                   Expression* iterandExpr,
                                                   WithClause* withClause,
                                                   BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;
  auto comments = gatherCommentsFromList(blockOrDo.exprList, locForall);
  auto blockStyle = determineBlockStyle(blockOrDo);
  auto statements = consumeAndFlattenTopLevelBlocks(blockOrDo);
  auto node = Forall::build(builder, convertLocation(locForall),
                            std::move(index),
                            toOwned(iterandExpr),
                            toOwned(withClause),
                            blockStyle,
                            std::move(statements),
                            /*isExpressionLevel*/ false);
  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildForeachLoopStmt(YYLTYPE locForeach,
                                                    YYLTYPE locIndex,
                                                    Expression* indexExpr,
                                                    Expression* iterandExpr,
                                                    WithClause* withClause,
                                                    BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;
  auto comments = gatherCommentsFromList(blockOrDo.exprList, locForeach);
  auto blockStyle = determineBlockStyle(blockOrDo);
  auto statements = consumeAndFlattenTopLevelBlocks(blockOrDo);
  auto node = Foreach::build(builder, convertLocation(locForeach),
                             std::move(index),
                             toOwned(iterandExpr),
                             toOwned(withClause),
                             blockStyle,
                             std::move(statements));

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildForLoopStmt(YYLTYPE locFor,
                                                YYLTYPE locIndex,
                                                Expression* indexExpr,
                                                Expression* iterandExpr,
                                                BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;
  auto comments = gatherCommentsFromList(blockOrDo.exprList, locFor);
  auto blockStyle = determineBlockStyle(blockOrDo);
  auto statements = consumeAndFlattenTopLevelBlocks(blockOrDo);
  auto node = For::build(builder, convertLocation(locFor),
                         std::move(index),
                         toOwned(iterandExpr),
                         blockStyle,
                         std::move(statements),
                         /*isExpressionLevel*/ false,
                         /*isParam*/ false);
  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildCoforallLoopStmt(YYLTYPE locCoforall,
                                                     YYLTYPE locIndex,
                                                     Expression* indexExpr,
                                                     Expression* iterandExpr,
                                                     WithClause* withClause,
                                                     BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;
  auto comments = gatherCommentsFromList(blockOrDo.exprList, locCoforall);
  auto blockStyle = determineBlockStyle(blockOrDo);
  auto statements = consumeAndFlattenTopLevelBlocks(blockOrDo);
  auto node = Coforall::build(builder, convertLocation(locCoforall),
                              std::move(index),
                              toOwned(iterandExpr),
                              toOwned(withClause),
                              blockStyle,
                              std::move(statements));

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt
ParserContext::buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                    YYLTYPE locCondition,
                                    YYLTYPE locThen,
                                    Expression* condition,
                                    CommentsAndStmt thenStmt) {

  auto thenBlockStyle = usesThenKeyword ? BlockStyle::IMPLICIT
                                        : BlockStyle::EXPLICIT;

  // E.g. 'if true then { ... }'
  if (thenBlockStyle == BlockStyle::IMPLICIT &&
      thenStmt.stmt->isBlock()) {
    thenBlockStyle = BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK;
  }

  auto thenExprLst = makeList(thenStmt);
  auto comments = gatherCommentsFromList(thenExprLst, locIf);

  // If there's a 'then' keyword, discard any comments before the 'then'.
  // Else, discard any comments before the condition.
  auto discardPoint = usesThenKeyword ? locThen : locCondition;
  discardCommentsFromList(thenExprLst, discardPoint);

  auto thenAstLst = consumeList(thenExprLst);
  auto thenStmts = builder->flattenTopLevelBlocks(std::move(thenAstLst));

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenStmts));

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt
ParserContext::buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                    YYLTYPE locCondition,
                                    YYLTYPE locThen,
                                    YYLTYPE locElse,
                                    Expression* condition,
                                    CommentsAndStmt thenStmt,
                                    CommentsAndStmt elseStmt) {

  auto thenBlockStyle = usesThenKeyword ? BlockStyle::IMPLICIT
                                        : BlockStyle::EXPLICIT;

  // E.g. 'if true then { ... }'
  if (thenBlockStyle == BlockStyle::IMPLICIT &&
      thenStmt.stmt->isBlock()) {
    thenBlockStyle = BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK;
  }

  // Else block can only be IMPLICIT or EXPLICIT.
  auto elseBlockStyle = elseStmt.stmt->isBlock() ? BlockStyle::EXPLICIT
                                                 : BlockStyle::IMPLICIT;

  auto thenExprLst = makeList(thenStmt);
  auto comments = gatherCommentsFromList(thenExprLst, locIf);

  // If there's a 'then' keyword, discard any comments before the 'then'.
  // Else, discard any comments before the condition.
  auto commentsToDiscard = usesThenKeyword
      ? gatherCommentsFromList(thenExprLst, locThen)
      : gatherCommentsFromList(thenExprLst, locCondition);
  delete commentsToDiscard;

  auto elseExprLst = makeList(elseStmt);

  // Move any comments before the 'else' into the 'thenExprLst'.
  auto commentsBeforeElse = gatherCommentsFromList(elseExprLst, locElse);
  thenExprLst = appendList(thenExprLst, commentsBeforeElse);

  // Create the 'then' statement list.
  auto thenAstLst = consumeList(thenExprLst);
  auto thenStmts = builder->flattenTopLevelBlocks(std::move(thenAstLst));

  // Create the 'else' statement list.
  auto elseAstLst = consumeList(elseExprLst);
  auto elseStmts = builder->flattenTopLevelBlocks(std::move(elseAstLst));

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenStmts),
                                 elseBlockStyle,
                                 std::move(elseStmts),
                                 /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}
