
class C {
  type bar;
  var x : bar;
}


function f(x : ?t) where t.bar == integer {
  writeln("x = ", x.x);
}

var c = C(x = 1);

f(c);
