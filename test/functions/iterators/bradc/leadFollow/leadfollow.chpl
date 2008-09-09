// TODO: This would need to be moved somewhere more standard
enum IteratorType { solo, leader, follower };

config param compilerRewrite = true;

var A: [0..8] real;

def myIter() {
  for i in 0..8 {
    yield i;
  }
}

def myIter(leader) {
  yield 0..3;
  yield 4..5;
  yield 6..8;
}

def myIter(follower) {
  for i in follower {
    yield i;
  }
}

if compilerRewrite {

  for i in myIter() do
    A(i) = i/10.0;

} else {

  for i in myIter() do
    A(i) = i/10.0;

}
writeln("A is: ", A);


if compilerRewrite {

  forall i in myIter() {
    A(i) = i + i/10.0;
  }

} else {

  // TODO: Next step would be to move coforall into leader itself
  coforall blk in myIter(leader=true) {
    for i in myIter(follower=blk) {
      A(i) = i + i/10.0;
    }
  }

}

writeln("A is: ", A);
