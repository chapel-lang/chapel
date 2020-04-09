config const parallelFactor = 2; // amount of parallelism to exploit
config const myLow = 4, myHigh = 12; // iterator arguments (config for testing)

enum IteratorType { solo, leader, follower };

iter singleLocaleIterator(param iterator: IteratorType, iterRange:range) {
  var low  = iterRange.low;
  var high = iterRange.high;
  select iterator {
    when IteratorType.solo {
      for i in low..high do
        yield i;
    }
    when IteratorType.leader {
      for i in 1..parallelFactor do
        yield low+(high-low+1)*(i-1)/parallelFactor..low-1+(high-low+1)*i/parallelFactor;
    }
    when IteratorType.follower {
      for i in iterRange do
        yield i;
    }
  }
}

writeln("parallel iteration");
// forall i in singleLocaleIterator(myLow..myHigh) do writeln(i);
coforall block in singleLocaleIterator(IteratorType.leader, myLow..myHigh) {
  for i in singleLocaleIterator(IteratorType.follower, block) {
    writeln(i);
  }
}

writeln("serial iteration");
// for i in singleLocaleIterator(myLow..myHigh) do writeln(i);
for i in singleLocaleIterator(IteratorType.solo, myLow..myHigh) {
  writeln(i);
}

writeln("parallel zipper iteration try 1");
// forall i in zip(singleLocaleIterator(myLow..myHigh),
//              singleLocaleIterator(myLow+2..myHigh+2)) do writeln(i);
coforall block in singleLocaleIterator(IteratorType.leader, myLow..myHigh) {
  for i in zip(singleLocaleIterator(IteratorType.follower, block),
            singleLocaleIterator(IteratorType.follower, 2+block)) {
    writeln(i);
  }
}

//
// Here, the leader iterators are each called and zippered together.
// The followers are also zippered. This way, the leaders can have different
// policies about, for example, what to give to the followers. In this case,
// both give the exact range for the follower to iterate over. In Steve's
// code that this is based from, the leader always yields an offset range
// that is zero based, so the low end of the actual range always needs to
// be added to what the leader yields.
// This code was based from:
// test/functions/deitz/iterators/parallel_thoughts2.chpl
//

writeln("parallel zipper iteration try 2");
// forall i in zip(singleLocaleIterator(myLow..myHigh),
//              singleLocaleIterator(myLow+2..myHigh+2)) do writeln(i);
coforall (block1, block2) in
  zip(singleLocaleIterator(IteratorType.leader, myLow..myHigh),
   singleLocaleIterator(IteratorType.leader, myLow+2..myHigh+2)) {
  for i in zip(singleLocaleIterator(IteratorType.follower, block1),
            singleLocaleIterator(IteratorType.follower, block2)) {
    writeln(i);
  }
}


