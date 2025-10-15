
proc testRange(type t) {
  var x = 1:t..10:t by 2;

  var y = x(2:t..4:t);
  writeln(y);
}

testRange(int(8));
testRange(int(16));
testRange(int(32));
testRange(int(64));
testRange(uint(8));
testRange(uint(16));
testRange(uint(32));
testRange(uint(64));
