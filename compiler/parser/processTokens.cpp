/*
 * Copyright 2004-2014 Cray Inc.
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

#include "processTokens.h"

#include "countTokens.h"
#include "misc.h"
#include "stringutil.h"
#include "yy.h"

#include <cstring>
#include <cctype>

#ifndef MODULE_FINDER
#include "chapel.tab.h"
#else
#include "modulefinder.tab.h"

#define countNewline()
#define countSingleLineComment(x)
#define countMultiLineComment(x)
#define countCommentLine()
#endif

static int stringBuffLen = 0;
static int stringLen = 0;
static char* stringBuffer = NULL;


static void newString(void) {
  stringLen = 0;
  if (stringBuffLen) {
    stringBuffer[stringLen] = '\0';
  }
}

// Returns the hexadecimal character for 0-16.
static char toHex(char c)
{
  if (0 <= c && c <= 9) return '0' + c;
  else return 'A' + (c - 10);
}

static void addCharMaybeEscape(char c, bool canEscape) {
  int escape = canEscape && !(isascii(c) && isprint(c));
  int charlen = escape ? 4 : 1; // convert nonasci to \xNN

  if (stringLen+charlen+1 > stringBuffLen) {
    stringBuffLen = 2*(stringBuffLen + charlen);
    stringBuffer = (char*)realloc(stringBuffer, stringBuffLen*sizeof(char));
  }

  if (escape) {
    stringBuffer[stringLen++] = '\\';
    stringBuffer[stringLen++] = 'x';
    stringBuffer[stringLen++] = toHex(((unsigned char)c) >> 4);
    stringBuffer[stringLen++] = toHex(c & 0xf);
  } else {
    stringBuffer[stringLen++] = c;
  }
  stringBuffer[stringLen] = '\0';
}

static inline void addCharString(char c) {
  addCharMaybeEscape(c, true);
}

static inline void addChar(char c) {
  addCharMaybeEscape(c, false);
}

static void addString(const char* str) {
  int i;
  for( i = 0; str[i]; i++ ) addChar(str[i]);
}

void processNewline(void) {
  chplLineno++;
  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = chplLineno;
  countNewline();
}


char* eatStringLiteral(const char* startChar) {
  register int c;
  const char startCh = *startChar;
  
  newString();
    while ((c = getNextYYChar()) != startCh && c != 0) {
      if (c == '\n')
     {
        yytext[0] = '\0'; yyerror(
          "end-of-line in a string literal without a preceeding backslash");
     } else {
      if (startCh == '\'' && c == '\"') {
        addCharString('\\');
      }
      addCharString(c);
     }
      if (c == '\\') {
        c = getNextYYChar();
        if (c == '\n')
        {
          processNewline(); addCharString('n');
        } else if (c != 0) {
          addCharString(c);
        }
        else
          break;
      }
    } /* eat up string */
    if (c == 0) {
      yyerror("EOF in string");
    }
  return stringBuffer;
}


void processSingleLineComment(void) {
  register int c;

  newString();
  countCommentLine();
  while (1) {
    while ( (c = getNextYYChar()) != '\n' && c != 0) {
      addChar(c);
    }    /* eat up text of comment */
    countSingleLineComment(stringBuffer);
    if (c != 0) {
      processNewline();
    }
    break;
  }
}


void processMultiLineComment() {
  int c;
  int lastc;
  int lastlastc;
  int depth;

  c = 0;
  lastc = 0;
  depth = 1;

  newString();
  countCommentLine();

  int labelIndex = 0;
  int len = strlen(fDocsCommentLabel);
  if (len >= 2) {
    labelIndex = 2;
  }

  std::string wholeComment = "";
  
  while (depth > 0) {
    lastlastc = lastc;
    lastc = c;
    c = getNextYYChar();
    if( c == '\n' ) {
      countMultiLineComment(stringBuffer);
      processNewline();
      if (fDocs && labelIndex == len) {
        wholeComment += stringBuffer;
        wholeComment += '\n';
      }
      newString();
      countCommentLine();
    } else {
      if ((labelIndex < len) && (labelIndex != -1)) {
        if (c == fDocsCommentLabel[labelIndex]) {
          labelIndex++;
        } else {
          labelIndex = -1;
        }
      }
      addChar(c);
    }
    if( lastc == '*' && c == '/' && lastlastc != '/' ) { // close comment
      depth--;
    } else if( lastc == '/' && c == '*' ) { // start nested
      depth++;
    } else if( c == 0 ) {
      yyerror( "EOF in comment" );
    }
  }

  // back up two to not print */ again.
  if( stringLen >= 2 ) stringLen -= 2;
  // back up further if the user has specified a special form of commenting
  if (len > 2 && labelIndex == len) stringLen -= (len - 2);
  stringBuffer[stringLen] = '\0';
  
  // Saves the comment grabbed to the comment field of the location struct,
  // for use when the --docs flag is implemented
  if (fDocs && labelIndex == len) {
    wholeComment += stringBuffer;
    if (len > 2) {
      len -= 2;
      wholeComment = wholeComment.substr(len);
      // Trim the start of the string if the user has specified a special form
      // of commenting
    }

    // Also, only need to fix indentation failure when the comment matters
    size_t location = wholeComment.find("\\x09");
    while (location != std::string::npos) {
      wholeComment = wholeComment.substr(0, location) + wholeComment.substr(location + 4);
      wholeComment.insert(location, "\t");
      location = wholeComment.find("\\x09");
    }
    yylloc.comment = (char *)astr(wholeComment.c_str());
  }

  countMultiLineComment(stringBuffer);
  newString();
}

char* eatExternCode() {
  // Note - when the lexer calls this function, it has already
  // consumed the first {
  int depth = 1;
  int c = 0, lastc = 0;
  const int in_code = 0;
  const int in_single_quote = 1;
  const int in_single_quote_backslash = 2;
  const int in_double_quote = 3;
  const int in_double_quote_backslash = 4;
  const int in_single_line_comment = 5;
  const int in_single_line_comment_backslash = 6;
  const int in_multi_line_comment = 7;
  int state = 0;

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
    c = getNextYYChar();
   
    if (c == 0) {
      switch (state) {
        case in_code:
          // there was no match to the {
          yyerror("Missing } in extern block");
          break;
        case in_single_quote:
        case in_single_quote_backslash:
          yyerror("Runaway \'string\' in extern block");
          break;
        case in_double_quote:
        case in_double_quote_backslash:
          yyerror("Runaway \"string\" in extern block");
          break;
        case in_single_line_comment: 
          yyerror("Missing newline after extern block // comment");
          break;
        case in_multi_line_comment: 
          yyerror("Runaway /* comment */ in extern block");
          break;
      }
      break;
    }

    addChar(c);

    if( c == '\n' ) processNewline();

    // Now update state (are we in a comment? a string?)
    switch (state) {
      case in_code:
        if( c == '\'' ) state = in_single_quote;
        else if( c == '"' ) state = in_double_quote;
        else if( lastc == '/' && c == '/' ) state = in_single_line_comment;
        else if( lastc == '/' && c == '*' ) state = in_multi_line_comment;
        else if( c == '{' ) depth++;
        else if( c == '}' ) depth--;
        break;
      case in_single_quote:
        if( c == '\\' ) state = in_single_quote_backslash;
        else if( c == '\'' ) state = in_code;
        break;
      case in_single_quote_backslash:
        state = in_single_quote;
        break;
      case in_double_quote:
        if( c == '\\' ) state = in_double_quote_backslash;
        else if( c == '"' ) state = in_code;
        break;
      case in_double_quote_backslash:
        state = in_double_quote;
        break;
      case in_single_line_comment: 
        if( c == '\n' ) state = in_code;
        break;
      case in_single_line_comment_backslash: 
        if( c == ' ' || c == '\t' || c == '\n' )
          state = in_single_line_comment_backslash;
        else state = in_single_line_comment;
        break;
      case in_multi_line_comment: 
        if( lastc == '*' && c == '/' ) state = in_code;
        break;
    }
  }

  //save the C String
  //eliminate the final '{'
  if (stringLen >=1) stringLen -= 1;
  stringBuffer[stringLen] = '\0';

  return stringBuffer;
}

void processWhitespace(const char* tabOrSpace) {
  // might eventually want to keep track of column numbers and do
  // something here
}


void processInvalidToken() {
  yyerror("Invalid token");
}
