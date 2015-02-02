/*
 * Copyright 2004-2015 Cray Inc.
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
#include "chapel.tab.h"

#include <cstring>
#include <cctype>
#include <string>

int   stringBuffLen = 0;
int   stringLen     = 0;
char* stringBuffer  = NULL;

void newString() {
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

void addCharString(char c) {
  addCharMaybeEscape(c, true);
}

void addChar(char c) {
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

