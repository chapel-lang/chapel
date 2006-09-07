%{
#include "yy.h"
#include "modulefinder.h"

#define YYLLOC_DEFAULT(Current, Rhs, N)          \
  if (N) { \
    (Current).first_line   = (Rhs)[1].first_line;      \
    if ((Current).first_line) yystartlineno = (Current).first_line; \
    (Current).first_column = (Rhs)[1].first_column;    \
    (Current).last_line    = (Rhs)[N].last_line;       \
    (Current).last_column  = (Rhs)[N].last_column; \
  } else (Current) = yylloc;

  %}


%start program

%union {
  char* pch;
}

%token TAND
%token TATOMIC
%token TBAND
%token TBEGIN
%token TBNOT
%token TBOR
%token TBREAK
%token TBXOR
%token TBY
%token TCLASS
%token TCOBEGIN
%token TCOMMA
%token TCONFIG
%token TCONST
%token TCONSTRUCTOR
%token TPARAM
%token TCONTINUE
%token TDEF
%token TDIVIDE
%token TDO
%token TDOMAIN
%token TDOTDOT
%token TELSE
%token TENUM
%token TEQUAL
%token TERROR
%token TEXP
%token TFOR
%token TFORALL
%token TGOTO
%token TGREATER
%token TGREATEREQUAL
%token TIF
%token TIN
%token TINDEX
%token TINOUT
%token TITERATOR
%token TLABEL
%token TLESS
%token TLESSEQUAL
%token TLET
%token TMINUS
%token TMOD
%token TMODULE
%token TNIL
%token TNOELSE
%token TNOT
%token TNOTEQUAL
%token TOF
%token TOR
%token TORDERED
%token TOTHERWISE
%token TOUT
%token TPLUS
%token TPRAGMA
%token TPRIMITIVE
%token TRECORD
%token TREDUCE
%token TREF
%token TRETURN
%token TSCAN
%token TSELECT
%token TSEQCAT
%token TSHIFTLEFT
%token TSHIFTRIGHT
%token TSPARSE
%token TSTAR
%token TSTATIC
%token TSYNC
%token TTHEN
%token TTYPE
%token TTYPEDEF
%token TUNION
%token TUSE
%token TVAL
%token TVAR
%token TWHEN
%token TWHERE
%token TWHILE
%token TYIELD

%token TIDENT
%token INTLITERAL FLOATLITERAL IMAGLITERAL UINTLITERAL
%token <pch> STRINGLITERAL

%token TASSIGN
%token TASSIGNPLUS
%token TASSIGNMINUS
%token TASSIGNMULTIPLY
%token TASSIGNDIVIDE
%token TASSIGNBAND
%token TASSIGNBOR
%token TASSIGNBXOR
%token TASSIGNSEQCAT

%token TSEMI
%token TCOMMA
%token TDOT
%token TDOTDOTDOT
%token TCOLON
%token TNOTCOLON
%token TQUESTION
%token TPARTIAL
%token TLP
%token TRP
%token TSEQBEGIN
%token TSEQEND
%token TLSBR
%token TRSBR
%token TLCBR
%token TRCBR

%type <pch> identifier

%%

program: programdef
  { (void)@1.first_line; };

programdef:
  moduledecl morestuff
| otherstuff morestuff
;

morestuff:
  moduledecl
| otherstuff
| moduledecl morestuff
| otherstuff morestuff
;

identifier:
  TIDENT
    { $$ = yytext; }
;

moduledecl:
  TMODULE identifier
    { registerModule($2, depth); }
;

otherstuff:
  nonmoduletokens
{
  if (filemodule == 0 && depth == 0 && (strcmp(yytext, "}") != 0)) {
    filemodule = 1;
    registerTopLevelCode();
    //    fprintf(stderr, "yytext = %s, depth = %d\n", yytext, depth);
  }
}
;

nonmoduletokens:
  TAND
| TATOMIC
| TBAND
| TBEGIN
| TBNOT
| TBOR
| TBREAK
| TBXOR
| TBY
| TCLASS
| TCOBEGIN
| TCONFIG
| TCONST
| TCONSTRUCTOR
| TPARAM
| TCONTINUE
| TDEF
| TDIVIDE
| TDO
| TDOMAIN
| TDOTDOT
| TELSE
| TENUM
| TEQUAL
| TERROR
| TEXP
| TFOR
| TFORALL
| TGOTO
| TGREATER
| TGREATEREQUAL
| TIF
| TIN
| TINDEX
| TINOUT
| TITERATOR
| TLABEL
| TLESS
| TLESSEQUAL
| TLET
| TMINUS
| TMOD
| TNIL
| TNOT
| TNOTEQUAL
| TOF
| TOR
| TORDERED
| TOTHERWISE
| TOUT
| TPLUS
| TPRAGMA
| TPRIMITIVE
| TRECORD
| TREDUCE
| TREF
| TRETURN
| TSCAN
| TSELECT
| TSEQCAT
| TSHIFTLEFT
| TSHIFTRIGHT
| TSPARSE
| TSTAR
| TSTATIC
| TSYNC
| TTHEN
| TTYPE
| TTYPEDEF
| TUNION
| TUSE
| TVAL
| TVAR
| TWHEN
| TWHERE
| TWHILE
| TYIELD
| TIDENT
| INTLITERAL 
| FLOATLITERAL
| IMAGLITERAL
| UINTLITERAL
| STRINGLITERAL
| TASSIGN
| TASSIGNPLUS
| TASSIGNMINUS
| TASSIGNMULTIPLY
| TASSIGNDIVIDE
| TASSIGNBAND
| TASSIGNBOR
| TASSIGNBXOR
| TASSIGNSEQCAT
| TSEMI
| TCOMMA
| TDOT
| TDOTDOTDOT
| TCOLON
| TNOTCOLON
| TQUESTION
| TPARTIAL
| TLP
| TRP
| TSEQBEGIN
| TSEQEND
| TLSBR
| TRSBR
| lcbr
| rcbr
;

lcbr: TLCBR { depth++; }

rcbr: TRCBR { depth--; }

