#include "../gramgram.h"
#include "../d.h"

extern D_ParserTables parser_tables_dparser_gram;

int
make_tables(char *grammar_pathname) {
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
  if (parse_grammar(g, grammar_pathname, &parser_tables_dparser_gram, sizeof(ParseNode_User)) < 0)
    d_fail("unable to parse grammar '%s'", grammar_pathname);
  if (g->productions.n < 2)
    d_fail("no productions in grammar '%s'", grammar_pathname);
  if (build_grammar(g) < 0)
    d_fail("unable to load grammar '%s'", grammar_pathname);
  if (write_binary_tables(g) < 0)
      d_fail("unable to write tables '%s'", grammar_pathname);
  free_D_Grammar(g);
  return 0;
}
