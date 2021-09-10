// Checks basic deprecated declarations of classes
deprecated class X {
  // Also fields, methods, and type methods
  deprecated var a: int;
  deprecated proc foo() { }
  deprecated proc type bar() {}
}

deprecated "Y is deprecated, use Z instead" class Y {
  // Also fields, methods, and type methods
  deprecated "field a is deprecated" var a: int;
  deprecated "method foo is deprecated" proc foo() { }
  deprecated "type method bar is deprecated" proc type bar() {}
}

class Z {
  // Checks deprecated fields, methods and type methods in an undeprecated class
  deprecated var a: int;
  deprecated "method foo is deprecated" proc foo() { }
  deprecated proc type bar() { }
}

// Checks interaction with documentation
/* There was documentation of this symbol */
deprecated class A {
  // Also fields, methods, and type methods

  /* There was documentation of this field */
  deprecated var a: int;
  /* There was documentation of this method */
  deprecated proc foo() { }
  /* There was documentation of this type method */
  deprecated proc type bar() {}
}

/* This symbol also was documented */
deprecated "B is deprecated, use C instead" class B {
  // Also fields, methods, and type methods

  /* There was documentation of this field */
  deprecated "field a is deprecated" var a: int;
  /* There was documentation of this method */
  deprecated "method foo is deprecated" proc foo() { }
  /* There was documentation of this type method */
  deprecated "type method bar is deprecated" proc type bar() {}
}

class C {
  // Checks deprecated fields, methods and type methods in an undeprecated class
  // when the deprecated symbols are also documented

  /* There was documentation of this field */
  deprecated var a: int;
  /* There was documentation of this method */
  deprecated "method foo is deprecated" proc foo() { }
  /* There was documentation of this type method */
  deprecated proc type bar() { }
}

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is deprecated */
deprecated class D {
  // Also fields, methods, and type methods

  /* This symbol is deprecated */
  deprecated var a: int;
  /* This symbol is deprecated */
  deprecated proc foo() { }
  /* This symbol is deprecated */
  deprecated proc type bar() {}
}

/* This symbol is also deprecated, please use F instead */
deprecated "E is deprecated, use F instead" class E {
  // Also fields, methods, and type methods

  /* This symbol is also deprecated, unfortunately */
  deprecated "field a is deprecated" var a: int;
  /* This symbol is also deprecated, unfortunately */
  deprecated "method foo is deprecated" proc foo() { }
  /* This symbol is also deprecated, unfortunately */
  deprecated "type method bar is deprecated" proc type bar() {}
}

class F {
  // Checks deprecated fields, methods and type methods in an undeprecated class
  // when the deprecated symbols are also documented and the documentation
  // mentions deprecation

  /* This symbol is also deprecated, unfortunately */
  deprecated var a: int;
  /* This symbol is deprecated */
  deprecated "method foo is deprecated" proc foo() { }
  /* This symbol is deprecated */
  deprecated proc type bar() { }
}
