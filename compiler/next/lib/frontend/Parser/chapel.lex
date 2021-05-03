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

%option outfile="flex-chapel.cpp"
%option header-file="flex-chapel.h"

%option noyywrap
%option nounput

/* These options create a re-entrant scanner that returns
     * an integer to indicate the token type
     * a bison-style YYSTYPE by reference. (value in yylval->pch)
     * a bison-style YYLTYPE by reference.

     i.e. int yylex(YYSTYPE*, YYLTYPE*, yyscan_t yyscanner);
*/

%option reentrant
%option bison-bridge
%option bison-locations

%option extra-type="ParserContext*"

/*
   Provide a condition stack
   This is used to alter the handling of "{" when it appears immediately after "extern"
*/

%option stack

%{

#include "bison-chapel.h"

#include <cstdio>

static int  processIdentifier(yyscan_t scanner, bool queried);
static int  processToken(yyscan_t scanner, int t);
static int  processStringLiteral(yyscan_t scanner, const char* q, int type);
static int  processTripleStringLiteral(yyscan_t scanner, const char* q, int type);

static int  processExtern(yyscan_t scanner);
static int  processExternCode(yyscan_t scanner);

static void processWhitespace(yyscan_t scanner);

static int  processSingleLineComment(yyscan_t scanner);
static int  processBlockComment(yyscan_t scanner);

static void processInvalidToken(yyscan_t scanner);

static bool yy_has_state(yyscan_t scanner);

%}

bit              [0-1]
octDigit         [0-7]
digit            [0-9]
hexDigit         [0-9a-fA-F]

letter           [_a-zA-Z]

ident            {letter}({letter}|{digit}|"$")*
queriedIdent     \?{ident}

binaryLiteral    0[bB]{bit}(_|{bit})*
octalLiteral     0[oO]{octDigit}(_|{octDigit})*
decimalLiteral   {digit}(_|{digit})*
hexLiteral       0[xX]{hexDigit}(_|{hexDigit})*
intLiteral       {binaryLiteral}|{octalLiteral}|{decimalLiteral}|{hexLiteral}

digitsOrSeps     {digit}(_|{digit})*
exponent         [Ee][\+\-]?{digitsOrSeps}
floatLiteral1    {digitsOrSeps}?"."{digitsOrSeps}({exponent})?
floatLiteral2    {digitsOrSeps}"."{exponent}
floatLiteral3    {digitsOrSeps}{exponent}

/* hex float literals, have decimal exponents indicating the power of 2 */
hexDigitsOrSeps  {hexDigit}(_|{hexDigit})*
hexDecExponent   [Pp][\+\-]?{digitsOrSeps}
floatLiteral4    0[xX]{hexDigitsOrSeps}?"."{hexDigitsOrSeps}({hexDecExponent})?
floatLiteral5    0[xX]{hexDigitsOrSeps}"."{hexDecExponent}
floatLiteral6    0[xX]{hexDigitsOrSeps}{hexDecExponent}

decFloatLiteral  {floatLiteral1}|{floatLiteral2}|{floatLiteral3}
hexFloatLiteral  {floatLiteral4}|{floatLiteral5}|{floatLiteral6}

floatLiteral     {decFloatLiteral}|{hexFloatLiteral}

%s               externmode

%%

align            return processToken(yyscanner, TALIGN);
as               return processToken(yyscanner, TAS);
atomic           return processToken(yyscanner, TATOMIC);
begin            return processToken(yyscanner, TBEGIN);
bool             return processToken(yyscanner, TBOOL);
borrowed         return processToken(yyscanner, TBORROWED);
break            return processToken(yyscanner, TBREAK);
by               return processToken(yyscanner, TBY);
bytes            return processToken(yyscanner, TBYTES);
catch            return processToken(yyscanner, TCATCH);
class            return processToken(yyscanner, TCLASS);
cobegin          return processToken(yyscanner, TCOBEGIN);
coforall         return processToken(yyscanner, TCOFORALL);
complex          return processToken(yyscanner, TCOMPLEX);
config           return processToken(yyscanner, TCONFIG);
const            return processToken(yyscanner, TCONST);
continue         return processToken(yyscanner, TCONTINUE);
defer            return processToken(yyscanner, TDEFER);
delete           return processToken(yyscanner, TDELETE);
dmapped          return processToken(yyscanner, TDMAPPED);
do               return processToken(yyscanner, TDO);
domain           return processToken(yyscanner, TDOMAIN);
else             return processToken(yyscanner, TELSE);
enum             return processToken(yyscanner, TENUM);
export           return processToken(yyscanner, TEXPORT);
except           return processToken(yyscanner, TEXCEPT);
extern           return processExtern(yyscanner);
false            return processToken(yyscanner, TFALSE);
for              return processToken(yyscanner, TFOR);
forall           return processToken(yyscanner, TFORALL);
foreach          return processToken(yyscanner, TFOREACH);
forwarding       return processToken(yyscanner, TFORWARDING);
if               return processToken(yyscanner, TIF);
imag             return processToken(yyscanner, TIMAG);
import           return processToken(yyscanner, TIMPORT);
in               return processToken(yyscanner, TIN);
include          return processToken(yyscanner, TINCLUDE);
index            return processToken(yyscanner, TINDEX);
inline           return processToken(yyscanner, TINLINE);
inout            return processToken(yyscanner, TINOUT);
int              return processToken(yyscanner, TINT);
implements       return processToken(yyscanner, TIMPLEMENTS);
interface        return processToken(yyscanner, TINTERFACE);
iter             return processToken(yyscanner, TITER);
label            return processToken(yyscanner, TLABEL);
lambda           return processToken(yyscanner, TLAMBDA);
let              return processToken(yyscanner, TLET);
lifetime         return processToken(yyscanner, TLIFETIME);
local            return processToken(yyscanner, TLOCAL);
locale           return processToken(yyscanner, TLOCALE);
module           return processToken(yyscanner, TMODULE);
new              return processToken(yyscanner, TNEW);
nil              return processToken(yyscanner, TNIL);
noinit           return processToken(yyscanner, TNOINIT);
none             return processToken(yyscanner, TNONE);
nothing          return processToken(yyscanner, TNOTHING);
on               return processToken(yyscanner, TON);
only             return processToken(yyscanner, TONLY);
operator         return processToken(yyscanner, TOPERATOR);
otherwise        return processToken(yyscanner, TOTHERWISE);
out              return processToken(yyscanner, TOUT);
override         return processToken(yyscanner, TOVERRIDE);
owned            return processToken(yyscanner, TOWNED);
param            return processToken(yyscanner, TPARAM);
pragma           return processToken(yyscanner, TPRAGMA);
__primitive      return processToken(yyscanner, TPRIMITIVE);
private          return processToken(yyscanner, TPRIVATE);
proc             return processToken(yyscanner, TPROC);
prototype        return processToken(yyscanner, TPROTOTYPE);
public           return processToken(yyscanner, TPUBLIC);
real             return processToken(yyscanner, TREAL);
record           return processToken(yyscanner, TRECORD);
reduce           return processToken(yyscanner, TREDUCE);
ref              return processToken(yyscanner, TREF);
require          return processToken(yyscanner, TREQUIRE);
return           return processToken(yyscanner, TRETURN);
scan             return processToken(yyscanner, TSCAN);
select           return processToken(yyscanner, TSELECT);
serial           return processToken(yyscanner, TSERIAL);
shared           return processToken(yyscanner, TSHARED);
single           return processToken(yyscanner, TSINGLE);
sparse           return processToken(yyscanner, TSPARSE);
string           return processToken(yyscanner, TSTRING);
subdomain        return processToken(yyscanner, TSUBDOMAIN);
sync             return processToken(yyscanner, TSYNC);
then             return processToken(yyscanner, TTHEN);
this             return processToken(yyscanner, TTHIS);
throw            return processToken(yyscanner, TTHROW);
throws           return processToken(yyscanner, TTHROWS);
true             return processToken(yyscanner, TTRUE);
try              return processToken(yyscanner, TTRY);
"try!"           return processToken(yyscanner, TTRYBANG);
type             return processToken(yyscanner, TTYPE);
uint             return processToken(yyscanner, TUINT);
union            return processToken(yyscanner, TUNION);
unmanaged        return processToken(yyscanner, TUNMANAGED);
use              return processToken(yyscanner, TUSE);
var              return processToken(yyscanner, TVAR);
void             return processToken(yyscanner, TVOID);
when             return processToken(yyscanner, TWHEN);
where            return processToken(yyscanner, TWHERE);
while            return processToken(yyscanner, TWHILE);
with             return processToken(yyscanner, TWITH);
yield            return processToken(yyscanner, TYIELD);
zip              return processToken(yyscanner, TZIP);

"_"              return processToken(yyscanner, TUNDERSCORE);

"="              return processToken(yyscanner, TASSIGN);
"+="             return processToken(yyscanner, TASSIGNPLUS);
"-="             return processToken(yyscanner, TASSIGNMINUS);
"*="             return processToken(yyscanner, TASSIGNMULTIPLY);
"/="             return processToken(yyscanner, TASSIGNDIVIDE);
"**="            return processToken(yyscanner, TASSIGNEXP);
"%="             return processToken(yyscanner, TASSIGNMOD);
"&="             return processToken(yyscanner, TASSIGNBAND);
"|="             return processToken(yyscanner, TASSIGNBOR);
"^="             return processToken(yyscanner, TASSIGNBXOR);
"&&="            return processToken(yyscanner, TASSIGNLAND);
"||="            return processToken(yyscanner, TASSIGNLOR);
"<<="            return processToken(yyscanner, TASSIGNSL);
">>="            return processToken(yyscanner, TASSIGNSR);
"reduce="        return processToken(yyscanner, TASSIGNREDUCE);

"init="          return processToken(yyscanner, TINITEQUALS);

"=>"             return processToken(yyscanner, TALIAS);

"<=>"            return processToken(yyscanner, TSWAP);

"#"              return processToken(yyscanner, THASH);
".."             return processToken(yyscanner, TDOTDOT);
"..<"            return processToken(yyscanner, TDOTDOTOPENHIGH);
                 /* The following cases would extend the current '..<'
                    open range interval constructor to also support
                    '<..' and '<..<'.  This concept didn't win enough
                    support to merge as present, but are here in case
                    we change our minds in a future release. */
                 /* "<.."            return processToken(yyscanner, TDOTDOTOPENLOW); */
                 /* "<..<"           return processToken(yyscanner, TDOTDOTOPENBOTH); */
"..."            return processToken(yyscanner, TDOTDOTDOT);

"&&"             return processToken(yyscanner, TAND);
"||"             return processToken(yyscanner, TOR);
"!"              return processToken(yyscanner, TBANG);

"&"              return processToken(yyscanner, TBAND);
"|"              return processToken(yyscanner, TBOR);
"^"              return processToken(yyscanner, TBXOR);
"~"              return processToken(yyscanner, TBNOT);

"<<"             return processToken(yyscanner, TSHIFTLEFT);
">>"             return processToken(yyscanner, TSHIFTRIGHT);

"=="             return processToken(yyscanner, TEQUAL);
"!="             return processToken(yyscanner, TNOTEQUAL);
"<="             return processToken(yyscanner, TLESSEQUAL);
">="             return processToken(yyscanner, TGREATEREQUAL);
"<"              return processToken(yyscanner, TLESS);
">"              return processToken(yyscanner, TGREATER);

"+"              return processToken(yyscanner, TPLUS);
"-"              return processToken(yyscanner, TMINUS);
"*"              return processToken(yyscanner, TSTAR);
"/"              return processToken(yyscanner, TDIVIDE);
"%"              return processToken(yyscanner, TMOD);
"--"             return processToken(yyscanner, TMINUSMINUS);
"++"             return processToken(yyscanner, TPLUSPLUS);

"**"             return processToken(yyscanner, TEXP);

":"              return processToken(yyscanner, TCOLON);
";"              return processToken(yyscanner, TSEMI);
","              return processToken(yyscanner, TCOMMA);
"."              return processToken(yyscanner, TDOT);
"("              return processToken(yyscanner, TLP);
")"              return processToken(yyscanner, TRP);
"["              return processToken(yyscanner, TLSBR);
"]"              return processToken(yyscanner, TRSBR);
<externmode>"{"  return processExternCode(yyscanner);
<INITIAL>"{"     return processToken(yyscanner, TLCBR);
"}"              return processToken(yyscanner, TRCBR);
"<~>"            return processToken(yyscanner, TIO);
"?"              return processToken(yyscanner, TQUESTION);

{intLiteral}     return processToken(yyscanner, INTLITERAL);
{floatLiteral}   return processToken(yyscanner, REALLITERAL);

{intLiteral}i    return processToken(yyscanner, IMAGLITERAL);
{floatLiteral}i  return processToken(yyscanner, IMAGLITERAL);

{ident}          return processIdentifier(yyscanner, false);
{queriedIdent}   return processIdentifier(yyscanner, true);

"\"\"\""         return processTripleStringLiteral(yyscanner, "\"", STRINGLITERAL);
"'''"            return processTripleStringLiteral(yyscanner, "'", STRINGLITERAL);
"b\"\"\""        return processTripleStringLiteral(yyscanner, "\"", BYTESLITERAL);
"b'''"           return processTripleStringLiteral(yyscanner, "'", BYTESLITERAL);
"\""             return processStringLiteral(yyscanner, "\"", STRINGLITERAL);
"'"              return processStringLiteral(yyscanner, "'", STRINGLITERAL);
"b\""            return processStringLiteral(yyscanner, "\"", BYTESLITERAL);
"b'"             return processStringLiteral(yyscanner, "'", BYTESLITERAL);
"c\""            return processStringLiteral(yyscanner, "\"", CSTRINGLITERAL);
"c'"             return processStringLiteral(yyscanner, "'", CSTRINGLITERAL);
"//"             return processSingleLineComment(yyscanner);
"/*"             return processBlockComment(yyscanner);

\n               return processNewline(yyscanner);

[ \t\r\f]        processWhitespace(yyscanner);
.                processInvalidToken(yyscanner);

%%

#include "lexer-help.h"
