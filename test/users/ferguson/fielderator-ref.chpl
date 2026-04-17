use Reflection;

record R {
  var x: int;
  var y: string;
  var z: real;

  proc something() {
    return z;
  }
}

class C {
  var x: int;
  var y: string;
  var z: real;

  proc something() {
    return z;
  }
}


var rec = new R(3, "hi", 17.23);
var ownCls = new owned C(3, "hi", 17.23);
var cls = ownCls.borrow();
const epsilon = 1e-8; // for comparing reals.

myproc(R, rec);
myproc(borrowed C, cls);



proc myproc(type t, ref m) {
  assert(getNumFields(t) == 3);
  assert(getFieldName(t, 0) == "x");
  assert(getFieldName(t, 1) == "y");
  assert(getFieldName(t, 2) == "z");
  assert(getField(m, 0) == 3);
  assert(getField(m, 1) == "hi");
  assert(realEqual(getField(m, 2), 17.23));
  for param i in 0..<getNumFields(t) {
    writeln(getFieldName(t, i));
    writeln(getField(m, i));
  }
}

proc realEqual(a: real, b: real): bool {
  var diff = abs(a - b);
  return diff < epsilon;
}
