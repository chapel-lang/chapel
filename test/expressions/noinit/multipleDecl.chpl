// This test verifies that using noinit on a multiple variable declaration does
// not cause errors.  However, it does not check that these variables are
// uninitialized.  After brief experimentation, a method similar to stackVerify
// will not feasibly demonstrate what I am looking for.
var foo, bar, baz: int = noinit;
foo = 3;
bar = 2;
baz = 5;
writeln(foo + bar + baz);
