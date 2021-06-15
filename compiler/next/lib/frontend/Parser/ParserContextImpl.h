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

#include <cerrno>
#include <cfloat>
#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <cstdio>
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
  clearComments(comments);
}

void ParserContext::clearComments(std::vector<ParserComment>* comments) {
  if (comments != nullptr) {
    for (ParserComment parserComment : *comments) {
      delete parserComment.comment;
    }
    delete comments;
  }
}

void ParserContext::clearComments(ParserExprList* comments) {
  if (comments != nullptr) {
    for (Expression* expr : *comments) {
      assert(expr->isComment());
      delete expr;
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
  assert(e != nullptr);
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
  if (lst == nullptr) return dst;
  for (Expression* elt : *lst) {
    dst->push_back(elt);
  }
  delete lst;
  return dst;
}

ParserExprList* ParserContext::appendList(ParserExprList* dst, Expression* e) {
  assert(e != nullptr);
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

ASTList
ParserContext::consumeAndFlattenTopLevelBlocks(ParserExprList* exprLst) {
  return builder->flattenTopLevelBlocks(consumeList(exprLst));
}

void
ParserContext::prepareStmtPieces(std::vector<ParserComment>*& outComments,
                                 ParserExprList*& outExprLst,
                                 BlockStyle& outBlockStyle,
                                 YYLTYPE locStartKeyword,
                                 bool isBodyAnchorSecondKeyword,
                                 YYLTYPE locBodyAnchor,
                                 CommentsAndStmt consume) {
  auto comments = consume.comments;
  auto bodyStmt = consume.stmt;

  auto blockStyle = BlockStyle::IMPLICIT;

  // The body statement can be nullptr.
  if (bodyStmt && bodyStmt->isBlock()) {
    if (isBodyAnchorSecondKeyword) {
      blockStyle = BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK;
    } else {
      blockStyle = BlockStyle::EXPLICIT;
    }
  }

  // Set the block style of the statement.
  outBlockStyle = blockStyle;

  auto csCommentLst = appendList(makeList(), comments);

  // Set the comments preceding the statement.
  outComments = gatherCommentsFromList(csCommentLst, locStartKeyword);

  // If there is no body statement, or the body statement is a block, clear
  // all remaining comments. Else just clear the comments that precede the
  // anchor point.
  if (csCommentLst) {
    if (!bodyStmt || bodyStmt->isBlock()) {
      clearComments(csCommentLst);
      csCommentLst = nullptr;
    } else {
      discardCommentsFromList(csCommentLst, locBodyAnchor);
    }
  }

  auto bodyExprLst = bodyStmt ? makeList(bodyStmt) : nullptr;

  outExprLst = csCommentLst ? appendList(csCommentLst, bodyExprLst)
                            : bodyExprLst;
}

void
ParserContext::prepareStmtPieces(std::vector<ParserComment>*& outComments,
                                 ParserExprList*& outExprLst,
                                 BlockStyle& outBlockStyle,
                                 YYLTYPE locStartKeyword,
                                 YYLTYPE locBodyAnchor,
                                 BlockOrDo consume) {
  return prepareStmtPieces(outComments, outExprLst, outBlockStyle,
                           locStartKeyword,
                           consume.usesDo,
                           locBodyAnchor,
                           consume.cs);
}

CommentsAndStmt
ParserContext::buildBracketLoopStmt(YYLTYPE locLeftBracket,
                                    YYLTYPE locIndex,
                                    YYLTYPE locBodyAnchor,
                                    ParserExprList* indexExprs,
                                    Expression* iterandExpr,
                                    WithClause* withClause,
                                    CommentsAndStmt cs) {
  // Should not be nullptr, use other overload instead.
  assert(indexExprs && indexExprs->size() >= 1);

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locLeftBracket,
                    false,
                    locBodyAnchor,
                    cs);

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

  auto stmts = consumeAndFlattenTopLevelBlocks(exprLst);

  auto node = BracketLoop::build(builder, convertLocation(locLeftBracket),
                                 std::move(index),
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(stmts),
                                 /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildBracketLoopStmt(YYLTYPE locLeftBracket,
                                                    YYLTYPE locIterExprs,
                                                    YYLTYPE locBodyAnchor,
                                                    ParserExprList* iterExprs,
                                                    WithClause* withClause,
                                                    CommentsAndStmt cs) {
  assert(iterExprs && iterExprs->size() >= 1);

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locLeftBracket,
                    false,
                    locBodyAnchor,
                    cs);

  Expression* iterandExpr = nullptr;

  if (iterExprs->size() > 1) {
    const char* msg = "Invalid iterand expression";
    return { .comments=comments, .stmt=raiseError(locIterExprs, msg) };
  } else {
    auto uncastedIterandExpr = consumeList(iterExprs)[0].release();
    iterandExpr = uncastedIterandExpr->toExpression();
  }

  assert(iterandExpr);

  auto stmts = consumeAndFlattenTopLevelBlocks(exprLst);

  auto node = BracketLoop::build(builder, convertLocation(locLeftBracket),
                                 /*index*/ nullptr,
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(stmts),
                                 /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildForallLoopStmt(YYLTYPE locForall,
                                                   YYLTYPE locIndex,
                                                   YYLTYPE locBodyAnchor,
                                                   Expression* indexExpr,
                                                   Expression* iterandExpr,
                                                   WithClause* withClause,
                                                   BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locForall,
                    locBodyAnchor,
                    blockOrDo);

  auto stmts = consumeAndFlattenTopLevelBlocks(exprLst);

  auto node = Forall::build(builder, convertLocation(locForall),
                            std::move(index),
                            toOwned(iterandExpr),
                            toOwned(withClause),
                            blockStyle,
                            std::move(stmts),
                            /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildForeachLoopStmt(YYLTYPE locForeach,
                                                    YYLTYPE locIndex,
                                                    YYLTYPE locBodyAnchor,
                                                    Expression* indexExpr,
                                                    Expression* iterandExpr,
                                                    WithClause* withClause,
                                                    BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locForeach,
                    locBodyAnchor,
                    blockOrDo);

  auto stmts = consumeAndFlattenTopLevelBlocks(exprLst);

  auto node = Foreach::build(builder, convertLocation(locForeach),
                             std::move(index),
                             toOwned(iterandExpr),
                             toOwned(withClause),
                             blockStyle,
                             std::move(stmts));

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildForLoopStmt(YYLTYPE locFor,
                                                YYLTYPE locIndex,
                                                YYLTYPE locBodyAnchor,
                                                Expression* indexExpr,
                                                Expression* iterandExpr,
                                                BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locFor,
                    locBodyAnchor,
                    blockOrDo);

  auto stmts = consumeAndFlattenTopLevelBlocks(exprLst);

  auto node = For::build(builder, convertLocation(locFor),
                         std::move(index),
                         toOwned(iterandExpr),
                         blockStyle,
                         std::move(stmts),
                         /*isExpressionLevel*/ false,
                         /*isParam*/ false);

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildCoforallLoopStmt(YYLTYPE locCoforall,
                                                     YYLTYPE locIndex,
                                                     YYLTYPE locBodyAnchor,
                                                     Expression* indexExpr,
                                                     Expression* iterandExpr,
                                                     WithClause* withClause,
                                                     BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locCoforall,
                    locBodyAnchor,
                    blockOrDo);

  auto stmts = consumeAndFlattenTopLevelBlocks(exprLst);

  auto node = Coforall::build(builder, convertLocation(locCoforall),
                              std::move(index),
                              toOwned(iterandExpr),
                              toOwned(withClause),
                              blockStyle,
                              std::move(stmts));

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt
ParserContext::buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                    YYLTYPE locThenBodyAnchor,
                                    Expression* condition,
                                    CommentsAndStmt thenCs) {

  std::vector<ParserComment>* comments;
  ParserExprList* thenExprLst;
  BlockStyle thenBlockStyle;

  prepareStmtPieces(comments, thenExprLst, thenBlockStyle, locIf,
                    usesThenKeyword,
                    locThenBodyAnchor,
                    thenCs);

  auto thenStmts = consumeAndFlattenTopLevelBlocks(thenExprLst);

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenStmts));

  // Do NOT clear comments here! Due to lookahead we might clear a valid
  // comment that has already been stored.
  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt
ParserContext::buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                    YYLTYPE locThenBodyAnchor,
                                    YYLTYPE locElse,
                                    Expression* condition,
                                    CommentsAndStmt thenCs,
                                    CommentsAndStmt elseCs) {

  std::vector<ParserComment>* comments;
  ParserExprList* thenExprLst;
  BlockStyle thenBlockStyle;

  prepareStmtPieces(comments, thenExprLst, thenBlockStyle,
                    locIf,
                    usesThenKeyword,
                    locThenBodyAnchor,
                    thenCs);

  const bool isElseBodyBlock = elseCs.stmt->isBlock();

  auto elseBlockStyle = isElseBodyBlock ? BlockStyle::EXPLICIT
                                        : BlockStyle::IMPLICIT;

  auto thenStmts = consumeAndFlattenTopLevelBlocks(thenExprLst);

  // If the else body is a block, discard all comments preceding it.
  if (isElseBodyBlock) {
    clearComments(elseCs.comments);
    elseCs.comments = nullptr;
  }

  auto elseExprLst = makeList(elseCs);

  // If else body is not a block, discard comments preceding the 'else'.
  if (!isElseBodyBlock) discardCommentsFromList(elseExprLst, locElse);

  auto elseStmts = consumeAndFlattenTopLevelBlocks(elseExprLst);

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenStmts),
                                 elseBlockStyle,
                                 std::move(elseStmts),
                                 /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}

uint64_t ParserContext::binStr2uint64(YYLTYPE location,
                                      const char* str,
                                      bool& erroneous) {
  assert(str);
  assert(str[0] == '0' && (str[1] == 'b' || str[1] == 'B'));

  int len = strlen(str);
  assert(len >= 3);

  erroneous = false;

  // Remove leading 0s
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }
  // Check length
  if (len-startPos > 64) {
    erroneous = true;
    std::string msg = "Integer literal overflow: '";
    msg += str;
    msg += "' is too big for type uint64";
    noteError(location, msg);
  }
  uint64_t val = 0;
  for (int i=startPos; i<len; i++) {
    val <<= 1;
    switch (str[i]) {
    case '0':
      break;
    case '1':
      val += 1;
      break;
    default:
      erroneous = true;
      noteError(location, std::string("illegal character '") +
                          str[i] + "' in binary literal");
    }
  }

  if (erroneous)
    return 0;

  return val;
}

uint64_t ParserContext::octStr2uint64(YYLTYPE location,
                                      const char* str,
                                      bool& erroneous) {
  assert(str);
  assert(str[0] == '0' && (str[1] == 'o' || str[1] == 'O'));

  int len = strlen(str);
  assert(len >= 3);

  /* Remove leading 0s */
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }

  if (len-startPos > 22 || (len-startPos == 22 && str[startPos] != '1')) {
    erroneous = true;
    std::string msg = "Integer literal overflow: '";
    msg += str;
    msg += "' is too big for type uint64";
    noteError(location, msg);
  }

  for (int i = startPos; i < len; i++) {
    if ('0' <= str[i] && str[i] <= '8') {
      // OK
    } else {
      erroneous = true;
      noteError(location, std::string("illegal character '") +
                          str[i] + "' in octal literal");
    }
  }

  if (erroneous == true)
    return 0;

  uint64_t val;
  int numitems = sscanf(str+startPos, "%" SCNo64, &val);
  if (numitems != 1) {
    erroneous = true;
    noteError(location, "error converting octal literal");
  }

  return val;
}

uint64_t ParserContext::decStr2uint64(YYLTYPE location,
                                      const char* str,
                                      bool& erroneous) {
  assert(str);

  int len = strlen(str);
  assert(len >= 1);

  /* Remove leading 0s */
  int startPos = 0;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }

  for (int i = startPos; i < len; i++) {
    if ('0' <= str[i] && str[i] <= '9') {
      // OK
    } else {
      erroneous = true;
      noteError(location, std::string("illegal character '") +
                          str[i] + "' in decimal literal");
    }
  }

  int64_t val;
  int numitems = sscanf(str+startPos, "%" SCNu64, &val);
  if (numitems != 1) {                                          \
    erroneous = true;
    noteError(location, "error converting decimal literal");
  }                                                   

  char* checkStr = (char*)malloc(len+1);
  snprintf(checkStr, len+1, "%" SCNu64, val);
  if (strcmp(str+startPos, checkStr) != 0) {
    erroneous = true;
    std::string msg = "Integer literal overflow: '";
    msg += str;
    msg += "' is too big for type uint64";
    noteError(location, msg);
  }
  free(checkStr);

  if (erroneous)
    return 0;

  return val;
}



uint64_t ParserContext::hexStr2uint64(YYLTYPE location,
                                      const char* str,
                                      bool& erroneous) {
  assert(str);
  assert(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'));

  int len = strlen(str);
  assert(len >= 3);

  /* Remove leading 0s */
  int startPos = 2;
  while (str[startPos] == '0' && startPos < len-1) {
    startPos++;
  }

  if (len-startPos > 16) {
    erroneous = true;
    std::string msg = "Integer literal overflow: '";
    msg += str;
    msg += "' is too big for type uint64";
    noteError(location, msg);
  }

  for (int i = startPos; i < len; i++) {
    if (('0' <= str[i] && str[i] <= '9') ||
        ('a' <= str[i] && str[i] <= 'f') ||
        ('A' <= str[i] && str[i] <= 'F')) {
      // OK
    } else {
      erroneous = true;
      noteError(location, std::string("illegal character '") +
                          str[i] + "' in hexadecimal literal");
    }
  }

  if (erroneous)
    return 0;

  uint64_t val;
  int numitems = sscanf(str+2, "%" SCNx64, &val);
  if (numitems != 1) {
    erroneous = true;
    noteError(location, "error converting hexadecimal literal");
  }
  return val;
}

double ParserContext::str2double(YYLTYPE location,
                                 const char* str,
                                 bool& erroneous) {
  char* endptr = nullptr;
  double num = strtod(str, &endptr);
  if (std::isnan(num) || std::isinf(num)) { 
    // don't worry about checking magnitude of these
  } else {
    double mag = fabs(num);
    // check strtod result
    if ((mag == HUGE_VAL || mag == DBL_MIN) && errno == ERANGE) {
      erroneous = true;
      noteError(location, "overflow or underflow in floating point literal");
    } else if (num == 0.0 && endptr == str) {
      erroneous = true;
      noteError(location, "error in floating point literal");
    }
  }

  return num;
}

Expression* ParserContext::buildNumericLiteral(YYLTYPE location,
                                               PODUniqueString str,
                                               int type) {
  const char* pch = str.c_str();
  uint64_t ull;
  int len = strlen(pch);
  char* noUnderscores = (char*)malloc(len+1);

  // remove all underscores from the number
  int noUnderscoresLen = 0;
  for (int i=0; i<len; i++) {
    if (pch[i] != '_') {
      noUnderscores[noUnderscoresLen++] = pch[i];
    }
  }
  noUnderscores[noUnderscoresLen] = '\0';

  bool erroneous = false;
  int base = 0;
  Expression* ret = nullptr;
  auto loc = convertLocation(location);

  if (type == INTLITERAL) {
    if (!strncmp("0b", pch, 2) || !strncmp("0B", pch, 2)) {
      ull = binStr2uint64(location, noUnderscores, erroneous);
      base = 2;
    } else if (!strncmp("0o", pch, 2) || !strncmp("0O", pch, 2)) {
      // The second case is difficult to read, but is zero followed by a capital
      // letter 'o'
      ull = octStr2uint64(location, noUnderscores, erroneous);
      base = 8;
    } else if (!strncmp("0x", pch, 2) || !strncmp("0X", pch, 2)) {
      ull = hexStr2uint64(location, noUnderscores, erroneous);
      base = 16;
    } else {
      ull = decStr2uint64(location, noUnderscores, erroneous);
      base = 10;
    }

    if (erroneous)
      ret = ErroneousExpression::build(builder, loc).release();
    else if (ull <= 9223372036854775807ull)
      ret = IntLiteral::build(builder, loc, ull, base).release();
    else
      ret =  UintLiteral::build(builder, loc, ull, base).release();
  } else if (type == REALLITERAL || type == IMAGLITERAL) {

    if (type == IMAGLITERAL) {
      // Remove the trailing `i` from the noUnderscores number
      assert(noUnderscores[noUnderscoresLen-1] == 'i');
      noUnderscores[noUnderscoresLen-1] = '\0';
    }

    if (!strncmp("0x", pch, 2) || !strncmp("0X", pch, 2)) {
      base = 16;
    } else {
      base = 10;
    }

    double num = str2double(location, noUnderscores, erroneous);

    if (erroneous)
      ret = ErroneousExpression::build(builder, loc).release();
    else if (type == IMAGLITERAL)
      ret = ImagLiteral::build(builder, loc, num, base).release();
    else
      ret = RealLiteral::build(builder, loc, num, base).release();

  } else {
    assert(false && "Case note handled in buildNumericLiteral");
  }

  free(noUnderscores);
  return ret;
}
