#ifndef _ITERATOR_H_
#define _ITERATOR_H_

class IteratorInfo {
public:
  ClassType* classType;
  FnSymbol* advance;
  FnSymbol* hasMore;
  FnSymbol* getValue;
  IteratorInfo();
};

void prototypeIteratorClass(FnSymbol* fn);
void lowerIterator(FnSymbol* fn);

#endif
