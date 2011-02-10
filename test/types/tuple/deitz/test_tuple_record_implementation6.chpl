record mytuple {
  var field1 : int;
  var field2 : real;
}

proc foo(param i : int, t : mytuple) {
  return 1;
}

var t : mytuple;
writeln(t);

writeln(foo(1, t));
writeln(foo(2, t));
