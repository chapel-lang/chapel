class C {
  type t;
  param p: int = 1;
  var x: t;
}

var c: borrowed C(int);

c = new borrowed C(int);

writeln("{p = ", c.p, ", x = ", c.x, "}");
