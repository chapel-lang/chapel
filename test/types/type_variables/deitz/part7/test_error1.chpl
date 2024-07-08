class C {
  type t;
  var x : t;
}
var cObj = new C(int);
var c = cObj.borrow();

writeln(c);

proc foo(i : int) {
  writeln(i);
}

foo(c);
