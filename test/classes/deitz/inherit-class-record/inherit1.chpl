use List;

module M1 {
  class C {
    var s: list(int);

    proc deinit() {
      s.destroy();
    }
  }
}

use M1;

class D: C {
  var i: int;
}

proc main() {
  var d = new D();

  d.s.append(4);
  d.s.append(5);
  d.s.append(6);
  d.i = 7;

  writeln(d);

  delete d;
}
