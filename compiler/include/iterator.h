#ifndef _ITERATOR_H_
#define _ITERATOR_H_

class IteratorInfo {
public:
  ClassType* classType;
  FnSymbol* advance;
  FnSymbol* hasMore;
  FnSymbol* getValue;
  FnSymbol* zip1;
  FnSymbol* zip2;
  FnSymbol* zip3;
  FnSymbol* zip4;
  IteratorInfo();
};

void prototypeIteratorClass(FnSymbol* fn);
void lowerIterator(FnSymbol* fn);

#endif
