/*
 * A mini-Chapel scanner.  Derived from code that
 * Shannon wrote in learning to use flex.
 *
 * Brad, 6/2004
 */

%{

#include <stdio.h>
#include <typeinfo>
#include "lexyacc.h"
#include "chapel.tab.h"

/* make sure flex-generated code uses these */
#undef malloc
#undef realloc
#undef free
#define malloc MALLOC
#define realloc REALLOC
#define free FREE

/* avoid warnings */
#define YY_NO_UNPUT

%}

digit           [0-9]
letter          [_a-zA-Z]
ident           \??{letter}({letter}|{digit})*
intLiteral      {digit}+{digit}*      
floatLiteral    {digit}+"."{digit}+

%%

and             return TAND;
by              return TBY;
call            return TCALL;
class           return TCLASS;
config          return TCONFIG;
const           return TCONST;
do              return TDO;
domain          return TDOMAIN;
else            return TELSE;
enum            return TENUM;
for             return TFOR;
forall          return TFORALL;
function        return TFUNCTION;
if              return TIF;
in              return TIN;
index           return TINDEX;
inout           return TINOUT;
mod             return TMOD;
not             return TNOT;
or              return TOR;
out             return TOUT;
reduce          return TREDUCE;
ref             return TREF;
return          return TRETURN;
static          return TSTATIC;
then            return TTHEN;
type            return TTYPE;
val             return TVAL;
var             return TVAR;
while           return TWHILE;

"="             return TASSIGN;
"+="            return TASSIGNPLUS;
"-="            return TASSIGNMINUS;
"*="            return TASSIGNMULTIPLY;
"/="            return TASSIGNDIVIDE;
"&="            return TASSIGNBAND;
"|="            return TASSIGNBOR;
"^="            return TASSIGNBXOR;

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

":"             return TCOLON;
";"             return TSEMI;
","             return TCOMMA;
"."             return TDOT;
"("             return TLP;
")"             return TRP;
"["             return TLSBR;
"]"             return TRSBR;
"{"             return TLCBR;
"}"             return TRCBR;

{ident}         {
                  if (yytext[0] == '?') {
                    return QUERY_IDENT;
                  }
                  Symbol* sym = Symboltable::lookup(yytext, false, true);

		  if (typeid(*sym) == typeid(TypeSymbol)) {
		    yylval.ptsym = (TypeSymbol*)sym;
		    return TYPE_IDENT;
		  } else if (typeid(*sym) == typeid(ClassSymbol)) {
		    yylval.pcsym = (ClassSymbol*)sym;
		    return CLASS_IDENT;
		  } else if (typeid(*sym) == typeid(ReduceSymbol)) {
		    yylval.redsym = (ReduceSymbol*)sym;
		    return REDUCE_IDENT;
		  } else {
		    return TIDENT;
		  }
                }
{intLiteral}    return INTLITERAL;
{floatLiteral}  return FLOATLITERAL;
{floatLiteral}i return COMPLEXLITERAL;
"\""            {
                  register int c;
                  int bufflen = 256;
		  static char* stringbuffer = 
                    (char*)MALLOC(bufflen*sizeof(char));
                  int stringlen = 0;

                  while (1) {
                    while ((c = yyinput()) != '\"' && c != EOF) {
		      stringbuffer[stringlen] = c;
                      stringlen++;
                      if (stringlen == bufflen) {
                        bufflen *= 2;
                        stringbuffer = (char*)REALLOC(stringbuffer,
                                                      bufflen*sizeof(char));
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
		      //		      printf("%c", c);
		      //		      fflush(stdout);
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
                    }    /* eat up text of comment */
          
                    if ( c == '*' ) {
                      while ( (c = yyinput()) == '*' ) {
                      }
                      if ( c == '/' ) {
                        break;    /* found the end */
                      }
                    }
          
                    if ( c == EOF ) {
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
