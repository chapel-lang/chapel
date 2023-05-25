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

var oc = new owned C();
c = oc.borrow();
foo(c!);

var od = new owned D();
c = od.borrow();
foo(c!);
