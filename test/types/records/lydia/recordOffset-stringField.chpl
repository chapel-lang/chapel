// Tests having a string field
// Note: c_offsetof is intended for use in C interoperability only.
record Test {
  var x : string;
  var y : int;

}

writeln(c_offsetof(Test,"x"));
writeln(c_offsetof(Test,"y")); // 32?  Because string is implemented as a record
