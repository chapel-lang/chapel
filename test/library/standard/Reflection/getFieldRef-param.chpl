use Reflection;
config param case = 0;

record R {
  param x = 10;
}

class C {
  param x = 10;
}

if case == 0 {
  var r: R;
  writeln(getFieldRef(r, 0));
} else if case == 1 {
  var c = new C();
  writeln(getFieldRef(c, 0));
} else {
  var r: R;
  writeln(getFieldRef(r, "x"));
}
