
// Eventually the compiler should attempt to resolve a copy-initializer for
// "R(int)". If the compiler attempts to use the default-initializer, the
// compiler will emit an error because 'foobar(R(int))' cannot be resolved.
//
// The compiler should not attempt to use a default-initializer when resolving
// a call that looks like a copy-init.

proc foobar(foo : int) {
  return 0;
}

record R {
  var foo;

  var something = foobar(foo);
}

var r : R(int);
