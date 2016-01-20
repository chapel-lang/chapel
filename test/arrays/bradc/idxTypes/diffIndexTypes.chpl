proc testit(type t) {
  writeln("Testing ", t:string, "...");
  var a: [0:t..4:t] real = (1.1, 2.2, 3.3, 4.4, 5.5);
  var b: [0:t..4:t by 2] real = (1.1, 3.3, 5.5);
  var c: [0:t..4:t by -1] real = (1.1, 2.2, 3.3, 4.4, 5.5);
  var d: [0:t..4:t by -2] real = (1.1, 3.3, 5.5);

  writeln("(Represented in domain using ", a.domain.idxType:string, ")");
  writeln("a is: ", a);
  writeln("b is: ", b);
  writeln("c is: ", c);
  writeln("d is: ", d);
  writeln();
}

testit(int(64));
testit(uint(64));
testit(int(32));
testit(uint(32));
testit(int(16));
testit(uint(16));
testit(int(8));
testit(uint(8));
