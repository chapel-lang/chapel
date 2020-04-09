class C {
  var x: int;
  proc foo() {
    writeln("C: ", this);
  }
}

class D: C {
  var y: int;
  override proc foo() {
    writeln("D: ", this);
  }
}

var d: borrowed D = new borrowed D();

(d:borrowed C).foo();
