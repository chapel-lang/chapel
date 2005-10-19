class trio {
  type elt_type;

  var x1 : elt_type;
  var x2 : elt_type;
  var x3 : elt_type;

  function sum()
    return x1 + x2 + x3;
}

function fwrite(f : file, t : trio) {
  fwrite(f, "(", t.x1, " & ", t.x2, " & ", t.x3, ")");
}

var t1 : trio(integer) = trio(integer, 1, 2, 3);
var t2 : trio(float) = trio(float, x1=1.0);

writeln(t1);
writeln(t1.sum());
writeln(t2);
writeln(t2.sum());
