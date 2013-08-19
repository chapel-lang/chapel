#ifndef _ITERATOR_H_
#define _ITERATOR_H_

enum IteratorTag { it_iterator, it_leader, it_follower };

class IteratorInfo {
public:
  IteratorTag tag;
  FnSymbol* iterator;
  FnSymbol* getIterator;
  ClassType* iclass;
  ClassType* irecord;
  FnSymbol* advance;
  FnSymbol* zip1;
  FnSymbol* zip2;
  FnSymbol* zip3;
  FnSymbol* zip4;
  FnSymbol* hasMore;
  FnSymbol* getValue;
  IteratorInfo();
};

void lowerIterator(FnSymbol* fn);

extern Vec<FnSymbol*> iteratorsWithRemovedRetInitSet;

#endif
