// TODO: This would need to be moved somewhere more standard
enum IteratorType { solo, leader, follower };

config param compilerRewrite = true;

var A: [0..8] real;

iter myIter() ref {
  for i in 0..8 {
    yield A(i);
  }
}

iter myIter(param tag: iterKind) where tag == iterKind.leader {
  yield 0..3;
  yield 4..5;
  yield 6..8;
}

iter myIter(param tag: iterKind, followThis) ref where tag == iterKind.follower {
  for i in followThis {
    yield A(i);
  }
}

if compilerRewrite {

  for a in myIter() do
    a = 1.1;

} else {

  for a in myIter() do
    a = 1.1;

}
writeln("A is: ", A);


if compilerRewrite {

  forall a in myIter() {
    a = 1.2;
  }

} else {

  // TODO: Next step would be to move coforall into leader itself
  coforall blk in myIter(leader=true) {
    for a in myIter(followThis=blk) {
      a = 1.2;
    }
  }

}

writeln("A is: ", A);
