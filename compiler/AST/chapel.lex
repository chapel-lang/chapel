/*
 * Description:  Scan a simple Chapel program.
 * 
 * > flex chapel_tokens.lex
 * > gcc -DSTANDALONE lex.yy.c -lfl
 * > ./a.out
 *
 * Shannon Hoffswell
 * 3/9/2004
 *
 */

/* SHANNON: Figure out how to get rid of leading zeroes with the intLiteral */

%{

#include <stdio.h>
#include "lexyacc.h"
#include "chapel.tab.h"

%}


digit           [0-9]
letter          [_a-zA-Z]
ident           {letter}({letter}|{digit})*
intLiteral      {digit}+{digit}*      
floatLiteral    {digit}+"."{digit}+

%%
var             return VAR;
const           return CONST;

integer         return INTEGER;
float           return FLOAT;
domain          return DOMAIN;
locale          return LOCALE;

if              return IF;
then            return THEN;
else            return ELSE;
forall          return FORALL;
in              return IN;
by              return BY;
return          return RETURN;

sum             return SUM;
dim             return DIM;

".."            return ELLIPSIS;
"=="            return EQUALS;
"!="            return NEQUALS;
"<="            return LEQUALS;
">="            return GEQUALS;
"&&"            return LOGAND;
"||"            return LOGOR;
"&"             return BITAND;
"|"             return BITOR;
"^"             return BITXOR;


{ident}         return IDENT;
{intLiteral}    return INTLITERAL;
{floatLiteral}  return FLOATLITERAL;

[ \t]           /* no action, eat spaces and tabs */
\n              yylineno++;

.               return (int) yytext[0];

"--"            {
                  register int c;
          
                  while (1) {
                    while ( (c = yyinput()) != '\n' && c != EOF ) {
		      fflush(stdout);
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
