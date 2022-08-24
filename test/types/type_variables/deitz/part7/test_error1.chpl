class C {
  type t;
  var x : t;
}

var c = (new owned C(int)).borrow();

writeln(c);

proc foo(i : int) {
  writeln(i);
}

foo(c);
