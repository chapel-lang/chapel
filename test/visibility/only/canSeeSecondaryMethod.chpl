use secondaryMethod only Foo;

// Verifies that methods defined outside of the type are also recognized by
// the only

var a = new Foo(7);
writeln(a.innerMethod(3)); // Should be 21
writeln(a.outerMethod(1)); // Should be 6
