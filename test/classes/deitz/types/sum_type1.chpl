class C {
  var x : int = 1;
}

class D {
  var y : real = 2.0;
}

proc foo(c : borrowed C) {
  writeln(c.x);
}

proc foo(d : borrowed D) {
  writeln(d.y);
}

proc bar(e) {
  foo(e);
}

var ownC = new owned C(), ownD = new owned D(); 
var c : borrowed C = ownC.borrow(), d : borrowed D = ownD.borrow();

foo(c);
foo(d);

bar(c);
bar(d);
