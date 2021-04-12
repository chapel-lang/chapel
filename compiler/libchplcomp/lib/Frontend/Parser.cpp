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

#include "chpl/Frontend/Parser.h"

#include "../Util/files.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Comment.h"
#include "chpl/AST/Expr.h"

#include "Parser/bison-chapel.h"
#include "Parser/flex-chapel.h"

#include <cstdlib>
#include <cstring>

#define DEBUG_PARSER 0

namespace chpl {

using namespace ast;

/*owned<Parser> Parser::build(Context* context) {
  return toOwned(new Parser(context));
}*/

static void updateParseResult(ParserContext* parserContext,
                              Parser::ParseResult* result) {

  // Save the top-level exprs
  if (parserContext->topLevelStatements != nullptr) {
    for (Expr* stmt : *parserContext->topLevelStatements) {
      result->topLevelExprs.push_back(toOwned(stmt));
    }
  }
  // Save any remaining top-level comments
  if (parserContext->comments != nullptr) {
    for (ParserComment parserComment : *parserContext->comments) {
      result->topLevelExprs.push_back(
                           Comment::build(parserContext->builder,
                                          parserComment.comment.allocatedData,
                                          parserComment.comment.size));
    }
  }


  Context* aCtx = parserContext->astContext;
  // Save the parse errors
  for (ParserError & parserError : parserContext->errors) {
    // Need to convert the error to a regular ErrorMessage
    UniqueString upath = UniqueString::build(aCtx, parserContext->filename);
    Location loc(upath,
                 parserError.location.first_line,
                 parserError.location.first_column,
                 parserError.location.last_line,
                 parserError.location.last_column);

    result->parseErrors.push_back(ErrorMessage(loc, parserError.message));
  }

  // TODO: generate locations map
}


Parser::ParseResult Parser::parseFile(const char* path) {
  Parser::ParseResult ret;

  ErrorMessage fileError;
  FILE* fp = openfile(path, "r", fileError);
  if (fp == NULL) {
    ret.parseErrors.push_back(fileError);
    return ret;
  }

  // Otherwise, we have successfully openned the file.

  // Set the (global) parser debug state
  if (DEBUG_PARSER)
    yydebug = DEBUG_PARSER;

  // State for the lexer
  int           lexerStatus  = 100;

  // State for the parser
  yypstate*     parser       = yypstate_new();
  int           parserStatus = YYPUSH_MORE;
  YYLTYPE       yylloc;
  Builder       builder(this->context());
  ParserContext parserContext(path, &builder);

  yylloc.first_line             = 1;
  yylloc.first_column           = 1;
  yylloc.last_line              = 1;
  yylloc.last_column            = 1;

  yylex_init_extra(&parserContext, &parserContext.scanner);

  yyset_in(fp, parserContext.scanner);

  while (lexerStatus != 0 && parserStatus == YYPUSH_MORE) {
    YYSTYPE yylval;

    lexerStatus = yylex(&yylval, &yylloc, parserContext.scanner);

    if        (lexerStatus >= 0) {
      parserStatus          = yypush_parse(parser,
                                           lexerStatus,
                                           &yylval,
                                           &yylloc,
                                           &parserContext);

    } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
      // comment should already be noted in processBlockComment
    } else if (lexerStatus == YYLEX_SINGLE_LINE_COMMENT) {
      // comment should already be noted in processSingleLineComment
    }
  }

  // Cleanup after the parser
  yypstate_delete(parser);

  // Cleanup after the lexer
  yylex_destroy(parserContext.scanner);

  if (closefile(fp, path, fileError)) {
    ret.parseErrors.push_back(fileError);
    return ret;
  }

  // Store errors and collected top-level exprs in the result
  updateParseResult(&parserContext, &ret);
  return ret;
}


Parser::ParseResult Parser::parseString(const char* path, const char* str) {
  // Set the (global) parser debug state
  if (DEBUG_PARSER)
    yydebug = DEBUG_PARSER;

  // State for the lexer
  YY_BUFFER_STATE handle       =   0;
  int             lexerStatus  = 100;
  YYLTYPE         yylloc;

  // State for the parser
  yypstate*       parser       = yypstate_new();
  int             parserStatus = YYPUSH_MORE;
  Builder       builder(this->context());
  ParserContext parserContext(path, &builder);

  yylex_init_extra(&parserContext, &parserContext.scanner);

  handle              = yy_scan_string(str, parserContext.scanner);

  yylloc.first_line   = 1;
  yylloc.first_column = 1;
  yylloc.last_line    = 1;
  yylloc.last_column  = 1;

  while (lexerStatus != 0 && parserStatus == YYPUSH_MORE) {
    YYSTYPE yylval;

    lexerStatus  = yylex(&yylval, &yylloc, parserContext.scanner);

    if (lexerStatus >= 0) {
      parserStatus          = yypush_parse(parser,
                                           lexerStatus,
                                           &yylval,
                                           &yylloc,
                                           &parserContext);

    } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
      // comment should already be noted in processBlockComment
    } else if (lexerStatus == YYLEX_SINGLE_LINE_COMMENT) {
      // comment should already be noted in processSingleLineComment
    }
  }

  // Cleanup after the parser
  yypstate_delete(parser);

  // Cleanup after the lexer
  yy_delete_buffer(handle, parserContext.scanner);
  yylex_destroy(parserContext.scanner);

  Parser::ParseResult ret;
  // Store errors and collected top-level exprs in the result
  updateParseResult(&parserContext, &ret);
  return ret;
}

} // namespace chpl
