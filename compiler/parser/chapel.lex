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

%option noyywrap
%option nounput

%{

#include <cstdio>
#include "lexyacc.h"
#include "chapel.tab.h"

#ifndef processToken
#define processToken(t)                         \
  countToken(yytext);                           \
  if (captureTokens) {                          \
    if (t == TASSIGN ||                         \
        t == TDOTDOTDOT)                        \
      strcat(captureString, " ");               \
    if (t != TLCBR)                             \
      strcat(captureString, yytext);            \
    if (t == TCOMMA ||                          \
        t == TPARAM ||                          \
        t == TZIP  ||                           \
        t == TTYPE ||                           \
        t == TCONST ||                          \
        t == TIN ||                             \
        t == TINOUT ||                          \
        t == TOUT ||                            \
        t == TREF ||                            \
        t == TCOLON ||                          \
        t == TASSIGN ||                         \
        t == TRSBR)                             \
      strcat(captureString, " ");               \
  }                                             \
  /* processToken means we are parsing Chapel */\
  BEGIN(INITIAL);                               \
  return(t)
#endif

#define processStringLiteral(q)            \
  yylval.pch = eatStringLiteral(q);        \
  countToken(astr(q, yylval.pch, q)); \
  if (captureTokens) {                     \
    strcat(captureString, yytext);         \
    strcat(captureString, yylval.pch);     \
    strcat(captureString, yytext);         \
  }                                        \
  /* string literals only in Chapel */     \
  BEGIN(INITIAL);                          \
  return(STRINGLITERAL)

#define processExtern()                    \
  countToken(yytext);                      \
  if (captureTokens) {                     \
    strcat(captureString, yytext);         \
  }                                        \
  BEGIN(externmode);                       \
  return TEXTERN;

#define processExternCode()                \
  yylval.pch = eatExternCode();            \
  countToken(astr(yylval.pch));            \
  if (captureTokens) {                     \
    strcat(captureString, yylval.pch);     \
  }                                        \
  /* only one { } block is special */      \
  BEGIN(INITIAL);                          \
  return(EXTERNCODE)



%}

digit            [0-9]
bit              [0-1]
octDigit         [0-7]
hexDigit         [0-9a-fA-F]
letter           [_a-zA-Z]
ident            {letter}({letter}|{digit}|"$")*
binaryLiteral    0[bB]{bit}+
octalLiteral     0[oO]{octDigit}+
hexLiteral       0[xX]{hexDigit}+
intLiteral       {digit}+|{binaryLiteral}|{octalLiteral}|{hexLiteral}
exponent         [Ee][\+\-]?{digit}+
floatLiteral1    {digit}*"."{digit}+({exponent})?
floatLiteral2    {digit}+"."{exponent}
floatLiteral3    {digit}+{exponent}
floatLiteral     {floatLiteral1}|{floatLiteral2}|{floatLiteral3}

%s externmode

%%

align            processToken(TALIGN);
atomic           processToken(TATOMIC);
begin            processToken(TBEGIN);
break            processToken(TBREAK);
by               processToken(TBY);
class            processToken(TCLASS);
cobegin          processToken(TCOBEGIN);
coforall         processToken(TCOFORALL);
config           processToken(TCONFIG);
const            processToken(TCONST);
continue         processToken(TCONTINUE);
delete           processToken(TDELETE);
dmapped          processToken(TDMAPPED);
do               processToken(TDO);
domain           processToken(TDOMAIN);
else             processToken(TELSE);
enum             processToken(TENUM);
export           processToken(TEXPORT);
extern           processExtern();
for              processToken(TFOR);
forall           processToken(TFORALL);
if               processToken(TIF);
in               processToken(TIN);
index            processToken(TINDEX);
inline           processToken(TINLINE);
inout            processToken(TINOUT);
iter             processToken(TITER);
label            processToken(TLABEL);
lambda           processToken(TLAMBDA);
let              processToken(TLET);
local            processToken(TLOCAL);
module           processToken(TMODULE);
new              processToken(TNEW);
nil              processToken(TNIL);
noinit           processToken(TNOINIT);
on               processToken(TON);
otherwise        processToken(TOTHERWISE);
out              processToken(TOUT);
param            processToken(TPARAM);
zip              processToken(TZIP);
pragma           processToken(TPRAGMA);
__primitive      processToken(TPRIMITIVE);
proc             processToken(TPROC);
record           processToken(TRECORD);
reduce           processToken(TREDUCE);
ref              processToken(TREF);
return           processToken(TRETURN);
scan             processToken(TSCAN);
select           processToken(TSELECT);
serial           processToken(TSERIAL);
single           processToken(TSINGLE);
sparse           processToken(TSPARSE);
subdomain        processToken(TSUBDOMAIN);
sync             processToken(TSYNC);
then             processToken(TTHEN);
type             processToken(TTYPE);
union            processToken(TUNION);
use              processToken(TUSE);
var              processToken(TVAR);
when             processToken(TWHEN);
where            processToken(TWHERE);
while            processToken(TWHILE);
with             processToken(TWITH);
yield            processToken(TYIELD);

"_"              processToken(TUNDERSCORE);

"="              processToken(TASSIGN);
"+="             processToken(TASSIGNPLUS);
"-="             processToken(TASSIGNMINUS);
"*="             processToken(TASSIGNMULTIPLY);
"/="             processToken(TASSIGNDIVIDE);
"**="            processToken(TASSIGNEXP);
"%="             processToken(TASSIGNMOD);
"&="             processToken(TASSIGNBAND);
"|="             processToken(TASSIGNBOR);
"^="             processToken(TASSIGNBXOR);
"&&="            processToken(TASSIGNLAND);
"||="            processToken(TASSIGNLOR);
"<<="            processToken(TASSIGNSL);
">>="            processToken(TASSIGNSR);

"=>"             processToken(TALIAS);

"<=>"            processToken(TSWAP);

{floatLiteral}   processToken(REALLITERAL);

"#"              processToken(THASH);
".."             processToken(TDOTDOT);
"..."            processToken(TDOTDOTDOT);

"&&"             processToken(TAND);
"||"             processToken(TOR);
"!"              processToken(TNOT);

"&"              processToken(TBAND);
"|"              processToken(TBOR);
"^"              processToken(TBXOR);
"~"              processToken(TBNOT);

"<<"             processToken(TSHIFTLEFT);
">>"             processToken(TSHIFTRIGHT);

"=="             processToken(TEQUAL);
"!="             processToken(TNOTEQUAL);
"<="             processToken(TLESSEQUAL);
">="             processToken(TGREATEREQUAL);
"<"              processToken(TLESS);
">"              processToken(TGREATER);

"+"              processToken(TPLUS);
"-"              processToken(TMINUS);
"*"              processToken(TSTAR);
"/"              processToken(TDIVIDE);
"%"              processToken(TMOD);
"--"             processToken(TMINUSMINUS);
"++"             processToken(TPLUSPLUS);

"**"             processToken(TEXP);

":"              processToken(TCOLON);
";"              processToken(TSEMI);
","              processToken(TCOMMA);
"."              processToken(TDOT);
"("              processToken(TLP);
")"              processToken(TRP);
"["              processToken(TLSBR);
"]"              processToken(TRSBR);
<externmode>"{"  processExternCode();
<INITIAL>"{"     processToken(TLCBR);
"}"              processToken(TRCBR);
"<~>"            processToken(TIO);


"?"              processToken(TQUESTION);

{ident}          processToken(TIDENT);
{intLiteral}     processToken(INTLITERAL);
{intLiteral}i    processToken(IMAGLITERAL);
{floatLiteral}i  processToken(IMAGLITERAL);

"\""             processStringLiteral("\"");
"\'"             processStringLiteral("\'");

[ \t\r]          processWhitespace(yytext);
\n               processNewline();

"//"             processSingleLineComment();

"/*"             processMultiLineComment();

.                processInvalidToken();

%%

// it's difficult to prototype yyinput, so this is a way of exporting
// it to other files in a controlled way

int getNextYYChar() {
  int retval = yyinput();
  if (retval == EOF) {
    retval = 0;
  }
  return retval;
}

bool chplParseString;
const char *chplParseStringMsg = NULL;
BlockStmt*
parseString(const char* string, const char* filename, const char* msg) {
  yyblock = NULL;
  yyfilename = filename;
  chplParseString = true;
  chplParseStringMsg = msg;
  yy_scan_string(string);
  yyparse();
  chplParseString = false;
  chplParseStringMsg = NULL;
  YY_NEW_FILE; // reset the lexer
  return yyblock;
}

