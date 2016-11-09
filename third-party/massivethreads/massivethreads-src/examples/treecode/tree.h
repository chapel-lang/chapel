/* tree.h */

#ifndef TREE_H
#define TREE_H

#include "vector.h"
#include "mol.h"

#define NSUBS 8
#define BODY  0
#define CELL  1

struct node {
  // tree structure
  node *parent;
  node **subs;

  // data
  int type;
  real mass;
  vector r;

  node(void);
  void split(int nsubs = NSUBS);
};

struct body : node {
  vector rv;
  vector ra;
  body(void);
};

struct cell : node {
  cell(void);
};


struct tree {
  node *root;
  int nnode;  // number of node
  int depth;  // tree levels
  
  void print(void);
  void print_tree(node *);
};

// Memory allocation wrappers
body * newbody(void);
cell * newcell(void);

#endif /* TREE_H */
