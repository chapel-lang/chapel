// ensure that the serial iterator is called no matter which 'foreach' is called

iter myIter() {
  writeln("Serial Iter");
  for i in 1..10 do yield i;
}

iter myIter(param tag: iterKind) where tag == iterKind.standalone {
  writeln("Standalone Iter");
  for i in 1..10 do yield i;
}

iter myIter(param tag: iterKind) where tag == iterKind.leader {
  writeln("Leader Iter");
  yield 1..10;
}

iter myIter(param tag: iterKind, followThis) where tag == iterKind.follower {
  writeln("Follower Iter");
  for i in followThis do yield i;
}

foreach i in myIter() do writeln(i);

foreach i in zip(myIter(), myIter()) do writeln(i);

foreach i in zip(myIter(), myIter()) do writeln(i);


foreach i in myIter() do writeln(i);

foreach i in zip(myIter(), myIter()) do writeln(i);

foreach i in zip(myIter(), myIter()) do writeln(i);
