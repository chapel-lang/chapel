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

%}


digit           [0-9]
letter          [_a-zA-Z]
ident           \??{letter}({letter}|{digit})*
intLiteral      {digit}+{digit}*      
floatLiteral    {digit}+"."{digit}+

%%
config          return CONFIG;
static          return STATIC;

var             return VAR;
const           return CONST;

domain          return DOMAIN;
index           return INDEX;

type            return TYPEDEF;
enum            return ENUM;
class           return CLASS;

function        return FUNCTION;
inout           return INOUT;
in              return IN;
out             return OUT;
ref             return REF;
val             return VAL;

if              return IF;
else            return ELSE;

for             return FOR;
forall          return FORALL;

while           return WHILE;
do              return DO;

by              return BY;
return          return RETURN;

dim             return DIM;
reduce          return REDUCE;

"+="            return PLUSGETS;
"-="            return MINUSGETS;
"*="            return TIMESGETS;
"/="            return DIVGETS;
"&="            return BITANDGETS;
"|="            return BITORGETS;
"^="            return BITXORGETS;
"<<="           return BITSLGETS;
">>="           return BITSRGETS;

".."            return ELLIPSIS;

"&&"            return LOGAND;
"||"            return LOGOR;

"<<"            return BITSL;
">>"            return BITSR;
"&"             return BITAND;
"|"             return BITOR;
"^"             return BITXOR;

"=="            return EQUALS;
"!="            return NEQUALS;
"<="            return LEQUALS;
">="            return GEQUALS;
">"             return GTHAN;
"<"             return LTHAN;

"="             return GETS;

"**"            return EXP;


{ident}         {
                  if (yytext[0] == '?') {
                    return QUERY_IDENT;
                  }
                  Symbol*  sym = Symboltable::lookup(yytext, true);

		  if (typeid(*sym) == typeid(TypeSymbol)) {
		    yylval.ptsym = (TypeSymbol*)sym;
		    return TYPE_IDENT;
		  } else if (typeid(*sym) == typeid(ReduceSymbol)) {
		    yylval.redsym = (ReduceSymbol*)sym;
		    return REDUCE_IDENT;
		  } else {
		    return IDENT;
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
