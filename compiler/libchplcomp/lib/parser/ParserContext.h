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
  ParserContext(Builder* astBuilder)
  {
    scanner       = nullptr;
    builder       = astBuilder;
    astContext    = astBuilder->context();
  }

  yyscan_t scanner;
  Builder* builder;
  Context* astContext;

  std::vector<UniqueString> currentModuleStack;
  std::vector<UniqueString> currentFunctionStack;
  std::vector<Expr*>* topLevelStatements;
  std::vector<ParserError> errors;

  // comments are gathered here
  // Consider a 'proc' declaration. Comments preceding it should be consumed
  // when the decl_stmt starts. Then, after the decl_stmt is created, comments
  // accumulated here should be cleared (since they must have come from inside
  // the statement).
  std::vector<ParserComment> comments;

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
