class C {
  type t = int;
  var x: t;
}

var ownC1 = new owned C(int);
var c1 = ownC1.borrow();
var ownC2 = new owned C(real);
var c2 = ownC2.borrow();

proc foo(c: borrowed C(?)) {
  writeln(c);
}

foo(c1);
foo(c2);

proc bar(x: ?) {
  writeln(x);
}

bar(c1);
bar(c2);
bar(3.14);
