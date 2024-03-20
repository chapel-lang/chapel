@deprecated
proc foo() {}

@deprecated
var a = 10;

@deprecated
record R {
  proc myMethod() {}
}
@deprecated
proc R.myMethod2() {}

@deprecated
class C {
  var a, b = 10;

  const c: real;

  iter these() {
    yield a; yield b;
  }
}
@deprecated
operator C.+(a: C, b: C) {}

@deprecated
union U {
  var a: int;
  var b: real;
}

@deprecated
enum E {
  A,
  B,
  C
}

// @deprecated
// interface myHashable {
//   proc Self.myHash(): uint;
// }

private proc myPrivate() {}

@deprecated
module M {
  // just retest a few common cases in an explicit module
  proc foo() {}
  record R {}
  var a: int;
  enum E { A }
}

proc chpl_foo() {}
pragma "chpldoc ignore chpl prefix"
@deprecated
proc chpl_bar() {}
