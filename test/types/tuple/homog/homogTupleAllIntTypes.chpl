proc testit(type nt, type t) {
  param n: nt = 5: nt;
  var myTuple: n*t;
  writeln(myTuple);
}

testit(int(8), int);
testit(int(16), int);
testit(int(32), int);
testit(int(64), int);
testit(uint(8), int);
testit(uint(16), int);
testit(uint(32), int);
testit(uint(64), int);
