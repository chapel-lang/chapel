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

%option noyywrap
%option nounput

%{

#include "lexyacc.h"
#include "chapel.tab.h"

#include <cstdio>

static int  processToken(int t);
static int  processStringLiteral(const char* q);
static int  processExtern();
static int  processExternCode();

static void processWhitespace(const char* tabOrSpace);
static void processInvalidToken();

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

align            return processToken(TALIGN);
atomic           return processToken(TATOMIC);
begin            return processToken(TBEGIN);
break            return processToken(TBREAK);
by               return processToken(TBY);
class            return processToken(TCLASS);
cobegin          return processToken(TCOBEGIN);
coforall         return processToken(TCOFORALL);
config           return processToken(TCONFIG);
const            return processToken(TCONST);
continue         return processToken(TCONTINUE);
delete           return processToken(TDELETE);
dmapped          return processToken(TDMAPPED);
do               return processToken(TDO);
domain           return processToken(TDOMAIN);
else             return processToken(TELSE);
enum             return processToken(TENUM);
export           return processToken(TEXPORT);
extern           return processExtern();
for              return processToken(TFOR);
forall           return processToken(TFORALL);
if               return processToken(TIF);
in               return processToken(TIN);
index            return processToken(TINDEX);
inline           return processToken(TINLINE);
inout            return processToken(TINOUT);
iter             return processToken(TITER);
label            return processToken(TLABEL);
lambda           return processToken(TLAMBDA);
let              return processToken(TLET);
local            return processToken(TLOCAL);
module           return processToken(TMODULE);
new              return processToken(TNEW);
nil              return processToken(TNIL);
noinit           return processToken(TNOINIT);
on               return processToken(TON);
otherwise        return processToken(TOTHERWISE);
out              return processToken(TOUT);
param            return processToken(TPARAM);
zip              return processToken(TZIP);
pragma           return processToken(TPRAGMA);
__primitive      return processToken(TPRIMITIVE);
proc             return processToken(TPROC);
record           return processToken(TRECORD);
reduce           return processToken(TREDUCE);
ref              return processToken(TREF);
return           return processToken(TRETURN);
scan             return processToken(TSCAN);
select           return processToken(TSELECT);
serial           return processToken(TSERIAL);
single           return processToken(TSINGLE);
sparse           return processToken(TSPARSE);
subdomain        return processToken(TSUBDOMAIN);
sync             return processToken(TSYNC);
then             return processToken(TTHEN);
type             return processToken(TTYPE);
union            return processToken(TUNION);
use              return processToken(TUSE);
var              return processToken(TVAR);
when             return processToken(TWHEN);
where            return processToken(TWHERE);
while            return processToken(TWHILE);
with             return processToken(TWITH);
yield            return processToken(TYIELD);

"_"              return processToken(TUNDERSCORE);

"="              return processToken(TASSIGN);
"+="             return processToken(TASSIGNPLUS);
"-="             return processToken(TASSIGNMINUS);
"*="             return processToken(TASSIGNMULTIPLY);
"/="             return processToken(TASSIGNDIVIDE);
"**="            return processToken(TASSIGNEXP);
"%="             return processToken(TASSIGNMOD);
"&="             return processToken(TASSIGNBAND);
"|="             return processToken(TASSIGNBOR);
"^="             return processToken(TASSIGNBXOR);
"&&="            return processToken(TASSIGNLAND);
"||="            return processToken(TASSIGNLOR);
"<<="            return processToken(TASSIGNSL);
">>="            return processToken(TASSIGNSR);

"=>"             return processToken(TALIAS);

"<=>"            return processToken(TSWAP);

{floatLiteral}   return processToken(REALLITERAL);

"#"              return processToken(THASH);
".."             return processToken(TDOTDOT);
"..."            return processToken(TDOTDOTDOT);

"&&"             return processToken(TAND);
"||"             return processToken(TOR);
"!"              return processToken(TNOT);

"&"              return processToken(TBAND);
"|"              return processToken(TBOR);
"^"              return processToken(TBXOR);
"~"              return processToken(TBNOT);

"<<"             return processToken(TSHIFTLEFT);
">>"             return processToken(TSHIFTRIGHT);

"=="             return processToken(TEQUAL);
"!="             return processToken(TNOTEQUAL);
"<="             return processToken(TLESSEQUAL);
">="             return processToken(TGREATEREQUAL);
"<"              return processToken(TLESS);
">"              return processToken(TGREATER);

"+"              return processToken(TPLUS);
"-"              return processToken(TMINUS);
"*"              return processToken(TSTAR);
"/"              return processToken(TDIVIDE);
"%"              return processToken(TMOD);
"--"             return processToken(TMINUSMINUS);
"++"             return processToken(TPLUSPLUS);

"**"             return processToken(TEXP);

":"              return processToken(TCOLON);
";"              return processToken(TSEMI);
","              return processToken(TCOMMA);
"."              return processToken(TDOT);
"("              return processToken(TLP);
")"              return processToken(TRP);
"["              return processToken(TLSBR);
"]"              return processToken(TRSBR);
<externmode>"{"  return processExternCode();
<INITIAL>"{"     return processToken(TLCBR);
"}"              return processToken(TRCBR);
"<~>"            return processToken(TIO);


"?"              return processToken(TQUESTION);

{ident}          return processToken(TIDENT);
{intLiteral}     return processToken(INTLITERAL);
{intLiteral}i    return processToken(IMAGLITERAL);
{floatLiteral}i  return processToken(IMAGLITERAL);

"\""             return processStringLiteral("\"");
"\'"             return processStringLiteral("\'");

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

void lexerScanString(const char* string) {
  yy_scan_string(string);
}

void lexerResetFile() {
  YY_NEW_FILE;
}

static int processToken(int t) {
  countToken(yytext);

  if (captureTokens) {
    if (t == TASSIGN ||
        t == TDOTDOTDOT)
      strcat(captureString, " ");

    if (t != TLCBR)
      strcat(captureString, yytext);

    if (t == TCOMMA  ||
        t == TPARAM  ||
        t == TZIP    ||
        t == TTYPE   ||
        t == TCONST  ||
        t == TIN     ||
        t == TINOUT  ||
        t == TOUT    ||
        t == TREF    ||
        t == TCOLON  ||
        t == TASSIGN ||
        t == TRSBR)
      strcat(captureString, " ");
  }

  /* processToken means we are parsing Chapel */
  BEGIN(INITIAL);

  return t;
}


static int processStringLiteral(const char* q) {
  yylval.pch = eatStringLiteral(q);

  countToken(astr(q, yylval.pch, q));

  if (captureTokens) {
    strcat(captureString, yytext);
    strcat(captureString, yylval.pch);
    strcat(captureString, yytext);
  }

  /* string literals only in Chapel */
  BEGIN(INITIAL);

  return STRINGLITERAL;
}

static int processExtern() {
  countToken(yytext);

  if (captureTokens) {
    strcat(captureString, yytext);
  }

  BEGIN(externmode);

  return TEXTERN;
}

static int processExternCode() {
  yylval.pch = eatExternCode();

  countToken(astr(yylval.pch));

  if (captureTokens) {
    strcat(captureString, yylval.pch);
  }

  /* only one { } block is special */

  BEGIN(INITIAL);

  return EXTERNCODE;
}

static void processWhitespace(const char* tabOrSpace) {
  // might eventually want to keep track of column numbers and do
  // something here
}


static void processInvalidToken() {
  yyerror("Invalid token");
}
