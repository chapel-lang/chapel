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

%option outfile="flex-chapel.cpp"
%option header-file="../include/flex-chapel.h"

%option noyywrap
%option nounput

%{

#include "bison-chapel.h"
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

static int  processToken(int t);
static int  processStringLiteral(const char* q);
static int  processExtern();
static int  processExternCode();

static void processWhitespace(const char* tabOrSpace);

static void processSingleLineComment();
static void processMultiLineComment();

static void processInvalidToken();

%}

bit              [0-1]
octDigit         [0-7]
digit            [0-9]
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

%s               externmode

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

static void  newString();
static void  addString(const char* str);
static void  addChar(char c);
static void  addCharString(char c);
static int   getNextYYChar();

static int   stringBuffLen = 0;
static int   stringLen     = 0;
static char* stringBuffer  = NULL;

void lexerScanString(const char* string) {
  yy_scan_string(string);
}

void lexerResetFile() {
  YY_NEW_FILE;
}

void processNewline() {
  chplLineno++;

  yylloc.first_column = 0;
  yylloc.last_column  = 0;

  yylloc.first_line   = chplLineno;
  yylloc.last_line    = chplLineno;

  countNewline();
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static char* eatStringLiteral(const char* startChar);

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

static char* eatStringLiteral(const char* startChar) {
  const char startCh = *startChar;
  int        c       = 0;

  newString();

  while ((c = getNextYYChar()) != startCh && c != 0) {
    if (c == '\n') {
      yytext[0] = '\0';
      yyerror("end-of-line in a string literal without a preceeding backslash");
    } else {
      if (startCh == '\'' && c == '\"') {
        addCharString('\\');
      }

      addCharString(c);
    }

    if (c == '\\') {
      c = getNextYYChar();

      if (c == '\n') {
        processNewline();
        addCharString('n');
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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static int processExtern() {
  countToken(yytext);

  if (captureTokens) {
    strcat(captureString, yytext);
  }

  BEGIN(externmode);

  return TEXTERN;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static char* eatExternCode();

// When the lexer calls this function, it has already consumed the first '{'
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

static char* eatExternCode() {
  const int in_code                          = 0;
  const int in_single_quote                  = 1;
  const int in_single_quote_backslash        = 2;
  const int in_double_quote                  = 3;
  const int in_double_quote_backslash        = 4;
  const int in_single_line_comment           = 5;
  const int in_single_line_comment_backslash = 6;
  const int in_multi_line_comment            = 7;

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
    c     = getNextYYChar();

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

    if (c == '\n')
      processNewline();

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

        else if ( c == '"')
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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processWhitespace(const char* tabOrSpace) {
  // might eventually want to keep track of column numbers and do
  // something here
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processSingleLineComment() {
  int c;

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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processMultiLineComment() {
  int         c            = 0;
  int         lastc        = 0;
  int         depth        = 1;
  int         labelIndex   = 0;
  int         len          = 0;
  std::string wholeComment = "";

  newString();
  countCommentLine();

  len = strlen(fDocsCommentLabel);

  if (len >= 2) {
    labelIndex = 2;
  }

  while (depth > 0) {
    int lastlastc = lastc;

    lastc = c;
    c     = getNextYYChar();

    if (c == '\n') {
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

    if (lastc == '*' && c == '/' && lastlastc != '/') { // close comment
      depth--;

    } else if (lastc == '/' && c == '*') { // start nested
      depth++;
    } else if (c == 0) {
      yyerror( "EOF in comment" );
    }
  }

  // back up two to not print */ again.
  if (stringLen >= 2)
    stringLen -= 2;

  // back up further if the user has specified a special form of commenting
  if (len > 2 && labelIndex == len)
    stringLen -= (len - 2);

  stringBuffer[stringLen] = '\0';

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

    yylloc.comment = (char*) astr(wholeComment.c_str());
  }

  countMultiLineComment(stringBuffer);

  newString();
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void processInvalidToken() {
  yyerror("Invalid token");
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void addCharMaybeEscape(char c, bool canEscape);
static char toHex(char c);

static void newString() {
  stringLen = 0;

  if (stringBuffLen) {
    stringBuffer[stringLen] = '\0';
  }
}

static void addString(const char* str) {
  for (int i = 0; str[i]; i++)
    addChar(str[i]);
}

static void addChar(char c) {
  addCharMaybeEscape(c, false);
}

static void addCharString(char c) {
  addCharMaybeEscape(c, true);
}

static void addCharMaybeEscape(char c, bool canEscape) {
  int escape  = canEscape && !(isascii(c) && isprint(c));
  int charlen = escape ? 4 : 1; // convert nonasci to \xNN

  if (stringLen + charlen + 1 > stringBuffLen) {
    stringBuffLen = 2*(stringBuffLen + charlen);
    stringBuffer  = (char*) realloc(stringBuffer,
                                    stringBuffLen * sizeof(char));
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

// Returns the hexadecimal character for 0-16.
static char toHex(char c) {
  return (0 <= c && c <= 9) ? '0' + c : 'A' + (c - 10);
}

static int getNextYYChar() {
  int retval = yyinput();

  if (retval == EOF) {
    retval = 0;
  }

  return retval;
}

