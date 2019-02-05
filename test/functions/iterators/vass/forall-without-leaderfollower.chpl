
record TEST {
  iter these() { yield 5; }
}
var t: TEST;
[i in t]
  writeln(i);

class CTEST {
  iter these() { yield 6; yield 7; }
  iter these(param tag, followThis) { yield followThis; } // should be unused
}
var c = new owned CTEST();
[j in c]
  writeln(j);

writeln("done");
