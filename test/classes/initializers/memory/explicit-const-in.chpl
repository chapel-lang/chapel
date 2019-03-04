
// BHARSH 2018-05-11: At one point the compiler was handling destruction of
// formals with 'const in' intent differently from 'in' intent. This test
// tracks the amount of initializing and destroying to avoid future
// double-frees.

record R {
  var x : int;
  proc init() {
    this.x = 0;
    writeln("R.init: ", x);
  }
  proc init=(other:R) {
    this.x = other.x + 1;
    writeln("R.init=(R): ", x);
  }
  proc deinit() {
    writeln("R.deinit: ", x);
  }
}

class C {
  var r : R;
  proc init(const in r : R) {
    this.r = r;
  }
}

var r = new R();
var c = new owned C(r);
writeln("c = ", c);
