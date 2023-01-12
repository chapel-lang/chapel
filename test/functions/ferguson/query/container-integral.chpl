// from issue #11000

class C {
  type elementType;
  type indexType;
  type containerType;
}
class Container {
  type containedType;
}
proc f(c: C(real,?t,?u)) {
  writeln("in f(", c.type:string, ")");
}
proc g(c: C(?t,?u,Container(integral))) {
  writeln("in g(", c.type:string, ")");
}
var cc = new Container(int);
var b = cc.borrow();
var c = new C(real, int, b.type);
f(c);
g(c);
