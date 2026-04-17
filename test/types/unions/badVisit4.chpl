
union U {
  var x: int;
  var y: real;
}
var u: U;
u.visit(proc(x: int) { writeln("int: ", x); },
        proc(x: real) { writeln("real: ", x); });
