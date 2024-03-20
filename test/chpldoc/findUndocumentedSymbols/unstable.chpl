@unstable
proc foo() {}

@unstable
var a = 10;

@unstable
record R {
  proc myMethod() {}
}
@unstable
proc R.myMethod2() {}

@unstable
class C {
  var a, b = 10;

  const c: real;

  iter these() {
    yield a; yield b;
  }
}
@unstable
operator C.+(a: C, b: C) {}

@unstable
union U {
  var a: int;
  var b: real;
}

@unstable
enum E {
  A,
  B,
  C
}

// @unstable
// interface myHashable {
//   proc Self.myHash(): uint;
// }

private proc myPrivate() {}

@unstable
module M {
  // just retest a few common cases in an explicit module
  proc foo() {}
  record R {}
  var a: int;
  enum E { A }
}

proc chpl_foo() {}
pragma "chpldoc ignore chpl prefix"
@unstable
proc chpl_bar() {}
