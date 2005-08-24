record mytuple {
  var field1 : integer;
  var field2 : float;
}

function foo(param i : integer, t : mytuple) {
  return 1;
}

var t : mytuple;
writeln(t);

writeln(foo(1, t));
writeln(foo(2, t));
