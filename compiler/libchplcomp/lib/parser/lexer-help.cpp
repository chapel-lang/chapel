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

#include <cstring>
#include <cctype>
#include <string>
#include <algorithm>

static void  newString();
static void  addString(const char* str);
static void  addChar(char c);
static void  addCharEscapeNonprint(char c);
static void  addCharEscapingC(yyscan_t scanner, char c);

static int   getNextYYChar(yyscan_t scanner);

static std::string stringBuffer;

int processNewline(yyscan_t scanner) {
  YYLTYPE* yyLloc = yyget_lloc(scanner);

  // TODO: we had line counting here with global variables,
  // probably because things like eatStringLiteral don't have
  // flex updating the current location.

  return YYLEX_NEWLINE;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void stringBufferInit() {
  stringBuffer.clear();
}

static int  processIdentifier(yyscan_t scanner, bool queried) {
  YYSTYPE* yyLval = yyget_lval(scanner);
  int      retval = processToken(scanner, queried ? TQUERIEDIDENT : TIDENT);
  ParserContext* context = yyget_extra(scanner);

  yyLval->pch = context->astContext->uniqueCString(yyget_text(scanner));

  return retval;
}

static int processToken(yyscan_t scanner, int t) {
  YYSTYPE* yyLval = yyget_lval(scanner);

  yyLval->pch = yyget_text(scanner);

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

static const char* eatStringLiteral(yyscan_t scanner, const char* startChar);
static const char* eatMultilineStringLiteral(yyscan_t scanner,
                                             const char* startChar);

static int processStringLiteral(yyscan_t scanner, const char* q, int type) {
  const char* yyText = yyget_text(scanner);
  YYSTYPE*    yyLval = yyget_lval(scanner);

  yyLval->pch = eatStringLiteral(scanner, q);

  return type;
}

static int processMultilineStringLiteral(yyscan_t scanner, const char* q,
                                         int type) {
  const char* yyText = yyget_text(scanner);
  YYSTYPE* yyLval = yyget_lval(scanner);
  yyLval->pch = eatMultilineStringLiteral(scanner, q);

  return type;
}

static const char* eatStringLiteral(yyscan_t scanner, const char* startChar) {
  char*      yyText  = yyget_text(scanner);
  YYLTYPE*   yyLloc  = yyget_lloc(scanner);
  const char startCh = *startChar;
  int        c       = 0;

  newString();

  while ((c = getNextYYChar(scanner)) != startCh && c != 0) {
    if (c == '\n') {
      ParserContext* context = yyget_extra(scanner);

      yyText[0] = '\0';
      yyerror(yyLloc, context, "end-of-line in a string literal without a preceding backslash");
    } else {
      if (startCh == '\'' && c == '\"') {
        addCharEscapeNonprint('\\');
      }

      // \ escape ? to avoid C trigraphs
      if (c == '?')
        addCharEscapeNonprint('\\');

      addCharEscapeNonprint(c);
    }

    if (c == '\\') {
      c = getNextYYChar(scanner);

      if (c == '\n') {
        processNewline(scanner);
        addCharEscapeNonprint('n');
      } else if (c == 'u' || c == 'U') {
        ParserContext* context = yyget_extra(scanner);
        yyerror(yyLloc, context, "universal character name not yet supported in string literal");
        addCharEscapeNonprint('t'); // add a valid escape to continue parsing
      } else if ('0' <= c && c <= '7' ) {
        ParserContext* context = yyget_extra(scanner);
        yyerror(yyLloc, context, "octal escape not supported in string literal");
        addCharEscapeNonprint('t'); // add a valid escape to continue parsing
      } else if (c == 0) {
        // we've reached EOF
        addCharEscapeNonprint('t'); // add a valid escape to continue parsing
        break; // EOF reached, so stop
      } else {
        addCharEscapeNonprint(c);
      }
    }
  } /* eat up string */

  if (c == 0) {
    ParserContext* context = yyget_extra(scanner);

    yyerror(yyLloc, context, "EOF in string");
  }

  return astr(stringBuffer);
}

static const char* eatMultilineStringLiteral(yyscan_t scanner,
                                             const char* startChar) {
  YYLTYPE* yyLloc    = yyget_lloc(scanner);
  const char startCh = *startChar;
  int startChCount   = 0;
  int c              = 0;

  newString();

  while (true) {
    c = getNextYYChar(scanner);

    if (c == 0) {
      break;
    }

    if (c == startCh) {
      startChCount++;
      if (startChCount == 3) {
        break;
      }
    } else {
      startChCount = 0;
    }

    addCharEscapingC(scanner, c);
  } /* eat up string */

  if (c == 0) {
    ParserContext* context = yyget_extra(scanner);

    yyerror(yyLloc, context, "EOF in string");
  }
  // Remove two escaped quotes from the end of the string that are
  // actually part of the string closing token.  If this is a single
  // quoted string that will be two characters, but if it is a double
  // quoted string it will be four because of extra escape characters
  int removeChars = (startCh == '\'') ? 2 : 4;
  std::string sub = stringBuffer.substr(0, stringBuffer.length()-removeChars);

  return astr(sub);
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processExtern(yyscan_t scanner) {
  const char* yyText = yyget_text(scanner);
  YYSTYPE*    yyLval = yyget_lval(scanner);

  yyLval->pch = yyget_text(scanner);

  // Push a state to record that "extern" has been seen
  yy_push_state(externmode, scanner);

  return TEXTERN;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static const char* eatExternCode(yyscan_t scanner);

// When the lexer calls this function, it has already consumed the first '{'
static int processExternCode(yyscan_t scanner) {
  YYSTYPE* yyLval = yyget_lval(scanner);

  yyLval->pch = eatExternCode(scanner);

  return EXTERNCODE;
}

static const char* eatExternCode(yyscan_t scanner) {
  const int in_code                          = 0;
  const int in_single_quote                  = 1;
  const int in_single_quote_backslash        = 2;
  const int in_double_quote                  = 3;
  const int in_double_quote_backslash        = 4;
  const int in_single_line_comment           = 5;
  const int in_single_line_comment_backslash = 6;
  const int in_multi_line_comment            = 7;

  YYLTYPE*  yyLloc                           = yyget_lloc(scanner);

  int       depth                            = 1;
  int       c                                = 0;
  int       lastc                            = 0;
  int       state                            = 0;

  newString();

  // First, store the line information.
  addString("#line ");
  addString(istr(chplLineno));
  addString(" \"");
  addString(yyfilename);
  addString("\" ");
  addString("\n");

  // Now, append the C code until we get to a }.
  while (depth > 0) {
    lastc = c;
    c     = getNextYYChar(scanner);

    if (c == 0) {
      ParserContext* context = yyget_extra(scanner);

      switch (state) {
        case in_code:
          // there was no match to the {
          yyerror(yyLloc, context, "Missing } in extern block");
          break;

        case in_single_quote:
        case in_single_quote_backslash:
          yyerror(yyLloc, context, "Runaway \'string\' in extern block");
          break;

        case in_double_quote:
        case in_double_quote_backslash:
          yyerror(yyLloc, context, "Runaway \"string\" in extern block");
          break;

        case in_single_line_comment:
          yyerror(yyLloc, context, "Missing newline after extern block // comment");
          break;

        case in_multi_line_comment:
          yyerror(yyLloc, context, "Runaway /* comment */ in extern block");
          break;
      }
      break;
    }

    addChar(c);

    if (c == '\n')
      processNewline(scanner);

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

  //save the C String
  //eliminate the final '{'
  if (stringBuffer.size() >= 1)
    stringBuffer.resize(stringBuffer.size()-1);

  return astr(stringBuffer);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processWhitespace(yyscan_t scanner) {
  // might eventually want to keep track of column numbers and do
  // something here
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processSingleLineComment(yyscan_t scanner) {
  YYSTYPE* yyLval = yyget_lval(scanner);
  int      c      = 0;

  newString();
  countCommentLine();

  // Read until the end of the line
  while ((c = getNextYYChar(scanner)) != '\n' && c != 0) {
    addChar(c);
  }

  countSingleLineComment(stringBuffer.c_str());

  if (c != 0) {
    processNewline(scanner);
  }

  yyLval->pch = astr(stringBuffer);

  return YYLEX_SINGLE_LINE_COMMENT;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processBlockComment(yyscan_t scanner) {
  YYSTYPE*    yyLval       = yyget_lval(scanner);
  YYLTYPE*    yyLloc       = yyget_lloc(scanner);

  int nestedStartLine = -1;
  int startLine = chplLineno;
  const char* startFilename = yyfilename;

  int         c            = 0;
  int         d            = 1;
  bool        badComment = false;
  int         lastc        = 0;
  int         depth        = 1;
  std::string wholeComment = "";

  newString();
  countCommentLine();

  while (depth > 0) {
    int lastlastc = lastc;

    lastc = c;
    c     = getNextYYChar(scanner);

    if (c == '\n') {
      countMultiLineComment(stringBuffer.c_str());
      processNewline(scanner);

      wholeComment += stringBuffer;
      wholeComment += '\n';

      newString();
      countCommentLine();
    } else {
      addChar(c);
    }

    d = 1;

    if (lastc == '*' && c == '/' && lastlastc != '/') { // close comment
      depth--;
      
      d = 1;
    } else if (lastc == '/' && c == '*') { // start nested
      depth++;
      // keep track of the start of the last nested comment
      nestedStartLine = chplLineno;
    } else if (c == 0) {
      ParserContext* context = yyget_extra(scanner);

      fprintf(stderr, "%s:%d: unterminated comment started here\n",
              startFilename, startLine);
      if( nestedStartLine >= 0 ) {
        fprintf(stderr, "%s:%d: nested comment started here\n",
                startFilename, nestedStartLine);
      }
      yyerror(yyLloc, context, "EOF in comment");
      break;
    }
  }

  // Saves the comment grabbed to the comment field of the location struct,
  // for use when the --docs flag is implemented
  {
    wholeComment += stringBuffer;

    // Also, only need to fix indentation failure when the comment matters
    size_t location = wholeComment.find("\\x09");

    while (location != std::string::npos) {
      wholeComment = wholeComment.substr(0, location) + wholeComment.substr(location + 4);

      wholeComment.insert(location, "\t");

      location = wholeComment.find("\\x09");
    }
    if(!badComment)
      yyLval->pch = astr(wholeComment.c_str());
    else {

      fprintf(stderr, "Warning:%d: chpldoc comment not closed, ignoring comment:%s\n",
              startLine, wholeComment.c_str());
      yyLval->pch = NULL;
    }
  } else {
    yyLval->pch = NULL;
  }

  countMultiLineComment(stringBuffer.c_str());

  newString();

  return YYLEX_BLOCK_COMMENT;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processInvalidToken(yyscan_t scanner) {
  ParserContext* context = yyget_extra(scanner);
  YYLTYPE*      yyLloc = yyget_lloc(scanner);

  yyerror(yyLloc, context, "Invalid token");
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static char toHex(char c);

static void newString() {
  stringBuffer.clear();
}

// Does not escape
static void addString(const char* str) {
  stringBuffer.append(str);
}

// Does not escape
static void addChar(char c) {
  stringBuffer.push_back(c);
}

// Escapes
static void addCharEscapeNonprint(char c) {
  int escape  = !(isascii(c) && isprint(c));
  //
  // If the previous character sequence was a hex escape and the current
  // character is a hex digit, escape it also.  Otherwise, conforming
  // target C compilers interpret this character as a continuation of
  // the previous hex escape.
  //
  if (isxdigit(c)) {
    size_t len = stringBuffer.length();
    if (len >= 4 && stringBuffer[len - 4] == '\\' &&
        (stringBuffer[len - 3] == 'x' || stringBuffer[len - 3] == 'X') &&
        isxdigit(stringBuffer[len - 2]) && isxdigit(stringBuffer[len - 1])) {
      escape = 1;
    }
  }

  if (escape) {
    stringBuffer.push_back('\\');
    stringBuffer.push_back('x');
    stringBuffer.push_back(toHex(((unsigned char)c) >> 4));
    stringBuffer.push_back(toHex(c & 0xf));
  } else {
    stringBuffer.push_back(c);
  }
}

// Convert C escape characters into two characters: '\\' and the other character
static void addCharEscapingC(yyscan_t scanner, char c) {
  switch (c) {
    case '\"' :
      addChar('\\');
      addChar('"');
      break;
    case '?' :
      addChar('\\');
      addChar('?');
      break;
    case '\\' :
      addChar('\\');
      addChar('\\');
      break;
    case '\a' :
      addChar('\\');
      addChar('a');
      break;
    case '\b' :
      addChar('\\');
      addChar('b');
      break;
    case '\f' :
      addChar('\\');
      addChar('f');
      break;
    case '\n' :
      addChar('\\');
      addChar('n');
      // Keep track of line numbers when a newline is found in a string
      processNewline(scanner);
      break;
    case '\r' :
      addChar('\\');
      addChar('r');
      break;
    case '\t' :
      addChar('\\');
      addChar('t');
      break;
    case '\v' :
      addChar('\\');
      addChar('v');
      break;
    default :
      addChar(c);
      break;
  }
}

// Returns the hexadecimal character for 0-16.
static char toHex(char c) {
  return (0 <= c && c <= 9) ? '0' + c : 'A' + (c - 10);
}

static int getNextYYChar(yyscan_t scanner) {
  int retval = yyinput(scanner);

  if (retval == EOF) {
    retval = 0;
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
