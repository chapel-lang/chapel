%{

#include <stdio.h>
#include <typeinfo>
#include "lexyacc.h"
#include "chapel.tab.h"

/* make sure flex-generated code uses these */
#ifdef USE_GC
#undef malloc
#undef realloc
#undef free
#define malloc MALLOC
#define realloc REALLOC
#define free FREE
#endif

/* avoid warnings */
#define YY_NO_UNPUT

#define processToken(t) \
  countToken(yytext);  \
  return(t)

#define processStringLiteral(q)     \
  yylval.pch = eatStringLiteral(q); \
  countToken(stringcat(q, yylval.pch, q)); \
  return(STRINGLITERAL)


%}

digit           [0-9]
letter          [_a-zA-Z]
ident           {letter}({letter}|{digit}|\?)*
intLiteral      {digit}+{digit}*      
floatLiteral    {digit}+"."{digit}+

%%

and             processToken(TAND);
atomic          processToken(TATOMIC);
break           processToken(TBREAK);
by              processToken(TBY);
call            processToken(TCALL);
class           processToken(TCLASS);
cobegin         processToken(TCOBEGIN);
config          processToken(TCONFIG);
const           processToken(TCONST);
constructor     processToken(TCONSTRUCTOR);
continue        processToken(TCONTINUE);
do              processToken(TDO);
else            processToken(TELSE);
enum            processToken(TENUM);
for             processToken(TFOR);
forall          processToken(TFORALL);
function        processToken(TFUNCTION);
goto            processToken(TGOTO);
if              processToken(TIF);
in              processToken(TIN);
index           processToken(TINDEX);
inout           processToken(TINOUT);
iterator        processToken(TITERATOR);
label           processToken(TLABEL);
let             processToken(TLET);
like            processToken(TLIKE);
mod             processToken(TMOD);
module          processToken(TMODULE);
nil             processToken(TNIL);
not             processToken(TNOT);
of              processToken(TOF);
or              processToken(TOR);
ordered         processToken(TORDERED);
otherwise       processToken(TOTHERWISE);
out             processToken(TOUT);
param           processToken(TPARAM);
pragma          processToken(TPRAGMA);
record          processToken(TRECORD);
reduce          processToken(TREDUCE);
return          processToken(TRETURN);
scan            processToken(TSCAN);
select          processToken(TSELECT);
static          processToken(TSTATIC);
then            processToken(TTHEN);
type            processToken(TTYPE);
typedef         processToken(TTYPEDEF);
union           processToken(TUNION);
use             processToken(TUSE);
var             processToken(TVAR);
when            processToken(TWHEN);
where           processToken(TWHERE);
while           processToken(TWHILE);
with            processToken(TWITH);
yield           processToken(TYIELD);

"_"             processToken(TUNSPECIFIED);

"="             processToken(TASSIGN);
"+="            processToken(TASSIGNPLUS);
"-="            processToken(TASSIGNMINUS);
"*="            processToken(TASSIGNMULTIPLY);
"/="            processToken(TASSIGNDIVIDE);
"&="            processToken(TASSIGNBAND);
"|="            processToken(TASSIGNBOR);
"^="            processToken(TASSIGNBXOR);
"#="            processToken(TASSIGNSEQCAT);

".."            processToken(TDOTDOT);

"&"             processToken(TBAND);
"|"             processToken(TBOR);
"^"             processToken(TBXOR);
"~"             processToken(TBNOT);

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

"**"            processToken(TEXP);

"#"             processToken(TSEQCAT);

"!:"            processToken(TNOTCOLON);
":"             processToken(TCOLON);
";"             processToken(TSEMI);
","             processToken(TCOMMA);
"."             processToken(TDOT);
"("             processToken(TLP);
")"             processToken(TRP);
"(/"            processToken(TSEQBEGIN);
"/)"            processToken(TSEQEND);
"["             processToken(TLSBR);
"]"             processToken(TRSBR);
"{"             processToken(TLCBR);
"}"             processToken(TRCBR);

"?"             processToken(TQUESTION);

{ident}         processToken(TIDENT);
{intLiteral}    processToken(INTLITERAL);
{floatLiteral}  processToken(FLOATLITERAL);
{intLiteral}i   processToken(IMAGLITERAL);
{floatLiteral}i processToken(IMAGLITERAL);

"\""            processStringLiteral("\"");
"\'"            processStringLiteral("\'");

[ \t]           processWhitespace(yytext);
\n              processNewline();

"--"            processSingleLineComment();
"//"            processSingleLineComment();

"/*"            processMultiLineComment();


%%

// it's difficult to prototype yyinput, so this is a way of exporting
// it to other files in a controlled way

int getNextYYChar() {
  return yyinput();
}
