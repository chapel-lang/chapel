class trio {
  type elt_type;

  var x1 : elt_type;
  var x2 : elt_type;
  var x3 : elt_type;

  proc sum() do
    return x1 + x2 + x3;
}

override proc trio.writeThis(f) throws {
  f.write("(", x1, " & ", x2, " & ", x3, ")");
}

var ownT1 = new owned trio(int, 1, 2, 3);
var t1 : borrowed trio(int)  = ownT1.borrow();
var ownT2 = new owned trio(real, x1=1.0);
var t2 : borrowed trio(real) = ownT2.borrow();

writeln(t1);
writeln(t1.sum());

writeln(t2);
writeln(t2.sum());
