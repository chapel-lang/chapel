class C {
  var x;
}

class Child : C {
  var y:int;
}

proc C.foo() {
  writeln("in generic C.foo");
}

proc (C(int)).foo() {
  writeln("in specific C(int).foo");
}

proc Child.foo() {
  writeln("in generic Child.foo");
}

proc (Child(int)).foo() {
  writeln("in specific Child(int).foo");
}

writeln("static C");
var a = new C(1.0);
a.foo();

var b = new C(2);
b.foo();

writeln("static Child");

var c = new Child(x=1.0, y=100);
c.foo();

var d = new Child(x=2, y=100);
d.foo();


writeln("static C dynamic Child");
var e:C(real) = new Child(x=1.0, y=100);
e.foo();

var f:C(int) = new Child(x=2, y=100);
f.foo();


delete a;
delete b;
delete c;
delete d;
delete e;
delete f;
