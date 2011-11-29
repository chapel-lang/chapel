%{

#include <stdio.h>
#include "processTokens.h"
#include "modulefinder.tab.h"
#include "yy.h"

/* avoid warnings */
#define YY_NO_UNPUT

#ifndef processToken
#define processToken(t) \
  return(t)
#endif

#define processStringLiteral(q)     \
  yylval.pch = eatStringLiteral(q); \
  return(TSTRINGLITERAL)


%}

digit           [0-9]
bit             [0-1]
hexDigit        [0-9a-fA-F]
letter          [_a-zA-Z]
ident           {letter}({letter}|{digit}|\?)*
binaryLiteral   0b{bit}+
hexLiteral      0x{hexDigit}+
intLiteral      {digit}+|{binaryLiteral}|{hexLiteral}
exponent        [Ee][\+\-]?{digit}+
floatLiteral    {digit}*"."{digit}+{exponent}?


%%

module          processToken(TMODULE);
{ident}         processToken(TIDENT);
"{"             processToken(TLCBR);
"}"             processToken(TRCBR);
"\""            processStringLiteral("\"");
"\'"            processStringLiteral("\'");

[ \t]           processWhitespace(yytext);
\n              processNewline();

"--"            processSingleLineComment();
"//"            processSingleLineComment();

"/*"            processMultiLineComment();

.               processToken(TOTHER);

%%

// it's difficult to prototype yyinput, so this is a way of exporting
// it to other files in a controlled way

int getNextYYChar() {
  return yyinput();
}
