use CPtr;
// Note: c_offsetof is intended for use in C interoperability only.
record Test {
  var x : int(8);
  var y : int;
  var z : real;

}

writeln(c_offsetof(Test,"x"));
writeln(c_offsetof(Test,"y")); // Should this actually be 8?
writeln(c_offsetof(Test,"z"));
