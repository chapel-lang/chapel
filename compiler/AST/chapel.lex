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
config          return CONFIG;
static          return STATIC;

var             return VAR;
const           return CONST;

boolean         return BOOLEAN;
integer         return INTEGER;
float           return FLOAT;

domain          return DOMAIN;
index           return INDEX;

locale          return LOCALE;
timer           return TIMER;

type            return TYPE;
enum            return ENUM;

function        return FUNCTION;
inout           return INOUT;
in              return IN;
out             return OUT;
ref             return REF;
val             return VAL;

if              return IF;
else            return ELSE;
elsif           return ELSIF;

for             return FOR;
forall          return FORALL;

while           return WHILE;
do              return DO;
repeat          return REPEAT;
until           return UNTIL;

by              return BY;
return          return RETURN;

sum             return SUM;
dim             return DIM;
reduce          return REDUCE;

"="             return GETS;
"+="            return PLUSGETS;
"-="            return MINUSGETS;
"*="            return TIMESGETS;
"/="            return DIVGETS;
"<<="           return LSHGETS;
">>="           return RSHGETS;

".."            return ELLIPSIS;

"=="            return EQUALS;
"!="            return NEQUALS;
"<="            return LEQUALS;
">="            return GEQUALS;
">"             return GTHAN;
"<"             return LTHAN;

"&&"            return LOGAND;
"||"            return LOGOR;

"&"             return BITAND;
"|"             return BITOR;
"^"             return BITXOR;

"**"            return EXP;


{ident}         {
                  Symbol* sym = Symboltable::lookup(yytext);
                  if (sym == NULL) {
                    return IDENT;
                  } else {
		    yylval.ptsym = (TypeSymbol*)sym;  // BLC: cast == yuck!
                    return DEFINED_IDENT;
                  }
                }
{intLiteral}    return INTLITERAL;
{floatLiteral}  return FLOATLITERAL;
"\""            {
                  register int c;
                  int bufflen = 256;
		  static char* stringbuffer = 
                    (char*)malloc(bufflen*sizeof(char));
                  int stringlen = 0;

                  while (1) {
                    while ((c = yyinput()) != '\"' && c != EOF) {
		      stringbuffer[stringlen] = c;
                      stringlen++;
                      if (stringlen == bufflen) {
                        bufflen *= 2;
                        stringbuffer = (char*)realloc(stringbuffer,
                                                      bufflen*sizeof(char));
                      }
                      stringbuffer[stringlen] = '\0';
                    } /* eat up string */
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

.               return (int) yytext[0];

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
