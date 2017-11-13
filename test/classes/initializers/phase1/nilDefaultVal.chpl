class A {
  var k: int;

  proc init(k=5) {
    this.k = k;
    super.init();
  }
}

class B {
  var a: A;

  proc init(a = nil) {
    this.a = a;
    super.init();
  }
}

proc main() {
  const a1: A = new A();

  writeln(a1);
  const b = new B(a=a1);
  writeln(b);

  delete b;
  delete a1;
}
