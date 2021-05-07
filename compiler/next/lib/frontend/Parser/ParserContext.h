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
  Sym::Visibility visibility;
  Variable::Tag varDeclTag;

  ParserContext(const char* filename, Builder* builder)
  {
    auto uniqueFilename = UniqueString::build(builder->context(), filename);

    this->scanner            = nullptr;
    this->filename           = uniqueFilename;
    this->builder            = builder;
    this->topLevelStatements = nullptr;
    this->comments           = nullptr;
    this->visibility         = Sym::VISIBILITY_DEFAULT;
    this->varDeclTag         = Variable::VAR;
  }

  Context* context() { return builder->context(); }

  void noteComment(YYLTYPE loc, const char* data, long size);
  std::vector<ParserComment>* gatherComments(YYLTYPE location);
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
  ParserExprList* blockToParserExprList(YYLTYPE rbrLoc, ParserExprList* body);

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
