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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

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
  int startColumn = (nLines==0)?(loc->first_column):(1);
  loc->last_column = startColumn + nCols;
}

int processNewline(yyscan_t scanner) {
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 1, 0);
  return YYLEX_NEWLINE;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processIdentifier(yyscan_t scanner, bool queried) {
  YYSTYPE* val = yyget_lval(scanner);
  int retval = processToken(scanner, queried ? TQUERIEDIDENT : TIDENT);
  // note: processToken calls updateLocation.

  const char* pch = yyget_text(scanner);
  ParserContext* context = yyget_extra(scanner);
  val->uniqueStr = PODUniqueString::build(context->context(), pch);

  return retval;
}

static int processToken(yyscan_t scanner, int t) {
  YYSTYPE* val = yyget_lval(scanner);

  const char* pch = yyget_text(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));

  ParserContext* context = yyget_extra(scanner);
  val->uniqueStr = PODUniqueString::build(context->context(), pch);

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
                                    bool& isErroneousOut);
static std::string eatTripleStringLiteral(yyscan_t scanner,
                                          const char* startChar,
                                          bool& isErroneousOut);

static int processStringLiteral(yyscan_t scanner,
                                const char* startChar,
                                int type) {
  // update the location for the string start (e.g. b" )
  const char* pch = yyget_text(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));

  bool erroneous = false;
  std::string value = eatStringLiteral(scanner, startChar, erroneous);

  ParserContext* context = yyget_extra(scanner);

  StringLiteral::QuoteStyle quotes = StringLiteral::SINGLE;
  if (startChar && startChar[0] == '"')
    quotes = StringLiteral::DOUBLE;

  Expression* lit = nullptr;
  if (erroneous) {
    lit = ErroneousExpression::build(context->builder,
                                     context->convertLocation(*loc)).release();
  } else if (type == STRINGLITERAL) {
    lit = StringLiteral::build(context->builder,
                               context->convertLocation(*loc),
                               quotes,
                               std::move(value)).release();
  } else if (type == BYTESLITERAL) {
    lit = BytesLiteral::build(context->builder,
                              context->convertLocation(*loc),
                              quotes,
                              std::move(value)).release();
  } else if (type == CSTRINGLITERAL) {
    lit = CStringLiteral::build(context->builder,
                                context->convertLocation(*loc),
                                quotes,
                                std::move(value)).release();
  } else {
    assert(false && "unknown type in processStringLiteral");
  }

  YYSTYPE* val = yyget_lval(scanner);
  val->expr = lit;

  return type;
}

static int processTripleStringLiteral(yyscan_t scanner,
                                      const char* startChar,
                                      int type) {
  // update the location for the string start (e.g. b" )
  const char* pch = yyget_text(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));

  bool erroneous = false;
  std::string value = eatTripleStringLiteral(scanner, startChar, erroneous);

  ParserContext* context = yyget_extra(scanner);

  StringLiteral::QuoteStyle quotes = StringLiteral::TRIPLE_SINGLE;
  if (startChar && startChar[0] == '"')
    quotes = StringLiteral::TRIPLE_DOUBLE;

  Expression* lit = nullptr;

  if (erroneous) {
    lit = ErroneousExpression::build(context->builder,
                                     context->convertLocation(*loc)).release();
  } else if (type == STRINGLITERAL) {
    lit = StringLiteral::build(context->builder,
                               context->convertLocation(*loc),
                               quotes,
                               std::move(value)).release();
  } else if (type == BYTESLITERAL) {
    lit = BytesLiteral::build(context->builder,
                              context->convertLocation(*loc),
                              quotes,
                              std::move(value)).release();
  } else if (type == CSTRINGLITERAL) {
    lit = CStringLiteral::build(context->builder,
                                context->convertLocation(*loc),
                                quotes,
                                std::move(value)).release();
  } else {
    assert(false && "unknown type in processStringLiteral");
  }

  YYSTYPE* val = yyget_lval(scanner);
  val->expr = lit;

  return type;
}

static
void noteErrInString(yyscan_t scanner, int nLines, int nCols, const char* msg) {
  ParserContext* context = yyget_extra(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  YYLTYPE myloc = *loc;
  updateLocation(&myloc, nLines, nCols);
  noteError(myloc, context, msg);
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
                                    bool& isErroneousOut) {
  YYLTYPE* loc = yyget_lloc(scanner);
  const char startCh = *startChar;
  int        c       = 0;
  int nLines = 0;
  int nCols = 0;
  std::string s;

  isErroneousOut = false;
  c = getNextYYChar(scanner);
  nCols++; // if it's newline, this will immediately be reset

  while (c != startCh && c != 0) {
    if (c == '\n') {
      // TODO: string literals with newline after backslash
      // are not documented in the spec
      noteErrInString(scanner, nLines, nCols, "end-of-line in a string literal without a preceding backslash");
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
        // Read hexadecimal digits until we run out
        char buf[3] = {'\0', '\0', '\0'};
        bool foundNonHex = false;
        bool overflow = false;
        for (int i = 0; c != 0; i++) {
          c = getNextYYChar(scanner);
          nCols++;
          // TODO the exact behavior here is not documented in the spec
          if (('0' <= c && c <= '9') ||
              ('A' <= c && c <= 'F') ||
              ('a' <= c && c <= 'f')) {
            if (i < 2)
              buf[i] = c;
            else
              overflow = true;
          } else {
            foundNonHex = true;
            break;
          }
        }

        long hexChar = strtol(buf, NULL, 16);

        if (hexChar == LONG_MIN) {
          noteErrInString(scanner, nLines, nCols,
                          "underflow when reading \\x escape");
          isErroneousOut = true;
        } else if (overflow || hexChar == LONG_MAX) {
          noteErrInString(scanner, nLines, nCols,
                          "overflow when reading \\x escape");
          isErroneousOut = true;
        }

        if (0 <= hexChar && hexChar <= 255) {
          // append the character
          char cc = (char) hexChar;
          s += cc;
        }

        if (foundNonHex)
          continue; // need to process c as the next character

      } else if (c == 'u' || c == 'U') {
        noteErrInString(scanner, nLines, nCols, "universal character name not yet supported in string literal");
        s += "\\u"; // this is a dummy value
        isErroneousOut = true;
      } else if ('0' <= c && c <= '7' ) {
        noteErrInString(scanner, nLines, nCols, "octal escape not supported in string literal");
        s += "\\";
        s += c; // a dummy value
        isErroneousOut = true;
      } else if (c == 0) {
        // we've reached EOF
        s += "\\";
        break; // EOF reached, so stop
      } else {
        noteErrInString(scanner, nLines, nCols, "unexpected string escape");
        isErroneousOut = true;
      }
    } else {
      s += c;
    }
    c = getNextYYChar(scanner);
    nCols++;
  } /* eat up string */

  if (c == 0) {
    noteErrInString(scanner, nLines, nCols, "EOF in string");
    isErroneousOut = true;
  }

  // update the location
  updateLocation(loc, nLines, nCols);

  return s;
}

static std::string eatTripleStringLiteral(yyscan_t scanner,
                                          const char* startChar,
                                          bool& isErroneousOut) {
  YYLTYPE* loc       = yyget_lloc(scanner);
  const char startCh = *startChar;
  int startChCount   = 0;
  int c              = 0;
  int nLines = 0;
  int nCols = 0;
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
    noteErrInString(scanner, nLines, nCols, "EOF in string");
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
          noteErrInString(scanner, nLines, nCols,
                          "Missing } in extern block");
          break;

        case in_single_quote:
        case in_single_quote_backslash:
          noteErrInString(scanner, nLines, nCols,
                          "Runaway \'string\' in extern block");
          break;

        case in_double_quote:
        case in_double_quote_backslash:
          noteErrInString(scanner, nLines, nCols,
                          "Runaway \"string\" in extern block");
          break;

        case in_single_line_comment:
          noteErrInString(scanner, nLines, nCols,
                          "Missing newline after extern block // comment");
          break;

        case in_multi_line_comment:
          noteErrInString(scanner, nLines, nCols,
                          "Runaway /* comment */ in extern block");
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

  ParserContext* context = yyget_extra(scanner);
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
  // start with the comment introduction
  std::string s;
  s += yyget_text(scanner);
  nCols += s.size();

  while (depth > 0) {
    int lastlastc = lastc;

    lastc = c;
    c     = getNextYYChar(scanner);

    if (c == '\n') {
      nCols = 0;
      nLines++;
    } else {
      nCols++;
    }
    s += c;

    if (lastc == '*' && c == '/' && lastlastc != '/') { // close comment
      depth--;
    } else if (lastc == '/' && c == '*') { // start nested
      depth++;
      // keep track of the start of the last nested comment
      nestedStartLine = nLines;
      nestedStartCol = nCols;
    } else if (c == 0) {
      noteErrInString(scanner, nLines, nCols, "EOF in comment");
      noteErrInString(scanner, startLine, startCol, "unterminated comment started here");
      if( nestedStartLine >= 0 ) {
        noteErrInString(scanner, nestedStartLine, nestedStartCol,
                        "nested comment started here");
      }
      break;
    }
  }

  updateLocation(loc, nLines, nCols);

  // allocate the value to return
  long size = s.size();
  char* buf = (char*) malloc(size+1);
  memcpy(buf, s.c_str(), size+1);

  ParserContext* context = yyget_extra(scanner);
  context->noteComment(*loc, buf, size);

  return YYLEX_BLOCK_COMMENT;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processInvalidToken(yyscan_t scanner) {
  const char* pch = yyget_text(scanner);
  YYLTYPE* loc = yyget_lloc(scanner);
  updateLocation(loc, 0, strlen(pch));
  ParserContext* context = yyget_extra(scanner);
  yychpl_error(loc, context, "Invalid token");
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
