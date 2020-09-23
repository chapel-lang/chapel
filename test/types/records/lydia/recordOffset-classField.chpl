use CPtr;
// Tests having a class field
// Note: c_offsetof is intended for use in C interoperability only.
record Outer {
  var x : borrowed Inner;
  var y : int;
}

class Inner {
  var a: real;
  var b: int;
  var c: real;
}

writeln(c_offsetof(Outer,"x"));
writeln(c_offsetof(Outer,"y"));
