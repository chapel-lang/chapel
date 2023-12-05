use CTypes;

writeln("Printing before modification");

writeln("Hello World!");
writeln(string.createBorrowingBuffer("Hello World!".c_str()));

writeln(b"Hello World!");
writeln(string.createBorrowingBuffer(b"Hello World!".c_str()));

// Now modify a string literal
// this is not an OK thing for code to do in general
// but we do it in this test to make sure that if strings end
// up in the read-only data section again, issues #18284 and #18287
// are suitably resolved in a different way.

// Programs that modify string literals like this should expect
// undefined behavior.
writeln("Modifying");

{
  var s = "Hello World!";
  var cptr = c_ptrTo(s);
  cptr[0] = 66; // 'B'
  writeln("Hello World!");
  writeln(s);
  writeln(string.createBorrowingBuffer(c_ptrTo(s)));
}

{
  var s = b"Hello World!";
  var cptr = c_ptrTo(s);
  cptr[0] = 66; // 'B'
  writeln(b"Hello World!");
  writeln(s);
  writeln(string.createBorrowingBuffer(c_ptrTo(s)));
}
