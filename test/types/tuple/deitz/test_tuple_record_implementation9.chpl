record mytuple {
  var field1 : integer;
  var field2 : float;
}

function foo(parameter i : integer, t : mytuple) where i == 1 {
  return t.field1;
}

function foo(parameter i : integer, t : mytuple) where i == 2 {
  return t.field2;
}

var t : mytuple(12, 13.4);
writeln(t);

writeln(foo(1, t));
writeln(foo(2, t));
