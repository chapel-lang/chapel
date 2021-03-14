
iter myIter() {
  writeln("start myIter");
  yield [1, 2, 3];
  yield [4, 5, 6];
  writeln("finish myIter");
}

var r = + reduce [idx in myIter()] idx;
writeln(r);
