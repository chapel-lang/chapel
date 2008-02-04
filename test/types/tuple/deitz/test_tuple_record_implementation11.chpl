record mytuple {
  var field1 : int;
  var field2 : real;
  def this(param i : int) where i == 1 {
    return field1;
  }
  def this(param i : int) where i == 2 {
    return field2;
  }
}

var t = new mytuple(12, 14.0);

writeln(t);

writeln(t(1));
writeln(t(2));
