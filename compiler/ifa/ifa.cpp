#include "ifa.h"
#include "ast.h"
#include "if1.h"
#include "cg.h"
#include "clone.h"
#include "dead.h"
#include "dom.h"
#include "fa.h"
#include "fun.h"
#include "inline.h"
#include "pdb.h"
#include "html.h"
#include "graph.h"

void ifa_init(IFACallbacks *callbacks) {
  new IF1;
  new PDB(if1);
  init_ast(callbacks);
}

int
ifa_analyze(char *fn) {
  if1_finalize(if1);
  if1_write_log();
  if (!fdce_if1)
    fail("unable to translate dead code");
  Sym *init = if1_get_builtin(if1, "init");
  for (int i = 0; i < if1->allclosures.n; i++) {
    Fun *f = new Fun(if1->allclosures.v[i], if1->allclosures.v[i] == init);
    if (!f)
      fail("IF1 invalid");
    pdb->add(f);
  }
  FA *fa = pdb->fa;
  fa->fn = fn;
  if (fa->analyze(if1->top->fun) < 0)
    return -1;
  if (clone(fa, if1->top->fun) < 0)
    return -1;
  if (mark_dead_code(fa, if1->top->fun) < 0)
    fail("dead code detection failed");
  if (logging(LOG_TEST_FA))
    log_test_fa(fa);
  forv_Fun(f, fa->funs)
    build_forward_cfg_dominators(f);
  frequency_estimation(fa);
  return 0;
}

void 
ifa_graph(char *fn) {
  graph(pdb->fa, fn);
}

void 
ifa_html(char *fn) {
  dump_html(pdb->fa, fn);
}

void 
ifa_cg(char *fn) {
  cg_write_c(pdb->fa, if1->top->fun,  fn);
}

void 
ifa_compile(char *fn) {
  cg_compile(fn);
}

