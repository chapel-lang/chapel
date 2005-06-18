record mytuple {
  var field1 : integer;
  var field2 : float;
  function this(parameter i : integer) var where i == 1 {
    return field1;
  }
  function this(parameter i : integer) var where i == 2 {
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
