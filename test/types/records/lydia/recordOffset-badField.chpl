use CPtr;
// Checks behavior when string passed not actually a name of a field
// Note: c_offsetof is intended for use in C interoperability only.
record Test {
  var x : int(8);
  var y : int;
  var z : real;

}

writeln(c_offsetof(Test,"foo"));
