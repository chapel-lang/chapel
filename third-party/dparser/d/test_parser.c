/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/

#include "d.h"
#include "mkdparse.h"

extern D_ParserTables parser_tables_dparser_gram;

#define SIZEOF_MY_PARSE_NODE	100	/* permit test cases up to this size */

static int save_parse_tree = 1;
static int partial_parses = 0;
static int fixup = 1;
static int fixup_ebnf = 0;
static int compare_stacks = 1;
static int commit_actions_interval = 100;
static int start_state = 0;

static int set_op_priority_from_rule = 0;
static int right_recursive_BNF = 0;
static int states_for_whitespace = 1;
static int states_for_all_nterms = 0;
static int tokenizer = 0;
static int longest_match = 0;
static int scanner_blocks = 4;
static int scanner_block_size;

static void help(ArgumentState *arg_state, char *arg_unused);

ArgumentDescription arg_desc[] = {
 {"longest_match", 'l', "Use Longest Match Rule for Tokens", "T",
  &longest_match, "D_MAKE_LONGEST_MATCH", NULL},
 {"tokenizer", 'T', "Tokenizer for START", "T", &tokenizer,
  "D_MAKE_PARSER_TOKENIZER", NULL},
 {"whitespace_states", 'C', "Compute Whitespace States", "T",
  &states_for_whitespace,  "D_MAKE_PARSER_WHITESPACE", NULL},
 {"all_states", 'A', "Compute States For All NTERMs", "T",
  &states_for_all_nterms,  "D_MAKE_PARSER_ALL_NTERMS", NULL},
 {"scanner_blocks", 'b', "Scanner Blocks", "I", &scanner_blocks,
  "D_MAKE_PARSER_SCANNER_BLOCKS", NULL},
 {"op_pri_from_rule", 'p', "Set Operator Priority From Rule", "T",
  &set_op_priority_from_rule, "D_MAKE_PARSER_SET_PRIORITY", NULL},
 {"right_recurse_BNF", 'r', "Use Right Recursion For */+", "T",
  &right_recursive_BNF, "D_MAKE_PARSER_RIGHT_RECURSIVE_BNF", NULL},

 {"start_state", 'S', "Start State", "I", &start_state, 
  "D_PARSE_START_STATE", NULL},
 {"save_parse_tree", 's', "Save Parse Tree", "T", &save_parse_tree, 
  "D_PARSE_SAVE_PARSE_TREE", NULL},
 {"partial_parses", 'p', "Partial Parses", "T", &partial_parses, 
  "D_PARSE_PARTIAL_PARSES", NULL},
 {"compare_stacks", 'c', "Compare Stacks", "T", &compare_stacks, 
  "D_PARSE_COMPARE_STACKS", NULL},
 {"commit_interval", 'i', "Commit Interval", "I", &commit_actions_interval, 
  "D_PARSE_COMMIT_INTERVAL", NULL},
 {"fixup", 'f', "Fixup Internal Productions", "T", &fixup, 
  "D_PARSE_FIXUP", NULL},
 {"fixup_ebnf", 'e', "Fixup EBNF Productions", "T", &fixup_ebnf, 
  "D_PARSE_FIXUP_EBNF", NULL},

 {"verbose", 'v', "Verbose", "+", &d_verbose_level, 
  "D_PARSE_VERBOSE", NULL},
 {"test", 't', "Test", "+", &test_level, 
  "D_PARSE_TEST", NULL},
 {"debug", 'd', "Debug", "+", &d_debug_level, 
  "D_PARSE_DEBUG", NULL},
 {"help", 'h', "Help", NULL, NULL, 
  NULL, help},
 {0}
};

ArgumentState arg_state = {
  0, 0,
  "program", 
  arg_desc
};

static void
help(ArgumentState *arg_state, char *arg_unused) {
  char ver[30];
  d_version(ver);
  fprintf(stderr, "Test DParser Version %s ", ver);
  fprintf(stderr, "Copyright (c) 2002-2004 John Plevyak\n");
  usage(arg_state, arg_unused);
}

char *ops = "+";
void *ops_cache = NULL; 
int ops_scan(char *ops, void *ops_cache, char **as, int *col, int *line,
	     unsigned char *op_assoc, int *op_priority) 
{
  if (**as == '+') {
    (*as)++;
    *op_assoc = ASSOC_BINARY_LEFT;
    *op_priority = 9500;
    return 1;
  }
  return 0;
}

int spec_code(void *new_ps, void **children, int n_children, int pn_offset, 
              struct D_Parser *parser)
{
  return 0;
}

int final_code(void *new_ps, void **children, int n_children, int pn_offset, 
               struct D_Parser *parser)
{
  return 0;
}

int
main(int argc, char *argv[]) {
  int i, len = 0;
  char *buf = NULL, *fn, *grammar_pathname;
  D_Parser *p;
  D_ParseNode *pn = NULL;
  unsigned char *str = NULL;
  unsigned int str_len;
  Grammar *g;
  D_ParserTables *parser_tables_gram;

  process_args(&arg_state, argv);
  if (arg_state.nfile_arguments < 2)
    help(&arg_state, NULL);

  /* build grammar */
  grammar_pathname = arg_state.file_argument[0];
  g = new_D_Grammar(grammar_pathname);
  g->set_op_priority_from_rule = set_op_priority_from_rule;
  g->right_recursive_BNF = right_recursive_BNF;
  g->states_for_whitespace = states_for_whitespace;
  g->states_for_all_nterms = states_for_all_nterms;
  g->tokenizer = tokenizer;
  g->longest_match = longest_match;
  g->scanner_blocks = scanner_blocks;
  g->scanner_block_size = scanner_block_size;
  
  if (!(str = sbuf_read(grammar_pathname)))
    d_fail("unable to read grammar file '%s'", grammar_pathname);
  mkdparse_from_string(g, str);
  str = 0;
  if (write_binary_tables_to_string(g, &str, &str_len) < 0)
    d_fail("unable to write tables to string '%s'", grammar_pathname);
  free_D_Grammar(g);

  /* execute parser */
  parser_tables_gram = read_binary_tables_from_string(str, spec_code, final_code);
  p = new_D_Parser(parser_tables_gram, SIZEOF_MY_PARSE_NODE);
  p->save_parse_tree = save_parse_tree;
  p->ambiguity_fn = ambiguity_count_fn;
  p->partial_parses = partial_parses;
  p->dont_fixup_internal_productions = !fixup;
  p->fixup_EBNF_productions = fixup_ebnf;
  p->dont_compare_stacks = !compare_stacks;
  p->commit_actions_interval = commit_actions_interval;
  p->start_state = start_state;
  for (i = 1; i < arg_state.nfile_arguments; i++) {
    p->loc.pathname = arg_state.file_argument[i];
    p->loc.line = 1;
    p->loc.col = p->loc.previous_col = 0;
    if (buf_read(arg_state.file_argument[i], &buf, &len) > 0)
      pn = dparse(p, buf, len);
    else 
      d_fail("unable to read file '%s'", arg_state.file_argument[i]);
    if (pn) {
      free_D_ParseNode(p, pn);
      pn = 0;
    } else {
      fn = d_dup_pathname_str(p->loc.pathname);
      fprintf(stderr, "fatal error, '%s' line %d\n", fn, p->loc.line);
      FREE(fn);
    }
    if (buf)
      FREE(buf);
  }
  free_D_Parser(p);
  free_args(&arg_state);
  exit(0);
}

