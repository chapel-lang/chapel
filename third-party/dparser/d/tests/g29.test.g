{
#include <stdio.h>

typedef struct My_Sym {
  int value;
} My_Sym;
#define D_UserSym My_Sym
typedef struct My_ParseNode {
  int value;
  struct D_Scope *scope;
} My_ParseNode;
#define D_ParseNode_User My_ParseNode
}

translation_unit: statement*;
 
statement 
  : expression ';' 
  { printf("%d\n", $0.value); }
  | '{' new_scope statement* '}'
  [ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
  ;

new_scope: [ ${scope} = new_D_Scope(${scope}); ];

expression 
  : identifier ':' expression 
  [ 
    D_Sym *s;
    if (find_D_Sym_in_Scope(${scope}, ${scope}, $n0.start_loc.s, $n0.end))
      printf("duplicate identifier line %d\n", $n0.start_loc.line);
    s = NEW_D_SYM(${scope}, $n0.start_loc.s, $n0.end);
    s->user.value = $2.value;
    $$.value = s->user.value;
  ]
  | identifier '=' expression
  [ D_Sym *s = find_D_Sym(${scope}, $n0.start_loc.s, $n0.end);
    s = UPDATE_D_SYM(s, &${scope});
    s->user.value = $2.value;
    $$.value = s->user.value;
  ]
  | integer 
  [ $$.value = atoi($n0.start_loc.s); ]
  | identifier 
  [ D_Sym *s = find_D_Sym(${scope}, $n0.start_loc.s, $n0.end);
    if (s)
      $$.value = s->user.value;
  ]
  | expression '+' expression
  [ $$.value = $0.value + $1.value; ]
  ;

integer: "-?([0-9]|0(x|X))[0-9]*(u|U|b|B|w|W|L|l)*" $term -1;
identifier: "[a-zA-Z_][a-zA-Z_0-9]*";
