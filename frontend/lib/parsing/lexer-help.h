/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

#include "chpl/framework/ErrorBase.h"
#include "chpl/parsing/parser-error.h"

#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>

namespace chpl {

static int   getNextYYChar(yyscan_t scanner);

static void updateLocation(YYLTYPE* loc, int nLines, int nCols) {
  loc->first_line = loc->last_line;
  loc->first_column = loc->last_column;
  loc->last_line = loc->first_line + nLines;
  int startColumn = (nLines == 0) ? (loc->first_column) : (1);
  loc->last_column = startColumn + nCols;
}

static YYLTYPE getLocation(yyscan_t scanner, int nLines, int nCols,
                        bool moveToEnd = false) {
  ParserContext* pContext = yyget_extra(scanner);
  YYLTYPE loc = *yyget_lloc(scanner);
  updateLocation(&loc, nLines, nCols);
  if (moveToEnd) loc = pContext->makeLocationAtLast(loc);
  return loc;
}

int processNewline(yyscan_t scanner) {
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 1, 0);
  ParserContext* context = yyget_extra(scanner);
  if (context->parseStats)
    context->parseStats->countNewline();
  return YYLEX_NEWLINE;
}

static void syntax(yyscan_t scanner, int nLines, int nCols,
                   const char* fmt, ...) {
  ParserContext* pContext = yychpl_get_extra(scanner);
  YYLTYPE flexLoc = getLocation(scanner, nLines, nCols);
  Location loc = pContext->convertLocation(flexLoc);
  va_list args;
  va_start(args, fmt);
  auto error = GeneralError::vbuild(ErrorBase::SYNTAX, loc, fmt, args);
  pContext->report(flexLoc, std::move(error));
  va_end(args);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

// specialInitialChar might be:
//  * '?' for a query identifier as in 'proc foo(arg: ?t)'
//  * '@' for an attribute identifier as in @chpldoc.nodoc
//  * everything else is considered a normal identifier
static int processIdentifier(yyscan_t scanner, char specialInitialChar) {
  YYSTYPE* val = yyget_lval(scanner);
  int tokenType = TIDENT;
  if (specialInitialChar == '?')      tokenType = TQUERIEDIDENT;
  else if (specialInitialChar == '@') tokenType = TATTRIBUTEIDENT;
  int retval = processToken(scanner, tokenType);
  // note: processToken calls updateLocation.

  const char* pch = yyget_text(scanner);
  ParserContext* context = yyget_extra(scanner);
  val->uniqueStr = PODUniqueString::get(context->context(), pch);

  return retval;
}

static int processToken(yyscan_t scanner, int t) {
  YYSTYPE* val = yyget_lval(scanner);

  const char* pch = yyget_text(scanner);

  ParserContext* context = yyget_extra(scanner);
  if (context->parseStats)
    context->parseStats->countToken(pch);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));


  val->uniqueStr = PODUniqueString::get(context->context(), pch);

  // If the stack has a value then we must be in externmode.
  // Return to INITIAL
  if (yy_has_state(scanner) == true) {
    yy_pop_state(scanner);
  }

  return t;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static std::string eatStringLiteral(yyscan_t scanner,
                                    const char* startChar,
                                    int nColOffset,
                                    bool& isErroneousOut);
static std::string eatTripleStringLiteral(yyscan_t scanner,
                                          const char* startChar,
                                          int nColOffset,
                                          bool& isErroneousOut);

static int processStringLiteral(yyscan_t scanner,
                                const char* startChar,
                                int type) {
  // figure out the location offset for the string start (e.g. b" )
  const char* pch = yyget_text(scanner);
  int nColOffset = (int) strlen(pch);
  YYLTYPE* loc = yyget_lloc(scanner);

  bool erroneous = false;
  std::string value = eatStringLiteral(scanner, startChar, nColOffset, erroneous);

  ParserContext* context = yyget_extra(scanner);
  if (context->parseStats)
    context->parseStats->countToken(startChar, value.c_str(), startChar);

  StringLiteral::QuoteStyle quotes = StringLiteral::SINGLE;
  if (startChar && startChar[0] == '"')
    quotes = StringLiteral::DOUBLE;

  AstNode* lit = nullptr;
  if (erroneous) {
    lit = ErroneousExpression::build(context->builder,
                                     context->convertLocation(*loc)).release();
  } else if (type == STRINGLITERAL) {
    lit = StringLiteral::build(context->builder,
                               context->convertLocation(*loc),
                               std::move(value),
                               quotes).release();
  } else if (type == BYTESLITERAL) {
    lit = BytesLiteral::build(context->builder,
                              context->convertLocation(*loc),
                              std::move(value),
                              quotes).release();
  } else if (type == CSTRINGLITERAL) {
    lit = CStringLiteral::build(context->builder,
                                context->convertLocation(*loc),
                                std::move(value),
                                quotes).release();
  } else {
    CHPL_ASSERT(false && "unknown type in processStringLiteral");
  }

  YYSTYPE* val = yyget_lval(scanner);
  val->expr = lit;

  return type;
}

static int processTripleStringLiteral(yyscan_t scanner,
                                      const char* startChar,
                                      int type) {
  // figure out the location offset for the string start (e.g. b" )
  const char* pch = yyget_text(scanner);
  int nColOffset = (int) strlen(pch);
  YYLTYPE* loc = yyget_lloc(scanner);

  bool erroneous = false;
  std::string value = eatTripleStringLiteral(scanner, startChar, nColOffset, erroneous);

  ParserContext* context = yyget_extra(scanner);
  if (context->parseStats)
    context->parseStats->countToken(startChar, value.c_str(), startChar);

  StringLiteral::QuoteStyle quotes = StringLiteral::TRIPLE_SINGLE;
  if (startChar && startChar[0] == '"')
    quotes = StringLiteral::TRIPLE_DOUBLE;

  AstNode* lit = nullptr;

  if (erroneous) {
    lit = ErroneousExpression::build(context->builder,
                                     context->convertLocation(*loc)).release();
  } else if (type == STRINGLITERAL) {
    lit = StringLiteral::build(context->builder,
                               context->convertLocation(*loc),
                               std::move(value),
                               quotes).release();
  } else if (type == BYTESLITERAL) {
    lit = BytesLiteral::build(context->builder,
                              context->convertLocation(*loc),
                              std::move(value),
                              quotes).release();
  } else if (type == CSTRINGLITERAL) {
    lit = CStringLiteral::build(context->builder,
                                context->convertLocation(*loc),
                                std::move(value),
                                quotes).release();
  } else {
    CHPL_ASSERT(false && "unknown type in processStringLiteral");
  }

  YYSTYPE* val = yyget_lval(scanner);
  val->expr = lit;

  return type;
}

static
char simpleEscape(int c) {
  if (c == '\'')
    return '\'';
  if (c == '"')
    return '"';
  if (c == '?')
    return '?';
  if (c == '\\')
    return '\\';
  if (c == 'a')
    return '\a';
  if (c == 'b')
    return '\b';
  if (c == 'f')
    return '\f';
  if (c == 'n')
    return '\n';
  if (c == 'r')
    return '\r';
  if (c == 't')
    return '\t';
  if (c == 'v')
    return '\v';
  if (c == '\n')
    // to allow newline followed by \ at the end of the line
    return '\n';

  return '\0';
}

static inline SizedStr makeSizedStr(const char* allocatedData, long size) {
  SizedStr ret = {allocatedData, size};
  return ret;
}

static std::string eatStringLiteral(yyscan_t scanner,
                                    const char* startChar,
                                    int nColOffset,
                                    bool& isErroneousOut) {
  YYLTYPE* loc = yyget_lloc(scanner);
  const char startCh = *startChar;
  int        c       = 0;
  int nLines = 0;
  int nCols = nColOffset;
  std::string s;

  isErroneousOut = false;
  c = getNextYYChar(scanner);
  nCols++; // if it's newline, this will immediately be reset

  while (c != startCh && c != 0) {
    if (c == '\n') {
      // TODO: string literals with newline after backslash
      // are not documented in the spec
      syntax(scanner, nLines, nCols,
          "end-of-line in a string literal without a preceding backslash.");
      isErroneousOut = true;
      s += c;
      nLines++;
      nCols = 0;
    } else if (c == '\\') {
      c = getNextYYChar(scanner);
      nCols++;

      // account for newlines after \ at the end
      if (c == '\n') {
        nLines++;
        nCols = 0;
      }

      // handle the escapes described in the spec
      //   \' \" \? \\ \a \b \f \n \r \t \v and \xDIGITS
      char esc = simpleEscape(c);
      if (esc != 0) {
        s += esc;
      } else if (c == 'x') {
        // Read hexadecimal digits until we run out or we have
        // reached 2 digits.
        char buf[3] = {'\0', '\0', '\0'};
        bool foundNonHex = false;
        bool foundHex = false;
        for (int i = 0; i < 2 && c != 0; i++) {
          c = getNextYYChar(scanner);
          nCols++;
          // TODO the exact behavior here is not documented in the spec
          if (('0' <= c && c <= '9') ||
              ('A' <= c && c <= 'F') ||
              ('a' <= c && c <= 'f')) {
            buf[i] = c;
            foundHex = true;
          } else {
            foundNonHex = true;
            break;
          }
        }

        long hexChar = strtol(buf, NULL, 16);

        CHPL_ASSERT((hexChar != LONG_MIN && hexChar != LONG_MAX) &&
               "overflow/underflow shouldn't be possible in the allowed number "
               "of digits");
        if (!foundHex) {
          syntax(scanner, nLines, nCols,
                 "non-hexadecimal character follows \\x.");
          isErroneousOut = true;
        } else if (0 <= hexChar && hexChar <= 255) {
          // append the character
          char cc = (char)hexChar;
          s += cc;
        } else {
          syntax(scanner, nLines, nCols,
                 "unknown problem while reading \\x escape.");
          isErroneousOut = true;
        }

        if (foundNonHex)
          continue; // need to process c as the next character

      } else if (c == 'u' || c == 'U') {
        syntax(scanner, nLines, nCols,
               "universal character name not yet supported in string literal.");
        s += "\\u"; // this is a dummy value
        isErroneousOut = true;
      } else if ('0' <= c && c <= '7' ) {
        syntax(scanner, nLines, nCols,
               "octal escape not supported in string literal.");
        s += "\\";
        s += c; // a dummy value
        isErroneousOut = true;
      } else if (c == 0) {
        // we've reached EOF
        s += "\\";
        break; // EOF reached, so stop
      } else {
        syntax(scanner, nLines, nCols,
               "unexpected string escape: '\\%c'.", (char) c);
        isErroneousOut = true;
      }
    } else {
      s += c;
    }
    c = getNextYYChar(scanner);
    nCols++;
  } /* eat up string */

  if (c == 0) {
    CHPL_LEXER_REPORT(scanner, nLines, nCols, StringLiteralEOF, startCh,
                      /* startCharCount */ 1);
    isErroneousOut = true;
  }

  // update the location
  updateLocation(loc, nLines, nCols);

  return s;
}

static std::string eatTripleStringLiteral(yyscan_t scanner,
                                          const char* startChar,
                                          int nColOffset,
                                          bool& isErroneousOut) {
  YYLTYPE* loc       = yyget_lloc(scanner);
  const char startCh = *startChar;
  int startChCount   = 0;
  int c              = 0;
  int nLines = 0;
  int nCols = nColOffset;
  std::string s;

  isErroneousOut = false;

  while (true) {
    c = getNextYYChar(scanner);
    if (c == 0) {
      break;
    }

    if (c == '\n') {
      nLines++;
      nCols = 0;
    } else {
      nCols++;
    }

    if (c == startCh) {
      startChCount++;
      if (startChCount == 3) {
        break;
      }
    } else {
      startChCount = 0;
    }

    s += c;
  } /* eat up string */

  if (c == 0) {
    CHPL_LEXER_REPORT(scanner, nLines, nCols, StringLiteralEOF, startCh,
                      startChCount);
    isErroneousOut = true;
  }

  // update the location
  updateLocation(loc, nLines, nCols);

  size_t size = s.size();
  // Remove two quotes from the end of the string that are
  // actually part of the string closing token.
  for (int i = 0; i < 2; i++) {
    if (s[size-1] == startCh) size--;
  }
  s.resize(size);

  return s;
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processExtern(yyscan_t scanner) {
  const char* pch = yyget_text(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));

  ParserContext* context = yyget_extra(scanner);
  if (context->parseStats)
    context->parseStats->countToken(pch);
  // Push a state to record that "extern" has been seen
  yy_push_state(externmode, scanner);

  return TEXTERN;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static SizedStr eatExternCode(yyscan_t scanner);

// When the lexer calls this function, it has already consumed the first '{'
static int processExternCode(yyscan_t scanner) {
  const char* pch = yyget_text(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));

  YYSTYPE* val = yyget_lval(scanner);
  val->sizedStr = eatExternCode(scanner);

  ParserContext* context = yyget_extra(scanner);
  if (context->parseStats)
    context->parseStats->countToken(pch);
  return EXTERNCODE;
}

static SizedStr eatExternCode(yyscan_t scanner) {
  const int in_code                          = 0;
  const int in_single_quote                  = 1;
  const int in_single_quote_backslash        = 2;
  const int in_double_quote                  = 3;
  const int in_double_quote_backslash        = 4;
  const int in_single_line_comment           = 5;
  const int in_single_line_comment_backslash = 6;
  const int in_multi_line_comment            = 7;

  YYLTYPE*  loc                              = yyget_lloc(scanner);

  int       depth                            = 1;
  int       c                                = 0;
  int       lastc                            = 0;
  int       state                            = 0;

  ParserContext* context = yyget_extra(scanner);

  int nLines = 0;
  int nCols = 0;
  std::string s;

  const char* filename = context->filename.c_str();

  if (filename != NULL && filename[0] != '\0') {
    // First, store the line information.
    s += "#line ";
    s += std::to_string(loc->first_line + nLines);
    s += " \"";
    s += filename;
    s += "\" \n";
  }

  // Now, append the C code until we get to a }.
  while (depth > 0) {
    lastc = c;
    c     = getNextYYChar(scanner);

    if (c == 0) {
      switch (state) {
        case in_code:
          // there was no match to the {
          CHPL_LEXER_REPORT(scanner, nLines, nCols, ExternUnclosedPair, "}");
          break;

        case in_single_quote:
        case in_single_quote_backslash:
          CHPL_LEXER_REPORT(scanner, nLines, nCols, ExternUnclosedPair, "'");
          break;

        case in_double_quote:
        case in_double_quote_backslash:
          CHPL_LEXER_REPORT(scanner, nLines, nCols, ExternUnclosedPair, "\"");
          break;

        case in_single_line_comment:
          syntax(scanner, nLines, nCols,
                 "missing newline after // comment in extern block.");
          break;

        case in_multi_line_comment:
          CHPL_LEXER_REPORT(scanner, nLines, nCols, ExternUnclosedPair, "*/");
          break;
      }
      break;
    }

    s += c;

    if (c == '\n') {
      nCols = 0;
      nLines++;
    }

    // Now update state (are we in a comment? a string?)
    switch (state) {
      case in_code:
        if (c == '\'')
          state = in_single_quote;

        else if (c == '"')
          state = in_double_quote;

        else if (lastc == '/' && c == '/')
          state = in_single_line_comment;

        else if (lastc == '/' && c == '*')
          state = in_multi_line_comment;

        else if (c == '{' )
          depth++;

        else if (c == '}' )
          depth--;

        break;

      case in_single_quote:
        if (c == '\\')
          state = in_single_quote_backslash;

        else if (c == '\'')
          state = in_code;

        break;

      case in_single_quote_backslash:
        state = in_single_quote;
        break;

      case in_double_quote:
        if (c == '\\')
          state = in_double_quote_backslash;

        else if (c == '"')
          state = in_code;

        break;

      case in_double_quote_backslash:
        state = in_double_quote;
        break;

      case in_single_line_comment:
        if (c == '\n')
          state = in_code;
        break;

      case in_single_line_comment_backslash:
        if (c == ' ' || c == '\t' || c == '\n')
          state = in_single_line_comment_backslash;

        else
          state = in_single_line_comment;

        break;

      case in_multi_line_comment:
        if (lastc == '*' && c == '/')
          state = in_code;
        break;
    }
  }

  // update the location
  updateLocation(loc, nLines, nCols);

  //save the C String
  //eliminate the final '}'
  long size = s.size();
  if (s[size-1] == '}') size--;
  char* buf = (char*) malloc(size+1);
  memcpy(buf, s.c_str(), size);
  buf[size] = '\0';

  return makeSizedStr(buf, size);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processWhitespace(yyscan_t scanner) {
  const char* pch = yyget_text(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processSingleLineComment(yyscan_t scanner) {
  int      c   = 0;

  // start with the comment introduction
  std::string s;
  s += yyget_text(scanner);

  ParserContext* context = yyget_extra(scanner);
  if (context->parseStats)
    context->parseStats->countCommentLine();

  // Read until the end of the line
  while ((c = getNextYYChar(scanner)) != '\n' && c != 0) {
    s += c;
  }

  YYLTYPE* loc = yyget_lloc(scanner);
  int nLines = 1;
  int nCols = 0;
  if (c == 0) {
    nCols = s.size();
    nLines = 0;
  }
  updateLocation(loc, nLines, nCols);

  // allocate the value to return
  long size = s.size();
  char* buf = (char*) malloc(size+1);
  memcpy(buf, s.c_str(), size+1);

 if (context->parseStats) {
    context->parseStats->countSingleLineComment(buf);
    if (c != 0)
      processNewline(scanner);
 }
  context->noteComment(*loc, buf, size);

  return YYLEX_SINGLE_LINE_COMMENT;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processBlockComment(yyscan_t scanner) {
  YYLTYPE*    loc       = yyget_lloc(scanner);

  int nestedStartLine = -1;
  int nestedStartCol = -1;
  int startLine = loc->first_line;
  int startCol = loc->first_column;

  int         c            = 0;
  int         lastc        = 0;
  int         depth        = 1;

  int nLines = 0;
  int nCols = 0;
  ParserContext* context = yyget_extra(scanner);
  // start with the comment introduction
  std::string s;
  std::string wholeComment;
  s += yyget_text(scanner);
  nCols += s.size();
  if (context->parseStats)
    context->parseStats->countCommentLine();
  while (depth > 0) {
    int lastlastc = lastc;

    lastc = c;
    c     = getNextYYChar(scanner);

    if (c == '\n') {
      if (context->parseStats) {
        context->parseStats->countMultiLineComment(s.c_str());
        processNewline(scanner);
        context->parseStats->countCommentLine();
      }
      nCols = 0;
      nLines++;
      wholeComment += s;
      wholeComment += '\n';
      s.clear();
    } else {
      nCols++;
      s += c;
    }

    if (lastc == '*' && c == '/' && lastlastc != '/') { // close comment
      depth--;
    } else if (lastc == '/' && c == '*') { // start nested
      depth++;
      // keep track of the start of the last nested comment
      nestedStartLine = nLines;
      nestedStartCol = nCols;
    } else if (c == 0) {
      // report error with location of EOF, as well as unterminated comment
      // start and nested comment (if present) start
      ParserContext* pContext = yyget_extra(scanner);
      YYLTYPE loc = *yyget_lloc(scanner);
      updateLocation(&loc, startLine, startCol);
      const Location startLoc = pContext->convertLocation(loc);
      Location nestedLoc = Location();
      if (nestedStartLine >= 0) {
        loc = *yyget_lloc(scanner);
        updateLocation(&loc, nestedStartLine, nestedStartCol);
        loc = pContext->makeLocationAtLast(loc);
        nestedLoc = pContext->convertLocation(loc);
      }
      CHPL_LEXER_REPORT_END(scanner, nLines, nCols, CommentEOF, startLoc,
                            nestedLoc);
      break;
    }
  }

  updateLocation(loc, nLines, nCols);

  wholeComment += s;
  // allocate the value to return
  long size = wholeComment.size();
  char* buf = (char*) malloc(size+1);
  memcpy(buf, wholeComment.c_str(), size+1);

  context->noteComment(*loc, buf, size);
  if (context->parseStats)
    context->parseStats->countMultiLineComment(s.c_str());

  return YYLEX_BLOCK_COMMENT;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processInvalidToken(yyscan_t scanner) {
  std::string pch = std::string(yyget_text(scanner));
  syntax(scanner, 0, pch.length(), "invalid token: %s.", pch.c_str());
}

static int getNextYYChar(yyscan_t scanner) {
  int retval = yyinput(scanner);

  if (retval == EOF) {
    retval = 0;
  }

  if (retval == 0) {
    ParserContext* context = yyget_extra(scanner);
    context->atEOF = true;
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static bool yy_has_state(yyscan_t yyscanner)
{
  // This is only to suppress a compiler warning
  (void) yy_top_state;

  struct yyguts_t * yyg = (struct yyguts_t*) yyscanner;

  return yyg->yy_start_stack_ptr > 0;
}

} // end namespace chpl
