#include <cstring>
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


static void addChar(char c) {
  if (stringLen+2 > stringBuffLen) {
    stringBuffLen = 2*(stringBuffLen + 1);
    stringBuffer = (char*)realloc(stringBuffer, stringBuffLen*sizeof(char));
  }
  stringBuffer[stringLen] = c;
  stringLen++;
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
  register int c;
          
  newString();
  countCommentLine();
  while (1) {
    while ((c = getNextYYChar()) != '*' && c != 0) {
      if (c == '\n') {
        countMultiLineComment(stringBuffer);
        processNewline();
        newString();
        countCommentLine();
      } else {
        addChar(c);
      }
    }    /* eat up text of comment */
    
    if ( c == '*' ) {
      while ( (c = getNextYYChar()) == '*' ) {
        addChar(c);
      }
      if ( c == '/' ) {
        countMultiLineComment(stringBuffer);
        newString();
        break;    /* found the end */
      } else if (c == '\n') {
        countMultiLineComment(stringBuffer);
        processNewline();
        newString();
        countCommentLine();
      }
    } else {      // c == EOF
      yyerror( "EOF in comment" );
      break;
    }
  }
}


void processWhitespace(const char* tabOrSpace) {
  // might eventually want to keep track of column numbers and do
  // something here
}


void processInvalidToken() {
  yyerror("Invalid token");
}
