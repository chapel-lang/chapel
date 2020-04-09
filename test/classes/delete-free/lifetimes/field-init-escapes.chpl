class C {
  var x: int;

  proc init(x) {
    this.x = x;
    writeln("Initing C");
  }
  proc deinit() {
    writeln("De-initing C");
  }
}

class D {
  var x: borrowed C;

  proc init() {
    this.x = new C(42);
  }
}

var myD = new D();
writeln(myD);
