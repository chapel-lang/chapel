class C {
  var x;
}

class Child : C {
  var y:int;
}

proc C.foo() {
  writeln("in generic C.foo");
}

proc (borrowed C(int)).foo() {
  writeln("in specific C(int).foo");
}

override proc Child.foo() {
  writeln("in generic Child.foo");
}

override proc (borrowed Child(int)).foo() {
  writeln("in specific Child(int).foo");
}

writeln("static C");
var a = new borrowed C(1.0);
a.foo();

var b = new borrowed C(2);
b.foo();

writeln("static Child");

var c = new borrowed Child(x=1.0, y=100);
c.foo();

var d = new borrowed Child(x=2, y=100);
d.foo();


writeln("static C dynamic Child");
var e:borrowed C(real) = new borrowed Child(x=1.0, y=100);
e.foo();

var f:borrowed C(int) = new borrowed Child(x=2, y=100);
f.foo();
