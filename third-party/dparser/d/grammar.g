/*
 Grammar Grammar
*/
{
#include "gramgram.h"
#include "d.h"

extern D_ParserTables parser_tables_dparser_gram;
}

grammar: top_level_statement*;

top_level_statement: global_code | production | include_statement;

include_statement: 'include' regex {
  char *grammar_pathname = dup_str($n1.start_loc.s+1, $n1.end-1);
  if (parse_grammar($g, grammar_pathname, &parser_tables_dparser_gram, sizeof(ParseNode_User)) < 0)
    d_fail("unable to parse grammar '%s'", grammar_pathname);
};

global_code
  : '%<' balanced_code* '%>'
    { add_global_code($g, $n0.start_loc.s+2, $n2.end-2, $n0.start_loc.line); }
  | curly_code { add_global_code($g, $n0.start_loc.s+1, $n0.end-1, $n0.start_loc.line); }
  | '${scanner' balanced_code+ '}' {
      $g->scanner.code = dup_str($n1.start_loc.s, $n1.end);
      $g->scanner.line = $n0.start_loc.line;
    }
  | '${declare' declarationtype identifier* '}' {
      if (!d_get_number_of_children(&$n2))
     	add_declaration($g, $n2.start_loc.s, $n2.end,  $1.kind, $n2.start_loc.line);
      else {
	int i, n = d_get_number_of_children(&$n2);
	for (i = 0; i < n; i++) {
	  D_ParseNode *pn = d_get_child(&$n2, i);
	  add_declaration($g, pn->start_loc.s, pn->end,  $1.kind, pn->start_loc.line);
	}
      }
    }
  | '${token' token_identifier+ '}'
  | '${action}' { $g->action_index++; }
  | '${pass' identifier pass_types '}' {
      add_pass($g, $n1.start_loc.s, $n1.end,  $2.kind, $n1.start_loc.line);
    }
  ;

pass_types
  : 
  | pass_type pass_types { $$.kind = $0.kind | $1.kind; }
  ;

pass_type 
  : 'preorder' { $$.kind |= D_PASS_PRE_ORDER; } 
  | 'postorder' { $$.kind |= D_PASS_POST_ORDER; }
  | 'manual' { $$.kind |= D_PASS_MANUAL; }
  | 'for_all'  { $$.kind |= D_PASS_FOR_ALL; }
  | 'for_undefined' { $$.kind |= D_PASS_FOR_UNDEFINED; }
  ;

declarationtype
  : 'tokenize' { $$.kind = DECLARE_TOKENIZE; } 
  | 'longest_match' { $$.kind = DECLARE_LONGEST_MATCH; }
  | 'whitespace' { $$.kind = DECLARE_WHITESPACE; }
  | 'all_matches' { $$.kind = DECLARE_ALL_MATCHES; }
  | 'set_op_priority_from_rule' { $$.kind = DECLARE_SET_OP_PRIORITY; }
  | 'all_subparsers' { $$.kind = DECLARE_STATES_FOR_ALL_NTERMS; }
  | 'subparser' { $$.kind = DECLARE_STATE_FOR; }
  | 'save_parse_tree' { $$.kind = DECLARE_SAVE_PARSE_TREE; }
  ;

token_identifier: identifier { new_token($g, $n0.start_loc.s, $n0.end); };

production 
  : production_name ':' rules ';' 
  | production_name regex_production rules ';'
  | ';';
regex_production : '::=' ('$name' regex)? { 
  if ($#1) {
    D_ParseNode *pn = ${child 1, 0, 1};
    $g->p->regex_term_name = dup_str(pn->start_loc.s+1, pn->end-1); 
  }
  $g->p->regex = 1; 
}; 

production_name : (identifier | '_') { $g->p = new_production($g, dup_str($n0.start_loc.s, $n0.end)); } ;

rules : rule ('|' rule)*; 

rule : new_rule ((element element_modifier*)* simple_element element_modifier*)? rule_modifier* rule_code {
  vec_add(&$g->p->rules, $g->r);
};

new_rule : { $g->r = new_rule($g, $g->p); };

simple_element
  : string { $g->e = new_string($g, $n0.start_loc.s, $n0.end, $g->r); }
  | regex { $g->e = new_string($g, $n0.start_loc.s, $n0.end, $g->r); }
  | identifier { $g->e = new_ident($n0.start_loc.s, $n0.end, $g->r); }
  | '${scan' balanced_code+ '}' { $g->e = new_code($g, $n1.start_loc.s, $n1.end, $g->r); }
  | '(' new_subrule rules ')' {
      $g->e = new_elem_nterm($g->p, $1.r);
      $g->p = $1.p;
      $g->r = $1.r;
      vec_add(&$g->r->elems, $g->e);
    }
  ;

element
  : simple_element
  | bracket_code {
      Production *p = new_internal_production($g, NULL);
      Rule *r = new_rule($g, p);
      vec_add(&p->rules, r);
      r->speculative_code.code = dup_str($n0.start_loc.s + 1, $n0.end - 1);
      r->speculative_code.line = $n0.start_loc.line;
      $g->e = new_elem_nterm(p, $g->r);
      vec_add(&$g->r->elems, $g->e);
    }
  | curly_code {
      Production *p = new_internal_production($g, NULL);
      Rule *r = new_rule($g, p);
      vec_add(&p->rules, r);
      r->final_code.code = dup_str($n0.start_loc.s + 1, $n0.end - 1);
      r->final_code.line = $n0.start_loc.line;
      $g->e = new_elem_nterm(p, $g->r);
      vec_add(&$g->r->elems, $g->e);
  }
  ;

new_subrule : {
  $$.p = $g->p;
  $$.r = $g->r;
  $g->p = new_internal_production($g, $g->p);
  $g->r = new_rule($g, $g->p);
};

element_modifier 
  : '$term' integer { 
      if ($g->e->kind != ELEM_TERM) 
        d_fail("terminal priority on non-terminal");
      $g->e->e.term->term_priority = strtol($n1.start_loc.s, NULL, 0); 
    }
  | '$name' (string|regex) { 
      if ($g->e->kind != ELEM_TERM) 
	d_fail("terminal name on non-terminal");
      $g->e->e.term->term_name = dup_str($n1.start_loc.s+1, $n1.end-1); 
    }
  | '/i' { 
      if ($g->e->kind != ELEM_TERM) 
	d_fail("ignore-case (/i) on non-terminal");
      $g->e->e.term->ignore_case = 1; 
    }
  | '?' { conditional_EBNF($g); }
  | '*' { star_EBNF($g); }
  | '+' { plus_EBNF($g); } ;

rule_modifier : rule_assoc rule_priority;

rule_assoc
  : '$unary_op_right' { $g->r->op_assoc = ASSOC_UNARY_RIGHT; }
  | '$unary_op_left' { $g->r->op_assoc = ASSOC_UNARY_LEFT; }
  | '$binary_op_right' { $g->r->op_assoc = ASSOC_BINARY_RIGHT; }
  | '$binary_op_left' { $g->r->op_assoc = ASSOC_BINARY_LEFT; }
  | '$unary_right' { $g->r->rule_assoc = ASSOC_UNARY_RIGHT; }
  | '$unary_left' { $g->r->rule_assoc = ASSOC_UNARY_LEFT; }
  | '$binary_right' { $g->r->rule_assoc = ASSOC_BINARY_RIGHT; }
  | '$binary_left' { $g->r->rule_assoc = ASSOC_BINARY_LEFT; }
  | '$right' { $g->r->rule_assoc = ASSOC_NARY_RIGHT; }
  | '$left' { $g->r->rule_assoc = ASSOC_NARY_LEFT; }
  ;

rule_priority : integer { 
  if ($g->r->op_assoc) $g->r->op_priority = strtol($n0.start_loc.s, NULL, 0); 
  else $g->r->rule_priority = strtol($n0.start_loc.s, NULL, 0); 
};

rule_code : speculative_code? final_code? pass_code* ;

speculative_code : bracket_code {
  $g->r->speculative_code.code = dup_str($n0.start_loc.s + 1, $n0.end - 1);
  $g->r->speculative_code.line = $n0.start_loc.line;
};

final_code : curly_code {
  $g->r->final_code.code = dup_str($n0.start_loc.s + 1, $n0.end - 1);
  $g->r->final_code.line = $n0.start_loc.line;
};

pass_code : identifier ':' curly_code {
  add_pass_code($g, $g->r, $n0.start_loc.s, $n0.end, $n2.start_loc.s+1,
    $n2.end-1, $n0.start_loc.line, $n2.start_loc.line);
};

curly_code: '{' balanced_code* '}';
bracket_code: '[' balanced_code* ']';
balanced_code 
  : '(' balanced_code* ')' | '[' balanced_code* ']' | '{' balanced_code* '}'
  | string | identifier | regex | integer | symbols;
symbols : "[!~`@#$%^&*\-_+=|:;\\<,>.?/]";
string: "'([^'\\]|\\[^])*'";
regex: "\"([^\"\\]|\\[^])*\"";
identifier: "[a-zA-Z_][a-zA-Z_0-9]*" $term -1;
integer: decimalint | hexint | octalint;
decimalint: "-?[1-9][0-9]*[uUlL]?";
hexint: "-?(0x|0X)[0-9a-fA-F]+[uUlL]?";
octalint: "-?0[0-7]*[uUlL]?";

