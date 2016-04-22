use List;

class C {
  var x: int;
}

class D: C {
  var y: int;
}

var s: list(C);

s.append(new D(1, 2));
s.append(new C(3));

writeln(s);

for c in s do delete c;

s.destroy();
