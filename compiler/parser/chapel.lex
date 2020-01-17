/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
%option header-file="../include/flex-chapel.h"

%option noyywrap
%option nounput

/* These options create a re-entrant scanner that returns
     an integer to indicate the token type
     a  bison-style YYSTYPE by reference. The value will always be in yylval->pch.
     a  bison-style YYLTYPE by reference.

     i.e. int yylex(YYSTYPE*, YYLTYPE*, yyscan_t yyscanner);
*/

%option reentrant
%option bison-bridge
%option bison-locations

/*
   Provide a condition stack
   This is used to alter the handling of "{" when it appears immediately after "extern"
*/

%option stack

%{

#include "bison-chapel.h"
#include "docsDriver.h"
#include "parser.h"

#include <cstdio>

//
// Make sure exit is undefined:
//

#ifdef exit
#undef exit
#endif

// And redefine it to call our exit routine:
#define exit(x) clean_exit(x)

static int  processIdentifier(yyscan_t scanner, bool queried);
static int  processToken(yyscan_t scanner, int t);
static int  processStringLiteral(yyscan_t scanner, const char* q, int type);
static int  processMultilineStringLiteral(yyscan_t scanner, const char* q,
                                          int type);

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
forwarding       return processToken(yyscanner, TFORWARDING);
if               return processToken(yyscanner, TIF);
imag             return processToken(yyscanner, TIMAG);
in               return processToken(yyscanner, TIN);
index            return processToken(yyscanner, TINDEX);
inline           return processToken(yyscanner, TINLINE);
inout            return processToken(yyscanner, TINOUT);
int              return processToken(yyscanner, TINT);
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

"\"\"\""         return processMultilineStringLiteral(yyscanner, "\"", STRINGLITERAL);
"'''"            return processMultilineStringLiteral(yyscanner, "'", STRINGLITERAL);
"b\"\"\""        return processMultilineStringLiteral(yyscanner, "\"", BYTESLITERAL);
"b'''"           return processMultilineStringLiteral(yyscanner, "'", BYTESLITERAL);
"\""             return processStringLiteral(yyscanner, "\"", STRINGLITERAL);
"\'"             return processStringLiteral(yyscanner, "\'", STRINGLITERAL);
"b\""            return processStringLiteral(yyscanner, "\"", BYTESLITERAL);
"b\'"            return processStringLiteral(yyscanner, "\'", BYTESLITERAL);
"c\""            return processStringLiteral(yyscanner, "\"", CSTRINGLITERAL);
"c\'"            return processStringLiteral(yyscanner, "\'", CSTRINGLITERAL);
"//"             return processSingleLineComment(yyscanner);
"/*"             return processBlockComment(yyscanner);

\n               return processNewline(yyscanner);

[ \t\r\f]        processWhitespace(yyscanner);
.                processInvalidToken(yyscanner);

%%

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

#include "countTokens.h"
#include "misc.h"
#include "stringutil.h"

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

  chplLineno++;

  yyLloc->first_column = 0;
  yyLloc->last_column  = 0;

  yyLloc->first_line   = chplLineno;
  yyLloc->last_line    = chplLineno;

  countNewline();

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

  yyLval->pch = astr(yyget_text(scanner));

  return retval;
}

static int processToken(yyscan_t scanner, int t) {
  YYSTYPE* yyLval = yyget_lval(scanner);

  countToken(yyget_text(scanner));

  yyLval->pch = yyget_text(scanner);

  if (captureTokens) {
    if (t == TASSIGN ||
        t == TDOTDOTDOT) {
      captureString.push_back(' ');
    }

    if (t != TLCBR) {
      captureString.append(yyget_text(scanner));
    }

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
        t == TRSBR ||
        t == TBORROWED ||
        t == TUNMANAGED ||
        t == TOWNED ||
        t == TSHARED) {
      captureString.push_back(' ');
    }
  }

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

  countToken(q, yyLval->pch, q);

  if (captureTokens) {
    captureString.append(yyText);
    captureString.append(yyLval->pch);
    captureString.append(yyText);
  }

  return type;
}

static int processMultilineStringLiteral(yyscan_t scanner, const char* q,
                                         int type) {
  const char* yyText = yyget_text(scanner);
  YYSTYPE* yyLval = yyget_lval(scanner);
  yyLval->pch = eatMultilineStringLiteral(scanner, q);

  countToken(q, yyLval->pch, q);

  if (captureTokens) {
    captureString.append(yyText);
    captureString.append(yyLval->pch);
    captureString.append(yyText);
  }
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
      ParserContext context(scanner);

      yyText[0] = '\0';
      yyerror(yyLloc, &context, "end-of-line in a string literal without a preceding backslash");
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
        ParserContext context(scanner);
        yyerror(yyLloc, &context, "universal character name not yet supported in string literal");
        addCharEscapeNonprint('t'); // add a valid escape to continue parsing
      } else if ('0' <= c && c <= '7' ) {
        ParserContext context(scanner);
        yyerror(yyLloc, &context, "octal escape not supported in string literal");
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
    ParserContext context(scanner);

    yyerror(yyLloc, &context, "EOF in string");
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
    ParserContext context(scanner);

    yyerror(yyLloc, &context, "EOF in string");
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

  countToken(yyText);

  if (captureTokens) {
    captureString.append(yyText);
  }

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

  countToken(astr(yyLval->pch));

  if (captureTokens) {
    captureString.append(yyLval->pch);
  }

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
      ParserContext context(scanner);

      switch (state) {
        case in_code:
          // there was no match to the {
          yyerror(yyLloc, &context, "Missing } in extern block");
          break;

        case in_single_quote:
        case in_single_quote_backslash:
          yyerror(yyLloc, &context, "Runaway \'string\' in extern block");
          break;

        case in_double_quote:
        case in_double_quote_backslash:
          yyerror(yyLloc, &context, "Runaway \"string\" in extern block");
          break;

        case in_single_line_comment:
          yyerror(yyLloc, &context, "Missing newline after extern block // comment");
          break;

        case in_multi_line_comment:
          yyerror(yyLloc, &context, "Runaway /* comment */ in extern block");
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

  int         len          = strlen(fDocsCommentLabel);
  int         labelIndex   = (len >= 2) ? 2 : 0;

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

    if (len != 0 && c == fDocsCommentLabel[len - d])
      d++;
    else
      d = 1;

    if (lastc == '*' && c == '/' && lastlastc != '/') { // close comment
      if(labelIndex == len && d != len + 1) {
        depth--;
        badComment = true;
      }
      else
        depth--;
      
      d = 1;
    } else if (lastc == '/' && c == '*') { // start nested
      depth++;
      // keep track of the start of the last nested comment
      nestedStartLine = chplLineno;
    } else if (c == 0) {
      ParserContext context(scanner);

      fprintf(stderr, "%s:%d: unterminated comment started here\n",
              startFilename, startLine);
      if( nestedStartLine >= 0 ) {
        fprintf(stderr, "%s:%d: nested comment started here\n",
                startFilename, nestedStartLine);
      }
      yyerror(yyLloc, &context, "EOF in comment");
      break;
    }
  }

  // back up two to not print */ again.
  if (stringBuffer.size() >= 2)
    stringBuffer.resize(stringBuffer.size()-2);

  // back up further if the user has specified a special form of commenting
  if (len > 2 && labelIndex == len)
    stringBuffer.resize(stringBuffer.size() - (len - 2));

  // Saves the comment grabbed to the comment field of the location struct,
  // for use when the --docs flag is implemented
  if (fDocs && labelIndex == len) {
    wholeComment += stringBuffer;

    if (len > 2) {
      len          = len - 2;
      wholeComment = wholeComment.substr(len);
    }

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
  ParserContext context(scanner);
  YYLTYPE*      yyLloc = yyget_lloc(scanner);

  yyerror(yyLloc, &context, "Invalid token");
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

