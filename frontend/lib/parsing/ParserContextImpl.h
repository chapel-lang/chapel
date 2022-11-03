/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Pragma.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/ErrorBase.h"

#include <cerrno>
#include <cfloat>
#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

using chpl::types::Param;
using chpl::owned;

/**
  Helper macro to report errors from the parser, which takes care of saving the
  error in the ParserContext, retrieving the global Context from the
  ParserContext, and converting YYLTYPE locations.

  Evaluates to an ErroneousExpression error sentinel at the location of the
  error, which may be used or ignored.
 */
#define CHPL_PARSER_REPORT(PARSER_CONTEXT__, NAME__, LOC__, EINFO__...) \
  (({                                                                   \
     const ErrorBase* ERR__ = Error##NAME__::get(                       \
         PARSER_CONTEXT__->context(),                                   \
         std::make_tuple(PARSER_CONTEXT__->convertLocation(LOC__),      \
                         ##EINFO__));                                   \
     PARSER_CONTEXT__->context()->report(ERR__);                        \
     PARSER_CONTEXT__->saveError(ERR__);                                \
   }),                                                                  \
   ErroneousExpression::build(PARSER_CONTEXT__->builder,                \
                              PARSER_CONTEXT__->convertLocation(LOC__)) \
       .release())

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
  assert(!hasNotedVarDeclKind);
  this->hasNotedVarDeclKind = true;
  this->varDeclKind = varDeclKind;
  return this->varDeclKind;
}

void ParserContext::storeVarDeclLinkageName(AstNode* linkageName) {
  assert(this->varDeclLinkageName == nullptr);
  this->varDeclLinkageName = linkageName;
}

owned<AstNode> ParserContext::consumeVarDeclLinkageName(void) {
  auto ret = this->varDeclLinkageName;
  this->varDeclLinkageName = nullptr;
  return toOwned(ret);
}

owned<Attributes> ParserContext::buildAttributes(YYLTYPE locationOfDecl) {
  numAttributesBuilt += 1;

  // There may be nothing to return.
  if (!hasAttributeParts) {
    return nullptr;
  }

  // Create a local copy of the attributes we can move into the node.
  auto pragmaCopy = attributeParts.pragmas
      ? *(attributeParts.pragmas)
      : std::set<PragmaTag>();

  auto node = Attributes::build(builder, convertLocation(locationOfDecl),
                                std::move(pragmaCopy),
                                attributeParts.isDeprecated,
                                attributeParts.isUnstable,
                                attributeParts.deprecationMessage,
                                attributeParts.unstableMessage);
  return node;
}

PODUniqueString ParserContext::notePragma(YYLTYPE loc,
                                          AstNode* pragmaStr) {
  auto ret = PODUniqueString::get();

  // Extract the string literal and convert it into a pragma flag.
  if (auto strLit = pragmaStr->toStringLiteral()) {
    ret = PODUniqueString::get(context(), strLit->str().c_str());
    auto tag = pragmaNameToTag(ret.c_str());

    if (tag == PRAGMA_UNKNOWN) {
      CHPL_PARSER_REPORT(this, UnknownPragma, loc, strLit->str().str());
    }

    // Initialize the pragma flags if needed.
    auto& pragmas = attributeParts.pragmas;
    if (pragmas == nullptr) pragmas = new std::set<PragmaTag>();
    hasAttributeParts = true;

    // Always insert, even if PRAGMA_UNKNOWN.
    pragmas->insert(tag);
  }

  // Make sure to clean up the UAST node since it will be discarded.
  delete pragmaStr;

  return ret;
}

void ParserContext::noteDeprecation(YYLTYPE loc, AstNode* messageStr) {
  if (!hasAttributeParts) {
    hasAttributeParts = true;
  } else {
    assert(!attributeParts.isDeprecated);
    assert(attributeParts.deprecationMessage.isEmpty());
  }

  attributeParts.isDeprecated = true;

  if (messageStr) {
    if (auto strLit = messageStr->toStringLiteral()) {
      attributeParts.deprecationMessage = strLit->str();
    }

    delete messageStr;
  }
}

void ParserContext::noteUnstable(YYLTYPE loc, AstNode* messageStr) {
  if (!hasAttributeParts) {
    hasAttributeParts = true;
  }
  else {
    assert(!attributeParts.isUnstable);
    assert(attributeParts.unstableMessage.isEmpty());
  }

  attributeParts.isUnstable = true;

  if (messageStr) {
    if (auto strLit = messageStr->toStringLiteral()) {
      attributeParts.unstableMessage = strLit->str();
    }

    delete messageStr;
  }
}
void ParserContext::resetAttributePartsState() {
  if (hasAttributeParts) {
    auto& pragmas = attributeParts.pragmas;
    if (pragmas) delete pragmas;
    attributeParts = { nullptr, false, false, UniqueString(), UniqueString() };
    hasAttributeParts = false;
  }

  assert(attributeParts.pragmas == nullptr);
  assert(!attributeParts.isDeprecated);
  assert(!attributeParts.isUnstable);
  assert(attributeParts.deprecationMessage.isEmpty());
  assert(attributeParts.unstableMessage.isEmpty());
  assert(!hasAttributeParts);

  numAttributesBuilt = 0;
}

// TODO: The original builder skips if the statement is a forwarding
// statement, how do we handle that? Do we do anything with the pragmas,
// or do we also just silently ignore it?
CommentsAndStmt
ParserContext::buildPragmaStmt(YYLTYPE loc, CommentsAndStmt cs) {
  CommentsAndStmt ret = cs;

  // If statement is not a decl, we should not have built any, otherwise
  // it was and the counter got reset at some point before this.
  assert(numAttributesBuilt == 0);

  if (cs.stmt && cs.stmt->isDecl()) {

    // If a decl was produced then the attributes should have been reset.
    // If they were _not_ reset, then it means that a deprecated statement
    // came before a pragma list.
    // TODO: Can we just make deprecated_stmt and pragma_ls alternates?
    // This solves that problem.
    if (hasAttributeParts) {
      assert(attributeParts.pragmas == nullptr);
      assert(attributeParts.isDeprecated);
      assert(attributeParts.isUnstable);
      CHPL_PARSER_REPORT(this, PragmasBeforeDeprecation, loc);
    }

  } else {
    assert(numAttributesBuilt == 0);
    if(cs.stmt) assert(hasAttributeParts);

    // TODO: The original builder also states the first pragma.
    CHPL_PARSER_REPORT(this, CannotAttachPragmas, loc);

    // Clean up the attribute parts.
    resetAttributePartsState();
  }

  return ret;
}

bool ParserContext::noteIsBuildingFormal(bool isBuildingFormal) {
  this->isBuildingFormal = isBuildingFormal;
  return this->isBuildingFormal;
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

void ParserContext::resetDeclStateOnError() {
  // Consume the linkage name just to clean it up.
  auto droppedLinkageName = consumeVarDeclLinkageName();
  (void) droppedLinkageName;
  this->resetDeclState();
}

void ParserContext::resetDeclState() {
  this->resetAttributePartsState();
  this->varDeclKind = Variable::VAR;
  this->hasNotedVarDeclKind = false;
  this->visibility = Decl::DEFAULT_VISIBILITY;
  this->linkage = Decl::DEFAULT_LINKAGE;
  this->isVarDeclConfig = false;
  YYLTYPE emptyLoc = {0};
  this->declStartLocation = emptyLoc;
  this->isBuildingFormal = false;
}

void ParserContext::enterScope(asttags::AstTag tag, UniqueString name) {
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
void ParserContext::exitScope(asttags::AstTag tag, UniqueString name) {
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
    for (AstNode* expr : *comments) {
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
ParserExprList* ParserContext::makeList(AstNode* e) {
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
  for (AstNode* elt : *lst) {
    dst->push_back(elt);
  }
  delete lst;
  return dst;
}

ParserExprList* ParserContext::appendList(ParserExprList* dst, AstNode* e) {
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

AstList ParserContext::consumeList(ParserExprList* lst) {
  AstList ret;
  if (lst != nullptr) {
    for (AstNode* e : *lst) {
      if (e != nullptr) {
        ret.push_back(toOwned(e));
      }
    }
    delete lst;
  }
  return ret;
}

AstList ParserContext::consume(AstNode* e) {
  AstList ret;
  ret.push_back(toOwned(e));
  return ret;
}

void ParserContext::consumeNamedActuals(MaybeNamedActualList* lst,
                                        AstList& actualsOut,
                                        std::vector<UniqueString>& namesOut) {
  bool anyActualNames = false;
  if (lst != nullptr) {
    for (auto& elt : *lst) {
      if (!elt.name.isEmpty())
        anyActualNames = true;
    }
    for (auto& elt : *lst) {
      // check if expr is not NULL, which can happen in some bad examples
      // like new A;
      if (elt.expr) {
        actualsOut.push_back(toOwned(elt.expr));
      }
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
    AstNode* e = (*lst)[nToMove];
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

CommentsAndStmt ParserContext::finishStmt(AstNode* e) {
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

AstNode* ParserContext::buildPrimCall(YYLTYPE location,
                                      MaybeNamedActualList* lst) {
  Location loc = convertLocation(location);
  AstList actuals;
  std::vector<UniqueString> actualNames;
  UniqueString primName;

  consumeNamedActuals(lst, actuals, actualNames);

  bool anyNames = false;
  for (auto name : actualNames) {
    if (!name.isEmpty()) {
      anyNames = true;
    }
  }
  // first argument must be a string literal, might be a cstring though
  if (actuals.size() > 0) {
    if (auto lit = actuals[0]->toCStringLiteral()) {
      primName = lit->str();
      // and erase that element
      actuals.erase(actuals.begin());
    } else if (auto lit = actuals[0]->toStringLiteral()) {
      primName = lit->str();
      // and erase that element
      actuals.erase(actuals.begin());
    }
  }

  if (anyNames || primName.isEmpty()) {
    if (anyNames)
      CHPL_PARSER_REPORT(this, PrimCallNamedArgs, location);
    else
      CHPL_PARSER_REPORT(this, PrimCallNoStrLiteral, location);

    return ErroneousExpression::build(builder, loc).release();
  }

  PrimitiveTag tag = primNameToTag(primName.c_str());
  if (tag == PRIM_UNKNOWN) {
    CHPL_PARSER_REPORT(this, UnknownPrimitive, location);
    return ErroneousExpression::build(builder, loc).release();
  }

  auto prim = PrimCall::build(builder, loc, tag, std::move(actuals));
  return prim.release();
}

OpCall* ParserContext::buildBinOp(YYLTYPE location,
                                  AstNode* lhs,
                                  PODUniqueString op,
                                  AstNode* rhs) {
  return OpCall::build(builder, convertLocation(location),
                       op, toOwned(lhs), toOwned(rhs)).release();
}
OpCall* ParserContext::buildUnaryOp(YYLTYPE location,
                                    PODUniqueString op,
                                    AstNode* expr) {
  auto ustrOp = UniqueString(op);
  // may reassign op here to match old parser
  // as in buildPreDecIncWarning
  if (ustrOp == "++") {
    CHPL_PARSER_REPORT(this, PreIncDecOp, location, "++", "increment");
    ustrOp = USTR("+");
    // conver the ++a to +(+a)
    auto innerOp = OpCall::build(builder, convertLocation(location),
                       ustrOp, toOwned(expr)).release();
    return OpCall::build(builder, convertLocation(location),
                       ustrOp, toOwned(innerOp)).release();
  } else if (ustrOp == "--") {
    CHPL_PARSER_REPORT(this, PreIncDecOp, location, "--", "decrement");
    ustrOp = USTR("-");
    // convert the --a to -(-a)
    auto innerOp = OpCall::build(builder, convertLocation(location),
                       ustrOp, toOwned(expr)).release();
    return OpCall::build(builder, convertLocation(location),
                       ustrOp, toOwned(innerOp)).release();
  }

  return OpCall::build(builder, convertLocation(location),
                       ustrOp, toOwned(expr)).release();
}

AstNode* ParserContext::buildManagerExpr(YYLTYPE location,
                                         AstNode* expr,
                                         Variable::Kind kind,
                                         YYLTYPE locResourceName,
                                         UniqueString resourceName) {
  auto var = Variable::build(builder, convertLocation(locResourceName),
                             nullptr,
                             Decl::DEFAULT_VISIBILITY,
                             Decl::DEFAULT_LINKAGE,
                             nullptr,
                             resourceName,
                             kind,
                             false,
                             false,
                             nullptr,
                             nullptr);
  auto as = As::build(builder, convertLocation(location),
                      toOwned(expr),
                      std::move(var));
  return as.release();
}

AstNode* ParserContext::buildManagerExpr(YYLTYPE location,
                                         AstNode* expr,
                                         YYLTYPE locResourceName,
                                         UniqueString resourceName) {
  return buildManagerExpr(location, expr, Variable::INDEX,
                          locResourceName,
                          resourceName);
}

CommentsAndStmt ParserContext::buildManageStmt(YYLTYPE location,
                                               ParserExprList* managerExprs,
                                               YYLTYPE locBlockOrDo,
                                               BlockOrDo blockOrDo) {
  std::vector<ParserComment>* comments;
  ParserExprList* stmtExprList;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, stmtExprList, blockStyle, location,
                    locBlockOrDo,
                    blockOrDo);

  AstList managers = consumeList(managerExprs);
  AstList stmts = consumeList(stmtExprList);

  auto node = Manage::build(builder, convertLocation(location),
                            std::move(managers),
                            blockStyle,
                            std::move(stmts));

  CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };

  return ret;
}

FunctionParts ParserContext::makeFunctionParts(bool isInline,
                                               bool isOverride) {
  FunctionParts fp = {false,
                      nullptr,
                      nullptr,
                      nullptr,
                      this->visibility,
                      Decl::DEFAULT_LINKAGE,
                      nullptr,
                      isInline,
                      isOverride,
                      Function::PROC,
                      Formal::DEFAULT_INTENT,
                      nullptr,
                      nullptr,
                      Function::DEFAULT_RETURN_INTENT,
                      false,
                      nullptr, nullptr, nullptr, nullptr,
                      nullptr};
  return fp;
}

static AstNode*
findErrorInFnSignatureParts(ParserContext* context, YYLTYPE location,
                       FunctionParts& fp) {
  if (fp.errorExpr) return fp.errorExpr;
  if (fp.formals) {
    for (auto ast : *fp.formals) {
      if (!ast->isErroneousExpression()) continue;
      auto loc = context->convertLocation(location);
      auto errorExpr = ErroneousExpression::build(context->builder, loc);
      fp.errorExpr = errorExpr.release();
      return fp.errorExpr;
    }
  }

  return nullptr;
}

AstNode*
ParserContext::buildFunctionExpr(YYLTYPE location, FunctionParts& fp) {
  if (fp.isBodyNonBlockExpression) assert(false && "Not handled!");
  auto ret = buildLambda(location, fp);
  return ret;
}

AstNode*
ParserContext::buildFormal(YYLTYPE location, Formal::Intent intent,
                           PODUniqueString name,
                           AstNode* typeExpr,
                           AstNode* initExpr,
                           bool consumeAttributes) {
  auto attr = consumeAttributes ? buildAttributes(location) : nullptr;
  auto loc = convertLocation(location);
  auto node = Formal::build(builder, loc, std::move(attr), name, intent,
                            toOwned(typeExpr),
                            toOwned(initExpr));
  this->noteIsBuildingFormal(false);
  if (consumeAttributes) resetAttributePartsState();
  return node.release();
}

AstNode*
ParserContext::buildVarArgFormal(YYLTYPE location, Formal::Intent intent,
                                 PODUniqueString name,
                                 AstNode* typeExpr,
                                 AstNode* initExpr,
                                 bool consumeAttributes) {
  auto attr = consumeAttributes ? buildAttributes(location) : nullptr;
  auto loc = convertLocation(location);
  auto node = VarArgFormal::build(builder, loc, std::move(attr), name,
                                  intent,
                                  toOwned(typeExpr),
                                  toOwned(initExpr));
  this->noteIsBuildingFormal(false);
  if (consumeAttributes) resetAttributePartsState();
  return node.release();
}

AstNode*
ParserContext::buildTupleFormal(YYLTYPE location, Formal::Intent intent,
                                ParserExprList* components,
                                AstNode* typeExpr,
                                AstNode* initExpr) {
  auto loc = convertLocation(location);
  auto node = TupleDecl::build(builder, loc, nullptr,
                               this->visibility,
                               this->linkage,
                               ((TupleDecl::IntentOrKind) intent),
                               this->consumeList(components),
                               toOwned(typeExpr),
                               toOwned(initExpr));
  this->noteIsBuildingFormal(false);
  return node.release();
}

AstNode*
ParserContext::buildAnonFormal(YYLTYPE location, YYLTYPE locIntent,
                               Formal::Intent intent,
                               AstNode* formalType) {
  std::ignore = locIntent;
  auto loc = convertLocation(location);
  auto node = AnonFormal::build(builder, loc, intent, toOwned(formalType));
  auto ret = node.release();
  return ret;
}

AstNode*
ParserContext::buildAnonFormal(YYLTYPE location, PODUniqueString name) {
  auto ident = this->buildIdent(location, name);
  auto ret = this->buildAnonFormal(location, ident);
  return ret;
}

AstNode*
ParserContext::buildAnonFormal(YYLTYPE location, AstNode* formalType) {
  auto loc = convertLocation(location);
  auto node = AnonFormal::build(builder, loc, Formal::DEFAULT_INTENT,
                                toOwned(formalType));
  auto ret = node.release();
  return ret;
}

AstNode*
ParserContext::consumeFormalToAnonFormal(AstNode* ast) {
  auto formal = ast->toFormal();

  assert(formal);
  assert(!formal->initExpression() && !formal->typeExpression());
  assert(!formal->name().isEmpty());
  assert(!formal->attributes());
  assert(formal->visibility() == Decl::DEFAULT_VISIBILITY);

  auto loc = builder->getLocation(formal);
  assert(!loc.isEmpty());

  // TODO: Fix the location...
  auto typeExpr = Identifier::build(builder, loc, formal->name());
  auto node = AnonFormal::build(builder, loc, formal->intent(),
                                std::move(typeExpr));

  return node.release();
}

AstNode*
ParserContext::buildFunctionType(YYLTYPE location, FunctionParts& fp) {
  auto errorExpr = findErrorInFnSignatureParts(this, location, fp);
  if (errorExpr != nullptr) {
    this->clearComments();
    return errorExpr;
  }

  std::ignore = toOwned(fp.name); // Consume to clean up.
  auto loc = convertLocation(location);

  auto kind = (FunctionSignature::Kind) fp.kind;
  owned<Formal> receiver = nullptr;
  auto returnIntent = (FunctionSignature::ReturnIntent) fp.returnIntent;
  const bool parenless = false;
  auto formals = consumeList(fp.formals);

  // Workaround for parsing not being powerful enough to handle this case.
  // In a function expr the formal expression 'x' is a generic formal, but
  // in a function type it is the type 'x'.
  for (auto& ast : formals) {
    if (auto formal = ast->toFormal()) {
      if (!formal->typeExpression() && !formal->initExpression()) {
        auto moved = std::move(ast).release();
        auto anon = consumeFormalToAnonFormal(moved);
        ast = toOwned(anon);
      }
    }
  }

  auto returnType = toOwned(fp.returnType);
  bool throws = fp.throws;

  // TODO: Should this be location of whole type or just the start keyword?
  auto node = FunctionSignature::build(builder, loc, kind,
                                       std::move(receiver),
                                       returnIntent,
                                       parenless,
                                       std::move(formals),
                                       std::move(returnType),
                                       throws);
  auto ret = node.release();
  return ret;
}

CommentsAndStmt
ParserContext::buildExternExportFunctionDecl(YYLTYPE location,
                                             FunctionParts& fp) {
  return buildFunctionDecl(location, fp);
}

CommentsAndStmt
ParserContext::buildRegularFunctionDecl(YYLTYPE location, FunctionParts& fp) {
  return buildFunctionDecl(location, fp);
}

CommentsAndStmt ParserContext::buildFunctionDecl(YYLTYPE location,
                                                 FunctionParts& fp) {
  auto errorExpr = findErrorInFnSignatureParts(this, location, fp);
  if (errorExpr != nullptr) {
    CommentsAndStmt cs = {fp.comments, errorExpr};
    this->clearComments();
    return cs;
  }

  CommentsAndStmt cs = {fp.comments, nullptr};
  assert(!fp.errorExpr);

  bool parenless = (fp.formals == parenlessMarker);
  if (parenless) fp.formals = nullptr; // Don't free the marker.

  // Detect primary methods and create a receiver for them
  bool primaryMethod = false;
  auto scope = currentScope();
  if (currentScopeIsAggregate()) {
    if (fp.receiver == nullptr) {
      auto loc = convertLocation(location);
      auto ths = UniqueString::get(context(), "this");
      UniqueString cls = scope.name;
      fp.receiver = Formal::build(builder, loc, /*attributes*/ nullptr,
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

  // Own the recorded identifier to clean it up, but grab its location.
  owned<Identifier> identName = toOwned(fp.name);
  assert(identName.get());
  auto identNameLoc = builder->getLocation(identName.get());
  assert(!identNameLoc.isEmpty());

  // TODO: It would be nice to get both the location of the entire function
  // as well as the location of the symbol.
  auto f = Function::build(builder, identNameLoc,
                           toOwned(fp.attributes),
                           fp.visibility,
                           fp.linkage,
                           toOwned(fp.linkageNameExpr),
                           identName->name(),
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

  // If we are not a method then the receiver intent is discarded,
  // because there is no receiver formal to store it in.
  // So do the check now.
  // TODO: I think we should redundantly store the receiver intent
  // in the function as well as the receiver formal.
  if (!f->isMethod() && fp.thisIntent != Formal::DEFAULT_INTENT) {
    if (fp.thisIntent == Formal::TYPE)
      CHPL_PARSER_REPORT(this, SecondaryTypeMethodNoType, location);
    else
      CHPL_PARSER_REPORT(this, ThisIntentNotMethod, location);
  }

  this->clearComments();

  cs.stmt = f.release();
  return cs;
}

AstNode* ParserContext::buildLambda(YYLTYPE location, FunctionParts& fp) {
  // drop any comments before the lambda
  clearComments(fp.comments);

  AstNode* ret = nullptr;

  if (fp.errorExpr == nullptr) {
    owned<Block> body;
    if (fp.body != nullptr) {
      body = consumeToBlock(location, fp.body);
    }

    // Own the recorded identifier to clean it up, but grab its location.
    owned<Identifier> identName = toOwned(fp.name);
    assert(identName.get());
    auto identNameLoc = builder->getLocation(identName.get());
    assert(!identNameLoc.isEmpty());

    // TODO: Right now this location is the start of the function, and
    // it seems more natural for the location to be the symbol.
    auto f = Function::build(builder, identNameLoc,
                             toOwned(fp.attributes),
                             Decl::DEFAULT_VISIBILITY,
                             Decl::DEFAULT_LINKAGE,
                             /* linkageName */ nullptr,
                             identName->name(),
                             /* inline */ false,
                             /* override */ false,
                             Function::LAMBDA,
                             /* receiver */ nullptr,
                             fp.returnIntent,
                             fp.throws,
                             /* primaryMethod */ false,
                             /* parenless */ false,
                             this->consumeList(fp.formals),
                             toOwned(fp.returnType),
                             toOwned(fp.where),
                             this->consumeList(fp.lifetime),
                             std::move(body));
    ret = f.release();
  } else {
    ret = fp.errorExpr;
  }
  this->clearComments();
  return ret;
}

AstNode*
ParserContext::buildLetExpr(YYLTYPE location, ParserExprList* decls,
                            AstNode* expr) {
  auto declExprs = consumeList(decls);
  auto node = Let::build(builder, convertLocation(location),
                         std::move(declExprs),
                         toOwned(expr));
  return node.release();
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
AstNode*
ParserContext::buildArrayTypeWithIndex(YYLTYPE location,
                                       YYLTYPE locIndexExprs,
                                       ParserExprList* indexExprs,
                                       AstNode* domainExpr,
                                       AstNode* typeExpr) {
  auto index = buildLoopIndexDecl(locIndexExprs, indexExprs);

  // Reconstruct usable location for 'consumeToBlock()'.
  auto locTypeExpr = builder->getLocation(typeExpr);
  YYLTYPE yyLocTypeExpr = {
    .first_line = locTypeExpr.firstLine(),
    .first_column = locTypeExpr.firstColumn(),
    .last_line = locTypeExpr.lastLine(),
    .last_column = locTypeExpr.lastColumn()
  };

  auto node = BracketLoop::build(builder, convertLocation(location),
                                 std::move(index),
                                 toOwned(domainExpr),
                                 /*withClause*/ nullptr,
                                 BlockStyle::IMPLICIT,
                                 consumeToBlock(yyLocTypeExpr, typeExpr),
                                 /*isExpressionLevel*/ true);

  return node.release();
}

AstNode*
ParserContext::buildArrayType(YYLTYPE location, YYLTYPE locDomainExprs,
                              ParserExprList* domainExprs,
                              AstNode* typeExpr) {

  // In some cases the 'domainExprs' may not exist (think array formal).
  auto domainBody = domainExprs ? consumeList(domainExprs) : AstList();

  // TODO: What if there is only a single expr that is a domain? Do we
  // really want to build '{{...}}'?
  const bool usedCurlyBraces = false;
  auto domain = Domain::build(builder, convertLocation(locDomainExprs),
                              usedCurlyBraces,
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

AstNode* ParserContext::
buildTupleComponent(YYLTYPE location, PODUniqueString name) {
  AstNode* ret = nullptr;

  if (isBuildingFormal) {
    auto node = Formal::build(builder, convertLocation(location),
                              this->buildAttributes(location),
                              name,
                              Formal::DEFAULT_INTENT,
                              /*typeExpression*/ nullptr,
                              /*initExpression*/ nullptr);
    ret = node.release();
  } else {
    auto node = Variable::build(builder, convertLocation(location),
                                this->buildAttributes(location),
                                visibility,
                                linkage,
                                consumeVarDeclLinkageName(),
                                name,
                                varDeclKind,
                                isVarDeclConfig,
                                currentScopeIsAggregate(),
                                /*typeExpression*/ nullptr,
                                /*initExpression*/ nullptr);
    ret = node.release();
  }

  assert(ret != nullptr);

  return ret;
}

AstNode* ParserContext::
buildTupleComponent(YYLTYPE location, ParserExprList* exprs) {
  auto node = TupleDecl::build(builder, convertLocation(location),
                               /*attributes*/ nullptr,
                               this->visibility,
                               this->linkage,
                               (TupleDecl::IntentOrKind) this->varDeclKind,
                               this->consumeList(exprs),
                               /*typeExpression*/ nullptr,
                               /*initExpression*/ nullptr);
  return node.release();
}

owned<Decl> ParserContext::buildLoopIndexDecl(YYLTYPE location,
                                              const AstNode* e) {
  auto convLoc = convertLocation(location);

  if (const Identifier* ident = e->toIdentifier()) {
    return Variable::build(builder, convLoc, /*attributes*/ nullptr,
                           Decl::DEFAULT_VISIBILITY,
                           Decl::DEFAULT_LINKAGE,
                           /*linkageName*/ nullptr,
                           ident->name(),
                           Variable::INDEX,
                           /*isConfig*/ false,
                           /*isField*/ false,
                           /*typeExpression*/ nullptr,
                           /*initExpression*/ nullptr);

  } else if (const uast::Tuple* tup = e->toTuple()) {
    AstList elements;
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

    return TupleDecl::build(builder, convLoc, /*attributes*/ nullptr,
                            Decl::DEFAULT_VISIBILITY,
                            Decl::DEFAULT_LINKAGE,
                            (TupleDecl::IntentOrKind) Variable::INDEX,
                            std::move(elements),
                            /*typeExpression*/ nullptr,
                            /*initExpression*/ nullptr);
  } else {
    CHPL_PARSER_REPORT(this, InvalidIndexExpr, location);
    return nullptr;
  }
}

// TODO: Need way to clear location of 'e' in the builder.
owned<Decl> ParserContext::buildLoopIndexDecl(YYLTYPE location,
                                              owned<AstNode> e) {
  return buildLoopIndexDecl(location, e.get());
}

// TODO: Use me in 'buildBracketLoop' as well.
owned<Decl> ParserContext::buildLoopIndexDecl(YYLTYPE location,
                                              ParserExprList* indexExprs) {
  // TODO: We have to handle the possibility of [1..2, 3..4] here.
  if (indexExprs->size() > 1) {
    CHPL_PARSER_REPORT(this, InvalidIndexExpr, location);
    return nullptr;
  } else {
    auto uncastedIndexExpr = consumeList(indexExprs)[0].release();
    auto indexExpr = uncastedIndexExpr;
    assert(indexExpr);
    return buildLoopIndexDecl(location, toOwned(indexExpr));
  }
}

AstNode* ParserContext::buildNewExpr(YYLTYPE location,
                                     New::Management management,
                                     AstNode* expr) {
  if (FnCall* fnCall = expr->toFnCall()) {
    return this->wrapCalledExpressionInNew(location, management, fnCall);
  } else if (OpCall* opCall = expr->toOpCall()) {
    assert(opCall->numActuals() == 1);
    auto& child = builder->mutableRefToChildren(opCall)[0];
    if (FnCall* fnCall = child->toFnCall()) {
      child.release();
      auto wrappedFn = wrapCalledExpressionInNew(location, management,
                                                 fnCall);
      child.reset(wrappedFn);
      return expr;
    }
  } else {
    if (expr->isIdentifier() && expr->toIdentifier()->numChildren() == 0) {
      // try to capture case of new A; (new without parens)
      CHPL_PARSER_REPORT(this, NewWithoutArgs, location);
    } else if (expr->isDot() ) {
      // try to capture case of var z20a = new C().tmeth;
      // and var z20c = new C()?.tmeth;
      if (expr->toDot()->receiver()->isFnCall() ||
          expr->toDot()->receiver()->isOpCall()) {
        CHPL_PARSER_REPORT(this, DotAfterNew, location);
      } else {
        // try to capture case of new M.Q;
        CHPL_PARSER_REPORT(this, NewWithoutArgs, location);
      }
    } else {
      // It's an error for one reason or another. TODO: Specialize these
      // errors later (e.g. 'new a.field' would require parens around
      // the expression 'a.field'; 'new foo' would require an argument
      // list for 'foo'; and something like 'new __primitive()' just
      // doesn't make any sense...
      CHPL_PARSER_REPORT(this, InvalidNewForm, location);
    }
  }
  auto loc = convertLocation(location);
  return ErroneousExpression::build(builder, loc).release();
}

FnCall* ParserContext::wrapCalledExpressionInNew(YYLTYPE location,
                                                 New::Management management,
                                                 FnCall* fnCall) {
  assert(fnCall->calledExpression());
  bool wrappedBaseExpression = false;

  // Find the child slot containing the called expression. Then remove it,
  // wrap it in a new expression, and swap in the new expression.
  for (auto& child : builder->mutableRefToChildren(fnCall)) {
    if (child.get() != fnCall->calledExpression()) continue;

    // We have the base expression.
    auto calledExpr = std::move(child).release();
    assert(calledExpr);
    auto newExpr = New::build(builder, convertLocation(location),
                              toOwned(calledExpr),
                              management);
    child = std::move(newExpr);

    // Extend the location of the call to the left and patch the location.
    auto fnCallLoc = builder->getLocation(fnCall);
    assert(!fnCallLoc.isEmpty());

    auto updatedLoc = Location(fnCallLoc.path(), location.first_line,
                               location.first_column,
                               fnCallLoc.lastLine(),
                               fnCallLoc.lastColumn());

    builder->noteLocation(fnCall, std::move(updatedLoc));
    wrappedBaseExpression = true;
    break;
  }

  assert(wrappedBaseExpression);
  (void) wrappedBaseExpression;

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

  auto stmts = (!lst || !lst->size()) ? AstList() : consumeList(lst);

  // if it consists of other non-block statements, create a new block
  return Block::build(builder, convertLocation(blockLoc),
                      std::move(stmts));
}

owned<Block>
ParserContext::consumeToBlock(YYLTYPE blockLoc, AstNode* e) {
  auto list = e ? makeList(e) : nullptr;
  return consumeToBlock(blockLoc, list);
}

AstList
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
                                    AstNode* iterandExpr,
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

  AstNode* indexExpr = nullptr;

  if (indexExprs->size() > 1) {
    return {.comments = comments,
            .stmt = CHPL_PARSER_REPORT(this, InvalidIndexExpr, locIndex)};
  } else {
    auto uncastedIndexExpr = consumeList(indexExprs)[0].release();
    indexExpr = uncastedIndexExpr;
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

  AstNode* iterandExpr = nullptr;

  if (iterExprs->size() > 1) {
    return {.comments = comments,
            .stmt = CHPL_PARSER_REPORT(this, InvalidIterandExpr, locIterExprs)};
  } else {
    auto uncastedIterandExpr = consumeList(iterExprs)[0].release();
    iterandExpr = uncastedIterandExpr;
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
                                                   AstNode* indexExpr,
                                                   AstNode* iterandExpr,
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
                                                    AstNode* indexExpr,
                                                    AstNode* iterandExpr,
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
                                                AstNode* indexExpr,
                                                AstNode* iterandExpr,
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
                                                     AstNode* indexExpr,
                                                     AstNode* iterandExpr,
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
                                    AstNode* condition,
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
                                 std::move(thenBlock),
                                 /*isExpressionLevel*/ false);

  // Do NOT clear comments here! Due to lookahead we might clear a valid
  // comment that has already been stored.
  return { .comments=comments, .stmt=node.release() };
}

CommentsAndStmt
ParserContext::buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                    YYLTYPE locThenBodyAnchor,
                                    YYLTYPE locElse,
                                    AstNode* condition,
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

AstNode* ParserContext::buildNumericLiteral(YYLTYPE location,
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
  AstNode* ret = nullptr;
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
      CHPL_PARSER_REPORT(this, InvalidNumericLiteral, location, str.str(), err);
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
        err = "invalid imaginary literal - does not end in 'i'";
      }
      noUnderscoresLen--;
      noUnderscores[noUnderscoresLen] = '\0';
    }

    double num = Param::str2double(noUnderscores, noUnderscoresLen, err);

    if (!err.empty()) {
      CHPL_PARSER_REPORT(this, InvalidNumericLiteral, location, str.str(), err);
      ret = ErroneousExpression::build(builder, loc).release();
    } else if (type == IMAGLITERAL) {
      ret = ImagLiteral::build(builder, loc, num, text).release();
    } else {
      ret = RealLiteral::build(builder, loc, num, text).release();
    }

  } else {
    assert(false && "Case not handled in buildNumericLiteral");
  }

  free(noUnderscores);
  return ret;
}

AstNode* ParserContext::
buildVisibilityClause(YYLTYPE location, owned<AstNode> symbol,
                      VisibilityClause::LimitationKind limitationKind,
                      AstList limitations, bool isImport) {
  if (!symbol->isAs() && !symbol->isIdentifier() && !symbol->isDot()) {
    return CHPL_PARSER_REPORT(this, UseImportNeedsModule, location, isImport);
  }

  for (auto& expr : limitations) {
    auto asExpr = expr->toAs();
    ErroneousExpression* error = nullptr;

    // TODO: Sanitize dot expressions?
    if (expr->isIdentifier() || expr->isDot() || expr->isComment())
      continue;

    if (limitationKind == VisibilityClause::LimitationKind::EXCEPT ||
        limitationKind == VisibilityClause::LimitationKind::ONLY) {
      if (!asExpr) {
        error = CHPL_PARSER_REPORT(this, ExceptOnlyInvalidExpr, location,
                                   limitationKind);
      }
    } else if (limitationKind == VisibilityClause::LimitationKind::BRACES) {
      assert(isImport && "braces should only be used with import statements");
      if (asExpr) {
        if (!asExpr->symbol()->isIdentifier() ||
            !asExpr->rename()->isIdentifier()) {
          error = CHPL_PARSER_REPORT(this, ImportInvalidAs, location);
        }
      } else {
        error = CHPL_PARSER_REPORT(this, ImportInvalidExpr, location);
      }
    }

    if (error) {
      return error;
    }
  }

  auto node = VisibilityClause::build(builder, convertLocation(location),
                                      std::move(symbol),
                                      limitationKind,
                                      std::move(limitations));

  return node.release();
}

AstNode* ParserContext::
buildVisibilityClause(YYLTYPE location, owned<AstNode> symbol, bool isImport) {
  return buildVisibilityClause(location, std::move(symbol),
                               VisibilityClause::NONE,
                               AstList(), isImport);
}


CommentsAndStmt ParserContext::
buildForwardingDecl(YYLTYPE location, owned<Attributes> attributes,
                    owned<AstNode> expr,
                    VisibilityClause::LimitationKind limitationKind,
                    ParserExprList* limitations) {

  auto comments = gatherComments(location);
  if (attributes && attributes->isDeprecated()) {
    CHPL_PARSER_REPORT(this, DeprecateForwardingStmt, location);
  }
  if (limitationKind == VisibilityClause::NONE) {
    auto node = ForwardingDecl::build(builder, convertLocation(location),
                                      std::move(attributes),
                                      std::move(expr));

    return { .comments=comments, .stmt=node.release() };
  }

  auto limitationsList = limitations ? consumeList(limitations) : AstList();

  auto visClause = buildVisibilityClause(location, std::move(expr),
                                         limitationKind,
                                         std::move(limitationsList), false);

  auto node = ForwardingDecl::build(builder, convertLocation(location),
                                    std::move(attributes),
                                    toOwned(visClause));

  return { .comments=comments, .stmt=node.release() };
}


CommentsAndStmt ParserContext::
buildForwardingDecl(YYLTYPE location,
                    owned<Attributes> attributes,
                    CommentsAndStmt cs) {
  assert(cs.stmt->isVariable() || cs.stmt->isMultiDecl() || cs.stmt->isTupleDecl());
  auto decl = cs.stmt->toDecl();
  assert(decl);
  assert(!decl->attributes());
  // TODO: pattern for composing comments should be extracted to helper
  auto commentExprs = appendList(makeList(), cs.comments);
  auto comments = gatherCommentsFromList(commentExprs, location);
  delete commentExprs;

  auto node = ForwardingDecl::build(builder, convertLocation(location),
                                    std::move(attributes),
                                    toOwned(cs.stmt),
                                    decl->visibility());

  return { .comments=comments, .stmt=node.release() };
}



AstNode* ParserContext::buildAsExpr(YYLTYPE locName, YYLTYPE locRename,
                                    owned<AstNode> name,
                                    owned<AstNode> rename) {
  YYLTYPE locEverything = makeSpannedLocation(locName, locRename);

  auto node = As::build(builder, convertLocation(locEverything),
                        std::move(name),
                        std::move(rename));

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

  this->visibility = Decl::DEFAULT_VISIBILITY;

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

  this->visibility = Decl::DEFAULT_VISIBILITY;

  return finishStmt(cs);
}

CommentsAndStmt ParserContext::
buildSingleUseStmt(YYLTYPE locEverything, YYLTYPE locVisibilityClause,
                   Decl::Visibility visibility,
                   owned<AstNode> name,
                   VisibilityClause::LimitationKind limitationKind,
                   ParserExprList* limitationExprs) {
  auto comments = gatherComments(locEverything);

  auto visClause = buildVisibilityClause(locVisibilityClause,
                                         std::move(name),
                                         limitationKind,
                                         consumeList(limitationExprs), false);

  if (visClause->isErroneousExpression()) {
    auto convLoc = convertLocation(locVisibilityClause);
    auto node = ErroneousExpression::build(builder, convLoc);
    return { .comments=comments, .stmt=node.release() };
  }

  AstList uses;
  uses.push_back(toOwned(visClause));

  auto node = Use::build(builder, convertLocation(locEverything),
                         visibility,
                         std::move(uses));

  this->visibility = Decl::DEFAULT_VISIBILITY;

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return finishStmt(cs);
}

// Given a list of vars, build either a single var or a multi-decl.
CommentsAndStmt
ParserContext::buildVarOrMultiDeclStmt(YYLTYPE locEverything,
                                       ParserExprList* vars) {
  int numDecls = 0;
  Decl* firstDecl = nullptr;
  Decl* lastDecl = nullptr;

  for (auto elt : *vars) {
    if (Decl* d = elt->toDecl()) {
      if (!firstDecl) firstDecl = d;
      lastDecl = d;
      numDecls++;
    }
  }

  assert(numDecls > 0);
  assert(firstDecl && lastDecl);

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

    // TODO: Just embed and catch this in a tree-walk instead.
    if (firstDecl->linkageName()) {
      bool isTypeVar = false;

      if (auto typeVar = firstDecl->toVariable()) {
        isTypeVar = typeVar->kind() == Variable::TYPE;
      }

      if (!isTypeVar) {
        CHPL_PARSER_REPORT(this, MultipleExternalRenaming, locEverything);
      }
    }

    auto attributes = buildAttributes(locEverything);
    auto multi = MultiDecl::build(builder, convertLocation(locEverything),
                                  std::move(attributes),
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
                                               asttags::AstTag tag) {
  auto loc = declStartLoc(locStart);

  enterScope(tag, name);

  TypeDeclParts ret = {
    .comments=this->gatherComments(loc),
    .visibility=this->visibility,
    .linkage=this->linkage,
    /* The linkage name must be set by the rule that uses these parts. */
    .linkageName=nullptr,
    .attributes=this->buildAttributes(locStart).release(),
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
    CHPL_PARSER_REPORT(this, ClassExportExtern, location);

    // Clear the linkage state for this so that parsing can continue.
    clearTypeDeclPartsLinkage(parts);
  }

  if (parts.tag == asttags::Union && parts.linkage == Decl::EXPORT) {
    CHPL_PARSER_REPORT(this, UnionExport, location);

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
      if (parts.tag == asttags::Record || parts.tag == asttags::Union) {
        CHPL_PARSER_REPORT(this, RecordInheritanceNotSupported, inheritLoc);
      } else if (parts.tag == asttags::Union) {
        CHPL_PARSER_REPORT(this, UnionInheritanceNotAllowed, inheritLoc);
      } else {
        if (optInherit->size() > 1)
          CHPL_PARSER_REPORT(this, MultipleInheritance, inheritLoc);
        AstNode* ast = (*optInherit)[0];
        if (ast->isIdentifier()) {
          inheritIdentifier = toOwned(ast->toIdentifier());
          (*optInherit)[0] = nullptr;
        } else {
          CHPL_PARSER_REPORT(this, InheritInvalidExpr, inheritLoc);
        }
      }
    }
    consumeList(optInherit); // just to delete it
  }

  AstNode* decl = nullptr;
  if (parts.tag == asttags::Class) {

    // These should have been cleared when validated above (the Class node
    // constructor and builder function would have to change if we ever
    // wanted to permit export/extern classes.
    assert(parts.linkage == Decl::DEFAULT_LINKAGE);
    assert(!parts.linkageName);

    decl = Class::build(builder, convertLocation(location),
                        toOwned(parts.attributes),
                        parts.visibility,
                        parts.name,
                        std::move(inheritIdentifier),
                        std::move(contentsList)).release();
  } else if (parts.tag == asttags::Record) {
    decl = Record::build(builder, convertLocation(location),
                         toOwned(parts.attributes),
                         parts.visibility,
                         parts.linkage,
                         toOwned(parts.linkageName),
                         parts.name,
                         std::move(contentsList)).release();
  } else if (parts.tag == asttags::Union) {

    // Should have been cleared because this is not possible right now.
    assert(parts.linkage != Decl::EXPORT);

    decl = Union::build(builder, convertLocation(location),
                        toOwned(parts.attributes),
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

AstNode* ParserContext::buildReduce(YYLTYPE location,
                                    YYLTYPE locOp,
                                    PODUniqueString op,
                                    AstNode* iterand) {
  auto ident = buildIdent(locOp, op);
  return buildReduce(location, locOp, ident, iterand);
}

AstNode* ParserContext::buildReduce(YYLTYPE location,
                                    YYLTYPE locOp,
                                    AstNode* op,
                                    AstNode* iterand) {
  (void) locOp;
  auto node = Reduce::build(builder, convertLocation(location),
                            toOwned(op),
                            toOwned(iterand));
  return node.release();
}

AstNode* ParserContext::buildReduceIntent(YYLTYPE location,
                                          YYLTYPE locOp,
                                          PODUniqueString op,
                                          AstNode* iterand) {
  auto ident = buildIdent(locOp, op);
  return buildReduceIntent(location, locOp, ident, iterand);
}

AstNode* ParserContext::buildReduceIntent(YYLTYPE location,
                                          YYLTYPE locOp,
                                          AstNode* op,
                                          AstNode* iterand) {
  (void) locOp;
  const Identifier* ident = iterand->toIdentifier();
  if (ident == nullptr) {
    CHPL_PARSER_REPORT(this, ReduceIntentNoIdent, location);
    auto loc = convertLocation(location);
    return ErroneousExpression::build(builder, loc).release();
  }
  auto node = ReduceIntent::build(builder, convertLocation(location),
                                  toOwned(op),
                                  ident->name());
  return node.release();
}

AstNode* ParserContext::buildScan(YYLTYPE location,
                                  YYLTYPE locOp,
                                  PODUniqueString op,
                                  AstNode* iterand) {
  auto ident = buildIdent(locOp, op);
  return buildScan(location, locOp, ident, iterand);
}

AstNode* ParserContext::buildScan(YYLTYPE location,
                                  YYLTYPE locOp,
                                  AstNode* op,
                                  AstNode* iterand) {
  (void) locOp;
  auto node = Scan::build(builder, convertLocation(location),
                          toOwned(op),
                          toOwned(iterand));
  return node.release();
}

AstNode* ParserContext::buildTypeQuery(YYLTYPE location,
                                       PODUniqueString queriedIdent) {
  assert(!queriedIdent.isEmpty() && queriedIdent.c_str()[0] == '?');
  assert(queriedIdent.length() >= 2);
  const char* adjust = queriedIdent.c_str() + 1;
  auto name = UniqueString::get(context(), adjust);
  auto node = TypeQuery::build(builder, convertLocation(location), name);
  return node.release();
}

AstNode* ParserContext::buildTypeConstructor(YYLTYPE location,
                                             PODUniqueString baseType,
                                             MaybeNamedActual actual) {
  auto maybeNamedActuals = new MaybeNamedActualList();
  maybeNamedActuals->push_back(actual);
  return buildTypeConstructor(location, baseType, maybeNamedActuals);
}

AstNode* ParserContext::buildTypeConstructor(YYLTYPE location,
                                             PODUniqueString baseType,
                                             AstNode* subType) {
  auto maybeNamedActuals = new MaybeNamedActualList();

  MaybeNamedActual actual = {
    .expr=subType,
    .name=PODUniqueString::get()
  };

  maybeNamedActuals->push_back(actual);
  return buildTypeConstructor(location, baseType, maybeNamedActuals);
}

AstNode* ParserContext::
buildTypeConstructor(YYLTYPE location, PODUniqueString baseType,
                     MaybeNamedActualList* maybeNamedActuals) {
  auto ident = buildIdent(location, baseType);
  std::vector<UniqueString> actualNames;
  AstList actuals;

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
ParserContext::buildTryExprStmt(YYLTYPE location, AstNode* expr,
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

AstNode*
ParserContext::buildTryExpr(YYLTYPE location, AstNode* expr,
                            bool isTryBang) {
  auto node = Try::build(builder, convertLocation(location), toOwned(expr),
                         isTryBang,
                         /*isExpressionLevel*/ true);
  return node.release();
}

AstNode*
ParserContext::buildEmptyStmt(YYLTYPE location) {
  auto node = EmptyStmt::build(builder, convertLocation(location));
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

  assert(block.stmt != nullptr && block.stmt->isBlock());
  Block* bodyBlock = block.stmt->toBlock();
  auto catches = consumeList(handlers);
  auto node = Try::build(builder, convertLocation(location),
                         toOwned(bodyBlock),
                         std::move(catches),
                         isTryBang);

  CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
  return ret;
}

AstNode* ParserContext::buildCatch(YYLTYPE location, AstNode* error,
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

  if (blockOrDo.usesDo && stmt && stmt->isBlock()) {
    blockStyle = BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK;
  }

  auto caseList = caseExprs ? consumeList(caseExprs) : AstList();

  auto stmtExprs = stmt ? makeList(stmt) : makeList();
  auto stmtList = consumeAndFlattenTopLevelBlocks(stmtExprs);

  auto node = When::build(builder, convertLocation(location),
                          std::move(caseList),
                          blockStyle,
                          std::move(stmtList));

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return cs;
}

CommentsAndStmt
ParserContext::buildSelectStmt(YYLTYPE location, owned<AstNode> expr,
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
      CommentsAndStmt cs = {
          .comments = comments,
          .stmt = CHPL_PARSER_REPORT(this, SelectMultipleOtherwise, location)};
      return cs;
    }
  }

  auto node = Select::build(builder, convertLocation(location),
                            std::move(expr),
                            std::move(stmts));

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return cs;
}

AstNode* ParserContext::buildInterfaceFormal(YYLTYPE location,
                                             PODUniqueString name) {
  return buildFormal(location, Formal::Intent::TYPE, name,
                    /* typeExpr */ nullptr,
                    /* initExpr */ nullptr,
                    /* consumeAttributes */ false);
}

CommentsAndStmt ParserContext::buildInterfaceStmt(YYLTYPE location,
                                                  PODUniqueString name,
                                                  ParserExprList* formals,
                                                  YYLTYPE locBody,
                                                  CommentsAndStmt body) {
  std::vector<ParserComment>* comments;
  ParserExprList* bodyExprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, bodyExprLst, blockStyle, location,
                    false,
                    locBody,
                    body);

  AstList formalList;
  if (formals != nullptr) {
    formalList = consumeList(formals);
  } else {
    auto selfStr = PODUniqueString::get(context(), "Self");
    formalList.push_back(toOwned(buildInterfaceFormal(location, selfStr)));
  }

  AstList bodyStmts = bodyExprLst
      ? consumeAndFlattenTopLevelBlocks(bodyExprLst)
      : AstList();
  const bool isFormalListPresent = formals != nullptr;

  auto node = Interface::build(builder, convertLocation(location),
                               buildAttributes(location),
                               visibility,
                               name,
                               isFormalListPresent,
                               std::move(formalList),
                               std::move(bodyStmts));


  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return cs;
}

owned<AstNode>
ParserContext::buildInterfaceExpr(YYLTYPE location,
                                  PODUniqueString name,
                                  MaybeNamedActualList* formals) {
  owned<AstNode> ret = nullptr;

  ret = Identifier::build(builder, convertLocation(location),
                          name);
  if (formals != nullptr) {
    AstList actuals;
    std::vector<UniqueString> actualNames;
    consumeNamedActuals(formals, actuals, actualNames);
    ret = FnCall::build(builder, convertLocation(location),
                        std::move(ret),
                        std::move(actuals),
                        actualNames,
                        false);
  }

  assert(ret.get() != nullptr);

  return ret;
}

CommentsAndStmt
ParserContext::buildImplementsStmt(YYLTYPE location,
                                   YYLTYPE locInterfaceExpr,
                                   PODUniqueString name,
                                   MaybeNamedActualList* formals) {
  auto comments = gatherComments(location);
  auto interfaceExpr = this->buildInterfaceExpr(locInterfaceExpr,
                                                name,
                                                formals);

  const bool isExpressionLevel = false;
  auto node = Implements::build(builder, convertLocation(location),
                                /*typeExpr*/ nullptr,
                                std::move(interfaceExpr),
                                isExpressionLevel);

  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return cs;
}

CommentsAndStmt
ParserContext::buildImplementsStmt(YYLTYPE location,
                                   YYLTYPE locTypeExpr,
                                   PODUniqueString type,
                                   YYLTYPE locInterfaceExpr,
                                   PODUniqueString name,
                                   MaybeNamedActualList* formals) {

  auto comments = gatherComments(location);
  auto interfaceExpr = buildInterfaceExpr(locInterfaceExpr,
                                          name,
                                          formals);

  auto typeIdent = Identifier::build(builder, convertLocation(locTypeExpr),
                                     type);
  const bool isExpressionLevel = false;
  auto node = Implements::build(builder, convertLocation(location),
                                std::move(typeIdent),
                                std::move(interfaceExpr),
                                isExpressionLevel);
  CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };

  return cs;
}

AstNode*
ParserContext::buildImplementsConstraint(YYLTYPE location,
                                         YYLTYPE locInterfaceExpr,
                                         PODUniqueString name,
                                         MaybeNamedActualList* formals) {
   owned<AstNode> interfaceExpr = this->buildInterfaceExpr(locInterfaceExpr,
                                                          name,
                                                          formals);

  const bool isExpressionLevel = true;
  auto node = Implements::build(builder, convertLocation(location),
                                /*typeExpr*/ nullptr,
                                std::move(interfaceExpr),
                                isExpressionLevel);

  return node.release();
}

AstNode*
ParserContext::buildImplementsConstraint(YYLTYPE location,
                                         YYLTYPE locTypeExpr,
                                         PODUniqueString type,
                                         YYLTYPE locInterfaceExpr,
                                         PODUniqueString name,
                                         MaybeNamedActualList* formals) {
  auto interfaceExpr = buildInterfaceExpr(locInterfaceExpr,
                                          name,
                                          formals);

  auto typeExpr = Identifier::build(builder, convertLocation(locTypeExpr),
                                    type);

  const bool isExpressionLevel = true;
  auto node = Implements::build(builder, convertLocation(location),
                                std::move(typeExpr),
                                std::move(interfaceExpr),
                                isExpressionLevel);

  return node.release();
}

CommentsAndStmt
ParserContext::buildLabelStmt(YYLTYPE location, PODUniqueString name,
                              CommentsAndStmt cs) {
  if (cs.stmt && (cs.stmt->isFor() || cs.stmt->isWhile() ||
                  cs.stmt->isDoWhile())) {
    auto exprLst = makeList(cs);
    auto comments = gatherCommentsFromList(exprLst, location);
    auto astLst = consumeList(exprLst);
    Loop* loop = nullptr;
    for (auto& ast : astLst) {
      if (ast->isLoop()) {
        loop = ast.release()->toLoop();
        break;
      }
    }
    assert(loop);
    auto node = Label::build(builder, convertLocation(location), name,
                             toOwned(loop));
    return { .comments=comments, .stmt=node.release() };
  } else {
    return finishStmt(CHPL_PARSER_REPORT(this, LabelIneligibleStmt, location));
  }
}
