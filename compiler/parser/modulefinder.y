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

%token TMODULE
%token TIDENT
%token <pch> TSTRINGLITERAL
%token TOTHER
%token TRCBR
%token TLCBR

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
  TSTRINGLITERAL
| TIDENT
| TOTHER
| lcbr
| rcbr
;

lcbr: TLCBR { depth++; }

rcbr: TRCBR { depth--; }

