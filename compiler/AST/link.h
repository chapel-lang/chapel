#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"

class AInfo; // analysis information

class ILink : public Loc {
 public:
  ILink* prev;
  ILink* next;
  AInfo* ainfo;

  ILink(void);

  virtual void print(FILE*) = 0;
  virtual void printList(FILE* outfile, char* separator = ", ");
  virtual void codegen(FILE* outfile) = 0;
  virtual void codegenList(FILE* outfile, char* separator = ", ");
  virtual bool isNull(void);

  void insert(ILink* newlink);
  void add(ILink* newlink);
  void append(ILink* newlink);

  void filter(bool filter(ILink*), ILink** truelinks, ILink** falselinks);
};

#define prevLink(type, node) (dynamic_cast<type*>((node)->prev))
#define nextLink(type, node) (dynamic_cast<type*>((node)->next))

#define appendLink(list, node) (((list) != NULL && !(list)->isNull()) ? \
                                (list)->append(node), (list) : \
                                (node))

#endif
