{

struct D_Sym;
struct D_Scope;

typedef struct My_ParseNode {
  struct D_Sym *sym;
  struct D_Scope *saved_scope;
} My_ParseNode;
#define D_ParseNode_User My_ParseNode

#define NEW_SYM(_s, _scope, _start, _end) do {				\
  _scope = enter_D_Scope(_scope, _scope); /* clone scope */		\
  _s = NEW_D_SYM(_scope, _start, _end);					\
} while (0)

}

statement: expression ';' ;

expression
 : identifier
 | def_identifier expression $right 5100 /* define variable */
 | def_function expression $right 5100 /* define function */
   [ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ]
 | pre_operator expression 
 | expression binary_operator expression 
 ;

def_identifier: identifier ':'
    [ NEW_SYM($$.sym, ${scope}, $n0.start_loc.s, $n0.end); ]
  ;

def_function: identifier identifier+ ':' 
    [
      NEW_SYM($$.sym, ${scope}, $n0.start_loc.s, $n0.end);
      $$.saved_scope = ${scope};
      ${scope} = new_D_Scope(${scope});
    ]
  ;

binary_operator
  : '+'         $binary_op_left 9500
  |             $binary_op_left 7000
  ;

pre_operator
  : '+'         $unary_op_right 9800
  ;

identifier: "[a-zA-Z_][a-zA-Z0-9_]*" $term -1;
