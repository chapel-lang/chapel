class C {
  var x: int;
}

class D: C {
  var y: int;
}

proc f(b: bool) {
  if b then
    return new C();
  else
    return new D();
}

var t1 = f(false);
var t2 = f(true);

writeln(t1, t2);

delete t2;
delete t1;

