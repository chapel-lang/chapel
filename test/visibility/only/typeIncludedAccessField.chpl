use typeToExcept only foo;

// This test verifies the behavior when specifying a type in your 'only' list,
// and then accessing a field on that type
proc main() {
  var a = new foo(2);

  writeln(a.fieldA);

  delete a;

  // Listing a type in the 'only' list should allow normal access to its
  // methods and fields as well.
}
