
class C {
  type bar;
  var x : bar;
}


proc f(x : ?t) where x.bar == int {
  writeln("x = ", x.x);
}

var c = new unmanaged C(bar=int, x=1);

f(c);

delete c;
