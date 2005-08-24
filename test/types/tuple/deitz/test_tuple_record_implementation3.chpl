record mytuple {
  var field1 : integer;
  var field2 : float;
}

function foo(t : mytuple, param i : integer) where i == 1 {
  return 1;
}

function foo(t : mytuple, param i : integer) where i == 2 {
  return 2;
}

var t : mytuple;

writeln(t);

t.field1 = 12;
t.field2 = 14.0;

writeln(t);

writeln(foo(t, 1));
writeln(foo(t, 2));
