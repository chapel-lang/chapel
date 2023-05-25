// Given a formal argument with a default value that is calculated
// using by calling a function and has a runtime type,
// here fn() returning an array,
// in what cases should the function be invoked?

proc fn(size) {
  writeln("in fn()");
  var A: [1..size] int;
  return A;
}

proc DA(arg = fn(5)) {
  writeln("in DA()");
}

DA();  // fn() is invoked just once

var B: [1..6] int;

DA(B); // fn() is not invoked; should it be?

proc nonD(arg: [1..7] int) {
  writeln("in nonD()");
}
nonD(B); // halt "Domain mismatch passing array argument"

writeln("done");
