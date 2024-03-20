proc foo() {}

var a = 10;

record R {
  proc myMethod() {}
}
proc R.myMethod2() {}

class C {
  var a, b = 10;

  const c: real;

  iter these() {
    yield a; yield b;
  }
}
operator C.+(a: C, b: C) {}

union U {
  var a: int;
  var b: real;
}

enum E {
  A, B, C
}

interface myHashable {
  proc Self.myHash(): uint;
}

private proc myPrivate() {}

module M {
  // just retest a few common cases in an explicit module
  proc foo() {}
  record R {}
  var a: int;
  enum E { A }
}

proc chpl_foo() {}
pragma "chpldoc ignore chpl prefix"
proc chpl_bar() {}
