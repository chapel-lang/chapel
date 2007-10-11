%option noyywrap
%option nounput

%{

#include <stdio.h>
#include "lexyacc.h"
#include "chapel.tab.h"

#ifndef processToken
#define processToken(t) \
  countToken(yytext);  \
  return(t)
#endif

#define processStringLiteral(q)     \
  yylval.pch = eatStringLiteral(q); \
  countToken(stringcat(q, yylval.pch, q)); \
  return(STRINGLITERAL)


%}

digit           [0-9]
bit             [0-1]
hexDigit        [0-9a-fA-F]
letter          [_a-zA-Z$]
ident           {letter}({letter}|{digit})*
binaryLiteral   0b{bit}+
hexLiteral      0x{hexDigit}+
intLiteral      {digit}+|{binaryLiteral}|{hexLiteral}
exponent        [Ee][\+\-]?{digit}+
floatLiteral    {digit}*"."{digit}+{exponent}?|{digit}+{exponent}


%%

atomic          processToken(TATOMIC);
begin           processToken(TBEGIN);
break           processToken(TBREAK);
by              processToken(TBY);
class           processToken(TCLASS);
cobegin         processToken(TCOBEGIN);
coforall        processToken(TCOFORALL);
compilerError   processToken(TCOMPILERERROR);
config          processToken(TCONFIG);
const           processToken(TCONST);
continue        processToken(TCONTINUE);
def             processToken(TDEF);
distributed     processToken(TDISTRIBUTED);
do              processToken(TDO);
domain          processToken(TDOMAIN);
else            processToken(TELSE);
enum            processToken(TENUM);
_extern         processToken(TEXTERN);
for             processToken(TFOR);
forall          processToken(TFORALL);
goto            processToken(TGOTO);
if              processToken(TIF);
in              processToken(TIN);
index           processToken(TINDEX);
inout           processToken(TINOUT);
label           processToken(TLABEL);
let             processToken(TLET);
module          processToken(TMODULE);
nil             processToken(TNIL);
of              processToken(TOF);
on              processToken(TON);
ordered         processToken(TORDERED);
otherwise       processToken(TOTHERWISE);
out             processToken(TOUT);
param           processToken(TPARAM);
pragma          processToken(TPRAGMA);
__primitive     processToken(TPRIMITIVE);
record          processToken(TRECORD);
reduce          processToken(TREDUCE);
return          processToken(TRETURN);
scan            processToken(TSCAN);
select          processToken(TSELECT);
serial          processToken(TSERIAL);
single          processToken(TSINGLE);
sparse          processToken(TSPARSE);
subdomain       processToken(TSUBDOMAIN);
sync            processToken(TSYNC);
then            processToken(TTHEN);
type            processToken(TTYPE);
union           processToken(TUNION);
use             processToken(TUSE);
var             processToken(TVAR);
when            processToken(TWHEN);
where           processToken(TWHERE);
while           processToken(TWHILE);
yield           processToken(TYIELD);

"="             processToken(TASSIGN);
"+="            processToken(TASSIGNPLUS);
"-="            processToken(TASSIGNMINUS);
"*="            processToken(TASSIGNMULTIPLY);
"/="            processToken(TASSIGNDIVIDE);
"**="           processToken(TASSIGNEXP);
"%="            processToken(TASSIGNMOD);
"&="            processToken(TASSIGNBAND);
"|="            processToken(TASSIGNBOR);
"^="            processToken(TASSIGNBXOR);
"&&="           processToken(TASSIGNLAND);
"||="           processToken(TASSIGNLOR);
"<<="           processToken(TASSIGNSL);
">>="           processToken(TASSIGNSR);

"=>"            processToken(TALIAS);

"<=>"           processToken(TSWAP);

".."            processToken(TDOTDOT);
"..."           processToken(TDOTDOTDOT);

"&&"            processToken(TAND);
"||"            processToken(TOR);
"!"             processToken(TNOT);

"&"             processToken(TBAND);
"|"             processToken(TBOR);
"^"             processToken(TBXOR);
"~"             processToken(TBNOT);

"<<"            processToken(TSHIFTLEFT);
">>"            processToken(TSHIFTRIGHT);

"=="            processToken(TEQUAL);
"!="            processToken(TNOTEQUAL);
"<="            processToken(TLESSEQUAL);
">="            processToken(TGREATEREQUAL);
"<"             processToken(TLESS);
">"             processToken(TGREATER);

"+"             processToken(TPLUS);
"-"             processToken(TMINUS);
"*"             processToken(TSTAR);
"/"             processToken(TDIVIDE);
"%"             processToken(TMOD);

"**"            processToken(TEXP);

"!:"            processToken(TNOTCOLON);
":"             processToken(TCOLON);
";"             processToken(TSEMI);
","             processToken(TCOMMA);
"."             processToken(TDOT);
"("             processToken(TLP);
")"             processToken(TRP);
"["             processToken(TLSBR);
"]"             processToken(TRSBR);
"{"             processToken(TLCBR);
"}"             processToken(TRCBR);

"?"             processToken(TQUESTION);

{ident}         processToken(TIDENT);
{intLiteral}    processToken(INTLITERAL);
{floatLiteral}  processToken(REALLITERAL);
{intLiteral}i   processToken(IMAGLITERAL);
{floatLiteral}i processToken(IMAGLITERAL);

"\""            processStringLiteral("\"");
"\'"            processStringLiteral("\'");

[ \t\r]         processWhitespace(yytext);
\n              processNewline();

"//"            processSingleLineComment();

"/*"            processMultiLineComment();

.               processInvalidToken();

%%

// it's difficult to prototype yyinput, so this is a way of exporting
// it to other files in a controlled way

int getNextYYChar() {
  return yyinput();
}

/*
AList<Stmt>* parse_string(char* string) {
  yyblock = NULL;
  yy_scan_string(string);
  yyparse();
  return yyblock;
}
*/
