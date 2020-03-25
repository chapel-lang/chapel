record mytuple {
  type t1;
  type t2;
  var f1 : t1;
  var f2 : t2;
  proc this(param i : int) ref where i == 1 {
    return f1;
  }
  proc this(param i : int) ref where i == 2 {
    return f2;
  }
}

proc mytuple.writeThis(f) throws {
  f.write("(", f1, ", ", f2, ")");
}

var t : mytuple(int, real);

t(1) = 12;
t(2) = 14.0;

writeln(t);
