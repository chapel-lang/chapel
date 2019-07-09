class C {
  var x : int = 1;
  proc print() {
    writeln(this);
  }
}

class D : C {
  var y : real = 2.0;
  override proc print() {
    writeln(this);
  }
}

proc foo(c : borrowed C) {
  c.print();
}

var c : borrowed C?;

c = new borrowed C();
foo(c!);
c = new borrowed D();
foo(c!);
