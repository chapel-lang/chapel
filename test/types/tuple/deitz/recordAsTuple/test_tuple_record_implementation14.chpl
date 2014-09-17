record mytuple {
  var field1 : int;
  var field2 : real;
  proc this(param i : int) ref where i == 1 {
    return field1;
  }
  proc this(param i : int) ref where i == 2 {
    return field2;
  }
}

var t = new mytuple(12, 14.0);

writeln(t);

writeln(t(1));
writeln(t(2));

t(1) = 13;
t(2) = 15.0;

writeln(t);
