proc g(inout arg: [1..3] int) {
  writeln("in g, arg= ", arg);
  arg = 2;
}

{
  var A:[1..10] int;
  ref slice = A[1..3];
  g(slice); // error: value from coercion passed to ref formal 'arg'
  writeln(slice);
}
