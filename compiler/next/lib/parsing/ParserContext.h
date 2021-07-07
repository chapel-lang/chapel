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

// This is included at the right time in the generated bison .h/.cpp
// so that it can depend upon YYLTYPE.
//
// It is split out into this separate file for easier maintenance.

// Headers that this depends upon should be defined in ParserDependencies.h

struct ParserError {
  // Note that this does not include the filename;
  // that should be known by whatever code is parsing something
  // when it goes to report errors.
  //
  // When an error occurs during parsing, the parser should
  // emit errors here and create a stand-in ErroneousExpression AST
  // node.
  YYLTYPE location;
  std::string message;
  ParserError(YYLTYPE location, std::string message)
    : location(location), message(message) { }
  ParserError(YYLTYPE location, const char* message)
    : location(location), message(message) { }
};

struct ParserComment {
  YYLTYPE location;
  Comment* comment;
};

struct ParserContext {
  yyscan_t scanner;
  UniqueString filename;
  Builder* builder;

  ParserExprList* topLevelStatements;
  std::vector<ParserError> errors;

  // TODO: this should just hash on the pointer; the void* is a hack to do that
  std::unordered_map<void*, YYLTYPE> commentLocations;

  // comments are gathered here
  // Consider a 'proc' declaration. Comments preceding it should be consumed
  // when the decl_stmt starts. Then, after the decl_stmt is created, comments
  // accumulated here should be cleared (since they must have come from inside
  // the statement and should not apply later).
  std::vector<ParserComment>* comments;

  // Tracking a current state for these makes it easier to write
  // the parser rules.
  Decl::Visibility visibility;
  Variable::Kind varDeclKind;
  bool isVarDeclConfig;
  YYLTYPE declStartLocation;

  // this type and stack helps the parser know if a function
  // declaration is a method.
  struct ParserScope {
    asttags::ASTTag tag;
    UniqueString name;
  };
  std::vector<ParserScope> scopeStack;

  // note when EOF is reached
  bool atEOF;

  ParserContext(const char* filename, Builder* builder)
  {
    auto uniqueFilename = UniqueString::build(builder->context(), filename);

    this->scanner            = nullptr;
    this->filename           = uniqueFilename;
    this->builder            = builder;
    this->topLevelStatements = nullptr;
    this->comments           = nullptr;
    this->visibility         = Decl::DEFAULT_VISIBILITY;
    this->varDeclKind        = Variable::VAR;
    this->isVarDeclConfig    = false;
    YYLTYPE emptyLoc = {0};
    this->declStartLocation  = emptyLoc;
    this->atEOF              = false;
  }

  Context* context() { return builder->context(); }

  void noteDeclStartLoc(YYLTYPE loc);
  Decl::Visibility noteVisibility(Decl::Visibility visibility);
  Variable::Kind noteVarDeclKind(Variable::Kind varDeclKind);
  bool noteIsVarDeclConfig(bool isConfig);
  YYLTYPE declStartLoc(YYLTYPE curLoc);
  void resetDeclState();

  void enterScope(asttags::ASTTag tag, UniqueString name);
  ParserScope currentScope();
  bool currentScopeIsAggregate();
  void exitScope(asttags::ASTTag tag, UniqueString name);

  // Given a location, create a new one pointing to the end of it.
  YYLTYPE makeLocationAtLast(YYLTYPE location) {
    return {
      .first_line     = location.last_line,
      .first_column   = location.last_column,
      .last_line      = location.last_line,
      .last_column    = location.last_column
    };
  }

  // Given two locations, create a new location that spans between them.
  YYLTYPE makeSpannedLocation(YYLTYPE startLoc, YYLTYPE endLoc) {
    return {
      .first_line     = startLoc.first_line,
      .first_column   = startLoc.first_column,
      .last_line      = endLoc.last_line,
      .last_column    = endLoc.last_column
    };
  }

  void noteError(ParserError error) {
    errors.push_back(std::move(error));
  }
  void noteError(YYLTYPE location, const char* msg) {
    noteError(ParserError(location, msg));
  }
  void noteError(YYLTYPE location, std::string msg) {
    noteError(ParserError(location, std::move(msg)));
  }
  ErroneousExpression* raiseError(ParserError error) {
    Location ll = convertLocation(error.location);
    // note the error for printing
    noteError(std::move(error));
    // return an error sentinel
    return ErroneousExpression::build(builder, ll).release();
  }
  ErroneousExpression* raiseError(YYLTYPE location, const char* msg) {
    return raiseError(ParserError(location, msg));
  }
  ErroneousExpression* raiseError(YYLTYPE location, std::string msg) {
    return raiseError(ParserError(location, msg));
  }

  void noteComment(YYLTYPE loc, const char* data, long size);
  std::vector<ParserComment>* gatherComments(YYLTYPE location);
  void clearCommentsBefore(YYLTYPE loc);
  void clearComments(std::vector<ParserComment>* comments);
  void clearComments(ParserExprList* comments);
  void clearComments();
  ParserExprList* makeList();
  ParserExprList* makeList(ParserExprList* lst);
  ParserExprList* makeList(Expression* e);
  ParserExprList* makeList(owned<Expression> e) {
    return this->makeList(e.release());
  }

  ParserExprList* makeList(CommentsAndStmt cs);

  ParserExprList* appendList(ParserExprList* dst, ParserExprList* lst);
  ParserExprList* appendList(ParserExprList* dst, Expression* e);
  ParserExprList* appendList(ParserExprList* dst, owned<Expression> e) {
    this->appendList(dst, e.release());
    return dst;
  }
  ParserExprList* appendList(ParserExprList* dst,
                             std::vector<ParserComment>* comments);
  ParserExprList* appendList(ParserExprList* dst, CommentsAndStmt cs);
  ASTList consumeList(ParserExprList* lst);

 void consumeNamedActuals(MaybeNamedActualList* lst,
                          ASTList& actualsOut,
                          std::vector<UniqueString>& namesOut);

  std::vector<ParserComment>* gatherCommentsFromList(ParserExprList* lst,
                                                     YYLTYPE location);

  void discardCommentsFromList(ParserExprList* lst, YYLTYPE location);

  void appendComments(CommentsAndStmt*cs, std::vector<ParserComment>* comments);

  // clears the inner comments that should have already been captured
  // to handle things like this
  //     { /* doc comment } proc myproc()
  CommentsAndStmt finishStmt(CommentsAndStmt cs);
  CommentsAndStmt finishStmt(Expression* e);
  CommentsAndStmt finishStmt(owned<Expression> e) {
    return this->finishStmt(e.release());
  }



  // Create a ParserExprList containing the passed statements, and any
  // comments before the right brace brace location.
  ParserExprList* blockToParserExprList(YYLTYPE lbrLoc, YYLTYPE rbrLoc,
                                        ParserExprList* body);

  // This should consume the comments that occur before
  // and return them. (Including looking at source locations).
  // If there is any argument, it will
  // be also appended to the returned list.
  ParserExprList* enterStmt(YYLTYPE location, ParserExprList* lst);
  ParserExprList* enterStmt(YYLTYPE location, Expression* e);
  ParserExprList* enterStmt(YYLTYPE location, owned<Expression> e) {
    return this->enterStmt(location, e.release());
  }
  ParserExprList* enterStmt(YYLTYPE location);

  // These should clear the comments (since there might be some inside the stmt)
  ParserExprList* exitStmt(ParserExprList* lst);
  ParserExprList* exitStmt(Expression* e);
  ParserExprList* exitStmt(owned<Expression> e) {
    return this->exitStmt(e.release());
  }

  Location convertLocation(YYLTYPE location);

  Identifier* buildEmptyIdent(YYLTYPE location);
  Identifier* buildIdent(YYLTYPE location, PODUniqueString name);

  OpCall* buildBinOp(YYLTYPE location,
                     Expression* lhs, PODUniqueString op, Expression* rhs);
  OpCall* buildUnaryOp(YYLTYPE location,
                       PODUniqueString op, Expression* expr);

  FunctionParts makeFunctionParts(bool isInline,
                                  bool isOverride);
  CommentsAndStmt buildFunctionDecl(YYLTYPE location, FunctionParts& fp);

  // Build a loop index decl from a given expression. The expression is owned
  // because it will be consumed. 
  owned<Decl> buildLoopIndexDecl(YYLTYPE location, owned<Expression> e);

  FnCall* wrapCalledExpressionInNew(YYLTYPE location,
                                    New::Management management,
                                    FnCall* fnCall);

  BlockStyle determineBlockStyle(BlockOrDo blockOrDo);

  ASTList consumeAndFlattenTopLevelBlocks(ParserExprList* exprLst);

  // Lift up top level comments, clear expression level comments, prepare
  // the statement body, and determine the block style.
  //
  // Note that the body anchor is the location at which non-statement
  // expressions preceding the body end. For constructs that precede their
  // body with a keyword, the body anchor is the keyword. For constructs
  // that always have a block as their body, the body anchor is the '{'.
  //
  // For constructs which have a <stmt> as the body and are preceded by
  // some number of non-statement expressions, the anchor should be the
  // location which marks the end of the non-statement expressions, and
  // not the location of the <stmt>. For example, given:
  //
  //    begin /*c2*/ with (var a=0) ^^^ /*c3*/ writeln(a);
  //
  // The anchor point is at ^^^, because this is the point where the with
  // clause ends. We want /*c3*/ to be included in the statements of the
  // begin, so we cannot use the location of the 'writeln' as the anchor
  // point. If we do, then /*c3*/ will be discarded along with /*c2*/.
  //
  // Constructs that have an arbitrary <stmt> as their body can use the
  // 'makeLocationAtLast' method to produce a location at the ^^^. The
  // constructed location can be passed as the anchor.
  //
  // This approach is used over mid-rule actions because mid-rule actions
  // make the parser rules/actions harder to read.
  void prepareStmtPieces(std::vector<ParserComment>*& outComments,
                         ParserExprList*& outExprLst,
                         BlockStyle& outBlockStyle,
                         YYLTYPE locStartKeyword,
                         bool isBodyAnchorSecondKeyword,
                         YYLTYPE locBodyAnchor,
                         CommentsAndStmt consume);

  void prepareStmtPieces(std::vector<ParserComment>*& outComments,
                         ParserExprList*& outExprLst,
                         BlockStyle& outBlockStyle,
                         YYLTYPE locStartKeyword,
                         YYLTYPE locBodyAnchor, 
                         BlockOrDo consume);

  CommentsAndStmt buildBracketLoopStmt(YYLTYPE locLeftBracket,
                                       YYLTYPE locIndex,
                                       YYLTYPE locBodyAnchor,
                                       ParserExprList* indexExprs,
                                       Expression* iterandExpr,
                                       WithClause* withClause,
                                       CommentsAndStmt cs);

  CommentsAndStmt buildBracketLoopStmt(YYLTYPE locLeftBracket,
                                       YYLTYPE locIterExprs,
                                       YYLTYPE locBodyAnchor,
                                       ParserExprList* iterExprs,
                                       WithClause* withClause,
                                       CommentsAndStmt cs);

  CommentsAndStmt buildForallLoopStmt(YYLTYPE locForall,
                                      YYLTYPE locIndex,
                                      YYLTYPE locBodyAnchor,
                                      Expression* indexExpr,
                                      Expression* iterandExpr,
                                      WithClause* withClause,
                                      BlockOrDo blockOrDo);

  CommentsAndStmt buildForeachLoopStmt(YYLTYPE locForeach,
                                       YYLTYPE locIndex,
                                       YYLTYPE locBodyAnchor,
                                       Expression* indexExpr,
                                       Expression* iterandExpr,
                                       WithClause* withClause,
                                       BlockOrDo blockOrDo);

  CommentsAndStmt buildForLoopStmt(YYLTYPE locFor,
                                   YYLTYPE locIndex,
                                   YYLTYPE locBodyAnchor,
                                   Expression* indexExpr,
                                   Expression* iterandExpr,
                                   BlockOrDo blockOrDo);

  CommentsAndStmt buildCoforallLoopStmt(YYLTYPE locCoforall,
                                        YYLTYPE locIndex,
                                        YYLTYPE locBodyAnchor,
                                        Expression* indexExpr,
                                        Expression* iterandExpr,
                                        WithClause* withClause,
                                        BlockOrDo blockOrDo);

  CommentsAndStmt buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                       YYLTYPE locThenBodyAnchor,
                                       Expression* condition,
                                       CommentsAndStmt thenCs);

  CommentsAndStmt buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                       YYLTYPE locThenBodyAnchor,
                                       YYLTYPE locElse,
                                       Expression* condition,
                                       CommentsAndStmt thenCs,
                                       CommentsAndStmt elseCs);

  uint64_t binStr2uint64(YYLTYPE location, const char* str, bool& erroroneous);
  uint64_t octStr2uint64(YYLTYPE location, const char* str, bool& erroroneous);
  uint64_t decStr2uint64(YYLTYPE location, const char* str, bool& erroroneous);
  uint64_t hexStr2uint64(YYLTYPE location, const char* str, bool& erroroneous);
  double str2double(YYLTYPE location, const char* str, bool& erroroneous);

  Expression* buildNumericLiteral(YYLTYPE location,
                                  PODUniqueString str,
                                  int type);

  // Returns Expression because it may build an ErroneousExpression.
  Expression* buildAsExpr(YYLTYPE locName, YYLTYPE locRename,
                          owned<Expression> name,
                          owned<Expression> rename);

  CommentsAndStmt
  buildSingleUseStmt(YYLTYPE locEverything, YYLTYPE locVisibilityClause,
                     Decl::Visibility visibility,
                     owned<Expression> name,
                     VisibilityClause::LimitationKind limitationKind,
                     ParserExprList* limitationExprs);

  // Given a list of vars, build either a single var or a multi-decl.
  CommentsAndStmt
  buildVarOrMultiDecl(YYLTYPE locEverything, ParserExprList* vars);

  CommentsAndStmt buildAggregateTypeDecl(YYLTYPE location,
                                         TypeDeclParts parts,
                                         YYLTYPE inheritLoc,
                                         ParserExprList* optInherit,
                                         YYLTYPE openingBrace,
                                         ParserExprList* contents,
                                         YYLTYPE closingBrace);

};
