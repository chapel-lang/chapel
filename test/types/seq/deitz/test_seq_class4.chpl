use List;

class C {
  var x: int;
}

class D: C {
  var y: int;
}

var s: list of C;

s.append(D(1, 2));
s.append(C(3));

writeln(s);
