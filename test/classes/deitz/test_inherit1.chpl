module M1 {
  class C {
    var s: seq of int;
  }
}

use M1;

class D: C {
  var i: int;
}

def main() {
  var d = D();

  d.s #= 4;
  d.s #= 5;
  d.s #= 6;
  d.i = 7;

  writeln(d);
}
