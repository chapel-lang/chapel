/*
  Copyright 2002-2004 John Plevyak, All Rights Reserved
*/
#include "gramgram.h"
#include "d.h"
#include "mkdparse.h"

static void help(ArgumentState *arg_state, char *arg_unused);

static int set_op_priority_from_rule = 0;
static int right_recursive_BNF = 0;
static int states_for_whitespace = 1;
static int states_for_all_nterms = 0;
static int tokenizer = 0;
static int longest_match = 0;
static char grammar_ident[256] = "gram";
static int ident_from_filename = 0;
static int scanner_blocks = 4;
static int scanner_block_size;
static int write_line_directives = 1;
static int write_header = -1;
static int token_type = 0;
static char write_extension[256] = "c";

static ArgumentDescription arg_desc[] = {
 {"longest_match", 'l', "Use Longest Match Rule for Tokens", "T", 
  &longest_match, "D_MAKE_LONGEST_MATCH", NULL},
 {"tokenizer", 'T', "Tokenizer for START", "T", &tokenizer,
  "D_MAKE_PARSER_TOKENIZER", NULL},
 {"write_header", 'H', "Write Header (-1:if not empty)", "I", &write_header,
  "D_MAKE_PARSER_HEADER", NULL},
 {"token_type", 't', "Token Type (0:define, 1:enum)", "I", &token_type,
  "D_MAKE_PARSER_TOKEN", NULL},
 {"whitespace_states", 'C', "Compute Whitespace States", "T", 
  &states_for_whitespace,  "D_MAKE_PARSER_WHITESPACE", NULL},
 {"all_states", 'A', "Compute States For All NTERMs", "T", 
  &states_for_all_nterms,  "D_MAKE_PARSER_ALL_NTERMS", NULL},
 {"grammar_ident", 'i', "Grammar Identifier", "S255", grammar_ident,
  "D_MAKE_PARSER_GRAMMAR_IDENT", NULL},
 {"filename_ident", 'I', "Use Filename as Identifier", "T", &ident_from_filename,
  "D_MAKE_PARSER_IDENT_FROM_FILENAME", NULL},
 {"scanner_blocks", 'b', "Scanner Blocks", "I", &scanner_blocks,
  "D_MAKE_PARSER_SCANNER_BLOCKS", NULL},
 {"op_pri_from_rule", 'p', "Set Operator Priority From Rule", "T", 
  &set_op_priority_from_rule, "D_MAKE_PARSER_SET_PRIORITY", NULL},
 {"right_recurse_BNF", 'r', "Use Right Recursion For */+", "T", 
  &right_recursive_BNF, "D_MAKE_PARSER_RIGHT_RECURSIVE_BNF", NULL},
 {"write_lines", 'L', "Write #line(s)", "T", 
  &write_line_directives, "D_MAKE_PARSER_WRITE_LINE_DIRECTIVES", NULL},
 {"ext", 'X', "Code file extension (e.g. cpp)", "S256", 
  &write_extension[0], "D_MAKE_PARSER_WRITE_EXT", NULL},
 {"verbose", 'v', "Verbose", "+", &d_verbose_level, 
  "D_MAKE_PARSER_VERBOSE", NULL},
 {"debug", 'd', "Debug", "+", &d_debug_level, 
  "D_MAKE_PARSER_DEBUG", NULL},
 {"help", 'h', "Help", NULL, NULL, NULL, help},
 {0}
};

static ArgumentState arg_state = {
  0, 0,
  "program", 
  arg_desc
};

extern D_ParserTables parser_tables_dparser_gram;

static void
help(ArgumentState *arg_state, char *arg_unused) {
  char ver[30];
  d_version(ver);
  fprintf(stderr, "Make DParser Version %s ", ver);  
  fprintf(stderr, "Copyright (c) 2002-2004 John Plevyak\n");  
  usage(arg_state, arg_unused);
}

int
main(int argc, char *argv[]) {
  char *grammar_pathname;
  Grammar *g;

  process_args(&arg_state, argv);
  if (arg_state.nfile_arguments != 1)
    help(&arg_state, NULL);
  grammar_pathname = arg_state.file_argument[0];
  g = new_D_Grammar(grammar_pathname);
  /* grammar construction options */
  g->set_op_priority_from_rule = set_op_priority_from_rule;
  g->right_recursive_BNF = right_recursive_BNF;
  g->states_for_whitespace = states_for_whitespace;
  g->states_for_all_nterms = states_for_all_nterms;
  g->tokenizer = tokenizer;
  g->longest_match = longest_match;
  /* grammar writing options */
  strcpy(g->grammar_ident, grammar_ident);
  if (ident_from_filename) {
    char *n = strrchr(grammar_pathname, '/'), *e;
    n = n ? n : grammar_pathname;
    e = strchr(n, '.');
    e = e ? e : n + strlen(n);
    memcpy(g->grammar_ident, n, e-n);
    g->grammar_ident[e-n] = 0;
  }
  g->scanner_blocks = scanner_blocks;
  g->scanner_block_size = scanner_block_size;
  g->write_line_directives = write_line_directives;
  g->write_header = write_header;
  g->token_type = token_type;
  strcpy(g->write_extension, write_extension);
   
  mkdparse(g, grammar_pathname);
  if (write_c_tables(g) < 0)
    d_fail("unable to write C tables '%s'", grammar_pathname);
  free_D_Grammar(g);
  return 0;
}


