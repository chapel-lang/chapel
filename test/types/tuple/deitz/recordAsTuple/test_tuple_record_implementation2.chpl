record mytuple {
  var field1 : int;
  var field2 : real;
}

proc foo(t : mytuple, param i : int) where i == 1 {
  return t.field1;
}

proc foo(t : mytuple, param i : int) where i == 2 {
  return t.field2;
}

var t : mytuple;

writeln(t);

t.field1 = 12;
t.field2 = 14.0;

writeln(t);

writeln(foo(t, 1));
writeln(foo(t, 2));
