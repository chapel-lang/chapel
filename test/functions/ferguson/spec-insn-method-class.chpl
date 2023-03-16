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
var ownA = new owned C(1.0);
var a = ownA.borrow();
a.foo();

var ownB = new owned C(2);
var b = ownB.borrow();
b.foo();

writeln("static Child");

var ownC = new owned Child(x=1.0, y=100);
var c = ownC.borrow();
c.foo();

var ownD = new owned Child(x=2, y=100);
var d = ownD.borrow();
d.foo();


writeln("static C dynamic Child");
var ownE = new owned Child(x=1.0, y=100);
var e:borrowed C(real) = ownE.borrow();
e.foo();

var ownF = new owned Child(x=2, y=100);
var f:borrowed C(int) = ownF.borrow();
f.foo();
