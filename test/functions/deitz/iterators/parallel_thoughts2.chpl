config const parallelFactor = 2; // amount of parallelism to exploit
config const myLow = 4, myHigh = 12; // iterator arguments (config for testing)

//
// In the serial context, we can simply change the 'coforall' into a
// 'for' loop, but we may want to make this more explicit so that it
// can be more easily and efficiently zippered with a standard serial
// iterator.  The problem is that we don't want to block up the
// iteration space if we don't need to especially if we are going to
// zip it up with a serial iterator that does no such blocking.
//
// One way to do that would be to replace 'leader' by an argument of
// enumerated type 'IteratorType' called 'iterator' where the values
// in IteratorType are solo, leader, and follower.  The solo case
// handles calling this iterator in a serial (at compile-time)
// context.
//
enum IteratorType { solo, leader, follower };

iter singleLocaleIterator(low: int, high: int,
                         param iterator: IteratorType, followThis) {
  if iterator == IteratorType.solo {
    for i in low..high do
      yield i;
  } else if iterator == IteratorType.leader {
    for i in 1..parallelFactor do
      yield 1+(high-low+1)*(i-1)/parallelFactor..(high-low+1)*i/parallelFactor;
  } else {
    for i in followThis do
      yield low+i-1;
  }
}

writeln("parallel iteration");
// forall i in singleLocaleIterator(myLow, myHigh) do writeln(i);
coforall block in singleLocaleIterator(myLow, myHigh, IteratorType.leader, 0) {
  for i in singleLocaleIterator(myLow, myHigh, IteratorType.follower, block) {
    writeln(i);
  }
}

writeln("serial iteration");
// for i in singleLocaleIterator(myLow, myHigh) do writeln(i);
for i in singleLocaleIterator(myLow, myHigh, IteratorType.solo, 0) {
  writeln(i);
}

//
// More notes:
//
// I think coforall should iterate over leader-follower iterators in
// solo mode thereby invoking all the parallelism outside of the
// iterator.
//
