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
var r = new A.R(1);
writeln(r); // {a = 2}
