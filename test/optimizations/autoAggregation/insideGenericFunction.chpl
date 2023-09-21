use BlockDist;

var myArr = blockDist.createArray({0..10}, int);

proc main() {
  foo(myArr);
}

proc foo(ref a) {
  var b = blockDist.createArray({0..10}, int);
  for bb in b do bb = 1;

  writeln("Loop 1");
  forall i in a.domain with (ref a) {
    a[i] = b[10-i];
  }
  writeln("End Loop 1");
  writeln(a);
  writeln();

  for bb in b do bb = 2;

  writeln("Loop 2");
  forall (aElem, i) in zip(a, 0..10) {
    aElem = b[10-i];
  }
  writeln("End Loop 2");
  writeln(b);
  writeln();
}
