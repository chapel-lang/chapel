/* tree.cc */

#include <cstdio>
#include "utils.h"
#include "tree.h"

node::node(void) {
  parent = NULL;
  subs = NULL;
}

void node::split(int nsubs)
{
#if USE_MALLOC
  subs = xmalloc(sizeof(node*) * nsubs);
#else
  subs = new node*[nsubs];
#endif
}

cell::cell(void)
{
  type = CELL;
}

body::body(void)
{
  type = BODY;
}

void tree::print(void)
{
  print_tree(root);
}

void tree::print_tree(node *t)
{
  static int level = 0;
  if (t != NULL) {
    if (t->type == BODY) 
      printf("%*s" "body: \n", level, "");
    else if (t->type == CELL)
      printf("%*s" "cell: \n", level, "");
    if (t->subs != NULL) {
      level++;
      for (int i = 0; i < NSUBS; i++)
        print_tree(t->subs[i]);
    }
  }
}

body * newbody(void)
{
#if USE_MALLOC
  return (body *) xmalloc(sizeof(body));
#else
  return new body();
#endif
}

cell * newcell(void)
{
#if USE_MALLOC
  return (cell *) xmalloc(sizeof(cell));
#else
  return new cell();
#endif
}
