
class C {
  var x : int;
  var y : real;

  proc init() {
    writeln("C.init");
    this.x = 5;
    this.y = 1.0;
  }

  proc postInit() {
    writeln("C.postInit");
    y *= x;
  }
}

var c = new C();
writeln("c = ", c);
writeln();
delete c;

record R {
  var x : int;

  proc init() {
    writeln("R.init");
  }

  proc postInit() {
    writeln("R.postInit");
  }
}

var r = new R();
