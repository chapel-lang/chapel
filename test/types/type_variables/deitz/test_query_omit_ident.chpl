class C {
  type t = int;
  var x: t;
}

var c1 = (new owned C(int)).borrow();
var c2 = (new owned C(real)).borrow();

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
