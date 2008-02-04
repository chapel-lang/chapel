use List;

class C {
  var x: int;
}

var s: list of C;

writeln(s);

s.append(new C(2));

writeln(s);
