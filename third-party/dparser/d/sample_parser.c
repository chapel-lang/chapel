/*
  Copyright 2002-2003 John Plevyak, All Rights Reserved
*/

#include "d.h"

#define SIZEOF_MY_PARSE_NODE	100	/* permit test cases up to this size */

extern D_ParserTables parser_tables_gram;

int save_parse_tree = 1;
int partial_parses = 0;
int fixup = 1;
int fixup_ebnf = 0;
int compare_stacks = 1;
int commit_actions_interval = 100;
int start_state = 0;

static void help(ArgumentState *arg_state, char *arg_unused);

ArgumentDescription arg_desc[] = {
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
  fprintf(stderr, "Sample DParser Version %s ", ver);
  fprintf(stderr, "Copyright (c) 2002-2003 John Plevyak\n");
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

int
main(int argc, char *argv[]) {
  int i, len = 0;
  char *buf = NULL, *fn;
  D_Parser *p;
  D_ParseNode *pn = NULL;

  process_args(&arg_state, argv);
  if (!arg_state.nfile_arguments)
    help(&arg_state, NULL);
  p = new_D_Parser(&parser_tables_gram, SIZEOF_MY_PARSE_NODE);
  p->save_parse_tree = save_parse_tree;
  p->ambiguity_fn = ambiguity_count_fn;
  p->partial_parses = partial_parses;
  p->dont_fixup_internal_productions = !fixup;
  p->fixup_EBNF_productions = fixup_ebnf;
  p->dont_compare_stacks = !compare_stacks;
  p->commit_actions_interval = commit_actions_interval;
  p->start_state = start_state;
  for (i = 0; i < arg_state.nfile_arguments; i++) {
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

