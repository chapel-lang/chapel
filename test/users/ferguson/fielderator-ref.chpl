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
var cls = new C(3, "hi", 17.23);
const epsilon = 1e-8; // for comparing reals.

myproc(R, rec);
myproc(C, cls);

proc myproc(type t, ref m) {
  assert(__primitive("num fields", t) == 3);
  assert(__primitive("field num to name", t, 1) == "x");
  assert(__primitive("field num to name", t, 2) == "y");
  assert(__primitive("field num to name", t, 3) == "z");
  assert(__primitive("field value by num", m, 1) == 3);
  assert(__primitive("field value by num", m, 2) == "hi");
  assert(realEqual(__primitive("field value by num", m, 3), 17.23));
  assert(__primitive("field value by name", m, "x") == 3);
  assert(__primitive("field value by name", m, "y") == "hi");
  assert(realEqual(__primitive("field value by name", m, "z"), 17.23));
  for param i in 1..(__primitive("num fields", t)) {
    writeln(__primitive("field num to name", t, i));
    writeln(__primitive("field value by num", m, i));
  }
}

proc realEqual(a: real, b: real): bool {
  var diff = abs(a - b);
  return diff < epsilon;
}
