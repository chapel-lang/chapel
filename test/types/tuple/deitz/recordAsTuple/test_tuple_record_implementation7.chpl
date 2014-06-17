record mytuple {
  var field1 : int;
  var field2 : real;
}

proc foo(param i : int, t : mytuple) where i == 1 {
  return 1;
}

proc foo(param i : int, t : mytuple) where i == 2 {
  return 2;
}

var t : mytuple;
writeln(t);

writeln(foo(1, t));
writeln(foo(2, t));
