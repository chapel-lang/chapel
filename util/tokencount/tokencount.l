%{
#include "tokencount.h"
#include "tokencount.tab.h"

#define handleToken(t) \
  sprintf(line, "%s%s", line, yytext); \
  lineTokens++; \
  fileTokens++; \
  totTokens++; \
  lineBlank = 0; \
  lineComment = 0; \
  return(t)


#define addChar(c) sprintf(line, "%s%c", line, c)

#define addString(s) sprintf(line, "%s%s", line, s)

%}
digit [0-9]
letter [_a-zA-Z]
octdigit [0-7]
hexdigit [0-9a-fA-F]
identifier {letter}({letter}|{digit})*
exponent_part [eE][-+]?[0-9]+
fractional_literal ([0-9]*"."[0-9]+)|([0-9]+".")
floating_literal (({fractional_literal}{exponent_part}?)|([0-9]+{exponent_part}))[FfLl]?
integer_suffix_opt ([uU]?[lL]?)|([lL][uU])
decimal_literal [1-9][0-9]*{integer_suffix_opt}
octal_literal "0"[0-7]*{integer_suffix_opt}
hex_literal "0"[xX][0-9a-fA-F]+{integer_suffix_opt}
simple_escape [abfnrtv'"?\\]
octal_escape  [0-7]{1,3}
hex_escape "x"[0-9a-fA-F]+
escape_sequence [\\]({simple_escape}|{octal_escape}|{hex_escape})
%%
#define              {handleToken(DEFINE);}
#defined             {handleToken(OPDEFINED);}
#else                {handleToken(CPPELSE);}
#endif               {handleToken(ENDIF);}
#if                  {handleToken(CPPIF);}
#ifdef               {handleToken(IFDEF);}
#ifndef              {handleToken(IFNDEF);}
#include             {handleToken(INCLUDE); }
#line                {handleToken(LINE);}
#undef                {handleToken(UNDEF);}
auto                 {handleToken(AUTO);}
break                {handleToken(BREAK);}
case                 {handleToken(CASE);}
char                 {handleToken(CHAR);}
const                {handleToken(CONST);}
continue             {handleToken(CONTINUE);}
default              {handleToken(DEFAULT);}
do                   {handleToken(DO);}
double               {handleToken(DOUBLE);}
elif                 {handleToken(ELIF);}
else                 {handleToken(ELSE);}
enum                 {handleToken(ENUM);}
error                {handleToken(ERROR);}
extern               {handleToken(EXTERN);}
float                {handleToken(FLOAT);}
for                  {handleToken(FOR);}
goto                 {handleToken(GOTO);}
if                   {handleToken(IF);}
int                  {handleToken(INT);}
long                 {handleToken(LONG);}
pragma               {handleToken(PRAGMA);}
register             {handleToken(REGISTER);}
return               {handleToken(RETURN);}
short                {handleToken(SHORT);}
signed               {handleToken(SIGNED);}
sizeof               {handleToken(SIZEOF);}
static               {handleToken(STATIC);}
struct               {handleToken(STRUCT);}
switch               {handleToken(SWITCH);}
typedef              {handleToken(TYPEDEF);}
union                {handleToken(UNION);}
unsigned             {handleToken(UNSIGNED);}
void                 {handleToken(VOID);}
volatile             {handleToken(VOLATILE);}
while                {handleToken(WHILE);}
class                {handleToken(CLASS);}
delete               {handleToken(DELETE);}
friend               {handleToken(FRIEND);}
inline               {handleToken(INLINE);}
new                  {handleToken(NEW);}
operator             {handleToken(OPERATOR);}
overload             {handleToken(OVERLOAD);}
protected            {handleToken(PROTECTED);}
private              {handleToken(PRIVATE);}
public               {handleToken(PUBLIC);}
this                 {handleToken(THIS);}
virtual              {handleToken(VIRTUAL);}
{identifier}         {handleToken(IDENTIFIER); }
{decimal_literal}    {handleToken(INTEGERliteral);}
{octal_literal}      {handleToken(OCTALliteral);}
{hex_literal}        {handleToken(HEXliteral);}
{floating_literal}   {handleToken(FLOATINGliteral);}
"##"                 {handleToken(POUNDPOUND);}
".*"                 {handleToken(DOTstar);}
"::"                 {handleToken(CLCL);}
"->"                 {handleToken(ARROW);}
"->*"                {handleToken(ARROWstar);}
"++"                 {handleToken(ICR);}
"--"                 {handleToken(DECR);}
"<<"                 {handleToken(LS);}
">>"                 {handleToken(RS);}
"<="                 {handleToken(LE);}
">="                 {handleToken(GE);}
"=="                 {handleToken(EQ);}
"!="                 {handleToken(NE);}
"&&"                 {handleToken(ANDAND);}
"||"                 {handleToken(OROR);}
"*="                 {handleToken(MULTassign);}
"/="                 {handleToken(DIVassign);}
"%="                 {handleToken(MODassign);}
"+="                 {handleToken(PLUSassign);}
"-="                 {handleToken(MINUSassign);}
"<<="                {handleToken(LSassign);}
">>="                {handleToken(RSassign);}
"&="                 {handleToken(ANDassign);}
"^="                 {handleToken(ERassign);}
"|="                 {handleToken(ORassign);}
"..."                {handleToken(ELLIPSIS);}

"//"            {
                  register int c;

                  addString(yytext);
                  lineBlank = 0;
                  while (1) {
                    while ( (c = input()) != '\n' && c != EOF ) {
                      addChar(c);
                    }    /* eat up text of comment */
                    if (c == '\n') {
                      handleLinefeed();
                    }
                    break;
                  }
		}

"/*"            {
                  register int c;

                  addString(yytext);
                  lineBlank = 0;
                  while (1) {
                    while ((c = input()) != '*' && c != EOF) {
                      if (c == '\n') {
                        handleLinefeed();
                        lineBlank = 0;
                      } else {
                        addChar(c);
                      }
                    }    /* eat up text of comment */
          
                    if ( c == '*' ) {
                      addChar(c);
                      while ( (c = input()) == '*' ) {
                        addChar(c);
                      }
                      addChar(c);
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


"("                  {handleToken(LP);}
")"                  {handleToken(RP);}
","                  {handleToken(COMMA);}
"{"                  {handleToken(LC);}
"}"                  {handleToken(RC);}
"["                  {handleToken(LB);}
"]"                  {handleToken(RB);}
"."                  {handleToken(DOT);}
"&"                  {handleToken(AND);}
"*"                  {handleToken(STAR);}
"+"                  {handleToken(PLUS);}
"-"                  {handleToken(MINUS);}
"~"                  {handleToken(NEGATE);}
"!"                  {handleToken(NOT);}
"/"                  {handleToken(DIV);}
"%"                  {handleToken(MOD);}
"<"                  {handleToken(LT);}
">"                  {handleToken(GT);}
"^"                  {handleToken(XOR);}
"|"                  {handleToken(PIPE);}
"?"                  {handleToken(QUESTION);}
":"                  {handleToken(COLON);}
";"                  {handleToken(SEMICOLON);}
"="                  {handleToken(ASSIGN);}
"#"                  {handleToken(POUND);}
"\\"                 {handleToken(SLASH); }
"\""            {  /* BLC: TODO clean up the following, append into line */
                  register int c;
		  
		  addString(yytext);

                  while (1) {
                    while ((c = input()) != '\"' && c != EOF) {
 		      addChar(c);
                      if (c == '\\') {
                        c = input(); /* BLC: assume only single-char escapes */
                        addChar(c);
                      } else if (c == '\n') {
		        yyerror("EOLN in string");
                      }
                    } /* eat up string */
                    if (c == EOF) {
                      yyerror("EOF in string");
                    } else {
                      handleToken(STRINGliteral);
                    }
                  }
                }
"\'"            {
                  register int c;

		  addString(yytext);

                  while (1) {
                    while ((c = input()) != '\'' && c != EOF) {
 		      addChar(c);
                      if (c == '\\') {
                        c = input(); /* BLC: assume only single-char escapes */
			addChar(c);
                      } else if (c == '\n') {
		        yyerror("EOLN in string");
                      }
                    } /* eat up string */
                    if (c == EOF) {
                      yyerror("EOF in char");
                    } else {
                      handleToken(CHARliteral);
                    }
                  }
                }
{escape_sequence}    {handleToken(ESCCHAR);}
[ ]                  {sprintf(line, "%s%s", line, yytext);}
[\t]                 {sprintf(line, "%s%s", line, yytext);}
[\n]                 { handleLinefeed(); }
<*>.|\\n        fprintf(stderr, "ERROR: Unknown token: %s\n", yytext); fprintf(stderr, "Line so far: %s\n", line); exit(1);
%%
