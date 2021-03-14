class C { }
class D: C { }

var c = new C();
var d = new D();

proc f(a: bool) ref : C {
  if a then
    return c;
  else
    return d;
}

f(true) = new C();
f(false) = new D();
