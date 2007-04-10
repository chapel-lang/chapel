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

  d.s._append_in_place(4);
  d.s._append_in_place(5);
  d.s._append_in_place(6);
  d.i = 7;

  writeln(d);
}
