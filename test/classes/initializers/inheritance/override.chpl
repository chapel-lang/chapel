class C {
  var x: int;
  proc init() {
    this.x = 45;
  }
}

class D: C {
  var y: int;
  override proc init() {
    this.y = 33;
  }
}

var myC = new C();
var myD = new D();
writeln((myC, myD));
