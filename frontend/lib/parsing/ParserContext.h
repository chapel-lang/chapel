/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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

struct ParserComment {
  YYLTYPE location;
  Comment* comment;
};

// To store the different attributes of a symbol as they are built.
struct AttributeGroupParts {
  ParserExprList* attributeList; // this is where the attributes are accumulated
  std::set<PragmaTag>* pragmas;
  bool isDeprecated;
  bool isUnstable;
  bool isParenfulDeprecated;
  bool isStable;
  bool hasEdition;
  UniqueString deprecationMessage;
  UniqueString unstableMessage;
  UniqueString parenfulDeprecationMessage;
  UniqueString firstEdition;
  UniqueString lastEdition;
};

struct ParserContext {
  yyscan_t scanner;
  UniqueString filename;
  Builder* builder;
  parsing::ParserStats* parseStats;

  ParserExprList* topLevelStatements;

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
  YYLTYPE configLoc;
  bool isBuildingFormal;
  AstNode* varDestinationExpr;
  AttributeGroupParts attributeGroupParts;
  bool hasAttributeGroupParts;
  int numAttributesBuilt;
  std::vector<owned<AttributeGroup>> loopAttributes;
  YYLTYPE declStartLocation;
  YYLTYPE curlyBraceLocation;

  // this type and stack helps the parser know if a function
  // declaration is a method.
  struct ParserScope {
    asttags::AstTag tag;
    UniqueString name;
  };
  std::vector<ParserScope> scopeStack;

  // note when EOF is reached
  bool atEOF;

  // an easier-to-use copy of Context::Configuration::includeComments
  bool includeComments;

  ParserExprList* parenlessMarker;

  ParserContext(const char* filename, Builder* builder,
                parsing::ParserStats* parseStats)
  {
    auto uniqueFilename = UniqueString::get(builder->context(), filename);
    YYLTYPE emptyLoc = {0};

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
    this->configLoc               = emptyLoc;
    this->varDestinationExpr      = nullptr;
    this->attributeGroupParts     = {nullptr, nullptr, false, false, false, false, false, UniqueString(), UniqueString(), UniqueString(), UniqueString(), UniqueString() };
    this->hasAttributeGroupParts  = false;
    this->numAttributesBuilt      = 0;
    this->declStartLocation       = emptyLoc;
    this->curlyBraceLocation      = emptyLoc;
    this->atEOF                   = false;
    this->includeComments =
      builder->context()->configuration().includeComments;
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

  void noteAttribute(YYLTYPE loc, AstNode* firstIdent,
                     ParserExprList* toolspace,
                     MaybeNamedActualList* actuals);

  owned<Attribute> buildAttribute(YYLTYPE loc, AstNode* firstIdent,
                                  ParserExprList* toolspace,
                                  MaybeNamedActualList* actuals);
  owned<AttributeGroup> popLoopAttributeGroup();

  // If attributes do not exist yet, returns nullptr.
  owned<AttributeGroup> buildAttributeGroup(YYLTYPE locationOfDecl);
  // Same as buildAttributeGroup, but resets the state and pushes onto group stack
  void buildAndPushAttributeGroup(YYLTYPE locationOfDecl);

  PODUniqueString notePragma(YYLTYPE loc, AstNode* pragmaStr);
  void noteDeprecation(YYLTYPE loc, MaybeNamedActualList* actuals);
  void noteUnstable(YYLTYPE loc, MaybeNamedActualList* actuals);
  void noteStable(YYLTYPE loc, MaybeNamedActualList* actuals);
  void noteEdition(YYLTYPE loc, MaybeNamedActualList* actuals);
  void resetAttributeGroupPartsState();

  CommentsAndStmt buildPragmaStmt(YYLTYPE loc, CommentsAndStmt stmt);

  bool noteIsBuildingFormal(bool isBuildingFormal);
  bool noteIsVarDeclConfig(bool isConfig, YYLTYPE loc);
  void noteVarDestinationExpr(AstNode* targetExpr);
  owned<AstNode> consumeVarDestinationExpr();
  YYLTYPE declStartLoc(YYLTYPE curLoc);
  void resetDeclStateOnError();
  void resetDeclState();

  void enterScope(asttags::AstTag tag, UniqueString name);
  ParserScope currentScope();
  bool currentScopeIsAggregate();
  void exitScope(asttags::AstTag tag, UniqueString name);

  void noteCurlyBraces(YYLTYPE left, YYLTYPE right);
  bool isValidCurlyBracesLoc(YYLTYPE loc);
  bool hasCurlyBracesLoc();
  YYLTYPE curlyBracesLoc();
  void resetCurlyBracesLoc();

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

  YYLTYPE locationFromChplLocation(AstNode* ast) {
    auto chapelLoc = builder->getLocation(ast);
    YYLTYPE ret = {
      .first_line = chapelLoc.firstLine(),
      .first_column = chapelLoc.firstColumn(),
      .last_line = chapelLoc.lastLine(),
      .last_column = chapelLoc.lastColumn()
    };
    return ret;
  }
  YYLTYPE locationFromChplLocation(const AstNode* ast) {
    auto chapelLoc = builder->getLocation(ast);
    YYLTYPE ret = {
      .first_line = chapelLoc.firstLine(),
      .first_column = chapelLoc.firstColumn(),
      .last_line = chapelLoc.lastLine(),
      .last_column = chapelLoc.lastColumn()
    };
    return ret;
  }

  ErroneousExpression* report(YYLTYPE loc, owned<ErrorBase> error);
  ErroneousExpression* error(YYLTYPE loc, const char* fmt, ...);
  ErroneousExpression* syntax(YYLTYPE loc, const char* fmt, ...);

  // This overload deep copies 'data' and will also deallocate 'data'.
  void noteComment(YYLTYPE loc, const char* data, long size);

  // This overload takes ownership and does not deep copy 'pc'.
  void noteComment(ParserComment pc);

  std::vector<ParserComment>* gatherComments(YYLTYPE location);
  void clearCommentsBefore(YYLTYPE loc);
  void clearCommentsAfter(YYLTYPE loc);
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

  ParserNDArrayList* makeNDArrayList();
  ParserNDArrayList* appendNDArrayList(ParserNDArrayList* dst,
                                       NDArrayElement e);
  Array* buildNDArray(YYLTYPE location, ParserNDArrayList* lst);

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

  void adjustLocation(CommentsAndStmt cs, YYLTYPE location) {
    builder->noteLocation(cs.stmt, convertLocation(location));
  }
  void adjustLocation(CommentsAndStmt cs, YYLTYPE start, YYLTYPE end) {
    builder->noteLocation(cs.stmt, convertLocation(
      makeSpannedLocation(start, end)));
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

  AstNode* buildTaskIntent(YYLTYPE loc,
                           YYLTYPE nameLoc,
                           AttributeGroup* attributeGroup,
                           AstNode* identNode,
                           MaybeIntent intent,
                           AstNode* typeExpression,
                           AstNode* initExpression);

  WithClause* buildWithClause(YYLTYPE location, YYLTYPE locWith,
                              YYLTYPE locLeftParen,
                              YYLTYPE locTaskIntentList,
                              YYLTYPE locRightParen,
                              ParserExprList* exprList);

  CommentsAndStmt buildBeginStmt(YYLTYPE location, YYLTYPE locBegin,
                                 YYLTYPE locWithClause,
                                 YYLTYPE locStmt,
                                 WithClause* withClause,
                                 CommentsAndStmt stmt);

  Identifier* buildEmptyIdent(YYLTYPE location);
  Identifier* buildIdent(YYLTYPE location, PODUniqueString name);
  Identifier* buildAttributeIdent(YYLTYPE location, PODUniqueString name);

  AstNode* buildPrimCall(YYLTYPE location, MaybeNamedActualList* lst);

  OpCall* buildBinOp(YYLTYPE location,
                     AstNode* lhs, PODUniqueString op, AstNode* rhs);
  OpCall* buildUnaryOp(YYLTYPE location,
                       PODUniqueString op, AstNode* expr);

  AstNode* buildDot(YYLTYPE location, YYLTYPE locReceiver, YYLTYPE locPeriod,
                    YYLTYPE locDotField,
                    AstNode* receiver,
                    UniqueString field,
                    bool wrapInCall=false);

  AstNode* buildManagerExpr(YYLTYPE location, AstNode* expr,
                            Variable::Kind kind,
                            YYLTYPE locResourceName,
                            UniqueString resourceName);

  AstNode* buildManagerExpr(YYLTYPE location, AstNode* expr,
                            YYLTYPE locResourceName,
                            UniqueString resourceName);

  CommentsAndStmt buildManageStmt(YYLTYPE location,
                                  YYLTYPE locHeader,
                                  ParserExprList* managerExprs,
                                  YYLTYPE locBlockOrDo,
                                  BlockOrDo blockOrDo);

  FunctionParts makeFunctionParts(bool isInline,
                                  bool isOverride);

  AstNode*
  buildFormal(YYLTYPE location,
              YYLTYPE locName,
              YYLTYPE locIntent,
              MaybeIntent intent,
              PODUniqueString name,
              AstNode* typeExpr,
              AstNode* initExpr,
              bool consumeAttributeGroup=false);

  AstNode*
  buildVarArgFormal(YYLTYPE location,
                    YYLTYPE locIntent,
                    MaybeIntent intent,
                    PODUniqueString name,
                    YYLTYPE nameLocation,
                    AstNode* typeExpr,
                    AstNode* initExpr,
                    bool consumeAttributeGroup=false);

  AstNode*
  buildTupleFormal(YYLTYPE location,
                   YYLTYPE locIntent,
                   MaybeIntent intent,
                   ParserExprList* components,
                   AstNode* typeExpr,
                   AstNode* initExpr);

  AstNode*
  consumeFormalToAnonFormal(AstNode* formal);

  AstNode*
  buildFunctionExpr(YYLTYPE location, FunctionParts& fp);

  AstNode*
  buildFunctionType(YYLTYPE location, FunctionParts& fp);

  AstNode*
  buildAnonFormal(YYLTYPE location, YYLTYPE locIntent,
                  MaybeIntent intent,
                  AstNode* formalType);

  AstNode*
  buildAnonFormal(YYLTYPE location, PODUniqueString name);

  AstNode*
  buildAnonFormal(YYLTYPE location, AstNode* formalType);

  Formal*
  buildThisFormal(YYLTYPE location,
                  YYLTYPE locIntent,
                  MaybeIntent intent,
                  AstNode* typeExpression,
                  AstNode* initExpression);

  CommentsAndStmt
  buildExternExportFunctionDecl(YYLTYPE location, FunctionParts& fp);

  CommentsAndStmt
  buildRegularFunctionDecl(YYLTYPE location, FunctionParts& fp);

  CommentsAndStmt buildFunctionDecl(YYLTYPE location, FunctionParts& fp);

  ErroneousExpression* checkForFunctionErrors(FunctionParts& fp,
                                              AstNode* retType);

  void enterScopeForFunctionDecl(FunctionParts& fp,
                                 AstNode* retType);
  void exitScopeForFunctionDecl(YYLTYPE bodyLocation, FunctionParts& fp);

  AstNode* buildLetExpr(YYLTYPE location, ParserExprList* decls,
                        AstNode* expr);

  // In certain locations the expression '[a, b]' is interpreted as an
  // array type and not an array literal, e.g., formal types and return
  // types. Those are represented by a BracketLoop node, while an array
  // literal is represented by the Array node.
  AstNode* sanitizeArrayType(YYLTYPE location, AstNode* ast);

  // These different overloads for building bracket loop expressions exist
  // to maintain compatibility between loops and array types. The
  // loop variants have a more normalized form e.g., '[i in 1..100] i',
  // while the array type variants may omit quite a few things in the case
  // that the type is generic, e.g., just '[]'.
  //
  // .chpl: '[]'
  AstNode* buildBracketLoopExpr(YYLTYPE location);

  // .chpl: '[] int'
  AstNode* buildBracketLoopExpr(YYLTYPE location, YYLTYPE locRightBracket,
                                AstNode* bodyExpr);

  // .chpl: '[i in 1..100] i' (multiple indices is an error in this case)
  AstNode* buildBracketLoopExpr(YYLTYPE location,
                                YYLTYPE locIndexExprs,
                                ParserExprList* indexExprs,
                                AstNode* iterandExpr,
                                AstNode* bodyExpr);

  // .chpl: '[i in 1..100] if i % 2 then i'
  AstNode* buildBracketLoopExpr(YYLTYPE location,
                                YYLTYPE locIndexExprs,
                                YYLTYPE locIf,
                                ParserExprList* indexExprs,
                                AstNode* iterandExpr,
                                AstNode* bodyIfCond,
                                AstNode* bodyIfExpr);

  // .chpl: '[a, b, c] int' || '[0..100] doSomething()'
  AstNode* buildBracketLoopExpr(YYLTYPE location,
                                YYLTYPE locIterandExprs,
                                ParserExprList* iterandExprs,
                                AstNode* bodyExpr);

  AstNode* buildGeneralLoopExpr(YYLTYPE locWhole,
                                YYLTYPE locIndex,
                                YYLTYPE locBodyAnchor,
                                PODUniqueString loopType,
                                AstNode* indexExpr,
                                AstNode* iterandExpr,
                                AstNode* blockOrDo);

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

  CommentsAndStmt buildBracketLoopStmt(YYLTYPE locLoop,
                                       YYLTYPE locHeader,
                                       YYLTYPE locIndex,
                                       YYLTYPE locBodyAnchor,
                                       ParserExprList* indexExprs,
                                       AstNode* iterandExpr,
                                       WithClause* withClause,
                                       CommentsAndStmt cs);

  CommentsAndStmt buildBracketLoopStmt(YYLTYPE locLoop,
                                       YYLTYPE locHeader,
                                       YYLTYPE locIterExprs,
                                       YYLTYPE locBodyAnchor,
                                       ParserExprList* iterExprs,
                                       WithClause* withClause,
                                       CommentsAndStmt cs);

  CommentsAndStmt buildGeneralLoopStmt(YYLTYPE locLoop,
                                      YYLTYPE locIndex,
                                      YYLTYPE locHeader,
                                      YYLTYPE locBodyAnchor,
                                      PODUniqueString loopType,
                                      AstNode* indexExpr,
                                      AstNode* iterandExpr,
                                      WithClause* withClause,
                                      BlockOrDo blockOrDo);
                                      // AttributeGroup* attributeGroup);

  CommentsAndStmt buildCoforallLoopStmt(YYLTYPE locCoforall,
                                        YYLTYPE locHeader,
                                        YYLTYPE locIndex,
                                        YYLTYPE locBodyAnchor,
                                        AstNode* indexExpr,
                                        AstNode* iterandExpr,
                                        WithClause* withClause,
                                        BlockOrDo blockOrDo);
                                        // AttributeGroup* attributeGroup);

  CommentsAndStmt buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                       // same as locThenBody if no 'then'
                                       YYLTYPE locThenKw,
                                       YYLTYPE locThenBody,
                                       AstNode* condition,
                                       CommentsAndStmt thenCs);

  CommentsAndStmt buildConditionalStmt(bool usesThenKeyword, YYLTYPE locIf,
                                       // same as locThenBody if no 'then'
                                       YYLTYPE locThenKw,
                                       YYLTYPE locThenBody,
                                       YYLTYPE locElseKw,
                                       YYLTYPE locElseBody,
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
  buildVarOrMultiDeclStmt(YYLTYPE locEverything, AttributeGroup* attributeGroup, ParserExprList* vars);

  TypeDeclParts enterScopeAndBuildTypeDeclParts(YYLTYPE locStart,
                                                YYLTYPE locName,
                                                PODUniqueString name,
                                                asttags::AstTag tag);

  void validateExternTypeDeclParts(YYLTYPE locStart, TypeDeclParts& parts);

  CommentsAndStmt buildAggregateTypeDecl(YYLTYPE location,
                                         YYLTYPE headerLoc,
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

  AstNode* buildReduceIntent(YYLTYPE location, YYLTYPE locOp,
                             PODUniqueString op,
                             AstNode* iterand, YYLTYPE iterandLoc);

  AstNode* buildReduceIntent(YYLTYPE location, YYLTYPE locOp,
                             AstNode* op,
                             AstNode* iterand, YYLTYPE iterandLoc);

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
                      bool hasParensAroundError,
                      YYLTYPE parenLocation);

  CommentsAndStmt buildWhenStmt(YYLTYPE location,
                                YYLTYPE headerLocation,
                                ParserExprList* caseExprs,
                                BlockOrDo blockOrDo);

  CommentsAndStmt buildSelectStmt(YYLTYPE location,
                                  YYLTYPE headerLocation,
                                  owned<AstNode> expr,
                                  ParserExprList* whenStmts);

  CommentsAndStmt
  buildForwardingDecl(YYLTYPE location, owned<AttributeGroup> attributeGroup,
                      owned<AstNode> expr,
                      VisibilityClause::LimitationKind limitationKind,
                      ParserExprList* limitations);

  CommentsAndStmt
  buildForwardingDecl(YYLTYPE location, owned<AttributeGroup> attributeGroup,
                      CommentsAndStmt cs);

  AstNode* buildInterfaceFormal(YYLTYPE location,
                                YYLTYPE locName,
                                PODUniqueString name);

  CommentsAndStmt buildInterfaceStmt(YYLTYPE location,
                                     YYLTYPE headerLoc,
                                     TypeDeclParts parts,
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

  ParserExprList* buildSingleStmtRoutineBody(YYLTYPE location, CommentsAndStmt cs);
};
