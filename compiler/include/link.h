#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"
#include "misc.h"
#include "vec.h"
#include "../traversals/traversal.h"


class ILink : public Loc {
 public:
  ILink* prev;
  ILink* next;

  ILink(void);

  virtual void traverse(Traversal* traversal, bool atTop = true);
  void traverseList(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);
  void traverseDefList(Traversal* traversal, bool atTop = true);

  virtual void print(FILE* outfile);
  void println(FILE* outfile);
  virtual void printList(FILE* outfile, char* separator = ", ");
  virtual void codegen(FILE* outfile);
  virtual void codegenList(FILE* outfile, char* separator = ", ");

  virtual void append(ILink* newlink);

  void filter(bool filter(ILink*), ILink** truelinks, ILink** falselinks);

  ILink* head(void);
  ILink* tail(void);
  int length(void);
  ILink* get(int);
};

#define prevLink(type, node) (dynamic_cast<type*>((node)->prev))
#define nextLink(type, node) (dynamic_cast<type*>((node)->next))

#define appendLink(list, node) \
  ((!(list)) ? (node) : ((list)->append(node), (list)))

template <class astType>
void getLinkElements(Vec<astType*>& elements, ILink* link) {
  astType* element;

  elements.clear();
  while (link) {
    element = dynamic_cast<astType*>(link);
    if (element) {
      elements.add(element);
    } else {
      INT_FATAL(link, "Unexpected type in getElements");
    }

    link = link->next;
  }
}

#endif
