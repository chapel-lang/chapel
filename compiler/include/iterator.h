#ifndef _ITERATOR_H_
#define _ITERATOR_H_

class AggregateType;
class FnSymbol;

enum IteratorTag { 
  it_iterator, 
  it_leader,
  it_follower 
};

class IteratorInfo {
public:
  IteratorInfo();

  IteratorTag    tag;

  FnSymbol*      iterator;
  FnSymbol*      getIterator;
  AggregateType* iclass;
  AggregateType* irecord;
  FnSymbol*      advance;
  FnSymbol*      zip1;
  FnSymbol*      zip2;
  FnSymbol*      zip3;
  FnSymbol*      zip4;
  FnSymbol*      hasMore;
  FnSymbol*      getValue;
};

void lowerIterator(FnSymbol* fn);

#endif
