// Test various successful scenarios of iterating over an array

class C {
  var x: int;
  proc show() { writeln("C(", x, ")"); }
}

var nums = [2, 5, 0, 3];

var Cs = [i in nums] if i then new C[i] else nil: owned C?;

const globalC = new C(11);

proc showCs() {
  var i = 0;
  while const Cnn = Cs[i] {
    Cnn.show();
    i += 1;
  }
  writeln("nil.");
}

showCs();

var i = 0;
while var Cnn = Cs[i] {
  Cnn.show();
  Cnn = globalC; // borrows; does not affect Cs
  Cnn.show();
  i += 1;
}

writeln(Cs);
