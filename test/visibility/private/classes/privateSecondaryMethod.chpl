class C {
  var foo: int;
}

// Because this method is defined at the module level, should it be
// visible at the module level, even though it is private?
// At the very least, it shouldn't be visible outside of this module.
private proc C.getFoo() {
  return foo;
}

var c = new C(15);
writeln(c.getFoo());
