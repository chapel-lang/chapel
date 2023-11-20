// Checks basic unstable declarations of classes
@unstable class X {
  // Also fields, methods, and type methods
  @unstable var a: int;
  @unstable proc foo() { }
  @unstable proc type bar() {}
}

@unstable(reason="Y is unstable, use Z instead") class Y {
  // Also fields, methods, and type methods
  @unstable(reason="field a is unstable") var a: int;
  @unstable(reason="method foo is unstable") proc foo() { }
  @unstable(reason="type method bar is unstable") proc type bar() {}
}

class Z {
  // Checks unstable fields, methods and type methods in an ununstable class
  @unstable var a: int;
  @unstable(reason="method foo is unstable") proc foo() { }
  @unstable proc type bar() { }
}

// Checks interaction with documentation
/* There was documentation of this symbol */
@unstable class A {
  // Also fields, methods, and type methods

  /* There was documentation of this field */
  @unstable var a: int;
  /* There was documentation of this method */
  @unstable proc foo() { }
  /* There was documentation of this type method */
  @unstable proc type bar() {}
}

/* This symbol also was documented */
@unstable(reason="B is unstable, use C instead") class B {
  // Also fields, methods, and type methods

  /* There was documentation of this field */
  @unstable(reason="field a is unstable") var a: int;
  /* There was documentation of this method */
  @unstable(reason="method foo is unstable") proc foo() { }
  /* There was documentation of this type method */
  @unstable(reason="type method bar is unstable") proc type bar() {}
}

class C {
  // Checks unstable fields, methods and type methods in an ununstable class
  // when the unstable symbols are also documented

  /* There was documentation of this field */
  @unstable var a: int;
  /* There was documentation of this method */
  @unstable(reason="method foo is unstable") proc foo() { }
  /* There was documentation of this type method */
  @unstable proc type bar() { }
}

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is unstable */
@unstable class D {
  // Also fields, methods, and type methods

  /* This symbol is unstable */
  @unstable var a: int;
  /* This symbol is unstable */
  @unstable proc foo() { }
  /* This symbol is unstable */
  @unstable proc type bar() {}
}

/* This symbol is also unstable, please use F instead */
@unstable(reason="E is unstable, use F instead") class E {
  // Also fields, methods, and type methods

  /* This symbol is also unstable, unfortunately */
  @unstable(reason="field a is unstable") var a: int;
  /* This symbol is also unstable, unfortunately */
  @unstable(reason="method foo is unstable") proc foo() { }
  /* This symbol is also unstable, unfortunately */
  @unstable(reason="type method bar is unstable") proc type bar() {}
}

class F {
  // Checks unstable fields, methods and type methods in an ununstable class
  // when the unstable symbols are also documented and the documentation
  // mentions deprecation

  /* This symbol is also unstable, unfortunately */
  @unstable var a: int;
  /* This symbol is unstable */
  @unstable(reason="method foo is unstable") proc foo() { }
  /* This symbol is unstable */
  @unstable proc type bar() { }
}

// Ensures deprecation doesn't cause "no doc" symbols to turn up in
// documentation
@chpldoc.nodoc
@unstable class G {
  // Also fields, methods, and type methods
  @chpldoc.nodoc
  @unstable var a: int;
  @chpldoc.nodoc
  @unstable proc foo() { }
  @chpldoc.nodoc
  @unstable proc type bar() {}
}

@chpldoc.nodoc
@unstable(reason="H is unstable, use I instead") class H {
  // Also fields, methods, and type methods
  @chpldoc.nodoc
  @unstable(reason="field a is unstable") var a: int;
  @chpldoc.nodoc
  @unstable(reason="method foo is unstable") proc foo() { }
  @chpldoc.nodoc
  @unstable(reason="type method bar is unstable") proc type bar() {}
}

class I {
  // Checks unstable fields, methods and type methods in an ununstable class
  @chpldoc.nodoc
  @unstable var a: int;
  @chpldoc.nodoc
  @unstable(reason="method foo is unstable") proc foo() { }
  @chpldoc.nodoc
  @unstable proc type bar() { }
}
