#include "../gramgram.h"
#include "../d.h"
#include "../mkdparse.h"

int
make_tables(char *grammar_string, char *grammar_pathname) {
  Grammar *g;
  g = new_D_Grammar(grammar_pathname);
  g->set_op_priority_from_rule = 0;
  g->right_recursive_BNF = 0;
  g->states_for_whitespace = 1;
  g->states_for_all_nterms = 1;
  g->tokenizer = 0;
  g->longest_match = 0;
  strcpy(g->grammar_ident, "gram");
  g->scanner_blocks = 4;
  g->scanner_block_size = 0;
  g->write_line_directives = 1;
  g->write_header = -1;
  g->token_type = 0;
  strcpy(g->write_extension, "dat");

  mkdparse_from_string(g, grammar_string);

  if (write_binary_tables(g) < 0)
    d_fail("unable to write tables");

  free_D_Grammar(g);
  return 0;
}
