class C {
  var x: int;
}

class D {
  var y: C;
}

def foo(c: C) {
  writeln("x is: ", c.x);
}

def foo(d: D) {
  foo(d.y);
}

def main() {
  var myC = new C(x=1);
  foo(myC);
  var myD = new D(new C(2));
  foo(myD);
}
