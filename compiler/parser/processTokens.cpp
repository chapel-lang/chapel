#include <cstring>
#include <cctype>
#include "countTokens.h"
#include "misc.h"
#include "processTokens.h"
#include "yy.h"
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

static void addChar(char c) {
  int escape = !(isascii(c) && isprint(c));
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
        addChar('\\');
      }
      addChar(c);
     }
      if (c == '\\') {
        c = getNextYYChar();
        if (c == '\n')
        {
          processNewline(); addChar('n');
        } else if (c != 0) {
          addChar(c);
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


void processMultiLineComment(void) {
  int c;
  int lastc;
  int depth;

  c = 0;
  lastc = 0;
  depth = 1;

  newString();
  countCommentLine();
  
  while (depth > 0) {
    lastc = c;
    c = getNextYYChar();
    if( c == '\n' ) {
      countMultiLineComment(stringBuffer);
      processNewline();
      newString();
      countCommentLine();
    } else {
      addChar(c);
    }
    if( lastc == '*' && c == '/' ) { // close comment
      depth--;
    } else if( lastc == '/' && c == '*' ) { // start nested
      depth++;
    } else if( c == 0 ) {
      yyerror( "EOF in comment" );
    }
  }

  // back up two to not print */ again.
  if( stringLen >= 2 ) stringLen -= 2;
  stringBuffer[stringLen] = '\0';
  countMultiLineComment(stringBuffer);
  newString();
}


void processWhitespace(const char* tabOrSpace) {
  // might eventually want to keep track of column numbers and do
  // something here
}


void processInvalidToken() {
  yyerror("Invalid token");
}
