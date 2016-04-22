use List;

class C {
  var x: int;
}

var s: list(C);

writeln(s);

s.append(new C(2));

writeln(s);

for c in s do delete c;

s.destroy();

