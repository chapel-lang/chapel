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
  // emit errors here and create a stand-in ErroneousExpr AST
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
  SizedStr comment;
};

struct ParserContext {
  yyscan_t scanner;
  const char* filename;
  Builder* builder;
  Context* astContext;

  // TODO: move these to the astContext
  std::vector<UniqueString> currentModuleStack;
  std::vector<UniqueString> currentFunctionStack;

  ParserExprList* topLevelStatements;
  std::vector<ParserError> errors;

  // comments are gathered here
  // Consider a 'proc' declaration. Comments preceding it should be consumed
  // when the decl_stmt starts. Then, after the decl_stmt is created, comments
  // accumulated here should be cleared (since they must have come from inside
  // the statement and should not apply later).
  std::vector<ParserComment>* comments;

  VisibilityTag visibility;

  ParserContext(const char* filename, Builder* builder)
  {
    this->scanner            = nullptr;
    this->filename           = filename;
    this->builder            = builder;
    this->topLevelStatements = nullptr;
    this->comments           = nullptr;
    this->visibility         = VisibilityTag_DEFAULT;
  }

  void noteComment(YYLTYPE loc, const char* data, long size);
  std::vector<ParserComment>* gatherComments(YYLTYPE location);
  void clearComments();
  ParserExprList* makeList();
  ParserExprList* makeList(ParserExprList* lst);
  ParserExprList* makeList(Expr* e);
  ParserExprList* makeList(CommentsAndStmt cs);
  void appendList(ParserExprList* dst, ParserExprList* lst);
  void appendList(ParserExprList* dst, Expr* e);
  void appendList(ParserExprList* dst, CommentsAndStmt cs);
  void appendComments(CommentsAndStmt*cs, std::vector<ParserComment>* comments);

  // clears the inner comments that should have already been captured
  // to handle things like this
  //     { /* doc comment } proc myproc()
  CommentsAndStmt finishStmt(CommentsAndStmt cs);
  CommentsAndStmt finishStmt(Expr* e);

  // TODO: move these to astContext

  // These adjust for the IDs
  // and call enterStmt / exitStmt.
  //ParserExprList* enterModule(YYLTYPE loc, const char* name, Expr* decl);
  //ParserExprList* exitModule(ParserExprList* decl, ParserExprList* body);
  //ParserExprList* enterFunction(YYLTYPE loc, const char* name, Expr* decl);
  //ParserExprList* exitFunction(ParserExprList* decl, ParserExprList* body);

  // This should consume the comments that occur before
  // and return them. (Including looking at source locations).
  // If there is any argument, it will
  // be also appended to the returned list.
  ParserExprList* enterStmt(YYLTYPE location, ParserExprList* lst);
  ParserExprList* enterStmt(YYLTYPE location, Expr* e);
  ParserExprList* enterStmt(YYLTYPE location);

  // These should clear the comments (since there might be some inside the stmt)
  ParserExprList* exitStmt(ParserExprList* lst);
  ParserExprList* exitStmt(Expr* e);

  // Do we really need these?
  /*
  int         captureTokens; // no, new AST meant to be more faithful to src;
                             // if we want source code we can read input again.
  std::string captureString;
  bool        parsingPrivate; // no, create config var and
                              // replace its initialization with something
                              // provided and separately parsed.
   */
};
