class C {
  var x: int;
}

class D {
  var y: borrowed C;

  proc init() {
    this.y = new unmanaged C(42);
  }

  proc deinit() {
    delete (this.y): unmanaged C;
  }
}

var myD = new D();
writeln(myD);
