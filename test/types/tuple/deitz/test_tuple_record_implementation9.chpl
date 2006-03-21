record mytuple {
  var field1 : int;
  var field2 : float;
}

fun foo(param i : int, t : mytuple) where i == 1 {
  return t.field1;
}

fun foo(param i : int, t : mytuple) where i == 2 {
  return t.field2;
}

var t = mytuple(12, 13.4);
writeln(t);

writeln(foo(1, t));
writeln(foo(2, t));
