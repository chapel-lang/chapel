use typeToExcept except foo;

// This test verifies that an except on a type will cause a failure in code
// that uses that type.
var a: foo = new foo();
writeln(a);
