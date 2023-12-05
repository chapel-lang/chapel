@chpldoc.nodoc
proc foo() {}

@chpldoc.nodoc
var a = 10;

@chpldoc.nodoc
record R {
  proc myMethod() {}
}
@chpldoc.nodoc
proc R.myMethod2() {}

@chpldoc.nodoc
class C {
  var a, b = 10;

  const c: real;

  iter these() {
    yield a; yield b;
  }
}
@chpldoc.nodoc
operator C.+(a: C, b: C) {}

@chpldoc.nodoc
union U {
  var a: int;
  var b: real;
}

@chpldoc.nodoc
enum E {
  A,
  B,
  C
}

// @chpldoc.nodoc
// interface myHashable {
//   proc Self.myHash(): uint;
// }

private proc myPrivate() {}

@chpldoc.nodoc
module M {
  // just retest a few common cases in an explicit module
  proc foo() {}
  record R {}
  var a: int;
  enum E { A }
}

proc chpl_foo() {}
pragma "chpldoc ignore chpl prefix"
@chpldoc.nodoc
proc chpl_bar() {}
