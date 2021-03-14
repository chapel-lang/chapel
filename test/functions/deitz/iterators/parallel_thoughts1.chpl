config const parallelFactor = 2; // amount of parallelism to exploit
config const myLow = 4, myHigh = 12; // iterator arguments (config for testing)

//
// This is a basic parallel iterator that iterates over the numbers
// between 'low' and 'high' inclusive.  It demonstrates the basic
// usage of the leader-follower mechanism.  This is implemented with
// two special arguments: 'leader' and 'follower'.
//
// The argument 'leader' determines whether the iterator is being
// called in the leader context (when true) or the follower context
// (when false).  The compiler will insert the actual as appropriate.
//
// The argument 'follower' is generic and should only be used when
// 'leader' is false.  In the simple case (one dimension), it is used
// only to capture the range that the leader identifies that we should
// iterate over.
//
iter singleLocaleIterator(low: int, high: int, param leader: bool, followThis) {
  if leader {
    for i in 1..parallelFactor do
      yield 1+(high-low+1)*(i-1)/parallelFactor..(high-low+1)*i/parallelFactor;
  } else {
    for i in followThis do
      yield low+i-1;
  }
}

//
// In the parallel context, the programmer may write something like
//
//   forall i in singleLocaleIterator(myLow, myHigh) do writeln(i);
//
// The compiler would translate this into the following:
//
coforall block in singleLocaleIterator(myLow, myHigh, true, 0) {
  for i in singleLocaleIterator(myLow, myHigh, false, block) {
    writeln(i);
  }
}

//
// To zipper in parallel, the programmer may write something like
//
//   forall i in (singleLocaleIterator(myLow, myHigh),
//                singleLocaleIterator(myLow+2, myHigh+2)) do writeln(i);
//
// The compiler would translate this into the following:
//
coforall block in singleLocaleIterator(myLow, myHigh, true, 0) {
  for i in zip(singleLocaleIterator(myLow, myHigh, false, block),
            singleLocaleIterator(myLow+2, myHigh+2, false, block)) {
    writeln(i);
  }
}

//
// More notes:
//
// I've made 'leader' and 'followThis' explicit arguments, but we can
// make them implicit too.  Then we only need a way to distinguish
// between parallel iterators that use the leader-follower mechanism
// and serial iterators that do not.  One way to do this would be to
// simply look for uses of the implicit parameter 'leader'.
//
