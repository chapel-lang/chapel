// Error: accessing the while-var variable in while condition

class C {
  var x: int;
  proc show() { writeln("C(", x, ")"); }
}

var nums = [2, 5, 0, 3];

var Cs = [i in nums] if i then new C[i] else nil: owned C?;

const globalC = new C(11);

proc showCs() {
  var i = 0;
  while const Cnn = Cnn.next {  // error
    Cnn.show();
    i += 1;
  }
  writeln("nil.");
}

showCs();
