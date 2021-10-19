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

#include "chpl/types/Param.h"

#include <cerrno>
#include <cfloat>
#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

using chpl::types::Param;

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

Decl::Linkage ParserContext::noteLinkage(Decl::Linkage linkage) {
  this->linkage = linkage;
  return this->linkage;
}

Variable::Kind ParserContext::noteVarDeclKind(Variable::Kind varDeclKind) {
  this->varDeclKind = varDeclKind;
  return this->varDeclKind;
}

void ParserContext::storeVarDeclLinkageName(Expression* linkageName) {
  assert(this->varDeclLinkageName == nullptr);
  this->varDeclLinkageName = linkageName;
}

owned<Expression> ParserContext::consumeVarDeclLinkageName(void) {
  auto ret = this->varDeclLinkageName;
  this->varDeclLinkageName = nullptr;
  return toOwned(ret);
}

bool ParserContext::noteIsVarDeclConfig(bool isConfig) {
  this->isVarDeclConfig = isConfig;
  return this->isVarDeclConfig;
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
  this->linkage = Decl::DEFAULT_LINKAGE;
  this->isVarDeclConfig = false;
  YYLTYPE emptyLoc = {0};
  this->declStartLocation = emptyLoc;
}

void ParserContext::enterScope(asttags::ASTTag tag, UniqueString name) {
  ParserScope entry = {tag, name};
  scopeStack.push_back(entry);
}
ParserContext::ParserScope ParserContext::currentScope() {
  if (scopeStack.size() == 0) {
    ParserScope entry = {asttags::Module, UniqueString()};
    return entry;
  }
  return scopeStack.back();
}
bool ParserContext::currentScopeIsAggregate() {
  auto scope = currentScope();
  return (scope.tag == asttags::Class ||
          scope.tag == asttags::Record ||
          scope.tag == asttags::Union);
}
void ParserContext::exitScope(asttags::ASTTag tag, UniqueString name) {
  assert(scopeStack.size() > 0);
  assert(scopeStack.back().tag == tag);
  assert(scopeStack.back().name == name);
  scopeStack.pop_back();
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
      if (e != nullptr) {
        ret.push_back(toOwned(e));
      }
    }
    delete lst;
  }
  return ret;
}

ASTList ParserContext::consume(Expression* e) {
  ASTList ret;
  ret.push_back(toOwned(e));
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

void ParserContext::discardCommentsFromList(ParserExprList* lst) {
  if (lst == nullptr) return;

  ParserExprList tmp;

  for (auto ast : *lst) {
    if (ast->isComment()) {
      delete ast;
    } else {
      tmp.push_back(ast);
    }
  }

  lst->swap(tmp);
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

CommentsAndStmt ParserContext::finishStmt(YYLTYPE location,
                                          CommentsAndStmt cs) {
  auto last = makeLocationAtLast(location);
  auto commentsToDiscard = gatherComments(last);
  clearComments(commentsToDiscard);
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

Expression* ParserContext::buildPrimCall(YYLTYPE location,
                                         MaybeNamedActualList* lst) {
  Location loc = convertLocation(location);
  ASTList actuals;
  std::vector<UniqueString> actualNames;
  UniqueString primName;

  consumeNamedActuals(lst, actuals, actualNames);

  bool anyNames = false;
  for (auto name : actualNames) {
    if (!name.isEmpty()) {
      anyNames = true;
    }
  }
  // first argument must be a string literal
  if (actuals.size() > 0) {
    if (auto lit = actuals[0]->toStringLiteral()) {
      primName = lit->str();
      // and erase that element
      actuals.erase(actuals.begin());
    }
  }

  if (anyNames || primName.isEmpty()) {
    if (anyNames)
      noteError(location, "primitive calls cannot use named arguments");
    else
      noteError(location, "primitive calls must start with string literal");

    return ErroneousExpression::build(builder, loc).release();
  }

  auto prim = PrimCall::build(builder, loc, primName, std::move(actuals));
  return prim.release();
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
                      Decl::DEFAULT_LINKAGE,
                      nullptr,
                      isInline,
                      isOverride,
                      Function::PROC,
                      Formal::DEFAULT_INTENT,
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
    // detect parenless functions
    bool parenless = false;
    if (fp.formals == parenlessMarker) {
      parenless = true;
      fp.formals = nullptr; // don't try to free the marker
    }
    // Detect primary methods and create a receiver for them
    bool primaryMethod = false;
    auto scope = currentScope();
    if (currentScopeIsAggregate()) {
      if (fp.receiver == nullptr) {
        auto loc = convertLocation(location);
        auto ths = UniqueString::build(context(), "this");
        UniqueString cls = scope.name;
        fp.receiver = Formal::build(builder, loc,
                                    ths,
                                    fp.thisIntent,
                                    Identifier::build(builder, loc, cls),
                                    nullptr).release();
        primaryMethod = true;
      }
    }

    owned<Block> body;
    if (fp.body != nullptr) {
      body = consumeToBlock(location, fp.body);
    }

    auto f = Function::build(builder, this->convertLocation(location),
                             fp.name, this->visibility,
                             fp.linkage, toOwned(fp.linkageNameExpr),
                             fp.isInline,
                             fp.isOverride,
                             fp.kind,
                             toOwned(fp.receiver),
                             fp.returnIntent,
                             fp.throws,
                             primaryMethod,
                             parenless,
                             this->consumeList(fp.formals),
                             toOwned(fp.returnType),
                             toOwned(fp.where),
                             this->consumeList(fp.lifetime),
                             std::move(body));
    cs.stmt = f.release();
  } else {
    cs.stmt = fp.errorExpr;
  }
  this->clearComments();
  return cs;
}

// This is the weird one. I can't even parse what is happening here...
/*
1920 | TLSBR expr_ls TIN expr TRSBR type_level_expr
1921     {
1922       if ($2->argList.length != 1)
1923         USR_FATAL($4, "invalid index expression");
1924       $$ = new CallExpr("chpl__buildArrayRuntimeType",
1925              new CallExpr("chpl__ensureDomainExpr", $4), $6, $2->get(1)->remove(),
1926              new CallExpr("chpl__ensureDomainExpr", $4->copy()));
1927     }
*/
Expression*
ParserContext::buildArrayTypeWithIndex(YYLTYPE location,
                                       YYLTYPE locIndexExprs,
                                       ParserExprList* indexExprs,
                                       Expression* domainExpr,
                                       Expression* typeExpr) {
  assert(false && "Not handled yet!");
  return nullptr;
}

Expression*
ParserContext::buildArrayType(YYLTYPE location, YYLTYPE locDomainExprs,
                              ParserExprList* domainExprs,
                              Expression* typeExpr) {

  // In some cases the 'domainExprs' may not exist (think array formal).
  auto domainBody = domainExprs ? consumeList(domainExprs) : ASTList();

  auto domain = Domain::build(builder, convertLocation(locDomainExprs),
                              std::move(domainBody));

  // If 'typeExpr' is null, then the resulting block is empty.
  auto block = consumeToBlock(location, typeExpr);

  auto node = BracketLoop::build(builder, convertLocation(location),
                                 /*index*/ nullptr,
                                 std::move(domain),
                                 /*withClause*/ nullptr,
                                 BlockStyle::IMPLICIT,
                                 std::move(block),
                                 /*isExpressionLevel*/ true);

  return node.release();
}

owned<Decl> ParserContext::buildLoopIndexDecl(YYLTYPE location,
                                              const Expression* e) {
  auto convLoc = convertLocation(location);

  if (const Identifier* ident = e->toIdentifier()) {
    return Variable::build(builder, convLoc, ident->name(),
                           Decl::DEFAULT_VISIBILITY,
                           Decl::DEFAULT_LINKAGE,
                           /*linkageName*/ nullptr,
                           Variable::INDEX,
                           /*isConfig*/ false,
                           /*isField*/ false,
                           /*typeExpression*/ nullptr,
                           /*initExpression*/ nullptr);

  } else if (const uast::Tuple* tup = e->toTuple()) {
    ASTList elements;
    for (auto expr : tup->actuals()) {
      auto decl = buildLoopIndexDecl(location, expr);
      if (decl.get() != nullptr) {
        assert(decl->isDecl());
        elements.push_back(std::move(decl));
      } else {
        auto err = ErroneousExpression::build(builder, convLoc);
        elements.push_back(std::move(err));
      }
    }

    return TupleDecl::build(builder, convLoc, Decl::DEFAULT_VISIBILITY,
                            Decl::DEFAULT_LINKAGE,
                            Variable::INDEX,
                            std::move(elements),
                            /*typeExpression*/ nullptr,
                            /*initExpression*/ nullptr);
  } else {
    const char* msg = "Cannot handle this kind of index var";
    noteError(location, msg);
    return nullptr;
  }
}

// TODO: Need way to clear location of 'e' in the builder.
owned<Decl> ParserContext::buildLoopIndexDecl(YYLTYPE location,
                                              owned<Expression> e) {
  return buildLoopIndexDecl(location, e.get());
}

// TODO: Use me in 'buildBracketLoop' as well.
owned<Decl> ParserContext::buildLoopIndexDecl(YYLTYPE location,
                                              ParserExprList* indexExprs) {
  if (indexExprs->size() > 1) {
    const char* msg = "Invalid index expression";
    noteError(location, msg);
    return nullptr;
  } else {
    auto uncastedIndexExpr = consumeList(indexExprs)[0].release();
    auto indexExpr = uncastedIndexExpr->toExpression();
    assert(indexExpr);
    return buildLoopIndexDecl(location, toOwned(indexExpr));
  }
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

owned<Block>
ParserContext::consumeToBlock(YYLTYPE blockLoc, ParserExprList* lst) {

  // if it consists of only a block, return that block
  if (lst != nullptr && lst->size() == 1) {
    if (Block* b = (*lst)[0]->toBlock()) {
      delete lst;
      return toOwned(b);
    }
  }

  auto stmts = (!lst || !lst->size()) ? ASTList() : consumeList(lst);

  // if it consists of other non-block statements, create a new block
  return Block::build(builder, convertLocation(blockLoc),
                      std::move(stmts));
}

owned<Block>
ParserContext::consumeToBlock(YYLTYPE blockLoc, Expression* e) {
  auto list = e ? makeList(e) : nullptr;
  return consumeToBlock(blockLoc, list);
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

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = BracketLoop::build(builder, convertLocation(locLeftBracket),
                                 std::move(index),
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(body),
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

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = BracketLoop::build(builder, convertLocation(locLeftBracket),
                                 /*index*/ nullptr,
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(body),
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

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = Forall::build(builder, convertLocation(locForall),
                            std::move(index),
                            toOwned(iterandExpr),
                            toOwned(withClause),
                            blockStyle,
                            std::move(body),
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

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = Foreach::build(builder, convertLocation(locForeach),
                             std::move(index),
                             toOwned(iterandExpr),
                             toOwned(withClause),
                             blockStyle,
                             std::move(body));

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

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = For::build(builder, convertLocation(locFor),
                         std::move(index),
                         toOwned(iterandExpr),
                         blockStyle,
                         std::move(body),
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

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = Coforall::build(builder, convertLocation(locCoforall),
                              std::move(index),
                              toOwned(iterandExpr),
                              toOwned(withClause),
                              blockStyle,
                              std::move(body));

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

  auto thenBlock = consumeToBlock(locThenBodyAnchor, thenExprLst);

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenBlock));

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

  auto thenBlock = consumeToBlock(locThenBodyAnchor, thenExprLst);

  // If the else body is a block, discard all comments preceding it.
  if (isElseBodyBlock) {
    clearComments(elseCs.comments);
    elseCs.comments = nullptr;
  }

  auto elseExprLst = makeList(elseCs);

  // If else body is not a block, discard comments preceding the 'else'.
  if (!isElseBodyBlock) discardCommentsFromList(elseExprLst, locElse);

  auto elseBlock = consumeToBlock(locElse, elseExprLst);

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenBlock),
                                 elseBlockStyle,
                                 std::move(elseBlock),
                                 /*isExpressionLevel*/ false);

  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt ParserContext::buildExternBlockStmt(YYLTYPE locEverything,
                                                    SizedStr sizedStr) {
  auto comments = gatherComments(locEverything);
  auto code = std::string(sizedStr.allocatedData, sizedStr.size);
  auto node = ExternBlock::build(builder, convertLocation(locEverything),
                                 std::move(code));

  // This was allocated in 'eatExternCode', see 'lexer-help.h'.
  free((void*) sizedStr.allocatedData);

  return { .comments=comments, .stmt=node.release() };
}

Expression* ParserContext::buildNumericLiteral(YYLTYPE location,
                                               PODUniqueString str,
                                               int type) {
  UniqueString text = str;
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

  std::string err;
  Expression* ret = nullptr;
  auto loc = convertLocation(location);

  if (type == INTLITERAL) {
    if (!strncmp("0b", pch, 2) || !strncmp("0B", pch, 2)) {
      ull = Param::binStr2uint64(noUnderscores, noUnderscoresLen,  err);
    } else if (!strncmp("0o", pch, 2) || !strncmp("0O", pch, 2)) {
      // The second case is difficult to read, but is zero followed by a capital
      // letter 'o'
      ull = Param::octStr2uint64(noUnderscores, noUnderscoresLen, err);
    } else if (!strncmp("0x", pch, 2) || !strncmp("0X", pch, 2)) {
      ull = Param::hexStr2uint64(noUnderscores, noUnderscoresLen, err);
    } else {
      ull = Param::decStr2uint64(noUnderscores, noUnderscoresLen, err);
    }

    if (!err.empty()) {
      noteError(location, err);
      ret = ErroneousExpression::build(builder, loc).release();
    } else if (ull <= 9223372036854775807ull) {
      ret = IntLiteral::build(builder, loc, ull, text).release();
    } else {
      ret =  UintLiteral::build(builder, loc, ull, text).release();
    }
  } else if (type == REALLITERAL || type == IMAGLITERAL) {

    if (type == IMAGLITERAL) {
      // Remove the trailing `i` from the noUnderscores number
      if (noUnderscores[noUnderscoresLen-1] != 'i') {
        err = "invalig imag literal - does not end in i";
      }
      noUnderscoresLen--;
      noUnderscores[noUnderscoresLen] = '\0';
    }

    double num = Param::str2double(noUnderscores, noUnderscoresLen, err);

    if (!err.empty()) {
      noteError(location, err);
      ret = ErroneousExpression::build(builder, loc).release();
    } else if (type == IMAGLITERAL) {
      ret = ImagLiteral::build(builder, loc, num, text).release();
    } else {
      ret = RealLiteral::build(builder, loc, num, text).release();
    }

  } else {
    assert(false && "Case note handled in buildNumericLiteral");
  }

  free(noUnderscores);
  return ret;
}

Expression* ParserContext::
buildVisibilityClause(YYLTYPE location, owned<Expression> symbol,
                      VisibilityClause::LimitationKind limitationKind,
                      ASTList limitations) {
  if (!symbol->isAs() && !symbol->isIdentifier() && !symbol->isDot()) {
    auto msg = "Expected symbol in visibility clause";
    return raiseError(location, msg);
  }

  for (auto& expr : limitations) {
    if (expr->isAs() || expr->isIdentifier() || expr->isDot() ||
        expr->isComment()) {
      continue;
    } else {
      auto msg = "Expected symbol in limitation list";
      return raiseError(location, msg);
    }
  }

  auto node = VisibilityClause::build(builder, convertLocation(location),
                                      std::move(symbol),
                                      limitationKind,
                                      std::move(limitations));

  return node.release();
}

Expression* ParserContext::
buildVisibilityClause(YYLTYPE location, owned<Expression> symbol) {
  return buildVisibilityClause(location, std::move(symbol),
                               VisibilityClause::NONE,
                               ASTList());
}

Expression* ParserContext::buildAsExpr(YYLTYPE locName, YYLTYPE locRename,
                                       owned<Expression> name,
                                       owned<Expression> rename) {
  if (!rename->isIdentifier()) {
    const char* msg = "Rename in as expression must be identifier";
    return raiseError(locRename, msg);
  }

  if (!name->isDot() && !name->isIdentifier()) {
    const char* msg = "Symbol in as expression must be dot or identifer";
    return raiseError(locName, msg);
  }

  auto renameAsIdent = toOwned(rename.release()->toIdentifier());

  YYLTYPE locEverything = makeSpannedLocation(locName, locRename);

  auto node = As::build(builder, convertLocation(locEverything),
                        std::move(name),
                        std::move(renameAsIdent));

  return node.release();
}

CommentsAndStmt ParserContext::
buildImportStmt(YYLTYPE locEverything, Decl::Visibility visibility,
                ParserExprList* visibilityClauses) {
  auto comments = gatherComments(locEverything);
  auto convLoc = convertLocation(locEverything);

  auto vcs = consumeList(visibilityClauses);

  // If any of the clauses are EEs, then discard the entire import.
  for (auto& vc : vcs) {
    if (vc->isErroneousExpression()) {
      auto node = ErroneousExpression::build(builder, convLoc);
      return { .comments=comments, .stmt=node.release() };
    }
  }

  auto node = Import::build(builder, convLoc, visibility, std::move(vcs));

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
  return finishStmt(cs);
}

CommentsAndStmt ParserContext::
buildMultiUseStmt(YYLTYPE locEverything, Decl::Visibility visibility,
                  ParserExprList* visibilityClauses) {
  auto comments = gatherComments(locEverything);
  auto convLoc = convertLocation(locEverything);

  auto vcs = consumeList(visibilityClauses);

  // If any of the clauses are EEs, then discard the entire use.
  for (auto& vc : vcs) {
    if (vc->isErroneousExpression()) {
      auto node = ErroneousExpression::build(builder, convLoc);
      return { .comments=comments, .stmt=node.release() };
    }
  }

  // TODO: Make sure that all the vis clauses are correct for multi-use?
  auto node = Use::build(builder, convLoc, visibility, std::move(vcs));

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
  return finishStmt(cs);
}

CommentsAndStmt ParserContext::
buildSingleUseStmt(YYLTYPE locEverything, YYLTYPE locVisibilityClause,
                   Decl::Visibility visibility,
                   owned<Expression> name,
                   VisibilityClause::LimitationKind limitationKind,
                   ParserExprList* limitationExprs) {
  auto comments = gatherComments(locEverything);

  auto visClause = buildVisibilityClause(locVisibilityClause,
                                         std::move(name),
                                         limitationKind,
                                         consumeList(limitationExprs));

  if (visClause->isErroneousExpression()) {
    auto convLoc = convertLocation(locVisibilityClause);
    auto node = ErroneousExpression::build(builder, convLoc);
    return { .comments=comments, .stmt=node.release() };
  }

  ASTList uses;
  uses.push_back(toOwned(visClause));

  auto node = Use::build(builder, convertLocation(locEverything),
                         visibility,
                         std::move(uses));

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return finishStmt(cs);
}

// Given a list of vars, build either a single var or a multi-decl.
CommentsAndStmt ParserContext::buildVarOrMultiDecl(YYLTYPE locEverything,
                                                   ParserExprList* vars) {
  int numDecls = 0;
  Decl* lastDecl = nullptr;

  for (auto elt : *vars) {
    if (Decl* d = elt->toDecl()) {
      lastDecl = d;
      numDecls++;
    }
  }

  assert(numDecls > 0);
  assert(lastDecl);

  auto comments = gatherCommentsFromList(vars, locEverything);
  CommentsAndStmt cs = { .comments=comments, .stmt=nullptr };

  if (numDecls == 1) {
    cs.stmt = lastDecl;

    // Delete any remaining comments and then the expr list.
    for (auto elt : *vars) {
      if (elt->isComment()) delete elt;
    }
    delete vars;

  } else {
    auto multi = MultiDecl::build(builder, convertLocation(locEverything),
                                  visibility,
                                  linkage,
                                  consumeList(vars));
    cs.stmt = multi.release();
  }

  assert(cs.stmt);

  resetDeclState();

  return cs;
}

TypeDeclParts
ParserContext::enterScopeAndBuildTypeDeclParts(YYLTYPE locStart,
                                               PODUniqueString name,
                                               asttags::ASTTag tag) {
  auto loc = declStartLoc(locStart);

  enterScope(tag, name);

  TypeDeclParts ret = {
    .comments=this->gatherComments(loc),
    .visibility=this->visibility,
    .linkage=this->linkage,
    /* The linkage name must be set by the rule that uses these parts. */
    .linkageName=nullptr,
    .name=name,
    .tag=tag
  };

  clearComments();

  // Important stuff has been recorded, so reset.
  resetDeclState();

  return ret;
}

static void clearTypeDeclPartsLinkage(TypeDeclParts& parts) {
  parts.linkage = Decl::DEFAULT_LINKAGE;
  if (parts.linkageName) {
    delete parts.linkageName;
    parts.linkageName = nullptr;
  }
}

void ParserContext::validateExternTypeDeclParts(YYLTYPE location,
                                                TypeDeclParts& parts) {
  if (parts.tag == asttags::Class) {
    assert(parts.linkage != Decl::DEFAULT_LINKAGE);
    auto msg = "Cannot declare class types as export or extern";
    noteError(location, msg);

    // Clear the linkage state for this so that parsing can continue.
    clearTypeDeclPartsLinkage(parts);
  }

  if (parts.tag == asttags::Union && parts.linkage == Decl::EXPORT) {
    auto msg = "Cannot export union types";
    noteError(location, msg);

    // Clear the linkage state for this so that parsing can continue.
    clearTypeDeclPartsLinkage(parts);
  }
}

CommentsAndStmt
ParserContext::buildAggregateTypeDecl(YYLTYPE location,
                                      TypeDeclParts parts,
                                      YYLTYPE inheritLoc,
                                      ParserExprList* optInherit,
                                      YYLTYPE openingBrace,
                                      ParserExprList* contents,
                                      YYLTYPE closingBrace) {

  if (parts.linkage != Decl::DEFAULT_LINKAGE) {
    validateExternTypeDeclParts(location, parts);
  } else {
    assert(parts.linkageName == nullptr);
  }

  CommentsAndStmt cs = {parts.comments, nullptr};
  // adjust the contents list to have the right comments
  discardCommentsFromList(contents, openingBrace);
  appendList(contents, gatherComments(closingBrace));

  auto contentsList = consumeList(contents);

  owned<Identifier> inheritIdentifier;
  if (optInherit != nullptr) {
    if (optInherit->size() > 0) {
      if (parts.tag == asttags::Record) {
        noteError(inheritLoc, "records cannot inherit");
      } else if (parts.tag == asttags::Union) {
        noteError(inheritLoc, "unions cannot inherit");
      } else {
        if (optInherit->size() > 1)
          noteError(inheritLoc, "only single inheritance is supported");
        ASTNode* ast = (*optInherit)[0];
        if (ast->isIdentifier()) {
          inheritIdentifier = toOwned(ast->toIdentifier());
          (*optInherit)[0] = nullptr;
        } else {
          noteError(inheritLoc, "non-Identifier expression cannot be inherited");
        }
      }
    }
    consumeList(optInherit); // just to delete it
  }

  Expression* decl = nullptr;
  if (parts.tag == asttags::Class) {

    // These should have been cleared when validated above (the Class node
    // constructor and builder function would have to change if we ever
    // wanted to permit export/extern classes.
    assert(parts.linkage == Decl::DEFAULT_LINKAGE);
    assert(!parts.linkageName);

    decl = Class::build(builder, convertLocation(location),
                        parts.visibility,
                        parts.name,
                        std::move(inheritIdentifier),
                        std::move(contentsList)).release();
  } else if (parts.tag == asttags::Record) {
    decl = Record::build(builder, convertLocation(location),
                         parts.visibility,
                         parts.linkage,
                         toOwned(parts.linkageName),
                         parts.name,
                         std::move(contentsList)).release();
  } else if (parts.tag == asttags::Union) {

    // Should have been cleared because this is not possible right now.
    assert(parts.linkage != Decl::EXPORT);

    decl = Union::build(builder, convertLocation(location),
                        parts.visibility,
                        parts.linkage,
                        toOwned(parts.linkageName),
                        parts.name,
                        std::move(contentsList)).release();
  } else {
    assert(false && "case not handled");
  }

  cs.stmt = decl;
  return cs;
}

Expression* ParserContext::buildCustomReduce(YYLTYPE location,
                                             YYLTYPE locIdent,
                                             Expression* lhs,
                                             Expression* rhs) {
  if (!lhs->isIdentifier()) {
    const char* msg = "Expected identifier for reduction name";
    return raiseError(locIdent, msg);
  } else {
    auto identName = lhs->toIdentifier()->name();
    auto node = Reduce::build(builder, convertLocation(location),
                              identName,
                              toOwned(rhs));
    return node.release();
  }
}

Expression* ParserContext::buildTypeConstructor(YYLTYPE location,
                                                PODUniqueString baseType,
                                                MaybeNamedActual actual) {
  auto maybeNamedActuals = new MaybeNamedActualList();
  maybeNamedActuals->push_back(actual);
  return buildTypeConstructor(location, baseType, maybeNamedActuals);
}

Expression* ParserContext::buildTypeConstructor(YYLTYPE location,
                                                PODUniqueString baseType,
                                                Expression* subType) {
  auto maybeNamedActuals = new MaybeNamedActualList();
  MaybeNamedActual actual = { .expr=subType, .name=baseType };
  maybeNamedActuals->push_back(actual);
  return buildTypeConstructor(location, baseType, maybeNamedActuals);
}

Expression* ParserContext::
buildTypeConstructor(YYLTYPE location, PODUniqueString baseType,
                     MaybeNamedActualList* maybeNamedActuals) {
  auto ident = buildIdent(location, baseType);
  std::vector<UniqueString> actualNames;
  ASTList actuals;

  consumeNamedActuals(maybeNamedActuals, actuals, actualNames);

  // TODO: Record in node that this call did not use parens.
  const bool callUsedSquareBrackets = false;

  auto node = FnCall::build(builder, convertLocation(location),
                            toOwned(ident),
                            std::move(actuals),
                            std::move(actualNames),
                            callUsedSquareBrackets);

  return node.release();
}

CommentsAndStmt
ParserContext::buildTryExprStmt(YYLTYPE location, Expression* expr,
                                bool isTryBang) {
  auto comments = gatherComments(location);
  auto node = Try::build(builder, convertLocation(location), toOwned(expr),
                         isTryBang,
                         /*isExpressionLevel*/ false);
  CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
  return ret;
}

CommentsAndStmt
ParserContext::buildTryExprStmt(YYLTYPE location, CommentsAndStmt cs,
                                bool isTryBang) {

  // TODO: The gathered comments from two different locations may not be
  // stored in order - I am assuming that they are for now.
  auto commentList = makeList();
  appendList(commentList, gatherComments(location));
  appendList(commentList, cs.comments);
  cs.comments = nullptr;

  auto comments = gatherCommentsFromList(commentList, location);
  clearComments(commentList);

  auto node = Try::build(builder, convertLocation(location),
                         toOwned(cs.stmt),
                         isTryBang,
                         /*isExpressionLevel*/ false);

  CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
  return ret;
}

Expression*
ParserContext::buildTryExpr(YYLTYPE location, Expression* expr,
                            bool isTryBang) {
  auto node = Try::build(builder, convertLocation(location), toOwned(expr),
                         isTryBang,
                         /*isExpressionLevel*/ true);
  return node.release();
}

CommentsAndStmt
ParserContext::buildTryCatchStmt(YYLTYPE location, CommentsAndStmt block,
                                 ParserExprList* handlers,
                                 bool isTryBang) {

  // TODO: The gathered comments from two different locations may not be
  // stored in order - I am assuming that they are for now.
  auto commentList = makeList();
  appendList(commentList, gatherComments(location));
  appendList(commentList, block.comments);
  block.comments = nullptr;

  auto comments = gatherCommentsFromList(commentList, location);
  clearComments(commentList);

  auto stmts = consumeAndFlattenTopLevelBlocks(makeList(block));
  auto catches = consumeList(handlers);

  auto node = Try::build(builder, convertLocation(location),
                         std::move(stmts),
                         std::move(catches),
                         isTryBang);

  CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
  return ret;
}

Expression* ParserContext::buildCatch(YYLTYPE location, Expression* error,
                                      CommentsAndStmt cs,
                                      bool hasParensAroundError) {
  assert(cs.stmt->isBlock());
  if (error != nullptr) assert(error->isVariable());

  clearComments(cs.comments);
  cs.comments = nullptr;

  auto errorVar = error ? error->toVariable() : nullptr;

  auto node = Catch::build(builder, convertLocation(location),
                           toOwned(errorVar),
                           toOwned(cs.stmt->toBlock()),
                           hasParensAroundError);

  return node.release();
}

CommentsAndStmt
ParserContext::buildWhenStmt(YYLTYPE location, ParserExprList* caseExprs,
                             BlockOrDo blockOrDo) {

  // No need to gather comments, they'll have been collected here...
  auto comments = blockOrDo.cs.comments;
  auto stmt = blockOrDo.cs.stmt;

  BlockStyle blockStyle = blockOrDo.usesDo ? BlockStyle::IMPLICIT
                                           : BlockStyle::EXPLICIT;

  if (blockOrDo.usesDo && stmt->isBlock()) {
    blockStyle = BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK;
  }

  auto caseList = caseExprs ? consumeList(caseExprs) : ASTList();

  auto stmtList = consumeAndFlattenTopLevelBlocks(makeList(stmt));

  auto node = When::build(builder, convertLocation(location),
                          std::move(caseList),
                          blockStyle,
                          std::move(stmtList));

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return cs;
}

CommentsAndStmt
ParserContext::buildSelectStmt(YYLTYPE location, owned<Expression> expr,
                               ParserExprList* whenStmts) {
  auto comments = gatherCommentsFromList(whenStmts, location);

  // Discard all remaining comments.
  discardCommentsFromList(whenStmts);

  auto stmts = consumeList(whenStmts);

  int numOtherwise = 0;

  // Return an error if there is more than one otherwise.
  for (auto& ast : stmts) {
    auto when = ast->toWhen();
    assert(when != nullptr);

    // TODO: Do we also care about the position of the otherwise?
    if (when->isOtherwise() && numOtherwise++) {
      const char* msg = "Select has multiple otherwise clauses";
      auto error = raiseError(location, msg);
      CommentsAndStmt cs = { .comments=comments, .stmt=error };
      return cs;
    }
  }

  auto node = Select::build(builder, convertLocation(location),
                            std::move(expr),
                            std::move(stmts));

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return cs;
}
