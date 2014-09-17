/*
 * Copyright 2004-2014 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

