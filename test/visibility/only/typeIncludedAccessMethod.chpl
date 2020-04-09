use typeToExcept only foo;

// This test verifies the behavior when specifying a type in your 'only' list,
// and then calling a method on that type
proc main() {
  var a = new borrowed foo(2);

  writeln(a.someMethod(3));



  // Listing a type in the 'only' list should allow normal access to its
  // methods and fields as well.
}
