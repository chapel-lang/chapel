#ifndef _SYMLINK_H_
#define _SYMLINK_H_

#include "link.h"
#include "symbol.h"

class SymLink : public ILink {
public:
  Symbol* pSym;
  
  SymLink(Symbol* init_pSym = NULL);

  void traverse(Traversal* traversal);
  //  void traverseLinks(SymtabTraversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};

#endif
