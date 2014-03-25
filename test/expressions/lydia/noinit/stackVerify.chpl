proc thing1() {
  var foo: int(32) = noinit;
  writeln(foo);
}

proc thing2() {
  var bar: int(32) = 84;
  writeln(bar);
}

// This is a very hacky test.  Basically, it relies on linux stack frames.
// The first value for each method will be placed in the same location in
// memory, so a noinit value will "take on" the value of whatever variable was
// stored there previously.
thing1(); // Starts at 0, or gobbledygook
thing2(); // Sets the stack location to 84
thing1(); // Whoops!  The value of foo is 84
