use CPtr;
// Tests when record contains another record
// Note: c_offsetof is intended for use in C interoperability only.
record Outer {
  var x: Inner;
  var y: int;
}

record Inner {
  var a: real;
  var b: int;
  var c: real;
}

writeln(c_offsetof(Outer, "x"));
writeln(c_offsetof(Outer, "y"));
