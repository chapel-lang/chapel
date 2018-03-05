module A {
  record R {
    var a: int;

    proc init(a: int) {
      this.a = a + 1;
      super.init();
    }
  }
}

use A;
var r: A.R = new A.R(1); // Doesn't work.
/* var r: R = new R(1); // Works. */
writeln(r); // {a = 2}
