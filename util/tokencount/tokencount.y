/*
 * A simple mini-Chapel parser developed from Shannon's
 * experiments teaching herself flex and bison, and used
 * to exercise a prototype Chapel AST.
 *
 * Brad Chamberlain, 6/2004
 *
 */

%{

#include "tokencount.h"

%}

/* union type goes here */

%token ASM AUTO BOOL BREAK CASE CHAR CONST CONTINUE DEFAULT TYPEDEFname IDENTIFIER
%token DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INT LONG REGISTER
%token RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION
%token UNSIGNED VOID VOLATILE WHILE CLASS DELETE FRIEND INLINE NEW OPERATOR
%token OVERLOAD PROTECTED PRIVATE PUBLIC THIS VIRTUAL INTEGERliteral
%token OCTALliteral HEXliteral FLOATINGliteral
%token STRINGliteral CHARliteral DOTstar CLCL ARROW ARROWstar ICR DECR LS RS 
%token LE GE EQ NE ANDAND OROR MULTassign DIVassign MODassign PLUSassign
%token MINUSassign LSassign RSassign ANDassign ERassign ORassign
%token ELLIPSIS LP RP COMMA POUND POUNDPOUND LC RC LB RB DOT AND STAR PLUS
%token MINUS NEGATE NOT DIV MOD LT GT XOR PIPE QUESTION COLON SEMICOLON ASSIGN
%token SLASH DEFINE OPDEFINED CPPELSE ELIF ENDIF ERROR CPPIF IFDEF IFNDEF 
%token INCLUDE LINE PRAGMA UNDEF QUOTE ESCCHAR


%% 

program:
/* empty */
| tokenlist
;

tokenlist:
  token
| tokenlist token
;


token:
  AUTO
| BREAK
| CASE
| CHAR
| CONST
| CONTINUE
| DEFAULT
| TYPEDEFname
| IDENTIFIER
| DO
| DOUBLE
| ELSE
| CPPELSE
| ENUM
| EXTERN
| FLOAT
| FOR
| GOTO
| CPPIF
| IF
| INT
| LONG
| REGISTER
| RETURN
| SHORT
| SIGNED
| SIZEOF
| STATIC
| STRUCT
| SWITCH
| TYPEDEF
| UNION
| UNSIGNED
| VOID
| VOLATILE
| WHILE
| CLASS
| DELETE
| FRIEND
| INLINE
| NEW
| OPERATOR
| OVERLOAD
| PROTECTED
| PRIVATE
| PUBLIC
| THIS
| VIRTUAL
| INTEGERliteral
| OCTALliteral
| HEXliteral
| FLOATINGliteral
| CHARliteral
| STRINGliteral
| DOTstar
| CLCL
| ARROW
| ARROWstar
| ICR
| DECR
| LS
| RS
| LE
| GE
| EQ
| NE
| ANDAND
| OROR
| MULTassign
| DIVassign
| MODassign
| PLUSassign
| MINUSassign
| LSassign
| RSassign
| ANDassign
| ERassign
| ORassign
| ELLIPSIS
| LP
| RP
| COMMA
| POUND
| POUNDPOUND
| LC
| RC
| LB
| RB
| DOT
| AND
| STAR
| PLUS
| MINUS
| NEGATE
| NOT
| DIV
| MOD
| LT
| GT
| XOR
| PIPE
| QUESTION
| COLON
| SEMICOLON
| ASSIGN
| SLASH
| DEFINE
| OPDEFINED
| ELIF
| ENDIF
| ERROR
| IFDEF
| IFNDEF
| INCLUDE
| LINE
| PRAGMA
| UNDEF
  | QUOTE  /* should be string literal ? */
| ESCCHAR
;


%%
