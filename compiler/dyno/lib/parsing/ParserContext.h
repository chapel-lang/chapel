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
  ErrorMessage::Kind kind;
  YYLTYPE location;
  std::string message;
  ParserError(YYLTYPE location, std::string message,
              ErrorMessage::Kind kind)
    : kind(kind), location(location), message(message) {
    gdbShouldBreakHere();
  }
  ParserError(YYLTYPE location, const char* message,
              ErrorMessage::Kind kind)
    : kind(kind), location(location), message(message) {
    gdbShouldBreakHere();
  }
};

struct ParserComment {
  YYLTYPE location;
  Comment* comment;
};

// To store the different attributes of a symbol as they are built.
struct AttributeParts {
  std::set<PragmaTag>* pragmas;
  bool isDeprecated;
  UniqueString deprecationMessage;
};

struct ParserContext {
  yyscan_t scanner;
  UniqueString filename;
  Builder* builder;
  parsing::ParserStats* parseStats;

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
  Decl::Linkage linkage;
  AstNode* varDeclLinkageName;
  bool hasNotedVarDeclKind;
  Variable::Kind varDeclKind;
  bool isVarDeclConfig;
  bool isBuildingFormal;
  AttributeParts attributeParts;
  bool hasAttributeParts;
  int numAttributesBuilt;
  YYLTYPE declStartLocation;

  // this type and stack helps the parser know if a function
  // declaration is a method.
  struct ParserScope {
    asttags::AstTag tag;
    UniqueString name;
  };
  std::vector<ParserScope> scopeStack;

  // note when EOF is reached
  bool atEOF;

  ParserExprList* parenlessMarker;

  ParserContext(const char* filename, Builder* builder,
                parsing::ParserStats* parseStats)
  {
    auto uniqueFilename = UniqueString::get(builder->context(), filename);

    this->scanner                 = nullptr;
    this->filename                = uniqueFilename;
    this->builder                 = builder;
    this->topLevelStatements      = nullptr;
    this->comments                = nullptr;
    this->visibility              = Decl::DEFAULT_VISIBILITY;
    this->linkage                 = Decl::DEFAULT_LINKAGE;
    this->varDeclLinkageName      = nullptr;
    this->hasNotedVarDeclKind     = false;
    this->varDeclKind             = Variable::VAR;
    this->isBuildingFormal        = false;
    this->isVarDeclConfig         = false;
    this->attributeParts          = { nullptr, false, UniqueString() };
    this->hasAttributeParts       = false;
    this->numAttributesBuilt      = 0;
    YYLTYPE emptyLoc = {0};
    this->declStartLocation       = emptyLoc;
    this->atEOF                   = false;
    this->parenlessMarker         = new ParserExprList();
    this->parseStats              = parseStats;
  }
  ~ParserContext() {
    delete this->parenlessMarker;
  }

  Context* context() { return builder->context(); }

  void noteDeclStartLoc(YYLTYPE loc);
  Decl::Visibility noteVisibility(Decl::Visibility visibility);
  Decl::Linkage noteLinkage(Decl::Linkage linkage);
  Variable::Kind noteVarDeclKind(Variable::Kind varDeclKind);

  // When 'store' is called the value of 'this->linkageName' must be empty
  // or an assert will fire. When 'consume' is called 'this->linkageName'
  // will be returned, and the new value will be nullptr. This is done
  // to ensure that only one UAST node is a parent of the returned linkage
  // name expression.
  void storeVarDeclLinkageName(AstNode* linkageName);
  owned<AstNode> consumeVarDeclLinkageName(void);

  // If attributes do not exist yet, returns nullptr.
  owned<Attributes> buildAttributes(YYLTYPE locationOfDecl);
  PODUniqueString notePragma(YYLTYPE loc, AstNode* pragmaStr);
  void noteDeprecation(YYLTYPE loc, AstNode* messageStr);
  void resetAttributePartsState();

  CommentsAndStmt buildPragmaStmt(YYLTYPE loc, CommentsAndStmt stmt);

  bool noteIsBuildingFormal(bool isBuildingFormal);
  bool noteIsVarDeclConfig(bool isConfig);
  YYLTYPE declStartLoc(YYLTYPE curLoc);
  void resetDeclStateOnError();
  void resetDeclState();

  void enterScope(asttags::AstTag tag, UniqueString name);
  ParserScope currentScope();
  bool currentScopeIsAggregate();
  void exitScope(asttags::AstTag tag, UniqueString name);

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

  void noteSyntaxError(ParserError error) {
    assert(error.kind == ErrorMessage::SYNTAX);
    errors.push_back(std::move(error));
  }

  void noteError(ParserError error) {
    assert(error.kind == ErrorMessage::ERROR);
    errors.push_back(std::move(error));
  }

  void noteError(YYLTYPE location, const char* msg) {
    noteError(ParserError(location, msg, ErrorMessage::ERROR));
  }

  void noteError(YYLTYPE location, std::string msg) {
    noteError(ParserError(location, std::move(msg), ErrorMessage::ERROR));
  }

  void noteWarning(ParserError error) {
    assert(error.kind == ErrorMessage::WARNING);
    errors.push_back(std::move(error));
  }

  void noteWarning(YYLTYPE location, const char* msg) {
    noteWarning(ParserError(location, msg, ErrorMessage::WARNING));
  }

  void noteWarning(YYLTYPE location, std::string msg) {
    noteWarning(ParserError(location, std::move(msg), ErrorMessage::WARNING));
  }

  void noteNote(YYLTYPE location, std::string msg) {
    auto err = ParserError(location, std::move(msg), ErrorMessage::NOTE);
    errors.push_back(std::move(err));
  }

  ErroneousExpression* raiseError(ParserError error) {
    assert(error.kind == ErrorMessage::ERROR);
    Location ll = convertLocation(error.location);
    // note the error for printing
    noteError(std::move(error));
    // return an error sentinel
    return ErroneousExpression::build(builder, ll).release();
  }

  ErroneousExpression* raiseError(YYLTYPE location, const char* msg) {
    return raiseError(ParserError(location, msg, ErrorMessage::ERROR));
  }

  ErroneousExpression* raiseError(YYLTYPE location, std::string msg) {
    return raiseError(ParserError(location, msg, ErrorMessage::ERROR));
  }

  void noteComment(YYLTYPE loc, const char* data, long size);
  std::vector<ParserComment>* gatherComments(YYLTYPE location);
  void clearCommentsBefore(YYLTYPE loc);
  void clearComments(std::vector<ParserComment>* comments);
  void clearComments(ParserExprList* comments);
  void clearComments();
  ParserExprList* makeList();
  ParserExprList* makeList(ParserExprList* lst);
  ParserExprList* makeList(AstNode* e);
  ParserExprList* makeList(owned<AstNode> e) {
    return this->makeList(e.release());
  }

  ParserExprList* makeList(CommentsAndStmt cs);

  ParserExprList* appendList(ParserExprList* dst, ParserExprList* lst);
  ParserExprList* appendList(ParserExprList* dst, AstNode* e);
  ParserExprList* appendList(ParserExprList* dst, owned<AstNode> e) {
    this->appendList(dst, e.release());
    return dst;
  }
  ParserExprList* appendList(ParserExprList* dst,
                             std::vector<ParserComment>* comments);
  ParserExprList* appendList(ParserExprList* dst, CommentsAndStmt cs);
  AstList consumeList(ParserExprList* lst);
  AstList consume(AstNode* e);

  void consumeNamedActuals(MaybeNamedActualList* lst,
                           AstList& actualsOut,
                           std::vector<UniqueString>& namesOut);

  std::vector<ParserComment>* gatherCommentsFromList(ParserExprList* lst,
                                                     YYLTYPE location);

  void discardCommentsFromList(ParserExprList* lst, YYLTYPE location);
  void discardCommentsFromList(ParserExprList* lst);

  void appendComments(CommentsAndStmt*cs, std::vector<ParserComment>* comments);

  // clears the inner comments that should have already been captured
  // to handle things like this
  //     { /* doc comment } proc myproc()
  CommentsAndStmt finishStmt(CommentsAndStmt cs);
  CommentsAndStmt finishStmt(YYLTYPE location, CommentsAndStmt cs);
  CommentsAndStmt finishStmt(AstNode* e);
  CommentsAndStmt finishStmt(owned<AstNode> e) {
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
  ParserExprList* enterStmt(YYLTYPE location, AstNode* e);
  ParserExprList* enterStmt(YYLTYPE location, owned<AstNode> e) {
    return this->enterStmt(location, e.release());
  }
  ParserExprList* enterStmt(YYLTYPE location);

  // These should clear the comments (since there might be some inside the stmt)
  ParserExprList* exitStmt(ParserExprList* lst);
  ParserExprList* exitStmt(AstNode* e);
  ParserExprList* exitStmt(owned<AstNode> e) {
    return this->exitStmt(e.release());
  }

  Location convertLocation(YYLTYPE location);

  Identifier* buildEmptyIdent(YYLTYPE location);
  Identifier* buildIdent(YYLTYPE location, PODUniqueString name);

  AstNode* buildPrimCall(YYLTYPE location, MaybeNamedActualList* lst);

  OpCall* buildBinOp(YYLTYPE location,
                     AstNode* lhs, PODUniqueString op, AstNode* rhs);
  OpCall* buildUnaryOp(YYLTYPE location,
                       PODUniqueString op, AstNode* expr);

  AstNode* buildManagerExpr(YYLTYPE location, AstNode* expr,
                            Variable::Kind kind,
                            YYLTYPE locResourceName,
                            UniqueString resourceName);

  AstNode* buildManagerExpr(YYLTYPE location, AstNode* expr,
                            YYLTYPE locResourceName,
                            UniqueString resourceName);

  CommentsAndStmt buildManageStmt(YYLTYPE location,
                                  ParserExprList* managerExprs,
                                  YYLTYPE locBlockOrDo,
                                  BlockOrDo blockOrDo);

  FunctionParts makeFunctionParts(bool isInline,
                                  bool isOverride);

  CommentsAndStmt
  buildExternExportFunctionDecl(YYLTYPE location, FunctionParts& fp);

  CommentsAndStmt
  buildRegularFunctionDecl(YYLTYPE location, FunctionParts& fp);

  CommentsAndStmt buildFunctionDecl(YYLTYPE location, FunctionParts& fp);

  AstNode* buildLambda(YYLTYPE location, FunctionParts& fp);

  AstNode* buildLetExpr(YYLTYPE location, ParserExprList* decls,
                        AstNode* expr);

  AstNode* buildArrayTypeWithIndex(YYLTYPE location,
                                   YYLTYPE locIndexExprs,
                                    ParserExprList* indexExprs,
                                    AstNode* domainExpr,
                                    AstNode* typeExpr);

  AstNode* buildArrayType(YYLTYPE location, YYLTYPE locDomainExprs,
                          ParserExprList* domainExprs,
                          AstNode* typeExpr);

  AstNode* buildTupleComponent(YYLTYPE location, PODUniqueString name);
  AstNode* buildTupleComponent(YYLTYPE location, ParserExprList* exprs);

  // Build a loop index decl from a given expression. May return nullptr
  // if the index expression is not valid. TODO: Adjust me to return an
  // AstNode instead if possible?
  owned<Decl> buildLoopIndexDecl(YYLTYPE location, const AstNode* e);
  owned<Decl> buildLoopIndexDecl(YYLTYPE location, owned<AstNode> e);
  owned<Decl> buildLoopIndexDecl(YYLTYPE location, ParserExprList* exprLst);

  AstNode* buildNewExpr(YYLTYPE location,
                           New::Management management,
                           AstNode* expr);

  FnCall* wrapCalledExpressionInNew(YYLTYPE location,
                                    New::Management management,
                                    FnCall* fnCall);

  BlockStyle determineBlockStyle(BlockOrDo blockOrDo);

  AstList consumeAndFlattenTopLevelBlocks(ParserExprList* exprLst);

  owned<Block> consumeToBlock(YYLTYPE blockLoc, ParserExprList* lst);
  owned<Block> consumeToBlock(YYLTYPE blockLoc, AstNode* e);

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
                                       AstNode* iterandExpr,
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
                                      AstNode* indexExpr,
                                      AstNode* iterandExpr,
                                      WithClause* withClause,
                                      BlockOrDo blockOrDo);

  CommentsAndStmt buildForeachLoopStmt(YYLTYPE locForeach,
                                       YYLTYPE locIndex,
                                       YYLTYPE locBodyAnchor,
                                       AstNode* indexExpr,
                                       AstNode* iterandExpr,
                                       WithClause* withClause,
                                       BlockOrDo blockOrDo);

  CommentsAndStmt buildForLoopStmt(YYLTYPE locFor,
                                   YYLTYPE locIndex,
                                   YYLTYPE locBodyAnchor,
                                   AstNode* indexExpr,
                                   AstNode* iterandExpr,
                                   BlockOrDo blockOrDo);

  CommentsAndStmt buildCoforallLoopStmt(YYLTYPE locCoforall,
                                        YYLTYPE locIndex,
                                        YYLTYPE locBodyAnchor,
                                        AstNode* indexExpr,
                                        AstNode* iterandExpr,
                                        WithClause* withClause,
                                        BlockOrDo blockOrDo);

  CommentsAndStmt buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                       YYLTYPE locThenBodyAnchor,
                                       AstNode* condition,
                                       CommentsAndStmt thenCs);

  CommentsAndStmt buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                       YYLTYPE locThenBodyAnchor,
                                       YYLTYPE locElse,
                                       AstNode* condition,
                                       CommentsAndStmt thenCs,
                                       CommentsAndStmt elseCs);

  CommentsAndStmt buildExternBlockStmt(YYLTYPE locEverything,
                                       SizedStr sizedStr);

  AstNode* buildNumericLiteral(YYLTYPE location,
                               PODUniqueString str,
                               int type);

  // Returns AstNode because it may build an ErroneousExpression.
  AstNode* buildAsExpr(YYLTYPE locName, YYLTYPE locRename,
                       owned<AstNode> name,
                       owned<AstNode> rename);

  AstNode*
  buildVisibilityClause(YYLTYPE location, owned<AstNode> symbol, bool isImport);

  AstNode*
  buildVisibilityClause(YYLTYPE location, owned<AstNode> symbol,
                        VisibilityClause::LimitationKind limitationKind,
                        AstList limitations, bool isImport);

  CommentsAndStmt
  buildImportStmt(YYLTYPE locEverything, Decl::Visibility visibility,
                  ParserExprList* visibilityClauses);

  CommentsAndStmt
  buildMultiUseStmt(YYLTYPE locEverything, Decl::Visibility visibility,
                    ParserExprList* visibilityClauses);

  CommentsAndStmt
  buildSingleUseStmt(YYLTYPE locEverything, YYLTYPE locVisibilityClause,
                     Decl::Visibility visibility,
                     owned<AstNode> name,
                     VisibilityClause::LimitationKind limitationKind,
                     ParserExprList* limitationExprs);

  // Given a list of vars, build either a single var or a multi-decl.
  CommentsAndStmt
  buildVarOrMultiDeclStmt(YYLTYPE locEverything, ParserExprList* vars);

  TypeDeclParts enterScopeAndBuildTypeDeclParts(YYLTYPE locStart,
                                                PODUniqueString name,
                                                asttags::AstTag tag);

  void validateExternTypeDeclParts(YYLTYPE locStart, TypeDeclParts& parts);

  CommentsAndStmt buildAggregateTypeDecl(YYLTYPE location,
                                         TypeDeclParts parts,
                                         YYLTYPE inheritLoc,
                                         ParserExprList* optInherit,
                                         YYLTYPE openingBrace,
                                         ParserExprList* contents,
                                         YYLTYPE closingBrace);

  AstNode* buildReduce(YYLTYPE location, YYLTYPE locOp,
                       PODUniqueString op,
                       AstNode* iterand);

  AstNode* buildReduce(YYLTYPE location, YYLTYPE locOp,
                       AstNode* op,
                       AstNode* iterand);

  AstNode* buildScan(YYLTYPE location, YYLTYPE locOp,
                     PODUniqueString op,
                     AstNode* iterand);

  AstNode* buildScan(YYLTYPE location, YYLTYPE locOp,
                     AstNode* op,
                     AstNode* iterand);

  AstNode* buildTypeQuery(YYLTYPE location,
                          PODUniqueString queriedIdent);

  AstNode* buildTypeConstructor(YYLTYPE location,
                                PODUniqueString baseType,
                                AstNode* subType);

  AstNode* buildTypeConstructor(YYLTYPE location,
                                PODUniqueString baseType,
                                MaybeNamedActual actual);

  AstNode* buildTypeConstructor(YYLTYPE location,
                                PODUniqueString baseType,
                                MaybeNamedActualList* actuals);

  CommentsAndStmt buildTryExprStmt(YYLTYPE location, AstNode* expr,
                                   bool isTryBang);

  CommentsAndStmt buildTryExprStmt(YYLTYPE location, CommentsAndStmt cs,
                                   bool isTryBang);

  AstNode* buildTryExpr(YYLTYPE location, AstNode* expr,
                        bool isTryBang);

  AstNode* buildEmptyStmt(YYLTYPE location);

  CommentsAndStmt buildTryCatchStmt(YYLTYPE location, CommentsAndStmt block,
                                    ParserExprList* handlers,
                                    bool isTryBang);

  AstNode* buildCatch(YYLTYPE location, AstNode* error,
                      CommentsAndStmt block,
                      bool hasParensAroundError);

  CommentsAndStmt buildWhenStmt(YYLTYPE location, ParserExprList* caseExprs,
                                BlockOrDo blockOrDo);

  CommentsAndStmt buildSelectStmt(YYLTYPE location, owned<AstNode> expr,
                                  ParserExprList* whenStmts);

  CommentsAndStmt
  buildForwardingDecl(YYLTYPE location, owned<Attributes> attributes,
                      owned<AstNode> expr,
                      VisibilityClause::LimitationKind limitationKind,
                      ParserExprList* limitations);

  CommentsAndStmt
  buildForwardingDecl(YYLTYPE location, owned<Attributes> attributes,
                      CommentsAndStmt cs);

  AstNode* buildInterfaceFormal(YYLTYPE location, PODUniqueString name);

  CommentsAndStmt buildInterfaceStmt(YYLTYPE location,
                                     PODUniqueString name,
                                     ParserExprList* formals,
                                     YYLTYPE locBody,
                                     CommentsAndStmt body);

  owned<AstNode>
  buildInterfaceExpr(YYLTYPE location, PODUniqueString name,
                     MaybeNamedActualList* formals);

  CommentsAndStmt buildImplementsStmt(YYLTYPE location,
                                      YYLTYPE locTypeExpr,
                                      PODUniqueString type,
                                      YYLTYPE locInterfaceExpr,
                                      PODUniqueString name,
                                      MaybeNamedActualList* formals);

  CommentsAndStmt buildImplementsStmt(YYLTYPE location,
                                      YYLTYPE locInterfaceExpr,
                                      PODUniqueString name,
                                      MaybeNamedActualList* formals);

  AstNode* buildImplementsConstraint(YYLTYPE location,
                                     YYLTYPE locTypeExpr,
                                     PODUniqueString type,
                                     YYLTYPE locInterfaceExpr,
                                     PODUniqueString name,
                                     MaybeNamedActualList* formals);

  AstNode* buildImplementsConstraint(YYLTYPE location,
                                     YYLTYPE locInterfaceExpr,
                                     PODUniqueString name,
                                     MaybeNamedActualList* formals);

  CommentsAndStmt buildLabelStmt(YYLTYPE location, PODUniqueString name,
                                 CommentsAndStmt cs);
};
