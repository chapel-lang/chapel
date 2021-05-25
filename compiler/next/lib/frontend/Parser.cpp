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

#include "chpl/frontend/Parser.h"

#include "../util/files.h"

#include "chpl/queries/ErrorMessage.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Expression.h"

#include "Parser/bison-chapel.h"
#include "Parser/flex-chapel.h"

#include <cstdlib>
#include <cstring>

#define DEBUG_PARSER 0

namespace chpl {
namespace frontend {


using namespace chpl::uast;

Parser::Parser(Context* context)
  : context_(context) {
}

owned<Parser> Parser::build(Context* context) {
  return toOwned(new Parser(context));
}

static void updateParseResult(ParserContext* parserContext) {

  Builder* builder = parserContext->builder;

  // Save the top-level exprs
  if (parserContext->topLevelStatements != nullptr) {
    for (Expression* stmt : *parserContext->topLevelStatements) {
      builder->addToplevelExpression(toOwned(stmt));
    }
    delete parserContext->topLevelStatements;
  }
  // Save any remaining top-level comments
  if (parserContext->comments != nullptr) {
    for (ParserComment parserComment : *parserContext->comments) {
      builder->addToplevelExpression(toOwned(parserComment.comment));
    }
    delete parserContext->comments;
  }

  // Save the parse errors
  for (ParserError & parserError : parserContext->errors) {
    // Need to convert the error to a regular ErrorMessage
    Location loc = parserContext->convertLocation(parserError.location);
    builder->addError(ErrorMessage(loc, parserError.message));
  }
}


Builder::Result Parser::parseFile(const char* path) {
  auto builder = Builder::build(this->context(), path);
  ErrorMessage fileError;

  FILE* fp = openfile(path, "r", fileError);
  if (fp == NULL) {
    builder->addError(fileError);
    return builder->result();
  }

  // Otherwise, we have successfully opened the file.

  // Set the (global) parser debug state
  if (DEBUG_PARSER)
    yychpl_debug = DEBUG_PARSER;

  // State for the lexer
  int           lexerStatus  = 100;

  // State for the parser
  yychpl_pstate* parser = yychpl_pstate_new();
  int           parserStatus = YYPUSH_MORE;
  YYLTYPE       yylloc;
  ParserContext parserContext(path, builder.get());

  yylloc.first_line             = 1;
  yylloc.first_column           = 1;
  yylloc.last_line              = 1;
  yylloc.last_column            = 1;

  yychpl_lex_init_extra(&parserContext, &parserContext.scanner);

  yychpl_set_in(fp, parserContext.scanner);

  while (parserStatus == YYPUSH_MORE) {
    YYSTYPE yylval;

    lexerStatus = yychpl_lex(&yylval, &yylloc, parserContext.scanner);

    if        (lexerStatus >= 0) {
      parserStatus          = yychpl_push_parse(parser,
                                                lexerStatus,
                                                &yylval,
                                                &yylloc,
                                                &parserContext);

    } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
      // comment should already be noted in processBlockComment
    } else if (lexerStatus == YYLEX_SINGLE_LINE_COMMENT) {
      // comment should already be noted in processSingleLineComment
    }

    if (lexerStatus == 0 || parserContext.atEOF)
      break;
  }

  // Cleanup after the parser
  yychpl_pstate_delete(parser);

  // Cleanup after the lexer
  yychpl_lex_destroy(parserContext.scanner);

  if (closefile(fp, path, fileError)) {
    builder->addError(fileError);
  }

  updateParseResult(&parserContext);

  // compute the result from the builder
  return builder->result();
}


Builder::Result Parser::parseString(const char* path, const char* str) {
  auto builder = Builder::build(this->context(), path);

  // Set the (global) parser debug state
  if (DEBUG_PARSER)
    yychpl_debug = DEBUG_PARSER;

  // State for the lexer
  YY_BUFFER_STATE handle       =   0;
  int             lexerStatus  = 100;
  YYLTYPE         yylloc;

  // State for the parser
  yychpl_pstate* parser = yychpl_pstate_new();
  int           parserStatus = YYPUSH_MORE;
  ParserContext parserContext(path, builder.get());

  yychpl_lex_init_extra(&parserContext, &parserContext.scanner);

  handle = yychpl__scan_string(str, parserContext.scanner);

  yylloc.first_line   = 1;
  yylloc.first_column = 1;
  yylloc.last_line    = 1;
  yylloc.last_column  = 1;

  while (parserStatus == YYPUSH_MORE) {
    YYSTYPE yylval;

    lexerStatus  = yychpl_lex(&yylval, &yylloc, parserContext.scanner);

    if (lexerStatus >= 0) {
      parserStatus          = yychpl_push_parse(parser,
                                                lexerStatus,
                                                &yylval,
                                                &yylloc,
                                                &parserContext);

    } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
      // comment should already be noted in processBlockComment
    } else if (lexerStatus == YYLEX_SINGLE_LINE_COMMENT) {
      // comment should already be noted in processSingleLineComment
    }

    if (lexerStatus == 0 || parserContext.atEOF)
      break;
  }

  // Cleanup after the parser
  yychpl_pstate_delete(parser);

  // Cleanup after the lexer
  yychpl__delete_buffer(handle, parserContext.scanner);
  yychpl_lex_destroy(parserContext.scanner);

  updateParseResult(&parserContext);

  return builder->result();
}


} // namespace frontend
} // namespace chpl
