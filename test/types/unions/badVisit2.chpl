
union U {
  var x: int;
  var y: real;
}
var u: U;
u.x = 10;
u.visit(proc(x: real) { writeln("real: ", x); });
