class C {
  var x: int;
  proc foo() {
    writeln("C: ", this);
  }
}

class D: C {
  var y: int;
  proc foo() {
    writeln("D: ", this);
  }
}

var d: D = new D();
(d:C).foo();
