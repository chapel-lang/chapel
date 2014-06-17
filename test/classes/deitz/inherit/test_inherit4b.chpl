class C {
  var x : int = 1;
  proc print() {
    writeln(this);
  }
}

class D : C {
  var y : real = 2.0;
  proc print() {
    writeln(this);
  }
}

proc foo(c : C) {
  c.print();
}

var c : C;

c = new C();
foo(c);
delete c;
c = new D();
foo(c);
delete c;
