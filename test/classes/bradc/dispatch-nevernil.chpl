class C {
  var x: int;
}

class D {
  var y: C = C();
}

def foo(c: C) {
  writeln("x is: ", c.x);
}

def foo(d: D) {
  foo(d.y);
}

def main() {
  var myC = C(x=1);
  foo(myC);
  var myD = D();
  myD.y = C();
  myD.y.x = 2;
  foo(myD);
}
