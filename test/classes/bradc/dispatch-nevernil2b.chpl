class C {
  var x: int;
}

class D {
  var y: C;
}

fun foo(c: C) {
  writeln("x is: ", c.x);
}

fun foo(d: D) {
  foo(d.y);
}

fun main() {
  var myC = C(x=1);
  foo(myC);
  var myD = D(y = C());
  myD.y.x = 2;
  foo(myD);
}
