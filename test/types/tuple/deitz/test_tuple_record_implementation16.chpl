record mytuple {
  type t1;
  type t2;
  var f1 : t1;
  var f2 : t2;
  def this(param i : int) var where i == 1 {
    return f1;
  }
  def this(param i : int) var where i == 2 {
    return f2;
  }
}

def fwrite(f : file, val : mytuple) {
  fwrite(f, "(", val.f1, ", ", val.f2, ")");
}

var t : mytuple(int, float);

t(1) = 12;
t(2) = 14.0;

writeln(t);
