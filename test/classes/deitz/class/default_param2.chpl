class C {
  type t;
  param p: int = 1;
  var x: t;
}

var c = new C(int);

writeln("{p = ", c.p, ", x = ", c.x, "}");
