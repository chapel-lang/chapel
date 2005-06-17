record mytuple {
  var field1 : integer;
  var field2 : float;
}

function foo(parameter i : integer, t : mytuple) where i == 1 {
  return 1;
}

function foo(parameter i : integer, t : mytuple) where i == 2 {
  return 2;
}

var t : mytuple;
writeln(t);

writeln(foo(1, t));
writeln(foo(2, t));
