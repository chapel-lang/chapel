use CTypes;

writeln("Printing before modification");

writeln("Hello World!");
writeln("Hello World!".c_str():string);

writeln(b"Hello World!");
writeln(b"Hello World!".c_str():string);

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
  var cptr = s.c_str():c_void_ptr:c_ptr(uint(8));
  cptr[0] = 66; // 'B'
  writeln("Hello World!");
  writeln(s);
  writeln(s.c_str():string);
}

{
  var s = b"Hello World!";
  var cptr = s.c_str():c_void_ptr:c_ptr(uint(8));
  cptr[0] = 66; // 'B'
  writeln(b"Hello World!");
  writeln(s);
  writeln(s.c_str():string);
}
