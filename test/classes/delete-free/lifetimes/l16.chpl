pragma "safe"
module l16 {

use SharedObject;

class MyClass {
  var x:int;
}

record R {
  var c:Shared(MyClass);
}

proc f(c:Shared(MyClass)) {
  var r = new R(c);
  return r;
}

proc g(c:Shared(MyClass)) {
  return new R(c);
}

proc h() {
  return new R(new Shared(new MyClass(10)));
}

proc i() {
  var r = new R(new Shared(new MyClass(10)));
  return r;
}

proc j() {
  return h();
}

proc k() {
  var r = h();
  return r;
}

proc l(c:Shared(MyClass)) {
  var r = f(c);
  return r;
}

proc m(c:Shared(MyClass)) {
  return f(c);
}

proc test() {
  var shared = new Shared(new MyClass(1));

  var v1 = f(shared);
  var v2 = g(shared);
  var v3 = h();
  var v4 = i();
  var v5 = j();
  var v6 = k();
  var v7 = l(shared);
  var v8 = m(shared);

  writeln(v1.c.borrow());
  writeln(v2.c.borrow());
  writeln(v3.c.borrow());
  writeln(v4.c.borrow());
  writeln(v5.c.borrow());
  writeln(v6.c.borrow());
  writeln(v7.c.borrow());
  writeln(v8.c.borrow());
}

test();
}
