// TODO: This would need to be moved somewhere more standard
enum IteratorType { solo, leader, follower };

config param compilerRewrite = true;

var A: [0..8] real;

def myIter() var {
  for i in 0..8 {
    yield A(i);
  }
}

def myIter(param tag: iterator) where tag == iterator.leader {
  yield 0..3;
  yield 4..5;
  yield 6..8;
}

def myIter(param tag: iterator, follower) var where tag == iterator.follower {
  for i in follower {
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
    for a in myIter(follower=blk) {
      a = 1.2;
    }
  }

}

writeln("A is: ", A);
