
class C {
  var x : int;
  var y : real;

  proc init() {
    writeln("C.init");
    this.x = 5;
    this.y = 1.0;
  }

  proc postinit() {
    writeln("C.postinit");
    y *= x;
  }
}

var c = new unmanaged C();
writeln("c = ", c);
writeln();
delete c;

record R {
  var x : int;

  proc init() {
    writeln("R.init");
  }

  proc postinit() {
    writeln("R.postinit");
  }
}

var r = new R();
