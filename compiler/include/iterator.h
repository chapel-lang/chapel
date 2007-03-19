#ifndef _ITERATOR_H_
#define _ITERATOR_H_

class IteratorInfo {
public:
  ClassType* seqType;
  ClassType* classType;
  FnSymbol* getHeadCursor;
  FnSymbol* getNextCursor;
  FnSymbol* isValidCursor;
  FnSymbol* getValue;
  IteratorInfo();
};

void lowerIterator(FnSymbol* fn);

#endif
