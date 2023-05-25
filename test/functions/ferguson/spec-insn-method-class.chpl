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
var a = (new owned C(1.0)).borrow();
a.foo();

var b = (new owned C(2)).borrow();
b.foo();

writeln("static Child");

var c = (new owned Child(x=1.0, y=100)).borrow();
c.foo();

var d = (new owned Child(x=2, y=100)).borrow();
d.foo();


writeln("static C dynamic Child");
var e:borrowed C(real) = (new owned Child(x=1.0, y=100)).borrow();
e.foo();

var f:borrowed C(int) = (new owned Child(x=2, y=100)).borrow();
f.foo();
