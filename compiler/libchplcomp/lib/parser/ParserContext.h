// This is included at the right time in the generated bison .h/.cpp
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
  const char* message;
  ParserError(YYLTYPE location, const char* message)
    : location(location), message(message) { }
};

struct ParserComment {
  YYLTYPE location;
  UniqueString comment;
};

struct ParserContext {
  yyscan_t scanner;
  Builder* builder;
  Context* astContext;

  // TODO: move these to the astContext
  std::vector<UniqueString> currentModuleStack;
  std::vector<UniqueString> currentFunctionStack;

  ExprList* topLevelStatements;
  std::vector<ParserError> errors;

  // comments are gathered here
  // Consider a 'proc' declaration. Comments preceding it should be consumed
  // when the decl_stmt starts. Then, after the decl_stmt is created, comments
  // accumulated here should be cleared (since they must have come from inside
  // the statement).
  std::vector<ParserComment> comments;

  ParserContext(Builder* astBuilder)
  {
    scanner       = nullptr;
    builder       = astBuilder;
    astContext    = astBuilder->context();
  }

  void raiseParseError(YYLTYPE location, const char* message);

  // TODO: move these to astContext

  // These adjust for the IDs
  // and call enterStmt / exitStmt.
  //ExprList* enterModule(YYLTYPE loc, const char* name, Expr* decl);
  //ExprList* exitModule(ExprList* decl, ExprList* body);
  //ExprList* enterFunction(YYLTYPE loc, const char* name, Expr* decl);
  //ExprList* exitFunction(ExprList* decl, ExprList* body);

  // This should consume the comments that occur before
  // and return them. (Including looking at source locations).
  // If there is any argument, it will
  // be also appended to the returned list.
  ExprList* enterStmt(YYLTYPE location, ExprList* lst);
  ExprList* enterStmt(YYLTYPE location, Expr* e);
  ExprList* enterStmt(YYLTYPE location);

  // These should clear the comments (since there might be some inside the stmt)
  ExprList* exitStmt(ExprList* lst);
  ExprList* exitStmt(Expr* e);

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
