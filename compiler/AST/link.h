#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"
#include "misc.h"
#include "traversal.h"
#include "vec.h"

class ILink : public Loc {
 public:
  ILink* prev;
  ILink* next;

  ILink(void);

  virtual bool isNull(void);
  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseList(Traversal* traversal, bool atTop = true);

  virtual void print(FILE* outfile);
  virtual void printList(FILE* outfile, char* separator = ", ");
  virtual void codegen(FILE* outfile);
  virtual void codegenList(FILE* outfile, char* separator = ", ");

  //  void insert(ILink* newlink);
  //  void add(ILink* newlink);
  void append(ILink* newlink);

  void filter(bool filter(ILink*), ILink** truelinks, ILink** falselinks);
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
