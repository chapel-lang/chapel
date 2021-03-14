require "cArrayInRec.h";

extern record myBuffer {
  var foo : c_ptr(c_char);
}

record wrapper {
  var foo : myBuffer;
}

proc main() {
  var x : wrapper;
  for i in 0..9 do x.foo.foo[i] = 65:c_char;
  writeln(x.foo.foo[0]);
}
