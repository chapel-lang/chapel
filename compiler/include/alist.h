#ifndef _CHPL_LIST_H_
#define _CHPL_LIST_H_

#include <stdio.h>
#include "baseAST.h"
#include "chplalloc.h"
#include "map.h"
#include "../traversals/fixup.h"
#include "../traversals/updateSymbols.h"

class Traversal;
class SymtabTraversal;

template <class elemType>
class AList : public BaseAST {
 public:
  elemType* head;
  elemType* tail;
  elemType* cursor;
  bool debugNestedTraversals;

  // constructors
  AList();
  AList(elemType*);
  void clear(void);

  // copy routines
  virtual AList<elemType>* copy(bool clone = false,
                                Map<BaseAST*,BaseAST*>* map = NULL,
                                Vec<BaseAST*>* update_list = NULL,
                                bool internal = false);

  // checks for length
  bool isEmpty(void);
  int length(void);

  // iteration
  elemType* first(void); // begin iteration over a list
  elemType* next(void);  // continue iteration over the list
  elemType* last(void);  // begin backwards iteration over a list
  elemType* prev(void);  // continue backward iteration over the list
  void reset(void); // reset iteration if terminated prematurely

  // other ways to get elements from the list
  elemType* representative(void); // return an arbitrary element in a list
  elemType* only(void); // return the single element in a list
  elemType* get(int index); // get the index-th element in a list

  // add element(s) before an insertion point or at beginning of list
  void insertAtHead(elemType* newElem);

  // add element(s) at end of list
  void insertAtTail(elemType* newElem);
  void add(AList<elemType>* newList); 

  // pop front of list off and return it
  elemType* popHead(void);

  // different ways to print/codegen lists
  void print(FILE* outfile, char* separator = ", ");
  void printDef(FILE* outfile, char* separator = ", ");
  void codegen(FILE* outfile, char* separator = ", ");
  void codegenDef(FILE* outfile, char* separator = ", ");

  // traversals
  void traverse(Traversal* traversal, bool atTop);
  void traverseDef(Traversal* traversal, bool atTop);
  void traverse(SymtabTraversal* traversal);

  // convert list to vector
  void getElements(Vec<elemType*>& elements);

  // sort this list's nodes into trueElems and falseElems, leaving the
  // original list empty.  trueElems and falseElems will be created if
  // passed in as NULL; will be added to otherwise
  void filter(bool filter(elemType*), AList<elemType>*& trueElems,
              AList<elemType>*& falseElems);
};

#define for_alist(elemtype, node, list)               \
  for (elemtype *node = list->first(),                \
         *_alist_next = (node) ? list->next() : NULL; \
       node;                                          \
       node = _alist_next,                            \
         _alist_next = (node) ? list->next() : NULL)

#define for_alist_backward(elemtype, node, list)  \
  for (elemtype *node = list->last(),                 \
         *_alist_prev = (node) ? list->prev() : NULL; \
       node;                                          \
       node = _alist_prev,                            \
         _alist_prev = (node) ? list->prev() : NULL)

// this is intended for internal use only
// note that we store a node one ahead in case the current node is
// removed or something
#define _for_all_elems(node)                                            \
  elemType* node;                                                       \
  BaseAST* nextNode;                                                    \
  for (node = dynamic_cast<elemType*>(head->next), nextNode = node->next; \
       node != tail;                                                    \
       node = dynamic_cast<elemType*>(nextNode), nextNode = node->next)

#define _for_all_elems_nocache(node)                                    \
  elemType* node;                                                       \
  for (node = dynamic_cast<elemType*>(head->next);                      \
       node != tail;                                                    \
       node = dynamic_cast<elemType*>(node->next))


template <class elemType>
AList<elemType>::AList() :
  BaseAST(LIST),
  head(new elemType()),
  tail(new elemType()),
  cursor(NULL),
  debugNestedTraversals(false)
{
  clear();
}


template <class elemType>
AList<elemType>::AList(elemType* initList) :
  BaseAST(LIST),
  head(new elemType()),
  tail(new elemType()),
  cursor(NULL),
  debugNestedTraversals(false)
{
  clear();
  if (initList) {
    insertAtHead(initList);
  }
}


template <class elemType>
void AList<elemType>::clear(void) {
  head->next = tail;
  tail->prev = head;
}


template <class elemType>
bool AList<elemType>::isEmpty(void) {
  if (this == NULL) {
    return true;
  } else {
    return (head->next == tail);
  }
}


template <class elemType>
int AList<elemType>::length(void) {
  int numNodes = 0;
  _for_all_elems(node) {
    numNodes++;
  }
  return numNodes;
}


template <class elemType>
elemType* AList<elemType>::first(void) {
  if (this == NULL) {
    return NULL;
  }
  if (cursor != NULL) {
    if (debugNestedTraversals) {
      INT_FATAL(this, "Nested list iteration detected\n");
    }
  }
  if (isEmpty()) {
    cursor = NULL;
  } else {
    cursor = dynamic_cast<elemType*>(head->next);
  }
  return cursor;
}


template <class elemType>
elemType* AList<elemType>::next(void) {
  cursor = dynamic_cast<elemType*>(cursor->next);
  if (cursor == tail) {
    cursor = NULL;
  }
  return cursor;
}


template <class elemType>
elemType* AList<elemType>::last(void) {
  if (this == NULL) {
    return NULL;
  }
  if (cursor != NULL) {
    if (debugNestedTraversals) {
      INT_FATAL(this, "Nested list iteration detected\n");
    }
  }
  if (isEmpty()) {
    cursor = NULL;
  } else {
    cursor = dynamic_cast<elemType*>(tail->prev);
  }
  return cursor;
}


template <class elemType>
elemType* AList<elemType>::prev(void) {
  cursor = dynamic_cast<elemType*>(cursor->prev);
  if (cursor == head) {
    cursor = NULL;
  }
  return cursor;
}


template <class elemType>
void AList<elemType>::reset(void) {
  cursor = NULL;
}


template <class elemType>
elemType* AList<elemType>::representative(void) {
  if (isEmpty()) {
    INT_FATAL(this, "representative() called on empty list");
  }
  return dynamic_cast<elemType*>(head->next);
}


template <class elemType>
elemType* AList<elemType>::only(void) {
  if (isEmpty()) {
    INT_FATAL(this, "only() called on empty list");
  }
  if (head->next->next == tail) {
    return representative();
  } else {
    INT_FATAL(this, "only() called on list with more than one element");
    return NULL;
  }
}


template <class elemType>
elemType* AList<elemType>::get(int index) {
  if (index <=0) {
    INT_FATAL(this, "Indexing list must use positive integer");
  }
  int i = 0;
  _for_all_elems(node) {
    i++;
    if (i == index) {
      return node;
    }
  }
  INT_FATAL(this, "Indexing list out of bounds");
  return NULL;
}


template <class elemType>
void AList<elemType>::insertAtHead(elemType* newElem) {
  head->next->insertBefore(newElem);
}


template <class elemType>
void AList<elemType>::insertAtTail(elemType* newElem) {
  tail->prev->insertAfter(newElem);
}


template <class elemType>
void AList<elemType>::add(AList<elemType>* newList) {
  if (newList->isEmpty()) {
    return;
  }
  if (isEmpty()) {
    // adopt newList's nodes and size
    head->next = newList->head->next;
    tail->prev = newList->tail->prev;
    head->next->prev = head;
    tail->prev->next = tail;
  } else {
    elemType* node = newList->popHead();
    while (node) {
      insertAtTail(node);
      node = newList->popHead();
    }
  }
  // empty newList
  newList->clear();
}


template <class elemType>
elemType* AList<elemType>::popHead(void) {
  elemType* pop = dynamic_cast<elemType*>(head->next);

  if (isEmpty()) {
    return NULL;
  } else {
    head->next = pop->next;
    pop->next->prev = head;
    pop->next = NULL;
    pop->prev = NULL;

    return pop;
  }
}


template <class elemType>
void AList<elemType>::print(FILE* outfile, char* separator) {
  _for_all_elems(node) {
    node->print(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


template <class elemType>
void AList<elemType>::printDef(FILE* outfile, char* separator) {
  _for_all_elems(node) {
    node->printDef(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


template <class elemType>
void AList<elemType>::codegen(FILE* outfile, char* separator) {
  // we use the nocache version here only to support the _ArrayWriteStopgap hack
  // it relies on the ability to remove a "next" statement at codegen time
  _for_all_elems_nocache(node) {
    node->codegen(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


template <class elemType>
void AList<elemType>::codegenDef(FILE* outfile, char* separator) {
  _for_all_elems(node) {
    node->codegenDef(outfile);
    if (node->next != tail) {
      fprintf(outfile, "%s", separator);
    }
  }
}


template <class elemType>
AList<elemType>*
AList<elemType>::copy(bool clone,
                      Map<BaseAST*,BaseAST*>* map,
                      Vec<BaseAST*>* update_list,
                      bool internal) {
  if (isEmpty()) {
    return new AList<elemType>();
  }

  if (!map) {
    map = new Map<BaseAST*,BaseAST*>();
  }

  AList<elemType>* newList = new AList<elemType>();
  _for_all_elems(node) {
    elemType* newnode = COPY_INTERNAL(node);
    newnode->next = NULL;
    newnode->prev = NULL;
    newList->insertAtTail(newnode);
  }

  newList->copyFrom = this;
  newList->lineno = lineno;
  newList->filename = filename;
  newList->copyPragmas(pragmas);
  if (!internal) {
    if (update_list) {
      for (int j = 0; j < update_list->n; j++) {
        for (int i = 0; i < map->n; i++) {
          if (update_list->v[j] == map->v[i].key) {
            update_list->v[j] = map->v[i].value;
          }
        }
      }
    }
    TRAVERSE(newList, new UpdateSymbols(map), true);
  }
  return newList;
}


template <class elemType>
void AList<elemType>::traverse(Traversal* traversal, bool atTop) {
  if (dynamic_cast<Fixup*>(traversal)) {
    head->traverse(traversal, false);
    tail->traverse(traversal, false);
  }
  _for_all_elems(node) {
    node->traverse(traversal, false);
  }
}


template <class elemType>
void AList<elemType>::traverseDef(Traversal* traversal, bool atTop) {
  _for_all_elems(node) {
    node->traverseDef(traversal, false);
  }
}


template <class elemType>
void AList<elemType>::traverse(SymtabTraversal* traversal) {
  _for_all_elems(node) {
    node->traverse(traversal);
  }
}


template <class elemType>
void AList<elemType>::getElements(Vec<elemType*>& elements) {
  _for_all_elems(node) {
    elements.add(node);
  }
}


template <class elemType>
void AList<elemType>::filter(bool filter(elemType*), 
                             AList<elemType>*& trueElems,
                             AList<elemType>*& falseElems) {
  if (trueElems == NULL) {
    trueElems = new AList<elemType>();
  }
  if (falseElems == NULL) {
    falseElems = new AList<elemType>();
  }

  elemType* node = popHead();
  while (node) {
    if (filter(node)) {
      trueElems->insertAtTail(node);
    } else {
      falseElems->insertAtTail(node);
    }

    node = popHead();
  }
  clear();
}

#endif
