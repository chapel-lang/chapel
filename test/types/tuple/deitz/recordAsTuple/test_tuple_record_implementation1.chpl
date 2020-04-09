record mytuple {
  var field1 : int;
  var field2 : real;
  proc foo(param i : int) where i == 1 {
    return field1;
  }
  proc foo(param i : int) where i == 2 {
    return field2;
  }
}

var t : mytuple;

writeln(t);

t.field1 = 12;
t.field2 = 14.0;

writeln(t);

writeln(t.foo(1));
writeln(t.foo(2));
