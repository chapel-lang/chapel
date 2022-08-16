// Given a formal argument whose type is via calling a function
// returning an array value, should this array's bounds be checked
// against the actual argument?

proc fn(size) {
  writeln("in fn()");
  var A: [1..size] int;
  return A;
}

proc nonD(arg: fn(7).type) {
  writeln("in nonD()");
}

var B: [1..6] int;

nonD(B); // should this halt with "Domain mismatch passing array argument" ?

writeln("done");
