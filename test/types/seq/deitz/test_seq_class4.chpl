class C {
  var x: int;
}

class D: C {
  var y: int;
}

var s: seq of C;

s #= D(1, 2);
s #= C(3);

writeln(s);
