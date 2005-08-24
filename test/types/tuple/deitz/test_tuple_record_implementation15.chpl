record mytuple {
  var field1;
  var field2;
  function this(param i : integer) var where i == 1 {
    return field1;
  }
  function this(param i : integer) var where i == 2 {
    return field2;
  }
}

var t : mytuple(12, 14.0);

writeln(t);

writeln(t(1));
writeln(t(2));

t(1) = 13;
t(2) = 15.0;

writeln(t);

var t2 : mytuple(12.0, "hello world");

writeln(t2);

writeln(t2(1));
writeln(t2(2));

t2(1) = 13.0;
t2(2) = "good bye";

writeln(t2);
