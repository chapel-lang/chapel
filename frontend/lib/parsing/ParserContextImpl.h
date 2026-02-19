/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
#include "chpl/parsing/parser-error.h"

#include <cerrno>
#include <cfloat>
#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

using chpl::types::Param;
using chpl::owned;

static bool locationLessEq(YYLTYPE lhs, YYLTYPE rhs) {
  return (lhs.first_line < rhs.first_line) ||
         (lhs.first_line == rhs.first_line &&
          lhs.first_column <= rhs.first_column);
}


static bool checkNoDuplicateNamedActuals(const MaybeNamedActualList& actuals,
                                         std::set<UniqueString>& duplicates) {
  std::set<UniqueString> seen;
  for (auto& actual : actuals) {
    if (actual.name.isEmpty()) {
      continue;
    }
    if (seen.count(actual.name) > 0) {
      duplicates.insert(actual.name);
    }
    seen.insert(actual.name);
  }
  return duplicates.size() == 0;
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
  CHPL_ASSERT(!hasNotedVarDeclKind);
  this->hasNotedVarDeclKind = true;
  this->varDeclKind = varDeclKind;
  return this->varDeclKind;
}

void ParserContext::storeVarDeclLinkageName(AstNode* linkageName) {
  CHPL_ASSERT(this->varDeclLinkageName == nullptr);
  this->varDeclLinkageName = linkageName;
}

owned<AstNode> ParserContext::consumeVarDeclLinkageName(void) {
  auto ret = this->varDeclLinkageName;
  this->varDeclLinkageName = nullptr;
  return toOwned(ret);
}

owned<Attribute> ParserContext::buildAttribute(YYLTYPE loc, AstNode* firstIdent,
                                               ParserExprList* toolspace,
                                               MaybeNamedActualList* actuals) {

  UniqueString fullName;
  std::string tmpName;

  // the firstIdentifier may be the name of the attribute of the first element
  // of the toolspace. If so, we need to move it into the toolspace and get the
  // name of the attribute from the last entry in the toolspace.
  // e.g. @toolspace.attribute vs @attribute
  if (toolspace == nullptr) {
    toolspace = makeList();
  }
  if (actuals == nullptr) {
    actuals = new MaybeNamedActualList();
  }
  if (toolspace->size() == 0) {
    fullName = firstIdent->toIdentifier()->name();
  } else {
    tmpName = firstIdent->toIdentifier()->name().str();
    for (auto& tool : *toolspace) {
      tmpName += "." + tool->toIdentifier()->name().str();
    }
    fullName = UniqueString::get(context(), tmpName);
  }

  AstList actualsForReal;
  std::vector<UniqueString> actualNames;
  consumeNamedActuals(actuals, actualsForReal, actualNames);
  auto node = Attribute::build(builder, convertLocation(loc), fullName,
                               std::move(actualsForReal),
                               std::move(actualNames));
  return node;
}

owned<AttributeGroup> ParserContext::popLoopAttributeGroup() {
  CHPL_ASSERT(loopAttributes.size() > 0);
  auto toReturn = std::move(loopAttributes.back());
  loopAttributes.pop_back();
  return toReturn;
}

owned<AttributeGroup> ParserContext::buildAttributeGroup(YYLTYPE locationOfDecl) {
  numAttributesBuilt += 1;

  // There may be nothing to return.
  if (!hasAttributeGroupParts) {
    return nullptr;
  }

  // Create a local copy of the attributeGroup we can move into the node.
  auto pragmaCopy = attributeGroupParts.pragmas
      ? *(attributeGroupParts.pragmas)
      : std::set<PragmaTag>();

  AstList attrList = consumeList(attributeGroupParts.attributeList);

  auto node = AttributeGroup::build(builder, convertLocation(locationOfDecl),
                                std::move(pragmaCopy),
                                attributeGroupParts.isDeprecated,
                                attributeGroupParts.isUnstable,
                                attributeGroupParts.isParenfulDeprecated,
                                attributeGroupParts.hasEdition,
                                attributeGroupParts.deprecationMessage,
                                attributeGroupParts.unstableMessage,
                                attributeGroupParts.parenfulDeprecationMessage,
                                attributeGroupParts.firstEdition,
                                attributeGroupParts.lastEdition,
                                std::move(attrList));
  return node;
}


void ParserContext::buildAndPushAttributeGroup(YYLTYPE locationOfDecl) {
  auto attributeGroup = this->buildAttributeGroup(std::move(locationOfDecl));
  if (attributeGroup != nullptr) {
    this->resetAttributeGroupPartsState();
  }
  this->loopAttributes.push_back(std::move(attributeGroup));
}

PODUniqueString ParserContext::notePragma(YYLTYPE loc,
                                          AstNode* pragmaStr) {
  auto ret = PODUniqueString::get();

  // Extract the string literal and convert it into a pragma flag.
  if (auto strLit = pragmaStr->toStringLiteral()) {
    ret = PODUniqueString::get(context(), strLit->value().c_str());
    auto tag = pragmaNameToTag(ret.c_str());

    if (tag == PRAGMA_UNKNOWN)
      error(loc, "unknown pragma \"%s\".", strLit->value().c_str());

    // Initialize the pragma flags if needed.
    auto& pragmas = attributeGroupParts.pragmas;
    if (pragmas == nullptr) pragmas = new std::set<PragmaTag>();
    hasAttributeGroupParts = true;

    // Always insert, even if PRAGMA_UNKNOWN.
    pragmas->insert(tag);
  }

  // Make sure to clean up the UAST node since it will be discarded.
  delete pragmaStr;

  return ret;
}

void ParserContext::noteAttribute(YYLTYPE loc, AstNode* firstIdent,
                                  ParserExprList* toolspace,
                                  MaybeNamedActualList* actuals) {
  hasAttributeGroupParts = true;

  // initialize the list if it wasn't already
  auto& attrs = attributeGroupParts.attributeList;
  if (attrs == nullptr) {
    attrs = makeList();
  }

  // do some special handling for unstable and deprecated, because they
  // existed before the changes to allow general attributes
  auto ident = firstIdent->toIdentifier();
  if (ident->name()==USTR("unstable")) {
    noteUnstable(loc, actuals);
  } else if (ident->name()==USTR("deprecated")) {
    noteDeprecation(loc, actuals);
  } else if (ident->name()==USTR("stable")) {
    noteStable(loc, actuals);
  } else if (ident->name() == USTR("edition")) {
    noteEdition(loc, actuals);
  }

  // check the actual names are not duplicates
  if (actuals != nullptr) {
    std::set<UniqueString> duplicates;
    if (!checkNoDuplicateNamedActuals(*actuals, duplicates)) {
      for (auto& duplicate : duplicates) {
        error(loc, "repeated argument name '%s'", duplicate.c_str());
      }
    }
  }

  // make sure we don't put duplicate attributes (based on names) on one symbol
  auto attr = buildAttribute(loc, ident, toolspace, actuals);
  for (auto& attribute : *attrs) {
    if (attribute->toAttribute()->name() == attr->toAttribute()->name()) {
      error(loc, "repeated attribute '%s'", attr->toAttribute()->name().c_str());
    }
  }
  attrs = appendList(attrs, attr.release());
}



void ParserContext::noteDeprecation(YYLTYPE loc, MaybeNamedActualList* actuals) {
  hasAttributeGroupParts = true;

  if (attributeGroupParts.isStable) {
    error(loc, "cannot apply both stable and deprecated attributes to the same symbol");
  }

  bool isParenfulDeprecation = false;
  AstNode* messageStr = nullptr;
  bool allActualsNamed = true;

  if (actuals != nullptr && actuals->size() > 0) {
    for (auto& actual : *actuals) {
      if (!(actual.name == UniqueString::get(context(), "since").podUniqueString() ||
            actual.name == UniqueString::get(context(), "notes").podUniqueString() ||
            actual.name == UniqueString::get(context(), "suggestion").podUniqueString()||
            actual.name == UniqueString::get(context(), "parenful").podUniqueString() ||
            actual.name.isEmpty())) {
        error(loc, "unrecognized argument name '%s'. "
                   "'@deprecated' attribute only accepts 'since', 'notes', "
                   "'parenful', and 'suggestion' arguments",
                   actual.name.c_str());
      }
      if (actual.name.isEmpty()) {
        allActualsNamed = false;
      }
      if (actual.name == UniqueString::get(context(), "parenful").podUniqueString()) {
        auto boolLit = actual.expr->toBoolLiteral();
        if (!boolLit || !boolLit->value()) {
          error(loc, "invalid value for deprecated attribute's 'parenful' "
                     "parameter; only 'true' is supported.");
        }
        isParenfulDeprecation = true;
      } else if (!actual.expr->isStringLiteral()) {
        error(loc, "deprecated attribute arguments must be string literals for now");
      }
      // TODO: Decide how this interaction should work, if we want to continue
      // supporting "message" or if we should adapt that field to match the
      // argument names here
      // For now, use the notes field, or assume if there's only one argument
      // then it's the message
      if (actual.name == UniqueString::get(context(), "notes").podUniqueString() ||
          actuals->size() == 1) {
        messageStr = actual.expr;
      }
    }
  }
  if (!allActualsNamed && actuals->size() > 1) {
    error(loc, "deprecated attribute only accepts one unnamed argument");
  }

  UniqueString message;
  if (messageStr) {
    if (auto strLit = messageStr->toStringLiteral()) {
      message = strLit->value();
    }
  }

  if (isParenfulDeprecation) {
    attributeGroupParts.isParenfulDeprecated = true;
    attributeGroupParts.parenfulDeprecationMessage = message;
  } else {
    attributeGroupParts.isDeprecated = true;
    attributeGroupParts.deprecationMessage = message;
  }
}

void ParserContext::noteStable(YYLTYPE loc, MaybeNamedActualList* actuals) {
  hasAttributeGroupParts = true;

  if (attributeGroupParts.isUnstable) {
    error(loc, "cannot apply both unstable and stable attributes to the same symbol");
  }
  // TODO: does it make sense to deprecate something that's stable? Shouldn't
  // the stable attribute come off when it's marked deprecated?
  if (attributeGroupParts.isDeprecated) {
    error(loc, "cannot apply both deprecated and stable attributes to the same symbol");
  }
  attributeGroupParts.isStable = true;
  if (actuals->size() > 1) {
    error(loc, "stable attribute only accepts one argument");
  }
  if (actuals != nullptr && actuals->size() > 0) {
    for (auto& actual : *actuals) {
      if (!(actual.name == UniqueString::get(context(), "since").podUniqueString() ||
            actual.name.isEmpty())) {
        error(loc, "unrecognized argument name '%s'. "
                   "'@stable' attribute only accepts 'since' or unnamed argument",
                   actual.name.c_str());
      }
      if (!actual.expr->isStringLiteral()) {
        error(loc, "stable attribute arguments must be string literals for now");
      }
    }
  }
}

void ParserContext::noteUnstable(YYLTYPE loc, MaybeNamedActualList* actuals) {
  hasAttributeGroupParts = true;

  if (attributeGroupParts.isStable) {
    error(loc, "cannot apply both unstable and stable attributes to the same symbol");
  }
  attributeGroupParts.isUnstable = true;

  AstNode* messageStr = nullptr;
  bool allActualsNamed = true;
  if (actuals != nullptr && actuals->size() > 0) {
    for (auto& actual : *actuals) {
      if (!(actual.name == UniqueString::get(context(), "category").podUniqueString() ||
            actual.name == UniqueString::get(context(), "issue").podUniqueString() ||
            actual.name == UniqueString::get(context(), "reason").podUniqueString()||
            actual.name.isEmpty())) {
        error(loc, "unrecognized argument name '%s'. "
                   "'@unstable' attribute only accepts 'category', 'issue', "
                   "and 'reason' arguments",
                   actual.name.c_str());
      }
      if (actual.name.isEmpty()) {
        allActualsNamed = false;
      }
      if (!actual.expr->isStringLiteral()) {
        error(loc, "unstable attribute arguments must be string literals for now");
      }
     // use the reason field or assume if there's only one argument it's the message
      if (actual.name == UniqueString::get(context(), "reason").podUniqueString() ||
          actuals->size() == 1) {
        messageStr = actual.expr;
      }
    }
  }
  if (!allActualsNamed && actuals->size() > 1) {
    error(loc, "unstable attribute only accepts one unnamed argument");
  }
  if (messageStr) {
    if (auto strLit = messageStr->toStringLiteral()) {
      attributeGroupParts.unstableMessage = strLit->value();
    }
  }
}

void ParserContext::noteEdition(YYLTYPE loc, MaybeNamedActualList* actuals) {
  hasAttributeGroupParts = true;

  attributeGroupParts.hasEdition = true;

  if (actuals != nullptr && actuals->size() > 0) {
    for (auto& actual : *actuals) {
      if (actual.name.isEmpty()) {
        error(loc, "'@edition' attribute argument must be named");
      }

      if (!(actual.name == UniqueString::get(context(), "first").podUniqueString() ||
            actual.name == UniqueString::get(context(), "last").podUniqueString() ||
            actual.name.isEmpty())) {
        error(loc, "unrecognized argument name '%s'. "
                   "'@edition' attribute only accepts 'first' and 'last' "
                   "arguments",
                   actual.name.c_str());
      }
      if (!actual.expr->isStringLiteral()) {
        error(loc, "'@edition' attribute arguments must be string literals for now");
      }

      if (actual.name == UniqueString::get(context(),
                                           "first").podUniqueString()) {
        AstNode* firstStr = actual.expr;

        if (auto strLit = firstStr->toStringLiteral()) {
          attributeGroupParts.firstEdition = strLit->value();
        }
      }

      if (actual.name == UniqueString::get(context(),
                                           "last").podUniqueString()) {
        AstNode* lastStr = actual.expr;

        if (auto strLit = lastStr->toStringLiteral()) {
          attributeGroupParts.lastEdition = strLit->value();
        }
      }
    }
  } else {
    error(loc, "'@edition' attribute requires an argument");
  }
}

void ParserContext::resetAttributeGroupPartsState() {
  if (hasAttributeGroupParts) {
    auto& pragmas = attributeGroupParts.pragmas;
    if (pragmas) delete pragmas;
    attributeGroupParts = {nullptr, nullptr, false, false, false, false, false, UniqueString(), UniqueString(), UniqueString(), UniqueString(), UniqueString() };
    hasAttributeGroupParts = false;
  }

  CHPL_ASSERT(attributeGroupParts.pragmas == nullptr);
  CHPL_ASSERT(attributeGroupParts.attributeList == nullptr);
  CHPL_ASSERT(!attributeGroupParts.isDeprecated);
  CHPL_ASSERT(!attributeGroupParts.isUnstable);
  CHPL_ASSERT(!attributeGroupParts.isParenfulDeprecated);
  CHPL_ASSERT(!attributeGroupParts.isStable);
  CHPL_ASSERT(!attributeGroupParts.hasEdition);
  CHPL_ASSERT(attributeGroupParts.deprecationMessage.isEmpty());
  CHPL_ASSERT(attributeGroupParts.unstableMessage.isEmpty());
  CHPL_ASSERT(attributeGroupParts.parenfulDeprecationMessage.isEmpty());
  CHPL_ASSERT(attributeGroupParts.firstEdition.isEmpty());
  CHPL_ASSERT(attributeGroupParts.lastEdition.isEmpty());
  CHPL_ASSERT(!hasAttributeGroupParts);

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
  // CHPL_ASSERT(numAttributesBuilt == 0);

  if (cs.stmt && cs.stmt->isDecl()) {

    // If a decl was produced then the attributeGroup should have been reset.
    // If they were _not_ reset, then it means that a deprecated statement
    // came before a pragma list.
    // TODO: Can we just make deprecated_stmt and pragma_ls alternates?
    // This solves that problem.
    if (hasAttributeGroupParts) {
      CHPL_ASSERT(attributeGroupParts.pragmas == nullptr);
      CHPL_ASSERT(attributeGroupParts.isDeprecated);
      CHPL_ASSERT(attributeGroupParts.isUnstable);
      syntax(loc, "pragma list must come before deprecation statement.");
    }

  } else if (cs.stmt && cs.stmt->isErroneousExpression()) {
    // do nothing on an erroneous statement
    // Clean up the attribute parts.
    resetAttributeGroupPartsState();
  } else if (cs.stmt && cs.stmt->isLoop()) {
    // Loop pragmas are captured by the loop's own attribute handling logic.
    // Not an error, and resetAttributeGroupPartsState() has been invoked.
  } else {
    // TODO: The original builder also states the first pragma.
    CHPL_PARSER_REPORT(this, CannotAttachPragmas, loc, cs.stmt);

    // Clean up the attribute parts.
    resetAttributeGroupPartsState();
  }

  return ret;
}

bool ParserContext::noteIsBuildingFormal(bool isBuildingFormal) {
  this->isBuildingFormal = isBuildingFormal;
  return this->isBuildingFormal;
}

bool ParserContext::noteIsVarDeclConfig(bool isConfig, YYLTYPE loc) {
  this->isVarDeclConfig = isConfig;
  this->configLoc = std::move(loc);
  return this->isVarDeclConfig;
}

void ParserContext::noteVarDestinationExpr(AstNode* targetExpr) {
  this->varDestinationExpr = targetExpr;
}

owned<AstNode> ParserContext::consumeVarDestinationExpr() {
  auto toReturn = toOwned(this->varDestinationExpr);
  this->varDestinationExpr = nullptr;
  return toReturn;
}

YYLTYPE ParserContext::declStartLoc(YYLTYPE curLoc) {
  if (this->declStartLocation.first_line == 0)
    return curLoc;
  else
    return this->declStartLocation;
}

void ParserContext::resetDeclStateOnError() {
  // Consume the linkage name just to clean it up.
  std::ignore = consumeVarDeclLinkageName();
  std::ignore = consumeVarDestinationExpr();
  this->resetDeclState();
}

void ParserContext::resetDeclState() {
  this->resetAttributeGroupPartsState();
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
  CHPL_ASSERT(scopeStack.size() > 0);
  CHPL_ASSERT(scopeStack.back().tag == tag);
  CHPL_ASSERT(scopeStack.back().name == name);
  scopeStack.pop_back();
}


void ParserContext::noteCurlyBraces(YYLTYPE left, YYLTYPE right) {
  this->curlyBraceLocation = makeSpannedLocation(left, right);
}
bool ParserContext::isValidCurlyBracesLoc(YYLTYPE loc) {
  return loc.first_line > 0;
}
bool ParserContext::hasCurlyBracesLoc() {
  return isValidCurlyBracesLoc(this->curlyBraceLocation);
}
YYLTYPE ParserContext::curlyBracesLoc() {
  return this->curlyBraceLocation;
}
void ParserContext::resetCurlyBracesLoc() {
  this->curlyBraceLocation = {0};
}


ErroneousExpression* ParserContext::report(YYLTYPE loc, owned<ErrorBase> error) {
  context()->report(std::move(error));
  return ErroneousExpression::build(builder, convertLocation(loc)).release();
}

#define CHPL_PARSER_CONTEXT_LOG_ERROR_HELPER(p_context__, loc__, kind__, fmt__) \
  do { \
    va_list vl; \
    va_start(vl, fmt); \
    auto reportLoc = p_context__->convertLocation(loc__); \
    auto result = p_context__->report(loc__, \
        GeneralError::vbuild(kind__, reportLoc, fmt__, vl)); \
    va_end(vl); \
    return result; \
  } while(false)

ErroneousExpression* ParserContext::error(YYLTYPE loc, const char* fmt, ...) {
  CHPL_PARSER_CONTEXT_LOG_ERROR_HELPER(this, loc, ErrorBase::ERROR, fmt);
}
ErroneousExpression* ParserContext::syntax(YYLTYPE loc, const char* fmt, ...) {
  CHPL_PARSER_CONTEXT_LOG_ERROR_HELPER(this, loc, ErrorBase::SYNTAX, fmt);
}

void ParserContext::noteComment(YYLTYPE loc, const char* data, long size) {
  if (!includeComments) {
    return;
  }

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

void ParserContext::noteComment(ParserComment pc) {
  if (!includeComments) {
    return;
  }

  if (this->comments == nullptr) {
    this->comments = new std::vector<ParserComment>();
  }
  this->comments->push_back(pc);
}

void ParserContext::clearCommentsBefore(YYLTYPE loc) {
  auto comments = this->gatherComments(loc);
  clearComments(comments);
}

// Gather all the comments <= 'loc' and hold onto them. Clear all remaining
// comments, and then re-note the comments in 'keep' in the order they
// first appeared.
void ParserContext::clearCommentsAfter(YYLTYPE loc) {
  if (auto keep = gatherComments(loc)) {
    clearComments();
    for (auto pc : *keep) noteComment(pc);
    keep->clear();
    delete keep;
  }
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
      CHPL_ASSERT(expr->isComment());
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
  CHPL_ASSERT(e != nullptr);
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
  CHPL_ASSERT(e != nullptr);
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

ParserNDArrayList* ParserContext::makeNDArrayList() {
  return new ParserNDArrayList();
}
ParserNDArrayList* ParserContext::appendNDArrayList(ParserNDArrayList* dst,
                                                    NDArrayElement e) {
  dst->push_back(e);
  return dst;
}

struct BuildArrayRowsContext {
  std::unordered_set<int> errorInconsistentSeparatorIdx;
  std::unordered_set<int> errorSingleSeparatorIdx;

  void realizeErrors(ParserContext* context, ParserNDArrayList* lst) {
    if (!errorInconsistentSeparatorIdx.empty()) {
      for (auto i : errorInconsistentSeparatorIdx) {
        context->error(
          (*lst)[i].location,
          "inconsistent number of multi-dimensional array separators");
      }
    }
    if (!errorSingleSeparatorIdx.empty()) {
      for (auto i : errorSingleSeparatorIdx) {
        context->error(
          (*lst)[i].location,
          "the final dimension of an array must use a single separator");
      }
    }
  }

  std::pair<AstList, chpl::Location> buildArrayRows(
    ParserContext* context, ParserNDArrayList* lst,
    Location arrayLoc, int start, int stop,
    int previousNumSep = -1, std::unordered_set<int> previousMaxSepIdx = {}
  ) {
    CHPL_ASSERT(start <= stop);
    if (start == stop) {
      if (previousNumSep != -1 && previousNumSep != 1) {
        auto errIdx = start;
        if (start > 0 && (*lst)[start-1].nSeparator == previousNumSep) {
          errIdx = start-1;
        } else if (start < (int)lst->size()-1 &&
                   (*lst)[start+1].nSeparator == previousNumSep) {
          errIdx = start+1;
        }
        errorSingleSeparatorIdx.insert(errIdx);
      }
      // there is a single row, so just return it
      return std::make_pair(
        context->consumeList((*lst)[start].exprs),
        context->convertLocation((*lst)[start].location)
      );
    }

    // find the largest number of consecutive sep
    int maxSep = 0;
    std::unordered_set<int> maxSeps;
    for (int i = start; i <= stop; i++) {
      if ((*lst)[i].nSeparator > maxSep) {
        maxSep = (*lst)[i].nSeparator;
        maxSeps.clear();
        maxSeps.insert(i);
      } else if ((*lst)[i].nSeparator == maxSep) {
        maxSeps.insert(i);
      }
    }
    CHPL_ASSERT(maxSep > 0);
    if (previousNumSep != -1 && maxSep+1 != previousNumSep) {
      errorInconsistentSeparatorIdx = previousMaxSepIdx;
    }


    // iterate through the list from start to stop, each time we find a maxSep
    // recurse on the sublist from the last maxSep to the current element
    AstList ret;
    int start_ = start;
    for (int i = start_; i <= stop; i++) {
      if ((*lst)[i].nSeparator == maxSep) {
        auto [newRows, loc] = buildArrayRows(context, lst, arrayLoc,
                                            start_, i-1, maxSep, maxSeps);
        ret.push_back(ArrayRow::build(context->builder, loc, std::move(newRows)));
        start_ = i+1;
      }
    }
    // add the last row
    auto [newRows, loc] = buildArrayRows(context, lst, arrayLoc,
                                        start_, stop, maxSep, maxSeps);
    ret.push_back(ArrayRow::build(context->builder, loc, std::move(newRows)));

    return std::make_pair(std::move(ret), arrayLoc);
  }
};

Array* ParserContext::buildNDArray(YYLTYPE location, ParserNDArrayList* lst) {
  CHPL_ASSERT(lst->size() > 0);
  auto loc = convertLocation(location);

  BuildArrayRowsContext barc;
  auto rows = barc.buildArrayRows(this, lst, loc,
                                   /*start*/0, /*end*/lst->size()-1);
  barc.realizeErrors(this, lst);
  delete lst;
  return Array::build(builder, loc, std::move(rows.first)).release();
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
      CHPL_ASSERT(search != this->commentLocations.end());
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
    CHPL_ASSERT(c);
    auto search = this->commentLocations.find(c);
    CHPL_ASSERT(search != this->commentLocations.end());
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
Identifier* ParserContext::buildAttributeIdent(YYLTYPE location,
                                               PODUniqueString name) {
  // remove the leading '@' from the name
  UniqueString n = name;
  if (n.startsWith("@")) {
    // remove the @ sign
    n = UniqueString::get(context(), n.c_str()+1);
  } else {
    CHPL_ASSERT(false && "expect attributes to start with @");
  }
  return Identifier::build(builder, convertLocation(location), n).release();
}

AstNode*
ParserContext::buildTaskIntent(YYLTYPE loc,
                               YYLTYPE nameLoc,
                               AttributeGroup* attributeGroup,
                               AstNode* identNode,
                               MaybeIntent intent,
                               AstNode* typeExpression,
                               AstNode* initExpression) {
  if (!intent.isValid) {
    return syntax(loc, "'%s' intent is not supported in a 'with' clause", qualifierToString((Qualifier)intent.intent));
  }

  if (auto ident = identNode->toIdentifier()) {
    auto name = ident->name();
    auto node = TaskVar::build(builder,
                               convertLocation(loc),
                               toOwned(attributeGroup),
                               name,
                               (TaskVar::Intent)intent.intent,
                               toOwned(typeExpression),
                               toOwned(initExpression));
    builder->noteDeclNameLocation(node.get(), convertLocation(nameLoc));
    return node.release();
  } else {
    return syntax(loc, "expected identifier for task variable name.");
  }

}


WithClause*
ParserContext::buildWithClause(YYLTYPE location, YYLTYPE locWith,
                               YYLTYPE locLeftParen,
                               YYLTYPE locTaskIntentList,
                               YYLTYPE locRightParen,
                               ParserExprList* exprList) {
  auto exprs = consumeList(exprList);
  auto node = WithClause::build(builder, convertLocation(location),
                                std::move(exprs));
  auto ret = node.release();
  clearCommentsAfter(locWith);
  return ret;
}

CommentsAndStmt
ParserContext::buildBeginStmt(YYLTYPE location, YYLTYPE locBegin,
                              YYLTYPE locWithClause,
                              YYLTYPE locStmt,
                              WithClause* withClause,
                              CommentsAndStmt stmt) {
  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;
  YYLTYPE locBodyAnchor = makeLocationAtLast(locWithClause);
  prepareStmtPieces(comments, exprLst, blockStyle, locBegin, false,
                    locBodyAnchor,
                    stmt);
  auto stmts = consumeAndFlattenTopLevelBlocks(exprLst);
  auto node = Begin::build(builder, convertLocation(location),
                           toOwned(withClause),
                           blockStyle,
                           std::move(stmts));
  builder->noteBlockHeaderLocation(node.get(), convertLocation(locBegin));
  if (isValidCurlyBracesLoc(stmt.curlyLoc)) {
    builder->noteCurlyBracesLocation(node.get(), convertLocation(stmt.curlyLoc));
  }
  CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());
  return finishStmt(ret);
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
    if (auto lit = actuals[0]->toStringLiteral()) {
      primName = lit->value();
      // and erase that element
      actuals.erase(actuals.begin());
    }
  }

  if (anyNames || primName.isEmpty()) {
    if (anyNames)
      syntax(location, "primitive calls cannot use named arguments.");
    else
      syntax(location, "primitive calls must start with string literal.");

    return ErroneousExpression::build(builder, loc).release();
  }

  PrimitiveTag tag = primNameToTag(primName.c_str());
  if (tag == PRIM_UNKNOWN) {
    error(location, "unknown primitive '%s'.", primName.c_str());
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
    CHPL_PARSER_REPORT(this, PreIncDecOp, location, /* isDoublePlus */ true);
    ustrOp = USTR("+");
    // conver the ++a to +(+a)
    auto innerOp = OpCall::build(builder, convertLocation(location),
                       ustrOp, toOwned(expr)).release();
    return OpCall::build(builder, convertLocation(location),
                       ustrOp, toOwned(innerOp)).release();
  } else if (ustrOp == "--") {
    CHPL_PARSER_REPORT(this, PreIncDecOp, location, /* isDoublePlus */ false);
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

AstNode* ParserContext::buildDot(YYLTYPE location, YYLTYPE locReceiver,
                                 YYLTYPE locPeriod,
                                 YYLTYPE locField,
                                 AstNode* receiver,
                                 UniqueString field,
                                 bool wrapInCall) {
  auto locDotExpr = makeSpannedLocation(locReceiver, locField);
  auto dot = Dot::build(builder, convertLocation(locDotExpr),
                        toOwned(receiver),
                        field);
  builder->noteDotFieldLocation(dot.get(), convertLocation(locField));
  if (wrapInCall) {
    auto call = FnCall::build(builder, convertLocation(location),
                              std::move(dot),
                              false);
    return call.release();
  }
  return dot.release();
}

AstNode* ParserContext::buildManagerExpr(YYLTYPE location,
                                         AstNode* expr,
                                         Variable::Kind kind,
                                         YYLTYPE locResourceName,
                                         UniqueString resourceName) {
  auto nameLoc = convertLocation(locResourceName);
  auto var = Variable::build(builder, nameLoc, nameLoc,
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
                                               YYLTYPE locHeader,
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
  AstList stmts = consumeAndFlattenTopLevelBlocks(stmtExprList);

  auto node = Manage::build(builder, convertLocation(location),
                            std::move(managers),
                            blockStyle,
                            std::move(stmts));
  builder->noteBlockHeaderLocation(node.get(), convertLocation(locHeader));
  if (isValidCurlyBracesLoc(blockOrDo.cs.curlyLoc)) {
    builder->noteCurlyBracesLocation(node.get(), convertLocation(blockOrDo.cs.curlyLoc));
  }
  CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());

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
                      makeIntent(Formal::DEFAULT_INTENT),
                      YYLTYPE::create(),
                      nullptr,
                      nullptr,
                      makeIntent(Function::DEFAULT_RETURN_INTENT),
                      YYLTYPE::create(),
                      false,
                      nullptr, nullptr, nullptr, nullptr,
                      nullptr,
                      YYLTYPE::create(),
                      YYLTYPE::create()};
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

static inline
Function::ReturnIntent checkReturnIntent(ParserContext* context,
                                         YYLTYPE location,
                                         MaybeIntent intent) {
  auto returnIntent = Function::ReturnIntent::DEFAULT_RETURN_INTENT;
  if (!intent.isValid) {
    context->syntax(location, "'%s' intent is not a supported return intent", qualifierToString((Qualifier)intent.intent));
  } else {
    returnIntent = (Function::ReturnIntent)intent.intent;
  }
  return returnIntent;
}

AstNode*
ParserContext::buildFunctionExpr(YYLTYPE location, FunctionParts& fp) {
  if (fp.isBodyNonBlockExpression) CHPL_ASSERT(false && "Not handled!");

  clearComments(fp.comments);

  AstNode* ret = nullptr;

  if (fp.errorExpr != nullptr) {
    ret = fp.errorExpr;
    this->clearComments();
    return ret;
  }

  owned<Block> body = consumeToBlock(location, fp.body);

  // Own the recorded identifier to clean it up, but grab its location.
  owned<Identifier> identName = toOwned(fp.name);
  CHPL_ASSERT(identName.get());
  auto identNameLoc = builder->getLocation(identName.get());
  CHPL_ASSERT(!identNameLoc.isEmpty());

  auto returnIntent = checkReturnIntent(this, fp.returnIntentLoc, fp.returnIntent);

  auto f = Function::build(builder, identNameLoc,
                           toOwned(fp.attributeGroup),
                           Decl::DEFAULT_VISIBILITY,
                           Decl::DEFAULT_LINKAGE,
                           /*linkageName*/ nullptr,
                           UniqueString(),
                           /*inline*/ false,
                           /*override*/ false,
                           fp.kind,
                           /* receiver */ nullptr,
                           returnIntent,
                           fp.throws,
                           /*primaryMethod*/ false,
                           /*parenless*/ false,
                           this->consumeList(fp.formals),
                           toOwned(fp.returnType),
                           toOwned(fp.where),
                           this->consumeList(fp.lifetime),
                           std::move(body));
  builder->noteDeclNameLocation(f.get(), identNameLoc);
  ret = f.release();
  return ret;
}

AstNode*
ParserContext::buildFormal(YYLTYPE location,
                           YYLTYPE locName,
                           YYLTYPE locIntent,
                           MaybeIntent intent,
                           PODUniqueString name,
                           AstNode* typeExpr,
                           AstNode* initExpr,
                           bool consumeAttributeGroup) {
  auto attr = consumeAttributeGroup ? buildAttributeGroup(location) : nullptr;
  auto formalIntent = Formal::Intent::DEFAULT_INTENT;
  if (!intent.isValid) {
    syntax(locIntent, "'%s' intent is not supported on a formal", qualifierToString((Qualifier)intent.intent));
  } else {
    formalIntent = (Formal::Intent)intent.intent;
  }
  auto loc = convertLocation(location);
  auto node = Formal::build(builder, loc, std::move(attr), name,
                            formalIntent,
                            toOwned(typeExpr),
                            toOwned(initExpr));
  builder->noteDeclNameLocation(node.get(), convertLocation(locName));
  this->noteIsBuildingFormal(false);
  if (consumeAttributeGroup) resetAttributeGroupPartsState();
  return node.release();
}

AstNode*
ParserContext::buildVarArgFormal(YYLTYPE location,
                                 YYLTYPE locIntent,
                                 MaybeIntent intent,
                                 PODUniqueString name,
                                 YYLTYPE nameLocation,
                                 AstNode* typeExpr,
                                 AstNode* initExpr,
                                 bool consumeAttributeGroup) {
  auto formalIntent = Formal::Intent::DEFAULT_INTENT;
  if (!intent.isValid) {
    syntax(locIntent, "'%s' intent is not supported on a formal", qualifierToString((Qualifier)intent.intent));
  } else {
    formalIntent = (Formal::Intent)intent.intent;
  }
  auto attr = consumeAttributeGroup ? buildAttributeGroup(location) : nullptr;
  auto loc = convertLocation(location);
  auto node = VarArgFormal::build(builder, loc, std::move(attr), name,
                                  formalIntent,
                                  toOwned(typeExpr),
                                  toOwned(initExpr));
  this->noteIsBuildingFormal(false);
  builder->noteDeclNameLocation(node.get(), convertLocation(nameLocation));
  if (consumeAttributeGroup) resetAttributeGroupPartsState();
  return node.release();
}

AstNode*
ParserContext::buildTupleFormal(YYLTYPE location,
                                YYLTYPE locIntent,
                                MaybeIntent intent,
                                ParserExprList* components,
                                AstNode* typeExpr,
                                AstNode* initExpr) {
  auto formalIntent = Formal::Intent::DEFAULT_INTENT;
  if (!intent.isValid) {
    syntax(locIntent, "'%s' intent is not supported on a formal", qualifierToString((Qualifier)intent.intent));
  } else {
    formalIntent = (Formal::Intent)intent.intent;
  }
  auto loc = convertLocation(location);
  auto node = TupleDecl::build(builder, loc, nullptr,
                               this->visibility,
                               this->linkage,
                               (TupleDecl::IntentOrKind)formalIntent,
                               this->consumeList(components),
                               toOwned(typeExpr),
                               toOwned(initExpr));
  this->noteIsBuildingFormal(false);
  return node.release();
}

AstNode*
ParserContext::buildAnonFormal(YYLTYPE location, YYLTYPE locIntent,
                               MaybeIntent intent,
                               AstNode* formalType) {
  auto formalIntent = Formal::Intent::DEFAULT_INTENT;
  if (!intent.isValid) {
    syntax(locIntent, "'%s' intent is not supported on a formal", qualifierToString((Qualifier)intent.intent));
  } else {
    formalIntent = (Formal::Intent)intent.intent;
  }
  auto loc = convertLocation(location);
  auto node = AnonFormal::build(builder,
                                loc,
                                formalIntent,
                                toOwned(formalType));
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

Formal*
ParserContext::buildThisFormal(YYLTYPE location,
                               YYLTYPE locIntent,
                               MaybeIntent intent,
                               AstNode* typeExpression,
                               AstNode* initExpression) {

  // its simpler to report the error and then return a default rather than error
  Formal::Intent formalIntent = Formal::DEFAULT_INTENT;
  if (!intent.isValid) {
    syntax(locIntent, "'%s' intent is not supported for method receiver", qualifierToString((Qualifier)intent.intent));
  } else {
    formalIntent = (Formal::Intent)intent.intent;
  }

  auto loc = convertLocation(location);
  auto ths = UniqueString::get(context(), "this");

  auto node = Formal::build(builder, loc, /*attributeGroup*/ nullptr,
                            ths,
                            formalIntent,
                            toOwned(typeExpression),
                            toOwned(initExpression));
  return node.release();
}

AstNode*
ParserContext::consumeFormalToAnonFormal(AstNode* ast) {
  auto formal = ast->toFormal();

  CHPL_ASSERT(formal);
  CHPL_ASSERT(!formal->initExpression() && !formal->typeExpression());
  CHPL_ASSERT(!formal->name().isEmpty());
  CHPL_ASSERT(!formal->attributeGroup());
  CHPL_ASSERT(formal->visibility() == Decl::DEFAULT_VISIBILITY);

  auto loc = builder->getLocation(formal);
  CHPL_ASSERT(!loc.isEmpty());

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
  auto returnIntent = (FunctionSignature::ReturnIntent)
                        checkReturnIntent(this, fp.returnIntentLoc, fp.returnIntent);
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
  CHPL_ASSERT(!fp.errorExpr);

  bool parenless = (fp.formals == parenlessMarker);
  if (parenless) fp.formals = nullptr; // Don't free the marker.

  // Detect primary methods and create a receiver for them
  bool primaryMethod = false;
  if (currentScopeIsAggregate()) {
    if (fp.receiver == nullptr) {
      fp.receiver = buildThisFormal(fp.thisIntentLoc, fp.thisIntentLoc,
                                    fp.thisIntent, nullptr, nullptr);
      primaryMethod = true;
    }
  }

  owned<Block> body;
  if (fp.body != nullptr) {
    body = consumeToBlock(location, fp.body);
  }

  // Own the recorded identifier to clean it up, but grab its location.
  owned<Identifier> identName = toOwned(fp.name);
  CHPL_ASSERT(identName.get());
  auto identNameLoc = builder->getLocation(identName.get());
  CHPL_ASSERT(!identNameLoc.isEmpty());

  auto returnIntent = checkReturnIntent(this, fp.returnIntentLoc, fp.returnIntent);

  auto f = Function::build(builder, convertLocation(location),
                           toOwned(fp.attributeGroup),
                           fp.visibility,
                           fp.linkage,
                           toOwned(fp.linkageNameExpr),
                           identName->name(),
                           fp.isInline,
                           fp.isOverride,
                           fp.kind,
                           toOwned(fp.receiver),
                           returnIntent,
                           fp.throws,
                           primaryMethod,
                           parenless,
                           this->consumeList(fp.formals),
                           toOwned(fp.returnType),
                           toOwned(fp.where),
                           this->consumeList(fp.lifetime),
                           std::move(body));

  builder->noteDeclNameLocation(f.get(), identNameLoc);
  builder->noteDeclHeaderLocation(f.get(), convertLocation(fp.headerLoc));
  builder->noteCurlyBracesLocation(f.get(), convertLocation(fp.curlyLoc));

  // If we are not a method then the receiver intent is discarded,
  // because there is no receiver formal to store it in.
  // So do the check now.
  // TODO: I think we should redundantly store the receiver intent
  // in the function as well as the receiver formal.
  if (!f->isMethod() &&
      (Formal::Intent)fp.thisIntent.intent != Formal::DEFAULT_INTENT) {
    if (fp.thisIntent.isValid && (Formal::Intent)fp.thisIntent.intent == Formal::TYPE) {
      error(location, "missing type for secondary type method '%s'.",
            identName->name().c_str());
    } else {
      error(location,
            "'this' intents can only be applied to methods, "
            "but '%s' is not a method",
            identName->name().c_str());
    }
  }

  auto last = makeLocationAtLast(location);
  auto commentsToDiscard = gatherComments(last);
  clearComments(commentsToDiscard);

  cs.stmt = f.release();
  return cs;
}

ErroneousExpression* ParserContext::checkForFunctionErrors(FunctionParts& fp,
                                                           AstNode* retType) {
  if (fp.errorExpr != nullptr) {
    return fp.errorExpr;
  }
  if (retType != nullptr && retType->isErroneousExpression()) {
    return retType->toErroneousExpression();
  }
  return nullptr;
}
void ParserContext::enterScopeForFunctionDecl(FunctionParts& fp,
                                              AstNode* retType) {
  this->clearComments();
  this->resetDeclState();

  fp.errorExpr = checkForFunctionErrors(fp, retType);
  // May never have been built if there was a syntax error.
  if (!fp.errorExpr) {
    this->enterScope(asttags::Function, fp.name->name());
  }
}
void ParserContext::exitScopeForFunctionDecl(YYLTYPE bodyLocation,
                                             FunctionParts& fp) {
  auto last = makeLocationAtLast(bodyLocation);
  auto commentsToDiscard = gatherComments(last);
  clearComments(commentsToDiscard);

  fp.errorExpr = checkForFunctionErrors(fp, fp.returnType);
  // May never have been built if there was a syntax error.
  if (!fp.errorExpr) {
    this->exitScope(asttags::Function, fp.name->name());
  }
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

AstNode* ParserContext::sanitizeArrayType(YYLTYPE location, AstNode* arr) {
  if (!arr->isArray()) return arr;

  auto domainBody = std::move(builder->mutableRefToChildren(arr));
  auto domain = Domain::build(builder, convertLocation(location),
                              /*usedCurlyBraces*/ false,
                              std::move(domainBody));
  auto block = consumeToBlock(location, (AstNode*) nullptr);

  auto ast = BracketLoop::build(builder, convertLocation(location),
                                /*index*/ nullptr,
                                std::move(domain),
                                /*withClause*/ nullptr,
                                BlockStyle::IMPLICIT,
                                std::move(block),
                                /*isExpressionLevel*/ true);
  return ast.release();
}

// .chpl: '[]'
AstNode* ParserContext::buildBracketLoopExpr(YYLTYPE location) {

  // In some cases the 'domainExprs' may not exist (think array formal).
  auto domainBody = AstList();

  const bool usedCurlyBraces = false;
  auto domain = Domain::build(builder, convertLocation(location),
                              usedCurlyBraces,
                              std::move(domainBody));

  // Block is empty since we are just '[]'.
  auto block = consumeToBlock(location, (AstNode*) nullptr);

  auto ast = BracketLoop::build(builder, convertLocation(location),
                                /*index*/ nullptr,
                                std::move(domain),
                                /*withClause*/ nullptr,
                                BlockStyle::IMPLICIT,
                                std::move(block),
                                /*isExpressionLevel*/ true);
  return ast.release();
}

// .chpl: '[] int'
AstNode* ParserContext::buildBracketLoopExpr(YYLTYPE location,
                                             YYLTYPE locRightBracket,
                                             AstNode* bodyExpr) {

  // In some cases the 'domainExprs' may not exist (think array formal).
  auto domainBody = AstList();
  auto locDomain = makeSpannedLocation(location, locRightBracket);

  const bool usedCurlyBraces = false;
  auto domain = Domain::build(builder, convertLocation(locDomain),
                              usedCurlyBraces,
                              std::move(domainBody));

  auto block = consumeToBlock(location, bodyExpr);

  auto ast = BracketLoop::build(builder, convertLocation(location),
                                /*index*/ nullptr,
                                std::move(domain),
                                /*withClause*/ nullptr,
                                BlockStyle::IMPLICIT,
                                std::move(block),
                                /*isExpressionLevel*/ true);
  return ast.release();
}

// .chpl: '[i in 1..100] i' (multiple indices is an error in this case)
AstNode* ParserContext::buildBracketLoopExpr(YYLTYPE location,
                                             YYLTYPE locIndexExprs,
                                             ParserExprList* indexExprs,
                                             AstNode* iterandExpr,
                                             AstNode* bodyExpr) {
  auto index = buildLoopIndexDecl(locIndexExprs, indexExprs);
  auto locBodyExpr = locationFromChplLocation(bodyExpr);

  auto ast = BracketLoop::build(builder, convertLocation(location),
                                 std::move(index),
                                 toOwned(iterandExpr),
                                 /*withClause*/ nullptr,
                                 BlockStyle::IMPLICIT,
                                 consumeToBlock(locBodyExpr, bodyExpr),
                                 /*isExpressionLevel*/ true);
  return ast.release();
}

// .chpl: '[i in 1..100] if i % 2 then i'
AstNode* ParserContext::buildBracketLoopExpr(YYLTYPE location,
                                             YYLTYPE locIndexExprs,
                                             YYLTYPE locIf,
                                             ParserExprList* indexExprs,
                                             AstNode* iterandExpr,
                                             AstNode* bodyIfCond,
                                             AstNode* bodyIfExpr) {

  auto index = buildLoopIndexDecl(locIndexExprs, indexExprs);
  auto locBodyIfExpr = locationFromChplLocation(bodyIfExpr);
  auto locCond = makeSpannedLocation(locIf, locBodyIfExpr);

  auto cond = Conditional::build(builder, convertLocation(locCond),
                                 toOwned(bodyIfCond),
                                 BlockStyle::IMPLICIT,
                                 consumeToBlock(locBodyIfExpr, bodyIfExpr),
                                 /*isExpressionLevel*/ true);

  auto ast = BracketLoop::build(builder, convertLocation(location),
                                std::move(index),
                                toOwned(iterandExpr),
                                /*withClause*/ nullptr,
                                BlockStyle::IMPLICIT,
                                consumeToBlock(locCond, cond.release()),
                                /*isExpressionLevel*/ true);
  return ast.release();
}

// .chpl: '[a, b, c] int' || '[0..100] doSomething()'
AstNode* ParserContext::buildBracketLoopExpr(YYLTYPE location,
                                             YYLTYPE locIterandExprs,
                                             ParserExprList* iterandExprs,
                                             AstNode* bodyExpr) {
  owned<AstNode> iterand = nullptr;
  auto iterExprs = consumeList(iterandExprs);
  if (iterExprs.size() > 1) {
    iterand = Domain::build(builder, convertLocation(locIterandExprs),
                            /*usedCurlyBraces*/ false,
                            std::move(iterExprs));
  } else {
    iterand = std::move(iterExprs[0]);
  }

  CHPL_ASSERT(iterand != nullptr);

  // Go from 'chpl::Location' to 'YYLTYPE' for 'consumeToBlock'.
  auto locBodyExpr = locationFromChplLocation(bodyExpr);
  auto bodyBlock = consumeToBlock(locBodyExpr, bodyExpr);

  auto ast = BracketLoop::build(builder, convertLocation(location),
                                /*index*/ nullptr,
                                std::move(iterand),
                                /*withClause*/ nullptr,
                                BlockStyle::IMPLICIT,
                                std::move(bodyBlock),
                                /*isExpressionLevel*/ true);
  return ast.release();
}

AstNode* ParserContext::
buildTupleComponent(YYLTYPE location, PODUniqueString name) {
  AstNode* ret = nullptr;

  if (isBuildingFormal) {
    auto node = Formal::build(builder, convertLocation(location),
                              this->buildAttributeGroup(location),
                              name,
                              Formal::DEFAULT_INTENT,
                              /*typeExpression*/ nullptr,
                              /*initExpression*/ nullptr);
    ret = node.release();
  } else {
    auto loc = convertLocation(location);
    auto node = Variable::build(builder, loc, loc,
                                /*attributeGroup*/ nullptr,
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

  CHPL_ASSERT(ret != nullptr);

  return ret;
}

AstNode* ParserContext::
buildTupleComponent(YYLTYPE location, ParserExprList* exprs) {
  auto node = TupleDecl::build(builder, convertLocation(location),
                               /*attributeGroup*/ nullptr,
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
    auto var = Variable::build(builder, convLoc, convLoc,
                           /*attributeGroup*/ nullptr,
                           Decl::DEFAULT_VISIBILITY,
                           Decl::DEFAULT_LINKAGE,
                           /*linkageName*/ nullptr,
                           ident->name(),
                           Variable::INDEX,
                           /*isConfig*/ false,
                           /*isField*/ false,
                           /*typeExpression*/ nullptr,
                           /*initExpression*/ nullptr);
    builder->copyExprParenLocation(e, var.get());
    builder->deleteAllLocations(e);
    // Delete the location of 'e' because it's about to be deallocated;
    // we don't want a new allocation of an AST node to have the same pointer
    // and still be in the map, since that would pollute location information.
    //
    // This is more of a workaround; a more general solution is necessary
    // to handle pointer-based maps in the presence of AST node deallocation.
    return var;

  } else if (const uast::Tuple* tup = e->toTuple()) {
    AstList elements;
    for (auto expr : tup->actuals()) {
      auto decl = buildLoopIndexDecl(locationFromChplLocation(expr), expr);
      if (decl.get() != nullptr) {
        CHPL_ASSERT(decl->isDecl());
        elements.push_back(std::move(decl));
      } else {
        auto err = ErroneousExpression::build(builder, convLoc);
        elements.push_back(std::move(err));
      }
    }

    auto td = TupleDecl::build(builder, convLoc, /*attributeGroup*/ nullptr,
                               Decl::DEFAULT_VISIBILITY,
                               Decl::DEFAULT_LINKAGE,
                               (TupleDecl::IntentOrKind) Variable::INDEX,
                               std::move(elements),
                               /*typeExpression*/ nullptr,
                               /*initExpression*/ nullptr);
    builder->copyExprParenLocation(e, td.get());
    builder->deleteExprParenLocation(e);
    // See the comment above for why we delete the location of 'e'.
    return td;
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
    CHPL_ASSERT(indexExpr);
    return buildLoopIndexDecl(location, toOwned(indexExpr));
  }
}

AstNode* ParserContext::buildNewExpr(YYLTYPE location,
                                     New::Management management,
                                     AstNode* expr) {
  if (FnCall* fnCall = expr->toFnCall()) {
    return this->wrapCalledExpressionInNew(location, management, fnCall);
  } else if (OpCall* opCall = expr->toOpCall()) {
    CHPL_ASSERT(opCall->numActuals() == 1);
    auto& child = builder->mutableRefToChildren(opCall)[0];
    if (FnCall* fnCall = child->toFnCall()) {
      child.release();
      auto wrappedFn = wrapCalledExpressionInNew(location, management,
                                                 fnCall);
      child.reset(wrappedFn);
      return expr;
    } else {
      CHPL_PARSER_REPORT(this, InvalidNewForm, location, expr);
    }
  } else {
    if (expr->isIdentifier() && expr->toIdentifier()->numChildren() == 0) {
      // try to capture case of new A; (new without parens)
      CHPL_PARSER_REPORT(this, NewWithoutArgs, location, expr);
    } else if (expr->isDot() ) {
      // try to capture case of var z20a = new C().tmeth;
      // and var z20c = new C()?.tmeth;
      if (expr->toDot()->receiver()->isFnCall() ||
          expr->toDot()->receiver()->isOpCall()) {
        syntax(location,
            "must use parentheses to disambiguate dot expression after 'new'.");
      } else {
        // try to capture case of new M.Q;
        CHPL_PARSER_REPORT(this, NewWithoutArgs, location, expr);
      }
    } else {
      // It's an error for one reason or another. TODO: Specialize these
      // errors later (e.g. 'new a.field' would require parens around
      // the expression 'a.field'; 'new foo' would require an argument
      // list for 'foo'; and something like 'new __primitive()' just
      // doesn't make any sense...
      CHPL_PARSER_REPORT(this, InvalidNewForm, location, expr);
    }
  }
  auto loc = convertLocation(location);
  return ErroneousExpression::build(builder, loc).release();
}

FnCall* ParserContext::wrapCalledExpressionInNew(YYLTYPE location,
                                                 New::Management management,
                                                 FnCall* fnCall) {
  CHPL_ASSERT(fnCall->calledExpression());
  bool wrappedBaseExpression = false;

  // Find the child slot containing the called expression. Then remove it,
  // wrap it in a new expression, and swap in the new expression.
  for (auto& child : builder->mutableRefToChildren(fnCall)) {
    if (child.get() != fnCall->calledExpression()) continue;

    // We have the base expression.
    auto calledExpr = std::move(child).release();
    CHPL_ASSERT(calledExpr);
    auto newExpr = New::build(builder, convertLocation(location),
                              toOwned(calledExpr),
                              management);
    builder->copyExprParenLocation(fnCall, newExpr.get());
    builder->deleteExprParenLocation(fnCall);
    child = std::move(newExpr);

    // Extend the location of the call to the left and patch the location.
    auto fnCallLoc = builder->getLocation(fnCall);
    CHPL_ASSERT(!fnCallLoc.isEmpty());

    auto updatedLoc = Location(fnCallLoc.path(), location.first_line,
                               location.first_column,
                               fnCallLoc.lastLine(),
                               fnCallLoc.lastColumn());

    builder->noteLocation(fnCall, std::move(updatedLoc));
    wrappedBaseExpression = true;
    break;
  }

  CHPL_ASSERT(wrappedBaseExpression);

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
ParserContext::buildBracketLoopStmt(YYLTYPE locLoop,
                                    YYLTYPE locHeader,
                                    YYLTYPE locIndex,
                                    YYLTYPE locBodyAnchor,
                                    ParserExprList* indexExprs,
                                    AstNode* iterandExpr,
                                    WithClause* withClause,
                                    CommentsAndStmt cs) {
  // Should not be nullptr, use other overload instead.
  CHPL_ASSERT(indexExprs && indexExprs->size() >= 1);

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locLoop,
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

  CHPL_ASSERT(indexExpr);
  auto index = buildLoopIndexDecl(locIndex, toOwned(indexExpr));

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = BracketLoop::build(builder, convertLocation(locLoop),
                                 std::move(index),
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(body),
                                 /*isExpressionLevel*/ false,
                                 this->popLoopAttributeGroup());
  builder->noteLoopHeaderLocation(node.get(), convertLocation(locHeader));
  if (isValidCurlyBracesLoc(cs.curlyLoc)) {
    builder->noteCurlyBracesLocation(node.get(), convertLocation(cs.curlyLoc));
  }
  return makeCommentsAndStmt(comments, node.release());
}

CommentsAndStmt ParserContext::buildBracketLoopStmt(YYLTYPE locLoop,
                                                    YYLTYPE locHeader,
                                                    YYLTYPE locIterExprs,
                                                    YYLTYPE locBodyAnchor,
                                                    ParserExprList* iterExprs,
                                                    WithClause* withClause,
                                                    CommentsAndStmt cs) {
  CHPL_ASSERT(iterExprs && iterExprs->size() >= 1);

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locLoop,
                    false,
                    locBodyAnchor,
                    cs);

  AstNode* iterandExpr = nullptr;

  if (iterExprs->size() > 1) {
    return {.comments = comments,
            .stmt = error(locIterExprs, "invalid iterand expression.")};
  } else {
    auto uncastedIterandExpr = consumeList(iterExprs)[0].release();
    iterandExpr = uncastedIterandExpr;
  }

  CHPL_ASSERT(iterandExpr);

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  auto node = BracketLoop::build(builder, convertLocation(locLoop),
                                 /*index*/ nullptr,
                                 toOwned(iterandExpr),
                                 toOwned(withClause),
                                 blockStyle,
                                 std::move(body),
                                 /*isExpressionLevel*/ false,
                                 this->popLoopAttributeGroup());
  builder->noteLoopHeaderLocation(node.get(), convertLocation(locHeader));
  if (isValidCurlyBracesLoc(cs.curlyLoc)) {
    builder->noteCurlyBracesLocation(node.get(), convertLocation(cs.curlyLoc));
  }
  return makeCommentsAndStmt(comments, node.release());
}

CommentsAndStmt ParserContext::buildGeneralLoopStmt(YYLTYPE locLoop,
                                                    YYLTYPE locIndex,
                                                    YYLTYPE locHeader,
                                                    YYLTYPE locBodyAnchor,
                                                    PODUniqueString loopType,
                                                    AstNode* indexExpr,
                                                    AstNode* iterandExpr,
                                                    WithClause* withClause,
                                                    BlockOrDo blockOrDo) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;

  std::vector<ParserComment>* comments;
  ParserExprList* exprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(comments, exprLst, blockStyle, locLoop,
                    locBodyAnchor,
                    blockOrDo);

  auto body = consumeToBlock(locBodyAnchor, exprLst);

  // UniqueString supports '==' on interned strings.
  UniqueString loopTypeUstr = loopType;
  IndexableLoop* result = nullptr;
  ErroneousExpression* error = nullptr;
  if (loopTypeUstr == USTR("for")) {
    if (withClause) {
      error = syntax(locLoop,
                     "'with' clauses are not supported on 'for' loops.");

      // Since these are about to get deallocated, clear their location
      // maps to ensure the OS re-using memory won't bring in stale locations
      builder->deleteAllLocations(index.get());
      builder->deleteAllLocations(body.get());
    } else {
      result = For::build(builder, convertLocation(locLoop),
                          std::move(index),
                          toOwned(iterandExpr),
                          blockStyle,
                          std::move(body),
                          /*isExpressionLevel*/ false,
                          /*isParam*/ false,
                          this->popLoopAttributeGroup()).release();
    }
  } else if (loopTypeUstr == USTR("foreach")) {
    result = Foreach::build(builder, convertLocation(locLoop),
                            std::move(index),
                            toOwned(iterandExpr),
                            toOwned(withClause),
                            blockStyle,
                            std::move(body),
                            /* isExpressionLevel */ false,
                            this->popLoopAttributeGroup()).release();
  } else if (loopTypeUstr == USTR("forall")) {
    result = Forall::build(builder, convertLocation(locLoop),
                           std::move(index),
                           toOwned(iterandExpr),
                           toOwned(withClause),
                           blockStyle,
                           std::move(body),
                           /*isExpressionLevel*/ false,
                           this->popLoopAttributeGroup()).release();
  } else if (loopTypeUstr == USTR("coforall")) {
    result = Coforall::build(builder, convertLocation(locLoop),
                             std::move(index),
                             toOwned(iterandExpr),
                             toOwned(withClause),
                             blockStyle,
                             std::move(body),
                             this->popLoopAttributeGroup()).release();
  } else {
    CHPL_ASSERT(false); // unhandled loop stmt
  }

  if (error) {
    CHPL_ASSERT(!result);
    return makeCommentsAndStmt(comments, error);
  } else {
    CHPL_ASSERT(result);
    builder->noteLoopHeaderLocation(result, convertLocation(locHeader));
    if (isValidCurlyBracesLoc(blockOrDo.cs.curlyLoc)) {
      builder->noteCurlyBracesLocation(result, convertLocation(blockOrDo.cs.curlyLoc));
    }
    return makeCommentsAndStmt(comments, result);
  }
}

AstNode* ParserContext::buildGeneralLoopExpr(YYLTYPE locWhole,
                                                   YYLTYPE locIndex,
                                                   YYLTYPE locBodyAnchor,
                                                   PODUniqueString loopType,
                                                   AstNode* indexExpr,
                                                   AstNode* iterandExpr,
                                                   AstNode* bodyExpr) {
  auto index = indexExpr ? buildLoopIndexDecl(locIndex, toOwned(indexExpr))
                         : nullptr;
  auto body = consumeToBlock(locBodyAnchor, bodyExpr);

  UniqueString loopTypeUstr = loopType;
  IndexableLoop* result = nullptr;
  ErroneousExpression* error = nullptr;

  if (loopTypeUstr == USTR("for")) {
    result = For::build(builder, convertLocation(locWhole),
                        std::move(index), toOwned(iterandExpr),
                        BlockStyle::IMPLICIT,
                        std::move(body),
                        /*isExpressionLevel*/ true,
                        /*isParam*/ false,
                        this->popLoopAttributeGroup()).release();
  } else if (loopTypeUstr == USTR("foreach")) {
    result = Foreach::build(builder, convertLocation(locWhole),
                            std::move(index), toOwned(iterandExpr),
                           /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            std::move(body),
                            /*isExpressionLevel*/ true,
                            this->popLoopAttributeGroup()).release();
  } else if (loopTypeUstr == USTR("forall")) {
    result = Forall::build(builder, convertLocation(locWhole),
                           std::move(index), toOwned(iterandExpr),
                           /*withClause*/ nullptr,
                           BlockStyle::IMPLICIT,
                           std::move(body),
                           /*isExpressionLevel*/ true,
                           this->popLoopAttributeGroup()).release();
  } else if (loopTypeUstr == USTR("coforall")) {
    error = syntax(locWhole, "expression-level 'coforall' loops are not supported.");
  } else {
    CHPL_ASSERT(false); // unhandled loop expr
  }

  if (error) return error;
  return result;
}

CommentsAndStmt ParserContext::buildCoforallLoopStmt(YYLTYPE locCoforall,
                                                     YYLTYPE locHeader,
                                                     YYLTYPE locIndex,
                                                     YYLTYPE locBodyAnchor,
                                                     AstNode* indexExpr,
                                                     AstNode* iterandExpr,
                                                     WithClause* withClause,
                                                     BlockOrDo blockOrDo) {
                                                    //  AttributeGroup* attributeGroup) {
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
                              std::move(body),
                              this->popLoopAttributeGroup());
  builder->noteLoopHeaderLocation(node.get(), convertLocation(locHeader));
  if (isValidCurlyBracesLoc(blockOrDo.cs.curlyLoc)) {
    builder->noteCurlyBracesLocation(node.get(), convertLocation(blockOrDo.cs.curlyLoc));
  }

  return makeCommentsAndStmt(comments, node.release());
}

CommentsAndStmt
ParserContext::buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                    YYLTYPE locThenKw,
                                    YYLTYPE locThenBody,
                                    AstNode* condition,
                                    CommentsAndStmt thenCs) {

  std::vector<ParserComment>* comments;
  ParserExprList* thenExprLst;
  BlockStyle thenBlockStyle;

  prepareStmtPieces(comments, thenExprLst, thenBlockStyle, locIf,
                    usesThenKeyword,
                    locThenKw,
                    thenCs);

  auto thenBlock = consumeToBlock(locThenBody, thenExprLst);
  builder->tryNoteCurlyBracesLocation(thenBlock.get(), convertLocation(thenCs.curlyLoc));

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenBlock),
                                 /*isExpressionLevel*/ false);
  if (usesThenKeyword)
    builder->noteThenKeywordLocation(node.get(), convertLocation(locThenKw));

  // Do NOT clear comments here! Due to lookahead we might clear a valid
  // comment that has already been stored.
  return makeCommentsAndStmt(comments, node.release());
}

CommentsAndStmt
ParserContext::buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                    YYLTYPE locThenKw,
                                    YYLTYPE locThenBody,
                                    YYLTYPE locElseKw,
                                    YYLTYPE locElseBody,
                                    AstNode* condition,
                                    CommentsAndStmt thenCs,
                                    CommentsAndStmt elseCs) {

  std::vector<ParserComment>* comments;
  ParserExprList* thenExprLst;
  BlockStyle thenBlockStyle;

  prepareStmtPieces(comments, thenExprLst, thenBlockStyle,
                    locIf,
                    usesThenKeyword,
                    locThenKw,
                    thenCs);

  const bool isElseBodyBlock = elseCs.stmt->isBlock();

  auto elseBlockStyle = isElseBodyBlock ? BlockStyle::EXPLICIT
                                        : BlockStyle::IMPLICIT;

  auto thenBlock = consumeToBlock(locThenBody, thenExprLst);
  builder->tryNoteCurlyBracesLocation(thenBlock.get(), convertLocation(thenCs.curlyLoc));

  // If the else body is a block, discard all comments preceding it.
  if (isElseBodyBlock) {
    clearComments(elseCs.comments);
    elseCs.comments = nullptr;
  }

  auto elseExprLst = makeList(elseCs);

  // If else body is not a block, discard comments preceding the 'else'.
  if (!isElseBodyBlock) discardCommentsFromList(elseExprLst, locElseKw);

  auto elseBlock = consumeToBlock(locElseBody, elseExprLst);
  // adjust the else block location to include the 'else' keyword
  builder->noteLocation(elseBlock.get(), convertLocation(makeSpannedLocation(locElseKw, locElseBody)));
  builder->tryNoteCurlyBracesLocation(elseBlock.get(), convertLocation(elseCs.curlyLoc));

  auto node = Conditional::build(builder, convertLocation(locIf),
                                 toOwned(condition),
                                 thenBlockStyle,
                                 std::move(thenBlock),
                                 elseBlockStyle,
                                 std::move(elseBlock),
                                 /*isExpressionLevel*/ false);
  if (usesThenKeyword)  
    builder->noteThenKeywordLocation(node.get(), convertLocation(locThenKw));
  builder->noteElseKeywordLocation(node.get(), convertLocation(locElseKw));

  return makeCommentsAndStmt(comments, node.release());
}

CommentsAndStmt ParserContext::buildExternBlockStmt(YYLTYPE locEverything,
                                                    SizedStr sizedStr) {
  auto comments = gatherComments(locEverything);
  auto code = std::string(sizedStr.allocatedData, sizedStr.size);
  auto node = ExternBlock::build(builder, convertLocation(locEverything),
                                 std::move(code));

  // This was allocated in 'eatExternCode', see 'lexer-help.h'.
  free((void*) sizedStr.allocatedData);

  return makeCommentsAndStmt(comments, node.release());
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
      ret =
          CHPL_PARSER_REPORT(this, InvalidNumericLiteral, location, err + ".");
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
      ret =
          CHPL_PARSER_REPORT(this, InvalidNumericLiteral, location, err + ".");
    } else if (type == IMAGLITERAL) {
      ret = ImagLiteral::build(builder, loc, num, text).release();
    } else {
      ret = RealLiteral::build(builder, loc, num, text).release();
    }

  } else {
    CHPL_ASSERT(false && "Case not handled in buildNumericLiteral");
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
        auto limitationLoc = builder->getLocation(expr.get());
        error = CHPL_PARSER_REPORT(this, ExceptOnlyInvalidExpr, location,
                                   limitationLoc, limitationKind);
      }
    } else if (limitationKind == VisibilityClause::LimitationKind::BRACES) {
      CHPL_ASSERT(isImport && "braces should only be used with import statements");
      if (asExpr) {
        if (!asExpr->symbol()->isIdentifier() ||
            !asExpr->rename()->isIdentifier()) {
          error = syntax(location,
                         "incorrect expression in 'import' "
                         "list rename, identifier expected.");
        }
      } else {
        error = syntax(location,
            "incorrect expression in 'import' for unqualified access, "
            "identifier expected.");
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
buildForwardingDecl(YYLTYPE location, owned<AttributeGroup> attributeGroup,
                    owned<AstNode> expr,
                    VisibilityClause::LimitationKind limitationKind,
                    ParserExprList* limitations) {

  auto comments = gatherComments(location);
  if (attributeGroup && attributeGroup->isDeprecated()) {
    error(location, "can't deprecate a forwarding statement.");
  }
  if (limitationKind == VisibilityClause::NONE) {
    auto node = ForwardingDecl::build(builder, convertLocation(location),
                                      std::move(attributeGroup),
                                      std::move(expr));

    return makeCommentsAndStmt(comments, node.release());
  }

  auto limitationsList = limitations ? consumeList(limitations) : AstList();

  auto visClause = buildVisibilityClause(location, std::move(expr),
                                         limitationKind,
                                         std::move(limitationsList), false);

  auto node = ForwardingDecl::build(builder, convertLocation(location),
                                    std::move(attributeGroup),
                                    toOwned(visClause));

  return makeCommentsAndStmt(comments, node.release());
}


CommentsAndStmt ParserContext::
buildForwardingDecl(YYLTYPE location,
                    owned<AttributeGroup> attributeGroup,
                    CommentsAndStmt cs) {
  CHPL_ASSERT(cs.stmt->isVariable() || cs.stmt->isMultiDecl() || cs.stmt->isTupleDecl());
  auto decl = cs.stmt->toDecl();
  CHPL_ASSERT(decl);
  CHPL_ASSERT(!decl->attributeGroup());
  // TODO: pattern for composing comments should be extracted to helper
  auto commentExprs = appendList(makeList(), cs.comments);
  auto comments = gatherCommentsFromList(commentExprs, location);
  delete commentExprs;

  auto node = ForwardingDecl::build(builder, convertLocation(location),
                                    std::move(attributeGroup),
                                    toOwned(cs.stmt),
                                    decl->visibility());

  return makeCommentsAndStmt(comments, node.release());
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
      return makeCommentsAndStmt(comments, node.release());
    }
  }

  auto node = Import::build(builder, convLoc, visibility, std::move(vcs));

  CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());

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
      return makeCommentsAndStmt(comments, node.release());
    }
  }

  // TODO: Make sure that all the vis clauses are correct for multi-use?
  auto node = Use::build(builder, convLoc, visibility, std::move(vcs));

  CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());

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
    return makeCommentsAndStmt(comments, node.release());
  }

  AstList uses;
  uses.push_back(toOwned(visClause));

  auto node = Use::build(builder, convertLocation(locEverything),
                         visibility,
                         std::move(uses));

  this->visibility = Decl::DEFAULT_VISIBILITY;

  CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());

  return finishStmt(cs);
}

// Given a list of vars, build either a single var or a multi-decl.
CommentsAndStmt
ParserContext::buildVarOrMultiDeclStmt(YYLTYPE locEverything,
                                       AttributeGroup* attributeGroupPtr,
                                       ParserExprList* vars) {
  auto attributeGroup = toOwned(attributeGroupPtr);

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

  CHPL_ASSERT(numDecls > 0);
  CHPL_ASSERT(firstDecl && lastDecl);

  auto comments = gatherCommentsFromList(vars, locEverything);
  CommentsAndStmt cs = makeCommentsAndStmt(comments, nullptr);

  if (numDecls == 1) {
    cs.stmt = lastDecl;

    // Delete any remaining comments and then the expr list.
    for (auto elt : *vars) {
      if (elt->isComment()) delete elt;
    }
    delete vars;
    // for single element decls, we attach the attribute group to the decl
    // *note that this places the AttributeGroup as the LAST child of the decl
    if (attributeGroup) {
      lastDecl->attachAttributeGroup(std::move(attributeGroup));
    }
    auto destination = consumeVarDestinationExpr();
    if (destination) {
      if (auto var = lastDecl->toVariable()) {
        var->attachDestination(std::move(destination));
      } else {
        error(locEverything, "only (multi)variable declarations can target a specific locale");
      }
    }
    // fixup the existing decl location for configs
    auto loc = !this->isVarDeclConfig ? locEverything :
                                        makeSpannedLocation(this->configLoc, locEverything);
    builder->noteLocation(lastDecl, convertLocation(loc));
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
    // adjust the decl location for configs before building MultiDecl
    auto loc = !this->isVarDeclConfig ? locEverything :
                                        makeSpannedLocation(this->configLoc, locEverything);
    auto multi = MultiDecl::build(builder,
                                  convertLocation(loc),
                                  std::move(attributeGroup),
                                  visibility,
                                  linkage,
                                  consumeList(vars));
    auto destination = consumeVarDestinationExpr();
    if (destination) {
      multi->attachDestination(std::move(destination));
    }
    cs.stmt = multi.release();
  }

  CHPL_ASSERT(cs.stmt);

  resetDeclState();

  return cs;
}

TypeDeclParts
ParserContext::enterScopeAndBuildTypeDeclParts(YYLTYPE locStart,
                                               YYLTYPE locName,
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
    .attributeGroup=this->buildAttributeGroup(locStart).release(),
    .name=name,
    .tag=tag,
    .locName=locName
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
    CHPL_ASSERT(parts.linkage != Decl::DEFAULT_LINKAGE);
    error(location, "cannot declare class types as export or extern.");

    // Clear the linkage state for this so that parsing can continue.
    clearTypeDeclPartsLinkage(parts);
  }

  if (parts.tag == asttags::Union && parts.linkage == Decl::EXPORT) {
    error(location, "cannot export union types.");

    // Clear the linkage state for this so that parsing can continue.
    clearTypeDeclPartsLinkage(parts);
  }
}

CommentsAndStmt
ParserContext::buildAggregateTypeDecl(YYLTYPE location,
                                      YYLTYPE headerLoc,
                                      TypeDeclParts parts,
                                      YYLTYPE inheritLoc,
                                      ParserExprList* optInherit,
                                      YYLTYPE openingBrace,
                                      ParserExprList* contents,
                                      YYLTYPE closingBrace) {

  if (parts.linkage != Decl::DEFAULT_LINKAGE) {
    validateExternTypeDeclParts(location, parts);
  } else {
    CHPL_ASSERT(parts.linkageName == nullptr);
  }

  CommentsAndStmt cs = {parts.comments, nullptr};
  // adjust the contents list to have the right comments
  discardCommentsFromList(contents, openingBrace);
  appendList(contents, gatherComments(closingBrace));

  auto contentsList = consumeList(contents);

  AstList inheritExprs;
  if (optInherit != nullptr) {
    if (optInherit->size() > 0) {
      for (size_t i = 0; i < optInherit->size(); i++) {
        AstNode* ast = (*optInherit)[i];

        inheritExprs.push_back(toOwned(ast));
        (*optInherit)[i] = nullptr;
      }
    }
    consumeList(optInherit); // just to delete it
  }

  NamedDecl* decl = nullptr;
  if (parts.tag == asttags::Class) {

    // These should have been cleared when validated above (the Class node
    // constructor and builder function would have to change if we ever
    // wanted to permit export/extern classes.
    CHPL_ASSERT(parts.linkage == Decl::DEFAULT_LINKAGE);
    CHPL_ASSERT(!parts.linkageName);

    decl = Class::build(builder, convertLocation(location),
                        toOwned(parts.attributeGroup),
                        parts.visibility,
                        parts.name,
                        std::move(inheritExprs),
                        std::move(contentsList)).release();
  } else if (parts.tag == asttags::Record) {
    decl = Record::build(builder, convertLocation(location),
                         toOwned(parts.attributeGroup),
                         parts.visibility,
                         parts.linkage,
                         toOwned(parts.linkageName),
                         parts.name,
                         std::move(inheritExprs),
                         std::move(contentsList)).release();
  } else if (parts.tag == asttags::Union) {

    // Should have been cleared because this is not possible right now.
    CHPL_ASSERT(parts.linkage != Decl::EXPORT);

    decl = Union::build(builder, convertLocation(location),
                        toOwned(parts.attributeGroup),
                        parts.visibility,
                        parts.linkage,
                        toOwned(parts.linkageName),
                        parts.name,
                        std::move(inheritExprs),
                        std::move(contentsList)).release();
  } else {
    CHPL_ASSERT(false && "case not handled");
  }

  builder->noteDeclNameLocation(decl, convertLocation(parts.locName));
  builder->noteDeclHeaderLocation(decl, convertLocation(headerLoc));

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
                                          AstNode* iterand,
                                          YYLTYPE iterandLoc) {
  auto ident = buildIdent(locOp, op);
  return buildReduceIntent(location, locOp, ident, iterand, iterandLoc);
}

AstNode* ParserContext::buildReduceIntent(YYLTYPE location,
                                          YYLTYPE locOp,
                                          AstNode* op,
                                          AstNode* iterand,
                                          YYLTYPE iterandLoc) {
  (void) locOp;
  const Identifier* ident = iterand->toIdentifier();
  if (ident == nullptr) {
    return syntax(location, "expected identifier for reduce intent name.");
  }
  auto node = ReduceIntent::build(builder, convertLocation(location),
                                  toOwned(op),
                                  ident->name());
  builder->noteDeclNameLocation(node.get(), convertLocation(iterandLoc));
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
  CHPL_ASSERT(!queriedIdent.isEmpty() && queriedIdent.c_str()[0] == '?');
  CHPL_ASSERT(queriedIdent.length() >= 2);
  const char* adjust = queriedIdent.c_str() + 1;
  auto name = UniqueString::get(context(), adjust);
  auto node = TypeQuery::build(builder, convertLocation(location), name);
  // this is a bit of a hack, since the query is reported as a single token
  auto identLoc = Location(this->filename,
                  location.first_line,
                  location.first_column+1, // skip the '?'
                  location.last_line,
                  location.last_column);
  builder->noteDeclNameLocation(node.get(), identLoc);
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
  CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());
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

  CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());
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

  CHPL_ASSERT(block.stmt != nullptr && block.stmt->isBlock());
  Block* bodyBlock = block.stmt->toBlock();
  builder->tryNoteCurlyBracesLocation(bodyBlock, convertLocation(block.curlyLoc));
  auto catches = consumeList(handlers);
  auto node = Try::build(builder, convertLocation(location),
                         toOwned(bodyBlock),
                         std::move(catches),
                         isTryBang);

  CommentsAndStmt ret = makeCommentsAndStmt(comments, node.release());
  return ret;
}

AstNode* ParserContext::buildCatch(YYLTYPE location, AstNode* error,
                                   CommentsAndStmt cs,
                                   bool hasParensAroundError,
                                   YYLTYPE parenLocation) {
  CHPL_ASSERT(cs.stmt->isBlock());
  if (error != nullptr) CHPL_ASSERT(error->isVariable());

  clearComments(cs.comments);
  cs.comments = nullptr;

  auto errorVar = error ? error->toVariable() : nullptr;

  auto block = cs.stmt->toBlock();
  builder->tryNoteCurlyBracesLocation(block, convertLocation(cs.curlyLoc));

  auto node = Catch::build(builder, convertLocation(location),
                           toOwned(errorVar),
                           toOwned(block),
                           hasParensAroundError);
  if (hasParensAroundError) {
    builder->tryNoteExprParenLocation(errorVar, convertLocation(parenLocation));
  }
  return node.release();
}

CommentsAndStmt
ParserContext::buildWhenStmt(YYLTYPE location,
                             YYLTYPE headerLocation,
                             ParserExprList* caseExprs,
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
  builder->noteBlockHeaderLocation(node.get(), convertLocation(headerLocation));
  if (isValidCurlyBracesLoc(blockOrDo.cs.curlyLoc)) {
    builder->noteCurlyBracesLocation(node.get(), convertLocation(blockOrDo.cs.curlyLoc));
  }

  CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());

  return cs;
}

CommentsAndStmt
ParserContext::buildSelectStmt(YYLTYPE location,
                               YYLTYPE headerLocation,
                               owned<AstNode> expr,
                               ParserExprList* whenStmts) {
  auto comments = gatherCommentsFromList(whenStmts, location);

  // Discard all remaining comments.
  discardCommentsFromList(whenStmts);

  auto stmts = consumeList(whenStmts);

  int numOtherwise = 0;

  // Return an error if there is more than one otherwise.
  for (auto& ast : stmts) {
    auto when = ast->toWhen();
    CHPL_ASSERT(when != nullptr);

    // TODO: Do we also care about the position of the otherwise?
    if (when->isOtherwise() && numOtherwise++) {
      CommentsAndStmt cs = {
          .comments = comments,
          .stmt = syntax(location, "select has multiple otherwise clauses.")};
      return cs;
    }
  }

  auto node = Select::build(builder, convertLocation(location),
                            std::move(expr),
                            std::move(stmts));
  builder->noteBlockHeaderLocation(node.get(), convertLocation(headerLocation));

  CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());

  return cs;
}

AstNode* ParserContext::buildInterfaceFormal(YYLTYPE location,
                                             YYLTYPE locName,
                                             PODUniqueString name) {
  return buildFormal(location, locName, YYLTYPE::create(),
                    makeIntent(Formal::Intent::TYPE), name,
                    /* typeExpr */ nullptr,
                    /* initExpr */ nullptr,
                    /* consumeAttributeGroup */ false);
}

CommentsAndStmt ParserContext::buildInterfaceStmt(YYLTYPE location,
                                                  YYLTYPE headerLoc,
                                                  TypeDeclParts parts,
                                                  ParserExprList* formals,
                                                  YYLTYPE locBody,
                                                  CommentsAndStmt body) {
  // interfaces do not have visibility, linkage, or linkage names
  CHPL_ASSERT(parts.visibility == Decl::DEFAULT_VISIBILITY);
  CHPL_ASSERT(parts.linkage == Decl::DEFAULT_LINKAGE);
  CHPL_ASSERT(parts.linkageName == nullptr);

  std::vector<ParserComment>* bodyComments;
  ParserExprList* bodyExprLst;
  BlockStyle blockStyle;

  prepareStmtPieces(bodyComments, bodyExprLst, blockStyle, location,
                    false,
                    locBody,
                    body);

  AstList formalList;
  if (formals != nullptr) {
    formalList = consumeList(formals);
  } else {
    auto selfStr = PODUniqueString::get(context(), "Self");
    formalList.push_back(toOwned(buildInterfaceFormal(location, {}, selfStr)));
  }

  AstList bodyStmts = bodyExprLst
      ? consumeAndFlattenTopLevelBlocks(bodyExprLst)
      : AstList();
  const bool isFormalListPresent = formals != nullptr;

  auto node = Interface::build(builder, convertLocation(location),
                               toOwned(parts.attributeGroup),
                               visibility,
                               parts.name,
                               isFormalListPresent,
                               std::move(formalList),
                               std::move(bodyStmts));
  builder->noteDeclNameLocation(node.get(), convertLocation(parts.locName));
  builder->noteDeclHeaderLocation(node.get(), convertLocation(headerLoc));


  CommentsAndStmt cs = makeCommentsAndStmt(parts.comments, node.release());

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

  CHPL_ASSERT(ret.get() != nullptr);

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

  CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());

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
  CommentsAndStmt cs = makeCommentsAndStmt(comments, node.release());

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
    CHPL_ASSERT(loop);
    auto node = Label::build(builder, convertLocation(location), name,
                             toOwned(loop));
    return makeCommentsAndStmt(comments, node.release());
  } else {
    return finishStmt(
        CHPL_PARSER_REPORT(this, LabelIneligibleStmt, location, cs.stmt));
  }
}


ParserExprList*
ParserContext::buildSingleStmtRoutineBody(YYLTYPE location, CommentsAndStmt cs) {
  cs = this->finishStmt(location, cs);
  return this->makeList(cs);
}
