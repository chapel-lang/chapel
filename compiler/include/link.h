#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"
#include "misc.h"
#include "vec.h"
#include "../traversals/traversal.h"

#define TRAVERSE(link, traversal, atTop) \
  link->traverse(link, traversal, atTop)

#define TRAVERSE_LS(link, traversal, atTop) \
  link->traverseList(link, traversal, atTop)

#define TRAVERSABLE_ILINK(name)                                                      \
 public:                                                                             \
  void traverse(name* &_this, Traversal* traversal, bool atTop = true);              \
  void traverseList(name* &_this, Traversal* traversal, bool atTop = true) {         \
    if (isNull()) return;                                                            \
    TRAVERSE(_this, traversal, atTop);                                               \
    TRAVERSE_LS(_this->next, traversal, atTop);                                      \
  }

class ILink : public Loc {
  TRAVERSABLE_ILINK(ILink);
 public:
  ILink* prev;
  ILink* next;

  ILink(void);

  virtual bool isNull(void);

  virtual void print(FILE* outfile);
  void println(FILE* outfile);
  virtual void printList(FILE* outfile, char* separator = ", ");
  virtual void codegen(FILE* outfile);
  virtual void codegenList(FILE* outfile, char* separator = ", ");

  void preinsert(ILink* newlink);
  void postinsert(ILink* newlink);
  void add(ILink* newlink);
  void append(ILink* newlink);
  ILink* extract(void);

  void filter(bool filter(ILink*), ILink** truelinks, ILink** falselinks);

  static void replace(ILink* old_link, ILink* new_link);
};

extern ILink* nilILink;

#define prevLink(type, node) (dynamic_cast<type*>((node)->prev))
#define nextLink(type, node) (dynamic_cast<type*>((node)->next))

#define appendLink(list, node) ((((list) == NULL) || (list)->isNull()) ? \
                                (node) : \
                                ((list)->append(node), (list)))

template <class astType>
void getLinkElements(Vec<astType*>& elements, ILink* link) {
  astType* element;

  elements.clear();
  while (!link->isNull()) {
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
