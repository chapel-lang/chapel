class C {
  var x : int = 1;
  fun print() {
    writeln(this);
  }
}

class D : C {
  var y : float = 2.0;
  fun print() {
    writeln(this);
  }
}

fun foo(c : C) {
  c.print();
}

var c : C;

c = C();
foo(c);
c = D();
foo(c);
