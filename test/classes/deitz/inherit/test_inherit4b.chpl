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

c = (new owned C()).borrow();
foo(c!);
c = (new owned D()).borrow();
foo(c!);
