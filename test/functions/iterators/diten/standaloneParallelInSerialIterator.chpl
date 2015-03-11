iter myiter(n: int) {
  for i in 1..n do yield i;
}

iter myiter(n: int, param tag: iterKind) where tag == iterKind.leader {
  assert(n % 2 == 0);
  yield 1..(n/2);
  yield (n/2+1)..n;
}

iter myiter(followThis, param tag: iterKind) where tag  == iterKind.follower {
  for i in followThis {
    yield i;
  }
}

config param useStandalone = true;

iter myiter(n: int, param tag: iterKind) where tag == iterKind.standalone &&
                                               useStandalone {
  assert(n % 2 == 0);
  cobegin {
    for i in 1..(n/2) do yield i;
    for i in (n/2+1)..n do yield i;
  }
}

iter mySerialIter(n: int) {
  forall i in myiter(n) {
    yield i;
  }
}

for i in mySerialIter(10) {
  writeln(i);
}
