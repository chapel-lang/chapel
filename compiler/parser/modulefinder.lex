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
