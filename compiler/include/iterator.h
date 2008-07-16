#ifndef _ITERATOR_H_
#define _ITERATOR_H_

class IteratorInfo {
public:
  FnSymbol* iterator;
  ClassType* icType;
  FnSymbol* advance;
  FnSymbol* zip1;
  FnSymbol* zip2;
  FnSymbol* zip3;
  FnSymbol* zip4;
  FnSymbol* hasMore;
  FnSymbol* getValue;
  IteratorInfo* leader;
  IteratorInfo* follower;
  FnSymbol* iterator2leader;
  FnSymbol* iterator2follower;
  IteratorInfo();
};

void lowerIterator(FnSymbol* fn);

#endif
