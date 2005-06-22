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

%}

digit           [0-9]
letter          [_a-zA-Z]
ident           \??{letter}({letter}|{digit}|\?)*
intLiteral      {digit}+{digit}*      
floatLiteral    {digit}+"."{digit}+

%%

and             return TAND;
atomic          return TATOMIC;
break           return TBREAK;
by              return TBY;
call            return TCALL;
class           return TCLASS;
cobegin         return TCOBEGIN;
config          return TCONFIG;
const           return TCONST;
constructor     return TCONSTRUCTOR;
continue        return TCONTINUE;
do              return TDO;
domain          return TDOMAIN;
else            return TELSE;
enum            return TENUM;
for             return TFOR;
forall          return TFORALL;
function        return TFUNCTION;
goto            return TGOTO;
if              return TIF;
in              return TIN;
index           return TINDEX;
inout           return TINOUT;
iterator        return TITERATOR;
label           return TLABEL;
let             return TLET;
like            return TLIKE;
mod             return TMOD;
module          return TMODULE;
nil             return TNIL;
not             return TNOT;
of              return TOF;
or              return TOR;
otherwise       return TOTHERWISE;
out             return TOUT;
parameter       return TPARAMETER;
pragma          return TPRAGMA;
record          return TRECORD;
reduce          return TREDUCE;
return          return TRETURN;
scan            return TSCAN;
select          return TSELECT;
static          return TSTATIC;
then            return TTHEN;
type            return TTYPE;
union           return TUNION;
use             return TUSE;
var             return TVAR;
when            return TWHEN;
where           return TWHERE;
while           return TWHILE;
with            return TWITH;
yield           return TYIELD;

"="             return TASSIGN;
"+="            return TASSIGNPLUS;
"-="            return TASSIGNMINUS;
"*="            return TASSIGNMULTIPLY;
"/="            return TASSIGNDIVIDE;
"&="            return TASSIGNBAND;
"|="            return TASSIGNBOR;
"^="            return TASSIGNBXOR;
"#="            return TASSIGNSEQCAT;

".."            return TDOTDOT;

"&"             return TBAND;
"|"             return TBOR;
"^"             return TBXOR;
"~"             return TBNOT;

"=="            return TEQUAL;
"!="            return TNOTEQUAL;
"<="            return TLESSEQUAL;
">="            return TGREATEREQUAL;
"<"             return TLESS;
">"             return TGREATER;

"+"             return TPLUS;
"-"             return TMINUS;
"*"             return TSTAR;
"/"             return TDIVIDE;

"**"            return TEXP;

"#"             return TSEQCAT;

"!:"            return TNOTCOLON;
":"             return TCOLON;
";"             return TSEMI;
","             return TCOMMA;
"."             return TDOT;
"("             return TLP;
")"             return TRP;
"(/"            return TSEQBEGIN;
"/)"            return TSEQEND;
"["             return TLSBR;
"]"             return TRSBR;
"{"             return TLCBR;
"}"             return TRCBR;

"?"             return TQUESTION;

{ident}         {
                  if (yytext[0] == '?') {
                    return QUERY_IDENT;
                  }
                  return TIDENT;
                }
{intLiteral}    return INTLITERAL;
{floatLiteral}  return FLOATLITERAL;
{intLiteral}i   return COMPLEXLITERAL;
{floatLiteral}i return COMPLEXLITERAL;
"\""            {
                  register int c;
                  int bufflen = 256;
                  static char* stringbuffer = 
                    (char*)MALLOC(bufflen*sizeof(char));
                  int stringlen = 0;

                  while (1) {
                    while ((c = yyinput()) != '\"' && c != EOF) {
                    FORCE_NEXT:
                      stringbuffer[stringlen] = c;
                      stringlen++;
                      if (stringlen == bufflen) {
                        bufflen *= 2;
                        stringbuffer = (char*)REALLOC(stringbuffer,
                                                      bufflen*sizeof(char));
                      }
                      if (c == '\\') {
                        c = yyinput();
                        if (c != EOF) {
                          goto FORCE_NEXT;
                        }
                      }
                    } /* eat up string */
                    stringbuffer[stringlen] = '\0';
                    if (c == EOF) {
                      yyerror("EOF in string");
                    } else {
                      yylval.pch = stringbuffer;
                      return STRINGLITERAL;
                    }
                  }
                }

"\'"            {
                  register int c;
                  int bufflen = 256;
                  static char* stringbuffer = 
                    (char*)MALLOC(bufflen*sizeof(char));
                  int stringlen = 0;

                  while (1) {
                    while ((c = yyinput()) != '\'' && c != EOF) {
                      if (c == '\"') {
                        stringbuffer[stringlen] = '\\';
                        stringlen++;
                        if (stringlen == bufflen) {
                          bufflen *= 2;
                          stringbuffer = (char*)REALLOC(stringbuffer,
                                                        bufflen*sizeof(char));
                        }
                      }
                    FORCE_NEXT2:
                      stringbuffer[stringlen] = c;
                      stringlen++;
                      if (stringlen == bufflen) {
                        bufflen *= 2;
                        stringbuffer = (char*)REALLOC(stringbuffer,
                                                      bufflen*sizeof(char));
                      }
                      if (c == '\\') {
                        c = yyinput();
                        if (c != EOF) {
                          goto FORCE_NEXT2;
                        }
                      }
                    } /* eat up string */
                    stringbuffer[stringlen] = '\0';
                    if (c == EOF) {
                      yyerror("EOF in string");
                    } else {
                      yylval.pch = stringbuffer;
                      return STRINGLITERAL;
                    }
                  }
                }

[ \t]           /* no action, eat spaces and tabs */
\n              yylineno++;

"--"            {
                  register int c;
          
                  while (1) {
                    while ( (c = yyinput()) != '\n' && c != EOF ) {
                      //                      printf("%c", c);
                      //                      fflush(stdout);
                    }    /* eat up text of comment */
                    if (c == EOF) {
                      yyerror("EOF in comment");
                    } else {
                      yylineno++;
                      break;
                    }
                  }
                }

"/*"            {
                  register int c;
          
                  while (1) {
                    while ((c = yyinput()) != '*' && c != EOF ) {
                      if (c == '\n') {
                        yylineno++;
                      }
                    }    /* eat up text of comment */
          
                    if ( c == '*' ) {
                      while ( (c = yyinput()) == '*' ) {
                      }
                      if ( c == '/' ) {
                        break;    /* found the end */
                      } else if (c == '\n') {
                        yylineno++;
                      }
                    } else {      // c == EOF
                      yyerror( "EOF in comment" );
                      break;
                    }
                  }
                }


%%

#ifdef STANDALONE 
main(int argc, char* argv[]) {
  /* below copied from flex manpage */
  ++argv, --argc;  

  if (argc > 0)
    yyin = fopen(argv[0], "r");
  else
    yyin = stdin;

  yylex();
}
#endif
