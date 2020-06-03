class trio {
  type elt_type;

  var x1 : elt_type;
  var x2 : elt_type;
  var x3 : elt_type;

  proc sum()
    return x1 + x2 + x3;
}

override proc trio.writeThis(f) throws {
  f.write("(", x1, " & ", x2, " & ", x3, ")");
}

var t1 : borrowed trio(int)  = new borrowed trio(int, 1, 2, 3);
var t2 : borrowed trio(real) = new borrowed trio(real, x1=1.0);

writeln(t1);
writeln(t1.sum());

writeln(t2);
writeln(t2.sum());
