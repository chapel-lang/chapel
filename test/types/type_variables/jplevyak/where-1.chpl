
class C {
  type bar;
  var x : bar;
}


fun f(x : ?t) where x.bar == int {
  writeln("x = ", x.x);
}

var c = C(bar=int, x=1);

f(c);
